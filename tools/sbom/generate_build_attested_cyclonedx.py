#!/usr/bin/env python3
"""Merge vendored-lib manifests + PlatformIO packages into a build-attested CycloneDX.

Attestation here means: tied to a successful `pio run -e <env>` via recorded
git SHA, env name, firmware artifact hash, and exact platform/tool versions
from `pio pkg list`. Vendored Arduino libs still come from manifests (LDF does
not emit lockfiles); components linked in the build get dd:build-linked=true
when a dependency-graph capture is supplied.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import subprocess
import uuid
from datetime import datetime, timezone
from pathlib import Path
from urllib.parse import quote

from generate_lib_cyclonedx import collect, make_purl  # type: ignore


def sha256_file(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def git_sha(root: Path) -> str:
    try:
        return subprocess.check_output(
            ["git", "-C", str(root), "rev-parse", "HEAD"], text=True
        ).strip()
    except Exception:
        return "unknown"


def parse_pkg_list(text: str) -> list[dict]:
    pkgs = []
    for line in text.splitlines():
        m = re.search(r"(?:├──|└──|Platform)\s+(\S+)\s+@\s+(\S+)", line)
        if not m:
            continue
        name, ver = m.group(1), m.group(2)
        uri = None
        um = re.search(r"required:\s+(\S+)", line)
        if um:
            uri = um.group(1).rstrip(",").rstrip(")")
        pkgs.append({"name": name, "version": ver, "required": uri})
    return pkgs


def run_pkg_list(root: Path, env: str) -> list[dict]:
    out = subprocess.check_output(
        ["pio", "pkg", "list", "-e", env, "-v"],
        cwd=str(root),
        text=True,
    )
    return parse_pkg_list(out)


def parse_ldf_names(text: str) -> set[str]:
    names = set()
    for line in text.splitlines():
        m = re.match(r"\|--\s+(.+?)(?:\s+@\s+(.+))?$", line.strip())
        if m:
            names.add(m.group(1).strip())
    return names


def pio_component(pkg: dict) -> dict:
    name = pkg["name"]
    version = pkg["version"]
    uri = pkg.get("required")
    purl = f"pkg:generic/{quote(name, safe='')}@{quote(version, safe='')}"
    if uri and uri.startswith("http"):
        purl += f"?download_url={quote(uri, safe=':/+@%')}"
    elif uri and "/" in uri and not uri.startswith("http"):
        # platformio/owner-name style
        purl = f"pkg:platformio/{quote(uri, safe='/@')}@{quote(version, safe='')}"
    comp = {
        "type": "library" if name.startswith("framework-") or "lib" in name else "application",
        "bom-ref": f"pio:{name}@{version}",
        "name": name,
        "version": version,
        "purl": purl,
        "properties": [
            {"name": "dd:provenance", "value": "platformio-pkg-list; build-attested"},
            {"name": "dd:package-manager", "value": "platformio"},
        ],
    }
    if uri:
        comp["properties"].append({"name": "dd:pio-required", "value": uri})
        if uri.startswith("http"):
            comp["externalReferences"] = [{"type": "distribution", "url": uri}]
    # platforms/tools better typed
    if name.startswith("toolchain-") or name.startswith("tool-"):
        comp["type"] = "application"
    if name.startswith("espressif") or name.startswith("platform-"):
        comp["type"] = "platform"
    return comp


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--root", type=Path, default=Path.cwd())
    ap.add_argument("--env", required=True)
    ap.add_argument("--out", type=Path, required=True)
    ap.add_argument("--firmware", type=Path, default=None)
    ap.add_argument("--ldf-log", type=Path, default=None, help="pio run log containing Dependency Graph")
    ap.add_argument("--name", default="kettlecompanion-tasmota")
    args = ap.parse_args()
    root = args.root.resolve()
    sha = git_sha(root)
    now = datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")

    pio_pkgs = run_pkg_list(root, args.env)
    lib_comps = collect(root)
    linked = set()
    if args.ldf_log and args.ldf_log.exists():
        linked = parse_ldf_names(args.ldf_log.read_text(encoding="utf-8", errors="replace"))

    for c in lib_comps:
        # mark attestation layer
        for p in c.get("properties") or []:
            if p.get("name") == "dd:provenance":
                p["value"] = (
                    "manifest/repo-scan; included in build-attested BOM "
                    "(library identity from manifests; link status below)"
                )
        props = c.setdefault("properties", [])
        # heuristic link match by name
        linked_flag = c["name"] in linked or any(
            c["name"] in n or n.startswith(c["name"]) for n in linked
        )
        props.append({"name": "dd:build-env", "value": args.env})
        props.append(
            {
                "name": "dd:build-linked",
                "value": "true" if linked_flag else "false",
            }
        )

    components = [pio_component(p) for p in pio_pkgs] + lib_comps

    meta_props = [
        {"name": "dd:provenance", "value": "build-attested via platformio run"},
        {"name": "dd:build-env", "value": args.env},
        {"name": "dd:git-sha", "value": sha},
        {"name": "dd:built-at", "value": now},
    ]
    firmware = args.firmware
    if firmware is None:
        cand = root / "build_output" / "firmware" / f"{args.env}.bin"
        if cand.exists():
            firmware = cand
    if firmware and Path(firmware).exists():
        fw = Path(firmware)
        meta_props.append({"name": "dd:firmware-path", "value": str(fw)})
        meta_props.append({"name": "dd:firmware-sha256", "value": sha256_file(fw)})
        meta_props.append({"name": "dd:firmware-bytes", "value": str(fw.stat().st_size)})

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
                        "name": "generate_build_attested_cyclonedx.py",
                        "version": "0.1.0",
                    }
                ]
            },
            "component": {
                "type": "firmware",
                "name": args.name,
                "version": sha,
                "purl": f"pkg:github/DynamicDevices/Tasmota@{sha}?env={quote(args.env)}",
                "properties": meta_props,
            },
        },
        "components": components,
    }
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(bom, indent=2) + "\n", encoding="utf-8")
    linked_n = sum(
        1
        for c in lib_comps
        if any(p.get("name") == "dd:build-linked" and p.get("value") == "true" for p in c.get("properties") or [])
    )
    print(
        f"Wrote {args.out} pio_packages={len(pio_pkgs)} lib_manifests={len(lib_comps)} "
        f"lib_linked_heuristic={linked_n}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
