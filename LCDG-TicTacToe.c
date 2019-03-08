#include <p32xxxx.h>

#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8

void initPortD(void);
void initPortB(void);
void initPortE(void);
void initPortF(void);
void initPortG(void);

int checkfull();
void initLcd(void);
void writeXY(int x,int y,int lcd_cs);
void writeLcd(unsigned int num,int lcd_cs);
void printBorder();
void printX(int r,int c);
void printC(int r,int c);
int scan_key(void);
void getPlayerMove(int player);
void delay(int j);
void busy(void);
void print_lcd(char* msg1,char* msg2,int player);
int check_win();
int checkEmpty(int row,int col,int player);
void initTimer();
void WinningLed();
char control[6]={0x38,0x38,0x38,0xe,0x6,0x1};
unsigned int row[3] ={0,3,6};
unsigned int colomn[3] ={0,22,44};
unsigned int mat[3][3]={{0,0,0},{0,0,0},{0,0,0}};
unsigned int Cup[16] = {
	0x80,
	0x40,
	0x20,
	0x10,
	0x8,
	0x4,
	0x2,
	0x1,
	0x1,
	0x2,
	0x4,
	0x8,
	0x10,
	0x20,
	0x40,
	0x80,
};

unsigned int Cdown[16] = {
	0x1,//0b01000100,
	0x2,//0b01001000,
	0x4,//0b11110000,
	0x8,//0b00001000,
	0x10,//0b00000100,
	0x20,//0b01100010,
	0x40,//0b01100001,
	0x80,//0b00000010,
	0x80,//0b01000100,
	0x40,//0b01001000,
	0x20,//0b11110000,
	0x10,//0b00001000,
	0x8,//0b00000100,
	0x4,//0b01100010,
	0x2,
	0x1,
};
unsigned int Xup[16] = {
	0x1,//0b01000100,
	0x2,//0b01001000,
	0x4,//0b11110000,
	0x8,//0b00001000,
	0x10,//0b00000100,
	0x20,//0b01100010,
	0x40,//0b01100001,
	0x80,//0b00000010,
	0x80,//0b01000100,
	0x40,//0b01001000,
	0x20,//0b11110000,
	0x10,//0b00001000,
	0x8,//0b00000100,
	0x4,//0b01100010,
	0x2,//0b01100001,
	0x1,//0b00000010,
};

unsigned int Xdown[16] = {
	0x80,//0b01000100,
	0x40,//0b01001000,
	0x20,//0b11110000,
	0x10,//0b00001000,
	0x8,//0b00000100,
	0x4,//0b01100010,
	0x2,//0b01100001,
	0x1,//0b00000010,
	0x1,//0b01000100,
	0x2,//0b01001000,
	0x4,//0b11110000,
	0x8,//0b00001000,
	0x10,//0b00000100,
	0x20,//0b01100010,
	0x40,//0b01100001,
	0x80,//0b00000010,
};						

int checkfull()
{
	int i,j;
	
	for (i=0;i<3;i++)
		for (j=0;j<3;j++)
			if (mat[i][j]==0)
				return 0;
	return 1;
}

int main()
{
	unsigned int i,j,player=1;
	int winFlag;

	initPortD();
	initPortB();
	initPortE();
	initPortF();
	initPortG();

	
	initLcd();

	//clear
	for(i = 0;i < 8;i++)
	{
		for(j = 0;j < 64;j++)
		{
			writeXY(j,i,1);
			writeLcd(0x00,1);
			writeXY(j,i,2);
			writeLcd(0x00,2);
		}
	}

	printBorder()	;


	while(1)
	{
		if (checkfull()==1)
		{
			print_lcd("TIE "," PLAYERS",2);
			delay(100000);
			break;
		}
		print_lcd("player "," turn",player);
		delay(100000);
		initTimer();
		getPlayerMove(player);
		winFlag=check_win();
		
		if (winFlag==1)
		{
			print_lcd("p- "," Very Good!!",1);
			WinningLed();
			break;
		}
		else
			if (winFlag==2)
			{
				print_lcd("p- "," Very Good!!",2);
				WinningLed();
				break;
			}
		
		delay(100000);
		if (player==1)
			player=2;
		else
			player=1;
		delay(100000);


	}


	return;
}

