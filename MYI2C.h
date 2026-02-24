#ifndef MYI2C_H
#define MYI2C_H

#include <avr/io.h>
#include <stdint.h>

class MYI2C
{
public:

    // Initialize I2C
    void begin();

    // Low-level functions
    void start();
    void stop();
    void write(uint8_t data);
    uint8_t read(bool ack);

    // High-level functions
    void writeRegister(uint8_t dev, uint8_t reg, uint8_t data);
    uint8_t readRegister(uint8_t dev, uint8_t reg);
};

#endif
