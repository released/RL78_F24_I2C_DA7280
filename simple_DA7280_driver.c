/*_____ I N C L U D E S ____________________________________________________*/
#include <stdio.h>

#include "inc_main.h"

#include "misc_config.h"
#include "custom_func.h"

#include "simple_DA7280_driver.h"

#include "rm_da7280.h"

/*_____ D E C L A R A T I O N S ____________________________________________*/

struct flag_32bit flag_HAPTIC_CTL;
#define FLAG_HAPTIC_TRIG_BTN                 	            (flag_HAPTIC_CTL.bit0)
#define FLAG_HAPTIC_TRIG_DA7280_FINISH                      (flag_HAPTIC_CTL.bit1)
#define FLAG_HAPTIC_REVERSE2                 	            (flag_HAPTIC_CTL.bit2)
#define FLAG_HAPTIC_REVERSE3                                (flag_HAPTIC_CTL.bit3)
#define FLAG_HAPTIC_REVERSE4                                (flag_HAPTIC_CTL.bit4)
#define FLAG_HAPTIC_REVERSE5                                (flag_HAPTIC_CTL.bit5)
#define FLAG_HAPTIC_REVERSE6                                (flag_HAPTIC_CTL.bit6)
#define FLAG_HAPTIC_REVERSE7                                (flag_HAPTIC_CTL.bit7)

#define FLAG_HAPTIC_PLAY_IDX1                               (flag_HAPTIC_CTL.bit8)
#define FLAG_HAPTIC_PLAY_IDX2                               (flag_HAPTIC_CTL.bit9)
#define FLAG_HAPTIC_PLAY_IDX3                               (flag_HAPTIC_CTL.bit10)
#define FLAG_HAPTIC_PLAY_IDX4                               (flag_HAPTIC_CTL.bit11)
#define FLAG_HAPTIC_PLAY_IDX5                               (flag_HAPTIC_CTL.bit12)
#define FLAG_HAPTIC_PLAY_IDX6                               (flag_HAPTIC_CTL.bit13)
#define FLAG_HAPTIC_PLAY_IDX7                               (flag_HAPTIC_CTL.bit14)
#define FLAG_HAPTIC_PLAY_IDX8                               (flag_HAPTIC_CTL.bit15)

#define FLAG_HAPTIC_PLAY_IDX9                               (flag_HAPTIC_CTL.bit16)
#define FLAG_HAPTIC_PLAY_IDX10                              (flag_HAPTIC_CTL.bit17)
#define FLAG_HAPTIC_PLAY_IDX11                              (flag_HAPTIC_CTL.bit18)
#define FLAG_HAPTIC_PLAY_IDX12                              (flag_HAPTIC_CTL.bit19)
#define FLAG_HAPTIC_PLAY_IDX13                              (flag_HAPTIC_CTL.bit20)
#define FLAG_HAPTIC_PLAY_IDX14                              (flag_HAPTIC_CTL.bit21)
#define FLAG_HAPTIC_PLAY_IDX15                              (flag_HAPTIC_CTL.bit22)
#define FLAG_HAPTIC_REVERSE23                               (flag_HAPTIC_CTL.bit23)

/*_____ D E F I N I T I O N S ______________________________________________*/

/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/

extern MD_STATUS I2C_read(unsigned char device_addr,unsigned char reg_addr,unsigned char* rx_xfer_data,unsigned short rx_num);
extern MD_STATUS I2C_write(unsigned char device_addr,unsigned char reg_addr,unsigned char* tx_xfer_data,unsigned short tx_num);

/*
    example code : 9 mode
     SEQUENCE_SHORT         = 1,
     SEQUENCE_MEDIUM        = 2,
     SEQUENCE_LONG          = 3,
     SEQUENCE_VERY_LONG     = 4,
     SEQUENCE_SHORT_LONG    = 5,
     SEQUENCE_PHONE_RING    = 6,
     SEQUENCE_QUIET         = 7,
     SEQUENCE_RAMP_UP       = 8,
     SEQUENCE_ALARM_CLOCK,


    SEQUENCE_SHORT          1 Short,        constant buzz (40ms)
    SEQUENCE_MEDIUM         2 Medium,       constant buzz (140ms)
    SEQUENCE_LONG           3 Long,         constant buzz (200ms)
    SEQUENCE_VERY_LONG      4               Very long, constant buzz (450ms) 
    SEQUENCE_SHORT_LONG     5               Short buzz, followed by long buzz (600ms)
    SEQUENCE_PHONE_RING     6               2 short buzzes, followed by long buzz (800ms)
    SEQUENCE_QUIET          7               Low amplitude buzz that reduces in intensity (500ms)
    SEQUENCE_RAMP_UP        8               Gradually increasing buzz from silent (500ms)
    SEQUENCE_ALARM_CLOCK    9               Sequence of 9 short buzzes (1200ms)

*/