void printBorder()
{
	int i;	
	//border left vertical ------------------
	writeXY(20,0,1);
	writeLcd(0xff,1);
	writeXY(20,1,1);
	writeLcd(0xff,1);	

	writeXY(20,2,1);
	writeLcd(0xff,1);
	writeXY(20,3,1);
	writeLcd(0xff,1);

	writeXY(20,4,1);
	writeLcd(0xff,1);
	writeXY(20,5,1);
	writeLcd(0xff,1);

	writeXY(20,6,1);
	writeLcd(0xff,1);
	writeXY(20,7,1);
	writeLcd(0xff,1);
	//-----------------------------

	//border right vertical ------------------
	writeXY(40,0,1);
	writeLcd(0xff,1);
	writeXY(40,1,1);
	writeLcd(0xff,1);	

	writeXY(40,2,1);
	writeLcd(0xff,1);
	writeXY(40,3,1);
	writeLcd(0xff,1);

	writeXY(40,4,1);
	writeLcd(0xff,1);
	writeXY(40,5,1);
	writeLcd(0xff,1);

	writeXY(40,6,1);
	writeLcd(0xff,1);
	writeXY(40,7,1);
	writeLcd(0xff,1);


	//border top horizintal ------------------

	for(i = 0;i < 64;i++)
	{
		writeXY(i,2,1);
		writeLcd(0x04,1);
	}

	//-----------------------------
	//border top horizintal ------------------

	for(i = 0;i < 64;i++)
	{
		writeXY(i,5,1);
		writeLcd(0x04,1);
	}

	//-----------------------------

}


void printX(int r,int c)
{
	int i;
	writeXY(colomn[c],row[r],1);
	for(i = 0;i < 16;i++)
		writeLcd(Xup[i],1);
	writeXY(colomn[c],row[r]+1,1);
	for(i = 0;i < 16;i++)
		writeLcd(Xdown[i],1);
}

void printC(int r,int c)
{
	int i;
	writeXY(colomn[c],row[r],1);
	for(i = 0;i < 16;i++)
		writeLcd(Cup[i],1);
	writeXY(colomn[c],row[r]+1,1);
	for(i = 0;i < 16;i++)
		writeLcd(Cdown[i],1);
}

void initPortB(void)
{   unsigned int portMap;
	portMap = TRISB;
	portMap &= 0xFFFF7FFF;
	TRISB = portMap;
	
	AD1PCFG = 0x7fff; //Select PORTB to be digital port input
	CNCONbits.ON = 0; //Change Notice Module On bit CN module is disabled
	CNEN = 0x3C;	
	CNPUE = 0x3C;  	//Set RB0 - RB3 as inputs with weak pull-up
	CNCONbits.ON = 1; // 1 = CN module is enabled
}


void initPortD(void)
{
	unsigned int portMap;
	portMap = TRISD;
	portMap &= 0xFFFFFFCF;
	TRISD = portMap;
}

void initPortE(void)
{
	unsigned int portMap;
	portMap = TRISE;
	portMap &= 0xFFFFFF00;
	TRISE = portMap;
	PORTE = 0x00;
}

void initPortF(void)
{
	unsigned int portMap;
	portMap = TRISF;
	portMap &= 0xFFFFFEF8;
	TRISF = portMap;
	PORTFbits.RF8 = 1;
}

void initPortG(void)
{
	unsigned int portMap;
	portMap = TRISG;
	portMap &= 0xFFFF7FFC;
	TRISG = portMap;
	PORTG = 0x00;
}



void initLcd(void)
{	
	int CONTROL[4] = {0x40,0xB8,0xFF,0x3F};
	int i;

	PORTDbits.RD5 = 0;
	PORTBbits.RB15 = 0;
	PORTF = 0x01;
	PORTDbits.RD7 = 0;
	PORTDbits.RD7 = 1;
	PORTF = 0x02;
	PORTDbits.RD7 = 0;
	PORTDbits.RD7 = 1;
	PORTFbits.RF8 = 1;

	for(i = 0;i < 4;i++)
	{
		PORTE = CONTROL[i];
		PORTF = 0x01;
		PORTDbits.RD4 = 1;//enable=1
		PORTDbits.RD4 = 0;//enable=0
		delay(1500);
		PORTF = 0x02;
		PORTDbits.RD4 = 1;//enable=1
		PORTDbits.RD4 = 0;//enable=0
		delay(1500);	
	}
	PORTFbits.RF8 = 1;
}

