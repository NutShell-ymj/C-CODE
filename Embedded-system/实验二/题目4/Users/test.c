#include <reg52.h>	

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;

sbit led=P2^0;	 //����P20����led


void InitTimer0(void)
{
    TMOD = 0x01;
	TH0=0XFC;	//����ʱ������ֵ����ʱ1ms
	TL0=0X66;
    EA = 1;
    ET0 = 1;
    TR0 = 1;
}

void main(void)
{
    InitTimer0();
	while(1);
}

void Timer0Interrupt(void) interrupt 1
{
	static u16 i;
	TH0=0XFC;	//����ʱ������ֵ����ʱ1ms
	TL0=0X66;//Ĭ��ʹ��11.0592MHz����
	i++;
	if(i==500)
	{
		i=0;
		led=~led;	
	}	
}
