#include "system.h"
#include <io.h>
#include "alt_types.h"
#include "ext_var.h"
#include <unistd.h>
void arp_repl(void)
{

	if (flag_find_arp_in==0) return;
    while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& ready_wr_out)==0);
    flag_new_pak_tcp_2=0;
    IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out,0xffffffff);
    IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+4,0xffff + (main_mac_l.word0<<16));
    IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+8,main_mac_l.word1 + (main_mac_h.word0<<16));
    IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+12,0x01000608);
    IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+16,0x04060008);
    IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+20,(main_mac_l.word0<<16) + 0x0200);
    IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+24,(main_mac_h.word0<<16) + main_mac_l.word1);
    IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+28,(main_ip.word1<<16) + main_ip.word0);
    IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+32,(arp_req_mac_l.word1<<16) + arp_req_mac_l.word0);
    IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+36,(arp_req_ip.word0<<16) + arp_req_mac_h.word0);
    IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+40, arp_req_ip.word1);

    IOWR_32DIRECT(MAC_B_BASE,adr_reg_r_b_pak_out,11*4);

    while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& fifo_tcp_wr_out_st)!=0) ;
    while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& (busy_wr_ram_data|busy_rd_ram_data))!=0) ;
    IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_wr_ram_data|mask_ena_rd_ram_data);

    IOWR_32DIRECT(MAC_B_BASE, adr_reg_size_ip,60);
    usleep(500);
    IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_out|ena_out|mask_fifo_tcp_wr_out|mask_fifo_udp_wr_out|mask_ena_ip_crc);
    while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& busy_r_b_pak_out)!=0) ;
    flag_find_arp_in=0;
}
