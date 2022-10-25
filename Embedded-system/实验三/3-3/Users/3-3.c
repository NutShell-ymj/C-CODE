//��д����ʹ�ÿ�����ʵ�����¹��ܣ�
//1��	ʹ�ö�ʱ��0�ķ�ʽ0��ʵ��8��LED��0.5s�����˸
//2��	ʹ�ö�ʱ��1�ķ�ʽ1��ʵ�������ǰ��λ59sѭ����ʱ��
//ע�⣺����ܵ�������ʽ��鿴����PPT������ƣ�

#include "reg52.h"

typedef unsigned int u16;
typedef unsigned char u8;

u16 code smgduan[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//��ʾ0~9��ֵ
u16 count1=0,count2=0,n=60,S=0,G=0;

sbit led0 = P2^0;
sbit led1 = P2^1;
sbit led5 = P2^5;
sbit led6 = P2^6;
sbit led7 = P2^7;
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

void delay(u16 n)
{
	u16 a=0,b=0;
	for(a=0;a<n;a++)
	{
		for(b=0;b<110;b++);
	}
}

void Timer0Init(void)		//100us@11.0592MHz
{
	EA = 1;			//�����ж�
	TMOD = 0x10;	//���ö�ʱ��0��ʽ0,	��ʱ��1��ʽ1
	TL0 = 0xA4;		
	TH0 = 0x1F;		//���ö�ʱ��ֵ100us
	ET0 = 1;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	
} 

void Timer1Init(void)		//1ms@11.0592MHz
{
	EA = 1;			//�����ж�
	TL1 = 0x66;		
	TH1 = 0xFC;		//���ö�ʱ��ֵ1ms
	ET1 = 1;		//���TF0��־
	TR1 = 1;		//��ʱ��0��ʼ��ʱ
} 

void Timer0Interrupt() interrupt 1
{
	TL0 = 0xA4;		
	TH0 = 0x1F;		//���ö�ʱ��ֵ100us
	count1++;
	if(count1>=50)
	{
			count1 = 0;
			led0=~led0;
			led1=~led1;
			led5=~led5;
			led6=~led6;
			led7=~led7;
	}
}

void Timer1Interrupt() interrupt 3
{
	TL1 = 0x66;		
	TH1 = 0xFC;		//���ö�ʱ��ֵ1ms
	count2++;
	if(count2>=1000)
		{
			count2 = 0;
			n--;
			S = n/10;
			G = n%10;
			if(n<=0)
			{
				n=60;
			}

		}	
}


void DigDisplay()
{
	u8 i;
	for(i=0;i<2;i++)
	{
		switch(i)	 //λѡ��ѡ�����������ܣ�
		{
		    case(0):
				LSA=1;LSB=1;LSC=1;  P0=smgduan[S];break;//��ʾ��0λ
			case(1):
				LSA=0;LSB=1;LSC=1;  P0=smgduan[G];break;//��ʾ��1λ
		}
		delay(1); //���һ��ʱ��ɨ��1ms		
		P0=0x00;//����
	}


}

void main(void)
{
	led0 = 0;
	led1 = 0;
	led5 = 0;
	led6 = 0;
	led7 = 0;
    Timer0Init();
	Timer1Init();
	while(1)
	{		
		DigDisplay();
	}
}