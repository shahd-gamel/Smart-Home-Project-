#include "BIT_MATH.h"
#include "STD_TYPES.h"
#include "Kypd.h"
#include "DIO_interface.h"
#include "DIO_privet.h"


U8 Key_arr[4][4]={{'7','8','9','/'}//r0
                 ,{'4','5','6','*'}//r1
                 ,{'1','2','3','-'}//r2
                 ,{'c','0','=','+'}/*r3*/};

void Kypad_init(void)
{
	for(U8 i=0;i<4;i++)
	{
		/*SET Direction of col----->input*/
		DIO_enumSetPinDirection(FIRSTCOL+i,DIO_PORTB,DIO_INPUT);
	}
	for(U8 i=0;i<4;i++)
	{
		/*Set Direction of RW------->output*/
		DIO_enumSetPinDirection(FIRSTRW+i,DIO_PORTB,DIO_OUTPUT);
	}
	for(U8 i=0;i<4;i++)
	{
		/*SET Values of RW------->1*/
		DIO_enumSetPinValue(FIRSTRW+i,DIO_PORTB,DIO_HIGH);
	}
	for(U8 i=0;i<4;i++)
	{
		/*SET Values of COL------->Pull UP*/
		DIO_enumSetPinValue(FIRSTCOL+i,DIO_PORTB,DIO_HIGH);
	}

}
/*set any switch pressed&return this sw&loop 16 sw*/
U8   Kypad_GetKey(void)
{
   U8 COLITERATION=0;
   U8 RWITERATION=0;
   U8 RETURN_VAL='f';
   U8 KEY_VAL;
   for(RWITERATION=0;RWITERATION<4;RWITERATION++)
   {
	   DIO_enumSetPinValue(RWITERATION,DIO_PORTB,DIO_LOW);

       for(COLITERATION=4;COLITERATION<8;COLITERATION++)
       {

    	   DIO_enumGetPinValue(DIO_PORTB,COLITERATION,&KEY_VAL);/*DIO GET PIN VALUE*/
    	   if(KEY_VAL==0)
    	   {
    		   RETURN_VAL=Key_arr[RWITERATION][COLITERATION-4];
    	   }

    	   while(GET_BIT(PINB_Reg,COLITERATION)==0);
       }
       DIO_enumSetPinValue(RWITERATION,DIO_PORTB,DIO_HIGH);
   }

void Print_Binary(U8 number)
{
	U8 i;
	char binaryStr[9]; // 8 bits + null terminator for 8-bit binary number

	for (i = 0; i < 8; i++)
	{
		// Extract the bit at position (7 - i) and store it in the string
		binaryStr[i] = (number & (1 << (7 - i))) ? '1' : '0';
	}
	binaryStr[8] = '\0'; // Null-terminate the string

	// Display the binary string on the LCD
	CLCD_SendString(binaryStr);
}

return RETURN_VAL;

}
