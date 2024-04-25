/*
 * ISR_callback.c
 *
 *  Created on: Apr 25, 2024
 *      Author: Khoa
 */
#include "r_smc_entry.h"
#include "r_pinset.h"
#include "ISR_callback.h"
#include "rskrx65n_2mb.h"

void cb_switchpressed1(void){
	LED0_PDR=LED_OFF;
	LED1_PDR=LED_OFF;
	LED2_PDR=LED_OFF;
	LED3_PDR=LED_OFF;
}
void cb_switchpressed2(void){
	LED0_PDR=LED_ON;
	LED1_PDR=LED_ON;
	LED2_PDR=LED_ON;
	LED3_PDR=LED_ON;
}
