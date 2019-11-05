
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_timer.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"

#include "inc/hw_sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/i2c.h"
#include "driverlib/uart.h"


#include "driverlib/rom.h"
#include "uc1701.h"

unsigned char Received_Voltage_Current_SetVoltage[15];
unsigned char Index_of_Rece_V_C_SetV = 0;

unsigned int Real_Voltage = 0;
unsigned int Current = 0;
unsigned int Set_Voltage = 0;
bool Finish_Data_Convert_Flag = 0;

void UART3IntHandler(void)
{
    unsigned int status = UARTIntStatus(UART3_BASE, true);

    UARTIntClear(UART3_BASE, status);

    while (UARTCharsAvail(UART3_BASE))
    {
        //UARTCharPutNonBlocking(UART3_BASE, UARTCharGetNonBlocking(UART3_BASE));
        unsigned char tmp = UARTCharGetNonBlocking(UART3_BASE); //BEAR IN MIND THAT "tmp" is a integer ASCII Code.
        UARTCharPutNonBlocking(UART3_BASE, tmp);
        while(UARTBusy(UART3_BASE));
        Received_Voltage_Current_SetVoltage[Index_of_Rece_V_C_SetV] = tmp;
        Index_of_Rece_V_C_SetV++;
        if(Index_of_Rece_V_C_SetV > 15){
            short i;
            Index_of_Rece_V_C_SetV = 0;
            Real_Voltage = 0;
            Current = 0;
            Set_Voltage = 0;
            for(i = 0; i < 5; i++){
                Real_Voltage += (Received_Voltage_Current_SetVoltage[i] - '0') * pow(10, 4 - i);   //18715
                Current += (Received_Voltage_Current_SetVoltage[i + 5] - '0') * pow(10, 4 - i);   //09159
                Set_Voltage += (Received_Voltage_Current_SetVoltage[i + 10] - '0') * pow(10, 4 - i);   //18715
            }
            Finish_Data_Convert_Flag = 1;
        }

    }
}

void main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    GPIOPinConfigure(GPIO_PC6_U3RX);
    GPIOPinConfigure(GPIO_PC7_U3TX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);


    UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)); //8个数据位，1个停止位， 无校验位

    IntEnable(INT_UART3);
    UARTIntEnable(UART3_BASE, UART_INT_RX | UART_INT_RT);
    UARTIntRegister(UART3_BASE, UART3IntHandler);
    IntMasterEnable();

    //UC1701Init(60000);
    //UC1701Clear();
    PortFunctionInit();
    lcd_init();
    lcd_printf(1, 0, Received_Voltage_Current_SetVoltage);
    while (1)
    {
        while(UARTBusy(UART3_BASE));

        //UC1701DisplayN(0 , 1 , Real_Voltage);
        //UC1701DisplayN(1 , 1 , Current);
        //UC1701DisplayN(2 , 1 , Set_Voltage);

    }

}