void writeXY(int x,int y,int lcd_cs)
{
	PORTDbits.RD5 = 0;
	PORTBbits.RB15 = 0;
	PORTF = lcd_cs;
	PORTE = 0x40 + x;
	PORTDbits.RD4 = 1;//enable=1
	PORTDbits.RD4 = 0;//enable=0
	delay(1500);
	PORTE = 0xB8 + y;
	PORTDbits.RD4 = 1;//enable=1
	PORTDbits.RD4 = 0;//enable=0
	delay(1500);
	PORTFbits.RF8 = 1;
}

void writeLcd(unsigned int num,int lcd_cs)
{
	int i;

	PORTDbits.RD5 = 0;
	PORTBbits.RB15 = 1;
	PORTF = lcd_cs;
	PORTE = num;
	PORTDbits.RD4 = 1;//enable=1
	PORTDbits.RD4 = 0;//enable=0
	delay(1500);
	PORTFbits.RF8 = 1;
}



int scan_key(void)
{
	int RUN_ZERO[4] = {0xee,0xdd,0xbb,0x77};
	int scan_key_code_ascii[32]={0xee,'1',0xde,'2',0xbe,'3',0x7e,'A',
		0xed,'4',0xdd,'5',0xbd,'6',0x7d,'B',
		0xeb,'7',0xdb,'8',0xbb,'9',0x7b,'C',
		0xe7,'*',0xd7,'0',0xb7,'#',0x77,'D'};  
	int i,num_code;
	unsigned int keyVal;
	int  column=0;
	int  flag=0;  
	PORTG = 0x00;
	PORTF = 0x07;
	for(i=0;i<5;i++)
	{
		PORTE = RUN_ZERO[column];
		delay(5);	
		keyVal = PORTB & 0x0F;
		if(keyVal != 0x0f)
		{ flag=1;break;}
		column++;
		if(column==4)column = 0;//טור
	}

	if(flag==1)
	{	  num_code=((RUN_ZERO[column]&0xf0)|(keyVal));
	for(i=0;i<32;i+=2)
	{     
		if(num_code==scan_key_code_ascii[i])
		{
			i=scan_key_code_ascii[i+1];  
			break;
		}		 
	}
	}       
	else
		i=0xff;
	delay(1000);
	return(i);
} 

void initTimer()
{
	T2CONbits.ON = 0;
	T3CONbits.ON = 0;
	T2CONbits.TGATE=0;
	T2CONbits.TCS=0;
	T2CONbits.T32=1;
	T2CONbits.TCKPS0=1;
	T2CONbits.TCKPS1=1;
	T2CONbits.TCKPS2=1;
	T1CONbits.TSYNC=0;

	PR2=0xf5e1;
	PR3=5;	
	TMR3=0;
	TMR3=0;
	IFS0bits.T2IF=0;
	IFS0bits.T3IF=0;
	T2CONbits.ON=1;
	
}

int checkEmpty(int row,int col,int player)
{
	int res = 1;
	if  (mat[row][col] != 0)
	{
		print_lcd("p- "," Wrong choise",player);
		res=0;
	}
	return res;
}

