/**
 * Master SAML - SPI
 */
#include "mbed.h"

Serial pc(USBTX, USBRX);

SPI spi(PA06, PA04, PA07); // mosi, miso, sclk, ssel

DigitalOut cs(PA05);

void write_reg(uint8_t adr, uint8_t val)
{
    uint8_t cmd = adr & 0x0F;
    uint8_t stored_val;

    cs = 0;
    spi.write(cmd); // Send the cmd to write the register.
    wait_ms(10);
    spi.write(val); // Send the value to be written.
    wait_ms(10);
    stored_val = spi.write(0xFF); // Send dummy byte, to get the value stored from slave.
    printf("[0x%02X] Write to REG_%X value 0x%02X", cmd, adr, val);
    printf("  ... %s\n", stored_val == val ? "OK" : "FAIL");
    cs = 1;
    wait_ms(10);
}

uint8_t read_reg(uint8_t adr)
{
    uint8_t val;
    uint8_t cmd = 0x10 | (adr & 0x0F);

    cs = 0;
    spi.write(cmd); // Send the cmd to read the register.
    wait_ms(10);
    val = spi.write(0xFF); // Send dummy byte, to get register read from slave.
    printf("[0x%02X] Read from REG_%X value 0x%02X\n", cmd, adr, val);
    cs = 1;
    wait_ms(10);

    return val;
}

int main()
{
    pc.baud(115200);
    printf("--- START ---\n");

    cs = 1;

    // Setup the spi for 8 bit data, high steady state clock,
    // second edge capture, with a 1MHz clock rate
    spi.format(8, 1);
    spi.frequency(1000000);

    write_reg(0x0A, 0xAA);
    write_reg(0x0B, 0xBB);
    write_reg(0x0C, 0xCC);

    read_reg(0x0A);
    read_reg(0x0B);
    read_reg(0x0C);

    while (true)
    {
        write_reg(0x0A, rand());
        wait(5);
        read_reg(0x0A);
    }
}