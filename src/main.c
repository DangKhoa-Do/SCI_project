/***********************************************************************
*
*  FILE        : ProjectSCI.c
*  DATE        : 2024-04-25
*  DESCRIPTION : Main Program
*
*  NOTE:THIS IS A TYPICAL EXAMPLE.
*
***********************************************************************/
#include "r_smc_entry.h"
#include "r_sci_rx_if.h"
#include "r_pinset.h"
#include "Config_ICU.h"
#include "rskrx65n_2mb.h"
//#include "ISR_callback.h"

//handle
static sci_hdl_t sci_handle;

//private, global variables and functions
//variables


//functions
void SCIcallback(void *p_args);


void main(void);

void main(void)
{
	//Init Switches
	R_Config_ICU_IRQ11_Start(); //Switch 1
	R_Config_ICU_IRQ13_Start(); //Switch 2

	//var for SCI
	sci_err_t sci_err;
	sci_cfg_t configUART;
	uint8_t received_char;

	R_SCI_PinSet_SCI8();

	//config SCI async channel
	configUART.async.baud_rate	= 115200;
	configUART.async.clk_src  = SCI_CLK_INT;
	configUART.async.data_size   = SCI_DATA_8BIT;
	configUART.async.parity_en  = SCI_PARITY_OFF;
	configUART.async.parity_type = SCI_EVEN_PARITY;
	configUART.async.stop_bits = SCI_STOPBITS_1;
	configUART.async.int_priority = 3;

	//open async channel
	sci_err = R_SCI_Open(SCI_CH8, SCI_MODE_ASYNC, &configUART, SCIcallback, &sci_handle);

	//check if successfully opening channel
	if (sci_err!=SCI_SUCCESS){
		nop();
	}
	//print to console if successfully initialize
	printf("SCI initialize successfully\n");

	//check the first character after initialize
	do{
		sci_err = R_SCI_Receive(sci_handle,&received_char,1);
	}while (sci_err==SCI_ERR_INSUFFICIENT_DATA);
	printf("Received first character.\n");


	//loop
	while (1){
		//wait for character from the terminal
		do{
				sci_err = R_SCI_Receive(sci_handle,&received_char,1);
		}while (sci_err==SCI_ERR_INSUFFICIENT_DATA);
		//to do after received character
		if(received_char=='r'){
			LED0_PDR=LED_OFF;
			LED1_PDR=LED_OFF;
			LED2_PDR=LED_OFF;
			LED3_PDR=LED_OFF;
		R_SCI_Send(sci_handle,"LED OFF",8);
		}
		else{
			switch (received_char){
			case '1':
				LED0_PDR=LED_ON;
				R_SCI_Send(sci_handle,"LED 0 ON ",9);
				break;
			case '2':
				LED1_PDR=LED_ON;
				R_SCI_Send(sci_handle,"LED 1 ON ",8);
				break;
			case '3':
				LED2_PDR=LED_ON;
				R_SCI_Send(sci_handle,"LED 2 ON ",8);
				break;
			case '4':
				LED3_PDR=LED_ON;
				R_SCI_Send(sci_handle,"LED 3 ON ",8);
				break;
			}

		}
	}
}

void SCIcallback(void *p_args)
{
sci_cb_args_t *args;
args = (sci_cb_args_t *)p_args;
if (args->event == SCI_EVT_RX_CHAR)
{
//from RXI interrupt; character placed in queue is in args->byte
nop();
}
else if (args->event == SCI_EVT_RX_CHAR_MATCH)
{
//from RXI interrupt, received data match comparison data
//character placed in queue is in args->byte
nop();
}
#if SCI_CFG_TEI_INCLUDED
else if (args->event == SCI_EVT_TEI)
{
// from TEI interrupt; transmitter is idle
// possibly disable external transceiver here
nop();
}
#endif
else if (args->event == SCI_EVT_RXBUF_OVFL)
{
// from RXI interrupt; receive queue is full
// unsaved char is in args->byte
// will need to increase buffer size or reduce baud rate
nop();
}
else if (args->event == SCI_EVT_OVFL_ERR)
{
// from ERI/Group12 interrupt; receiver overflow error occurred
// error char is in args->byte
// error condition is cleared in ERI routine
nop();
}
else if (args->event == SCI_EVT_FRAMING_ERR)
{
// from ERI/Group12 interrupt; receiver framing error occurred
// error char is in args->byte; if = 0, received BREAK condition
// error condition is cleared in ERI routine
nop();
}
else if (args->event == SCI_EVT_PARITY_ERR)
{
// from ERI/Group12 interrupt; receiver parity error occurred
	// error char is in args->byte
	// error condition is cleared in ERI routine
	nop();
	}
	else if ( args->event == SCI_EVT_RX_DONE)
	{
	// Receive full data when SCI supported by DTC/DMAC
	nop();
	}
}

