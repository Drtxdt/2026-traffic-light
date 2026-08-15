# Traffic light firmware editions

## National edition

- Official flashing-tool file: `national/traffic-light-national.bin`
- BIN SHA-256: `82F0FDE68821E2F1336FE28DF8BD4AEE44A58E723F1161A39242AE2BEA3E25F2`
- Packaged image: `national/traffic-light-national.img`
- IMG SHA-256: `D599F79F6696032954A45C88BC8929AD626CEFB76B592A0B0EBB5F6FEA346B90`
- Behavior: straight/left/right green arrow, then the same arrow in yellow after 2 seconds, then solid red after another 2 seconds.

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
