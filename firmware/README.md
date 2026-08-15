# Traffic light firmware editions

## National edition

- Official flashing-tool file: `national/traffic-light-national.bin`
- BIN SHA-256: `BE8E276A81F18AF5DB106083BA05D338C0D44369E2FB27EB1AC50FE8304B7FFC`
- Packaged image: `national/traffic-light-national.img`
- IMG SHA-256: `646760668471245A550153DC71F9619D9E3E869635370DD4A25D6EABAAE068E7`
- Behavior: straight/left/right green arrow, then the same arrow in yellow after 2 seconds, then solid red after another 2 seconds.

### National safe rollback

- BIN: `national/rollback/traffic-light-national-safe-3CD54114.bin`
- BIN SHA-256: `3CD54114ED5579E2CC55084AF4DBA4394B1B0DE0D74747B94D94D45D37CC7783`
- IMG: `national/rollback/traffic-light-national-safe-3CD54114.img`
- IMG SHA-256: `BE1B52DBEF636761E6E2FF6F44BA4300573741749E096E6685E28C505B45A0CE`

## Provincial edition

- Official flashing-tool file: `provincial/traffic-light-provincial.bin`
- BIN SHA-256: `85778FBAC876E76027336DC4A85B415B2C1A5E148DC5F9395BFE75FA7EA5817F`
- Packaged image: `provincial/traffic-light-provincial.img`
- IMG SHA-256: `C9BBE8496CA1FB2A57146B44FD97703EF345A326D8E3B19E6093C9AF386C23D0`
- Behavior: the original red/straight/left/right states remain displayed until another command is recognized.

Use the `.bin` files with the official flashing tool. The `.img` files are the optional output of the final `csk5060-enc-pack.exe` step in `build.bat`.

## Build selection

The CMake cache variable `TRAFFIC_LIGHT_EDITION` selects the edition:

```powershell
cmake -S . -B build -DTRAFFIC_LIGHT_EDITION=national
cmake -S . -B build -DTRAFFIC_LIGHT_EDITION=provincial
```

After selecting an edition, run the normal `build.bat all` command. The default edition is `national`.
