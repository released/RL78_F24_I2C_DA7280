/*_____ I N C L U D E S ____________________________________________________*/

/*_____ D E C L A R A T I O N S ____________________________________________*/

/*_____ D E F I N I T I O N S ______________________________________________*/

/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/
extern unsigned char flag_play;

void set_TIMER_PERIOD_1000MS(void);
void reset_TIMER_PERIOD_1000MS(void);
unsigned char Is_TIMER_PERIOD_1000MS_Trig(void);
void Timer_1ms_IRQ(void);

void drv_I2C_callback_error(unsigned char err);
void drv_set_I2C_receive_flag(bool flag);
bool drv_get_I2C_receive_flag(void);
void drv_set_I2C_send_flag(bool flag);
bool drv_get_I2C_send_flag(void);


void DA7280_forceStop(void);
void DA7280_playback_finish_check(void);
void DA7280_playback_idx(unsigned char idx , unsigned char loop , unsigned char en);
void DA7280_init(void);
void DA7280_key_filter(unsigned char key);
void DA7280_playback(void);
void DA7280_Process(unsigned char key);
void DA7280_nIRQ_Process_in_IRQ(void);

void Button_Process_in_IRQ(void);
void UARTx_Process(unsigned char rxbuf);

void LED_Toggle(void);
void loop(void);
void hardware_init(void);
