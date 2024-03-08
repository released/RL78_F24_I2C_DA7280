/*_____ I N C L U D E S ____________________________________________________*/
#include <stdio.h>
#include "inc_main.h"

#include "misc_config.h"
#include "custom_func.h"

/*_____ D E C L A R A T I O N S ____________________________________________*/

struct flag_32bit flag_I2C_CTL;
#define FLAG_I2C_TRIG_I2C_SEND                 		    (flag_I2C_CTL.bit0)
#define FLAG_I2C_TRIG_I2C_RCV                       	(flag_I2C_CTL.bit1)
#define FLAG_I2C_REVERSE2                 				(flag_I2C_CTL.bit2)
#define FLAG_I2C_REVERSE3                      		    (flag_I2C_CTL.bit3)
#define FLAG_I2C_REVERSE4                       	    (flag_I2C_CTL.bit4)
#define FLAG_I2C_REVERSE5                               (flag_I2C_CTL.bit5)
#define FLAG_I2C_REVERSE6                               (flag_I2C_CTL.bit6)
#define FLAG_I2C_REVERSE7                               (flag_I2C_CTL.bit7)

/*_____ D E F I N I T I O N S ______________________________________________*/

/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/

// for driver level
MD_STATUS drv_I2C_read(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num)
{
	MD_STATUS ret = MD_OK;

	ret = R_Config_IICA0_Master_Receive(adr, rx_buf, rx_num, 100);

	if (ret != MD_OK)
    {
        printf("[drv_I2C_read error]0x%02X\r\n" , ret);
		return ret;
	} 
    return ret;
}


MD_STATUS drv_I2C_write(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num)
{
	MD_STATUS ret = MD_OK;

    ret = R_Config_IICA0_Master_Send(adr, tx_buf , tx_num , 100);

	if (ret != MD_OK)
    {
        printf("[drv_I2C_write error]0x%02X\r\n" , ret);
		return ret;
	} 
    return ret;
}

bool drv_Is_I2C_bus_busy(void)
{
    return IICBSY0;
}

void drv_I2C_callback_error(unsigned char err)
{
    R_Config_IICA0_StopCondition();
    printf("I2C MD_STATUS:0x%02X\r\n",err);
}

void drv_set_I2C_receive_flag(bool flag)
{
    FLAG_I2C_TRIG_I2C_RCV = flag;
}
bool drv_get_I2C_receive_flag(void)
{
    return FLAG_I2C_TRIG_I2C_RCV;
}

void drv_set_I2C_send_flag(bool flag)
{
    FLAG_I2C_TRIG_I2C_SEND = flag;
}
bool drv_get_I2C_send_flag(void)
{
    return FLAG_I2C_TRIG_I2C_SEND;
}

// for application

MD_STATUS I2C_read(unsigned char device_addr,unsigned char reg_addr,unsigned char* rx_xfer_data,unsigned short rx_num)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;

    tmp = reg_addr;

    while (drv_get_I2C_send_flag() || drv_Is_I2C_bus_busy()){ ; } 	//Make sure bus is ready for xfer
	drv_set_I2C_send_flag(1);

	ret = drv_I2C_write(device_addr, &tmp, 1);
	if (ret != MD_OK)
    {
        printf("[I2C read error1]0x%02X\r\n" , ret);
		return ret;
	} 
	while (drv_get_I2C_send_flag() || drv_Is_I2C_bus_busy()){ ; } 	//Wait until the xfer is complete

	drv_set_I2C_receive_flag(1);
	ret = drv_I2C_read(device_addr, rx_xfer_data, rx_num);
	while (drv_get_I2C_receive_flag() || drv_Is_I2C_bus_busy()){ ; } 	//Wait until the xfer is complete
	if (ret != MD_OK)
    {
        printf("[I2C read error2]0x%02X\r\n" , ret);
		return ret;
	} 

    return ret;
}

MD_STATUS I2C_write(unsigned char device_addr,unsigned char reg_addr,unsigned char* tx_xfer_data,unsigned short tx_num)
{
	MD_STATUS ret = MD_OK;
    unsigned char i = 0;
    unsigned char buffer[128] = {0};

    if (tx_num > 128)
    {
        printf("[I2C_write]tx_num over range(%2d)\r\n",tx_num);

        return MD_ARGERROR;
    }

    buffer[0] = reg_addr;
    for ( i = 0 ; i < tx_num ; i++ )
    {
        buffer[i+1] = tx_xfer_data[i];
    }

    while (drv_get_I2C_send_flag() || drv_Is_I2C_bus_busy()){ ; } 	//Make sure bus is ready for xfer
	drv_set_I2C_send_flag(1);

	ret = drv_I2C_write(device_addr, buffer , tx_num + 1);   // reg byte + data byte , at least 2 bytes
	if (ret != MD_OK)
    {
        printf("[I2C write error1]0x%02X\r\n" , ret);
		return ret;
	} 
	while (drv_get_I2C_send_flag() || drv_Is_I2C_bus_busy()){ ; } 	//Wait until the xfer is complete
   
    return ret;
}

