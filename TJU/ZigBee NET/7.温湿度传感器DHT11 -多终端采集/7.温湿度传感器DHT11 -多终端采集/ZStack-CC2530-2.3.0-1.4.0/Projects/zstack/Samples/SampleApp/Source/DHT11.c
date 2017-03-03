#include <ioCC2530.h>
#include "OnBoard.h"

typedef unsigned char uchar;
typedef unsigned int  uint;

#define DATA_PIN P0_7

void Delay_us(void);
void Delay_10us(void);
void Delay_ms(uint Time);
void COM(void);
void DHT11(void);

//��ʪ�ȶ���
uchar ucharFLAG,uchartemp;
uchar shidu_shi,shidu_ge,shidu_p,wendu_shi,wendu_ge,wendu_p;
uchar ucharT_data_H,ucharT_data_L,ucharRH_data_H,ucharRH_data_L,ucharcheckdata;
uchar ucharT_data_H_temp,ucharT_data_L_temp,ucharRH_data_H_temp,ucharRH_data_L_temp,ucharcheckdata_temp;
uchar ucharcomdata;

//��ʱ����
void Delay_us(void) //1 us��ʱ
{   asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
 //   asm("nop");    
   // asm("nop");
  
}

void Delay_10us(void) //10 us��ʱ
{
  Delay_us();
  Delay_us();
  Delay_us();
  Delay_us();
  Delay_us();
  Delay_us();
  Delay_us();
  Delay_us();
  Delay_us();
  Delay_us(); 
}

void Delay_ms(uint Time)//n ms��ʱ
{
  unsigned char i;
  while(Time--)
  {
    for(i=0;i<100;i++)
     Delay_10us();
  }
}

//��ʪ�ȴ���
void COM(void)    // ��ʪд��
{     
    uchar i;         
    for(i=0;i<8;i++)    
    {
        ucharFLAG=2; 
        while((DATA_PIN==0)&&ucharFLAG++);
         MicroWait(37);
        uchartemp=0;
        if(DATA_PIN==1)uchartemp=1;
        ucharFLAG=2;
        while((DATA_PIN==1)&&ucharFLAG++);   
      //  if(ucharFLAG==1)break;    
        ucharcomdata<<=1;
        ucharcomdata|=uchartemp; 
    }    
}

void DHT11(void)   //��ʪ��������
{   P0DIR |= 0x80; //IO����Ҫ��������
    DATA_PIN=0;
     MicroWait(2000); 
    DATA_PIN=1; 
    P0DIR &=0x7f; //��������IO�ڷ���
    MicroWait(35); 
   // Delay_10us();  
    if(DATA_PIN==0) 
    {
        ucharFLAG=2; 
        while((DATA_PIN==0)&&ucharFLAG++);
        ucharFLAG=2;
        while((DATA_PIN==1)&&ucharFLAG++); 
        COM();
        ucharRH_data_H_temp=ucharcomdata;
        COM();
        ucharRH_data_L_temp=ucharcomdata;
        COM();
        ucharT_data_H_temp=ucharcomdata;
        COM();
        ucharT_data_L_temp=ucharcomdata;
        COM();
        ucharcheckdata_temp=ucharcomdata;
        DATA_PIN=1; 
        uchartemp=(ucharT_data_H_temp+ucharT_data_L_temp+ucharRH_data_H_temp+ucharRH_data_L_temp);
        if(uchartemp==ucharcheckdata_temp)
        {
            ucharRH_data_H=ucharRH_data_H_temp;
            ucharRH_data_L=ucharRH_data_L_temp;
            ucharT_data_H=ucharT_data_H_temp;
            ucharT_data_L=ucharT_data_L_temp;
            ucharcheckdata=ucharcheckdata_temp;
        }
        wendu_shi=(ucharT_data_H*256+ucharT_data_L)/100; 
        wendu_ge=(ucharT_data_H*256+ucharT_data_L)/10%10;
        wendu_p =(ucharT_data_H*256+ucharT_data_L)/10; 
        shidu_shi=(ucharRH_data_H*256+ucharRH_data_L)/100; 
        shidu_ge=(ucharRH_data_H*256+ucharRH_data_L)/10%10;  
        shidu_p = (ucharRH_data_H*256+ucharRH_data_L)/10;  
    } 
    else //û�óɹ���ȡ������0
    {
        wendu_shi=0; 
        wendu_ge=0;
        wendu_p=0;
        shidu_shi=0; 
        shidu_ge=0;
        shidu_p=0;
        
    } 
    
    P0DIR |= 0x80; //IO����Ҫ�������� 
}
