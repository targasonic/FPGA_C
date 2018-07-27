#include "system.h"
#include "alt_types.h"
#include "io.h"
#include "define.h"
#include "sys/alt_stdio.h"
//#include "altera_avalon_pio_regs.h"
#include <stdio.h>



// ôóíêöèÿ çàïèñè çíà÷åíèÿ â ðåãèñòðû ìàòðèöû
void reg_command_send (void)
{
	int delay_reg_com_send_flag_in_1; // çàäåðæêà äëÿ óäåðæàíèÿ âëàãà ïåðåäà÷è êîìàíäû â 1, äëÿ ðàáîòû êîíòðîëëåðà ïåðåäà÷è êîìàíäû íà ìåíüøå ÷àñòîòå
	IOWR_32DIRECT(FPA_640_512_BASE, FPA_640_512_REG_SEND_COMMAND, 1);
	delay_reg_com_send_flag_in_1 = 0;
	while (delay_reg_com_send_flag_in_1 < 5)
	{
		delay_reg_com_send_flag_in_1 ++;
	}
	IOWR_32DIRECT(FPA_640_512_BASE, FPA_640_512_REG_SEND_COMMAND, 2);
}


// ôóíêöèÿ óñòàíîâêè âðåìåíè íàêîïëåíèÿ äëÿ ìàòðèöû
void int_time_set (unsigned int int_time_ns)
{
	unsigned int int_time_ns_to_sensor;

	if 		(int_time_ns < INT_TIME_MIN_ns) int_time_ns_to_sensor = INT_TIME_MIN_ns;
	else if (int_time_ns > INT_TIME_MAX_ns) int_time_ns_to_sensor = INT_TIME_MAX_ns;
	else 									int_time_ns_to_sensor = int_time_ns;

	IOWR_32DIRECT(FPA_640_512_BASE, FPA_640_512_INT_SET_COMMAND, int_time_ns_to_sensor);
}

// ôóíêöèÿ óñòàíîâêè çàäåðæêè çàõâàòà ïèêñåëåé äëÿ ÀÖÏ
void adc_delay_set (unsigned int adc_delay)
{
	unsigned int adc_delay_to_sensor;

	if (adc_delay > 31) adc_delay_to_sensor = 31;
	else 				adc_delay_to_sensor = adc_delay;

	IOWR_32DIRECT(FPA_640_512_BASE, FPA_640_512_ADC_DELAY_COMMAND, adc_delay_to_sensor);
}

// ôóíêöèÿ âêëþ÷åíèÿ ðåæèìà áîëüøîé åìêîñòè 1 - ON, 0 - OFF
void sensor_control_command (unsigned int sensor_control_word)
{
	IOWR_32DIRECT(FPA_640_512_BASE, FPA_640_512_CONTROL_REG_COMMAND, sensor_control_word);
}


// ôóíêöèÿ âêëþ÷åíèÿ îáðàáîòêè. ñìåùåíèå íà ïîñòîÿííûé óðîâåíü 1 - ON, 2 - OFF
void offset_video_ON_OFF (unsigned int on_off_flag)
{
	IOWR_32DIRECT(VIDEO_PROCESS_OFFSET_0_BASE, VIDEO_OFFSET_PROCESS_ON_COMMAND, on_off_flag);
}

// ôóíêöèÿ âêëþ÷åíèÿ ÊÎËÈÁÐÎÂÊÈ. 1 - ON, 2 - OFF
void offset_black_colibrate_start_ON_OFF (unsigned int on_off_black_colibrate)
{
	IOWR_32DIRECT(VIDEO_PROCESS_OFFSET_0_BASE, VIDEO_OFFSET_PROCESS_BLACK_COLIBRATE_COMMAND, on_off_black_colibrate);
}
// ôóíêöèÿ âêëþ÷åíèÿ îáðàáîòêè. zoom äàííûõ ñðàçó ïîñëå àöï 1 - ON, 2 - OFF
void zoom_adc_video_ON_OFF (unsigned int zoom_on_off_flag)
{
	IOWR_32DIRECT(VIDEO_PROCESS_OFFSET_0_BASE, VIDEO_OFFSET_PROCESS_ZOOM_ADC_ON_COMMAND, zoom_on_off_flag);
}

// óðîâåíü ñìåùåíèÿ
void offset_video_value_set (unsigned int ofs_vid_val)
{
	IOWR_32DIRECT(VIDEO_PROCESS_OFFSET_0_BASE, VIDEO_OFFSET_PROCESS_OFFSET_VALUE_SET_COMMAND, ofs_vid_val);
}

// ôóíêöèÿ óñòàíîâêè çíàêà. ñìåùåíèå íà ïîñòîÿííûé óðîâåíü
void offset_video_minus (void)
{
	IOWR_32DIRECT(VIDEO_PROCESS_OFFSET_0_BASE, VIDEO_OFFSET_PROCESS_SIGNED_COMMAND, 1);
}

// ôóíêöèÿ óñòàíîâêè çíàêà. ñìåùåíèå íà ïîñòîÿííûé óðîâåíü
void offset_video_plus (void)
{
	IOWR_32DIRECT(VIDEO_PROCESS_OFFSET_0_BASE, VIDEO_OFFSET_PROCESS_SIGNED_COMMAND, 2);
}

