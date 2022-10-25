#include <reg52.h>
#include <intrins.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <i2c.h>
#define NUM 20

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;

void delay1ms(u16 i);
void delay(u16 i);
void check0();
void LevelSelect();
u8 Keydown();
u8 Keydown1();
void displaysmile();
void displaysmile();
void displaynumber(u8 n);
void displaynumber10(u8 n);
void displayGo();
void displayT();
void displayV();
void displayscore10();
void displayscore1();
void mouse(u8 p,u8 l);
void gamefunc();
void InitTimer0();
void Int0Init();

sbit A0 = P1^0;
sbit A1 = P1^1;
sbit A2 = P1^2;//A0~A2Ϊ138������������ܽ�
sbit beep = P1^5;//��Դ������
sbit select = P1^6;//�Ѷ�ѡ�񰴼�
sbit enter = P1^7;//ȷ�ϰ���
sbit k0=P3^2;//���尴��k0ΪP3.2/INT0
sbit k1=P3^3;//���尴��k1ΪP3.3/INT1

u8 level = 1;//��¼�Ѷ�ֵ
u8 score=0;//scoreΪ��¼�ķ���.��ʱģʽ��ʾʱ��Ҫ��10
u8 max1=0;//��¼��ͨģʽ������������AT24C16
u8 max2=0;//��¼��ʱģʽ������������AT24C16
u8 i=0;
u8 p=1;//��Ϊ��������ӣ��溯��ִ������
u8 j=0;
u8 flag_limit=0;//�ж���Ϸģʽ��־λ
u16 time=0;
u16 TIME1=0;

u8 code number[]= {0x00,0x00,0x7E,0x42,0x42,0x7E,0x00,0x00, //0
                   0x00,0x00,0x42,0x7F,0x40,0x00,0x00,0x00,//1
                   0x00,0x00,0x79,0x49,0x49,0x4F,0x00,0x00,//2
                   0x00,0x00,0x49,0x49,0x49,0x7F,0x00,0x00,//3
                   0x00,0x00,0x0F,0x08,0x08,0x7F,0x00,0x00,//4
                   0x00,0x00,0x4F,0x49,0x49,0x79,0x00,0x00,//5
                   0x00,0x00,0x7F,0x49,0x49,0x79,0x00,0x00,//6
                   0x00,0x00,0x01,0x01,0x01,0x7F,0x00,0x00,//7
                   0x00,0x00,0x7F,0x49,0x49,0x7F,0x00,0x00,//8
                   0x00,0x00,0x4F,0x49,0x49,0x7F,0x00,0x00,//9
                   0x42,0x7F,0x40,0x00,0x7E,0x42,0x42,0x7E
                  };//8*8������ʾ0~10

u8 code number_shi[]= {0x00,0x00,0x7E,0x42,0x42,0x7E,0x00,0x00, //0
                       0x42,0x7F,0x40,0x00,0x7E,0x42,0x42,0x7E,//10
                       0x3A,0x2A,0x2E,0x00,0x3E,0x22,0x3E,0x00,//20
                       0x2A,0x2A,0x3E,0x00,0x3E,0x22,0x3E,0x00,//30
                       0x0E,0x08,0x3E,0x00,0x3E,0x22,0x3E,0x00,//40
                       0x2E,0x2A,0x3A,0x00,0x3E,0x22,0x3E,0x00,//50
                       0x3E,0x2A,0x3A,0x00,0x3E,0x22,0x3E,0x00,//60
                       0x02,0x02,0x3E,0x00,0x3E,0x22,0x3E,0x00,//70
                       0x3E,0x2A,0x3E,0x00,0x3E,0x22,0x3E,0x00,//80
                       0x2E,0x2A,0x3E,0x00,0x3E,0x22,0x3E,0x00,//90
                       0x7E,0x00,0x78,0x78,0x00,0x78,0x78,0x00,//100
					   0x7E,0x00,0x7E,0x00,0x7E,0x42,0x42,0x7E,//110
	                   0x3E,0x00,0x3A,0x2A,0x2E,0x00,0x3E,0x3E,//120
                      };//8*8������ʾ0~100,���10

