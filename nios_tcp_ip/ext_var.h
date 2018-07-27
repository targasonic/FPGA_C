#ifndef EXT_VAR_H_
#define EXT_VAR_H_


union byte_bits {
    struct  {
    unsigned b0 : 1;
    unsigned b1 : 1;
    unsigned b2 : 1;
    unsigned b3 : 1;
    unsigned b4 : 1;
    unsigned b5 : 1;
    unsigned b6 : 1;
    unsigned b7 : 1;
    };
    unsigned char byte;
};

union long_bits {
    struct  {
    unsigned b0 : 1;
    unsigned b1 : 1;
    unsigned b2 : 1;
    unsigned b3 : 1;
    unsigned b4 : 1;
    unsigned b5 : 1;
    unsigned b6 : 1;
    unsigned b7 : 1;
    unsigned b8 : 1;
    unsigned b9 : 1;
    unsigned b10 : 1;
    unsigned b11 : 1;
    unsigned b12 : 1;
    unsigned b13 : 1;
    unsigned b14 : 1;
    unsigned b15 : 1;
    unsigned b16 : 1;
    unsigned b17 : 1;
    unsigned b18 : 1;
    unsigned b19 : 1;
    unsigned b20 : 1;
    unsigned b21 : 1;
    unsigned b22 : 1;
    unsigned b23 : 1;
    unsigned b24 : 1;
    unsigned b25 : 1;
    unsigned b26 : 1;
    unsigned b27 : 1;
    unsigned b28 : 1;
    unsigned b29 : 1;
    unsigned b30 : 1;
    unsigned b31 : 1;
    };
    unsigned long data;
};

typedef union {
    unsigned long data ;
    struct {
        unsigned char byte0;
        unsigned char byte1;
        unsigned char byte2;
        unsigned char byte3;
    };
    struct {
        unsigned short word0;
        unsigned short word1;
    };
} UDWORDbytes;

typedef union {
    unsigned short data ;
    struct {
        unsigned char byte0;
        unsigned char byte1;
    };
} UWORDbytes;

//----àäðåñíîå ïðîñòðàíñòâî MAC----------------
#define adr_reg_main_ip 0
#define adr_reg_host_ip 4   //1<<2
#define adr_reg_main_mac_l 8   //2<<2
#define adr_reg_main_mac_h 12   //3<<2
#define adr_reg_host_mac_l 16   //4<<2
#define adr_reg_port_tcp 16   //4<<2
#define adr_reg_host_mac_h 20   //5<<2
#define adr_reg_int_if 24  //6<<2
#define adr_reg_size_ip  28  //7<<2
#define adr_reg_int_ie 32  //8<<2
#define adr_reg_upr 36  //9<<2
#define adr_reg_status 40    //10<<2
#define adr_reg_sum_head_tcp 44    //11<<2
#define adr_reg_num_byte_tcp 48 //12<<2
#define adr_reg_num_byte_udp 52 //13<<2
#define adr_reg_length_data_fifo_out 56 //14<<2
#define adr_reg_begin_adr_read_ram_in 60 //15<<2
#define adr_reg_begin_byte_data_out 64 //16<<2
#define adr_reg_r_b_pak_out 68 //17<<2
#define adr_reg_length_data_in_b1 72 //18<<2
#define adr_reg_sequence_num_in_b1	76 //19<<2
#define adr_reg_acknow_num_in_b1 80 //20<<2
#define adr_reg_window_size_in_b1 84 //21<<2
#define adr_reg_flag_tcp_b1 88 //22<<2
#define adr_reg_tcp_port_source 92 //23<<2
#define adr_reg_tcp_port_dest 96 //24<<2
#define adr_reg_address_ram_data 100 //25<<2
#define adr_reg_tr_data_t1 104 //26>>2
#define adr_reg_tr_data_t2 108 //27>>2

#define adr_reg_length_data_in_b2 112 //28<<2
#define adr_reg_sequence_num_in_b2	116 //29<<2
#define adr_reg_acknow_num_in_b2 120 //30<<2
#define adr_reg_window_size_in_b2 124 //31<<2
#define adr_reg_flag_tcp_b2 128 //32<<2

#define adr_reg_length_data_in_b3 132 //33<<2
#define adr_reg_sequence_num_in_b3	136 //34<<2
#define adr_reg_acknow_num_in_b3 140 //35<<2
#define adr_reg_window_size_in_b3 144 //36<<2
#define adr_reg_flag_tcp_b3 148 //37<<2

#define adr_reg_ram_test_nios 152 //38>>2

#define adr_reg_fifo_test 156 //39>>2

#define adr_reg_host_ip_syn_b1 160 //40>>2
#define adr_reg_host_ip_syn_b2 164 //41>>2
#define adr_reg_host_ip_syn_b3 168 //42>>2

#define adr_reg_upr2 172 //43>>2

