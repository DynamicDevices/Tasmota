#!/usr/bin/env python3
"""Fail if CycloneDX lacks basic inventory quality for CRA progress."""
from __future__ import annotations
import argparse, json, sys
from pathlib import Path

def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("sbom", type=Path)
    ap.add_argument("--max-missing-version-pct", type=float, default=20.0)
    args = ap.parse_args()
    d = json.loads(args.sbom.read_text())
    comps = d.get("components") or []
    if not comps:
        print("FAIL: no components"); return 1
    missing_v = sum(1 for c in comps if not c.get("version"))
    pct = 100.0 * missing_v / len(comps)
    print(f"components={len(comps)} missing_version={missing_v} ({pct:.1f}%)")
    if pct > args.max_missing_version_pct:
        print(f"FAIL: missing version > {args.max_missing_version_pct}%"); return 1
    print("PASS: version coverage gate")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
