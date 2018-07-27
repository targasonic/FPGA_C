#include "system.h"
#include "alt_types.h"
#include "io.h"
#include "define.h"
#include "sys/alt_stdio.h"
#include <stdio.h>
#include "font_table.h"

unsigned int adc_delay;
unsigned int work_delay = 0;

char aru_on		 		= 0;
float k_aru;
char bad_px_overwrite	= 0;

// ���������� ��� ������ � ����������� �� ETHERNET
unsigned char byte_from_udp_2;
unsigned char byte_from_udp_3;
unsigned char byte_from_udp_8;
unsigned char byte_from_udp_9;
unsigned char byte_from_udp_10;

unsigned int  		int_time_from_udp 			= 10000;
unsigned char 		aru_lock_from_udp			  = 0;
unsigned char 		aru_on_from_udp				   = 1;
unsigned char 		sensor_control_word  		= 0;
unsigned char 		video_pause_from_udp		= 0;
unsigned char 		stereo_mod_on_from_udp	= 0;
unsigned char 		test_self_from_udp			= 0;
unsigned int 		  gain_from_udp				    = 0;


void int_time_set					  (unsigned int);
void adc_delay_set 					    (unsigned int);
void reg_command_send				      (void);
void sensor_mode_C_out_large_ON_OFF (unsigned char);
void offset_video_value_set			  (unsigned int);
void offset_video_ON_OFF			    (unsigned char);
void offset_black_colibrate_start_ON_OFF(unsigned char);
void ddr_n_reset 					        (unsigned char);
void offset_video_minus				    (void);
void offset_video_plus				    (void);
void contrast_zoom_value_set 		  (unsigned int);
void contrast_video_ON_OFF			  (unsigned char);
void test_frame_mode_set			    (unsigned int);
void graph_id_ON_OFF 				      (unsigned char);
void write_char_to_ram 				    (unsigned char, unsigned char, unsigned char []);
void graph_id_size 					      (unsigned int, unsigned int);
void graph_id_colors				      (unsigned int, unsigned int);
void hist_level_for_contrast		  (unsigned int);
void hist_level_low_for_contrast	(unsigned int);
void hist_level_for_aru				    (unsigned int);
void temp_run_on 					        (unsigned char);
void temp_value_to_R 				      (unsigned int);
void ethernet_int_time_to_rtp		  (unsigned int);

void select_numeric (int temple_char, char temple_pos, char temple_line) {
	switch	(temple_char) {
	case 0 :  write_char_to_ram (temple_pos, temple_line, Data_0); break;
	case 1 :  write_char_to_ram (temple_pos, temple_line, Data_1); break;
	case 2 :  write_char_to_ram (temple_pos, temple_line, Data_2); break;
	case 3 :  write_char_to_ram (temple_pos, temple_line, Data_3); break;
	case 4 :  write_char_to_ram (temple_pos, temple_line, Data_4); break;
	case 5 :  write_char_to_ram (temple_pos, temple_line, Data_5); break;
	case 6 :  write_char_to_ram (temple_pos, temple_line, Data_6); break;
	case 7 :  write_char_to_ram (temple_pos, temple_line, Data_7); break;
	case 8 :  write_char_to_ram (temple_pos, temple_line, Data_8); break;
	case 9 :  write_char_to_ram (temple_pos, temple_line, Data_9); break;
	default:  write_char_to_ram (temple_pos, temple_line, Data_0); break;
	}
}

