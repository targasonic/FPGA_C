#include "system.h"
#include <io.h>
#include "alt_types.h"
#include "ext_var.h"
#include "ext_call.h"

volatile UDWORDbytes sequence_num_out;
volatile UDWORDbytes acknow_num_out;
volatile UWORDbytes window_size_out;

volatile UDWORDbytes sequence_num_in;
volatile UDWORDbytes acknow_num_in;
volatile unsigned long acknow_num_in_old;
volatile UWORDbytes window_size_in;

volatile unsigned char    TCP_OPEN=0;
volatile union byte_bits  flags_tcp;

volatile unsigned int timer_tcp_open=0;

volatile unsigned short sourse_port_in=0;
volatile UDWORDbytes host_ip_syn_in;

volatile unsigned char table_tcp[20]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
volatile unsigned char table_arp[30]={0x00,0x01,0x08,0x00,0x06,0x04,0x00,0x01,0x00,0x12,0x34,0x56,0x78,0x9a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
void tcp_open ()
{
 UDWORDbytes k;
 unsigned int i;
 
    switch (TCP_OPEN)
    {
        case 0:
				flag_tcp_reopen_port_1=0;
                flag_tcp_open=0;
                flag_tcp_open_port_1=0;
                flag_tcp_open_port_2=0;
                flag_close_tcp_host=0;
                flag_close_tcp_main=0;
                IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr2,mask_connect_port_1|mask_connect_port_2);
                IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_eth_ready|mask_connect_ok);
                sequence_num_out.data=sequence_num_out.data+100000;
                if (flag_ena_work!=0) TCP_OPEN=1;
                break;
        case 1:
				if ((timer_tcp_open==0)&&(flag_tcp_reopen_port_1==1)){
					TCP_OPEN=0;
					break;
				}
				if (flag_find_new_pak_tcp==0) break;
				if (flag_syn_tcp==0) break;
				if (flag_tcp_reopen_port_1==0){
					if ((flag_tcp_port!=1)&&(flag_tcp_port!=2)) break;
				}else {
					if (flag_tcp_port!=1) break;
				}
				timer_repeat_pak.data=10000;
                flag_tcp_open_port_1=0;
                flag_tcp_open_port_2=0;
                flag_close_tcp_host=0;
                flag_close_tcp_main=0;
				flag_tcp_reopen_port_1=0;
                IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr2,mask_connect_port_1|mask_connect_port_2);
				if (flag_tcp_port==1) {main_port_tcp.data=main_port_tcp_1.data;flag_tcp_open_port_1=1;}
				if (flag_tcp_port==2) {main_port_tcp.data=main_port_tcp_2.data;flag_tcp_open_port_2=1;}
				host_port_tcp.data=sourse_port_in;
				host_ip.data=host_ip_syn_in.data;
				acknow_num_out.data=sequence_num_in.data+1;
				main_sequence_num_in=sequence_num_in.data+1;
				TCP_OPEN=11;
				break;
       case 11:		
				while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& ready_wr_out)==0);
				table_eth[0]=0xff;
				table_eth[1]=0xff;
				table_eth[2]=0xff;
				table_eth[3]=0xff;
				table_eth[4]=0xff;
				table_eth[5]=0xff;
				table_eth[6]=main_mac_l.byte0;
				table_eth[7]=main_mac_l.byte1;
				table_eth[8]=main_mac_l.byte2;
				table_eth[9]=main_mac_l.byte3;
				table_eth[10]=main_mac_h.byte0;
				table_eth[11]=main_mac_h.byte1;
				table_eth[12]=0x08;
				table_eth[13]=0x06;

				table_arp[8]=main_mac_l.byte0;
				table_arp[9]=main_mac_l.byte1;
				table_arp[10]=main_mac_l.byte2;
				table_arp[11]=main_mac_l.byte3;
				table_arp[12]=main_mac_h.byte0;
				table_arp[13]=main_mac_h.byte1;
				table_arp[14]=main_ip.byte0;
				table_arp[15]=main_ip.byte1;
				table_arp[16]=main_ip.byte2;
				table_arp[17]=main_ip.byte3;
				table_arp[24]=host_ip.byte0;
				table_arp[25]=host_ip.byte1;
				table_arp[26]=host_ip.byte2;
				table_arp[27]=host_ip.byte3;

				IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out,table_eth[0]+(table_eth[1]<<8)+(table_eth[2]<<16)+(table_eth[3]<<24));
				IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+4,table_eth[4]+(table_eth[5]<<8)+(table_eth[6]<<16)+(table_eth[7]<<24));
				IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+8,table_eth[8]+(table_eth[9]<<8)+(table_eth[10]<<16)+(table_eth[11]<<24));
				IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+12,table_eth[12]+(table_eth[13]<<8)+(table_arp[0]<<16)+(table_arp[1]<<24));
				IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+16,table_arp[2]+(table_arp[3]<<8)+(table_arp[4]<<16)+(table_arp[5]<<24));
				IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+20,table_arp[6]+(table_arp[7]<<8)+(table_arp[8]<<16)+(table_arp[9]<<24));
				IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+24,table_arp[10]+(table_arp[11]<<8)+(table_arp[12]<<16)+(table_arp[13]<<24));
				IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+28,table_arp[14]+(table_arp[15]<<8)+(table_arp[16]<<16)+(table_arp[17]<<24));
				IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+32,table_arp[18]+(table_arp[19]<<8)+(table_arp[20]<<16)+(table_arp[21]<<24));
				IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+36,table_arp[22]+(table_arp[23]<<8)+(table_arp[24]<<16)+(table_arp[25]<<24));
				IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+40,table_arp[26]+(table_arp[27]<<8)+(table_arp[28]<<16)+(table_arp[29]<<24));

			  while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& fifo_tcp_wr_out_st)!=0) ;
			  while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& (busy_wr_ram_data|busy_rd_ram_data))!=0) ;
			  IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_wr_ram_data|mask_ena_rd_ram_data);

			  IOWR_32DIRECT(MAC_B_BASE,adr_reg_r_b_pak_out,11*4);
				IOWR_32DIRECT(MAC_B_BASE, adr_reg_size_ip,60);
				IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_out|ena_out|mask_fifo_tcp_wr_out);
				while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& busy_r_b_pak_out)!=0) ;
				flag_find_arp_out_ok=0;
				flag_find_arp_out=1;
				TCP_OPEN=12;
				timer_tcp_open=150;
				break;
        case 12:		
				if (timer_tcp_open==0){
					TCP_OPEN=11;
					break;
				}
                if (flag_find_arp_out_ok==0) break;
                TCP_OPEN=2;
                break;

        case 2:		
				while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& ready_wr_out)==0);
                window_size_out.data=1024;
                window_left_ram=0;
                window_rigth_ram=0;
                const_window_rigth_ram=256;

                table_eth[0]=host_mac_l.byte0;
                table_eth[1]=host_mac_l.byte1;
                table_eth[2]=host_mac_l.byte2;
                table_eth[3]=host_mac_l.byte3;
                table_eth[4]=host_mac_h.byte0;
                table_eth[5]=host_mac_h.byte1;
                table_eth[12]=0x08;
                table_eth[13]=0x00;

                table_ip[2]=0x00;
                table_ip[3]=0x28;
                table_ip[4]=ident_ip.byte1;
                table_ip[5]=ident_ip.byte0;
                table_ip[9]=0x06;
                table_ip[10]=0;
                table_ip[11]=0;
                table_ip[12]=main_ip.byte0;
                table_ip[13]=main_ip.byte1;
                table_ip[14]=main_ip.byte2;
                table_ip[15]=main_ip.byte3;
                table_ip[16]=host_ip.byte0;
                table_ip[17]=host_ip.byte1;
                table_ip[18]=host_ip.byte2;
                table_ip[19]=host_ip.byte3;
                ch_summ_ip((unsigned char*)&table_ip[0]);

                table_tcp[0]=main_port_tcp.byte1;
               	table_tcp[1]=main_port_tcp.byte0;
                table_tcp[2]=host_port_tcp.byte1;
               	table_tcp[3]=host_port_tcp.byte0;
                table_tcp[4]=sequence_num_out.byte3;
               	table_tcp[5]=sequence_num_out.byte2;
                table_tcp[6]=sequence_num_out.byte1;
               	table_tcp[7]=sequence_num_out.byte0;
                table_tcp[8]=acknow_num_out.byte3;
               	table_tcp[9]=acknow_num_out.byte2;
                table_tcp[10]=acknow_num_out.byte1;
               	table_tcp[11]=acknow_num_out.byte0;
                table_tcp[12]=0x50;
               	table_tcp[13]=0x12;
               	table_tcp[14]=window_size_out.byte1;
               	table_tcp[15]=window_size_out.byte0;
                table_tcp[16]=0;
               	table_tcp[17]=0;

                k.data=((unsigned long)main_ip.byte0<<8)+((unsigned long)main_ip.byte1)+((unsigned long)main_ip.byte2<<8)+((unsigned long)main_ip.byte3);
                k.data=k.data+((unsigned long)host_ip.byte0<<8)+((unsigned long)host_ip.byte1)+((unsigned long)host_ip.byte2<<8)+((unsigned long)host_ip.byte3);
                k.data=k.data+6+20;
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_sum_head_tcp,k.data);

                for (i=0;i<20;i=i+2)
                {
                	k.data=k.data+((unsigned long)table_tcp[i]<<8)+((unsigned long)table_tcp[i+1]);
                }
            	k.data=k.word0+k.word1;
            	k.word0=k.word0+k.word1;
            	k.word0^=0xffff;
            	table_tcp[16]=k.byte1;
            	table_tcp[17]=k.byte0;

                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out,table_eth[0]+(table_eth[1]<<8)+(table_eth[2]<<16)+(table_eth[3]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+4,table_eth[4]+(table_eth[5]<<8)+(table_eth[6]<<16)+(table_eth[7]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+8,table_eth[8]+(table_eth[9]<<8)+(table_eth[10]<<16)+(table_eth[11]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+12,table_eth[12]+(table_eth[13]<<8)+(table_ip[0]<<16)+(table_ip[1]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+16,table_ip[2]+(table_ip[3]<<8)+(table_ip[4]<<16)+(table_ip[5]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+20,table_ip[6]+(table_ip[7]<<8)+(table_ip[8]<<16)+(table_ip[9]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+24,table_ip[10]+(table_ip[11]<<8)+(table_ip[12]<<16)+(table_ip[13]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+28,table_ip[14]+(table_ip[15]<<8)+(table_ip[16]<<16)+(table_ip[17]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+32,table_ip[18]+(table_ip[19]<<8)+(table_tcp[0]<<16)+(table_tcp[1]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+36,table_tcp[2]+(table_tcp[3]<<8)+(table_tcp[4]<<16)+(table_tcp[5]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+40,table_tcp[6]+(table_tcp[7]<<8)+(table_tcp[8]<<16)+(table_tcp[9]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+44,table_tcp[10]+(table_tcp[11]<<8)+(table_tcp[12]<<16)+(table_tcp[13]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+48,table_tcp[14]+(table_tcp[15]<<8)+(table_tcp[16]<<16)+(table_tcp[17]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+52,table_tcp[18]+(table_tcp[19]<<8));
                while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& fifo_tcp_wr_out_st)!=0) ;
                while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& (busy_wr_ram_data|busy_rd_ram_data))!=0) ;
                IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_wr_ram_data|mask_ena_rd_ram_data);

                IOWR_32DIRECT(MAC_B_BASE,adr_reg_r_b_pak_out,14*4);
                IOWR_32DIRECT(MAC_B_BASE, adr_reg_size_ip,60);
                IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_out|ena_out|mask_fifo_tcp_wr_out);
                ident_ip.data++;
                while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& busy_r_b_pak_out)!=0) ;
                flag_find_new_pak_tcp=0;
                TCP_OPEN=3;
                timer_tcp_open=10;
                break;
        case 3: 
				if (timer_tcp_open==0){
					TCP_OPEN=2;
					break;
				}
                if (flag_find_new_pak_tcp==0) break;
                if (flag_rst_tcp!=0)
                {
    				if (flag_tcp_open_port_1==1) {
    					if ((flag_tcp_port==1)&&(host_port_tcp.data==sourse_port_in)&&(host_ip.data==host_ip_syn_in.data)){
    						flag_tcp_open=0;
    						flag_tcp_open_port_1=0;
    						TCP_OPEN=9;
    						break;}
    				}
    				if (flag_tcp_open_port_2==1) {
    					if ((flag_tcp_port==2)&&(host_port_tcp.data==sourse_port_in)&&(host_ip.data==host_ip_syn_in.data)){
    						flag_tcp_open=0;
    						flag_tcp_open_port_2=0;
    						TCP_OPEN=9;
    						break;}
    				}
                }
                if (flag_ack_tcp==0) break;
                if ((acknow_num_in.data-1)!=sequence_num_out.data) break;
                sequence_num_out.data++;
                window_left=acknow_num_in.data;
                acknow_num_in_old=acknow_num_in.data;
                window_middle=acknow_num_in.data;
                window_rigth=acknow_num_in.data+window_size_in.data;
                TCP_OPEN=5;
                flag_tcp_open=1;
                IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_eth_ready|eth_ready);
                IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_connect_ok|connect_ok);
                if (flag_tcp_open_port_1==1) IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr2,mask_connect_port_1|connect_port_1);
                if (flag_tcp_open_port_2==1) IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr2,mask_connect_port_2|connect_port_2);
                timer_tcp_open=200;
                break;
        case 5:	
    			if (flag_ena_work==0) {	
                    flag_tcp_open=0;
                    TCP_OPEN=51;
                    timer_tcp_open=5;
                    flag_close_tcp_main=1;
                    sequence_num_out.data=window_middle;
                    break;
    			}
                if (flag_find_new_pak_tcp==0) break;
                if (flag_tcp_open_port_2==1){
                	if ((flag_syn_tcp!=0)&&(flag_tcp_port==1)){
                		TCP_OPEN=51;
                		flag_tcp_open=0;
                		flag_tcp_reopen_port_1=1;
                		timer_tcp_open=5;
                		flag_close_tcp_main=1;
                		sequence_num_out.data=window_middle;
                		break;
                	}
                }
                if (flag_rst_tcp!=0) {
                    if ((host_port_tcp.data==sourse_port_in)&&(host_ip.data==host_ip_syn_in.data)){
                    	if (((flag_tcp_open_port_1==1)&&(flag_tcp_port==1))||((flag_tcp_open_port_2==1)&&(flag_tcp_port==2))){
                    		flag_tcp_open=0;
                    		timer_tcp_open=5;
                    		TCP_OPEN=9;
                    		break;}
                    }
                }
                if (flag_fin_tcp!=0){
                	if ((host_port_tcp.data==sourse_port_in)&&(host_ip.data==host_ip_syn_in.data)){
                		if (((flag_tcp_open_port_1==1)&&(flag_tcp_port==1))||((flag_tcp_open_port_2==1)&&(flag_tcp_port==2))){
                			flag_tcp_open=0;
                			TCP_OPEN=51;
                			timer_tcp_open=5;
                			flag_close_tcp_host=1;
                			acknow_num_out.data=sequence_num_in.data+1;
                			break;
                		}
                	}
                }
                if (timer_tcp_open!=0) break;
                if ((flag_syn_tcp!=0)&&(flag_tcp_port==1)){
                	flag_tcp_open=0;
                	timer_tcp_open=5;
                	TCP_OPEN=9;
                	break;
                }
                break;
        case 51:
			if (timer_tcp_open!=0) break;
				IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_eth_ready);
				IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_connect_ok);
				if (flag_close_tcp_main==1){
					TCP_OPEN=7;
					sequence_num_out.data=window_middle;
					break;
				}
				if (flag_close_tcp_host==1){
					TCP_OPEN=6;
					break;
				}
				break;

        case 6:
				if ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& ready_wr_out)==0) break;
				table_eth[12]=0x08;
                table_eth[13]=0x00;

                table_ip[2]=0x00;
                table_ip[3]=0x28;
                table_ip[4]=ident_ip.byte1;
                table_ip[5]=ident_ip.byte0;
                table_ip[9]=0x06;
                table_ip[10]=0;
                table_ip[11]=0;
                ch_summ_ip((unsigned char*)&table_ip[0]);

                if (flag_close_tcp_main==1){
                	sequence_num_out.data++;
                }else    sequence_num_out.data=window_middle;
                table_tcp[4]=sequence_num_out.byte3;
               	table_tcp[5]=sequence_num_out.byte2;
                table_tcp[6]=sequence_num_out.byte1;
               	table_tcp[7]=sequence_num_out.byte0;
                table_tcp[8]=acknow_num_out.byte3;
               	table_tcp[9]=acknow_num_out.byte2;
                table_tcp[10]=acknow_num_out.byte1;
               	table_tcp[11]=acknow_num_out.byte0;
               	table_tcp[13]=0x10;
                table_tcp[16]=0;
               	table_tcp[17]=0;

               	k.data=((unsigned long)main_ip.byte0<<8)+((unsigned long)main_ip.byte1)+((unsigned long)main_ip.byte2<<8)+((unsigned long)main_ip.byte3);
                k.data=k.data+((unsigned long)host_ip.byte0<<8)+((unsigned long)host_ip.byte1)+((unsigned long)host_ip.byte2<<8)+((unsigned long)host_ip.byte3);
                k.data=k.data+6+20;
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_sum_head_tcp,k.data);

                for (i=0;i<20;i=i+2)
                {
                	k.data=k.data+((unsigned long)table_tcp[i]<<8)+((unsigned long)table_tcp[i+1]);
                }
            	k.data=k.word0+k.word1;
            	k.word0=k.word0+k.word1;
            	k.word0^=0xffff;
            	table_tcp[16]=k.byte1;
            	table_tcp[17]=k.byte0;

                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out,table_eth[0]+(table_eth[1]<<8)+(table_eth[2]<<16)+(table_eth[3]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+4,table_eth[4]+(table_eth[5]<<8)+(table_eth[6]<<16)+(table_eth[7]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+8,table_eth[8]+(table_eth[9]<<8)+(table_eth[10]<<16)+(table_eth[11]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+12,table_eth[12]+(table_eth[13]<<8)+(table_ip[0]<<16)+(table_ip[1]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+16,table_ip[2]+(table_ip[3]<<8)+(table_ip[4]<<16)+(table_ip[5]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+20,table_ip[6]+(table_ip[7]<<8)+(table_ip[8]<<16)+(table_ip[9]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+24,table_ip[10]+(table_ip[11]<<8)+(table_ip[12]<<16)+(table_ip[13]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+28,table_ip[14]+(table_ip[15]<<8)+(table_ip[16]<<16)+(table_ip[17]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+32,table_ip[18]+(table_ip[19]<<8)+(table_tcp[0]<<16)+(table_tcp[1]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+36,table_tcp[2]+(table_tcp[3]<<8)+(table_tcp[4]<<16)+(table_tcp[5]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+40,table_tcp[6]+(table_tcp[7]<<8)+(table_tcp[8]<<16)+(table_tcp[9]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+44,table_tcp[10]+(table_tcp[11]<<8)+(table_tcp[12]<<16)+(table_tcp[13]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+48,table_tcp[14]+(table_tcp[15]<<8)+(table_tcp[16]<<16)+(table_tcp[17]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+52,table_tcp[18]+(table_tcp[19]<<8));

                while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& fifo_tcp_wr_out_st)!=0) ;
                while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& (busy_wr_ram_data|busy_rd_ram_data))!=0) ;
                IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_wr_ram_data|mask_ena_rd_ram_data);

                IOWR_32DIRECT(MAC_B_BASE,adr_reg_r_b_pak_out,14*4);
                IOWR_32DIRECT(MAC_B_BASE, adr_reg_size_ip,60);
                IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_out|ena_out|mask_fifo_tcp_wr_out);ident_ip.data++;
                while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& busy_r_b_pak_out)!=0) ;
                if (flag_close_tcp_main==1){
                	TCP_OPEN=9;
                	break;
                }
                TCP_OPEN=7;
                break;
        case 7:

				if ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& ready_wr_out)==0) break;

				table_eth[12]=0x08;
				table_eth[13]=0x00;

				table_ip[2]=0x00;
				table_ip[3]=0x28;
				table_ip[4]=ident_ip.byte1;
				table_ip[5]=ident_ip.byte0;
				table_ip[9]=0x06;
				table_ip[10]=0;
				table_ip[11]=0;

				ch_summ_ip((unsigned char*)&table_ip[0]);

				table_tcp[4]=sequence_num_out.byte3;
				table_tcp[5]=sequence_num_out.byte2;
				table_tcp[6]=sequence_num_out.byte1;
				table_tcp[7]=sequence_num_out.byte0;
				table_tcp[8]=acknow_num_out.byte3;
				table_tcp[9]=acknow_num_out.byte2;
				table_tcp[10]=acknow_num_out.byte1;
				table_tcp[11]=acknow_num_out.byte0;
				table_tcp[13]=0x11;
				table_tcp[16]=0;
				table_tcp[17]=0;

				k.data=((unsigned long)main_ip.byte0<<8)+((unsigned long)main_ip.byte1)+((unsigned long)main_ip.byte2<<8)+((unsigned long)main_ip.byte3);
				k.data=k.data+((unsigned long)host_ip.byte0<<8)+((unsigned long)host_ip.byte1)+((unsigned long)host_ip.byte2<<8)+((unsigned long)host_ip.byte3);
				k.data=k.data+6+20;
				IOWR_32DIRECT(MAC_B_BASE,adr_reg_sum_head_tcp,k.data);

				for (i=0;i<20;i=i+2)
				{
					k.data=k.data+((unsigned long)table_tcp[i]<<8)+((unsigned long)table_tcp[i+1]);
				}
				k.data=k.word0+k.word1;
				k.word0=k.word0+k.word1;
				k.word0^=0xffff;
				table_tcp[16]=k.byte1;
				table_tcp[17]=k.byte0;

                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out,table_eth[0]+(table_eth[1]<<8)+(table_eth[2]<<16)+(table_eth[3]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+4,table_eth[4]+(table_eth[5]<<8)+(table_eth[6]<<16)+(table_eth[7]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+8,table_eth[8]+(table_eth[9]<<8)+(table_eth[10]<<16)+(table_eth[11]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+12,table_eth[12]+(table_eth[13]<<8)+(table_ip[0]<<16)+(table_ip[1]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+16,table_ip[2]+(table_ip[3]<<8)+(table_ip[4]<<16)+(table_ip[5]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+20,table_ip[6]+(table_ip[7]<<8)+(table_ip[8]<<16)+(table_ip[9]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+24,table_ip[10]+(table_ip[11]<<8)+(table_ip[12]<<16)+(table_ip[13]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+28,table_ip[14]+(table_ip[15]<<8)+(table_ip[16]<<16)+(table_ip[17]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+32,table_ip[18]+(table_ip[19]<<8)+(table_tcp[0]<<16)+(table_tcp[1]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+36,table_tcp[2]+(table_tcp[3]<<8)+(table_tcp[4]<<16)+(table_tcp[5]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+40,table_tcp[6]+(table_tcp[7]<<8)+(table_tcp[8]<<16)+(table_tcp[9]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+44,table_tcp[10]+(table_tcp[11]<<8)+(table_tcp[12]<<16)+(table_tcp[13]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+48,table_tcp[14]+(table_tcp[15]<<8)+(table_tcp[16]<<16)+(table_tcp[17]<<24));
                IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+52,table_tcp[18]+(table_tcp[19]<<8));

                while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& fifo_tcp_wr_out_st)!=0) ;
                while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& (busy_wr_ram_data|busy_rd_ram_data))!=0) ;
                IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_wr_ram_data|mask_ena_rd_ram_data);

                IOWR_32DIRECT(MAC_B_BASE,adr_reg_r_b_pak_out,14*4);
                IOWR_32DIRECT(MAC_B_BASE, adr_reg_size_ip,60);
                IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_out|ena_out|mask_fifo_tcp_wr_out);ident_ip.data++;
                while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& busy_r_b_pak_out)!=0) ;
                TCP_OPEN=8;
                timer_tcp_open=500;
                break;
        case 8:
				if (timer_tcp_open==0){
					TCP_OPEN=9;
					break;
				}
                if (flag_find_new_pak_tcp==0) break;
                if (flag_ack_tcp==0) break;
                if ((acknow_num_in.data-1)!=sequence_num_out.data) break;
                timer_tcp_open=0;
                if (flag_close_tcp_main==1){
                	TCP_OPEN=20;
                	timer_tcp_open=2000;
                	break;
                }
                if (flag_close_tcp_host==1){
                	TCP_OPEN=9;
                	break;
                }
        case 9:
				if (timer_tcp_open!=0) break;
				IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_eth_ready);
				IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_connect_ok);
				if (flag_tcp_reopen_port_1==1) {TCP_OPEN=1;timer_tcp_open=500;}
				else TCP_OPEN=0;
                break;
        case 20:
				if (timer_tcp_open==0){
					flag_tcp_open=0;
					TCP_OPEN=9;
					break;
				}
                    if (flag_find_new_pak_tcp==0) break;
                    if (flag_rst_tcp!=0) {
                        if ((host_port_tcp.data==sourse_port_in)&&(host_ip.data==host_ip_syn_in.data)){
                        	if (((flag_tcp_open_port_1==1)&&(flag_tcp_port==1))||((flag_tcp_open_port_2==1)&&(flag_tcp_port==2))){
                        		flag_tcp_open=0;
                        		timer_tcp_open=2;
                        		TCP_OPEN=9;
                        		break;}
                        }
                    }
                   if (flag_fin_tcp==0) break;
                    if ((host_port_tcp.data==sourse_port_in)&&(host_ip.data==host_ip_syn_in.data)){
                    	if (((flag_tcp_open_port_1==1)&&(flag_tcp_port==1))||((flag_tcp_open_port_2==1)&&(flag_tcp_port==2))){
                    		TCP_OPEN=6;
                    		acknow_num_out.data=sequence_num_in.data+1;
                            break;
                    	}
                    }
                    break;
    }
}
