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
#include "driverlib/rom_map.h"
#include "inc/hw_gpio.h"


#define Baud_Rate 9600
#define STOP_BIT '\n'

#define D2 GPIO_PIN_0   //F // Need to be Unlocked
#define D3 GPIO_PIN_4   //A // Need to be Unlocked
#define D4 GPIO_PIN_6   //D // Need to be Unlocked

#define K1 GPIO_PIN_7   //D // Need to be Unlocked
#define K2 GPIO_PIN_4   //F // Need to be Unlocked
#define K3 GPIO_PIN_3   //A // Need to be Unlocked
#define K4 GPIO_PIN_2   //A // Need to be Unlocked


bool Vflag;
bool Aflag;
unsigned char Voltage_Current_SetVoltage[15];


uint32_t Analog_Voltage_pui32ADC0Value[8], Analog_Voltage_Sum;
unsigned int Analog_Voltage;
unsigned char M_to_L_Bit_of_Analog_Voltage[5];

uint32_t Current_pui32ADC0Value[8], Current_Sum;
unsigned int Current;
unsigned char M_to_L_Bit_of_Current[5];

volatile static double Set_Voltage = 5.0;
unsigned char M_to_L_Bit_of_Set_Voltage[5];
void UART0IntHandler(void)
{
    unsigned int status = UARTIntStatus(UART0_BASE, true);
    UARTIntClear(UART0_BASE, status);
    while (UARTCharsAvail(UART0_BASE))
        UARTCharPutNonBlocking(UART0_BASE, UARTCharGetNonBlocking(UART0_BASE));
}
void ADC0Sequence0Handler(void)
{
    uint16_t i;
    ADCIntClear(ADC0_BASE, 0);
    ADCIntClear(ADC1_BASE, 0);
    ADCSequenceDataGet(ADC0_BASE, 0, Analog_Voltage_pui32ADC0Value);
    for(i = 0; i < 8;i ++){
        Analog_Voltage_Sum = Analog_Voltage_Sum + (Analog_Voltage_pui32ADC0Value[i] * 3300 / 4096);
    }
    Vflag=1;
}
void ADC1Sequence0Handler(void)
{
    uint16_t i;
    ADCIntClear(ADC0_BASE, 0);
    ADCIntClear(ADC1_BASE, 0);
    ADCSequenceDataGet(ADC1_BASE, 0, Current_pui32ADC0Value);
    for(i = 0; i < 8;i ++){
        Current_Sum = Current_Sum + (Current_pui32ADC0Value[i] * 3300 / 4096);
    }
    Aflag=1;
}
void ConfigUART3(uint32_t BaudRate){

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//A0, A1 for RX & TX
    GPIOPinConfigure(GPIO_PC6_U3RX);
    GPIOPinConfigure(GPIO_PC7_U3TX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), BaudRate, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)); //8个数据位，1个停止位， 无校验位
}
void ConfigUART0(uint32_t BaudRate){

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//A0, A1 for RX & TX
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_1 | GPIO_PIN_0);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), BaudRate, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)); //8个数据位，1个停止位， 无校验位
}
void ConfigUART0Interrupt(void (*UART0IntHandler)(void)){

    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    UARTIntRegister(UART0_BASE, UART0IntHandler);
}
void Data_Break(unsigned int Analog_Voltage, unsigned int Current, unsigned int Set_Voltage, unsigned char M_to_L_Bit_of_Analog_Voltage[], unsigned char M_to_L_Bit_of_Current[], unsigned char M_to_L_Bit_of_Set_Voltage[]){
    ADCIntClear(ADC0_BASE, 0);
    ADCIntClear(ADC1_BASE, 0);
    //13925
    M_to_L_Bit_of_Analog_Voltage[0] = Analog_Voltage / 10000 + '0';       //1
    M_to_L_Bit_of_Analog_Voltage[1] = (Analog_Voltage / 1000) % 10 + '0'; //3
    M_to_L_Bit_of_Analog_Voltage[2] = (Analog_Voltage / 100) % 10 + '0';  //9
    M_to_L_Bit_of_Analog_Voltage[3] = (Analog_Voltage / 10) % 10 + '0';   //2
    M_to_L_Bit_of_Analog_Voltage[4] = Analog_Voltage % 10 + '0';          //5

    //Current
    M_to_L_Bit_of_Current[0] = Current / 10000 + '0';       //1
    M_to_L_Bit_of_Current[1] = (Current / 1000) % 10 + '0'; //3
    M_to_L_Bit_of_Current[2] = (Current / 100) % 10 + '0';  //9
    M_to_L_Bit_of_Current[3] = (Current / 10) % 10 + '0';   //2
    M_to_L_Bit_of_Current[4] = Current % 10 + '0';          //5


    M_to_L_Bit_of_Set_Voltage[0] = Set_Voltage / 10000 + '0';
    M_to_L_Bit_of_Set_Voltage[1] = (Set_Voltage / 1000) % 10 + '0'; //3
    M_to_L_Bit_of_Set_Voltage[2] = (Set_Voltage / 100) % 10 + '0';  //9
    M_to_L_Bit_of_Set_Voltage[3] = (Set_Voltage / 10) % 10 + '0';   //2
    M_to_L_Bit_of_Set_Voltage[4] = Set_Voltage % 10 + '0';          //5

    uint32_t index;
    for(index = 0; index < 5; index++){
        Voltage_Current_SetVoltage[index] = M_to_L_Bit_of_Analog_Voltage[index];
        //Voltage_Current_SetVoltage[index + 5] = M_to_L_Bit_of_Current[index];
        //Voltage_Current_SetVoltage[index + 10] = M_to_L_Bit_of_Set_Voltage[index];
    }
    for(index = 5; index < 10; index++){
            //Voltage_Current_SetVoltage[index] = M_to_L_Bit_of_Analog_Voltage[index];
            Voltage_Current_SetVoltage[index] = M_to_L_Bit_of_Current[index - 5];
            //Voltage_Current_SetVoltage[index + 10] = M_to_L_Bit_of_Set_Voltage[index];
        }
    for(index = 10; index < 15; index++){
            //Voltage_Current_SetVoltage[index] = M_to_L_Bit_of_Analog_Voltage[index];
            //Voltage_Current_SetVoltage[index + 5] = M_to_L_Bit_of_Current[index];
            Voltage_Current_SetVoltage[index] = M_to_L_Bit_of_Set_Voltage[index - 10];
        }
}
void Sent_Voltage(uint32_t ui32Base, unsigned char Voltage_Current_SetVoltage[]){

    ADCIntClear(ADC0_BASE, 0);
    ADCIntClear(ADC1_BASE, 0);
    int index = 0;
    for(index = 0; index < 15; index++){
        while(UARTBusy(ui32Base));
        UARTCharPut(ui32Base, Voltage_Current_SetVoltage[index]);
        UARTCharPut(UART0_BASE, Voltage_Current_SetVoltage[index]);
    }
    UARTCharPut(ui32Base, STOP_BIT);
}
void ConfigADC0(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1);
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    // PE1: AIN2
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
void ConfigADC1(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0);
    ADCSequenceConfigure(ADC1_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    // PE0: AIN3
    ADCSequenceStepConfigure(ADC1_BASE, 0, 0, ADC_CTL_CH3 );
    ADCSequenceStepConfigure(ADC1_BASE, 0, 1, ADC_CTL_CH3 );
    ADCSequenceStepConfigure(ADC1_BASE, 0, 2, ADC_CTL_CH3 );
    ADCSequenceStepConfigure(ADC1_BASE, 0, 3, ADC_CTL_CH3 );
    ADCSequenceStepConfigure(ADC1_BASE, 0, 4, ADC_CTL_CH3 );
    ADCSequenceStepConfigure(ADC1_BASE, 0, 5, ADC_CTL_CH3 );
    ADCSequenceStepConfigure(ADC1_BASE, 0, 6, ADC_CTL_CH3 );
    ADCSequenceStepConfigure(ADC1_BASE, 0, 7, ADC_CTL_CH3 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC1_BASE, 0);
}
void ConfigADC0Interrupt(){
    ADCIntRegister(ADC0_BASE, 0, ADC0Sequence0Handler);
    ADCIntEnable(ADC0_BASE, 0);
    IntEnable(INT_ADC0SS0);
}
void ConfigADC1Interrupt(){
    ADCIntRegister(ADC1_BASE, 0, ADC1Sequence0Handler);
    ADCIntEnable(ADC1_BASE, 0);
    IntEnable(INT_ADC1SS0);
}
void StartAllInterrupt(){
    IntMasterEnable();
}


