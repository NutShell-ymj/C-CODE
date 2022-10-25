#include<reg52.h>
typedef unsigned char u8;
typedef unsigned int u16;
u16 count=0;

sbit k3=P3^2;
sbit k4=P3^3;
sbit beep=P1^5;	
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

u16 code smgduan[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//��ʾ0~9��ֵ

void delay(u16 i)
{
	while(i--);
}


void IntInit()
{
	IT0=1;//�½��ش���
	EX0=1;//��INT0���ж�����
	EA=1;//�����ж�	
}

void DigDisplay()
{
	u8 i;
	for(i=0;i<8;i++)
	{
		switch(i)	 //λѡ��ѡ�����������ܣ�
		{
		   case(0):
				LSA=1;LSB=1;LSC=1;  P0=0x00;break;//��ʾ��0λ
			case(1):
				LSA=0;LSB=1;LSC=1;  P0=0x00;break;//��ʾ��1λ
			case(2):
				LSA=1;LSB=0;LSC=1;  P0=0x00;break;//��ʾ��2λ
			case(3):	
				LSA=0;LSB=0;LSC=1;  P0=0x00;break;//��ʾ��3λ
			case(4):
				LSA=1;LSB=1;LSC=0;  P0=0x00;break;//��ʾ��4λ
			case(5):
				LSA=0;LSB=1;LSC=0;  P0=0x00;break;//��ʾ��5λ
			case(6):
				LSA=1;LSB=0;LSC=0;  P0=0x00;break;//��ʾ��6λ
			case(7):
				LSA=0;LSB=0;LSC=0;  P0=smgduan[count];break;//��ʾ��7λ	
		}
		delay(100); //���һ��ʱ��ɨ��	
		P0=0x00;//����
	}
}
void main()
{
	IntInit();
	while(1)
	{
		DigDisplay();
	}
}



void Int0()	interrupt 0		//�ⲿ�ж�0���жϺ���
{
	if(k3==0)
	{
		delay(1000);
			while(!k3);
			count++;
		
	}
	
	if(count>=9)
	{
		while(1)
		{	
		
			beep=~beep;
			delay(10);
		}
	}
}