void getPlayerMove(int player)
{
	char keyboard,i; 
	int flag_move=0;
	while (flag_move==0 && (!IFS0bits.T3IF))
	{
		keyboard = scan_key();

		if(keyboard!=0xff)
		{

			switch (keyboard)
			{
			case '1':
				if (checkEmpty(0,0,player) == 1)
				{
					if (player==1)
					{
						printX(0,0);
						flag_move=1;
					}
					else
					{
						printC(0,0);
						flag_move=1;
					}
					mat[0][0] = player;
				}
				break;

			case '2':
				if (checkEmpty(0,1,player) == 1)
				{
					if (player==1)
					{
						printX(0,1);
						flag_move=1;
					}
					else
					{
						printC(0,1);
						flag_move=1;
					}
					mat[0][1] = player;
				}
				break;

			case '3':
				if (checkEmpty(0,2,player) == 1)
				{
					if (player==1)
					{
						printX(0,2);
						flag_move=1;
					}
					else
					{
						printC(0,2);
						flag_move=1;
					}
					mat[0][2] = player;
				}
				break;

			case '4':
				if (checkEmpty(1,0,player) == 1)
				{
					if (player==1)
					{
						printX(1,0);
						flag_move=1;
					}
					else
					{
						printC(1,0);
						flag_move=1;
					}
					mat[1][0] = player;
				}
				break;

			case '5':
				if (checkEmpty(1,1,player) == 1)
				{
					if (player==1)
					{
						printX(1,1);
						flag_move=1;
					}
					else
					{
						printC(1,1);
						flag_move=1;
					}
					mat[1][1] = player;
				}
				break;

			case '6':
				if (checkEmpty(1,2,player) == 1)
				{
					if (player==1)
					{
						printX(1,2);
						flag_move=1;
					}
					else
					{
						printC(1,2);
						flag_move=1;
					}
					mat[1][2] = player;
				}
				break;

			case '7':
				if (checkEmpty(2,0,player) == 1)
				{
					if (player==1)
					{
						printX(2,0);
						flag_move=1;
					}
					else
					{
						printC(2,0);
						flag_move=1;
					}
					mat[2][0] = player;
				}
				break;

			case '8':
				if (checkEmpty(2,1,player) == 1)
				{
					if (player==1)
					{
						printX(2,1);
						flag_move=1;
					}
					else
					{
						printC(2,1);
						flag_move=1;
					}
					mat[2][1] = player;
				}
				break;

			case '9':
				if (checkEmpty(2,2,player) == 1)
				{
					if (player==1)
					{
						printX(2,2);
						flag_move=1;
					}
					else
					{
						printC(2,2);
						flag_move=1;
					}
					mat[2][2] = player;
				}
				break;
			}
		}

	}
	if (flag_move==0)
	{
		print_lcd("p-","Time is Over",player);
		delay(2000000);
	}
}

void delay(int j)
{
	unsigned int i;

	for(i=0;i<j;i++);
}


void busy(void)
{ char RD,RS;
int STATUS_TRISE,portMap;
RD=PORTDbits.RD5;
RS=PORTBbits.RB15;
STATUS_TRISE=TRISE;
PORTDbits.RD5 = 1;//w/r
PORTBbits.RB15 = 0;//rs 
portMap = TRISE;
portMap |= 0x80;
TRISE = portMap;
do
{
	PORTDbits.RD4=1;//enable=1
	PORTDbits.RD4=0;//enable=0
}
while(PORTEbits.RE7);// BF בדיקה דגל
PORTDbits.RD5=RD; 
PORTBbits.RB15=RS;
TRISE=STATUS_TRISE;   
}


void print_lcd(char* msg1,char* msg2,int player)
{
	unsigned int portMap;
	char* string;
	int i;

	portMap = TRISB;
	portMap &= 0xFFFF7FFF;
	TRISB = portMap;

	portMap = TRISF;
	portMap &= 0xFFFFFEF8;
	TRISF = portMap;
	PORTFbits.RF8 = 1;

	portMap = TRISE;
	portMap &= 0xFFFFFF00;
	TRISE = portMap;

	portMap = TRISD;
	portMap &= 0xFFFFFFCF;
	TRISD = portMap;

	PORTDbits.RD5 = 0;//w/r
	PORTBbits.RB15 = 0;//rs
	PORTF = 0x00;
	for(i = 0;i < 6;i++)
	{
		PORTE=control[i];
		PORTDbits.RD4=1;//enable=1
		PORTDbits.RD4=0;//enable=0
		busy();
		PORTDbits.RD5 = 0;//w/r

	}

	PORTDbits.RD5 = 0;//w/r
	PORTBbits.RB15 = 1;//rs
	PORTF = 0x00;
	for(i = 0;i < strlen(msg1);i++)
	{
		PORTE=*(msg1+i);
		PORTDbits.RD4=1;//enable=1
		PORTDbits.RD4=0;//enable=0
		busy();
		PORTDbits.RD5 = 0;//w/r	
		PORTBbits.RB15 = 1;//rs

	}

	PORTE=player+'0';
	PORTDbits.RD4=1;//enable=1
	PORTDbits.RD4=0;//enable=0
	busy();
	PORTDbits.RD5 = 0;//w/r	
	PORTBbits.RB15 = 1;//rs




	for(i = 0;i < strlen(msg2);i++)
	{
		PORTE=*(msg2+i);
		PORTDbits.RD4=1;//enable=1
		PORTDbits.RD4=0;//enable=0
		busy();
		PORTDbits.RD5 = 0;//w/r	
		PORTBbits.RB15 = 1;//rs

	}   

}