const unsigned char snp_mem[100] = {
    0x08, 0x0B, 0x15, 0x16, 0x19, 0x1B, 0x1F, 0x21, 0x22, 0x23,
    0x29, 0x31, 0x32, 0x35, 0x38, 0x40, 0x4A, 0x4C, 0x4F, 0x51,
    0x5B, 0x27, 0x17, 0x04, 0xB3, 0xD0, 0xF2, 0xB5, 0x77, 0x39,
    0x70, 0x70, 0x79, 0x69, 0x70, 0x79, 0x09, 0x8C, 0x75, 0x08,
    0x84, 0x73, 0x01, 0x9C, 0x74, 0x00, 0x81, 0x08, 0x84, 0x73,
    0x0D, 0x1A, 0x88, 0x06, 0x1A, 0xA0, 0x06, 0x09, 0x88, 0x06,
    0x10, 0x90, 0x1A, 0xA0, 0x06, 0x0D, 0x10, 0x90, 0x0D, 0x10,
    0x90, 0x12, 0xB0, 0x06, 0x18, 0x1B, 0x18, 0x1C, 0x06, 0x18,
    0x0D, 0xB8, 0x12, 0xD0, 0x06, 0x10, 0xA0, 0x05, 0x10, 0xA0,
    0x05, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


MD_STATUS _simple_DA7280_CheckIRQ_EVENT_WARNING_DIAG(void)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_IRQ_EVENT_WARNING_DIAG;
    const unsigned char indicator[] = "DA7280_CheckIRQ_EVENT_WARNING_DIAG";
 
    ret = I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s-1:0x%02X\r\n",indicator,ret);
        return ret;
    }

    printf("%s-1:0x%02X\r\n",indicator,tmp);

    switch(tmp)
    {
        case DA7280_E_OVERTEMP_WARN_MASK:
            printf("%s-2:%s\r\n",indicator,"E_OVERTEMP_WARN");
            break;
        case DA7280_E_MEM_TYPE_MASK:
            printf("%s-2:%s\r\n",indicator,"E_MEM_TYPE");
            break;
        case DA7280_E_LIM_DRIVE_ACC_MASK:
            printf("%s-2:%s\r\n",indicator,"E_LIM_DRIVE_ACC");
            break;
        case DA7280_E_LIM_DRIVE_MASK:
            printf("%s-2:%s\r\n",indicator,"E_LIM_DRIVE");
            break;
        default:
            printf("[error]%s-2:0x%02X\r\n",indicator,tmp);
            break;
    }

    if (ret == MD_OK)
    {
        printf("%s done\r\n",indicator);
    }

	return ret;
}



MD_STATUS _simple_DA7280_CheckIRQ_EVENT_SEQ_DIAG(void)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_IRQ_EVENT_PAT_DIAG;
    const unsigned char indicator[] = "DA7280_CheckIRQ_EVENT_SEQ_DIAG";
 
    ret = I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s-1:0x%02X\r\n",indicator,ret);
        return ret;
    }

    printf("%s-1:0x%02X\r\n",indicator,tmp);

    switch(tmp)
    {
        case DA7280_E_PWM_FAULT_MASK:
            printf("%s-2:%s\r\n",indicator,"E_PWM_FAULT");
            break;
        case DA7280_E_MEM_FAULT_MASK:
            printf("%s-2:%s\r\n",indicator,"E_MEM_FAULT");
            break;
        case DA7280_E_SEQ_ID_FAULT_MASK:
            printf("%s-2:%s\r\n",indicator,"E_SEQ_ID_FAULT");
            break;
        default:
            printf("[error]%s-2:0x%02X\r\n",indicator,tmp);
            break;
    }

    if (ret == MD_OK)
    {
        printf("%s done\r\n",indicator);
    }

	return ret;
}


