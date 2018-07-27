#include "system.h"
#include <io.h>
#include "alt_types.h"
#include "ext_var.h"

volatile UDWORDbytes main_ip;
volatile UDWORDbytes host_ip;
volatile UDWORDbytes host_mac_h;
volatile UDWORDbytes host_mac_l;
volatile UDWORDbytes main_mac_h;
volatile UDWORDbytes main_mac_l;

volatile UWORDbytes ident_ip;

volatile unsigned char REG_INIT=0;

volatile unsigned char table_eth[14]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00};
volatile unsigned char table_ip[20]={0x45,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff};

void ch_summ_ip (unsigned char* point_tab)
{
unsigned int i;
unsigned char* point_tab_tmp;
UDWORDbytes summ_tmp1,summ_tmp2;

	point_tab_tmp=point_tab;
	summ_tmp1.data=0;
	for (i=0;i<10;i++)
	{
		summ_tmp1.data=summ_tmp1.data + (((unsigned long) (*point_tab_tmp)<<8));
		point_tab_tmp++;
		summ_tmp1.data=summ_tmp1.data + ((unsigned long) (*point_tab_tmp));
		point_tab_tmp++;
	}
	summ_tmp2.data=summ_tmp1.word0+summ_tmp1.word1;
	summ_tmp2.word0=summ_tmp2.word0+summ_tmp2.word1;
	summ_tmp2.word0^=0xffff;
	point_tab_tmp=point_tab+10;
	*point_tab_tmp=summ_tmp2.byte1;
	point_tab_tmp++;
	*point_tab_tmp=summ_tmp2.byte0;
}

unsigned int init(void)
{

	switch (REG_INIT)
	{
	case 0:
		if  ((IORD_32DIRECT(MAC_B_BASE,adr_reg_status)&ena_connect)==0) break;

    	 main_mac_h.data=IORD_32DIRECT(MAC_B_BASE,adr_reg_main_mac_h);
	    main_mac_l.data=IORD_32DIRECT(MAC_B_BASE,adr_reg_main_mac_l);

	    IOWR_32DIRECT(MAC_B_BASE, adr_reg_main_mac_l,main_mac_l.data);
	    IOWR_32DIRECT(MAC_B_BASE, adr_reg_main_mac_h,main_mac_h.data);

	    main_ip.data=IORD_32DIRECT(MAC_B_BASE,adr_reg_main_ip);
	    main_port_tcp_1.data=(unsigned short)IORD_32DIRECT(MAC_B_BASE,adr_reg_port_tcp);
	    main_port_tcp_2.data=(unsigned short)(IORD_32DIRECT(MAC_B_BASE,adr_reg_port_tcp)>>16);

	    IOWR_32DIRECT(MAC_B_BASE,adr_reg_tcp_port_source,main_port_tcp_2.data);
	    IOWR_32DIRECT(MAC_B_BASE,adr_reg_tcp_port_dest,main_port_tcp_1.data);

	    IOWR_32DIRECT(MAC_B_BASE, adr_reg_int_if,mask_int_in1_if|mask_int_in2_if|mask_int_in3_if|mask_int_main_mac_if);
	    IOWR_32DIRECT(MAC_B_BASE, adr_reg_int_ie,mask_int_in_ie|int_in_ie|mask_int_tcp_pak_ie|int_tcp_pak_ie);
	    IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_work_tcp_irq|ena_work_tcp_irq);

	    REG_INIT=1;
	    flag_ena_work=1;
	    break;
	case 1:
		if  ((IORD_32DIRECT(MAC_B_BASE,adr_reg_status)&ena_connect)!=0) break;
		flag_ena_work=0;
		REG_INIT=0;
		break;
	}
    return 0;
}
