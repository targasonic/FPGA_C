#include "xintc.h"

#ifndef AXI_UART_H_
#define AXI_UART_H_

#include "xparameters.h"
#include "xuartlite.h"

#endif /* AXI_UART_H_ */

#define TEST_MODE  1


/************************** Variable Definitions *****************************/
XGpio Gpio_gain_vis_out; 				// The Instance of the GPIO Driver
XGpio Gpio_gain_nir_out;
XGpio Gpio_gain_in;
XGpio Gpio_shutter_vis_out;
XGpio Gpio_shutter_nir_out;
XGpio Gpio_shutter_in;

XUartLite UartLite_AP;       			// The instance of the UartLite Device
XUartLite UartLite_LSU;

XGpio Gpio_wb_kr_out;
XGpio Gpio_wb_kg_out;
XGpio Gpio_wb_kb_out;

XIntc InterruptController;     /* The instance of the Interrupt Controller */

/************************** Constant Definitions *****************************/
#define GPIO_GAIN_VIS_OUT_DEVICE_ID  	XPAR_GPIO_2_DEVICE_ID
#define GPIO_GAIN_NIR_OUT_DEVICE_ID  	XPAR_GPIO_1_DEVICE_ID
#define GPIO_SHUTTER_VIS_OUT_DEVICE_ID  XPAR_GPIO_5_DEVICE_ID
#define GPIO_SHUTTER_NIR_OUT_DEVICE_ID  XPAR_GPIO_4_DEVICE_ID
#define GPIO_WB_KR_OUT_DEVICE_ID  		XPAR_GPIO_8_DEVICE_ID
#define GPIO_WB_KG_OUT_DEVICE_ID  		XPAR_GPIO_7_DEVICE_ID
#define GPIO_WB_KB_OUT_DEVICE_ID  		XPAR_GPIO_6_DEVICE_ID

#define GPIO_GAIN_IN_DEVICE_ID  		XPAR_GPIO_0_DEVICE_ID
#define GPIO_SHUTTER_IN_DEVICE_ID  		XPAR_GPIO_3_DEVICE_ID

#define UARTLITE_AP_BASEADDR	   		XPAR_AXI_UARTLITE_AP_DEVICE_ID
#define UARTLITE_LSU_BASEADDR	   		XPAR_AXI_UARTLITE_LSU_DEVICE_ID

#define INTC_DEVICE_ID         		 	XPAR_INTC_0_DEVICE_ID
#define UARTLITE_INT_IRQ_ID     		XPAR_INTC_0_UARTLITE_0_VEC_ID

#define TEST_BUFFER_SIZE        1

/************************** Function Prototypes ******************************/
void get_uart_ap_data();

void init_drivers();
void fill_input_data_struct();
void analyse_input_struct_fields();
void get_histograms();
void do_calculations();
void fill_out_struct();
void broadcast_results();

/************************** Test Table Values ********************************/

/************************** Default Values Input Struct ********************************/
#define DEFAULT_GAIN_VIS 			127
#define DEFAULT_GAIN_NIR 			127
#define DEFAULT_SHUTTER_VIS 		127
#define DEFAULT_SHUTTER_NIR 		127
#define DEFAULT_LIGHT		 		127

#define DEFAULT_MIN_GAIN_VIS 		1
#define DEFAULT_MIN_GAIN_NIR 		1
#define DEFAULT_MIN_SHUTTER_VIS 	1000
#define DEFAULT_MIN_SHUTTER_NIR 	1000

#define DEFAULT_MAX_GAIN_VIS 		255
#define DEFAULT_MAX_GAIN_NIR 		255
#define DEFAULT_MAX_SHUTTER_VIS 	65535
#define DEFAULT_MAX_SHUTTER_NIR 	65535

#define DEFAULT_MIN_LIGHT 			1
#define DEFAULT_MAX_LIGHT	 		255

