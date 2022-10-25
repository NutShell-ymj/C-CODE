#include <reg52.h>
#include <intrins.h>
 

#define GPIO_DIG P0
//ʹ��138�������Ӱ˸������
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;
typedef unsigned int u16;
typedef unsigned char u8;
u8 receiveData;
u8 i=0;
u16 j=10;
//--����ȫ�ֱ���--//
unsigned char code DIG_CODE[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
//0��1��2��3��4��5��6��7��8��9����ʾ��
unsigned char DisplayData[3];
//�������Ҫ��ʾ��3λ����ֵ

//--����ȫ�ֺ���--//
void DigDisplay();         //��̬��ʾ����           
void UsartConfiguration();
 
void main()
{
        UsartConfiguration();
		
        while(1)
        {
                DigDisplay();
        }
}

 
void UsartConfiguration()
{
    TMOD = 0x20;
    SCON = 0x50;
    TH1 = 0xF4;
    TL1 = 0xF4;
    PCON = 0x80;
    EA = 1;
    ES = 1;
    TR1 = 1; //�򿪼�����
}

void DigDisplay()
{


                switch(i)         //λѡ��ѡ�����������ܣ�
                {
                        case(0):
                                LSA=0;LSB=0;LSC=0;GPIO_DIG=0x40; //��ʾ��0λ��λ
						case(1):
                                LSA=1;LSB=0;LSC=0;GPIO_DIG=DisplayData[i-1];//��ʾ��1λʮλ
//						case(2):
//                                LSA=0;LSB=1;LSC=0; break;//��ʾ��2λ��λ
                }
                j=10;                                                 //ɨ����ʱ���趨
                while(j--);        
                GPIO_DIG=0x00;//����
}



void Usart() interrupt 4
{
	receiveData=SBUF;   //��ȥ���յ�������
	RI = 0;				//��������жϱ�־λ
	SBUF=receiveData;	//�����յ������ݷ��뵽���ͼĴ���
	while(!TI);			//�ȴ������������
    DisplayData[0] = DIG_CODE[(receiveData-'0')%10];//��λ
//	DisplayData[1] = DIG_CODE[(receiveData-'0')%100/10];//ʮλ
//	DisplayData[2] = DIG_CODE[(receiveData-'0')/100];//��λ
	i=1;
	TI=0;				//���������ɱ�־λ
}


