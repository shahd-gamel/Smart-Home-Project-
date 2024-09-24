#define F_CPU 16000000UL

#include <avr/io.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "DIO_interface.h"
#include "util/delay.h"
#include "CLCD_interface.h"
#include "Kypd.h"
#include "USART_interface.h"
#include "ADC_interface.h"

#include <string.h>
#define ID1 "203"
#define ID2 "206"
#define PASS1 "712"
#define PASS2 "305"
#define Room1 '1'
#define Room2 '2'
#define Led '1'
#define Fan '2'
#define On '1'
#define Off '2'
#define Back '3'
#define Yes '+'
#define No '-'
#define ExitKey '/'
#define C 'c'


int main(void)
{	CLCD_init() ;
	Kypad_init() ;
	USART_voidINIT() ;
	ADC_voidInit() ;
	DIO_enumSetPinDirection(DIO_PIN0 , DIO_PORTD , DIO_INPUT) ;
	DIO_enumSetPinDirection(DIO_PIN1 , DIO_PORTD , DIO_OUTPUT) ;
	DIO_enumSetPinDirection(DIO_PIN0 , DIO_PORTA , DIO_OUTPUT) ;
	DIO_enumSetPinDirection(DIO_PIN1 , DIO_PORTA , DIO_OUTPUT) ;
	DIO_enumSetPinDirection(DIO_PIN2 , DIO_PORTA , DIO_OUTPUT) ;
	DIO_enumSetPinDirection(DIO_PIN3 , DIO_PORTA , DIO_OUTPUT) ;
	DIO_enumSetPinDirection(DIO_PIN5 , DIO_PORTA , DIO_OUTPUT) ;
	DIO_enumSetPinDirection(DIO_PIN4 , DIO_PORTA , DIO_INPUT) ;
	DIO_enumSetPinDirection(DIO_PIN5 , DIO_PORTD , DIO_INPUT) ;
	DIO_enumSetPinDirection(DIO_PIN3 , DIO_PORTD , DIO_OUTPUT) ;
	DIO_enumSetPinDirection(DIO_PIN4 , DIO_PORTD , DIO_OUTPUT) ;
	DIO_enumSetPinValue(DIO_PIN5 , DIO_PORTD , DIO_HIGH) ;
	
	void doorbell_function(void) {
		U8 z;
		
		DIO_enumGetPinValue(DIO_PORTD, DIO_PIN5, &z); 

		if (z == DIO_LOW) 
		{ 
			DIO_enumSetPinValue(DIO_PIN4, DIO_PORTD, DIO_HIGH);
			CLCD_GoTo(1, 2);
			CLCD_SendString("Doorbell Ringing");
			_delay_ms(1000) ;
			CLCD_CLR() ; 
			} 
			else if (z == DIO_HIGH)
			{ 
			
			DIO_enumSetPinValue(DIO_PIN4, DIO_PORTD, DIO_LOW); 
			
			
		}
	}
	U8 check_heat_alert(void) {
		U16 result = ADC_u16GetChannalResult(ADC_CHANNAL_4);
		U8 *alert = " Heat Alert" ;
		U16 temp = (result*500UL)/ (1023) ; 
		if (temp > 30 ) {
			DIO_enumSetPinValue(DIO_PIN3, DIO_PORTD, DIO_HIGH);
			DIO_enumSetPinValue(DIO_PIN4, DIO_PORTD, DIO_HIGH);
			CLCD_CLR();
			CLCD_GoTo(1, 2);
			CLCD_SendString(alert);
			USART_VoidSendString("\r\n Heat Alert, Press C to resume\r\n");

			U16 key = 'f';
			while (key != C) {
				key = Kypad_GetKey();
			}
			
			DIO_enumSetPinValue(DIO_PIN3, DIO_PORTD, DIO_LOW);
			DIO_enumSetPinValue(DIO_PIN4, DIO_PORTD, DIO_LOW);
			CLCD_CLR();
			return key ;
		}
		return 0 ;
	}
	
	U8 *wel = "\r\nWelcome\r\n" ;
	U8 *ID = "Enter Your ID :\r\n ";
	U8 *pass = "\r\nEnter Password :\r\n " ;
	U8 *clo = "\r\nSystem Closed\r\n" ;
	U8 *try_again_msg = "Wrong ID. Try again? (+)Yes (-)No";
	U8 *wpass = "\r\nWrong Password" ;
	U8 *rom1 = "\r\n1- Room1 " ;
	U8 *rom2 = " 2- Room2 " ;
	U8 *led_msg = "\r\n1: LED ";
	U8 *fan_msg = " 2: Fan ";
	U8 *led_on = "\r\n1- Led  ON ";
	U8 *led_off = "2- Led Off " ;
	U8 *ledon = "\r\nLED ON" ;
	U8 *ledoff = "LED OFF " ;
	U8 *fanon = "\r\n1-Fan 0n " ;
	U8 *fanoff = "2-Fan Off ";
	U8 *fan_turn = " Fan is Turning ";
	U8 *fan_off = " Fan Off " ;
	U8 *back = "3- Back" ;
	U8 *star = '*' ;

	
	
	U8 entered_id [4] ;
	U8 entered_pass [4] ;
	U8 correct_id = 0;
	U8 room_choice ;
	U8 device_choice ;
	U8 valid_pass = 0;
	U8 choice ;
	U8 x , y ;
	U8 password_attempts = 0;
	U8 retry_decision;
	U16 result ;


	welc :USART_VoidSendString(wel) ;
	_delay_ms(1000);
	while (1)
	{
		Id:
		USART_VoidSendString(ID) ;
		
		for (U8 i = 0 ; i< 3 ; i++ )
		{ x = 'f' ;
			while (x =='f')
			{   if (check_heat_alert() == C)
			{
				goto welc ;
			}
				doorbell_function() ;
				x = Kypad_GetKey() ;
			}
			if (x == ExitKey) {
				USART_VoidSendString(wel) ;
				_delay_ms(1000);
				goto Id ;
				return;
			}
			
			entered_id[i] = x ;
			USART_voidTRANSMITR(entered_id[i]) ;
			_delay_ms(200) ;
			
		}
		entered_id[3] = '\0';
		if (strcmp((char*)entered_id, ID1) == 0 || strcmp((char*)entered_id, ID2) == 0)
		{
			doorbell_function() ;
			check_heat_alert() ;
			correct_id = 1;
			break ;
		}
		
		
		else
		{
			correct_id = 0;
			USART_VoidSendString(clo) ;
			_delay_ms(1000) ;
		}
	}
	
	while (correct_id && (password_attempts < 3))
	{
		doorbell_function() ;
		check_heat_alert() ;
		USART_VoidSendString(pass);

		for (U8 i = 0; i < 3; i++)
		{ y= 'f';
			while (y == 'f')
			{
				if (check_heat_alert() == C)
				{
					goto welc ;
				}
				doorbell_function() ;
				y = Kypad_GetKey();
			}
			if (y == ExitKey) {
				USART_VoidSendString(wel) ;
				_delay_ms(1000);
				goto Id ;
				return;
			}
			entered_pass [i] = y ;
			
			USART_voidTRANSMITR(star) ;
			_delay_ms(200) ;
		}
		entered_pass[3] = '\0';
		if (strcmp((char*)entered_pass, PASS1) == 0 || strcmp((char*)entered_pass, PASS2) == 0)
		{
			valid_pass = 1;
			break;
		}
		else
		{

			USART_VoidSendString(wpass);
			password_attempts++;
			_delay_ms(500);
		}
		if (password_attempts >=3)
		{
			USART_VoidSendString(clo);
		}
	}
	if (valid_pass)
	
	{	
		doorbell_function() ;
		check_heat_alert() ;
		room_selection :
		USART_VoidSendString(rom1);
		USART_VoidSendString(rom2) ;
		room_choice ='f' ;
		while (room_choice == 'f')
		{ 
			room_choice = Kypad_GetKey();
			if (check_heat_alert() == C)
			{
				goto welc ;
			}
			doorbell_function() ;
			
		}
		if (room_choice == ExitKey) {
			USART_VoidSendString(wel) ;
			_delay_ms(1000);
			goto Id ;
			return;
		}
		if (room_choice == Room1)
		{
			
			CLCD_GoTo(1,2);
			CLCD_SendString(" Room 1 ") ;
			USART_VoidSendString(led_msg) ;
			USART_VoidSendString(fan_msg) ;
			USART_VoidSendString(back);
			
			U8 room1_choice = 'f';
			while (room1_choice == 'f')
			{
				if (check_heat_alert() == C)
				{
					goto welc ;
				}
				doorbell_function() ;
				room1_choice = Kypad_GetKey() ;
			}
			if (room1_choice == ExitKey) {
				USART_VoidSendString(wel) ;
				_delay_ms(1000);
				goto Id ;
				return;
			}
			if (room1_choice ==Led)
			{
				USART_VoidSendString(led_on);
				USART_VoidSendString(led_off) ;
				
				U8 led_choice ='f' ;
				while (led_choice=='f')
				{
					if (check_heat_alert() == C)
					{
						goto welc ;
					}
					doorbell_function() ;
					led_choice =Kypad_GetKey() ;
				}
				
				if (led_choice == On)
				{
					
					CLCD_GoTo(2,1);
					CLCD_SendString(ledon) ;
					DIO_enumSetPinValue(DIO_PIN0 , DIO_PORTA , DIO_HIGH) ;
				}
				else if (led_choice == Off)
				{
					
					CLCD_GoTo(2,1);
					CLCD_SendString(ledoff) ;
					DIO_enumSetPinValue(DIO_PIN0 , DIO_PORTA , DIO_LOW) ;
				}
				
			}
			
			else if (room1_choice == Fan)
			{
				
				USART_VoidSendString(fanon) ;
				USART_VoidSendString(fanoff) ;
				
				U8 fan_choice = 'f' ;
				while (fan_choice=='f')
				{
					if (check_heat_alert() == C)
					{
						goto welc ;
					}
					doorbell_function() ;
					fan_choice = Kypad_GetKey();
				}
				if (fan_choice ==On)
				{
					CLCD_GoTo(2,1) ;
					CLCD_SendString(fan_turn) ;
					DIO_enumSetPinValue(DIO_PIN1 , DIO_PORTA , DIO_HIGH) ;
				}
				else if (fan_choice == Off)
				{
					CLCD_GoTo( 2,1 ) ;
					CLCD_SendString(fan_off) ;
					DIO_enumSetPinValue(DIO_PIN1 , DIO_PORTA , DIO_LOW) ;
				}
				
			}
			else if (room1_choice == Back)
			{
				CLCD_CLR();
				goto room_selection ;
			}
		}
		else if (room_choice == Room2)
		{
			
			CLCD_GoTo(1,2);
			CLCD_SendString(" Room 2 ") ;
			USART_VoidSendString(led_msg) ;
			USART_VoidSendString(fan_msg) ;
			USART_VoidSendString(back);
			U8 room2_choice = 'f';
			while (room2_choice == 'f')
			{
				if (check_heat_alert() == C)
				{
					goto welc ;
				}
				doorbell_function() ;
				room2_choice = Kypad_GetKey() ;
			}
			if (room2_choice ==Led)
			{
				
				USART_VoidSendString(led_on);
				USART_VoidSendString(led_off) ;
				
				U8 led_choice ='f' ;
				while (led_choice=='f')
				{
					if (check_heat_alert() == C)
					{
						goto welc ;
					}
					doorbell_function() ;
					led_choice =Kypad_GetKey() ;
				}
				if (led_choice == On)
				{
					
					CLCD_GoTo(2,1);
					CLCD_SendString(ledon) ;
					DIO_enumSetPinValue(DIO_PIN2 , DIO_PORTA , DIO_HIGH) ;
				}
				else if (led_choice == Off)
				{
					
					CLCD_GoTo(2,1);
					CLCD_SendString(ledoff) ;
					DIO_enumSetPinValue(DIO_PIN2 , DIO_PORTA , DIO_LOW) ;
				}
				
			}
			
			else if (room2_choice == Fan)
			{
				
				USART_VoidSendString(fanon) ;
				USART_VoidSendString(fanoff) ;
				
				U8 fan_choice = 'f' ;
				while (fan_choice=='f')
				{
					if (check_heat_alert() == C)
					{
						goto welc ;
					}
					doorbell_function() ;
					fan_choice = Kypad_GetKey();
				}
				if (fan_choice == On)
				{
					CLCD_GoTo(2,1) ;
					CLCD_SendString(fan_turn) ;
					DIO_enumSetPinValue(DIO_PIN3 , DIO_PORTA , DIO_HIGH) ;
				}
				else if (fan_choice == Off)
				{
					CLCD_GoTo( 2,1 ) ;
					CLCD_SendString(fan_off) ;
					DIO_enumSetPinValue(DIO_PIN3 , DIO_PORTA , DIO_LOW) ;
				}
				
			}
			else if (room2_choice == Back)
			{
				CLCD_CLR();
				goto room_selection ;
			}
		}
	}
	
	
	
	return 0;
}
