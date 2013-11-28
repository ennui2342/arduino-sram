arduino-sram
============

Arduino library for interfacing with 23K256 and 23LC1024 SRAM chips from Arduino

Implements Stream for convenience so you get readBytes() etc. But be aware this is not the fastest way to do it because of that (setting the address for every byte transfered)

## Installation

Clone as SRAM and put in your arduino libraries folder

## Notes

* Untested with 23K256
* Really only SRAM_SEQN_MODE is properly supported (please contribute if you use other modes)

## Usage

Use seek() to set the address before reading and writing

Example:
  
    SRAM sram(SRAM_CS_PIN, SRAM_1024);
    char test [] = "hello, world";
    sram.seek(1);
    sram.write((byte *) test, sizeof test);
    char buf[100];
    sram.seek(1);
    sram.readBytes((char *) buf, sizeof buf);
    Serial.println(buf);