/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/
/*********************************中断函数模块*********************************************/
/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/
/*K1按下触发中断，反转D1*/
void SwitchK1IntHandler(void){
    ADCIntClear(ADC0_BASE, 0);
    ADCIntClear(ADC1_BASE, 0);
    GPIOIntClear(GPIO_PORTD_BASE, K1);      //清除中断
    SysCtlDelay(3e5);
    if(GPIOPinRead(GPIO_PORTD_BASE, K1) == 0){      //软件消抖
        while(GPIOPinRead(GPIO_PORTD_BASE, K1) == 0);
        if(Set_Voltage + 1.0 <= 10.0)
            Set_Voltage += 1.0;
        else
            Set_Voltage = 10.0;
    }
}
/*K2按下触发中断，反转D2*/
void SwitchK2IntHandler(void){
    ADCIntClear(ADC0_BASE, 0);
    ADCIntClear(ADC1_BASE, 0);
    GPIOIntClear(GPIO_PORTF_BASE, K2);      //清除中断
    SysCtlDelay(3e5);
    if(GPIOPinRead(GPIO_PORTF_BASE, K2) == 0){      //软件消抖
        while(GPIOPinRead(GPIO_PORTF_BASE, K2) == 0);
        if(Set_Voltage - 1.0 >= 3.3)
            Set_Voltage -= 1.0;
        else
            Set_Voltage = 3.3;
    }
}
/*函数选择是K3还是K4*/
void ChoiceGPIO_PORTA_PIN_x_Int_Server(void){
    ADCIntClear(ADC0_BASE, 0);
    ADCIntClear(ADC1_BASE, 0);
    unsigned long ulStatus = GPIOIntStatus(GPIO_PORTA_BASE, false);

    /*K4按下触发中断，反转D4*/
    if(ulStatus & K4){
        GPIOIntClear(GPIO_PORTA_BASE, K4);
        SysCtlDelay(3e5);
        if(GPIOPinRead(GPIO_PORTA_BASE, K4) == 0){
            while(GPIOPinRead(GPIO_PORTA_BASE, K4) == 0);
            if(Set_Voltage - 0.1 >= 3.3)
                Set_Voltage -= 0.1;
            else
                Set_Voltage = 3.3;
        }
    }
    /*K3按下触发中断*/
    else if(ulStatus & K3){
        GPIOIntClear(GPIO_PORTA_BASE, K3);      //清除中断
        SysCtlDelay(3e5);
        if(GPIOPinRead(GPIO_PORTA_BASE, K3) == 0){      //软件消抖
            while(GPIOPinRead(GPIO_PORTA_BASE, K3) == 0);
            if(Set_Voltage + 0.1 <= 10.0)
                Set_Voltage += 0.1;
            else
                Set_Voltage = 10.0;
        }
    }
}
/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/
/*********************************中断函数模块*********************************************/
/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/




