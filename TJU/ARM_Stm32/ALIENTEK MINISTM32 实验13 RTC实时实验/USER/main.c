#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "usmart.h"
#include "rtc.h"
#include "key.h"
#include "malloc.h"  
#include "MMC_SD.h" 
#include "ff.h"  
#include "exfuns.h"
 u8 string6[20];
 unsigned char StringOfData1[100]="Room #1 2016-12-17 13:25:25 Pm2.5:123.5 Pm10:123.3 Pa:012.3 Dam:093.3% Temp:021.3 CO2:01000\r\n";
 unsigned char StringOfData2[100]="Room #2 2016-12-17 13:25:25 Pm2.5:123.5 Pm10:123.3 Pa:012.3 Dam:093.3% Temp:021.3 CO2:01000\r\n";
 unsigned char StringOfData3[100]="Room #3 2016-12-17 13:25:25 Pm2.5:123.5 Pm10:123.3 Pa:012.3 Dam:093.3% Temp:021.3 CO2:01000\r\n";
 unsigned char StringOfData4[100]="Room #4 2016-12-17 13:25:25 Pm2.5:123.5 Pm10:123.3 Pa:012.3 Dam:093.3% Temp:021.3 CO2:01000\r\n";
 unsigned char StringOfData5[100]="Room #5 2016-12-17 13:25:25 Pm2.5:123.5 Pm10:123.3 Pa:012.3 Dam:093.3% Temp:021.3 CO2:01000\r\n";
 unsigned char AT[20] = "AT\r\n";
 unsigned char ATCWMODE[20] = "AT+CWMODE=1\r\n";
 unsigned char ATRST[8] = "AT+RST\r\n";
 unsigned char ATCWJAP[40] = "AT+CWJAP=\"TPLINK\",\"123456789\"\r\n";
 unsigned char ATCIPMUX[20] = "AT+CIPMUX=0\r\n";
 unsigned char ATCIPMODE[20] = "AT+CIPMODE=1\r\n";
 unsigned char ATCIPSEND[20] = "AT+CIPSEND\r\n";
 unsigned char ATCIPSTART[60] = "AT+CIPSTART=\"TCP\",\"http://1.deveple.applinzi.com\",80\r\n";
 unsigned char GET[500] = "GET http://1.deveple.applinzi.com/data?time=2016-12-17%2013:25:25&pm25=023*5&pm25=123*5&pm25=123*5&pm25=123*5&pm25=123*5&tem=021*3&tem=021*3&tem=021*3&tem=021*3&tem=021*3&dam=093*3%&dam=093*3%&dam=093*3%&dam=093*3%&dam=093*3%&pm10=123*5&pm10=123*5&pm10=123*5&pm10=123*5&pm10=123*5&co2=01000&co2=01000&co2=01000&co2=01000&co2=01000\r\n";
 unsigned char ATCLOSE[3]="+++";
 int main(void)
 { 
		
	u8 t,keyValue,minOfTime=0,min0fiInt=0;	
	u8 len,IDOfRoom;
  u16 Temp,Pa,Dam,Pm=0,Pm_Point,Temp_Point,Pa_Point,Dam_Point;   
	u8 flag_st=0,interval_min=0;
	u32 total,free;
	u16 ooo; 
	u16 t1; 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// ÉèÖÃÖÐ¶ÏÓÅÏÈ¼¶·Ö×é2
	delay_init();	    	 //ÑÓÊ±º¯Êý³õÊ¼»¯	  
	uart_init(9600);	 	//´®¿Ú³õÊ¼»¯Îª9600
	uart_init5(115200);	 	//´®¿Ú³õÊ¼»¯Îª9600
//	LED_Init();				//³õÊ¼»¯ÓëLEDÁ¬½ÓµÄÓ²¼þ½Ó¿Ú
 	LCD_Init();				//³õÊ¼»¯LCD
	KEY_Init();          	//³õÊ¼»¯Óë°´¼üÁ¬½ÓµÄÓ²¼þ½Ó¿Ú
	exfuns_init();		//ÎªfatfsÏà¹Ø±äÁ¿ÉêÇëÄÚ´æ	
	mem_init();			//³õÊ¼»¯ÄÚ´æ³Ø	
	usmart_dev.init(72);	//³õÊ¼»¯USMART
	 for(t1=0;t1<20;t1++)
   {	 
		 USART_SendData(UART5, AT[t1]);
			while(USART_GetFlagStatus(UART5,USART_FLAG_TC)!=SET);
	 }//µÈ´ý·¢ËÍ½áÊø
	 
	 	 for(t1=0;t1<8;t1++)
   {	 
		 USART_SendData(UART5, ATRST[t1]);
			while(USART_GetFlagStatus(UART5,USART_FLAG_TC)!=SET);
	 }//µÈ´ý·¢ËÍ½áÊø
  	delay_ms(1000);
	  delay_ms(1000);
	  delay_ms(1000);
	  delay_ms(1000);
	  delay_ms(1000);
	  delay_ms(1000);
	  delay_ms(1000);
	  delay_ms(1000);
	  delay_ms(1000);
	  delay_ms(1000);
//	 for(t1=0;t1<20;t1++)
//   {	 
//		 USART_SendData(UART5, ATCIPMODE[t1]);
//			while(USART_GetFlagStatus(UART5,USART_FLAG_TC)!=SET);
//	 }//µÈ´ý·¢ËÍ½áÊø
//	  delay_ms(20);
	 for(t1=0;t1<20;t1++)
   {	 
		 USART_SendData(UART5, ATCIPMUX[t1]);
			while(USART_GetFlagStatus(UART5,USART_FLAG_TC)!=SET);
	 }//µÈ´ý·¢ËÍ½áÊø
	 delay_ms(20);
//	  for(t1=0;t1<40;t1++)
//   {	 
//		 USART_SendData(UART5, ATCWJAP[t1]);
//			while(USART_GetFlagStatus(UART5,USART_FLAG_TC)!=SET);
//	 }//µÈ´ý·¢ËÍ½áÊø
//	 
//	 delay_ms(1000);
//	 delay_ms(1000);
//	 delay_ms(1000);
//	 delay_ms(1000);
//	 delay_ms(1000);
//	 delay_ms(1000);
	 
//	  for(t1=0;t1<60;t1++)
//   {	 
//		 USART_SendData(UART5, ATCIPSTART[t1]);
//			while(USART_GetFlagStatus(UART5,USART_FLAG_TC)!=SET);
//	 }//µÈ´ý·¢ËÍ½áÊø
//	 
//	 delay_ms(1000);
//	 
//	  for(t1=0;t1<20;t1++)
//   {	 
//		 USART_SendData(UART5, ATCIPSEND[t1]);
//			while(USART_GetFlagStatus(UART5,USART_FLAG_TC)!=SET);
//	 }//µÈ´ý·¢ËÍ½áÊø
//	 
//	 delay_ms(1000);
//	 
//  for(t1=0;t1<500;t1++)
//	  {
//		  USART_SendData(UART5, GET[t1]);
//			while(USART_GetFlagStatus(UART5,USART_FLAG_TC)!=SET);//µÈ´ý·¢ËÍ½áÊø
//		}//Ïò´®¿Ú1·¢ËÍÊý¾Ý
//	 
//	delay_ms(200);
	POINT_COLOR=RED;//ÉèÖÃ×ÖÌåÎªºìÉ« 
	//LCD_ShowString(60,10,200,16,16,"Mini STM32");	  // col row
	LCD_ShowString(150,10,200,16,16,"stop...");	  // col row
	LCD_ShowString(150,30,200,16,16,"intval:  ");	  // col row
	LCD_ShowNum(206,30,interval_min,2,16);
	LCD_ShowString(3,106,200,16,16,"       PM:   . T:  . ");	  // col row 80
	LCD_ShowString(3,126,200,16,16,"       Pa:   . D:  . %");	  // col row  
  LCD_ShowString(3,116,200,16,16,"   #1");	  // col row
	 
 	LCD_ShowString(3,146,200,16,16,"       PM:   . T:  . ");	  // col row
	LCD_ShowString(3,166,200,16,16,"       Pa:   . D:  . %");	  // col row  
  LCD_ShowString(3,156,200,16,16,"   #2");	  // col row	
	
 	LCD_ShowString(3,186,200,16,16,"       PM:   . T:  . ");	  // col row
	LCD_ShowString(3,206,200,16,16,"       Pa:   . D:  . %");	  // col row  
  LCD_ShowString(3,196,200,16,16,"   #3");	  // col row
	
 	LCD_ShowString(3,226,200,16,16,"       PM:   . T:  . ");	  // col row
	LCD_ShowString(3,246,200,16,16,"       Pa:   . D:  . %");	  // col row  
  LCD_ShowString(3,236,200,16,16,"   #4");	  // col row
	
 	LCD_ShowString(3,266,200,16,16,"       PM:   . T:  . ");	  // col row
	LCD_ShowString(3,286,200,16,16,"       Pa:   . D:  . %");	  // col row  
  LCD_ShowString(3,276,200,16,16,"   #5");	  // col row
	while(RTC_Init())		//RTC³õÊ¼»¯	£¬Ò»¶¨Òª³õÊ¼»¯³É¹¦
	{ 
		LCD_ShowString(60,130,200,16,16,"RTC ERROR!   ");	
		delay_ms(800);
		LCD_ShowString(60,130,200,16,16,"RTC Trying...");	
	}	
	
	while(SD_Initialize())					//¼ì²âSD¿¨
	{
		LCD_ShowString(5,10,200,16,16,"NO SD CARD!");
		delay_ms(200);
		LCD_Fill(5,10,240,150+16,WHITE);//Çå³ýÏÔÊ¾			  
		delay_ms(200);
		//LED0=!LED0;//DS0ÉÁË¸
	}								   	
   exfuns_init();							 //ÎªfatfsÏà¹Ø±äÁ¿ÉêÇëÄÚ´æ				 
	 f_mount(fs[0],"0:",1); 					//¹ÒÔØSD¿ ¨ 
 	 // f_mount(fs[1],"1:",1); 					//¹ÒÔØFLASH.	  
	
	while(exf_getfree("0",&total,&free))	//µÃµ½SD¿¨µÄ×ÜÈÝÁ¿ºÍÊ£ÓàÈÝÁ¿
	{
		LCD_ShowString(60,30,200,16,16,"Fatfs Error!");
		delay_ms(200);
		LCD_Fill(60,30,240,150+16,WHITE);//Çå³ýÏÔÊ¾			  
		delay_ms(200);
	//	LED0=!LED0;//DS0ÉÁË¸
	}	
//	f_open(file,"0:data2.txt",FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
//	f_close(file);
	LCD_ShowString(5,10,200,16,16,"SD OK!     ");	 
	LCD_ShowString(5,30,200,16,16,"FS:     MB");	 
	//LCD_ShowString(5,50,200,16,16,"F:     "); 	    
 //	LCD_ShowNum(21,30,total>>10,5,16);					//ÏÔÊ¾SD¿¨×ÜÈÝÁ¿ MB
 	LCD_ShowNum(29,30,free>>10,5,16);						//ÏÔÊ¾SD¿¨Ê£ÓàÈÝÁ¿ MB		
//RTC_Set(2016,12,27,13,48,30);	
	//ÏÔÊ¾Ê±¼ä
	//POINT_COLOR=BLUE;//ÉèÖÃ×ÖÌåÎªÀ¶É«					 
	LCD_ShowString(60,50,200,16,16,"    -  -     ");	   
	//LCD_ShowString(60,162,200,16,16,"  :  :  ");	 		    
	while(1)
	{
    keyValue = KEY_Scan(0);
		
		switch(keyValue)
		{
			case KEY0_PRES:            //start or stop
			{	
				printf("%d\r\n",keyValue);
				if(flag_st==1)
				{
				flag_st = 0;
				LCD_ShowString(150,10,200,16,16,"start...");	  // col row	
				}
				else
				{
				flag_st = 1;
				LCD_ShowString(150,10,200,16,16,"stop... ");	  // col row	
				}
		//	LCD_ShowString(150,10,200,16,16,"start...");	  // col row	
			break;	}		
			case KEY1_PRES:           // add
			{printf("%d\r\n",keyValue);
				if(flag_st==1)
				{
				if(interval_min<60)
				{
				interval_min+=1;
				}
				}
				LCD_ShowNum(206,30,interval_min,2,16);
				break;}
				
			case WKUP_PRES:            //sub    
			{
				printf("%d\r\n",keyValue);

				if(flag_st==1)
				{
					if(interval_min>0)
					{interval_min-=1;}
				}
				LCD_ShowNum(206,30,interval_min,2,16);
				break;	
			}								
		}				
  if(USART_RX_STA&0x8000)
		{	int i;
			LED1 = 0;				   
			len=USART_RX_STA&0x3fff;    //µÃµ½´Ë´Î½ÓÊÕµ½µÄÊý¾Ý³¤¶È
			if(len==33&&USART_RX_BUF[0]=='I'&&USART_RX_BUF[1]=='D')
			{			
				IDOfRoom = USART_RX_BUF[2]-0x30; //get ID
				if((USART_RX_BUF[3]-0x30)!=0)
				{
				Pa = (USART_RX_BUF[4]-0x30)*10+(USART_RX_BUF[5]-0x30)*1;
				Pa_Point = USART_RX_BUF[7]-0x30;
				}
					else
					{					
				  Pa = (USART_RX_BUF[3]-0x30)*100+(USART_RX_BUF[4]-0x30)*10+(USART_RX_BUF[5]-0x30)*1;
				  Pa_Point = USART_RX_BUF[7]-0x30;
						
					}
				
				Pm = (USART_RX_BUF[8]-0x30)*100+(USART_RX_BUF[9]-0x30)*10+(USART_RX_BUF[10]-0x30)*1;
				Pm_Point = USART_RX_BUF[12]-0x30;
				
				Dam = (USART_RX_BUF[18]-0x30)*100+(USART_RX_BUF[19]-0x30)*10+(USART_RX_BUF[20]-0x30)*1;
				Dam_Point = USART_RX_BUF[22]-0x30;
				
				Temp = (USART_RX_BUF[23]-0x30)*100+(USART_RX_BUF[24]-0x30)*10+(USART_RX_BUF[25]-0x30)*1;
				Temp_Point = USART_RX_BUF[27]-0x30;
//				Pa = (USART_RX_BUF[11]-0x30)*100+(USART_RX_BUF[12]-0x30)*10+(USART_RX_BUF[13]-0x30)*1;
//				Pa_Point = USART_RX_BUF[14]-0x30;
			
				switch(IDOfRoom)   // update display
				{ 
					case 1:	
						for(i =0;i<5;i++)
             {
              if(i!=3)
							{ 
								GET[67+i] = USART_RX_BUF[8+i];
							}
							
							if(i!=3)
							{ 
								GET[121+i] = USART_RX_BUF[23+i];
							}
							if(i!=3)
							{ 
								GET[171+i] = USART_RX_BUF[18+i];
							}
							 if(i!=3)
							{ 
								GET[223+i] = USART_RX_BUF[13+i];
							}
							 GET[277+i] = USART_RX_BUF[28+i];
							 StringOfData1[34+i]=	USART_RX_BUF[8+i]; // pm2.5
							 StringOfData1[45+i]=	USART_RX_BUF[13+i];//pm10
							 StringOfData1[54+i]=	USART_RX_BUF[3+i]; //pa
							 StringOfData1[64+i]=	USART_RX_BUF[18+i];//dam
							 StringOfData1[76+i]=	USART_RX_BUF[23+i];//tem
							 StringOfData1[86+i]=	USART_RX_BUF[28+i];//co2
						 }		
            //if((USART_RX_BUF[3]-0x30)!=0)						 
						LCD_ShowNum(83,106,Pm,3,16); //int
					  LCD_ShowNum(115,106,Pm_Point,1,16);//float	
					
          	LCD_ShowNum(136,106,Temp,2,16);
					  LCD_ShowNum(160,106,Temp_Point,1,16);//float
					
          	LCD_ShowNum(83,126,Pa,3,16);
				  	LCD_ShowNum(115,126,Pa_Point,1,16);//float
					
          	LCD_ShowNum(136,126,Dam,2,16);
					  LCD_ShowNum(160,126,Dam_Point,1,16);//float
						 
						 if((USART_RX_BUF[3]-0x30)!=0)
						 { 
							 LCD_ShowString(83,126,200,16,16,"-");
						 }
            							 
					  break;								
					case 2:
						for(i =0;i<5;i++)
             {	
							 if(i!=3)
							{ 
								GET[78+i] = USART_RX_BUF[8+i];
							}
							if(i!=3)
							{ 
								GET[131+i] = USART_RX_BUF[23+i];
							}
							if(i!=3)
							{ 
								GET[181+i] = USART_RX_BUF[18+i];
							}
							 if(i!=3)
							{ 
								GET[234+i] = USART_RX_BUF[13+i];
							}
							 GET[287+i] = USART_RX_BUF[28+i];
							 StringOfData2[34+i]=	USART_RX_BUF[8+i]; // pm2.5
							 StringOfData2[45+i]=	USART_RX_BUF[13+i];
							 StringOfData2[54+i]=	USART_RX_BUF[3+i]; 
							 StringOfData2[64+i]=	USART_RX_BUF[18+i];
							 StringOfData2[76+i]=	USART_RX_BUF[23+i];
               StringOfData2[86+i]=	USART_RX_BUF[28+i];
						 }			
						LCD_ShowNum(83,146,Pm,3,16); //int
					  LCD_ShowNum(115,146,Pm_Point,1,16);//float	
					
          	LCD_ShowNum(136,146,Temp,2,16);
					  LCD_ShowNum(160,146,Temp_Point,1,16);//float
					
          	LCD_ShowNum(83,166,Pa,3,16);
				  	LCD_ShowNum(115,166,Pa_Point,1,16);//float
					
          	LCD_ShowNum(136,166,Dam,2,16);
					  LCD_ShowNum(160,166,Dam_Point,1,16);//float
						 if((USART_RX_BUF[3]-0x30)!=0)
						 { 
							 LCD_ShowString(83,166,200,16,16,"-");
						 }
					  break;
					case 3:
						for(i =0;i<5;i++)
             {	
							 if(i!=3)
							{ 
								GET[89+i] = USART_RX_BUF[8+i];
							}
							if(i!=3)
							{ 
								GET[141+i] = USART_RX_BUF[23+i];
							}
							if(i!=3)
							{ 
								GET[191+i] = USART_RX_BUF[18+i];
							}
							 if(i!=3)
							{ 
								GET[245+i] = USART_RX_BUF[13+i];
							}
							 GET[297+i] = USART_RX_BUF[28+i];
							 StringOfData3[34+i]=	USART_RX_BUF[8+i]; // pm2.5
							 StringOfData3[45+i]=	USART_RX_BUF[13+i];
							 StringOfData3[54+i]=	USART_RX_BUF[3+i]; 
							 StringOfData3[64+i]=	USART_RX_BUF[18+i];
							 StringOfData3[76+i]=	USART_RX_BUF[23+i];
               StringOfData3[86+i]=	USART_RX_BUF[28+i];
						 }			
					  LCD_ShowNum(83,186,Pm,3,16); //int
					  LCD_ShowNum(115,186,Pm_Point,1,16);//float	
					
          	LCD_ShowNum(136,186,Temp,2,16);
					  LCD_ShowNum(160,186,Temp_Point,1,16);//float
					
          	LCD_ShowNum(83,206,Pa,3,16);
				  	LCD_ShowNum(115,206,Pa_Point,1,16);//float
					
          	LCD_ShowNum(136,206,Dam,2,16);
					  LCD_ShowNum(160,206,Dam_Point,1,16);//float
						 if((USART_RX_BUF[3]-0x30)!=0)
						 { 
							 LCD_ShowString(83,206,200,16,16,"-");
						 }
					  break;
					case 4:
						for(i =0;i<5;i++)
             {	
							 if(i!=3)
							{ 
								GET[100+i] = USART_RX_BUF[8+i];
							}
							 if(i!=3)
							{ 
								GET[151+i] = USART_RX_BUF[23+i];
							}
							if(i!=3)
							{ 
								GET[201+i] = USART_RX_BUF[18+i];
							}
							 if(i!=3)
							{ 
								GET[256+i] = USART_RX_BUF[13+i];
							}
							GET[307+i] = USART_RX_BUF[28+i];
							 StringOfData4[34+i]=	USART_RX_BUF[8+i]; // pm2.5
							 StringOfData4[45+i]=	USART_RX_BUF[13+i];
							 StringOfData4[54+i]=	USART_RX_BUF[3+i]; 
							 StringOfData4[64+i]=	USART_RX_BUF[18+i];
							 StringOfData4[76+i]=	USART_RX_BUF[23+i];
               StringOfData4[86+i]=	USART_RX_BUF[28+i];
						 }			
						LCD_ShowNum(83,226,Pm,3,16); //int
					  LCD_ShowNum(115,226,Pm_Point,1,16);//float	
					
          	LCD_ShowNum(136,226,Temp,2,16);
					  LCD_ShowNum(160,226,Temp_Point,1,16);//float
					
          	LCD_ShowNum(83,246,Pa,3,16);
				  	LCD_ShowNum(115,246,Pa_Point,1,16);//float
					
          	LCD_ShowNum(136,246,Dam,2,16);
					  LCD_ShowNum(160,246,Dam_Point,1,16);//float
						 if((USART_RX_BUF[3]-0x30)!=0)
						 { 
							 LCD_ShowString(83,246,200,16,16,"-");
						 }
					  break;
					case 5:
						for(i =0;i<5;i++)
             {	
							 if(i!=3)
							{ 
								GET[111+i] = USART_RX_BUF[8+i];
							}
							if(i!=3)
							{ 
								GET[161+i] = USART_RX_BUF[23+i];
							}
							if(i!=3)
							{ 
								GET[211+i] = USART_RX_BUF[18+i];
							}
							 if(i!=3)
							{ 
								GET[267+i] = USART_RX_BUF[13+i];
							}
							 GET[317+i] = USART_RX_BUF[28+i];
							 StringOfData5[34+i]=	USART_RX_BUF[8+i]; // pm2.5
							 StringOfData5[45+i]=	USART_RX_BUF[13+i];
							 StringOfData5[54+i]=	USART_RX_BUF[3+i]; 
							 StringOfData5[64+i]=	USART_RX_BUF[18+i];
							 StringOfData5[76+i]=	USART_RX_BUF[23+i];
               StringOfData5[86+i]=	USART_RX_BUF[28+i];
						 }			
						LCD_ShowNum(83,266,Pm,3,16); //int
					  LCD_ShowNum(115,266,Pm_Point,1,16);//float	
					
          	LCD_ShowNum(136,266,Temp,2,16);
					  LCD_ShowNum(160,266,Temp_Point,1,16);//float
					
          	LCD_ShowNum(83,286,Pa,3,16);
				  	LCD_ShowNum(115,286,Pa_Point,1,16);//float
					
          	LCD_ShowNum(136,286,Dam,2,16);
					  LCD_ShowNum(160,286,Dam_Point,1,16);//float
						 if((USART_RX_BUF[3]-0x30)!=0)
						 { 
							 LCD_ShowString(83,286,200,16,16,"-");
						 }
					  break;
				}
			 // printf("GET IT");				
			}
			USART_RX_STA=0;
		}		
		else
		{
		LED1 = 1;
		}
		
		if(minOfTime!=calendar.min&&flag_st==0&&Pm!=0&&interval_min>0)  // one minute and start the device
		{ 
			GET[40]=calendar.w_year/1000+0x30;
			GET[41]=calendar.w_year/100%10+0x30;
			GET[42] = calendar.w_year/10%10+0x30;
			GET[43] = calendar.w_year%10+0x30;
			
			GET[45] = calendar.w_month/10+0x30;   //month
			GET[46] = calendar.w_month%10+0x30;
			
			GET[48] = calendar.w_date/10+0x30;   //day
			GET[49] = calendar.w_date%10+0x30;			
			 
			GET[53] = calendar.hour/10+0x30;        //hour
			GET[54] = calendar.hour%10+0x30;
			
			GET[56] = calendar.min/10+0x30;    //min
			GET[57] = calendar.min%10+0x30;	
			
			GET[59] = calendar.sec/10+0x30;    //sec
			GET[60] = calendar.sec%10+0x30;	
			
		 for(t1=0;t1<20;t1++)
   {	 
		 USART_SendData(UART5, ATCIPMODE[t1]);
			while(USART_GetFlagStatus(UART5,USART_FLAG_TC)!=SET);
	 }//µÈ´ý·¢ËÍ½áÊø
	  delay_ms(20);	
			
			 for(t1=0;t1<60;t1++)
   {	 
		 USART_SendData(UART5, ATCIPSTART[t1]);
			while(USART_GetFlagStatus(UART5,USART_FLAG_TC)!=SET);
	 }//µÈ´ý·¢ËÍ½áÊø
	 
	 delay_ms(1000);
	 
	  for(t1=0;t1<20;t1++)
   {	 
		 USART_SendData(UART5, ATCIPSEND[t1]);
			while(USART_GetFlagStatus(UART5,USART_FLAG_TC)!=SET);
	 }//µÈ´ý·¢ËÍ½áÊø
	 
	 delay_ms(1000);
			
		  for(t1=0;t1<500;t1++)
	  {
		  USART_SendData(UART5, GET[t1]);
			while(USART_GetFlagStatus(UART5,USART_FLAG_TC)!=SET);//µÈ´ý·¢ËÍ½áÊø
		}//Ïò´®¿Ú1·¢ËÍÊý¾Ý	
				 delay_ms(500);
		 for(t1=0;t1<3;t1++)
	  {
		  USART_SendData(UART5, ATCLOSE[t1]);
			while(USART_GetFlagStatus(UART5,USART_FLAG_TC)!=SET);//µÈ´ý·¢ËÍ½áÊø
		}//Ïò´®¿Ú1·¢ËÍÊý¾Ý	
				 delay_ms(50);
	  printf("it is time\r\n");			
		minOfTime=calendar.min;
		min0fiInt+=1;
		if(min0fiInt==interval_min)	  // it is time to write data to SD card
		{  
			 printf("GET\r\n");
			 
			 //StringOfData[6] = IDOfRoom+0x30;   //ID
			
		   StringOfData1[8] = calendar.w_year/1000+0x30;   //yerar
			 StringOfData1[9] = calendar.w_year/100%10+0x30;
			 StringOfData1[10] = calendar.w_year/10%10+0x30;
			 StringOfData1[11] = calendar.w_year%10+0x30;
			
			 StringOfData1[13] = calendar.w_month/10+0x30;   //month
			 StringOfData1[14] = calendar.w_month%10+0x30;
			
			 StringOfData1[16] = calendar.w_date/10+0x30;   //day
			 StringOfData1[17] = calendar.w_date%10+0x30;			
			 
			 StringOfData1[19] = calendar.hour/10+0x30;        //hour
			 StringOfData1[20] = calendar.hour%10+0x30;
			
			 StringOfData1[22] = calendar.min/10+0x30;    //min
			 StringOfData1[23] = calendar.min%10+0x30;	
			
			 StringOfData1[25] = calendar.sec/10+0x30;    //sec
			 StringOfData1[26] = calendar.sec%10+0x30;	
			 
			 
			 
			 StringOfData2[8] = calendar.w_year/1000+0x30;   //yerar
			 StringOfData2[9] = calendar.w_year/100%10+0x30;
			 StringOfData2[10] = calendar.w_year/10%10+0x30;
			 StringOfData2[11] = calendar.w_year%10+0x30;
			
			 StringOfData2[13] = calendar.w_month/10+0x30;   //month
			 StringOfData2[14] = calendar.w_month%10+0x30;
			
			 StringOfData2[16] = calendar.w_date/10+0x30;   //day
			 StringOfData2[17] = calendar.w_date%10+0x30;			
			 
			 StringOfData2[19] = calendar.hour/10+0x30;        //hour
			 StringOfData2[20] = calendar.hour%10+0x30;
			
			 StringOfData2[22] = calendar.min/10+0x30;    //min
			 StringOfData2[23] = calendar.min%10+0x30;	
			
			 StringOfData2[25] = calendar.sec/10+0x30;    //sec
			 StringOfData2[26] = calendar.sec%10+0x30;	
			 
			 
			 
			 
			 StringOfData3[8] = calendar.w_year/1000+0x30;   //yerar
			 StringOfData3[9] = calendar.w_year/100%10+0x30;
			 StringOfData3[10] = calendar.w_year/10%10+0x30;
			 StringOfData3[11] = calendar.w_year%10+0x30;
			
			 StringOfData3[13] = calendar.w_month/10+0x30;   //month
			 StringOfData3[14] = calendar.w_month%10+0x30;
			
			 StringOfData3[16] = calendar.w_date/10+0x30;   //day
			 StringOfData3[17] = calendar.w_date%10+0x30;			
			 
			 StringOfData3[19] = calendar.hour/10+0x30;        //hour
			 StringOfData3[20] = calendar.hour%10+0x30;
			
			 StringOfData3[22] = calendar.min/10+0x30;    //min
			 StringOfData3[23] = calendar.min%10+0x30;	
			
			 StringOfData3[25] = calendar.sec/10+0x30;    //sec
			 StringOfData3[26] = calendar.sec%10+0x30;	
			 
			 
			 
			 
			 StringOfData4[8] = calendar.w_year/1000+0x30;   //yerar
			 StringOfData4[9] = calendar.w_year/100%10+0x30;
			 StringOfData4[10] = calendar.w_year/10%10+0x30;
			 StringOfData4[11] = calendar.w_year%10+0x30;
			
			 StringOfData4[13] = calendar.w_month/10+0x30;   //month
			 StringOfData4[14] = calendar.w_month%10+0x30;
			
			 StringOfData4[16] = calendar.w_date/10+0x30;   //day
			 StringOfData4[17] = calendar.w_date%10+0x30;			
			 
			 StringOfData4[19] = calendar.hour/10+0x30;        //hour
			 StringOfData4[20] = calendar.hour%10+0x30;
			
			 StringOfData4[22] = calendar.min/10+0x30;    //min
			 StringOfData4[23] = calendar.min%10+0x30;	
			
			 StringOfData4[25] = calendar.sec/10+0x30;    //sec
			 StringOfData4[26] = calendar.sec%10+0x30;	
			 
			 
			 
			 StringOfData5[8] = calendar.w_year/1000+0x30;   //yerar
			 StringOfData5[9] = calendar.w_year/100%10+0x30;
			 StringOfData5[10] = calendar.w_year/10%10+0x30;
			 StringOfData5[11] = calendar.w_year%10+0x30;
			
			 StringOfData5[13] = calendar.w_month/10+0x30;   //month
			 StringOfData5[14] = calendar.w_month%10+0x30;
			
			 StringOfData5[16] = calendar.w_date/10+0x30;   //day
			 StringOfData5[17] = calendar.w_date%10+0x30;			
			 
			 StringOfData5[19] = calendar.hour/10+0x30;        //hour
			 StringOfData5[20] = calendar.hour%10+0x30;
			
			 StringOfData5[22] = calendar.min/10+0x30;    //min
			 StringOfData5[23] = calendar.min%10+0x30;	
			
			 StringOfData5[25] = calendar.sec/10+0x30;    //sec
			 StringOfData5[26] = calendar.sec%10+0x30;	
			 
//			 StringOfData[31] = Pm/100+0x30;
//			 StringOfData[32] = Pm/10%10+0x30;
//			 StringOfData[33] = Pm%10+0x30;
//			 StringOfData[35] = Pm_Point+0x30;	
//			 
//			 StringOfData[40] = Pa/10+0x30;
//			 StringOfData[41] = Pa%10+0x30;
//			 StringOfData[43] = Pa_Point+0x30;
//			 
//			 StringOfData[49] = Dam/10+0x30;
//			 StringOfData[50] = Dam%10+0x30;
//			 StringOfData[52] = Dam_Point+0x30;
//			 
//			 StringOfData[60] = Temp/10+0x30;
//			 StringOfData[61] = Temp%10+0x30;
//			 StringOfData[63] = Temp_Point+0x30;
		//	 printf(StringOfData);
			 min0fiInt=0;	
       //exfuns_init();							 			 
	    // f_mount(fs[0],"0:",1); 						 
  		 if(f_open(file,"0:data2.txt",FA_OPEN_ALWAYS | FA_WRITE | FA_READ))
				 printf("failed\r\n");
			 else
				 printf("success\r\n");
			 
			 f_lseek(file,file->fsize);  //to the end of file
       f_puts(StringOfData1,file);  //write string
			 
			 f_lseek(file,file->fsize);  //to the end of file
       f_puts(StringOfData2,file);  //write string	
			 
			 f_lseek(file,file->fsize);  //to the end of file
       f_puts(StringOfData3,file);  //write string
			 
			 f_lseek(file,file->fsize);  //to the end of file
       f_puts(StringOfData4,file);  //write string
			 
			 f_lseek(file,file->fsize);  //to the end of file
       f_puts(StringOfData5,file);  //write string
			 
       f_lseek(file,file->fsize);  //to the end of file
       f_puts("\r\n",file);  //write string
			 
			 delay_ms(200);
       f_close(file);
       delay_ms(200);			 
		
		}		
		}
		if(t!=calendar.sec)  // one seconds
		{
			t=calendar.sec;
			LCD_ShowNum(60,50,calendar.w_year,4,16);									  
			LCD_ShowNum(100,50,calendar.w_month,2,16);									  
			LCD_ShowNum(124,50,calendar.w_date,2,16);	 
			switch(calendar.week)
			{
				case 0:
					LCD_ShowString(60,68,200,16,16,"Sunday   ");
					break;
				case 1:
					LCD_ShowString(60,68,200,16,16,"Monday   ");
					break;
				case 2:
					LCD_ShowString(60,68,200,16,16,"Tuesday  ");
					break;
				case 3:
					LCD_ShowString(60,68,200,16,16,"Wednesday");
					break;
				case 4:
					LCD_ShowString(60,68,200,16,16,"Thursday ");
					break;
				case 5:
					LCD_ShowString(60,68,200,16,16,"Friday   ");
					break;
				case 6:
					LCD_ShowString(60,68,200,16,16,"Saturday ");
					break;  
			}
			sprintf(string6,"%2d:%02d:%02d   ",calendar.hour,calendar.min,calendar.sec);
			LCD_ShowString(60,86,200,16,16,string6);	
			LED0=!LED0;
		}	
		delay_ms(10);								  
	};  											    
}	



