#include "all.h"
#include "comm.h"



size_t addr;
// Memory
uint8_t mem[MEM_LEN];

void setupComm() {
  // Setup for Slave mode, address 0x44, pins 18/19, external pullups, 400kHz
  Wire.begin(I2C_SLAVE, slaveAddress, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE_400);
  Serial.print("i2c Slave address: ");
  Serial.println(slaveAddress, HEX);
  // init vars
  addr = 0;
  for (size_t i = 0; i < MEM_LEN; i++)
    mem[i] = 0;
  // register events
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  print_i2c_status();


}
//
// handle Rx Event (incoming I2C request/data)
//
void receiveEvent(size_t len)
{
  if (!Wire.available()) return;
  Serial.println("receive event...");
  // grab addr
  addr = Wire.readByte();
  Serial.println(addr);
  while (Wire.available()) {
    uint8_t value =  Wire.readByte();
    Serial.println(value);
    if (addr <= MEM_LEN) {
      mem[addr] = value; // copy data to mem

      printf(Serial, "mem[%d] = %d\n", addr, value);
      addr++;
    }
  }
  Serial.println();
}



//
// handle Tx Event (outgoing I2C data)
//
void requestEvent(void)
{
  Serial.println("requestEvent...");
  uint8_t v = 0;
  if (addr < MEM_LEN)
    v = mem[addr];

  Wire.write(v);
  printf(Serial, "Sent mem[%d] which is %d\n", addr, v);
  addr++;
}

//
// print I2C status
//
void print_i2c_status(void)
{
  switch (Wire.status())
  {
    case I2C_WAITING:  Serial.print("I2C waiting, no errors\n"); break;
    case I2C_ADDR_NAK: Serial.print("Slave addr not acknowledged\n"); break;
    case I2C_DATA_NAK: Serial.print("Slave data not acknowledged\n"); break;
    case I2C_ARB_LOST: Serial.print("Bus Error: Arbitration Lost\n"); break;
    default:           Serial.print("I2C busy\n"); break;
  }
}
