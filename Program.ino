#include <Arduino.h>
#include "MYI2C.h"

MYI2C i2c;

#define OLED_ADDR 0x3C
#define MPU_ADDR  0x68

#define PWR_MGMT_1   0x6B
#define ACCEL_XOUT_H 0x3B


// ---------------- FONT (5x7) ----------------

const uint8_t font[][5] =
{
{0x3E,0x51,0x49,0x45,0x3E}, //0
{0x00,0x42,0x7F,0x40,0x00}, //1
{0x42,0x61,0x51,0x49,0x46}, //2
{0x21,0x41,0x45,0x4B,0x31}, //3
{0x18,0x14,0x12,0x7F,0x10}, //4
{0x27,0x45,0x45,0x45,0x39}, //5
{0x3C,0x4A,0x49,0x49,0x30}, //6
{0x01,0x71,0x09,0x05,0x03}, //7
{0x36,0x49,0x49,0x49,0x36}, //8
{0x06,0x49,0x49,0x29,0x1E}  //9
};


// ---------------- OLED LOW LEVEL ----------------

void oled_cmd(uint8_t cmd)
{
    i2c.start();
    i2c.write(OLED_ADDR<<1);
    i2c.write(0x00);
    i2c.write(cmd);
    i2c.stop();
}

void oled_start()
{
    i2c.start();
    i2c.write(OLED_ADDR<<1);
    i2c.write(0x40);
}

void oled_stop()
{
    i2c.stop();
}


// ---------------- OLED INIT ----------------

void oled_init()
{
    delay(100);

    oled_cmd(0xAE);
    oled_cmd(0x20); oled_cmd(0x00);
    oled_cmd(0xB0);
    oled_cmd(0xC8);
    oled_cmd(0x00);
    oled_cmd(0x10);
    oled_cmd(0x40);
    oled_cmd(0x81); oled_cmd(0x7F);
    oled_cmd(0xA1);
    oled_cmd(0xA6);
    oled_cmd(0xA8); oled_cmd(0x3F);
    oled_cmd(0xD3); oled_cmd(0x00);
    oled_cmd(0xD5); oled_cmd(0x80);
    oled_cmd(0xD9); oled_cmd(0xF1);
    oled_cmd(0xDA); oled_cmd(0x12);
    oled_cmd(0xDB); oled_cmd(0x40);
    oled_cmd(0x8D); oled_cmd(0x14);
    oled_cmd(0xAF);
}


// ---------------- CLEAR ----------------

void oled_clear()
{
    for(int page=0; page<8; page++)
    {
        oled_cmd(0xB0+page);
        oled_cmd(0x00);
        oled_cmd(0x10);

        oled_start();

        for(int i=0;i<128;i++)
        i2c.write(0x00);

        oled_stop();
    }
}


// ---------------- CURSOR ----------------

void oled_set_cursor(uint8_t x,uint8_t page)
{
    oled_cmd(0xB0+page);
    oled_cmd(x & 0x0F);
    oled_cmd(0x10 | (x>>4));
}


// ---------------- PRINT CHAR ----------------

void oled_print_digit(char c)
{
    if(c<'0' || c>'9') return;

    oled_start();

    for(int i=0;i<5;i++)
    i2c.write(font[c-'0'][i]);

    i2c.write(0x00);

    oled_stop();
}


// ---------------- PRINT NUMBER ----------------

void oled_print_number(int num)
{
    char buffer[10];
    itoa(num,buffer,10);

    for(int i=0;buffer[i];i++)
    oled_print_digit(buffer[i]);
}


// ---------------- MPU ----------------

int16_t read16(uint8_t reg)
{
    uint8_t high=i2c.readRegister(MPU_ADDR,reg);
    uint8_t low=i2c.readRegister(MPU_ADDR,reg+1);

    return (int16_t)((high<<8)|low);
}

void mpu_init()
{
    i2c.writeRegister(MPU_ADDR,PWR_MGMT_1,0x00);
    delay(100);
}


// ---------------- SETUP ----------------

void setup()
{
    Serial.begin(115200);

    i2c.begin();

    oled_init();

    oled_clear();

    mpu_init();

    Serial.println("READY");
}


// ---------------- LOOP ----------------

void loop()
{
    int ax=read16(ACCEL_XOUT_H);
    int ay=read16(ACCEL_XOUT_H+2);
    int az=read16(ACCEL_XOUT_H+4);

    Serial.print("AX=");
    Serial.print(ax);
    Serial.print(" AY=");
    Serial.print(ay);
    Serial.print(" AZ=");
    Serial.println(az);

    oled_clear();

    oled_set_cursor(0,0);
    oled_print_number(ax);

    oled_set_cursor(0,2);
    oled_print_number(ay);

    oled_set_cursor(0,4);
    oled_print_number(az);

    delay(500);
}
