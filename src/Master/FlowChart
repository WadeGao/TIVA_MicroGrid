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
