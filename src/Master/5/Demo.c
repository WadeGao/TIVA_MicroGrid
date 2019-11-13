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
#include "driverlib/pwm.h"

#define _50KHz 50000
#define DEADZONETIME 0.062  //Make IGBT delay 1.24us for Freq is 50e3Hz
#define Kp_Optimised 0.5353
#define Ki_Optimised 0.1334
//接收数据帧
volatile unsigned char ReceivedVoltage_Current_SetValue[15];
//当前指针，指向将要处理的数据
unsigned char Index_of_Rece_V_C_SetV = 0;
//电压
volatile unsigned int Real_Voltage = 0;
unsigned char Real_Voltage_Array[5];
//电流
volatile unsigned int Current = 0;
unsigned char Current_Array[5];
//设定电压
volatile unsigned int Set_Voltage = 0;
unsigned char Set_Voltage_Array[5];
//数据转换完成标志位
bool Finish_Data_Convert_Flag = 0;

//当前占空比
volatile float Duty = 0;
volatile float Expected_Duty;
//当前电源电压计算值
volatile float Source_Voltage;

/*******************************PID结构体*******************************/
struct{
    float Set_Value;
    float Actual_Value;
    float err;
    float err_next;
    float err_last;
    float Kp, Ki, Kd;
}pid;
void pid_init()
{

    pid.Set_Value = 0.0;
    pid.Actual_Value = 0.0;

    pid.err = 0.0;
    pid.err_last = 0.0;
    pid.err_next = 0.0;
    pid.Kp = Kp_Optimised;
    pid.Ki = Ki_Optimised;
    pid.Kd = 0;
}
float PID_Controllor(float Volt)
{
    pid.Set_Value = Volt;
    pid.err = pid.Set_Value - pid.Actual_Value;
    float result = pid.Kp * (pid.err - pid.err_next) + pid.Ki * pid.err + pid.Kd * (pid.err - 2 * pid.err_next + pid.err_last);
    pid.Actual_Value += result;
    pid.err_last = pid.err;
    return pid.Actual_Value;
}
void Control_Voltage_With_PID(){
    while(1){
        unsigned int status = UARTIntStatus(UART3_BASE, true);
        UARTIntClear(UART3_BASE, status);
        Source_Voltage = Real_Voltage / Duty;

        Expected_Duty = Set_Voltage / Source_Voltage;

        Duty = PID_Controllor(Expected_Duty);

        PWM_ParaMeter_Duty_Set_D1_D0(SysCtlClockGet() / _50KHz, Duty);//TODO：这块要的两路方波还得加上死区
        break;
    }
}
/*******************************PID结构体*******************************/

/*******************************UART相关*******************************/
void UART3IntHandler(void)
{

    unsigned int status = UARTIntStatus(UART3_BASE, true);
    UARTIntClear(UART3_BASE, status);

    while (UARTCharsAvail(UART3_BASE)){
        while(UARTBusy(UART3_BASE));

        unsigned char tmp = UARTCharGetNonBlocking(UART3_BASE);
        UARTCharPutNonBlocking(UART3_BASE, tmp);

        ReceivedVoltage_Current_SetValue[Index_of_Rece_V_C_SetV] = tmp;
        Index_of_Rece_V_C_SetV++;

        if(Index_of_Rece_V_C_SetV > 15){

            short i;
            Index_of_Rece_V_C_SetV = 0;

            volatile uint32_t Copy_Real_Voltage = 0;
            volatile uint32_t Copy_Current = 0;
            volatile uint32_t Copy_Set_Voltage = 0;

            for(i = 0; i < 5; i++){//是怕像上次一样清除不了中断导致数据不准确，找一个数保存得到的数据，即使再进入，元数据也得到了保存
                Copy_Real_Voltage += (int)(ReceivedVoltage_Current_SetValue[i] - '0') * pow(10, 4 - i);   //18715
                Real_Voltage_Array[i] = ReceivedVoltage_Current_SetValue[i];

                Copy_Current += (int)(ReceivedVoltage_Current_SetValue[i + 5] - '0') * pow(10, 4 - i);   //09159
                Current_Array[i] = ReceivedVoltage_Current_SetValue[i + 5];

                Copy_Set_Voltage += (int)(ReceivedVoltage_Current_SetValue[i + 10] - '0') * pow(10, 4 - i);   //18715
                Set_Voltage_Array[i] = ReceivedVoltage_Current_SetValue[i + 10];

                Finish_Data_Convert_Flag = 0; //TODO:是否会有在for 里被打断的可能？
            }

            Finish_Data_Convert_Flag = 1;//

            if(Finish_Data_Convert_Flag == 1){
                Real_Voltage = Copy_Real_Voltage;
                Current = Copy_Current;
                Set_Voltage = Copy_Set_Voltage;
            }

        }
    }
}

