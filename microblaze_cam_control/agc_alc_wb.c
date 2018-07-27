/*
 * agc_alc_wb.c
 *
 *  Created on: 2017. 10. 17.
 *      Author: Ruslan Kozlov
 */
#include "xparameters.h"
#include "xgpio.h"
//#include "xuartlite.h"
#include "xuartlite_l.h"
#include "xil_exception.h"
#include "agc_alc_wb.h"
#include "xintc_l.h"




void init_drivers (void) {
	/* Initialize the GPIO drivers */
	XGpio_Initialize(&Gpio_gain_vis_out, GPIO_GAIN_VIS_OUT_DEVICE_ID);
	XGpio_Initialize(&Gpio_gain_nir_out, GPIO_GAIN_NIR_OUT_DEVICE_ID);
	XGpio_Initialize(&Gpio_shutter_vis_out, GPIO_SHUTTER_VIS_OUT_DEVICE_ID);
	XGpio_Initialize(&Gpio_shutter_nir_out, GPIO_SHUTTER_NIR_OUT_DEVICE_ID);

	XGpio_Initialize(&Gpio_wb_kr_out, GPIO_WB_KR_OUT_DEVICE_ID);
	XGpio_Initialize(&Gpio_wb_kg_out, GPIO_WB_KG_OUT_DEVICE_ID);
	XGpio_Initialize(&Gpio_wb_kb_out, GPIO_WB_KB_OUT_DEVICE_ID);

	XGpio_Initialize(&Gpio_gain_in, 	GPIO_GAIN_IN_DEVICE_ID);
	XGpio_Initialize(&Gpio_shutter_in, GPIO_SHUTTER_IN_DEVICE_ID);

	/* Initialize the UART drivers */
	XUartLite_Initialize(&UartLite_AP, UARTLITE_AP_BASEADDR);
	XUartLite_SelfTest(&UartLite_AP);
	XUartLite_Initialize(&UartLite_LSU, UARTLITE_LSU_BASEADDR);
	XUartLite_SelfTest(&UartLite_LSU);

	/* Enable MicroBlaze exception */
	microblaze_enable_interrupts();
	XIntc_Initialize(&InterruptController, INTC_DEVICE_ID);
	XIntc_Connect(&InterruptController, UARTLITE_INT_IRQ_ID, (XInterruptHandler)get_uart_ap_data,	&UartLite_AP);
	/* Start the interrupt controller */
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	/* Enable uart interrupt in the interrupt controller */
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR, XPAR_AXI_UARTLITE_AP_INTERRUPT_MASK);
	/* Enable Uartlite interrupt */
	XUartLite_EnableIntr(XPAR_AXI_UARTLITE_AP_BASEADDR);

	init_default_input_data(&inputData);
}

void get_uart_ap_data () {
	/* till uart FIFOs are empty */
	while (!XUartLite_IsReceiveEmpty(XPAR_AXI_UARTLITE_AP_BASEADDR)) {
		inputData.actualLight = XUartLite_RecvByte(XPAR_AXI_UARTLITE_AP_BASEADDR);
	}
}

void fill_input_data_struct () {

	if (TEST_MODE) {
		inputData = inputDataTest;
	}
	else {
		inputData.actualGainVis 		= XGpio_DiscreteRead(&Gpio_gain_in, 1);
		inputData.actualGainNir 		= XGpio_DiscreteRead(&Gpio_gain_in, 2);
		inputData.actualShutterVis  	= XGpio_DiscreteRead(&Gpio_shutter_in, 1);
		inputData.actualShutterNir  	= XGpio_DiscreteRead(&Gpio_shutter_in, 2);
	}
}

void analyse_input_struct_fields (){
	// define behavior
}

void get_histograms (){
	// readout AXI registers from FPGA fabric
}
void do_calculations (){
	// execute algorithms routine
	newData.actualGainVis		= inputData.actualGainVis;
	newData.actualGainNir		= inputData.actualGainNir;
	newData.actualShutterVis	= inputData.actualShutterVis;
	newData.actualShutterNir	= inputData.actualShutterNir;

	newData.wbKR	= newData.wbKR + 1;
	newData.wbKG	= inputData.actualLight;
	newData.wbKB	= newData.wbKB + 5;

	newData.actualLight = inputData.actualLight;
}

void fill_out_struct (){
	// put calculated values into the output structure
	outputData.actualGainVis	= newData.actualGainVis;
	outputData.actualGainNir	= newData.actualGainNir;
	outputData.actualShutterVis	= newData.actualShutterVis;
	outputData.actualShutterNir	= newData.actualShutterNir;

	outputData.wbKR	= newData.wbKR;
	outputData.wbKG	= newData.wbKG;
	outputData.wbKB	= newData.wbKB;

	outputData.actualLight	= newData.actualLight;
}