MD_STATUS _simple_DA7280_SetIRQEvent1(unsigned char report_event)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_IRQ_EVENT1;
    unsigned char event = 0;
    const unsigned char indicator[] = "DA7280_SetIRQEvent1";
 
    ret = I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    event = tmp;
    printf("%s-1:0x%02X\r\n",indicator,event);

    tmp = (unsigned char)(event & ~report_event);
    tmp = (unsigned char)(tmp | report_event );  

    I2C_write(device_addr,reg_addr,&tmp,1);

    printf("%s-2:0x%02X\r\n",indicator,tmp);

    //  read back event status  
    ret = I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    printf("%s-3:0x%02X\r\n",indicator,tmp);

	return ret;
}


MD_STATUS _simple_DA7280_CheckIRQEvent1(unsigned char* report_event)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_IRQ_EVENT1;
    const unsigned char indicator[] = "DA7280_CheckIRQEvent1";
 
    ret = I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    if (!tmp)
    {
        *report_event = HAPTIC_SUCCESS;
	    return ret;
    }

    switch(tmp)
    {
        case E_SEQ_CONTINUE:
            *report_event = E_SEQ_CONTINUE;
            break;
        case E_UVLO:
            *report_event = E_UVLO;
            break;
        case E_SEQ_DONE:
            *report_event = E_SEQ_DONE;
            break;
        case E_OVERTEMP_CRIT:
            *report_event = E_OVERTEMP_CRIT;
            break;
        case E_SEQ_FAULT:
            *report_event = E_SEQ_FAULT;
            break;
        case E_WARNING:
            *report_event = E_WARNING;
            break;
        case E_ACTUATOR_FAULT:
            *report_event = E_ACTUATOR_FAULT;
            break;
        case E_OC_FAULT:
            *report_event = E_OC_FAULT;
            break;
    }

    if (ret == MD_OK)
    {
        printf("%s done(0x%02X)\r\n",indicator,tmp);
    }

	return ret;
}



MD_STATUS _simple_DA7280_SetSequenceDone(bool enable)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_IRQ_MASK1;
    unsigned char SEQ_DONE_M  = 0;
    const unsigned char indicator[] = "DA7280_SetSequenceDone";

    ret = I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    SEQ_DONE_M = tmp;
    printf("%s-1:0x%02X\r\n",indicator,SEQ_DONE_M);

    tmp = (unsigned char)(SEQ_DONE_M & ~DA7280_SEQ_DONE_M_MASK);
    tmp = (unsigned char)(tmp | ((enable ? 1:0) << DA7280_SEQ_DONE_M_SHIFT));  

    I2C_write(device_addr,reg_addr,&tmp,1);

    printf("%s-2:0x%02X\r\n",indicator,tmp);
	return ret;
}


MD_STATUS _simple_DA7280_CheckSequenceDone(void)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_IRQ_MASK1;
    unsigned char res = 0;
    const unsigned char indicator[] = "DA7280_CheckSequenceDone";

    do
    {
        
        ret = I2C_read(device_addr,reg_addr,&tmp,1);
        if (ret != MD_OK)
        {
            printf("[error]%s:0x%02X\r\n",indicator,ret);
            return ret;
        }
        /*
            SEQ_DONE_M 
            1 : complete ( nIRQ = 0)
        */
        res =  (tmp >> DA7280_SEQ_DONE_M_SHIFT) & 0x01 ? 1 : 0 ; 

    } while (res != 1);    

	return ret;
}

/*
    STANDBY_EN 
    1 : stand by
    0 : idle
*/
MD_STATUS _simple_DA7280_SetStandByState(bool enable)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_TOP_CTL1;
    unsigned char STANDBY_EN  = 0;
    const unsigned char indicator[] = "DA7280_SetStandByState";

    ret = I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    STANDBY_EN = tmp;
    printf("%s-1:0x%02X\r\n",indicator,STANDBY_EN);

    tmp = (unsigned char)(STANDBY_EN & ~DA7280_STANDBY_EN_MASK);
    tmp = (unsigned char)(tmp | ((enable ? 1:0) << DA7280_STANDBY_EN_SHIFT));  

    I2C_write(device_addr,reg_addr,&tmp,1);

    printf("%s-2:0x%02X\r\n",indicator,tmp);
	return ret;
}


