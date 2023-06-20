#include "PCF8575.h"

/*! \brief 初始化PCF8575芯片，不发送数据，返回一个handler方便后续调用
 *  \ingroup PCF8575
 *
 * \param i2c 使用的设备i2c，i2c0或者i2c1
 * \param I2cAdd i2c设备地址（7位，不包括写地址和读地址）
 */
struct pcf8575 PCF8575_init(i2c_inst_t* i2c, uint8_t I2cAdd){
    struct pcf8575 r_handle;
    r_handle.I2c_ADD = I2cAdd;
    r_handle.i2c_dev = i2c;
    r_handle.DEBUG = 0;

    return r_handle;
}

/*! \brief 设置调试模式
 *  \ingroup PCF8575
 *
 * \param r_handle PCF8575的handler
 * \param DEBUG 调试与否 0/1
*/
void PCF8575_SET_DEBGU(struct pcf8575 *r_handle, bool DEBUG){
    r_handle->DEBUG = DEBUG;
}

/*! \brief 设置引脚的输入输出模式，
 *  \ingroup PCF8575
 *
 * \param __Pin__ 引脚号 0 1 ... 7, 10 11 ... 17
 * \param r_handle PCF8575的handler
 * \param Pin_Out 引脚是输入还是输出，0-输入， 1-输出
*/
void PCF8575_set_dir(uint8_t __Pin__, struct pcf8575 *r_handle, bool Pin_Out){
    if((__Pin__>7 && __Pin__ <10) || (__Pin__>17)){
        return;
    }
    if(__Pin__<=7){
        r_handle->IF_OUTPUT[__Pin__] = Pin_Out;
    }
    else{
        r_handle->IF_OUTPUT[__Pin__ -2] = Pin_Out;
    }
}


/*! \brief 一次性设置所有的引脚的输入输出
 *  \ingroup PCF8575
 *
 * \param P_OUT 16bit数，每一位代表一个引脚的输入、输出配置 0-输入， 1-输出；顺序为 17, 16 ... 10, 7 ... 1, 0 引脚
 * \param r_handle PCF8575的handler
*/
void PCF8575_set_dir_all(uint16_t P_OUT, struct pcf8575 *r_handle){
    int i = 0;

    for(i=0;i<16;i++){
        r_handle->IF_OUTPUT[i] = ( (0x0001<<i) == ((0x0001<<i)&P_OUT));
    }
}

/*
获取某个引脚设置输入输出的情况
*/
bool PCF8575_get_dir(uint8_t __Pin__, struct pcf8575 *r_handle){
    if((__Pin__>7 && __Pin__ <10) || (__Pin__>17)){
        return 0;
    }
    if(__Pin__<=7){
        return r_handle->IF_OUTPUT[__Pin__];
    }
    else{
        return  r_handle->IF_OUTPUT[__Pin__ -2];
    }
}

/*
获取所有引脚设置输入输出的情况
*/
uint16_t PCF8575_get_dir_all(struct pcf8575 *r_handle){
    int i = 0;
    uint16_t dir = 0x0000;
    for(i=0;i<16;i++){
        if(r_handle->IF_OUTPUT[i] == 1){
            dir += (0x0001<<i);
        }
    }
    return dir;
}

/*
设置某一个输出引脚的电平，如果引脚设置为输入，则在这操作无效；
此时不会将设置发送给芯片，需要调用PCF8575_SEND_VALUE（）
*/
void PCF8575_set_value(struct pcf8575 *r_handle, uint8_t __Pin__, bool value){
    if( (__Pin__>7 && __Pin__ <10) || (__Pin__>17) || (r_handle->IF_OUTPUT[__Pin__] == 0) ){

        if(r_handle->DEBUG){
            printf("NO PIN or Not output%d or Not %d\n",__Pin__,r_handle->IF_OUTPUT[__Pin__]);
        }

        return;
    }

    if(r_handle->DEBUG){
        printf("Setting Value %d-->%d\n",__Pin__,value);
    }

    if(__Pin__<=7){
        r_handle->PIN_SET_VALUE[__Pin__] = value;
    }
    else{
        r_handle->PIN_SET_VALUE[__Pin__ - 2] = value;
    }
}

/*
将引脚电平设置发送给芯片
*/
void PCF8575_SEND_VALUE(struct pcf8575 *r_handle){
    uint16_t cmd_value = 0xffff;
    uint8_t cmd[2] = {0xff,0xff};
    for(int i = 0; i <16; i++){
        if(r_handle->IF_OUTPUT[i]==1 && r_handle->PIN_SET_VALUE[i] == 0 ){
            cmd_value = cmd_value&(~(0x0001<<i));
        }
    }
    cmd[0] = cmd[0]&(cmd_value);
    cmd[1] = cmd[1]&(cmd_value>>8);

    if(r_handle->DEBUG){
        printf("Send data %02x %02x addr = %02x\n",cmd[0],cmd[1],0x20);
        printf("Handle Addr %02x", r_handle->I2c_ADD);
    }

    i2c_write_blocking(i2c0, 0x20, cmd,2,false);
}

void PCF8575_READ_VALUE(){

}