#define adr_reg_tab_in1 8192    //512<<2
#define adr_reg_tab_in2 16384    //1024<<2
#define adr_reg_tab_out 24576    //1536<<2
#define adr_reg_tab_in3 32768    //2048<<2
#define adr_reg_tab_beg_pak_out 40960

//------ìàñêè è áèòû ïðåðûâàíèé ------
#define mask_int_main_mac_if 0x00010000
#define int_main_mac_if 0x00000001
#define mask_int_in1_if 0x00020000
#define int_in1_if 0x00000002
#define mask_int_in2_if 0x00040000
#define int_in2_if 0x00000004
#define mask_int_out1_if 0x00080000
#define int_out1_if 0x00000008
#define mask_int_in3_if 0x00100000
#define int_in3_if 0x00000010
#define mask_int_tcp_pak_in1_if 0x00200000
#define int_tcp_pak_in1_if 0x00000020
#define mask_int_tcp_pak_in2_if 0x00400000
#define int_tcp_pak_in2_if 0x00000040
#define mask_int_tcp_pak_in3_if 0x00800000
#define int_tcp_pak_in3_if 0x00000080
#define mask_int2_if 0x01000000
#define int2_if 0x00000100

#define mask_int_main_mac_ie 0x00010000
#define int_main_mac_ie 0x00000001
#define mask_int_in_ie 0x00020000
#define int_in_ie 0x00000002
//#define mask_int_in2_ie 0x00040000
//#define int_in2_ie 0x00000004
#define mask_int_out1_ie 0x00080000
#define int_out1_ie 0x00000008
//#define mask_int_in3_ie 0x00100000
//#define int_in3_ie 0x00000010
#define mask_int_tcp_pak_ie 0x00200000
#define int_tcp_pak_ie 0x00000020
#define mask_int2_ie 0x00400000
#define int2_ie 0x00000040

//--------áèòû ðåãèñòðà STATUS--------------------
#define end_wr_fifo_out 0x00000001 	// ïåðåíîñ äàííûõ â âûõîäíîå ôèôî çàâåðøåí
#define ready_wr_out 0x00000002		//ìîæíî çàïèñûâàòü ñëåäóþùèé ïàêåò
#define fifo_tcp_wr_out_st 0x00000004	//
#define busy_r_b_pak_out 0x00000008	// çàãîëîâîê ïåðåïèñûâàåòñÿ
#define busy_wr_ram_data 0x00000010	// ìîäóëü îæèäàåò äîñòóï ê ïàìÿòè íà çàïèñü
#define busy_rd_ram_data 0x00000020	// ìîäóëü îæèäàåò äîñòóï ê ïàìÿòè íà ÷òåíèå
#define fifo_stream_ready 0x00000040	// åñòü äàííûå â fifo äëÿ ôîðìèðîâàíèÿ ïàêåòà
#define ena_connect 0x00000080	//ñîñòîÿíèå âõîäà óñòàíîâêè ñîåäèíåíèÿ


//--------áèòû ðåãèñòðà óïðàâëåíèÿ---------------
#define mask_ena_out 0x00010000
#define ena_out 0x00000001
#define mask_eth_ready 0x00020000
#define eth_ready 0x00000002
#define mask_test_pin 0x00040000
#define test_pin 0x00000004
#define mask_led1 0x00080000
#define led1 0x00000008
#define mask_led2 0x00100000
#define led2 0x00000010
#define mask_fifo_tcp_wr_out 0x00200000
#define fifo_tcp_wr_out 0x00000020
#define mask_fifo_udp_wr_out 0x00400000
#define fifo_udp_wr_out 0x00000040
#define mask_ena_wr_fifo_out_ram_b1 0x00800000
#define ena_wr_fifo_out_ram_b1 0x00000080
#define mask_ena_wr_fifo_out_ram_b2 0x01000000
#define ena_wr_fifo_out_ram_b2 0x00000100
#define mask_ena_wr_fifo_out_ram_b3 0x02000000
#define ena_wr_fifo_out_ram_b3 0x00000200
#define mask_ena_ip_crc 0x04000000
#define ena_ip_crc 0x00000400
#define mask_ena_work_tcp_irq  0x08000000
#define ena_work_tcp_irq 0x00000800
#define mask_ena_wr_ram_data  0x10000000
#define ena_wr_ram_data 0x00001000
#define mask_ena_rd_ram_data  0x20000000
#define ena_rd_ram_data 0x00002000
#define mask_connect_ok 0x40000000
#define connect_ok 0x00004000
#define mask_connect_err 0x80000000
#define connect_err 0x00008000

//--------áèòû ðåãèñòðà óïðàâëåíèÿ 2---------------
#define mask_connect_port_1 0x00010000
#define connect_port_1 0x00000001
#define mask_connect_port_2 0x00020000
#define connect_port_2 0x00000002

extern volatile UDWORDbytes main_ip;
extern volatile UDWORDbytes host_ip;
extern volatile UDWORDbytes host_mac_h;
extern volatile UDWORDbytes host_mac_l;
extern volatile UDWORDbytes main_mac_h;
extern volatile UDWORDbytes main_mac_l;
extern volatile UDWORDbytes ping_host_mac_h;
extern volatile UDWORDbytes ping_host_mac_l;

