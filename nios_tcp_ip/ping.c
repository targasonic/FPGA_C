#include "system.h"
#include <io.h>
#include "alt_types.h"
#include "ext_var.h"
#include "ext_call.h"
#include <unistd.h>

volatile UDWORDbytes ping_host_mac_h;
volatile UDWORDbytes ping_host_mac_l;

volatile UWORDbytes ch_summ (unsigned char* point_tab,unsigned short len)
{
unsigned short i;
unsigned char* point_tab_tmp;
UDWORDbytes summ_tmp1;
UWORDbytes summ_tmp2;


	point_tab_tmp=point_tab;
	summ_tmp1.data=0;
	i=0;
	do{
		summ_tmp1.data=summ_tmp1.data + (((unsigned long) (*point_tab_tmp)<<8));
		point_tab_tmp++;
		i++;
		if (i==len) break;
		summ_tmp1.data=summ_tmp1.data + ((unsigned long) (*point_tab_tmp));
		point_tab_tmp++;
		i++;
		if (i==len) break;
	}while (1);
	summ_tmp1.data=(unsigned long)summ_tmp1.word0+(unsigned long)summ_tmp1.word1;
	summ_tmp1.data=(unsigned long)summ_tmp1.word0+(unsigned long)summ_tmp1.word1;
	summ_tmp2.data=summ_tmp1.word0;
	summ_tmp2.data^=0xffff;
	return (summ_tmp2);
}


void ping_repl(void)
{
	unsigned long l,p;
volatile UWORDbytes k;
volatile UDWORDbytes ks;
unsigned char* point_ping;
	if (flag_in_ping==0) return;
    while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& ready_wr_out)==0);

    flag_new_pak_tcp_2=0;

	table_eth[0]=ping_host_mac_l.byte0;
    table_eth[1]=ping_host_mac_l.byte1;
    table_eth[2]=ping_host_mac_l.byte2;
    table_eth[3]=ping_host_mac_l.byte3;
    table_eth[4]=ping_host_mac_h.byte0;
    table_eth[5]=ping_host_mac_h.byte1;
    table_eth[6]=main_mac_l.byte0;
    table_eth[7]=main_mac_l.byte1;
    table_eth[8]=main_mac_l.byte2;
    table_eth[9]=main_mac_l.byte3;
    table_eth[10]=main_mac_h.byte0;
    table_eth[11]=main_mac_h.byte1;
	table_eth[12]=0x08;
    table_eth[13]=0x00;

	k.data=length_data_ping.data+20+8;
    table_ip[2]=k.byte1;
    table_ip[3]=k.byte0;
    table_ip[4]=ident_ip.byte1;
    table_ip[5]=ident_ip.byte0;
    table_ip[9]=0x01;
    table_ip[10]=0;
    table_ip[11]=0;
    table_ip[12]=main_ip.byte0;
    table_ip[13]=main_ip.byte1;
    table_ip[14]=main_ip.byte2;
    table_ip[15]=main_ip.byte3;
    table_ip[16]=source_ip_ping.byte0;
    table_ip[17]=source_ip_ping.byte1;
    table_ip[18]=source_ip_ping.byte2;
    table_ip[19]=source_ip_ping.byte3;
    ch_summ_ip((unsigned char*)&table_ip[0]);
//    k=ch_summ_ip((unsigned char*)&table_ip[0],20);
//    table_ip[10]=k.byte1;
//    table_ip[11]=k.byte0;
    ident_ip.data++;

    data_in_ping[0].word1=0;
    data_in_ping[1].word0=0;
    k=ch_summ(((unsigned char*)&data_in_ping[0]+2),length_data_ping.data+8);
    data_in_ping[1].byte0=k.byte1;
    data_in_ping[1].byte1=k.byte0;

    flag_new_pak_tcp_2=0;
	IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out,table_eth[0]+(table_eth[1]<<8)+(table_eth[2]<<16)+(table_eth[3]<<24));
	IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+4,table_eth[4]+(table_eth[5]<<8)+(table_eth[6]<<16)+(table_eth[7]<<24));
	IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+8,table_eth[8]+(table_eth[9]<<8)+(table_eth[10]<<16)+(table_eth[11]<<24));
	IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+12,table_eth[12]+(table_eth[13]<<8)+(table_ip[0]<<16)+(table_ip[1]<<24));
	IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+16,table_ip[2]+(table_ip[3]<<8)+(table_ip[4]<<16)+(table_ip[5]<<24));
	IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+20,table_ip[6]+(table_ip[7]<<8)+(table_ip[8]<<16)+(table_ip[9]<<24));
	IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+24,table_ip[10]+(table_ip[11]<<8)+(table_ip[12]<<16)+(table_ip[13]<<24));
	IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+28,table_ip[14]+(table_ip[15]<<8)+(table_ip[16]<<16)+(table_ip[17]<<24));
	IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+32,table_ip[18]+(table_ip[19]<<8)+(data_in_ping[0].byte2<<16)+(data_in_ping[0].byte3<<24));
//íàäî äîïèñàòü
	point_ping=((unsigned char*)&data_in_ping[0]+4);
	l=0;
	p=0;
	do{
		ks.data=((unsigned long) (*point_ping));
		point_ping++;
		l++;
		if (l==length_data_ping.data+6) {p=p+4;IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+32+p,ks.data);break;}
		ks.data=ks.data + (((unsigned long) (*point_ping)<<8));
		point_ping++;
		l++;
		if (l==length_data_ping.data+6) {p=p+4;IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+32+p,ks.data);break;}
		ks.data=ks.data + (((unsigned long) (*point_ping)<<16));
		point_ping++;
		l++;
		if (l==length_data_ping.data+6) {p=p+4;IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+32+p,ks.data);break;}
		ks.data=ks.data + (((unsigned long) (*point_ping)<<24));
		point_ping++;
		l++;
		if (l==length_data_ping.data+6) {p=p+4;IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+32+p,ks.data);break;}
		p=p+4;
		IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+32+p,ks.data);
	}while (1);

    IOWR_32DIRECT(MAC_B_BASE,adr_reg_r_b_pak_out,36+p);

    while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& fifo_tcp_wr_out_st)!=0) ;
    while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& (busy_wr_ram_data|busy_rd_ram_data))!=0) ;
    IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_wr_ram_data|mask_ena_rd_ram_data);

    IOWR_32DIRECT(MAC_B_BASE, adr_reg_size_ip,length_data_ping.data+8+20+14);
    usleep(500);
    IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_out|ena_out|mask_fifo_tcp_wr_out|mask_fifo_udp_wr_out|mask_ena_ip_crc);
    while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& busy_r_b_pak_out)!=0) ;

    flag_in_ping=0;

}
