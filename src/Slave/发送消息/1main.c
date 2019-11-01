#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/adc.h"

#define Baud_Rate 115200
#define STOP_BIT '\n'

bool flag;
volatile uint32_t pui32ADC0Value[8],sum;
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
void ADC0Sequence0Handler(void)
{
    uint16_t i;
    ADCIntClear(ADC0_BASE, 0);
    ADCSequenceDataGet(ADC0_BASE, 0, pui32ADC0Value);

    for(i = 0; i < 8;i ++)
        sum = sum + (pui32ADC0Value[i] * 3300/4096);

    flag=1;
}
void ConfigUART0(uint32_t BaudRate){

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//A0, A1 for RX & TX

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), BaudRate, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)); //8个数据位，1个停止位， 无校验位
}
void ConfigUART0Interrupt(void (*UART0IntHandler)(void)){

    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    UARTIntRegister(UART0_BASE, UART0IntHandler);
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
void ConfigADC0(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1);
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);

    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH2 );
    ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH2 );
    ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH2 );
    ADCSequenceStepConfigure(ADC0_BASE, 0, 3, ADC_CTL_CH2 );
    ADCSequenceStepConfigure(ADC0_BASE, 0, 4, ADC_CTL_CH2 );
    ADCSequenceStepConfigure(ADC0_BASE, 0, 5, ADC_CTL_CH2 );
    ADCSequenceStepConfigure(ADC0_BASE, 0, 6, ADC_CTL_CH2 );
    ADCSequenceStepConfigure(ADC0_BASE, 0, 7, ADC_CTL_CH2 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 0);
}
void ConfigADC0Interrupt(){
    ADCIntRegister(ADC0_BASE, 0, ADC0Sequence0Handler);
    ADCIntEnable(ADC0_BASE, 0);
    IntEnable(INT_ADC0SS0);
}
void StartAllInterrupt(){
    IntMasterEnable();
}
void main(void)
{

    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    ConfigUART0(Baud_Rate);
    ConfigADC0();
    ConfigADC0Interrupt();
    StartAllInterrupt();
    while (1)
    {
        ADCIntClear(ADC0_BASE, 0);
        ADCProcessorTrigger(ADC0_BASE, 0);
        if(flag == 1){
            Analog_Voltage = sum / 8;
            sum = 0;
            flag = 0;
        }

        Voltage_Data_Break(Analog_Voltage, M_to_L_Bit_of_Analog_Voltage);
        Sent_Voltage(UART0_BASE, M_to_L_Bit_of_Analog_Voltage);
        SysCtlDelay(SysCtlClockGet() / 100);


    }

}

