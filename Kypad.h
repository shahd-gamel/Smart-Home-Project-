#ifndef HAL_KYPD_KYPD_H_
#define HAL_KYPD_KYPD_H_
#include "DIO_interface.h"


#define FIRSTCOL  DIO_PIN4
#define FIRSTRW   DIO_PIN0





void Kypad_init(void);
U8   Kypad_GetKey(void);