// ÑÁÐÎÑ DDR ïàìÿòè
void ddr_n_reset (unsigned char ddr_nreset)
{
	IOWR_32DIRECT(VIDEO_PROCESS_OFFSET_0_BASE, DDR_N_RESET_COMMAND, ddr_nreset);
}

// ôóíêöèÿ óñòàíîâêè òåñòîâîé êàðòèíêè
void test_frame_mode_set (unsigned int test_frame_mode)
{
	IOWR_32DIRECT(TEST_FRAME_GENERATE_BASE, TEST_FRAME_GEN_MODE_COMMAND, test_frame_mode);
}


/*------------------------------------------------------------------------------------------------------------------------------------------
---------------------- ÃÐÀÔÈ×ÅÑÊÀß ÈÍÄÈÊÀÖÈß: ----------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------

|00|01|02|03|04|05|06|07|08|09|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|32|33|34|35|36|37|38|39|40| Line 1
|00|01|02|03|04|05|06|07|08|09|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|32|33|34|35|36|37|38|39|40| Line 2
|00|01|02|03|04|05|06|07|08|09|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|32|33|34|35|36|37|38|39|40| Line 3

Ïðèìåð:

|_|Ò|Å|Ì|Ï|Å|Ð|À|Ò|Ó|Ð|À|:|_|_|_|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|32|33|34|35|36|37|38|39|40| Line 1 [000:079]
|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|32|33|34|35|36|37|38|39|40| Line 2 [080:159]
|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|32|33|34|35|36|37|38|39|40| Line 3 [160:239]

Ñèìâîë ïåðåäå¸òñÿ ïî avalon øèíå. Ðàçìåð ñèìâîëà 8x16. Ïåðåäå¸òñÿ 16 ðàç ïî 1 áàéòó + àäðåñ õðàíåíèÿ ñèìâîëà â RAM ïàìÿòè:
Ñëîâî avalon = 32 áèòà:
  áèòû: |0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|
ñîñòàâ: | çíà÷åíèå px   |  àäðåñ â ïàìÿòè RAM (11 áèò) |	         ðåçåðâ					  |

Ôîðìèðîâàíèå àäðåñà: (íîìåð ñòðîêè - 1) * 16 + íîìå.........

Àäðåñà â ïàìÿòè RAM:

|000|001|002|003|004|005| Line 1
|080|081|082|083|084|085| Line 1
|160|161|162|163|164|165| Line 1

*/

// ÃÐÀÔÈ×ÅÑÊÀß ÈÍÄÈÊÀÖÈß. Âêëþ÷åíèå  = 1, Âûêëþ÷åíèå = 0.
void graph_id_ON_OFF (unsigned char on_off_flag)
{
	IOWR_32DIRECT(GRAPH_ID_GENERATE_BASE, GRAPH_ID_ON_OFF_COMMAND, on_off_flag);
}

// ÃÐÀÔÈ×ÅÑÊÀß ÈÍÄÈÊÀÖÈß. Óñòàíîâêà ôîíà è öâåò òåêñòà
void graph_id_colors (unsigned int text_color, unsigned int fone_color)
{
	unsigned int  	generated_word;
	generated_word	= text_color * 65536;
	generated_word	= generated_word + fone_color;
	IOWR_32DIRECT(GRAPH_ID_GENERATE_BASE, GRAPH_ID_COLORS_COMMAND, generated_word);
}

// ÃÐÀÔÈ×ÅÑÊÀß ÈÍÄÈÊÀÖÈß. Óñòàíîâêà ðàçìåðà è ïîëîæåíèÿ èíäèêàöèîííîãî îêíà
void graph_id_size (unsigned int graph_size, unsigned int graph_shift)
{
	unsigned int  	generated_word;
	generated_word	= graph_size * 65536;
	generated_word	= generated_word + graph_shift;
	IOWR_32DIRECT(GRAPH_ID_GENERATE_BASE, GRAPH_ID_SIZE_COMMAND, generated_word);
}

// ÇÀÏÈÑÜ ÑÈÌÂÎËÀ Â ÏÀÌßÒÜ RAM
// Ñèìâîë õðàíèòñÿ â ïàìÿòè RAM
void write_char_to_ram (unsigned char char_position, unsigned char char_line, unsigned char font_char[])
{
	short 			x_size_ram 		= 80;
	unsigned int   	y_size_char		= 1280;
	unsigned char 	i 				= 0;
	short			wr_char_delay 	= 0;
	unsigned int  	address_to_ram;
	unsigned int  	generated_word_to_hdl_module;


	for (i = 0; i < 17; i++)
	{
		address_to_ram = (char_line - 1)*y_size_char + i*x_size_ram + char_position; 						// ñ÷èòàåì àäðåñ â RAM ïàìÿòè äëÿ çàïèñü áàéòà ñèìâîëà
		generated_word_to_hdl_module = address_to_ram * 256;												// Ôîðìèðîâàíèå ñëîâà äëÿ ïåðåäà÷è hdl ìîäóëþ ïî avalon. Ñäâèã àäðåñà íà íåîáõîäèìóþ áèòîâóþ ïîçèöèþ â ñëîâå. Íà 8 áèò.
		generated_word_to_hdl_module = generated_word_to_hdl_module + font_char[i];							// Ôîðìèðîâàíèå ñëîâà äëÿ ïåðåäà÷è hdl ìîäóëþ ïî avalon. Äîáàâëÿåì çíà÷åíèå çàêðàøåíûõ ïèêñåëå â ñèìâîëå.

		IOWR_32DIRECT(GRAPH_ID_GENERATE_BASE, GRAPH_ID_WR_CHAR_COMMAND, generated_word_to_hdl_module);		// Ïåðåäà÷à ñëîâà hdl ìîäóëþ ïî avalon.
		wr_char_delay = 0;
		while (wr_char_delay < 5)
		{
			wr_char_delay ++;
		}

	}

}

