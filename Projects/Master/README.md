# 主机接收数据代码
文件夹1：第一版，数据控制策略有问题，UART函数是可以直接Get到int型数据的

文件夹2：因为文件夹１的原因，到时测量电压的时候直接转化为mV发送出去，测量范围可以达到`0~65.336V`


_Project_
|
|_____Master
|	   |UART_RX:
|          |        |___Algorithm to screen data
|          |        |___Data Structure:Real Voltage(5 Bytes)、Real Current(5 Bytes) and Setted Voltage(5 Bytes)
|          |        |___FeedBack:Real Voltage to screen Real Source Voltage and adjust duty dymaticly
|          |        |___Baud Rate:9600
|          |	    |___Port：UART3
|	   |PWM:
|	　 |　　　   |___Two PWM Wave: Complementary with each other
|	　 |　　　   |___Port：
|	   
|
|_____Slave
	  |UART: 
          |         |___Baud Rate:9600（//TODO:How to reslove the data received from server?）
	  |         |___Data Structure:Real Voltage(5 Bytes)、Real Current(5 Bytes) and Setted Voltage(5 Bytes)
	  |	    |___Port：UART3
	  |
	  |ADC: 
          |         |___Measure Voltage by ADC and measure current by sampling redsistors
 	  |         |___Port：PE1
 	  |
 	  |
          |GPIO: 
          |         |___K3, K4:Input setting values 
	  |         |___K2:Change Mode(const voltage or const current)
          |
          |
 	  |
 	  |LCD:
 	           |___Real Voltage、Real Current and Setted Voltage
