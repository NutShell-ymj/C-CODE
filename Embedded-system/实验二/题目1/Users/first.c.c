#include<reg52.h>
#include<intrins.h>	
#define led P2


typedef unsigned char u8;
typedef unsigned int u16;
u16 a,b;

sbit k1=P3^1;
sbit k2=P3^0;
sbit k3=P3^2;
sbit k4=P3^3;
sbit beep=P1^5;	

void delay(u16 i)
{
	while(i--);
}

void IntInit()
{
	//����INT0
	IT0=1;//�½���
	EX0=1;//��INT0���ж�����
	IT1=1;//�½��ؼ���
	EX1=1;//��INT1�ж�����	
	EA=1;//�����ж�	
}

void main()
{
	IntInit();
	while(1)
	{
		led=0x00;
		delay(50000);//LED��յ�Ƴ���
	}	
}

void Int1()	interrupt 2		//�ⲿ�ж�1�����ȼ�
{
	delay(1000);//��ʱ����
		led=0xfe;

			for(a=0;a<8;a++)
			{
				P2=~(0x01<<a);	 
				delay(50000); 
			}		

	
}
	

void Int0()	interrupt 0		//�ⲿ�ж�0���жϺ���
{
	delay(1000);//��ʱ����
		led=0xfe;

			for(b=0;b<8;b++)
			{
				P2=~(0x80>>b);	 
				delay(50000); 
			}	
	
}


