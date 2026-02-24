#include "MYI2C.h"
#include <avr/io.h>
#include <util/delay.h>

void MYI2C::begin()
{
    // Enable internal pull-ups on SDA and SCL
    PORTC |= (1 << PC4) | (1 << PC5);
    // Set prescaler = 1
    TWSR = 0x00;

    // bit rate = 100kHz
    TWBR = 72;

    // Enable TWI
    TWCR = (1 << TWEN);

    _delay_ms(100);
}

void MYI2C::start()
{
    //  START condition
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA);

    // Wait for TWINT flag set
    while (!(TWCR & (1 << TWINT)));
}

void MYI2C::stop()
{
    // Send STOP condition
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO);

    // Wait until STOP is completed
    while (TWCR & (1 << TWSTO));
}

void MYI2C::write(uint8_t data)
{
    // Load data into register
    TWDR = data;

    // Start transmission
    TWCR = (1 << TWEN) | (1 << TWINT);

    // Wait until transmission complete
    while (!(TWCR & (1 << TWINT)));
}

uint8_t MYI2C::read(bool ack)
{
    uint16_t timeout = 0;

    if (ack)
        TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    else
        TWCR = (1 << TWEN) | (1 << TWINT);

    while (!(TWCR & (1 << TWINT)))
    {
        timeout++;
        if (timeout > 50000)
            return 0;
    }

    return TWDR;
}

void MYI2C::writeRegister(uint8_t dev, uint8_t reg, uint8_t data)
{
    start();

    // Send device address + write
    write((dev << 1) | 0);

    // Send register address
    write(reg);

    // Send data
    write(data);

    stop();

    _delay_us(10);
}

uint8_t MYI2C::readRegister(uint8_t dev, uint8_t reg)
{
    uint8_t data;

    start();

    // Send device address + write
    write((dev << 1) | 0);

    // Send register address
    write(reg);

    start();

    // Send device address + read
    write((dev << 1) | 1);

    // Read data (no ACK because single byte)
    data = read(false);

    stop();

    _delay_us(10);

    return data;
}