#define DEFAULT_LAG_GAIN_VIS 		1
#define DEFAULT_LAG_GAIN_NIR 		1
#define DEFAULT_LAG_SHUTTER_VIS 	1
#define DEFAULT_LAG_SHUTTER_NIR 	1

#define DEFAULT_OVERLIGTHED_VIS				5
#define DEFAULT_OVERLIGTHED_NIR				5
#define DEFAULT_OVERLIGTH_MARGIN_VIS		210
#define DEFAULT_OVERLIGTH_MARGIN_NIR		210
#define	DEFAULT_MIN_AVER_BRIGHT_VIS			127
#define	DEFAULT_MIN_AVER_BRIGHT_NIR			127

#define DEFAULT_STEP_MARGINS_GAIN_VIS		25
#define DEFAULT_STEP_MARGINS_GAIN_NIR		25
#define DEFAULT_STEP_MARGINS_SHUTTER_VIS	500
#define DEFAULT_STEP_MARGINS_SHUTTER_NIR	500
#define DEFAULT_STEP_MARGINS_LIGHT_VIS		25
#define DEFAULT_STEP_MARGINS_LIGHT_NIR		25

#define DEFAULT_SNR_GAIN_VIS		230
#define DEFAULT_SNR_GAIN_NIR		230
#define DEFAULT_FL_SHUTTER_VIS		50000
#define DEFAULT_FL_SHUTTER_NIR		50000

#define DEFAUL_ENA_GAIN_VIS 		1
#define DEFAUL_ENA_GAIN_NIR 		1
#define DEFAUL_ENA_SHUTTER_VIS 		1
#define DEFAUL_ENA_SHUTTER_NIR 		1
#define DEFAUL_ENA_LIGHT 			1

#define HISTOGRAM_SIZE_UNITS		 		307200
#define HISTOGRAM_SIZE_SAMPLES		 		256
#define HISTOGRAM_OVERLIGHTED_AREA_PCT		5
#define HISTOGRAM_OVERLIGHTED_AREA_VALUE	HISTOGRAM_SIZE_UNITS * HISTOGRAM_OVERLIGHTED_AREA_PCT



