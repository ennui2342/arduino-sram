#include <SPI.h>
#include "SRAM.h"

SRAM::SRAM(uint8_t cs_pin, uint8_t size, uint8_t mode) {
  this->address = 1;
  this->cs_pin = cs_pin;
  this->mode = mode;
  this->size = size;
}

SRAM::SRAM(uint8_t cs_pin, uint8_t size) {
  this->address = 1;
  this->cs_pin = cs_pin;
  this->mode = SRAM_SEQN_MODE;
  this->size = size;
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