MD_STATUS _simple_DA7280_SetPlayBackMemory(bool enable)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_TOP_CTL1;
    unsigned char SEQ_START  = 0;
    const unsigned char indicator[] = "DA7280_SetPlayBackMemory";

    ret = I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    SEQ_START = tmp;
    printf("%s-1:0x%02X\r\n",indicator,SEQ_START);

    tmp = (unsigned char)(SEQ_START & ~DA7280_SEQ_START_MASK);
    tmp = (unsigned char)(tmp | ((enable ? 1:0) << DA7280_SEQ_START_SHIFT));  

    I2C_write(device_addr,reg_addr,&tmp,1);

    printf("%s-2:0x%02X\r\n",indicator,tmp);
	return ret;
}


MD_STATUS _simple_DA7280_Set_PS_SEQ_ID_LOOP(unsigned char SEQ_ID , unsigned char SEQ_LOOP)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_SEQ_CTL2;
    unsigned char SEQ_CTL2  = 0;
    const unsigned char indicator[] = "DA7280_Set_PS_SEQ_ID_LOOP";

    if (SEQ_ID > 15)
    {
        return MD_ARGERROR;
    }
    if (SEQ_LOOP > 15)
    {
        return MD_ARGERROR;
    }

    ret = I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    SEQ_CTL2 = tmp;
    printf("%s-1:0x%02X\r\n",indicator,SEQ_CTL2);

    // tmp = (unsigned char)(tmp | (SEQ_ID << DA7280_PS_SEQ_ID_SHIFT));  
    
    tmp = (unsigned char)(0x00 | (SEQ_ID << DA7280_PS_SEQ_ID_SHIFT));   // force to renew ID,loop each time
    tmp = (unsigned char)(tmp | (SEQ_LOOP << DA7280_PS_SEQ_LOOP_SHIFT));    // 0 ~ 0XF

    I2C_write(device_addr,reg_addr,&tmp,1);

    printf("%s-2:0x%02X\r\n",indicator,tmp);
	return ret;
}


MD_STATUS _simple_DA7280_SetContinueSequence(bool enable)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_SEQ_CTL1;
    unsigned char SEQ_CONTINUE  = 0;
    const unsigned char indicator[] = "DA7280_SetContinueSequence";

    ret = I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    SEQ_CONTINUE = tmp;
    printf("%s-1:0x%02X\r\n",indicator,SEQ_CONTINUE);

    tmp = (unsigned char)(SEQ_CONTINUE & ~DA7280_SEQ_CONTINUE_MASK);
    tmp = (unsigned char)(tmp | ((enable ? 1:0) << DA7280_SEQ_CONTINUE_SHIFT));  

    I2C_write(device_addr,reg_addr,&tmp,1);

    printf("%s-2:0x%02X\r\n",indicator,tmp);
	return ret;
}


MD_STATUS _simple_DA7280_PlayFromMemory(void)
{
    /*
        playFromMemory
        - read DA7280_TOP_CTL1 (1 byte)
        - write DA7280_TOP_CTL1 (1 byte)
            transfer_data[1] = (uint8_t)(playMem & ~DA7280_SEQ_START_MASK);
            transfer_data[1] = (uint8_t)(transfer_data[1] | DA7280_SEQ_START_MASK);
    */
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_TOP_CTL1;
    unsigned char playMem = 0;
    const unsigned char indicator[] = "DA7280_PlayFromMemory";

    ret = I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    playMem = tmp;
    printf("%s-1:0x%02X\r\n",indicator,playMem);

    tmp = (unsigned char)(playMem & ~DA7280_SEQ_START_MASK);
    tmp = (unsigned char)(tmp | DA7280_SEQ_START_MASK);

    I2C_write(device_addr,reg_addr,&tmp,1);

    printf("%s-2:0x%02X\r\n",indicator,tmp);
	return ret;
}

MD_STATUS _simple_DA7280_WriteWaveform(const unsigned char*  buffer_mem)
{
    /*
        setWaveform : snp_mem
            - DA7280_SNP_MEM_0 , with seq write ( snp_mem : 100 bytes )
                ex : addr: DA7280_SNP_MEM_0+i , data : snp_mem[i]
    */
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_SNP_MEM_0;
    unsigned char i = 0;
    const unsigned char indicator[] = "DA7280_WriteWaveform";

    for( i = 0 ; i < 100 ; i++)
    {
        tmp = buffer_mem[i];
        ret = I2C_write(device_addr,reg_addr + i, &tmp ,1);
        
        #if 1   // debug
        printf("0x%02X," , tmp);
        if ((i+1)%10 ==0)
        {
            printf("\r\n");
        }   
        #endif

        if (ret != MD_OK)
        {
            printf("[error]%s:0x%02X\r\n",indicator,ret);
            return ret;
        }
    }

    if (ret == MD_OK)
    {
        printf("%s done\r\n",indicator);
    }

	return ret;
}

