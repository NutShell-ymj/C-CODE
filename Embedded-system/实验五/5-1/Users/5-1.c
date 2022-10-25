//5-1
//��д���׼ӷ�����������ʹ�ÿ����壬ʵ�ֽ��С��10�ļӷ�����
//�趨9�����ְ�����1���ӷ�������Ϊ���룬�������ʾ��������
#include "reg52.h"
#define GPIO_KEY P1
typedef unsigned int u16;
typedef unsigned char u8;
u8 KeyValue1;//������Ŷ�ȡ���ļ�ֵ
u8 KeyValue2;//������Ŷ�ȡ���ļ�ֵ
u8 KeyState1;//������Ű���״̬
u8 KeyState2;//������Ű���״̬
u16 code smgduan[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//��ʾ0~9��ֵ
u16 sum=0,count1=0,count2=0;
u8 shi=0,ge=0;
u8 flag=0;
u8 add=0;
u16 m=0;
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;
sbit k3=P3^2;
sbit k4=P3^3;

void delay10ms()
{
    unsigned char a,b;
    for(b=249;b>0;b--)
        for(a=17;a>0;a--);
}
void delay(u16 a)
{
	u16 i=0,j=0;
	for(i=0;i<a;i++)
	{
		for(j=0;j<110;j++);
	}
}

void DigDisplayN(u8 NOw)
{
	u8 i;
	for(i=0;i<200;i++)
	{
		LSA=1;LSB=1;LSC=1; P0=smgduan[NOw];
		delay(1); //���һ��ʱ��ɨ��
	}
}

void KeyDown1()//������ް������²���ȡ��ֵ
{
//	char m;
    for(;;)
    {
	GPIO_KEY=0x0f;
	delay10ms();delay10ms();
	if(GPIO_KEY!=0x0f)
	{
		delay10ms();
		if(GPIO_KEY!=0x0f)
		{
			KeyState1=1; 
			//������
// 			Delay10ms();
			switch(GPIO_KEY)
			{
				case(0X07):	KeyValue1=0;break;//0000 0111
				case(0X0b):	KeyValue1=1;break;//0000 1011
				case(0X0d): KeyValue1=2;break;//0000 1101
				case(0X0e):	KeyValue1=3;break;//0000 1110
//				default:	KeyValue=17;	//������ظ�17��˼�ǰ������ȫ�����
			}
			//������
			GPIO_KEY=0xf0;
			delay10ms();
			switch(GPIO_KEY)
			{
				case(0X70):	KeyValue1=KeyValue1;break;
				case(0Xb0):	KeyValue1=KeyValue1+4;break;
				case(0Xd0): KeyValue1=KeyValue1+8;break;
				case(0Xe0):	KeyValue1=KeyValue1+12;break;
//				default:	KeyValue1=17;
			}
//			while((m<50)&&(GPIO_KEY!=0xf0))	 //��ⰴ�����ּ��
//			{
//			delay10ms();
//				m++;
//			}

		}
	}
	DigDisplayN(KeyValue1);
	if(KeyState1==1)
        break;
    }
}

void KeyDown2()//������ް������²���ȡ��ֵ
{
//	char n;
for(;;)
	{GPIO_KEY=0x0f;
	delay10ms();delay10ms();
	if(GPIO_KEY!=0x0f)
	{
		delay10ms();
		if(GPIO_KEY!=0x0f)
		{
			KeyState2=1;
			//������
			GPIO_KEY=0x0f;
//			delay10ms();
			switch(GPIO_KEY)
			{
				case(0X07):	KeyValue2=0;break;
				case(0X0b):	KeyValue2=1;break;
				case(0X0d): KeyValue2=2;break;
				case(0X0e):	KeyValue2=3;break;
//				default:	KeyValue=17;	//������ظ�17��˼�ǰ������ȫ�����
			}
			//������
			GPIO_KEY=0xf0;
			delay10ms();
			switch(GPIO_KEY)
			{
				case(0X70):	KeyValue2=KeyValue2;break;
				case(0Xb0):	KeyValue2=KeyValue2+4;break;
				case(0Xd0): KeyValue2=KeyValue2+8;break;
				case(0Xe0):	KeyValue2=KeyValue2+12;break;
//				default:	KeyValue=17;
			}
//			while((n<50)&&(GPIO_KEY!=0xf0))	 //��ⰴ�����ּ��
//			{
//			delay10ms();
//				n++;
//			}
		}
	}DigDisplayN(KeyValue2);
	if(KeyState2==1)
        break;
	}
}


void main()
{

	while(1)
	{
        if(flag==0)
		{
			KeyDown1();
			count1=KeyValue1;
			flag=1;
			delay(200);
		}
		
		if(k4==0)
		{		
			delay10ms();
			while(!k4);
			add=1;
		}
		if(flag==1 && add==1)
		{
			KeyDown2();
			count2=KeyValue2;
			if(GPIO_KEY!=0xff)
			flag=2;
			delay(200);
		}

		if(k3==0)
		{
			delay10ms();
			while(!k3);
			sum=count1+count2;
			shi = sum/10;
			ge = sum%10;
			for(m=0;m<1000;m++)
			{

				DigDisplayN(ge);
				
			}
		}		
	}
}
