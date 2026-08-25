#!/usr/bin/env python3
"""Generate CycloneDX 1.5 from vendored PlatformIO/Arduino lib manifests.

Prefer library.json over library.properties when both exist (matches Preloop
demo inventory). Not build-attested — provenance is manifest/repo-scan.
"""
from __future__ import annotations

import argparse
import json
import re
import uuid
from datetime import datetime, timezone
from pathlib import Path
from urllib.parse import quote


def parse_properties(text: str) -> dict:
    out = {}
    for line in text.splitlines():
        line = line.strip()
        if not line or line.startswith("#") or "=" not in line:
            continue
        k, v = line.split("=", 1)
        out[k.strip()] = v.strip()
    return out


def repo_url_from(data: dict, props: dict) -> str | None:
    repo = data.get("repository") if isinstance(data.get("repository"), dict) else None
    if repo and repo.get("url"):
        return str(repo["url"]).rstrip("/")
    for key in ("url", "repository"):
        if props.get(key) and "github.com" in props[key]:
            return props[key].rstrip("/")
    if data.get("homepage") and "github.com" in str(data["homepage"]):
        return str(data["homepage"]).rstrip("/")
    return None


def license_entries(data: dict, props: dict) -> list:
    lic = data.get("license") or props.get("license")
    if not lic:
        return []
    return [{"license": {"name": str(lic)}}]


def make_purl(name: str, version: str | None, vcs: str | None) -> str:
    ver = version or "unknown"
    base = f"pkg:generic/{quote(name, safe='')}@{quote(ver, safe='')}"
    if vcs:
        vcs_norm = vcs
        if vcs_norm.startswith("https://"):
            vcs_norm = "git+" + vcs_norm
        elif vcs_norm.startswith("git@"):
            vcs_norm = "git+" + vcs_norm
        return f"{base}?vcs_url={quote(vcs_norm, safe=':/+@')}"
    return base


def collect(root: Path) -> list[dict]:
    lib_root = root / "lib"
    by_dir: dict[Path, dict] = {}
    for path in lib_root.rglob("library.json"):
        by_dir.setdefault(path.parent, {})["json"] = path
    for path in lib_root.rglob("library.properties"):
        by_dir.setdefault(path.parent, {})["props"] = path

    components = []
    for folder, files in sorted(by_dir.items(), key=lambda x: str(x[0])):
        data = {}
        props = {}
        evidence = []
        if "json" in files:
            try:
                data = json.loads(files["json"].read_text(encoding="utf-8", errors="replace"))
                evidence.append(str(files["json"].relative_to(root)))
            except json.JSONDecodeError:
                data = {}
        if "props" in files:
            props = parse_properties(files["props"].read_text(encoding="utf-8", errors="replace"))
            evidence.append(str(files["props"].relative_to(root)))

        name = (data.get("name") or props.get("name") or folder.name).strip()
        version = (data.get("version") or props.get("version") or "").strip() or None
        # Prefer json version when both present (Preloop audit used json for JPEGDEC)
        if data.get("version"):
            version = str(data["version"]).strip()
        description = (
            data.get("description")
            or props.get("paragraph")
            or props.get("sentence")
            or ""
        )
        if isinstance(description, str) and len(description) > 240:
            description = description[:237] + "..."

        vcs = repo_url_from(data, props)
        purl = make_purl(name, version, vcs)
        bom_ref = f"{name}@{version or 'unknown'}"
        comp = {
            "type": "library",
            "bom-ref": bom_ref,
            "name": name,
            "purl": purl,
            "properties": [
                {
                    "name": "dd:evidence-source",
                    "value": ";".join(evidence) if evidence else str(folder.relative_to(root)),
                },
                {
                    "name": "dd:provenance",
                    "value": "manifest/repo-scan derived, not build-attested",
                },
            ],
        }
        if version:
            comp["version"] = version
        if description:
            comp["description"] = description
        licenses = license_entries(data, props)
        if licenses:
            comp["licenses"] = licenses
        if vcs:
            comp["externalReferences"] = [
                {"type": "vcs", "url": vcs},
            ]
        # surface version disagreement
        if data.get("version") and props.get("version") and str(data["version"]) != props["version"]:
            comp["properties"].append(
                {
                    "name": "dd:version-conflict",
                    "value": f"library.json={data['version']}; library.properties={props['version']}",
                }
            )
        components.append(comp)

    # de-dupe by bom-ref keeping first
    seen = set()
    unique = []
    for c in components:
        if c["bom-ref"] in seen:
            c["bom-ref"] = c["bom-ref"] + "#" + str(len(seen))
        seen.add(c["bom-ref"])
        unique.append(c)
    return unique


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--root", type=Path, default=Path.cwd())
    ap.add_argument("--out", type=Path, required=True)
    ap.add_argument("--name", default="kettlecompanion-tasmota")
    ap.add_argument("--version", default=None)
    args = ap.parse_args()
    root = args.root.resolve()
    version = args.version
    if not version:
        head = root / ".git" / "HEAD"
        # best-effort: read from git if available via file; else unknown
        version = "unknown"
        try:
            import subprocess

            version = subprocess.check_output(
                ["git", "-C", str(root), "rev-parse", "HEAD"], text=True
            ).strip()
        except Exception:
            pass

    components = collect(root)
    now = datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")
    bom = {
        "bomFormat": "CycloneDX",
        "specVersion": "1.5",
        "serialNumber": f"urn:uuid:{uuid.uuid4()}",
        "version": 1,
        "metadata": {
            "timestamp": now,
            "tools": {
                "components": [
                    {
                        "type": "application",
                        "name": "generate_lib_cyclonedx.py",
                        "version": "0.1.0",
                    }
                ]
            },
            "component": {
                "type": "application",
                "name": args.name,
                "version": version,
                "purl": f"pkg:github/DynamicDevices/Tasmota@{version}",
                "properties": [
                    {
                        "name": "dd:provenance",
                        "value": "manifest/repo-scan derived, not build-attested",
                    }
                ],
            },
        },
        "components": components,
    }
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(bom, indent=2) + "\n", encoding="utf-8")
    missing_ver = sum(1 for c in components if not c.get("version"))
    missing_lic = sum(1 for c in components if not c.get("licenses"))
    print(
        f"Wrote {args.out} components={len(components)} "
        f"missing_version={missing_ver} missing_license={missing_lic}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
