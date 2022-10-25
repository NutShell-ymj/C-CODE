#include "reg52.h"		 

typedef unsigned int u16;
typedef unsigned char u8;
u8 receiveData;

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
	UsartInit();  		//���ڳ�ʼ��
	while(1);		
}

void Usart() interrupt 4
{
	receiveData=SBUF;   //��ȥ���յ�������
	RI = 0;				//��������жϱ�־λ
	SBUF=receiveData;	//�����յ������ݷ��뵽���ͼĴ���
	while(!TI);			//�ȴ������������
	TI=0;				//���������ɱ�־λ
}