void broadcast_results (){
	// send output structure fields to UART or GPIO
	XGpio_DiscreteWrite(&Gpio_gain_vis_out, 1, outputData.actualGainVis);
	XGpio_DiscreteWrite(&Gpio_gain_nir_out, 1, outputData.actualGainNir);

	XGpio_DiscreteWrite(&Gpio_shutter_vis_out, 1, outputData.actualShutterVis);
	XGpio_DiscreteWrite(&Gpio_shutter_nir_out, 1, outputData.actualShutterNir);

	XGpio_DiscreteWrite(&Gpio_wb_kr_out, 1, outputData.wbKR);
	XGpio_DiscreteWrite(&Gpio_wb_kg_out, 1, outputData.wbKG);
	XGpio_DiscreteWrite(&Gpio_wb_kb_out, 1, outputData.wbKB);

//	XUartLite_Send(&UartLite_LSU, outputData.actualLight, 1);
//	XUartLite_Send(&UartLite_AP, newData.wbKR, 1);
//	uart_send(uartBuffer, uartBufferSize);
//	XUartLite_Recv(&UartLite_AP, newData.wbKB, 1);
//	XUartLite_Recv(&UartLite_AP, newData.wbKB, 1);
}

/************ Initialization default values input Data */
void init_default_input_data (inputDataStruct *inputDataInit){

	inputDataInit->actualGainVis 	= DEFAULT_GAIN_VIS;
	inputDataInit->actualGainNir 	= DEFAULT_GAIN_NIR;
	inputDataInit->actualShutterVis 	= DEFAULT_SHUTTER_VIS;
	inputDataInit->actualShutterNir	= DEFAULT_SHUTTER_NIR;

	inputDataInit->lagGainVis		= DEFAULT_LAG_GAIN_VIS;
	inputDataInit->lagGainNir		= DEFAULT_LAG_GAIN_NIR;
	inputDataInit->lagShutterVis		= DEFAULT_LAG_SHUTTER_VIS;
	inputDataInit->lagShutterNir		= DEFAULT_LAG_SHUTTER_NIR;

	inputDataInit->rangeGainMinVis	= DEFAULT_MIN_GAIN_VIS;
	inputDataInit->rangeGainMinNir	= DEFAULT_MIN_GAIN_NIR;
	inputDataInit->rangeGainMaxVis	= DEFAULT_MAX_GAIN_VIS;
	inputDataInit->rangeGainMaxNir	= DEFAULT_MAX_GAIN_NIR;

	inputDataInit->rangeShutterMinVis = DEFAULT_MIN_SHUTTER_VIS;
	inputDataInit->rangeShutterMinNir = DEFAULT_MIN_SHUTTER_NIR;
	inputDataInit->rangeShutterMaxVis = DEFAULT_MAX_SHUTTER_VIS;
	inputDataInit->rangeShutterMaxNir = DEFAULT_MAX_SHUTTER_NIR;

	inputDataInit->rangeLightMin 	 = DEFAULT_MIN_LIGHT;
	inputDataInit->rangeLightMax 	 = DEFAULT_MAX_LIGHT;

	inputDataInit->sOverlightedVis 	 = DEFAULT_OVERLIGTHED_VIS;
	inputDataInit->sOverlightedNir 	 = DEFAULT_OVERLIGTHED_NIR;
	inputDataInit->overLightMarginVis = DEFAULT_OVERLIGTH_MARGIN_VIS;
	inputDataInit->overLightMarginNir = DEFAULT_OVERLIGTH_MARGIN_NIR;

	inputDataInit->minAverBridhtnessVis = DEFAULT_MIN_AVER_BRIGHT_VIS;
	inputDataInit->minAverBridhtnessNir = DEFAULT_MIN_AVER_BRIGHT_NIR;

	inputDataInit->stepMarginsGainVis    = DEFAULT_STEP_MARGINS_GAIN_VIS;
	inputDataInit->stepMarginsGainNir    = DEFAULT_STEP_MARGINS_GAIN_NIR;
	inputDataInit->stepMarginsShutterVis = DEFAULT_STEP_MARGINS_SHUTTER_VIS;
	inputDataInit->stepMarginsShutterNir = DEFAULT_STEP_MARGINS_SHUTTER_NIR;
	inputDataInit->stepMarginsLightVis   = DEFAULT_STEP_MARGINS_LIGHT_VIS;
	inputDataInit->stepMarginsLightNir   = DEFAULT_STEP_MARGINS_LIGHT_NIR;

	inputDataInit->snrGainVis	 		= DEFAULT_SNR_GAIN_VIS;
	inputDataInit->snrGainNir	 		= DEFAULT_SNR_GAIN_NIR;
	inputDataInit->flShutterVis	 	= DEFAULT_FL_SHUTTER_VIS;
	inputDataInit->flShutterNir	 	= DEFAULT_FL_SHUTTER_NIR;

	inputDataInit->agcEnaGainVis		= DEFAUL_ENA_GAIN_VIS;
	inputDataInit->agcEnaGainNir		= DEFAUL_ENA_GAIN_NIR;
	inputDataInit->agcEnaShutterVis	= DEFAUL_ENA_SHUTTER_VIS;
	inputDataInit->agcEnaShutterNir	= DEFAUL_ENA_SHUTTER_NIR;
	inputDataInit->alcEnaLight			= DEFAUL_ENA_LIGHT;

	inputDataInit->defaultGainVis		= DEFAULT_GAIN_VIS;
	inputDataInit->defaultGainNir		= DEFAULT_GAIN_NIR;
	inputDataInit->defaultShutterVis	= DEFAULT_SHUTTER_VIS;
	inputDataInit->defaultShutterNir	= DEFAULT_SHUTTER_NIR;
	inputDataInit->defaultLight		= DEFAULT_LIGHT;

	inputDataInit->wbRequest		= 0;
	inputDataInit->lsuTimeout	= 0;

	inputDataInit->actualLight	= DEFAULT_LIGHT;
}
