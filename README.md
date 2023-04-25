# embedded

### Topics:
- Used microcontroller: ESP32 dev module.
- Main lib: https://github.com/don/NDEF and https://github.com/Seeed-Studio/PN532 using I2C
- Components: One `LED` for show result of entrance try with `RFID card` ( the rfid card store the `public key` of the account / metamask ) and `PN532` module for RFID controller.
- Dev tool: Platformio, with easy setup, only installing from [platformio website](https://platformio.org/) !

### Pins mapping of microcontroller:
- `PN532`: SDA -> 21, SCL -> 22 ( I2C Wire pins)
- `LED`: 23

### Runtime:
1. Init `PN532` module.
2. Connect to `WIFI`.
3. Setup pin of `LED`.
4. Wait to wifi connection ( for GraphQL query ).
5. Loop of `PN532` module, if detect a card, then read the card and compare with the `public key` of the account / metamask, if match, then turn on the `LED` for 5 seconds. The update of `public key` is done by GraphQL query every 5 seconds...


Demo video: https://www.youtube.com/watch?v=wvsjVdfHjt0