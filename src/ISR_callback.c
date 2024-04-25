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
	LED0_PDR=0;
	LED1_PDR=0;
	LED2_PDR=0;
	LED3_PDR=0;
}
void cb_switchpressed2(void){
	LED0_PDR=1;
	LED1_PDR=1;
	LED2_PDR=1;
	LED3_PDR=1;
}
