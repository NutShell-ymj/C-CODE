//��д���򣬱�дһ���Ӽ�ʱ����ʹ�ÿ�����ʵ�����¹��ܣ�
//1��	ʹ�ð���K1 K2 K3,�����ƣ��ֱ�Ϊ��ʼ��ʱ����ͣ��ʱ����ʱ���㣻
//2��	��ʼ��ʱ��K1����ÿ�δ��㿪ʼ��ʱ
//3��	��ͣ��ʱ��K2������һ����ͣ��ʱ���ٴΰ��¼�����ֹͣʱ���ʱ��
//4��	��ʱ���㣨K3������ʱ���㣬��ʾ00��
#include "reg52.h"

typedef unsigned int u16;
typedef unsigned char u8;
u16 code smgduan[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//��ʾ0~9��ֵ
u16 count=0,n=0,S=0,G=0,mb[2];
u16 flag=0;
sbit k2=P3^0;
sbit k1=P3^1;
sbit k3=P3^2;
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

void delay(u16 a)
{
	u16 i=0,j=0;
	for(i=0;i<a;i++)
	{
		for(j=0;j<110;j++);
	}
}

void Timer1Init(void)		//1ms@11.0592MHz
{

	TMOD = 0x10;	//���ö�ʱ��1-��ʽ1
	EA = 1;			//�����ж�
	TL1 = 0x66;		
	TH1 = 0xFC;		//���ö�ʱ��ֵ1ms
	ET1 = 1;		//���TF0��־
	TR1 = 0;		//��ʱ��1�жϼ�ʱ
} 

//void MidInit(void)		//@11.0592MHz
//{   
//	IT0 = 1;    
//    EX0 = 1;
//          
//}


void Timer1Interrupt() interrupt 3
{
	TL1 = 0x66;		
	TH1 = 0xFC;	//���ö�ʱ��ֵ1ms

		count++;	
		
		if(count>=1000)
		{
			count = 0;
			n++;		
			if(n>=60)
			{
				n=0;
			}
		}			
}

//void exint0() interrupt 0 
//{
//	delay(10);
//	if(k2==0)
//		{
//				delay(5);
//				while(!k2);
//				TR1=0;
//		}
//}


void DigDisplay()
{
	u8 i;
	for(i=0;i<2;i++)
	{
		switch(i)	 //λѡ��ѡ�����������ܣ�
		{
		   case(0):
				LSA=1;LSB=1;LSC=1; P0=smgduan[mb[i]];break;//��ʾ��0λ
			case(1):
				LSA=0;LSB=1;LSC=1; P0=smgduan[mb[i]];break;//��ʾ��1λ
		}
		delay(1); //���һ��ʱ��ɨ��		
		P0=0x00;//����
	}
}

void main(void)
{
	Timer1Init();
//	MidInit();
	while(1)
	{   
		mb[0] = n/10;
		mb[1] = n%10;
		if(k1==0)
		{
			delay(5);
			while(!k1);
			TR1=1;
		}
		if(k2==0)
		{
				delay(5);
				while(!k2);
				flag=!flag;
				if(flag==1)
				{
					TR1=0;
				}
				if(flag==0)
				{
					TR1=1;
				}
				
		}
		if(k3==0)
		{
			delay(5);
			n=0;
			TR1=0;	
			while(!k3)
			{
				DigDisplay();
			}		
		}		
	DigDisplay();

	}
}