MD_STATUS _simple_DA7280_SetMode(unsigned char mode)
{
    /*
        SetMode : DA7280_ETWM_MODE
        - read DA7280_TOP_CTL1 (1 byte)
        - write DA7280_TOP_CTL1 (1 byte)
            transfer_data[1] = (uint8_t)(modeSet & ~DA7280_OPERATION_MODE_MASK);
            transfer_data[1] = (uint8_t)(transfer_data[1] | (mode << DA7280_OPERATION_MODE_SHIFT));
    */
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_TOP_CTL1;
    unsigned char modeSet = 0;
    const unsigned char indicator[] = "DA7280_SetMode";

    ret = I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    modeSet = tmp;
    printf("%s-1:0x%02X\r\n",indicator,modeSet);

    tmp = (unsigned char)(modeSet & ~DA7280_OPERATION_MODE_MASK);
    tmp = (unsigned char)(tmp | (mode << DA7280_OPERATION_MODE_SHIFT));

    I2C_write(device_addr,reg_addr,&tmp,1);

    printf("%s-2:0x%02X\r\n",indicator,tmp);
	return ret;
}


MD_STATUS _simple_DA7280_SetFrequencyTracking(bool enable)
{
    /*  
        SetFrequencyTracking : enable
        - read DA7280_TOP_CFG1 (1 byte)
        - write DA7280_TOP_CFG1 (1 byte)
            transfer_data[1] = (uint8_t)(freqTrack & ~DA7280_FREQ_TRACK_EN_MASK);
            transfer_data[1] = (uint8_t)(transfer_data[1] | ((enable ? 1:0) << DA7280_FREQ_TRACK_EN_SHIFT));  
    */
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_TOP_CFG1;
    unsigned char freqTrack = 0;
    const unsigned char indicator[] = "DA7280_SetFrequencyTracking";

    ret = I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    freqTrack = tmp;
    printf("%s-1:0x%02X\r\n",indicator,freqTrack);

    tmp = (unsigned char)(freqTrack & ~DA7280_FREQ_TRACK_EN_MASK);
    tmp = (unsigned char)(tmp | ((enable ? 1:0) << DA7280_FREQ_TRACK_EN_SHIFT));  

    I2C_write(device_addr,reg_addr,&tmp,1);

    printf("%s-2:0x%02X\r\n",indicator,tmp);
	return ret;
}

MD_STATUS _simple_DA7280_Open(void)
{
    /*
        Open : 
        - read DA7280_CHIP_REV (1 byte) , check if return value is 0xBA  (1 byte)  
    */
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;
    unsigned char device_addr = DA7280_ADDR_8BIT;
    unsigned char reg_addr = DA7280_CHIP_REV;
    const unsigned char indicator[] = "DA7280_Open";

    ret = I2C_read(device_addr,reg_addr,&tmp,1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
        return ret;
    }

    printf("%s:0x%02X\r\n", indicator ,tmp);
	return ret;
}

void DA7280_forceStop(void)
{
    /*
        At any time during operation in RTWM mode, set OPERATION_MODE or SEQ_START = 0 to return 
        to the IDLE or STANDBY state. 
    */
   
    MD_STATUS ret = MD_OK;
    const unsigned char indicator[] = "DA7280_forceStop";

    ret = _simple_DA7280_SetPlayBackMemory(0);

    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
    }

    if (ret == MD_OK)
    {
        printf("%s done\r\n",indicator);
    }
}


