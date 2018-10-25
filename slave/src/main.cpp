/**
 * Slave xDot - SPI
 */
#include "mbed.h"

Serial pc(USBTX, USBRX);
SPISlave device(PB_15, PB_14, PB_13, PB_12);  // mosi, miso, sclk, ssel

InterruptIn cs(PB_12);

DigitalOut led(LED1);

uint8_t command = 0x00;

// Simulate registers.
uint8_t reg[16] = {};

bool should_reset_command = false;

void mark_command_reset()
{
    should_reset_command = true;
}

/**
 * Read a register for commands like 0x1*,
 * where * is the register address.
 */
void read_reg()
{
    uint8_t adr = (command & 0x0F);
    uint8_t operation = (command & 0xF0);
    bool should_read_reg = (0x10 == operation);

    if (should_read_reg)
    {
        printf("Read REG[%d] = 0x%02X\n", adr, reg[adr]);
        device.reply(reg[adr]);
    }
}

/**
 * Write a byte to a register for commands like 0x0*, 
 * where * is the register address.
 */
void write_reg(uint8_t b)
{
    uint8_t adr = (command & 0x0F);
    uint8_t operation = (command & 0xF0);
    bool should_write_reg = (0x00 == operation);

    if (should_write_reg)
    {
        printf("Write REG[%d] = 0x%02X\n", adr, b);
        reg[adr] = b;
        device.reply(b);
    }
}

void reset_command()
{
    if (should_reset_command)
    {
        printf("Reset command!\n\n");
        command = 0x00;
        should_reset_command = false;
    }
}

int main()
{
    pc.baud(115200);
    printf("--- START ---\n");

    cs.rise(&mark_command_reset);

    device.format(8, 1);
    device.frequency(1000000);

    while (true)
    {
        reset_command();

        if (device.receive())
        {
            uint8_t b = device.read();

            if (0xFF == b)
            {
                continue;
            }

            printf("Received Byte = 0x%02X\n", b);
            printf("Active Command = 0x%02X\n", command);

            if (!command)
            {
                printf("Command changed to 0x%02X\n", b);
                command = b;
            }
            else
            {
                // expecting more bytes
                write_reg(b);
            }

            read_reg();
        }
    }
}