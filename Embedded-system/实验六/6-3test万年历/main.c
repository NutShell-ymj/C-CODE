#include <reg51.h>
#include "lcd.h"
#include "ds1302.h"
typedef unsigned int u16;
typedef unsigned char u8;	
sbit beep=P1^5;	  
sbit K1=P3^1;
sbit K2=P3^0;
sbit K3=P3^2;
sbit K4=P3^3;
//void delay(u16 i);
void Int0Configuration();
void LcdDisplay();
void read_time();
//unsigned char SetState,SetPlace;
//void Delay10ms(void);   //��� 0us
u8 m=0;
u16 a=0;
u8 readtime[14];
u8 sec_buf=0;
u8 sec_flag=0;

//void delay(u16 i)
//{
//	while(i--);	
//}
void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;
	i = 18;
	j = 235;
	do
	{
		while (--j);
	} while (--i);
}

void Send_char(u8 T)
{
	SBUF = T;
	while(!TI);	
	TI=0;
}

void UsartInit()
{
			
	TMOD = 0x20;//���ü�����������ʽ2
    SCON = 0x50;//����Ϊ������ʽ1
    TH1 = 0xFA;//��������ʼֵ���ã�������4800
    TL1 = TH1;
    PCON = 0x00;//�����ʲ��ӱ�
    EA = 1;//�����ж�
    ES = 1;//�򿪽����ж�
    TR1 = 1;//�򿪼�����
}


void main()
{
//	unsigned char i;
	Int0Configuration();
	LcdInit();
	Ds1302Init();
	UsartInit();  		//���ڳ�ʼ��
	while(1)
	{	
		read_time();
		LcdDisplay();
		if(readtime[13]!=sec_buf)
		{		
			
			EA=0;
			sec_flag=0;
			sec_buf=readtime[13];
			Send_char(2 +'0');//��
			Send_char(0 +'0');
			Send_char(readtime[2]+'0');
			Send_char(readtime[3]+'0');
			Send_char('/');
			Send_char(readtime[4]+'0');//��
			Send_char(readtime[5]+'0') ;
			Send_char('/');
			Send_char(readtime[6]+'0');//��
			Send_char(readtime[7]+'0');
			Send_char(' ');
			Send_char(readtime[8]+'0');//ʱ
			Send_char(readtime[9]+'0');
			Send_char(':');
			Send_char(readtime [10]+'0');//��
			Send_char(readtime [11]+'0') ;
			Send_char(':');
			Send_char(readtime [12]+'0');//��
			Send_char(readtime [13]+'0');
			Send_char('\n');
 			SBUF='\n';
			while(!TI); //�ȴ��������
			TI=0;//���ж�
			EA=1;
		}
		Delay10ms();

	}
	
}


void LcdDisplay()
{
	LcdWriteCom(0x80+0X40);
	LcdWriteData('0'+TIME[2] /16);				//ʱ
	LcdWriteData('0'+(TIME[2]&0x0f));				 
	LcdWriteData('-');
	LcdWriteData('0'+TIME[1]/16);				//��
	LcdWriteData('0'+(TIME[1]&0x0f));	
	LcdWriteData('-');
	LcdWriteData('0'+TIME[0]/16);				//��
	LcdWriteData('0'+(TIME[0]&0x0f));

	LcdWriteCom(0x80);
	LcdWriteData('2');
	LcdWriteData('0');
	LcdWriteData('0'+TIME[6]/16);			//��
	LcdWriteData('0'+(TIME[6]&0x0f));
	LcdWriteData('-');
	LcdWriteData('0'+TIME[4]/16);			//��
	LcdWriteData('0'+(TIME[4]&0x0f));
	LcdWriteData('-');
	LcdWriteData('0'+TIME[3]/16);			//��
	LcdWriteData('0'+(TIME[3]&0x0f));
	LcdWriteCom(0x8D);
	LcdWriteData('0'+(TIME[5]&0x07));	//����	

}

void Int0Configuration()
{
	//����INT0
	IT0=1;//�����س�����ʽ���½��أ�
	EX0=1;//��INT0���ж�����
	EA=1;//�����ж�	
}



//void Delay10ms(void)   //��� 0us
//{
//    unsigned char a,b,c;
//    for(c=1;c>0;c--)
//        for(b=38;b>0;b--)
//            for(a=130;a>0;a--);
//}

void read_time()
{
	Ds1302ReadTime();
//	readtime [0]= (TIME[0]>>4); //�������ǧλ
//	readtime [1]=(TIME[0]&0x0F);// 11��������λ
	readtime [2]=(TIME[6]>>4); //1�������ʮλ
	readtime [3]= (TIME[6]&0x0F); //��������λ
	readtime [4]=(TIME[4]>>4); //1�������ʮλ
	readtime [5]= (TIME[4]&0x0F); //������¸�λ
	readtime [6]=(TIME[3]>>4); //�������ʮλ
	readtime [7]= (TIME[3]&0x0F); //������ո�λ
	readtime [8]=(TIME[2]>>4); //�����Сʱʮλ
	readtime [9]= (TIME[2]&0x0F); //�����Сʱ��λ
	readtime [10]=(TIME[1]>>4); //���������ʮλ
	readtime [11]= (TIME[1]&0x0F); //��������Ӹ�λ
	readtime [12]=(TIME[0]>>4); //���������ʮλ
	readtime [13]= (TIME[0]&0x0F); //��������Ӹ�λ
}
//void Usart() interrupt 4
//{
//	for(m=0;m<20;m++)
//	{
//	TIME[m] = SBUF;   //��ȥ���յ�������
//	RI = 0;				//��������жϱ�־λ
//	SBUF=TIME[m];	//�����յ������ݷ��뵽���ͼĴ���

//	}
//	while(!TI);			//�ȴ������������
//	TI=0;				//���������ɱ�־λ
//}