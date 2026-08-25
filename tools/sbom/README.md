# SBOM generation (KettleCompanion / Tasmota)

## What this produces

| Artifact | How | Attestation |
|----------|-----|-------------|
| `sbom/kettlecompanion-libs.cdx.json` | `generate_lib_cyclonedx.py` over `lib/**/library.json` + `library.properties` | Manifest/repo-scan — **not** build-attested |
| Syft filesystem CDX (CI) | `anchore/sbom-action` | Scan of checkout — complementary; weak on Arduino libs |

Preloop CRA audits need **build-attested** SBOMs (exact versions from a real `pio` build). This workflow is the first CI gate so releases always emit an inventory; a later step should run after `pio run -e tasmota-kettle` and merge PlatformIO package locks.

## Local

```bash
python3 tools/sbom/generate_lib_cyclonedx.py --root . --out sbom/kettlecompanion-libs.cdx.json
python3 tools/sbom/check_min_elements.py sbom/kettlecompanion-libs.cdx.json
```