u8 code ledGo[]= {0x3E,0x22,0x2A,0x3A,0x00,0x3C,0x24,0x3C};
u8 code ledduan[]= {0x03,0x0C,0x30,0xC0};
u8 code ledsmile[]= {0x3C,0x42,0x95,0xA1,0xA1,0x95,0x42,0x3C};
u8 code ledstar[]= {0x00,0x02,0x02,0x7E,0x02,0x02,0x00,0x00};
u8 code ledV[]= {0x00,0x1E,0x20,0x40,0x20,0x1E,0x00,0x00};
u8 position1[20]= {0};
u8 position2[20]= {0};

void delay1ms(u16 i)
{
    u8 x,y;
    for(x=0; x<i; x++)
    {
        for(y=0; y<120; y++);
    }
}
void delay(u16 i)
{
    while(i--);
}


void check0()//�����Լ����2*2������ѭ��16��
{	
	u16 c1,c5;
	for(c1=0;c1<4;c1++)
	{	
		for(c5=0;c5<380;c5++)
		{
			A0=0;A1=0;A2=0;P0=ledduan[c1];
			delay1ms(1);
			A0=1;A1=0;A2=0;P0=ledduan[c1];
			delay1ms(1);
			P0=0;
		}delay1ms(100);
		for(c5=0;c5<380;c5++)
		{		
			A0=0;A1=1;A2=0;P0=ledduan[c1];
			delay1ms(1);
			A0=1;A1=1;A2=0;P0=ledduan[c1];
			delay1ms(1);
			P0=0;
		}delay1ms(100);
		for(c5=0;c5<380;c5++)
		{
			A0=0;A1=0;A2=1;P0=ledduan[c1];
			delay1ms(1);
			A0=1;A1=0;A2=1;P0=ledduan[c1];
			delay1ms(1);
			P0=0;
		}delay1ms(100);
		for(c5=0;c5<380;c5++)
		{
			A0=0;A1=1;A2=1;P0=ledduan[c1];
			delay1ms(1);
			A0=1;A1=1;A2=1;P0=ledduan[c1];
			delay1ms(1);
			P0=0;
		}delay1ms(100);
	}
}
void displaysmile()//��ʾЦ��ͬʱ����������
{	
	u16 c1=0,c4=0;
	beep = 0;
	for(;c4<500;c4++)
	{
			c1 = 0;
			A0=0;A1=0;A2=0;P0=ledsmile[c1++];
			delay1ms(1);
			A0=1;A1=0;A2=0;P0=ledsmile[c1++];
			delay1ms(1);	
			A0=0;A1=1;A2=0;P0=ledsmile[c1++];
			delay1ms(1);
			A0=1;A1=1;A2=0;P0=ledsmile[c1++];
			delay1ms(1);
			A0=0;A1=0;A2=1;P0=ledsmile[c1++];
			delay1ms(1);
			A0=1;A1=0;A2=1;P0=ledsmile[c1++];
			delay1ms(1);
			A0=0;A1=1;A2=1;P0=ledsmile[c1++];
			delay1ms(1);
			A0=1;A1=1;A2=1;P0=ledsmile[c1++];
			delay1ms(1);	
		if(select==0)
		{
			At24c16Write(0x011,000);
			delay1ms(100);
			At24c16Write(0x021,000);
		}
	}
	beep = 1;P0=0;	
}


