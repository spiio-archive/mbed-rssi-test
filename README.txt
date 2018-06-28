- Clone the repo
- 'mbed deploy' in the root
- `mbed compile``
- Flash `st-flash write mbed-rssi-test.bin 0x8000000`

The master branch is using a 'raw' at command api and the 'ublox-api' branch is using the mbed ublox cellular api. 