/*------------------------------------------------------------------------------------------------------------------------------------------
---------------------- ÊÎÍÒÐÀÑÒÈÐÎÂÀÍÈÅ: ----------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------*/
// ÊÎÍÒÐÀÑÒÈÐÎÂÀÍÈÅ. Âêëþ÷åíèå  = 1, Âûêëþ÷åíèå = 0.
void contrast_video_ON_OFF (unsigned char on_off_flag)
{
	IOWR_32DIRECT(0x80, VIDEO_CONTRAST_ON_OFF_COMMAND, on_off_flag);
}
// Ïðåäåëû êîíòðàñòèðîâàíèÿ ñûðîãî ïîòîêà 16 áèò
void contrast_zoom_value_set (unsigned int zoom_adc_vid_val)
{
	IOWR_32DIRECT(0x80, VIDEO_CONTRAST_ZOOM_VALUE_COMMAND, zoom_adc_vid_val);
}

// ÊÎÍÒÐÀÑÒÈÐÎÂÀÍÈÅ.
//void contrast_level_min (unsigned int level_min)
//{
//	IOWR_32DIRECT(0x80, VIDEO_CONTRAST_LEVEL_MIN_COMMAND, level_min);
//}
// ÊÎÍÒÐÀÑÒÈÐÎÂÀÍÈÅ.
//void contrast_level_max (unsigned int level_max)
//{
//	IOWR_32DIRECT(0x80, VIDEO_CONTRAST_LEVEL_MIN_COMMAND, level_max);
//}
// ÊÎÍÒÐÀÑÒÈÐÎÂÀÍÈÅ.
//void contrast_corr_coef (unsigned int corr_coef)
//{
//	IOWR_32DIRECT(0x80, VIDEO_CONTRAST_CORR_COEF_COMMAND, corr_coef);
//}


/*------------------------------------------------------------------------------------------------------------------------------------------
---------------------- ÃÈÑÒÎÃÐÀÌÌÀ: ----------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------*/
// ÃÈÑÒÎÃÐÀÌÌÀ. Óðîâåíü ãèñòîãðàììû äëÿ êîíòðàñòèðîâàíèÿ
void hist_level_for_contrast (unsigned int hist_lvl_contrast)
{
	IOWR_32DIRECT(0xa0, VIDEO_CONTRAST_LEVEL_FOR_CONTRAST_COMMAND, hist_lvl_contrast);
}
// ÃÈÑÒÎÃÐÀÌÌÀ. Óðîâåíü ãèñòîãðàììû äëÿ àðó (5 ïðîöåíòîâ)
void hist_level_for_aru (unsigned int hist_lvl_aru)
{
	IOWR_32DIRECT(0xa0, VIDEO_CONTRAST_LEVEL_FOR_ARU_COMMAND, hist_lvl_aru);
}
// ÃÈÑÒÎÃÐÀÌÌÀ. Íèæíèé óðîâåíü äëÿ êîíòðàñòèðîâàíèÿ
void hist_level_low_for_contrast (unsigned int hist_lvl_aru)
{
	IOWR_32DIRECT(0xa0, 8, hist_lvl_aru);
}

/*------------------------------------------------------------------------------------------------------------------------------------------
---------------------- ÒÅÌÏÅÐÀÒÓÐÀ: ----------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------*/
// ÒÅÌÏÅÐÀÒÓÐÀ. Âêëþ÷èòü èñòî÷íèê ïèòàíèÿ äëÿ Ïåëåòüå
void temp_run_on (unsigned char temp_run)
{
	IOWR_32DIRECT(0xe0, 0, temp_run);
}
void temp_value_to_R (unsigned int temp_to_R)
{
	IOWR_32DIRECT(0xe0, 4, temp_to_R);
}


/*------------------------------------------------------------------------------------------------------------------------------------------
---------------------- ETHERNET: ----------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------*/
// Ôóíêöèÿ çàïèñè çíà÷åíèÿ âðåìåíè íàêîëåíèÿ â ïåðåäàò÷èê RTP

void ethernet_int_time_to_rtp (unsigned int int_time)
{
	IOWR_32DIRECT(RTP_PROTOCOL_BASE, 0, int_time);
}