extern volatile UDWORDbytes sequence_num_out;
extern volatile UDWORDbytes acknow_num_out;
extern volatile UWORDbytes window_size_out;

extern volatile UDWORDbytes sequence_num_in;
extern volatile UDWORDbytes acknow_num_in;
extern volatile UWORDbytes window_size_in;
extern volatile unsigned long window_left,window_middle,window_rigth;
extern volatile union long_bits flag1;
extern volatile UDWORDbytes data_in[40];
extern volatile UWORDbytes port_tcp;
extern volatile UWORDbytes main_port_tcp;
extern volatile UWORDbytes host_port_tcp;

extern volatile unsigned short length_data_in;
extern volatile unsigned char TCP_STREAM;
extern volatile unsigned long acknow_num_in_old;
extern volatile unsigned char prior_b1,prior_b2,prior_b3;
extern volatile UDWORDbytes arp_req_ip;
extern volatile UDWORDbytes arp_req_mac_h;
extern volatile UDWORDbytes arp_req_mac_l;
extern volatile unsigned long find_acknow_num_in;
extern volatile UDWORDbytes timer_repeat_pak;

extern volatile unsigned char table_eth[14];
extern volatile unsigned char table_ip[20];
extern volatile unsigned char table_tcp[20];
extern volatile unsigned char table_arp[30];
extern volatile unsigned char rflag_in1_pak;
extern volatile unsigned char rflag_in2_pak;
extern volatile unsigned char rflag_in3_pak;

extern volatile unsigned char table_bootp[42];

extern volatile unsigned long timer_main;
extern volatile unsigned char ttt;
extern volatile unsigned char ttt1;
extern volatile unsigned char ttt2;

extern volatile unsigned short window_left_ram;
extern volatile unsigned short window_rigth_ram;
extern volatile unsigned short const_window_rigth_ram;
extern volatile unsigned char TCP_OPEN;
extern volatile unsigned char BOOTP;
extern volatile UWORDbytes ident_ip;

extern volatile UWORDbytes host_port_udp;
extern volatile UWORDbytes main_port_udp;
extern volatile UWORDbytes length_udp;

extern volatile unsigned char table_udp[8];

extern volatile UDWORDbytes source_ip_ping;
extern volatile unsigned int ident_ping;
extern volatile unsigned int sequence_ping;
extern volatile UWORDbytes length_data_ping;
extern volatile unsigned char flag_in_ping;
extern volatile UDWORDbytes data_in_ping[20];
extern volatile unsigned char flag_int;
extern volatile unsigned char flag_tcp_port;
extern volatile unsigned short sourse_port_in;
extern volatile UDWORDbytes host_ip_syn_in;

extern volatile UWORDbytes main_port_tcp_1;
extern volatile UWORDbytes main_port_tcp_2;

extern volatile unsigned int timer_tcp_open;
extern volatile unsigned long acknow_num_out_old;
extern volatile unsigned char flag_find_pak_tcp_b;
extern volatile union byte_bits flags_tcp;

extern volatile unsigned char flag_timer_1;
extern volatile unsigned char flag_work_timer_1;

#define flag_fin_tcp                flags_tcp.b0
#define flag_syn_tcp                flags_tcp.b1
#define flag_rst_tcp                flags_tcp.b2
#define flag_psh_tcp                flags_tcp.b3
#define flag_ack_tcp                flags_tcp.b4

extern volatile unsigned long main_sequence_num_in;

#define flag_find_bootp_repl        flag1.b0
#define flag_find_bootp_repl_ok     flag1.b1
#define flag_find_arp_out           flag1.b2
#define flag_find_arp_out_ok        flag1.b3
#define flag_find_new_pak_tcp       flag1.b4
#define flag_find_pak_tcp			flag1.b5
#define flag_in1_pak                flag1.b10
#define flag_in2_pak                flag1.b11
#define flag_in3_pak                flag1.b12
#define flag_bootp_ok               flag1.b13
#define flag_tcp_open               flag1.b14
#define flag_find_arp_in            flag1.b15
#define flag_timer_0                flag1.b16
#define flag_work_timer_0			flag1.b19
#define flag_udp_in_pak				flag1.b20
#define flag_open_udp				flag1.b21
#define flag_new_pak_tcp_2			flag1.b22
#define flag_bootp_no				flag1.b23
#define flag_ena_work				flag1.b24
#define flag_end_stream				flag1.b25
#define flag_tcp_open_port_1        flag1.b26
#define flag_tcp_open_port_2        flag1.b27
#define flag_close_tcp_main			flag1.b28
#define flag_close_tcp_host			flag1.b29
#define flag_tcp_reopen_port_1      flag1.b30

extern volatile unsigned char flag_byte_tcp_open;

#endif /*EXT_VAR_H_*/
