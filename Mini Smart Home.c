#define F_CPU 16000000UL

#include <avr/io.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "DIO_interface.h"
#include "util/delay.h"
#include "CLCD_interface.h"
#include "Kypd.h"
#define ID1 '5'
#define ID2 '4'
#define PASS1 '7'
#define PASS2 '3'
#define Room1 '1'
#define Room2 '2'
#define Led '1' 
#define Fan '2'
#define On '1'
#define Off '2'

int main(void)
{
    CLCD_init();
    Kypad_init() ;
	DIO_enumSetPinDirection(DIO_PIN0 , DIO_PORTA , DIO_OUTPUT);
	DIO_enumSetPinDirection(DIO_PIN2 , DIO_PORTA , DIO_OUTPUT);
	U8 *wel = " Welcome" ;
	U8 *ID = " Enter Your ID :"; 
	U8 *pass = " Enter Password : " ;
	U8 *clo = " System Closed" ; 
	U8 *wpass = " Wrong Password" ;
	U8 *rom1 = " 1- Room1 " ;
	U8 *rom2 = " 2- Room2 " ;
	
	U8 *led_msg = " 1: LED ";
	U8 *fan_msg = " 2: Fan ";
	U8 *led_on = " 1- Led  ON ";
	U8 *led_off = " 2- Led Off " ;
	U8 *ledon = " LED ON" ;
	U8 *ledoff = " LED OFF " ;
	U8 *fanon = " 1-Fan 0n " ;
	U8 *fanoff = " 2-Fan Off ";
	U8 *fan_turn = " Fan is Turning ";
	U8 *fan_off = " Fan Off " ;
	
	U8 entered_id ; 
	U8 entered_pass ; 
	U8 tries = 0  ; 
	U8 correct_id = 0;
	U8 room_choice ;
	U8 device_choice ; 
	U8 valid_pass = 0;
	U8 choice ;
	
	
	CLCD_GoTo(1,2) ;
	CLCD_SendString(wel);
	_delay_ms (1000) ;

	CLCD_GoTo(1,1) ;
	CLCD_SendString(ID) ;
    while (1) 
    {
		entered_id = Kypad_GetKey() ; 

		if (entered_id !='f')
		{
		
			CLCD_GoTo(2,1) ;
			CLCD_SendString(entered_id) ; 
			if ((entered_id == ID1) || (entered_id == ID2) )
			{
				correct_id =1 ;
				CLCD_GoTo(2,1) ;
				CLCD_SendString(entered_id) ;
				break ;
			}
			else 
			{
				CLCD_CLR() ;
				CLCD_GoTo(1,2) ;
				CLCD_SendString(clo);
				 _delay_ms(1000);
				 CLCD_CLR();
				 CLCD_GoTo(1, 1);
				 CLCD_SendString(ID);
				
			}
			
					
				}
				
	}
	if (correct_id == 1)
	{
		CLCD_CLR();
		CLCD_GoTo(1, 2);
		CLCD_SendString(pass);
		entered_pass = Kypad_GetKey() ;
		while (1)
		{
			entered_pass = Kypad_GetKey() ;
			if (entered_pass != 'f')
			{
				CLCD_GoTo(2,1);
				CLCD_SendString(entered_pass);
			   if ((entered_pass == PASS1 && entered_id == ID1) || (entered_pass == PASS2 && entered_id == ID2))
			   {
				   valid_pass = 1;
				   break;
			   }
				  else 
				  { 
				    CLCD_CLR();
					CLCD_GoTo(1,2) ;
				    CLCD_SendString(wpass) ;
				    tries++ ;
					_delay_ms(500);
					CLCD_CLR();
					CLCD_GoTo(1,2) ;
					CLCD_SendString(pass) ;
				   entered_pass = Kypad_GetKey() ;
				    
				   if (tries >= 3)
				   {
					   CLCD_CLR();
					   CLCD_GoTo( 1 , 2) ;
					   CLCD_SendString(clo) ;
					   break ; 
				   }
			   }
			   
			   
			}
		}
	}
	if (valid_pass==1)
	{
		CLCD_CLR();
	CLCD_GoTo(1,2) ;
	CLCD_SendString(rom1) ;
	CLCD_GoTo(2 , 1 ) ;
	CLCD_SendString(rom2) ;
		while (1)
		{
				room_choice = Kypad_GetKey() ; 
				if (room_choice == Room1 || room_choice == Room2)
				{
					CLCD_CLR();
					CLCD_GoTo(1,2);
					CLCD_SendString(led_msg);
					CLCD_GoTo(2,1);
					CLCD_SendString(fan_msg) ;
					while (1)
					{
					device_choice = Kypad_GetKey();
					if (device_choice == Led)
					{
						
						CLCD_CLR();
						CLCD_GoTo(1,2);
						CLCD_SendString(led_on) ;
						CLCD_GoTo(2,1);
						CLCD_SendString(led_off);
				while (1)  // Loop until a valid choice is made
					{
							choice = Kypad_GetKey();
							if (choice == On)
						{
							CLCD_CLR();
							CLCD_GoTo(1,2);
							CLCD_SendString(ledon);
							DIO_enumSetPinValue(DIO_PIN0 , DIO_PORTA , DIO_HIGH) ; 
							break;
						}
						else if (choice == Off)
						{
							CLCD_CLR();
							CLCD_GoTo(1,2);
							CLCD_SendString(ledoff);
							DIO_enumSetPinValue(DIO_PIN0 , DIO_PORTA , DIO_LOW) ;
							break ;
						}
					
					}
					}
					else if (device_choice == Fan)
					{
						
						CLCD_CLR();
						CLCD_GoTo(1,2) ;
						CLCD_SendString(fanon) ;
						CLCD_GoTo(2,1);
						CLCD_SendString(fanoff);
			while (1)  // Loop until a valid choice is made
			{
						choice = Kypad_GetKey();
					
					if (choice == On)
					{
						CLCD_CLR();
						CLCD_GoTo(1,2) ;
						CLCD_SendString(fan_turn) ;
						DIO_enumSetPinValue(DIO_PIN2 , DIO_PORTA , DIO_HIGH) ;
						break;
					}
					else if (choice == Off)
					{
						CLCD_CLR();
						CLCD_GoTo(1,2) ;
						CLCD_SendString(fan_off) ;
						DIO_enumSetPinValue(DIO_PIN2 , DIO_PORTA , DIO_LOW) ;
						break;
					}
				
					}
					}
				}
			
			}
		  }
		}
    
	return 0 ;
}