void displaynumber(u8 n)//8*8����������ʾ0~10
{
	u8 c1=0;
	u8 c3=0;
	u8 c0=n*8;
	for(c3=0;c3<50;c3++)
	{	
		c1=0;
		A0=0;A1=0;A2=0;P0=number[c0+c1++];
		delay1ms(1);
		A0=1;A1=0;A2=0;P0=number[c0+c1++];
		delay1ms(1);	
		A0=0;A1=1;A2=0;P0=number[c0+c1++];
		delay1ms(1);
		A0=1;A1=1;A2=0;P0=number[c0+c1++];
		delay1ms(1);
		A0=0;A1=0;A2=1;P0=number[c0+c1++];
		delay1ms(1);
		A0=1;A1=0;A2=1;P0=number[c0+c1++];
		delay1ms(1);
		A0=0;A1=1;A2=1;P0=number[c0+c1++];
		delay1ms(1);
		A0=1;A1=1;A2=1;P0=number[c0+c1++];
		delay1ms(1);
	}
}
void displaynumber10(u8 n)//��ʾ��ʱģʽ����
{
	u8 c1=0;
	u8 c3=0;
	u8 c0=n*8;
	for(c3=0;c3<50;c3++)
	{	
		c1=0;
		A0=0;A1=0;A2=0;P0=number_shi[c0+c1++];
		delay1ms(1);
		A0=1;A1=0;A2=0;P0=number_shi[c0+c1++];
		delay1ms(1);	
		A0=0;A1=1;A2=0;P0=number_shi[c0+c1++];
		delay1ms(1);
		A0=1;A1=1;A2=0;P0=number_shi[c0+c1++];
		delay1ms(1);
		A0=0;A1=0;A2=1;P0=number_shi[c0+c1++];
		delay1ms(1);
		A0=1;A1=0;A2=1;P0=number_shi[c0+c1++];
		delay1ms(1);
		A0=0;A1=1;A2=1;P0=number_shi[c0+c1++];
		delay1ms(1);
		A0=1;A1=1;A2=1;P0=number_shi[c0+c1++];
		delay1ms(1);
	}
}

void displayT()//��ʾ��ʱģʽ����
{
	u16 c1=0,c4=0;

	for(;c4<100;c4++)
	{
			c1 = 0;
			A0=0;A1=0;A2=0;P0=ledstar[c1++];
			delay1ms(1);
			A0=1;A1=0;A2=0;P0=ledstar[c1++];
			delay1ms(1);	
			A0=0;A1=1;A2=0;P0=ledstar[c1++];
			delay1ms(1);
			A0=1;A1=1;A2=0;P0=ledstar[c1++];
			delay1ms(1);
			A0=0;A1=0;A2=1;P0=ledstar[c1++];
			delay1ms(1);
			A0=1;A1=0;A2=1;P0=ledstar[c1++];
			delay1ms(1);
			A0=0;A1=1;A2=1;P0=ledstar[c1++];
			delay1ms(1);
			A0=1;A1=1;A2=1;P0=ledstar[c1++];
			delay1ms(1);
	}
	P0=0;
}

void displayGo()//��ʾ����Go����
{
	u16 c1 = 0,c4 = 0;
	for(c4=0;c4<250;c4++)
	{
			c1 = 0;
			A0=0;A1=0;A2=0;P0=ledGo[c1++];
			delay1ms(1);
			A0=1;A1=0;A2=0;P0=ledGo[c1++];
			delay1ms(1);	
			A0=0;A1=1;A2=0;P0=ledGo[c1++];
			delay1ms(1);
			A0=1;A1=1;A2=0;P0=ledGo[c1++];
			delay1ms(1);
			A0=0;A1=0;A2=1;P0=ledGo[c1++];
			delay1ms(1);
			A0=1;A1=0;A2=1;P0=ledGo[c1++];
			delay1ms(1);
			A0=0;A1=1;A2=1;P0=ledGo[c1++];
			delay1ms(1);
			A0=1;A1=1;A2=1;P0=ledGo[c1++];
			delay1ms(1);
	}
}
void displayV()//��ʾ��ĸV
{
	u16 c1=0,c4=0;
	beep = 0;
	for(;c4<500;c4++)
	{
			c1 = 0;
			A0=0;A1=0;A2=0;P0=ledV[c1++];
			delay1ms(1);
			A0=1;A1=0;A2=0;P0=ledV[c1++];
			delay1ms(1);	
			A0=0;A1=1;A2=0;P0=ledV[c1++];
			delay1ms(1);
			A0=1;A1=1;A2=0;P0=ledV[c1++];
			delay1ms(1);
			A0=0;A1=0;A2=1;P0=ledV[c1++];
			delay1ms(1);
			A0=1;A1=0;A2=1;P0=ledV[c1++];
			delay1ms(1);
			A0=0;A1=1;A2=1;P0=ledV[c1++];
			delay1ms(1);
			A0=1;A1=1;A2=1;P0=ledV[c1++];
			delay1ms(1);
	}
	beep = 1;P0=0;	
}
   