void UART3Config(uint32_t Baud){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    GPIOPinConfigure(GPIO_PC6_U3RX);
    GPIOPinConfigure(GPIO_PC7_U3TX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), Baud, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)); //8个数据位，1个停止位， 无校验位

}

void UART3InterruptConfig(){
    IntEnable(INT_UART3);
    UARTIntEnable(UART3_BASE, UART_INT_RX | UART_INT_RT);
    UARTIntRegister(UART3_BASE, UART3IntHandler);
}
void DisplayData(){
    if(Finish_Data_Convert_Flag == 1){
        /*****防止再次进入中断*****/
        while(1){   //防止进入ＵＡＲＴ中断的手段，用死循环屏蔽
            /*****防止再次进入中断*****/
            unsigned int status = UARTIntStatus(UART3_BASE, true);
            UARTIntClear(UART3_BASE, status);
            /*****防止再次进入中断*****/

            //lcd_printf(1, 0, ReceivedVoltage_Current_SetValue);
            lcd_printf(1, 1, Real_Voltage_Array);
            lcd_printf(1, 2, Current_Array);
            lcd_printf(1, 3, Set_Voltage_Array);
            Finish_Data_Convert_Flag = 0;
            break;
        }
    }
}
/*******************************UART相关*******************************/

/*****************************PWM相关******************************/
void InitPWM_D1_D0(void)
{

    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);

    GPIOPinConfigure(GPIO_PD0_M1PWM0);
    GPIOPinConfigure(GPIO_PD1_M1PWM1);

    GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_1 | GPIO_PIN_0);

    PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    PWMDeadBandEnable(PWM1_BASE, PWM_GEN_0, SysCtlClockGet() / 50e3 * DEADZONETIME, SysCtlClockGet() / 50e3 * DEADZONETIME);
    PWMDeadBandEnable(PWM1_BASE, PWM_GEN_1, SysCtlClockGet() / 50e3 * DEADZONETIME, SysCtlClockGet() / 50e3 * DEADZONETIME);

}
void PWM_ParaMeter_Period_Set_D1_D0(unsigned long Period)
{
    //Set the Period (expressed in clock ticks)
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, Period);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, Period);
}
void PWM_ParaMeter_Duty_Set_D1_D0(unsigned long Period, float duty)
{

    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, Period * duty);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, Period * duty);

}
void TurnOnPWM_D1_D0(void)
{
    PWMGenEnable(PWM1_BASE, PWM_GEN_0);
    PWMGenEnable(PWM1_BASE, PWM_GEN_1);
    PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT, true);
}
/*****************************PWM相关******************************/
void main(void)
{
    FPUEnable();
    FPULazyStackingEnable();
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    UART3Config(9600);
    UART3InterruptConfig();

    IntMasterEnable();

    PortFunctionInit();
    lcd_init();


    InitPWM_D1_D0();
    PWM_ParaMeter_Period_Set_D1_D0(SysCtlClockGet() / _50KHz);
    PWM_ParaMeter_Duty_Set_D1_D0(SysCtlClockGet() / _50KHz, Duty);
    TurnOnPWM_D1_D0();

    while (1){
        DisplayData();
        Control_Voltage_With_PID();//Duty的值被更新；
        //TODO:想一下数据接受逻辑是否正确
    }

}

