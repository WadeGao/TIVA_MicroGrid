#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

#define Baud_Rate 115200
#define STOP_BIT '\n'
volatile unsigned int Analog_Voltage;
volatile unsigned char M_to_L_Bit_of_Analog_Voltage[5];
void UART0IntHandler(void)
{
    unsigned int status = UARTIntStatus(UART0_BASE, true);

    UARTIntClear(UART0_BASE, status);

    while (UARTCharsAvail(UART0_BASE))
    {
        UARTCharPutNonBlocking(UART0_BASE, UARTCharGetNonBlocking(UART0_BASE));
        /*unsigned int tmp = UARTCharGetNonBlocking(UART0_BASE); //BEAR IN MIND THAT "tmp" is a integer ASCII Code.*/
        //UARTCharPutNonBlocking(UART0_BASE, tmp);
        /*if(tmp > 65 + 10)
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3));*/

    }
}

void ConfigUART0(uint32_t BaudRate){

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//A0, A1 for RX & TX

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), BaudRate, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)); //8个数据位，1个停止位， 无校验位
}
void ConfigUART0Interrupt(void (*pfnHandler)(void)){

    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    UARTIntRegister(UART0_BASE, pfnHandler);
    IntMasterEnable();
}
void Voltage_Data_Break(unsigned int Analog_Voltage, unsigned char M_to_L_Bit_of_Analog_Voltage[]){
    //13925
    M_to_L_Bit_of_Analog_Voltage[0] = Analog_Voltage / 10000 + '0';       //1
    M_to_L_Bit_of_Analog_Voltage[1] = (Analog_Voltage / 1000) % 10 + '0'; //3
    M_to_L_Bit_of_Analog_Voltage[2] = (Analog_Voltage / 100) % 10 + '0';  //9
    M_to_L_Bit_of_Analog_Voltage[3] = (Analog_Voltage / 10) % 10 + '0';   //2
    M_to_L_Bit_of_Analog_Voltage[4] = Analog_Voltage % 10 + '0';          //5
}
void Sent_Voltage(uint32_t ui32Base, unsigned char M_to_L_Bit_of_Analog_Voltage[]){
    short index = 0;
    for(index = 0; index < 5; index++){
        while(UARTBusy(ui32Base));
        UARTCharPut(ui32Base, M_to_L_Bit_of_Analog_Voltage[index]);
        while(UARTBusy(ui32Base));
    }
    UARTCharPut(ui32Base, STOP_BIT);
}
void main(void)
{

    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    ConfigUART0(Baud_Rate);
    ConfigUART0Interrupt(UART0IntHandler);
    Analog_Voltage = 1;
    while (1)
    {
        Analog_Voltage++;

        Voltage_Data_Break(Analog_Voltage, M_to_L_Bit_of_Analog_Voltage);
        Sent_Voltage(UART0_BASE, M_to_L_Bit_of_Analog_Voltage);
        SysCtlDelay(SysCtlClockGet() / 5);
    }
}

