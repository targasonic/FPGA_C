#include "xparameters.h"
#include "xgpio.h"
#include "xuartlite_l.h"
#include "xil_exception.h"
#include "agc_alc_wb.h"

u32 HR[256], HG[256], HB[256], HY[256];

int main(void)
{
init_drivers();

	while (1) {

		fill_input_data_struct();

		analyse_input_struct_fields();

		get_histograms();

		do_calculations();

		fill_out_struct();

		broadcast_results();

	}

return XST_SUCCESS;
}
