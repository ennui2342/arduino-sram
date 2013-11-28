#include <SPI.h>
#include "SRAM.h"

// Should be good for 23K256 and 23LC1024 but untested with the former
//
// Implements Stream for convenience so you get readBytes() etc. Not the fastest way
// to do it because of that (setting the address for every byte transfered)
//
// Really only SRAM_SEQN_MODE is properly supported (please contribute)
// Use seek() to set the address before reading and writing

/*
  Example usage:
  
  SRAM sram(SRAM_CS_PIN, SRAM_1024);
  char test [] = "hello, world";
  sram.seek(1);
  sram.write((byte *) test, sizeof test);
  char buf[100];
  sram.seek(1);
  sram.readBytes((char *) buf, sizeof buf);
  Serial.println(buf);
*/

// setup SRAM device by turning into sequential mode
SRAM::SRAM(uint8_t cs_pin, uint8_t size, uint8_t mode) {
  this->address = 1;
  this->cs_pin = cs_pin;
  this->mode = mode;
  this->size = size;
  // Need to support 256k SRAM as well
}

SRAM::SRAM(uint8_t cs_pin, uint8_t size) {
  SRAM(cs_pin, size, SRAM_SEQN_MODE);
}
  
void SRAM::begin() {
  SPI.begin();
  pinMode(this->cs_pin, OUTPUT);
  digitalWrite(this->cs_pin, LOW);
  SPI.transfer(SRAM_WRSR);
  SPI.transfer(this->mode);
  digitalWrite(this->cs_pin, HIGH);
}

void SRAM::seek(uint32_t address) {
  this->address = address;
}

size_t SRAM::write(uint8_t data) {
  digitalWrite(this->cs_pin, LOW);  
  SPI.transfer(SRAM_WRITE);
  if(this->size == SRAM_1024)
    SPI.transfer((this->address >> 16) & 0xFF);
    
  SPI.transfer((this->address >> 8)  & 0xFF);
  SPI.transfer(this->address & 0xFF);
  SPI.transfer(data);
  this->address++;
  digitalWrite(this->cs_pin, HIGH);
  
  return 1;
}

int SRAM::read() {
  digitalWrite(this->cs_pin, LOW);
  SPI.transfer(SRAM_READ);
  if(this->size == SRAM_1024)
    SPI.transfer((this->address >> 16) & 0xFF);
    
  SPI.transfer((this->address >> 8)  & 0xFF);
  SPI.transfer(this->address & 0xFF);
  uint8_t data = SPI.transfer(0);
  this->address++;
  digitalWrite(this->cs_pin, HIGH);
  
  return data;
}
