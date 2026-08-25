# SBOM generation (KettleCompanion / Tasmota)

## Artifacts

| Artifact | How | Attestation |
|----------|-----|-------------|
| `sbom/kettlecompanion-libs.cdx.json` | Manifest scan of `lib/**` | Not build-attested |
| `sbom/tasmota-kettle.build-attested.cdx.json` | `pio run -e tasmota-kettle` + PIO packages + manifests + firmware SHA-256 | Build-attested |
| `sbom/tasmota-kettle-minimal.build-attested.cdx.json` | Same for minimal env | Build-attested |

Build-attested BOM records: git SHA, env, firmware path/size/SHA-256, exact PlatformIO platform/tool versions from `pio pkg list`, and `dd:build-linked` on vendored libs matched from the LDF dependency graph.

JPEGDEC lives under `libesp32/` — ESP8266 kettle builds will list it as present in-tree but `dd:build-linked=false`.

## Local

```bash
pio run -e tasmota-kettle 2>&1 | tee sbom/pio-run-tasmota-kettle.log
PYTHONPATH=tools/sbom python3 tools/sbom/generate_build_attested_cyclonedx.py \
  --root . --env tasmota-kettle \
  --out sbom/tasmota-kettle.build-attested.cdx.json \
  --firmware build_output/firmware/tasmota-kettle.bin \
  --ldf-log sbom/pio-run-tasmota-kettle.log
```
