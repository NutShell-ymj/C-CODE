#include <reg52.h>
#define uchar unsigned char
	
sbit p1=P1^0;
uchar  a = 0;

//void int1() interrupt 1
//{
//	a%=10;
//	if(a<2)
//	{
//		p1=1;
//	}
//	else 
//	{
//		p1=0;
//	}
//	a++;
//}

void int1() interrupt 1
{
	a++;
	if(a==80)
	{
		p1=1;
	}
	if(a==100)
	{
		P1=0;
		a=0;
	}

}

void main()
{
	TMOD=0x02; //����T0Ϊ��ʽ2
	TH0=216;
	TL0=216; //ÿ��40΢��
	TR0=1; //����T0��ʱ��
	ET0=1; //�����ⲿ�ж�0�ж�
	EA=1; //���ж�����
	while(1);
}