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

//define
#define maxlencommandline 30
#define backspace '\b'
#define space ' '
//handle
static sci_hdl_t sci_handle;

//private, global variables and functions
//variables


//functions
void SCIcallback(void *p_args);
uint8_t* cut_array(uint8_t* original_array, size_t start_index, size_t length);

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
	uint8_t commandline[maxlencommandline];
	uint8_t i=0;
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
	if (SCI_SUCCESS!=sci_err){
		nop();
	}
	//print to console if successfully initialize
	printf("SCI initialize successfully\n");

	R_SCI_Send(sci_handle,"Press any character to continue\n",32);

	//check the first character after initialize
	do{
		sci_err = R_SCI_Receive(sci_handle,&received_char,1);
	}while (SCI_ERR_INSUFFICIENT_DATA==sci_err);


	R_SCI_Send(sci_handle,"Waiting for commandlines\b\b\n",23);
	//loop
	while (1){
		//wait for character from the terminal
		while(maxlencommandline>i||'\n'!=commandline[i]){//while commandline still in length
			//received character
			do{
					sci_err = R_SCI_Receive(sci_handle,&received_char,1);
			}while (SCI_ERR_INSUFFICIENT_DATA==sci_err);

			if('\n' != received_char){//character is not enter ('\n')

				if(space!= received_char && backspace!=received_char){//character is not space/blank (' ')
					printf("%d\n",received_char,i);
					commandline[i]=received_char;
					i++;
					R_SCI_Send(sci_handle,&received_char,1);
				}
				else if(0!=i){//if i is not at 0, backspace and space is valid

					if(space==received_char){//if it is space
						if(commandline[i-1]!=space){//multiple space handle
							//receive space if no space before
							R_SCI_Send(sci_handle,&received_char,1);
							commandline[i]=space;
							i++;
							R_SCI_Send(sci_handle,"space",5);
						}
					}

					if(backspace==received_char){//if it is backspace

						R_SCI_Send(sci_handle,"\b \b",3);


							commandline[--i]='0';

					}
				}
			}
			else{//character is enter ('\n')

				commandline[i]='\0';
				char cmd[80];
				uint8_t *command;
				command=cut_array(&commandline,0,maxlencommandline);
				printf("%d\n",command);
				printf("%d\n",atoi("led"));

				if(strncmp("led on",command,6)==0){
					LED0_PDR=LED_ON;
					LED1_PDR=LED_ON;
					LED2_PDR=LED_ON;
					LED3_PDR=LED_ON;
				}
				if(strncmp("led off",command,7)==0){
					LED0_PDR=LED_OFF;
					LED1_PDR=LED_OFF;
					LED2_PDR=LED_OFF;
					LED3_PDR=LED_OFF;
				}
				if(strncmp("led0 off",command,8)==0){
									LED0_PDR=LED_OFF;

								}
				if(strncmp("led2 off",command,8)==0){

									LED2_PDR=LED_OFF;

								}
				if(strncmp("led3 off",command,8)==0){

									LED3_PDR=LED_OFF;
								}
				if(strncmp("led1 off",command,8)==0){

									LED1_PDR=LED_OFF;

								}
				if(strncmp("led0 on",command,7)==0){
													LED0_PDR=LED_ON;

												}
								if(strncmp("led2 on",command,7)==0){

													LED2_PDR=LED_ON;

												}
								if(strncmp("led3 on",command,7)==0){

													LED3_PDR=LED_ON;
												}
								if(strncmp("led1 on",command,7)==0){

													LED1_PDR=LED_ON;

												}





				for (uint8_t j=0;j<maxlencommandline;j++){
					commandline[j]='\0';

				}
				i=0;
			}
		}

	}
}

uint8_t* cut_array(uint8_t* original_array, size_t start_index, size_t length) {
    // Allocate memory for the new array
    uint8_t* cut = (uint8_t*)malloc(length * sizeof(uint8_t));

    // Copy the desired portion of the original array to the new array
    for (size_t i = 0; i < length; i++) {
        cut[i] = original_array[start_index + i];
        if(original_array[start_index + i]=='\0'){
            		return cut;
        }
    }

    return cut;
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