void DA7280_playback_finish_check(void)
{
    /*    
        4.  When the haptic sequence is completed, DA7280 will signal this by setting nIRQ = 0 and setting 
        SEQ_DONE_M = 1. DA7280 will then return to IDLE or STANDBY state, depending on the value 
        of STANDBY_EN.     
    */

    MD_STATUS ret = MD_OK;
    const unsigned char indicator[] = "DA7280_playback";
    unsigned char event = 0;

    do
    {
        ret = _simple_DA7280_CheckIRQEvent1(&event);
        if (ret != MD_OK)
        {
            printf("[error]%s:0x%02X\r\n",indicator,ret);
        }

        if ((event == E_SEQ_DONE) || 
            (event == HAPTIC_SUCCESS))
        {
            break;
        }
        else if (event == E_SEQ_FAULT)
        {
            _simple_DA7280_CheckIRQ_EVENT_SEQ_DIAG();
            printf("[error]%s:event-E_SEQ_FAULT\r\n",indicator);
            
            /*
                5.  Clear the nIRQ and SEQ_DONE_M signals, set SEQ_DONE_M =1. 
            */
            ret = _simple_DA7280_SetIRQEvent1(event);
            if (ret != MD_OK)
            {
                printf("[error]%s:0x%02X\r\n",indicator,ret);
            }
        }
        else if (event == E_WARNING)
        {
            _simple_DA7280_CheckIRQ_EVENT_WARNING_DIAG();
            printf("[error]%s:event-E_WARNING\r\n",indicator);
            
            /*
                5.  Clear the nIRQ and SEQ_DONE_M signals, set SEQ_DONE_M =1. 
            */
            ret = _simple_DA7280_SetIRQEvent1(event);
            if (ret != MD_OK)
            {
                printf("[error]%s:0x%02X\r\n",indicator,ret);
            }
        }
        else
        {
            printf("[error]%s:event-0x%02X\r\n",indicator,event);
            
            /*
                5.  Clear the nIRQ and SEQ_DONE_M signals, set SEQ_DONE_M =1. 
            */
            ret = _simple_DA7280_SetIRQEvent1(event);
            if (ret != MD_OK)
            {
                printf("[error]%s:0x%02X\r\n",indicator,ret);
            }

        }

    }while(1);

    /*
        5.  Clear the nIRQ and SEQ_DONE_M signals, set SEQ_DONE_M =1. 
    */
    ret = _simple_DA7280_SetIRQEvent1(event);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
    }    

    if (ret == MD_OK)
    {
        printf("%s(0x%02X) done\r\n",indicator,event);
    }

}

void DA7280_playback_idx(unsigned char idx , unsigned char loop , unsigned char enableContinue)
{

    MD_STATUS ret = MD_OK;
    const unsigned char indicator[] = "DA7280_playback";
    // unsigned char event = 0;

    printf("\r\n%s start\r\n",indicator);

    /*
        1.  While in the IDLE or STANDBY state, configure PS_SEQ_ID and PS_SEQ_LOOP to select the 
        desired sequence from Waveform Memory.  
    */
    ret = _simple_DA7280_Set_PS_SEQ_ID_LOOP(idx,loop);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
    }

    /*
        2.  For first-time playback, set OPERATION_MODE = 3. On subsequent sequence playbacks, this 
        step can be skipped (if OPERATION_MODE = 3). The haptic sequence will not begin playing 
        until a start event is created by setting SEQ_START = 1.     
    */
    ret = _simple_DA7280_SetPlayBackMemory(1);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
    }

    /*
        3.  While in the DRIVE state, set SEQ_CONTINUE = 1 to repeat the sequence.     
    */
    ret = _simple_DA7280_SetContinueSequence(enableContinue);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
    }

    #if 0
    /*    
        4.  When the haptic sequence is completed, DA7280 will signal this by setting nIRQ = 0 and setting 
        SEQ_DONE_M = 1. DA7280 will then return to IDLE or STANDBY state, depending on the value 
        of STANDBY_EN.     
    */


    do
    {
        ret = _simple_DA7280_CheckIRQEvent1(&event);
        if (ret != MD_OK)
        {
            printf("[error]%s:0x%02X\r\n",indicator,ret);
        }
    }while(event != E_SEQ_DONE);

    /*
        5.  Clear the nIRQ and SEQ_DONE_M signals, set SEQ_DONE_M =1. 
    */
    ret = _simple_DA7280_SetIRQEvent1(event);
    if (ret != MD_OK)
    {
        printf("[error]%s:0x%02X\r\n",indicator,ret);
    }
    #endif

    if (ret == MD_OK)
    {
        printf("%s done\r\n",indicator);
    }
}

/*
    SCL: P71
    SDA: P70
    nIRQ : P77 (INTP12)
*/

