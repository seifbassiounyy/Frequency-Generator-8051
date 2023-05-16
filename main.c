#include <REGX51.H>     
#include <stdlib.h>
#include <math.h>

#define SegOne   0xf7
#define SegTwo   0xfe
#define SegThree 0xfd
#define SegFour  0xfb	

void init(void);
void SerialInitialize(void);
void keypad_input(void);
void display(void);
//void msdelay(unsigned int time);
void getfreq(void);
//void timer_fun(void);

sbit c1  =  P1^0;
sbit c2  =  P1^1;
sbit c3  =  P1^2;
sbit r1  =  P1^3;
sbit r2  =  P1^4;
sbit r3  =  P1^5;
sbit r4  =  P1^6;
sbit out =  P3^7;

char hexvalue[] = {
		0xC0, // 0
    0xF9, // 1
    0xA4, // 2
    0xB0, // 3
    0x99, // 4
    0x92, // 5
    0x82, // 6
    0xF8, // 7
    0x80, // 8
    0x90  // 9
	};
char frequency[4]="0000";
float ovf=0.0;
char high=0, low=0, counter=0;
int whole, temp, num, freqint=5000;

void DELAY_ms(unsigned int ms_Count) 
{
    unsigned int i, j;
    for (i = 0; i < ms_Count; i++) 
    {
        for (j = 0; j < 100; j++);
    }
}
void ISR_ex0(void) interrupt 0  
{ 
	//if(freqint>7)
	//{
		getfreq();
		TR1 = 1;
	/*}
	else
	{
		getfreq(0);
		TR0 = 1;
	}*/
} 
void timer0_ISR (void) interrupt 1
{ 
	  display();
		TH0=0xFF;
		TL0=0x00;
	  //TF0 = 0;    // Clear Timer 1 interrupt flag
}   
void ISR_ex1(void) interrupt 2  
{ 
	frequency[0]=frequency[1]=frequency[2]=frequency[3]='0';
	freqint=counter=0;
	TR1=0;
}
void timer1_isr(void) interrupt 3
{
	//timer_fun();
	out=~out;
	//TR1=0;
	//TF1=0;
	TH1=high;
	TL1=low;
	//TR1=1;
}
void main()
{  
	 init();
	 while (1)
    {
			keypad_input();	
    }
}
void init(void)            
{
    P0=P1=P2=P3=0xFF;                             
    //P1 = 0xFF;                                    
    //P2 = 0xFF;                          
    //P3 = 0x0F;
		IP |= 0x08;
	
		TMOD = 0x11;                   		//Timer 1 In Mode 1 
    TH0 = 0xFF;                       //refresh rate = 276us = 0.276ms
		TL0 = 0x00;
    TR0 = 1;   
		
		ET1 = 1;
		EA = 1;
    ES = 1;	
		ET0 = 1;
		EX0 = 1;
		IT0 = 1;
		EX1 = 1;
		IT1 = 1;
		//rupt = 1;
		//out=1;
}
/*void msdelay(unsigned int time) 
{
    unsigned i,j ;

    for(i=0;i<time;i++)    

    for(j=0;j<1275;j++);
}*/
void getfreq(void)
{
	//if(f==1)
	//{
		ovf = 1000000/freqint;
		ovf/=2;
		ovf/=1.085069444;
		ovf-=9;
		whole = (int)ovf;
		//remainder = ovf-whole;
		//if(remainder>=0.5)
		//	whole++;
		whole = 65536-whole;
		TL1 = low =  whole;
		whole = whole>>8;
		TH1 = high = whole;
		//high = TH1;
		//low = TL1;
		//ctr=1;
	//}
	/*else if(f==0)
	{
		ovf = 1000000/freqint;
		ovf = ovf/2;
		ovf = ovf/1.085069444;
		ovf = ovf/65536;
		whole = (int)ovf;
		remainder = ovf-whole;
		ctr = whole+1;
		remainder = remainder*65536;
		whole = (int)remainder;
		TL0 = whole;
		whole = whole>>8;
		TH0 = whole;
		high = TH0;
		low = TL0;
	}*/
}
/*
void timer_fun(void)
{
	if(ctr==1)
	{
	TR0=0;
	TF0=0;
	out=~out;
	TH0=high;
	TL0=low;
	TR0=1;
	}
	else
	{
		if(iterator<ctr)
		{
			iterator++;
			TH0=0x00;
			TL0=0x00;
		}
		else
		{
			out=~out;
			TH0=high;
			TL0=low;
		}
	}
}
*/
void display(void)
{
		num = freqint;
		temp = num / 1000;
		num%=1000;
		P0 = SegOne; 
		P2 = hexvalue[temp]; 
		DELAY_ms(10);	

		temp = num / 100;
		num%=100;
		P0 = SegTwo; 
		P2 = hexvalue[temp];
		DELAY_ms(10);

		temp = num / 10;
		P0 = SegThree;
		P2 = hexvalue[temp];
		DELAY_ms(10);
	
		temp = num % 10;
		P0 = SegFour;
		P2 = hexvalue[temp];
		DELAY_ms(10);
}
void keypad_input(void)
{
	c1=c2=c3=r2=r3=r4=1;	
	r1=0;
	if(c1==0)
	{
		while(~c1);
		counter%=4;
		frequency[counter++]='1';
	}
	else if(c2==0)
	{
		while(~c2);
		counter%=4;
		frequency[counter++]='2';
	}
	else if(c3==0)
	{
		while(~c3);
		counter%=4;
		frequency[counter++]='3';
	}
	r2=0;r1=r3=r4=1;
	if(c1==0)
	{
		while(~c1);
		counter%=4;
		frequency[counter++]='4';
	}
	else if(c2==0)
	{
		while(~c2);
		counter%=4;
		frequency[counter++]='5';
	}
	else if(c3==0)
	{
		while(~c3);
		counter%=4;
		frequency[counter++]='6';
	}
	r3=0;r1=r2=r4=1;
	if(c1==0)
	{
		while(~c1);
		counter%=4;
		frequency[counter++]='7';
	}
	else if(c2==0)
	{
		while(~c2);
		counter%=4;
		frequency[counter++]='8';
	}
	else if(c3==0)
	{
		while(~c3);
		counter%=4;
		frequency[counter++]='9';
	}
	r4=0;r1=r2=r3=1;
	if(c2==0)
	{
		while(~c2);
		counter%=4;
		frequency[counter++]='0';
	}
	freqint=atoi(frequency);
}