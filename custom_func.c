/*_____ I N C L U D E S ____________________________________________________*/
#include <stdio.h>

#include "inc_main.h"

#include "misc_config.h"
#include "custom_func.h"

/*_____ D E C L A R A T I O N S ____________________________________________*/

struct flag_32bit flag_PROJ_CTL;
#define FLAG_PROJ_TIMER_PERIOD_1000MS                 	(flag_PROJ_CTL.bit0)
#define FLAG_PROJ_TRIG_BTN1                       	    (flag_PROJ_CTL.bit1)
#define FLAG_PROJ_TRIG_BTN2                 	        (flag_PROJ_CTL.bit2)
#define FLAG_PROJ_DEMO_MODE                             (flag_PROJ_CTL.bit3)
#define FLAG_PROJ_REVERSE4                              (flag_PROJ_CTL.bit4)
#define FLAG_PROJ_REVERSE5                              (flag_PROJ_CTL.bit5)
#define FLAG_PROJ_REVERSE6                              (flag_PROJ_CTL.bit6)
#define FLAG_PROJ_REVERSE7                              (flag_PROJ_CTL.bit7)


/*_____ D E F I N I T I O N S ______________________________________________*/

volatile unsigned int counter_tick = 0;
volatile unsigned int btn_counter_tick = 0;

unsigned char key_buffer = 0;
unsigned char flag_play = 0;

#define BTN_PRESSED_LONG                                (2500)

/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/

void set_TIMER_PERIOD_1000MS(void)
{
    FLAG_PROJ_TIMER_PERIOD_1000MS = 1;
}

void reset_TIMER_PERIOD_1000MS(void)
{
    FLAG_PROJ_TIMER_PERIOD_1000MS = 0;
}

bool Is_TIMER_PERIOD_1000MS_Trig(void)
{
    return FLAG_PROJ_TIMER_PERIOD_1000MS;
}

unsigned int btn_get_tick(void)
{
	return (btn_counter_tick);
}

void btn_set_tick(unsigned int t)
{
	btn_counter_tick = t;
}

void btn_tick_counter(void)
{
	btn_counter_tick++;
    if (btn_get_tick() >= 60000)
    {
        btn_set_tick(0);
    }
}

unsigned int get_tick(void)
{
	return (counter_tick);
}

void set_tick(unsigned int t)
{
	counter_tick = t;
}

void tick_counter(void)
{
	counter_tick++;
    if (get_tick() >= 60000)
    {
        set_tick(0);
    }
}

void delay_ms(unsigned int ms)
{
	#if 1
    unsigned int tickstart = get_tick();
    unsigned int wait = ms;
	unsigned int tmp = 0;
	
    while (1)
    {
		if (get_tick() > tickstart)	// tickstart = 59000 , tick_counter = 60000
		{
			tmp = get_tick() - tickstart;
		}
		else // tickstart = 59000 , tick_counter = 2048
		{
			tmp = 60000 -  tickstart + get_tick();
		}		
		
		if (tmp > wait)
			break;
    }
	
	#else
	TIMER_Delay(TIMER0, 1000*ms);
	#endif
}

void simple_test(void)
{

}

void Timer_1ms_IRQ(void)
{
    tick_counter();

    if ((get_tick() % 1000) == 0)
    {
        set_TIMER_PERIOD_1000MS();
    }

    if ((get_tick() % 2000) == 0)
    {
        DA7280_set_TIMER_PERIOD();
    }	

    if ((get_tick() % 50) == 0)
    {

    }	

    Button_Process_long_counter();
}


/*
    F24 target board
    LED1 connected to P66, LED2 connected to P67
*/
void LED_Toggle(void)
{
    PIN_WRITE(6,6) = ~PIN_READ(6,6);
    PIN_WRITE(6,7) = ~PIN_READ(6,7);
}

void loop(void)
{
	// static unsigned int LOG1 = 0;

    if (Is_TIMER_PERIOD_1000MS_Trig())
    {
        reset_TIMER_PERIOD_1000MS();

        // printf("log(timer):%4d\r\n",LOG1++);
        LED_Toggle();             
    }

    Button_Process_in_polling();

    DA7280_Process(key_buffer);
}

// F24 EVB , P137/INTP0 , set both edge 
void Button_Process_long_counter(void)
{
    if (FLAG_PROJ_TRIG_BTN2)
    {
        btn_tick_counter();
    }
    else
    {
        btn_set_tick(0);
    }
}

void Button_Process_in_polling(void)
{
    static unsigned char cnt = 0;

    if (FLAG_PROJ_TRIG_BTN1)
    {
        FLAG_PROJ_TRIG_BTN1 = 0;
        printf("BTN pressed(%d)\r\n",cnt);

        if (cnt == 0)
        {
            FLAG_PROJ_TRIG_BTN2 = 1;
        }
        else if (cnt == 1)
        {
            FLAG_PROJ_TRIG_BTN2 = 0;
        }

        cnt = (cnt >= 1) ? (0) : (cnt+1) ;
    }

    if ((FLAG_PROJ_TRIG_BTN2 == 1) && 
        (btn_get_tick() > BTN_PRESSED_LONG))
    {         
        printf("entry demo mode(%d)\r\n" , FLAG_PROJ_DEMO_MODE);
        btn_set_tick(0);
        /*
            FLAG_PROJ_DEMO_MODE
            0 : //first trig , enable demo mode
            1 : //sec trig , disable demo mode
        */
        DA7280_demo_enable(FLAG_PROJ_DEMO_MODE);   

        FLAG_PROJ_DEMO_MODE = ~FLAG_PROJ_DEMO_MODE;
    }
}

void Button_Process_in_IRQ(void)    
{
    FLAG_PROJ_TRIG_BTN1 = 1;
}

void UARTx_Process(unsigned char rxbuf)
{    
    if (rxbuf > 0x7F)
    {
        printf("invalid command\r\n");
    }
    else
    {
        printf("press:%c(0x%02X)\r\n" , rxbuf,rxbuf);   // %c :  C99 libraries.
        switch(rxbuf)
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'A':
            case 'a':
            case 'B':
            case 'b':
            case 'C':
            case 'c':
            case 'D':
            case 'd':
            case 'E':
            case 'e':
            case 'F':
            case 'f':
                key_buffer = rxbuf;
                flag_play = 1;
                break;

            case 'X':
            case 'x':
            case 'Z':
            case 'z':
                break;
        }
    }
}

// retarget printf
int __far putchar(int c)
{
    // F24 , UART0
    STMK0 = 1U;    /* disable INTST0 interrupt */
    SDR00L = (unsigned char)c;
    while(STIF0 == 0)
    {

    }
    STIF0 = 0U;    /* clear INTST0 interrupt flag */
    return c;
}

void hardware_init(void)
{
    BSP_EI();
    R_Config_UART0_Start();
    R_Config_TAU0_1_Start();  
    R_Config_INTC_INTP0_Start();
    R_Config_INTC_INTP12_Start();   // DA7280 nIRQ

    // simple_test();
    DA7280_init();
}