void DA7280_init(void)
{

    _simple_DA7280_Open();
    _simple_DA7280_SetFrequencyTracking(true);
    // _simple_DA7280_SetMode(DA7280_ETWM_MODE);    // DA7280_RTWM_MODE ? 
    _simple_DA7280_SetMode(DA7280_RTWM_MODE);
    _simple_DA7280_WriteWaveform(snp_mem);


    printf("DA7280_init end\r\n");
}


void DA7280_key_filter(unsigned char key)
{
    if (flag_play)
    {
        flag_play = 0;
        switch(key)
        {
            case '1':
                FLAG_HAPTIC_PLAY_IDX1 = 1;
                break;
            case '2':
                FLAG_HAPTIC_PLAY_IDX2 = 1;
                break;
            case '3':
                FLAG_HAPTIC_PLAY_IDX3 = 1;
                break;
            case '4':
                FLAG_HAPTIC_PLAY_IDX4 = 1;
                break;
            case '5':
                FLAG_HAPTIC_PLAY_IDX5 = 1;
                break;
            case '6':
                FLAG_HAPTIC_PLAY_IDX6 = 1;
                break;
            case '7':
                FLAG_HAPTIC_PLAY_IDX7 = 1;
                break;
            case '8':
                FLAG_HAPTIC_PLAY_IDX8 = 1;
                break;
            case '9':
                FLAG_HAPTIC_PLAY_IDX9 = 1;
                break;
            case 'A':
            case 'a':
                FLAG_HAPTIC_PLAY_IDX10 = 1;
                break;
            case 'B':
            case 'b':
                FLAG_HAPTIC_PLAY_IDX11 = 1;
                break;
            case 'C':
            case 'c':
                FLAG_HAPTIC_PLAY_IDX12 = 1;
                break;
            case 'D':
            case 'd':
                FLAG_HAPTIC_PLAY_IDX13 = 1;
                break;
            case 'E':
            case 'e':
                FLAG_HAPTIC_PLAY_IDX14 = 1;
                break;
            case 'F':
            case 'f':
                FLAG_HAPTIC_PLAY_IDX15 = 1;
                break;
        }
    }
}


void DA7280_playback(void)
{
    if (FLAG_HAPTIC_PLAY_IDX1)
    {
        DA7280_playback_idx(SEQUENCE_IDX1 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX1 = 0;
    }

    if (FLAG_HAPTIC_PLAY_IDX2)
    {
        DA7280_playback_idx(SEQUENCE_IDX2 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX2 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX3)
    {
        DA7280_playback_idx(SEQUENCE_IDX3 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX3 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX4)
    {
        DA7280_playback_idx(SEQUENCE_IDX4 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX4 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX5)
    {
        DA7280_playback_idx(SEQUENCE_IDX5 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX5 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX6)
    {
        DA7280_playback_idx(SEQUENCE_IDX6 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX6 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX7)
    {
        DA7280_playback_idx(SEQUENCE_IDX7 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX7 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX8)
    {
        DA7280_playback_idx(SEQUENCE_IDX8 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX8 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX9)
    {
        DA7280_playback_idx(SEQUENCE_IDX9 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX9 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX10)
    {
        DA7280_playback_idx(SEQUENCE_IDX10 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX10 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX11)
    {
        DA7280_playback_idx(SEQUENCE_IDX11 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX11 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX12)
    {
        DA7280_playback_idx(SEQUENCE_IDX12 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX12 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX13)
    {
        DA7280_playback_idx(SEQUENCE_IDX13 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX13 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX14)
    {
        DA7280_playback_idx(SEQUENCE_IDX14 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX14 = 0;
    }
    
    if (FLAG_HAPTIC_PLAY_IDX15)
    {
        DA7280_playback_idx(SEQUENCE_IDX15 , 0 ,0);
        FLAG_HAPTIC_PLAY_IDX15 = 0;
    }

}

void DA7280_Process(unsigned char key)
{
    
    if (FLAG_HAPTIC_TRIG_DA7280_FINISH)
    {
        FLAG_HAPTIC_TRIG_DA7280_FINISH = 0;
        DA7280_playback_finish_check();
        printf("DA7280 exec finish\r\n");
    }

    DA7280_key_filter(key);
    DA7280_playback();
}


void DA7280_nIRQ_Process_in_IRQ(void)
{
    FLAG_HAPTIC_TRIG_DA7280_FINISH = 1;
}