/***************************************************************************************/
/***************************************************************************************/
/**********************************解锁模块**********************************************/
/***************************************************************************************/
/***************************************************************************************/
/*解锁K1*/
void UnlockK1(void){
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;
    GPIODirModeSet(GPIO_PORTD_BASE, K1, GPIO_DIR_MODE_IN);
    MAP_GPIOPadConfigSet(GPIO_PORTD_BASE, K1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}
/*解锁D2*/
void UnlockD2(void){
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_DIR_MODE_OUT);
    MAP_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}
/***************************************************************************************/
/***************************************************************************************/
/**********************************解锁模块**********************************************/
/***************************************************************************************/
/***************************************************************************************/


/***************************************************************************************/
/***************************************************************************************/
/**********************************配置按键**********************************************/
/***************************************************************************************/
/***************************************************************************************/
void KeyInit(void){

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
}

void GPIOInterruptConfigure(void){

    GPIOIntEnable(GPIO_PORTD_BASE, K1);
    GPIOIntEnable(GPIO_PORTF_BASE, K2);
    GPIOIntEnable(GPIO_PORTA_BASE, K3);
    GPIOIntEnable(GPIO_PORTA_BASE, K4);

    GPIOIntTypeSet(GPIO_PORTD_BASE, K1, GPIO_FALLING_EDGE); //下降沿触发
    GPIOIntTypeSet(GPIO_PORTF_BASE, K2, GPIO_FALLING_EDGE); //下降沿触发
    GPIOIntTypeSet(GPIO_PORTA_BASE, K3, GPIO_FALLING_EDGE); //下降沿触发
    GPIOIntTypeSet(GPIO_PORTA_BASE, K4, GPIO_FALLING_EDGE); //下降沿触发

    GPIOIntRegister(GPIO_PORTD_BASE, SwitchK1IntHandler);
    GPIOIntRegister(GPIO_PORTF_BASE, SwitchK2IntHandler);
    GPIOIntRegister(GPIO_PORTA_BASE, ChoiceGPIO_PORTA_PIN_x_Int_Server);

    IntEnable(INT_GPIOA);
    IntEnable(INT_GPIOF);
    IntEnable(INT_GPIOD);
}

