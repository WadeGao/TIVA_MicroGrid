#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

#define INIT_VALUE 0

unsigned short LEN = 0;
unsigned short Inte_Count = 0;
unsigned short Decm_Count = 0;
unsigned short Dot_Count = 0;
volatile float result = 0.00;



void Reset(){
    LEN = INIT_VALUE;
    Inte_Count = INIT_VALUE;
    Decm_Count = INIT_VALUE;
    Dot_Count = INIT_VALUE;
    result = INIT_VALUE;
}
void UART3IntHandler(void)
{
    unsigned int status = UARTIntStatus(UART3_BASE, true);

    UARTIntClear(UART3_BASE, status);

    while (UARTCharsAvail(UART3_BASE))
    {
        //UARTCharPutNonBlocking(UART3_BASE, UARTCharGetNonBlocking(UART3_BASE));
        unsigned char tmp = UARTCharGetNonBlocking(UART3_BASE); //BEAR IN MIND THAT "tmp" is a integer ASCII Code.
        
        if (LEN == 4){
            if(result >= 10.24)
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3));
            Reset();
            break;
        }
        if((tmp > '9' || tmp < '0' ) && (tmp != '.')){        //Invalid Character except '0'~'9' or '.'
            Reset();
            break;
        }
        if(Dot_Count == 0 && ((tmp <= '9' && tmp >= '0') == 1))
            Inte_Count++;
        
        if(tmp == '.'){
            Decimal_STATUS = 1;
            Dot_Count++;
            if(Dot_Count >= 2)  //27..
                Reset();
            if(Inte_Count < 2)  //6.72
                Reset();
            break;
        }
        
        if(tmp <= '9' && tmp >='0'){
            LEN++;
        }
        
        result += (tmp - '0') * pow(10, 2 - LEN);
        LEN++;
     
        

    }
}
void main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    GPIOPinConfigure(GPIO_PC6_U3RX);
    GPIOPinConfigure(GPIO_PC7_U3TX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)); //8个数据位，1个停止位， 无校验位

    IntEnable(INT_UART3);
    UARTIntEnable(UART3_BASE, UART_INT_RX | UART_INT_RT);
    UARTIntRegister(UART3_BASE, UART3IntHandler);
    IntMasterEnable();
    while (1)
    {
        while(UARTBusy(UART3_BASE));
        //UARTCharPut(UART3_BASE, (int)DATA % 10 + '0');

    }

}