int main()
{
int delay;
int i = 0;
int j = 0;

/*------------------------------------------------------------------------------------------------------------------------------------------
---------------------- ������������� ������: -----------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------*/
IOWR_32DIRECT(VIDEO_PROCESS_OFFSET_0_BASE, 28, 5118);						// ��������� ������� ����� � DDR3 (1 ������ = 5 �������, 512 ����� * 5 �������)
IOWR_32DIRECT(VIDEO_PROCESS_OFFSET_0_BASE, 0, 53000);						// ��������� ������� ����� � DDR3 (1 ������ = 5 �������, 512 ����� * 5 �������)

//------------------------------------------------------------------------------------------------
// ���������� ��������
//------------------------------------------------------------------------------------------------
unsigned int int_time_ns 		= 10000;	// (INT_TIME_MAX_ns - INT_TIME_MIN_ns)/2;   ����� ����������
unsigned int_time_mks;
unsigned int frame_counter;
unsigned int sensor_reg_up;
unsigned int sensor_reg_im;
unsigned int sensor_reg_bw;
unsigned int sensor_reg_lp;
unsigned int sensor_reg_dp;
unsigned int sensor_reg_mp;
unsigned int sensor_reg_ap;
unsigned int sensor_mode_from_udp;

//------------------------------------------------------------------------------------------------
// �����������
//------------------------------------------------------------------------------------------------
int temperature   		 = -366;
int temp_mU   	  		 = 740;  	// �  ���� �������
int temp_adc_vref 		 = 1250; 	// �  ���� �������
int temp_adc_data 		 = 500;
int temp_current;
int	temp_values[5] 		 = {20,20,20,20,20};
int init_digital_R_count = 0;

int value_to_R;					
int power_to_peletie 	 = 10;		// �������� ���������� ��������� �������. �� 0 ��  100

temp_run_on (0);					// ��������� ��������� ������� ��� ������


//------------------------------------------------------------------------------------------------
// ������� ��������
//------------------------------------------------------------------------------------------------
unsigned int offset_video_value;
//offset_video_value = 14000;
// offset_video_value_set(offset_video_value);
//offset_video_minus();
offset_video_ON_OFF(1); 								// 1 - ON , 2 - OFF
offset_black_colibrate_start_ON_OFF (0);


//------------------------------------------------------------------------------------------------
// ����������������:
//------------------------------------------------------------------------------------------------
char contrast_on 					= 1;
unsigned int zoom					= 1;
unsigned int zoom_low_level			= 1;
unsigned int gain 					= 10; 									// �������� �������� ������� (� ������� ���� ����� ��������)
unsigned int zoom_numerator 		= 255;									// ������������ �������� ��������� ������� (8 ���)
unsigned int zoom_denominator 		= 65535;								// ������� �������� �������, ������� ����� �������� � ������������� ������� ���������
unsigned int zoom_signal_max_level 	= 65535;								// ������������ ������� �������� ������� ����� ������ ����

zoom = 65536*zoom_numerator + zoom_denominator;								// ������������ �������� ��� �������� �� �������
contrast_zoom_value_set	(zoom);												// �������� �� ������� �������� ���� (����������������)
contrast_video_ON_OFF 	(contrast_on);				
//contrast_level_min (1);
//contrast_level_max (250);


//------------------------------------------------------------------------------------------------
// ������� �����������:
//------------------------------------------------------------------------------------------------
int 			histogram_calc_done;
int 			hist_calc_going;
int 			hist_calc_going_pre;
int 			histogram_level_low;
int 			histogram_level_high;
int 			histogram_level_for_aru;
int 			histogram_average_from_frame;
unsigned int 	histogram_level_for_contrast    	= (X_SIZE*Y_SIZE/100)*1.0; 		// ������� ������� ����������� ��� ���������������� (��������)
unsigned int 	histogram_level_low_for_contrast   	= (X_SIZE*Y_SIZE/100)*1.5; 		// ������  ������� ����������� ��� ���������������� (��������)
unsigned int 	histogram_level_for_aru_process 	= (X_SIZE*Y_SIZE/100)*5;		// ������� ����������� ��� ��� (��������)
hist_level_for_contrast 	(histogram_level_for_contrast);							// ��������� ������� ��� ���������������� � ������ ������� �����������
hist_level_for_aru 			(histogram_level_for_aru_process);						// ��������� ������� ��� ��� � ������ ������� �����������
hist_level_low_for_contrast	(histogram_level_low_for_contrast);


//------------------------------------------------------------------------------------------------
// �������� ��������:
//------------------------------------------------------------------------------------------------
unsigned int test_frame_mode = 4;											// �������� ������: 0 - ������������ ����� ����; 1 - �������������� ����� ����; 2 - �������� �� ������; 3 - ����� ��� � ���������� �������
IOWR_32DIRECT(TEST_FRAME_GENERATE_BASE, 4, 0);								// ��������� ������ ������� ��� ��������������� �����
IOWR_32DIRECT(TEST_FRAME_GENERATE_BASE, 8, 255);							// ��������� ������ ������ ��� ��������������� �����
IOWR_32DIRECT(TEST_FRAME_GENERATE_BASE, 12, 255);							// ��������� ����� �������� ��� ������������ �������� ��������
IOWR_32DIRECT(TEST_FRAME_GENERATE_BASE, 16, 50);							// ��������� ������� �������� ��� ������������ �������� ��������
test_frame_mode_set(test_frame_mode);

//------------------------------------------------------------------------------------------------
// ����������� ��������:
//------------------------------------------------------------------------------------------------
unsigned int 	graph_size			= 48;									      // ������������ ������ ���� ��������� (�����)
unsigned int 	graph_shift			= Y_SIZE - graph_size; 					// ������������ ��������� ���� �������� (�����)
unsigned int 	graph_text_color	=  MAX_VIDEO_VALUE;						// ���� ������
unsigned int 	graph_fone_color	= 255;									    // ���� ���� = ������� ������ - ���������� graph_fone_color
unsigned char 	graph_id_on			= 1;									    // ����������� ���������. ���������  = 1, ���������� = 0.
unsigned char 	graph_id_aim_on		= 0;									  // �����������. ��������� = 1, ���������� = 0.
unsigned char 	graph_id_aim_color	= 0;									// �����������. ������ = 0, ����� = 1.

graph_id_colors (graph_text_color, graph_fone_color);				// ��������� ����� ���� � ������
graph_id_size 	(graph_size, graph_shift);									// ��������� �������� � ��������� ���� ���������
graph_id_ON_OFF (graph_id_on);												      // ��������� / ���������� ���������

if (graph_id_aim_on == 0)
{
	IOWR_32DIRECT(GRAPH_ID_GENERATE_BASE, 16, 0);
}
else if ((graph_id_aim_on == 1)&&(graph_id_aim_color == 1))
{
	IOWR_32DIRECT(GRAPH_ID_GENERATE_BASE, 16, 1);
}
else if ((graph_id_aim_on == 1)&&(graph_id_aim_color == 0))
{
	IOWR_32DIRECT(GRAPH_ID_GENERATE_BASE, 16, 2);
}

for (i = 0; i < 3; i++)
{
	for (j = 0; j < 80; j++) {write_char_to_ram (j, i, Data_clear);	}
}

int count;


IOWR_32DIRECT(FPA_640_512_BASE, 20, 10000); // ����� ����������

/*------------------------------------------------------------------------------------------------------------------------------------------
---------------------- ������� ����: -------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------*/

while (1)
{
adc_delay = 0;
adc_delay_set(adc_delay);

//------------------------------------------------------------------------------------------------
// ����� ������� ���������� � UDP:
//------------------------------------------------------------------------------------------------
power_to_peletie	 = IORD_32DIRECT(0xc0, 16); 				// ���� 7, ������ ����� 4

byte_from_udp_2 	= IORD_32DIRECT(0xc0, 0);
byte_from_udp_3 	= IORD_32DIRECT(0xc0, 4);
aru_lock_from_udp	= ((byte_from_udp_2)&(00000001));
aru_on_from_udp		= ((byte_from_udp_2>>1)&1);
gain_from_udp	  	= IORD_32DIRECT(0xc0, 12);
if (gain_from_udp == 0) gain_from_udp = 1;


if (aru_on_from_udp == 1) {
	aru_on	= 1;
}
else {
	aru_on	= 0;
	int_time_from_udp 	= IORD_32DIRECT(0xc0, 8);
	int_time_ns 	  	  = int_time_from_udp*10000;
}

contrast_on				    = (byte_from_udp_3&1);
graph_id_on				    = ((byte_from_udp_3>>1)&1);
graph_id_aim_on			  = ((byte_from_udp_2>>6)&1); 	// ( ((byte_from_udp_2>>7)&1) )&( ((byte_from_udp_2>>6)&1) );
graph_id_aim_color		= ((byte_from_udp_2>>2)&1);

sensor_mode_from_udp = ((byte_from_udp_2>>4)&1);
if (sensor_mode_from_udp == 1) { sensor_mode_from_udp = 0; }
else {sensor_mode_from_udp = 1; }

stereo_mod_on_from_udp 	= ((byte_from_udp_3>>2)&1);

byte_from_udp_8			= IORD_32DIRECT(0xc0, 20);  	// avalon mm addr = 5; byte_8  = 0|UP(6,5,4)|IM(3,2)|BW(1)|0
byte_from_udp_9			= IORD_32DIRECT(0xc0, 24);  	// avalon mm addr = 6; byte_9  = LP(7,6)|DP(5,4)|MP(3,2)|AP(1,0)
byte_from_udp_10		= IORD_32DIRECT(0xc0, 28);  	// avalon mm addr = 7; byte_10 = OE_EN(6)|RE(5)|TS(4,3,2,1,0))

sensor_reg_up			= ((byte_from_udp_8>>3)&7);
sensor_reg_im			= ((byte_from_udp_8>>1)&3);
sensor_reg_bw			= ((byte_from_udp_8>>0)&1);

sensor_control_word 	= (sensor_reg_up*16) + (sensor_reg_im*4) + (sensor_reg_bw*2) + sensor_mode_from_udp;
sensor_control_command (sensor_control_word);
IOWR_32DIRECT(FPA_640_512_BASE, 24, byte_from_udp_9);
IOWR_32DIRECT(FPA_640_512_BASE, 28, byte_from_udp_10);

//------------------------------------------------------------------------------------------------
// ��������� ���������� ����������:
//------------------------------------------------------------------------------------------------
while (histogram_calc_done == 0)
{
	hist_calc_going_pre = hist_calc_going;
	hist_calc_going		= IORD_32DIRECT(0xa0, 0);
	if ((hist_calc_going == 0) && (hist_calc_going_pre > 0))
	{
		histogram_calc_done = 1;
	}
	while (delay < 10)
	{
		delay ++;
	}
	delay = 0;

}

	histogram_level_low 		  = IORD_32DIRECT(0xa0, 4);
	histogram_level_high 		  = IORD_32DIRECT(0xa0, 8);
	histogram_level_for_aru 	= IORD_32DIRECT(0xa0, 12);
	histogram_calc_done			  = 0;


	frame_counter ++;

	if 		(frame_counter == 125)
	{
		IOWR_32DIRECT(FPA_640_512_BASE, 16, 1);
	}
	if 		(frame_counter == 126)
	{
		IOWR_32DIRECT(FPA_640_512_BASE, 16, 0);
		frame_counter = 0;
	}

//------------------------------------------------------------------------------------------------
// ��� (���� �������� ������):
//------------------------------------------------------------------------------------------------
if (aru_on)
{
	if (histogram_level_for_aru < 200)
	{
		int_time_ns = int_time_ns*210;
		int_time_ns = int_time_ns/histogram_level_for_aru;
	}
	else if (histogram_level_for_aru > 220)
	{
		int_time_ns = int_time_ns*210;
		int_time_ns = int_time_ns/histogram_level_for_aru;
	}
}
if (int_time_ns > INT_TIME_MAX_ns)	int_time_ns = INT_TIME_MAX_ns;
if (int_time_ns < INT_TIME_MIN_ns)	int_time_ns = INT_TIME_MIN_ns;
int_time_set(int_time_ns);

//------------------------------------------------------------------------------------------------
// ����������������:
//------------------------------------------------------------------------------------------------
if (contrast_on)
{
	zoom_denominator 	= ((histogram_level_high+1) * 256) - 1;
	zoom 				      = 65536  *zoom_numerator + zoom_denominator;
	contrast_zoom_value_set(zoom);
	zoom_low_level		= histogram_level_low * 255;
	IOWR_32DIRECT(0x80, 8, zoom_low_level);
	contrast_video_ON_OFF (contrast_on);
}
else
{
	if (aru_on)
	{
		zoom_denominator 	= 65535;
		zoom 				= 65536*zoom_numerator + zoom_denominator;
		contrast_zoom_value_set(zoom);
		zoom_low_level		= 5 * 255;
		IOWR_32DIRECT(0x80, 8, zoom_low_level);
		contrast_video_ON_OFF (1);
	}
	else
	{
		zoom_denominator 	= zoom_signal_max_level/gain_from_udp;
		zoom 				= 65536*zoom_numerator + zoom_denominator;
		contrast_zoom_value_set(zoom);
		zoom_low_level		= 5 * 255;
		IOWR_32DIRECT(0x80, 8, zoom_low_level);
		contrast_video_ON_OFF (1);
	}
}
gain =  ((zoom_signal_max_level*10) / zoom_denominator);


//------------------------------------------------------------------------------------------------
// ����������� �������:
//------------------------------------------------------------------------------------------------

temp_adc_data 	= IORD_32DIRECT(0xe0, 0);
temp_mU 	  	  = (temp_adc_data*temp_adc_vref)/32767;
temp_current	  = (-((temp_mU - 779.96)*100)/14.8)*(0.824)+305; // *(0.824)+30.5;
temp_values[0]	= temp_values[1];
temp_values[1]	= temp_values[2];
temp_values[2]	= temp_values[3];
temp_values[3]	= temp_values[4];
temp_values[4]	= temp_current;
temperature		  = (temp_values[0] + temp_values[1] + temp_values[2] + temp_values[3] + temp_values[4])/5;

temp_run_on (1);

if (init_digital_R_count < 100)
{
	value_to_R 				= 6146;
	init_digital_R_count	= init_digital_R_count + 1;
}
else
{
	if (power_to_peletie > 100)
	{
		power_to_peletie = 100;
	}
	else if (power_to_peletie < 1)
	{
		power_to_peletie = 1;
	}
	value_to_R = 1024 + (1023 - power_to_peletie*670/100); //
}
temp_value_to_R(value_to_R);


//------------------------------------------------------------------------------------------------
// ����������� ���������. ���������� ������ �� ������:
//------------------------------------------------------------------------------------------------
	if (graph_id_on == 1)
	{
			// ����������� ����������� �����������:
			int temp_value;
			unsigned char temp_char;
			if (temperature >= 0)								// ������������ ����� + ��� -
			{
				temp_value = temperature;
				write_char_to_ram (17, 1, Data_plus);
			}
			else
			{
				temp_value = 0 - temperature;
				write_char_to_ram (17, 1, Data_minus);
			}
			temp_char = (temp_value/100)%10;
			select_numeric (temp_char, 18, 1);
			temp_char = (temp_value/10)%10;
			select_numeric (temp_char, 19, 1);
			temp_char = (temp_value/1)%10;
			select_numeric (temp_char, 21, 1);

			// ����������� ����������� ����� ����������:

			int int_char;
			int temp_int_char_value = IORD_32DIRECT(FPA_640_512_BASE, 0);
			temp_int_char_value 	= temp_int_char_value / 100;
			int_char = (temp_int_char_value/100000)%10;
			select_numeric (int_char, 16, 2);
			int_char = (temp_int_char_value/10000)%10;
			select_numeric (int_char, 17, 2);
			int_char = (temp_int_char_value/1000)%10;
			select_numeric (int_char, 19, 2);
			int_char = (temp_int_char_value/100)%10;
			select_numeric (int_char, 20, 2);
			int_char = (temp_int_char_value/10)%10;
			select_numeric (int_char, 21, 2);

			// ����������� ����������� ��������:
			int zoom_denominator_for_id = IORD_32DIRECT(0x80, 0);
			int gain_value;
			unsigned char gain_char;
			gain_value 	      = ((zoom_signal_max_level*10) / zoom_denominator_for_id);
			gain_char         = (gain_value/100)%10;
			select_numeric (gain_char, 18, 3);
			gain_char         = (gain_value/10)%10;
			select_numeric (gain_char, 19, 3);
			gain_char         = (gain_value/1)%10;
			select_numeric (gain_char, 21, 3);
		  
			// ����������� ����������� �������� ����������:
			int power_to_peletie_value;
			unsigned char power_to_peletie_char;
			power_to_peletie_value 	= power_to_peletie;
			power_to_peletie_char = (power_to_peletie_value/100)%10;
			select_numeric (power_to_peletie_char, 59, 3);
			power_to_peletie_char = (power_to_peletie_value/10)%10;
			select_numeric (power_to_peletie_char, 60, 3);
			power_to_peletie_char = (power_to_peletie_value/1)%10;
			select_numeric (power_to_peletie_char, 61, 3);
	}
	graph_id_ON_OFF (graph_id_on);

	if (graph_id_aim_on == 0)
	{
		IOWR_32DIRECT(GRAPH_ID_GENERATE_BASE, 16, 0);
	}
	else if ((graph_id_aim_on == 1)&&(graph_id_aim_color == 1))
	{
		IOWR_32DIRECT(GRAPH_ID_GENERATE_BASE, 16, 1);
	}
	else if ((graph_id_aim_on == 1)&&(graph_id_aim_color == 0))
	{
		IOWR_32DIRECT(GRAPH_ID_GENERATE_BASE, 16, 2);
	}

}
return 0;
}