void GPIOInit(void){
    /*输出用的GPIO*/
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, D2);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, D3);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, D4);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    /*输入用的GPIO*/
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, K1);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, K2);
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, K3);
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, K4);
    /*LED初始化*/
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0b0000);
    GPIOPinWrite(GPIO_PORTF_BASE, D2, D2);
    GPIOPinWrite(GPIO_PORTA_BASE, D3, D3);
    GPIOPinWrite(GPIO_PORTD_BASE, D4, D4);
}
/***************************************************************************************/
/***************************************************************************************/
/**********************************配置按键**********************************************/
/***************************************************************************************/
/***************************************************************************************/





/***************************************************************************************/
/***************************************************************************************/
/**********************************显示模块**********************************************/
/***************************************************************************************/
/***************************************************************************************/
void DisplaySet_Voltage(){
    ADCIntClear(ADC0_BASE, 0);
    ADCIntClear(ADC1_BASE, 0);
    if(Set_Voltage == 10.0){
        lcd_printf(9, 0, "10.0 V");
    }
    else{
        lcd_printf(9, 0, " ");

        unsigned char tmp[1];

        tmp[0] = (int)Set_Voltage + '0';
        lcd_printf(10, 0, tmp);

        lcd_printf(11, 0, ".");

        tmp[0] = (int)(Set_Voltage * 10) % 10 + '0';
        lcd_printf(12, 0, tmp);

        lcd_printf(13, 0, " V");
    }
}
void DisplayVoltage(){
    ADCIntClear(ADC0_BASE, 0);
    ADCIntClear(ADC1_BASE, 0);
    if(Analog_Voltage >= 10000){
        unsigned char tmp[1];

        tmp[0] = Analog_Voltage / 10000 + '0';
        lcd_printf(7, 1, tmp);

        tmp[0] = (Analog_Voltage / 1000) % 10 + '0';
        lcd_printf(8, 1, tmp);

        lcd_printf(9, 1, ".");

        tmp[0] = (Analog_Voltage / 100) % 10 + '0';
        lcd_printf(10, 1, tmp);

        tmp[0] = (Analog_Voltage / 10) % 10 + '0';
        lcd_printf(11, 1, tmp);

        tmp[0] = Analog_Voltage % 10 + '0';
        lcd_printf(12, 1, tmp);

        lcd_printf(13, 1, " V");
    }
    else{
        lcd_printf(7, 1, " ");
        unsigned char tmp[1];

        tmp[0] = Analog_Voltage / 1000 + '0';
        lcd_printf(8, 1, tmp);


        lcd_printf(9, 1, ".");

        tmp[0] = (Analog_Voltage / 100) % 10 + '0';
        lcd_printf(10, 1, tmp);

        tmp[0] = (Analog_Voltage / 10) % 10 + '0';
        lcd_printf(11, 1, tmp);

        tmp[0] = Analog_Voltage % 10 + '0';
        lcd_printf(12, 1, tmp);

        lcd_printf(13, 1, " V");
    }
}
void DisplayCurrent(){
    ADCIntClear(ADC0_BASE, 0);
    ADCIntClear(ADC1_BASE, 0);
    if(Current >= 10000){
        unsigned char tmp[1];

        tmp[0] = Current / 10000 + '0';
        lcd_printf(7, 2, tmp);

        tmp[0] = (Current / 1000) % 10 + '0';
        lcd_printf(8, 2, tmp);

        lcd_printf(9, 2, ".");

        tmp[0] = (Current / 100) % 10 + '0';
        lcd_printf(10, 2, tmp);

        tmp[0] = (Current / 10) % 10 + '0';
        lcd_printf(11, 2, tmp);

        tmp[0] = Current % 10 + '0';
        lcd_printf(12, 2, tmp);

        lcd_printf(13, 2, " A");
    }
    else{
        lcd_printf(7, 2, " ");
        unsigned char tmp[1];

        tmp[0] = Current / 1000 + '0';
        lcd_printf(8, 2, tmp);


        lcd_printf(9, 2, ".");

        tmp[0] = (Current / 100) % 10 + '0';
        lcd_printf(10, 2, tmp);

        tmp[0] = (Current / 10) % 10 + '0';
        lcd_printf(11, 2, tmp);

        tmp[0] = Current % 10 + '0';
        lcd_printf(12, 2, tmp);

        lcd_printf(13, 2, " A");
    }
}
/***************************************************************************************/
/***************************************************************************************/
/**********************************配置按键**********************************************/
/***************************************************************************************/
/***************************************************************************************/
void main(void)
{

    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    ConfigADC0();
    ConfigADC0Interrupt();
    ConfigADC1();
    ConfigADC1Interrupt();


    KeyInit();
    GPIOInterruptConfigure();
    GPIOInit();
    UnlockK1();
    UnlockD2();

    PortFunctionInit();
    lcd_init();
    lcd_printf(1, 0, "SetU: ");
    lcd_printf(1, 1, "DC_U: ");
    lcd_printf(1, 2, "DC_I: ");

    lcd_printf(1, 3, "DC_P: ");

    //lcd_printf(13, 2, "mA");
    lcd_printf(13, 3, "mW");

    ConfigUART3(9600);
    ConfigUART0(9600);
    ConfigUART0Interrupt(UART0IntHandler);
    StartAllInterrupt();

    while (1)
    {

        ADCIntClear(ADC0_BASE, 0);
        ADCIntClear(ADC1_BASE, 0);
        ADCProcessorTrigger(ADC0_BASE, 0);
        ADCProcessorTrigger(ADC1_BASE, 0);
        if(Vflag == 1){
            Analog_Voltage = Analog_Voltage_Sum / 8;
            Analog_Voltage_Sum = 0;
            Vflag = 0;
        }
        if(Aflag == 1){
            Current = Current_Sum / 8;
            Current_Sum = 0;
            Aflag = 0;
        }
        Data_Break(Analog_Voltage, Current, (int)(Set_Voltage * 1000), M_to_L_Bit_of_Analog_Voltage, M_to_L_Bit_of_Current, M_to_L_Bit_of_Set_Voltage);
        Sent_Voltage(UART3_BASE, Voltage_Current_SetVoltage);

        DisplaySet_Voltage();
        DisplayVoltage();
        DisplayCurrent();

        //SysCtlDelay(SysCtlClockGet() * 0.3);
        //SysCtlDelay(SysCtlClockGet() / 30);
    }

}