typedef struct {
		u8  actualGainVis; 			// 0..255 Actual Gain Value VIS
		u8  actualGainNir;			// 0..255 Actual Gain Value NIR
		u16 actualShutterVis;		// 0..65535 Actual Shutter Value VIS
		u16 actualShutterNir;		// 0..65535 Actual Shutter Value NIR

		u8 lagGainVis;				// Specify the application delay (counting in frames) for GAIN VIS
		u8 lagGainNir;				// Specify the application delay (counting in frames) for GAIN NIR
		u8 lagShutterVis;			// Specify the application delay (counting in frames) for SHUTTER VIS
		u8 lagShutterNir;			// Specify the application delay (counting in frames) for SHUTTER NIR

		u8 rangeGainMinVis;			// Specify the range of GAIN MIN VIS
		u8 rangeGainMinNir;			// Specify the range of GAIN MIN NIR
		u8 rangeGainMaxVis;			// Specify the range of GAIN MAX VIS
		u8 rangeGainMaxNir;			// Specify the range of GAIN MAX NIR

		u8 rangeShutterMinVis;		// Specify the range of SHUTTER MIN VIS
		u8 rangeShutterMinNir;		// Specify the range of SHUTTER MIN NIR
		u8 rangeShutterMaxVis;		// Specify the range of SHUTTER MAX VIS
		u8 rangeShutterMaxNir;		// Specify the range of SHUTTER MAX NIR

		u8 rangeLightMin;			// Specify the range of LIGHT MIN
		u8 rangeLightMax;			// Specify the range of LIGHT MAX
//		u8 rangeLightMinVis;		// Specify the range of LIGHT MIN VIS
//		u8 rangeLightMinNir;		// Specify the range of LIGHT MIN NIR
//		u8 rangeLightMaxVis;		// Specify the range of LIGHT MAX VIS
//		u8 rangeLightMaxNir;		// Specify the range of LIGHT MAX NIR

		u8 sOverlightedVis;			// Allowed overlighted area (in %). VIS
		u8 sOverlightedNir;			// Allowed overlighted area (in %). VIS
		u8 overLightMarginVis;		// All samples higher than this are considered as overlighted. VIS
		u8 overLightMarginNir;		// All samples higher than this are considered as overlighted. NIR

		u8 minAverBridhtnessVis;	// The desired average brightness of image (has lower priority towards the S_Overlighted. VIS
		u8 minAverBridhtnessNir;	// The desired average brightness of image (has lower priority towards the S_Overlighted. NIR

		u8 stepMarginsGainVis;		// Number of values that limit step-up (or step-down) while incrementing or decrementing parameters. GAIN VIS
		u8 stepMarginsGainNir;		// Number of values that limit step-up (or step-down) while incrementing or decrementing parameters. GAIN NIR
		u8 stepMarginsShutterVis;	// Number of values that limit step-up (or step-down) while incrementing or decrementing parameters. SHUTTER VIS
		u8 stepMarginsShutterNir;	// Number of values that limit step-up (or step-down) while incrementing or decrementing parameters. SHUTTER NIR
		u8 stepMarginsLightVis;		// Number of values that limit step-up (or step-down) while incrementing or decrementing parameters. LIGHT VIS
		u8 stepMarginsLightNir;		// Number of values that limit step-up (or step-down) while incrementing or decrementing parameters. LIGHT NIR

		u8 snrGainVis;				// Maximum value of Gain which has acceptable Signal-to-noise ratio. GAIN VIS
		u8 snrGainNir;				// Maximum value of Gain which has acceptable Signal-to-noise ratio. GAIN NIR
		u8 flShutterVis;			// Maximum value of Shutter which has acceptable flickering. SHUTTER VIS
		u8 flShutterNir;			// Maximum value of Shutter which has acceptable flickering. SHUTTER NIR

		u8 agcEnaGainVis;			// Enable or disable automatic regulation for GAIN VIS
		u8 agcEnaGainNir;			// Enable or disable automatic regulation for GAIN NIR
		u8 agcEnaShutterVis;		// Enable or disable automatic regulation for SHUTTER VIS
		u8 agcEnaShutterNir;		// Enable or disable automatic regulation for SHUTTER NIR
		u8 alcEnaLight;				// Enable or disable automatic regulation for ALC LIGHT

//		u8 alcEnaVis;				// Enable or disable automatic regulation for ALC VIS
//		u8 alcEnaNir;				// Enable or disable automatic regulation for ALC NIR

		u8 defaultGainVis;			// The value which will be sent to the output for disabled algorithm
		u8 defaultGainNir;			// The value which will be sent to the output for disabled algorithm
		u8 defaultShutterVis;		// The value which will be sent to the output for disabled algorithm
		u8 defaultShutterNir;		// The value which will be sent to the output for disabled algorithm

		u8 defaultLight;			// The value which will be sent to the output for disabled algorithm

//		u8 defaultLightVis;			// The value which will be sent to the output for disabled algorithm
//		u8 defaultLightNir;			// The value which will be sent to the output for disabled algorithm

		u8 wbRequest;				// The signal which initiates white balance coefficients calculation procedure
		u8 lsuTimeout;

		u8 actualLight;
	} inputDataStruct;

typedef struct  {

	u8  actualGainVis;
	u8  actualGainNir;
	u16 actualShutterVis;
	u16 actualShutterNir;

	u16 wbKR;
	u16 wbKG;
	u16 wbKB;

	u8 actualLight;
} outputDataStruct;

inputDataStruct  inputData;
inputDataStruct  inputDataTest;
outputDataStruct outputData;
outputDataStruct newData;


