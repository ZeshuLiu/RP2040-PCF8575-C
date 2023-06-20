#include <stdio.h>
#include "pico/stdlib.h"
#include "PCF8575/PCF8575.h"
#include "hardware/i2c.h"

#define I2c0_CLK 100 // kHz
#define SDA0 4
#define SCL0 5
#define PCF8575_I2c i2c0

bool reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

void bus_scan(i2c_inst_t *scan_i2c){
    printf("\nI2C Bus Scan\n");
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

    for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x ", addr);
        }

        // Perform a 1-byte dummy read from the probe address. If a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. If the address byte is ignored, the function returns
        // -1.

        // Skip over any reserved addresses.
        int ret;
        uint8_t rxdata;
        if (reserved_addr(addr))
            ret = PICO_ERROR_GENERIC;
        else
            ret = i2c_read_blocking(scan_i2c, addr, &rxdata, 1, false);

        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
    }
    printf("Done.\n");
}

int main() {

    // 定义参数
    uint8_t cnt = 0;
    struct pcf8575 PCF0;
    uint8_t LED_Pin = PICO_DEFAULT_LED_PIN;
    char a[20];
    int i = 0;
    bool Led_Value;

    // 初始化输入输出
    stdio_init_all();

    sleep_ms(1000);//为了在vscode串行监视器自动重连后看到前面的内容

    //初始化I2c总线0
    i2c_init(PCF8575_I2c, I2c0_CLK * 1000);
    gpio_set_function(SDA0, GPIO_FUNC_I2C);
    gpio_set_function(SCL0, GPIO_FUNC_I2C);
    gpio_pull_up(SDA0);
    gpio_pull_up(SCL0);

    //初始化PCF8575
    PCF0 = PCF8575_init(PCF8575_I2c,0x20);
    PCF8575_set_dir_all(0x0001,&PCF0);
    PCF8575_SET_DEBGU(&PCF0, 0);

    //初始化板载LED灯
    gpio_init(LED_Pin);
    gpio_set_dir(LED_Pin, GPIO_OUT);
    gpio_put(LED_Pin, Led_Value);
    
    //后边是尝试将他作为Serial 命令行的一些尝试
    printf("Hello, world!\n");
    a[i] = getchar();
    while(a[i]!= '\n' && i<19){
        printf("%c",a[i]);
        i++;
        a[i] = getchar();
    }
    a[i+1] = '\0';
    printf("\n");
    printf("STR = %s", a);

    //扫描I2c总线上连接的所有设备，方便后面找地址
    bus_scan(PCF8575_I2c);

    while(true){
        //闪灯
        Led_Value = !Led_Value;
        gpio_put(LED_Pin,Led_Value);

        //利用闪灯的变量输出脉冲
        PCF8575_set_value(&PCF0,0,!Led_Value); //设置引脚输出，此时不会发送给设备
        printf("count = %d\n", cnt);
        PCF8575_SEND_VALUE(&PCF0); // 将设置发送给设备
        sleep_ms(80); //控制延时长度
        cnt += 1;
    }
    return 0;
}