int check_win()
{
	int i;
	int result = 0;
	for (i=0;i<9;i++)
	{
		switch(i)
		{
		case 0: //left up
			if ( (((mat[0][0] == mat[0][1])  && (mat[0][1]==mat[0][2])) ||
				 ((mat[0][0] == mat[1][0])  && (mat[1][0]==mat[2][0])) ||
				 ((mat[0][0] == mat[1][1])  && (mat[1][1]==mat[2][2]))) && 
				  mat[0][0] != 0 
				)
				result =   mat[0][0];
				break;
		case 1: //middle up
			if ( (((mat[0][1] == mat[0][0])  && (mat[0][0]==mat[0][2])) ||
				 ((mat[0][1] == mat[1][1])  && (mat[1][1]==mat[2][1])))  && 
				  mat[0][1] != 0 
				  )
				result =   mat[0][1];
		break;
		case 2: //right up
			if ( (((mat[0][2] == mat[0][1])  && (mat[0][1]==mat[0][0])) ||
				 ((mat[0][2] == mat[1][2])  && (mat[1][2]==mat[2][2])) ||
				 ((mat[0][2] == mat[1][1])  && (mat[1][1]==mat[2][0]))) && 
				  mat[0][0] != 0 
				)
				result =  mat[0][2];
		break;
		case 3: //middle left
			if ( (((mat[1][0] == mat[0][0])  && (mat[0][0]==mat[2][0])) ||
				 ((mat[1][0] == mat[1][1])  && (mat[1][1]==mat[1][2])))  && 
				  mat[1][0] != 0 
				  )
				result =  mat[1][0];
		break;
		case 4: //middle middle
			if ( (((mat[1][1] == mat[1][0])  && (mat[1][0]==mat[1][2])) ||
				 ((mat[1][1] == mat[0][1])  && (mat[0][1]==mat[2][1])) ||
				 ((mat[1][1] == mat[0][0])  && (mat[0][0]==mat[2][2])) ||
				 ((mat[1][1] == mat[0][2])  && (mat[0][2]==mat[2][0]))) && 
				  mat[1][1] != 0 
				)
				result =  mat[1][1];
		break;
		case 5: //middle right
			if ( (((mat[1][2] == mat[1][1])  && (mat[1][1]==mat[1][0])) ||
				 ((mat[1][2] == mat[0][2])  && (mat[0][2]==mat[2][2])))  && 
				  mat[1][2] != 0 
				  )
				result =   mat[1][2];
		break;
		case 6: //bottom left
			if ( (((mat[2][0] == mat[2][1])  && (mat[2][1]==mat[2][2])) ||
				 ((mat[2][0] == mat[1][1])  && (mat[1][1]==mat[0][2])) ||
				 ((mat[2][0] == mat[1][0])  && (mat[1][0]==mat[0][0])))  && 
				  mat[2][0] != 0 
				  )
				result =   mat[2][0];
		break;
		case 7: //bottom middle 
			if ( (((mat[2][1] == mat[2][0])  && (mat[2][0]==mat[2][2])) ||
				 ((mat[2][1] == mat[1][1])  && (mat[1][1]==mat[0][1])))  && 
				  mat[2][1] != 0 
				  )
				result =  mat[2][1];
		break;
		case 8: //bottom right
			if ( (((mat[2][2] == mat[2][1])  && (mat[2][1]==mat[2][0])) ||
				 ((mat[2][2] == mat[1][2])  && (mat[1][2]==mat[0][2])) ||
				 ((mat[2][2] == mat[1][1])  && (mat[1][1]==mat[0][0])))  && 
				  mat[2][2] != 0 
				  )
				result =  mat[2][2];
		break;
		}
	}

	return result;
}

void WinningLed()
{
	int i,j;
	unsigned char left,right,x;
	
		while(1){
		TRISE=0;
		PORTF=4;
		x = 129;
		for(i=0;i<4;i++)
		{
			PORTE=x;
	        PORTDbits.RD4=1;
	        PORTDbits.RD4=0;
			delay(60000);
			
	
			left = x&(0XF0);
			right = x&(0X0F);
			left >>=1;
			right <<=1;
			
			x = (right + left);

			TRISE=0;
			PORTF=4;  
		}

		x = 24;
		for(i=0;i<4;i++)
		{
			PORTE=x;
	        PORTDbits.RD4=1;
	        PORTDbits.RD4=0;
			
	       delay(60000);
	
			left = x&(0XF0);
			right = x&(0X0F);
			left <<=1;
			right >>= 1;
			
			x = (right + left);
			
			TRISE=0;
			PORTF=4; 
		}
	}
}