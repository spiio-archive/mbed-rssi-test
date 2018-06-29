The `master` branch is using a *raw* AT command api and the `ublox-api` branch is using the mbed ublox cellular api. 

- Clone the repo
- `mbed deploy` in the root
- If you are running this on a Spiio device copy the Spiio device target files into the `mbed/target` folder
- change the compiler setup in the file `.mbed` to reflect the target and the compiler you are using.
- your esim provider setup must be defined in the file `mbed_app.json`
- `mbed compile -c`
- Flash `st-flash write mbed-rssi-test.bin 0x8000000` or if you are running the program on a Ublox development board drag the `bin` file from the BUILD subdirectory to the development boards *USB* directory.

