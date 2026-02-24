# 📡 MYI2C – Register-Level I²C Driver for ATmega328P

A minimal custom I²C (TWI) driver implemented at the register level for the ATmega328P (Arduino Nano / Uno), built to understand how I²C works internally rather than relying on high-level libraries.

This project focuses on bare-metal communication, direct hardware control, and protocol fundamentals.

---

## 🚀 Project

This driver was developed to:

- Explore the ATmega328P TWI peripheral
- Practice datasheet-driven development
- Understand START / STOP / ACK / READ / WRITE sequences
- Gain confidence working with microcontroller registers

---

## ⚙️ Features

- ✔ Register-level TWI/I²C control
- ✔ START and STOP condition generation
- ✔ Byte-level READ / WRITE
- ✔ Basic register read/write abstraction
- ✔ Simple timeout protection in reads

---

## 🧠 How It Works

The driver directly configures AVR TWI registers:

| Register | Purpose |
|----------|---------|
| `TWBR` | Bit rate (I²C clock) |
| `TWSR` | Prescaler |
| `TWCR` | Control & status |
| `TWDR` | Data register |

Clock configured for:

```
I²C Speed ≈ 100 kHz @ 16 MHz CPU
```

---

## 📂 Library Structure

```
MYI2C.h      → Class interface
MYI2C.cpp    → Register-level implementation
main.ino     → Application example
```

---

## 🔌 Hardware Used

- Arduino Nano (ATmega328P)
- MPU6050 / MPU9560 (Accelerometer/Gyro)
- SSD1306 OLED Display (I²C)

**Connections:**

| Device | Arduino Nano |
|--------|-------------|
| SDA | A4 (PC4) |
| SCL | A5 (PC5) |
| VCC | 5V / 3.3V |
| GND | GND |

> External pull-ups (recommended): **4.7kΩ**

---

## 🧾 Example Usage

```cpp
MYI2C i2c;

void setup() {
    i2c.begin();
    i2c.writeRegister(MPU_ADDR, PWR_MGMT_1, 0x00);
}

int16_t ax = (i2c.readRegister(MPU_ADDR, ACCEL_XOUT_H) << 8) |
              i2c.readRegister(MPU_ADDR, ACCEL_XOUT_H + 1);
```

---

## ⚠️ Current Limitations

This driver is intentionally minimal.

- ❌ No ACK/status verification
- ❌ Fully blocking design
- ❌ Inefficient multi-byte reads
- ❌ No bus recovery mechanism

---

## 📜 License

Open for learning and experimentation.

---

## 🤝 Contributions

Suggestions, optimizations, and improvements are welcome.
