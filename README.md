# RL78_F24_I2C_DA7280
 RL78_F24_I2C_DA7280



udpate @ 2024/03/25

1. press F24 EVB button over 2.5 sec , for entry demo mode (play seq 0 ~ 3)

udpate @ 2024/03/08

1. use smart config and define SCL , SDA , IRQ pin for DA7280

	- SCL : P71
	
	- SDA : P70
	
	- nIRQ : P77(INTP12)
	
2. reference sample code , 

https://www.renesas.com/us/en/document/sws/us082-13-da7280evz-software-package?r=1601491


3. general flow (refer to sample and REN_da7280_datasheet_3v0_DST_20220608_1)

initial flow will be

- DA7280_Open

![image](https://github.com/released/RL78_F24_I2C_DA7280/blob/main/DA7280_Open.jpg)


- DA7280_SetFrequencyTracking

![image](https://github.com/released/RL78_F24_I2C_DA7280/blob/main/DA7280_SetFrequencyTracking_1.jpg)

![image](https://github.com/released/RL78_F24_I2C_DA7280/blob/main/DA7280_SetFrequencyTracking_2.jpg)


- DA7280_SetMode

![image](https://github.com/released/RL78_F24_I2C_DA7280/blob/main/DA7280_SetMode_1.jpg)

![image](https://github.com/released/RL78_F24_I2C_DA7280/blob/main/DA7280_SetMode_2.jpg)


- DA7280_WriteWaveform

![image](https://github.com/released/RL78_F24_I2C_DA7280/blob/main/DA7280_WriteWaveform_1.jpg)

![image](https://github.com/released/RL78_F24_I2C_DA7280/blob/main/DA7280_WriteWaveform_2.jpg)


when playback ,use 

- DA7280_Set_PS_SEQ_ID_LOOP

![image](https://github.com/released/RL78_F24_I2C_DA7280/blob/main/step1_DA7280_Set_PS_SEQ_ID_LOOP.jpg)


- DA7280_SetPlayBackMemory

![image](https://github.com/released/RL78_F24_I2C_DA7280/blob/main/step2_DA7280_SetPlayBackMemory.jpg)


- DA7280_SetContinueSequence

![image](https://github.com/released/RL78_F24_I2C_DA7280/blob/main/step3_DA7280_SetContinueSequence.jpg)


when playback finish , need to check nIRQ and check status 

![image](https://github.com/released/RL78_F24_I2C_DA7280/blob/main/DA7280_playback_finish_check.jpg)


- DA7280_CheckIRQEvent1

![image](https://github.com/released/RL78_F24_I2C_DA7280/blob/main/DA7280_CheckIRQEvent1.jpg)


- DA7280_SetIRQEvent1

![image](https://github.com/released/RL78_F24_I2C_DA7280/blob/main/DA7280_SetIRQEvent1.jpg)


4. below is waveform when play index , 

index 1

![image](https://github.com/released/RL78_F24_I2C_DA7280/blob/main/scope_idx_1.jpg)

index 2

![image](https://github.com/released/RL78_F24_I2C_DA7280/blob/main/scope_idx_2.jpg)

index 3

![image](https://github.com/released/RL78_F24_I2C_DA7280/blob/main/scope_idx_3.jpg)

5. 
below is log capture

![image](https://github.com/released/RL78_F24_I2C_DA7280/blob/main/log_init.jpg)


![image](https://github.com/released/RL78_F24_I2C_DA7280/blob/main/log_index1.jpg)