void mouse(u8 p,u8 l)//�������Ϸִ�к���
{
    u16 c3=0;
    u16 u= abs(l-4)*4000;//ͨ��������ʾʱ��ͼ��ʱ�������Ѷ�
    switch (p)
    {
    case 1:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=0;
            A2=0;
            P0=ledduan[0];
            delay(1);
            A0=1;
            A1=0;
            A2=0;
            P0=ledduan[0];
            delay(1);

            if(Keydown()==p)
            {
                score++;
                beep=0;
                delay(3000);
                beep=1;
                break;
            }
            if(Keydown())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;

    case 2:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=1;
            A2=0;
            P0=ledduan[0];
            delay(1);
            A0=1;
            A1=1;
            A2=0;
            P0=ledduan[0];
            delay(1);

            if(Keydown()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;

    case 3:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=0;
            A2=1;
            P0=ledduan[0];
            delay(1);
            A0=1;
            A1=0;
            A2=1;
            P0=ledduan[0];
            delay(1);

            if(Keydown()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;
    case 4:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=1;
            A2=1;
            P0=ledduan[0];
            delay(1);
            A0=1;
            A1=1;
            A2=1;
            P0=ledduan[0];
            delay(1);

            if(Keydown()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;
    case 5:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=0;
            A2=0;
            P0=ledduan[1];
            delay(1);
            A0=1;
            A1=0;
            A2=0;
            P0=ledduan[1];
            delay(1);

            if(Keydown()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;

    case 6:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=1;
            A2=0;
            P0=ledduan[1];
            delay(1);
            A0=1;
            A1=1;
            A2=0;
            P0=ledduan[1];
            delay(1);

            if(Keydown()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;

    case 7:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=0;
            A2=1;
            P0=ledduan[1];
            delay(1);
            A0=1;
            A1=0;
            A2=1;
            P0=ledduan[1];
            delay(1);

            if(Keydown()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;
    case 8:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=1;
            A2=1;
            P0=ledduan[1];
            delay(1);
            A0=1;
            A1=1;
            A2=1;
            P0=ledduan[1];
            delay(1);

            if(Keydown()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;
    case 9:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=0;
            A2=0;
            P0=ledduan[2];
            delay(1);
            A0=1;
            A1=0;
            A2=0;
            P0=ledduan[2];
            delay(1);

            if(Keydown()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;

    case 10:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=1;
            A2=0;
            P0=ledduan[2];
            delay(1);
            A0=1;
            A1=1;
            A2=0;
            P0=ledduan[2];
            delay(1);

            if(Keydown()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;
    case 11:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=0;
            A2=1;
            P0=ledduan[2];
            delay(1);
            A0=1;
            A1=0;
            A2=1;
            P0=ledduan[2];
            delay(1);

            if(Keydown()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;
    case 12:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=1;
            A2=1;
            P0=ledduan[2];
            delay(1);
            A0=1;
            A1=1;
            A2=1;
            P0=ledduan[2];
            delay(1);

            if(Keydown()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;
    case 13:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=0;
            A2=0;
            P0=ledduan[3];
            delay(1);
            A0=1;
            A1=0;
            A2=0;
            P0=ledduan[3];
            delay(1);

            if(Keydown()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;

    case 14:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=1;
            A2=0;
            P0=ledduan[3];
            delay(1);
            A0=1;
            A1=1;
            A2=0;
            P0=ledduan[3];
            delay(1);

            if(Keydown()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;

    case 15:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=0;
            A2=1;
            P0=ledduan[3];
            delay(1);
            A0=1;
            A1=0;
            A2=1;
            P0=ledduan[3];
            delay(1);

            if(Keydown()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;
    case 16:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=1;
            A2=1;
            P0=ledduan[3];
            delay(1);
            A0=1;
            A1=1;
            A2=1;
            P0=ledduan[3];
            delay(1);

            if(Keydown()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;
    }
}

void LevelSelect()//�Ѷ�ѡ��
{
    P0=0;
    select=1;
    enter=1;
    while(enter)
    {

        if(level>3)
        {
            level=1;
        }
        if(select==0)
        {
            while(!select);
            level++;
        }
        displaynumber(level);
    }
    if(enter==0)
    {
        displayGo();
        P0=0;
    }
}

void mouse1(u8 p,u8 l)//�������Ϸִ�к���
{
    u16 c3=0;
    u16 u= abs(l-4)*3000;//ͨ��������ʾʱ��ͼ��ʱ�������Ѷ�
    switch (p)
    {
    case 1:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=0;
            A2=0;
            P0=ledduan[0];
            delay(1);
            A0=1;
            A1=0;
            A2=0;
            P0=ledduan[0];
            delay(1);

            if(Keydown1()==p)
            {
                score++;
                beep=0;
                delay(3000);
                beep=1;
                break;
            }
            if(Keydown1())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;

    case 2:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=1;
            A2=0;
            P0=ledduan[0];
            delay(1);
            A0=1;
            A1=1;
            A2=0;
            P0=ledduan[0];
            delay(1);

            if(Keydown1()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown1())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;

    case 3:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=0;
            A2=1;
            P0=ledduan[0];
            delay(1);
            A0=1;
            A1=0;
            A2=1;
            P0=ledduan[0];
            delay(1);

            if(Keydown1()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown1())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;
    case 4:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=1;
            A2=1;
            P0=ledduan[0];
            delay(1);
            A0=1;
            A1=1;
            A2=1;
            P0=ledduan[0];
            delay(1);

            if(Keydown1()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown1())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;
    case 5:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=0;
            A2=0;
            P0=ledduan[1];
            delay(1);
            A0=1;
            A1=0;
            A2=0;
            P0=ledduan[1];
            delay(1);

            if(Keydown1()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown1())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;

    case 6:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=1;
            A2=0;
            P0=ledduan[1];
            delay(1);
            A0=1;
            A1=1;
            A2=0;
            P0=ledduan[1];
            delay(1);

            if(Keydown1()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown1())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;

    case 7:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=0;
            A2=1;
            P0=ledduan[1];
            delay(1);
            A0=1;
            A1=0;
            A2=1;
            P0=ledduan[1];
            delay(1);

            if(Keydown1()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown1())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;
    case 8:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=1;
            A2=1;
            P0=ledduan[1];
            delay(1);
            A0=1;
            A1=1;
            A2=1;
            P0=ledduan[1];
            delay(1);

            if(Keydown1()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown1())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;
    case 9:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=0;
            A2=0;
            P0=ledduan[2];
            delay(1);
            A0=1;
            A1=0;
            A2=0;
            P0=ledduan[2];
            delay(1);

            if(Keydown1()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown1())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;

    case 10:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=1;
            A2=0;
            P0=ledduan[2];
            delay(1);
            A0=1;
            A1=1;
            A2=0;
            P0=ledduan[2];
            delay(1);

            if(Keydown1()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown1())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;
    case 11:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=0;
            A2=1;
            P0=ledduan[2];
            delay(1);
            A0=1;
            A1=0;
            A2=1;
            P0=ledduan[2];
            delay(1);

            if(Keydown1()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown1())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;
    case 12:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=1;
            A2=1;
            P0=ledduan[2];
            delay(1);
            A0=1;
            A1=1;
            A2=1;
            P0=ledduan[2];
            delay(1);

            if(Keydown1()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown1())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;
    case 13:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=0;
            A2=0;
            P0=ledduan[3];
            delay(1);
            A0=1;
            A1=0;
            A2=0;
            P0=ledduan[3];
            delay(1);

            if(Keydown1()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown1())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;

    case 14:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=1;
            A2=0;
            P0=ledduan[3];
            delay(1);
            A0=1;
            A1=1;
            A2=0;
            P0=ledduan[3];
            delay(1);

            if(Keydown1()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown1())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;

    case 15:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=0;
            A2=1;
            P0=ledduan[3];
            delay(1);
            A0=1;
            A1=0;
            A2=1;
            P0=ledduan[3];
            delay(1);

            if(Keydown1()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown1())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;
    case 16:
        for(c3=0; c3<u; c3++)
        {
            A0=0;
            A1=1;
            A2=1;
            P0=ledduan[3];
            delay(1);
            A0=1;
            A1=1;
            A2=1;
            P0=ledduan[3];
            delay(1);

            if(Keydown1()==p)
            {
                beep=0;
                delay(3000);
                beep=1;
                score++;
                break;
            }
            if(Keydown1())
            {
                beep=0;
                delay(3000);
                beep=1;
                continue;
            }
        }
        break;


    }
}
u8 Keydown()//�������
{
    u8 line=0,row=0;
    P2=0X0F;
    switch(P2)
    {
    case 0x07:
        line=1;
        break;
    case 0x0b:
        line=2;
        break;
    case 0x0d:
        line=3;
        break;
    case 0x0e:
        line=4;
        break;
    default:
        return 0;
    }
    P2=0XF0;
    switch(P2)
    {
    case 0x70:
        row=1;
        break;
    case 0xb0:
        row=2;
        break;
    case 0xd0:
        row=3;
        break;
    case 0xe0:
        row=4;
        break;
    default:
        return 0;
    }
    P2=0X0F;
    while(P2!=0X0F);
    return (row-1)*4+line;
}


u8 Keydown1()//�������
{
    u8 line=0,row=0;
    P2=0X0F;
    switch(P2)
    {
    case 0x07:
        line=1;
        break;
    case 0x0b:
        line=2;
        break;
    case 0x0d:
        line=3;
        break;
    case 0x0e:
        line=4;
        break;
    default:
        return 0;
    }
    P2=0XF0;
    switch(P2)
    {
    case 0x70:
        row=1;
        break;
    case 0xb0:
        row=2;
        break;
    case 0xd0:
        row=3;
        break;
    case 0xe0:
        row=4;
        break;
    default:
        return 0;
    }
    P2=0X0F;
    while(P2!=0X0F);
    return (row-1)*4+line;
}

void gamefunc()//����λ�������ж���Ϸģʽִ����Ϸ
{
    u8 f=0;
    score=0;

    if(flag_limit==0)
    {
        displaynumber(max1);
        for(f=0; f<10; f++)
        {
            mouse(position1[f],level);
        }
        P0=0;
        if(score>max1)
        {
            displayV();
            max1=score;
            At24c16Write(0x011,max1);
        }
        for(j=0; j<10; j++)
        {
            displayscore1();
        }
    }

    if(flag_limit==1)
    {
        displayT();
        displaynumber10(max2);
        InitTimer0();//���ö�ʱ���ж�
        for(; TIME1<5;)
        {
            if(f>=20)
            {
                f=0;
            }
            mouse1(position2[f++],level);
        }
		TR0=0;
        TIME1=0;
        P0=0;
        if(score>max2)
        {
            displayV();
            max2=score;
            At24c16Write(0x021,max2);
        }
        for(j=0; j<10; j++)
        {
            displayscore10();
        }
    }
}

void displayscore1()//��ʾ����ģʽ����
{
	u8 c1=0;
	u8 c3=0;
	u8 c0=score*8;
	for(c3=0;c3<50;c3++)
	{
		c1=0;
		A0=0;A1=0;A2=0;P0=number[c0+c1++];
		delay1ms(1);
		A0=1;A1=0;A2=0;P0=number[c0+c1++];
		delay1ms(1);	
		A0=0;A1=1;A2=0;P0=number[c0+c1++];
		delay1ms(1);
		A0=1;A1=1;A2=0;P0=number[c0+c1++];
		delay1ms(1);
		A0=0;A1=0;A2=1;P0=number[c0+c1++];
		delay1ms(1);
		A0=1;A1=0;A2=1;P0=number[c0+c1++];
		delay1ms(1);
		A0=0;A1=1;A2=1;P0=number[c0+c1++];
		delay1ms(1);
		A0=1;A1=1;A2=1;P0=number[c0+c1++];
		delay1ms(1);
	}

}

void displayscore10()//��ʾ��ʱģʽ����
{
	u8 c1=0;
	u8 c3=0;
	u8 c0=score*8;

	for(c3=0;c3<50;c3++)
	{
		c1=0;
		A0=0;A1=0;A2=0;P0=number_shi[c0+c1++];
		delay1ms(1);
		A0=1;A1=0;A2=0;P0=number_shi[c0+c1++];
		delay1ms(1);	
		A0=0;A1=1;A2=0;P0=number_shi[c0+c1++];
		delay1ms(1);
		A0=1;A1=1;A2=0;P0=number_shi[c0+c1++];
		delay1ms(1);
		A0=0;A1=0;A2=1;P0=number_shi[c0+c1++];
		delay1ms(1);
		A0=1;A1=0;A2=1;P0=number_shi[c0+c1++];
		delay1ms(1);
		A0=0;A1=1;A2=1;P0=number_shi[c0+c1++];
		delay1ms(1);
		A0=1;A1=1;A2=1;P0=number_shi[c0+c1++];
		delay1ms(1);
	}

}

void IntInit()//�жϺ�����ʼ��
{
    IT0=1;//�����س�����ʽ���½��أ�
    EX0=1;//��INT0���ж�����
    IT1=1;//�����س�����ʽ���½��أ�
    EX1=1;//��INT1���ж�����
    IP=1;//����INT0Ϊ������ȼ�
    EA=1;//�����ж�
}

void InitTimer0()//��ʱ��0��ʽ1��ʼ����10ms
{
    TMOD = 0x01;
    TH0 = 0xDC;
    TL0 = 0x00;
    EA = 1;
    ET0 = 1;
    TR0 = 1;
}

void main()//������
{
	
    P0=0;
    IntInit();  //	�����ⲿ�ж�0

	max1=At24c16Read(0x011);
	delay1ms(100);
	max2=At24c16Read(0x021);
    check0();
    displaysmile();
    P0=0;
    while(1)
    {	

        LevelSelect();
        srand(p++);
        for (i = 0; i< NUM; i++)
        {
            position1[i] = (rand()%16)+1;
            position2[i] = ((rand()%16)+level)%16+1;
        }
        //����1��16��20��α���������λ������
        gamefunc();
        P0=0;
    }
}

void Int1() interrupt 2	//�ⲿ�ж�1���жϺ���
{
    u8 pause=0;
    if(k1==0)
    {
        while(!k1);
        pause=1;
        while(pause)
        {
            if(enter==0)
            {
               break;
            }
        }
    }
}


void Int0()	interrupt 0	//�ⲿ�ж�0���жϺ���
{
    if(k0==0)
    {
        while(!k0);
        flag_limit = !(flag_limit);
    }
}

void Timer0Interrupt() interrupt 1//��ʱ��0����
{
    TH0 = 0xDC;
    TL0 = 0x00;
    time++;//ִ��һ��Ϊ10ms��time����1
    if(time>=100)
    {
        time=0;
        TIME1++;
    }
}//time=100n��Ϊn��
