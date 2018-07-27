#include "system.h"
#include <io.h>
#include "alt_types.h"
#include "ext_var.h"
#include "ext_call.h"

volatile unsigned long window_left,window_middle,window_rigth;
volatile unsigned short length_data_in;
volatile unsigned int length_data_out=1024;
volatile unsigned char TCP_STREAM=0;
volatile unsigned long acknow_num_out_old;
volatile unsigned long find_acknow_num_in;
volatile unsigned char flag_wr_ram;

volatile unsigned char flag_wr_window_left_ram=0;
volatile unsigned char flag_new_wr=0;
volatile unsigned short window_ram_tmp;
volatile unsigned short window_left_ram;
volatile unsigned short window_middle_ram;
volatile unsigned short window_rigth_ram;
volatile unsigned short const_window_rigth_ram;

volatile unsigned char bit_error=0;
volatile unsigned int nums_error=0;
volatile unsigned int tmp_dat1=0;
volatile unsigned int tmp_dat2=0;

volatile UDWORDbytes sum_head_tcp;
volatile unsigned long old1_timer_repeat_pak=0;
volatile unsigned long old2_timer_repeat_pak=0;
volatile unsigned long old3_timer_repeat_pak=0;
volatile unsigned long old4_timer_repeat_pak=0;
volatile unsigned long old5_timer_repeat_pak=0;
volatile unsigned long old6_timer_repeat_pak=0;

volatile UDWORDbytes timer_repeat_pak;

void tcp_work_in_pak(void)
{
	volatile UDWORDbytes q,q1;
	if (flag_tcp_open==0) return;
    if ((flag_find_new_pak_tcp==0)&&(flag_timer_0==0)) return;

    if (flag_timer_0==1)
    {
       flag_timer_0=0;
       if (sequence_num_out.data!=acknow_num_in.data)           window_middle=window_left;
       return;
    }

    if (flag_work_timer_1==1)
    {
		if (acknow_num_in.data>=find_acknow_num_in)
		{
			IOWR_32DIRECT(TIMER_1_BASE, 16,0x00000000);
			q1.word0              = (unsigned short)IORD_32DIRECT(TIMER_1_BASE, 16);
			q1.word1              = (unsigned short)IORD_32DIRECT(TIMER_1_BASE, 20);
			q.data                = (ALT_CPU_FREQ/2-q1.data);
			old2_timer_repeat_pak = old1_timer_repeat_pak;
			old1_timer_repeat_pak = timer_repeat_pak.data;
			timer_repeat_pak.data = (q.data+timer_repeat_pak.data+old1_timer_repeat_pak+old2_timer_repeat_pak)>>2;
			IOWR_32DIRECT(TIMER_1_BASE, 4,0x00000000);
			flag_work_timer_1=0;
		}else
		{
			if (window_rigth<window_left)
			{
				if ((find_acknow_num_in>window_left)&&(acknow_num_in.data<window_rigth))
				{
					IOWR_32DIRECT(TIMER_1_BASE, 16,0x00000000);
					q1.word0              = (unsigned short)IORD_32DIRECT(TIMER_1_BASE, 16);
					q1.word1              = (unsigned short)IORD_32DIRECT(TIMER_1_BASE, 20);
	   			q.data                = (ALT_CPU_FREQ/2-q1.data);//<<1;
	   			old2_timer_repeat_pak = old1_timer_repeat_pak;
	   			old1_timer_repeat_pak = timer_repeat_pak.data;
	   			timer_repeat_pak.data = (q.data+timer_repeat_pak.data+old1_timer_repeat_pak+old2_timer_repeat_pak)>>2;
					IOWR_32DIRECT(TIMER_1_BASE, 4,0x00000000);
					flag_work_timer_1=0;
				}
			}
    	}
    }



    if (acknow_num_in_old!=acknow_num_in.data)
    {

        IOWR_32DIRECT(TIMER_0_BASE, 8,timer_repeat_pak.word0);
        IOWR_32DIRECT(TIMER_0_BASE, 12,timer_repeat_pak.word1);
        IOWR_32DIRECT(TIMER_0_BASE, 4,0x00000005);
    }

    if (acknow_num_in.data>=window_left)
    {
    	window_ram_tmp=(unsigned int)((acknow_num_in.data-window_left)>>10);
    }else
    {
    	window_ram_tmp=(unsigned int)((~window_left+1+acknow_num_in.data)>>10);
    }
    if (window_rigth_ram>=window_left_ram) {
    	if ((window_left_ram+window_ram_tmp)>window_rigth_ram) {
    		window_left_ram=window_left_ram+window_ram_tmp;
    		if (window_left_ram>=const_window_rigth_ram) window_left_ram=window_left_ram-const_window_rigth_ram;
    		window_rigth_ram=window_left_ram;
    	}
    	else{
    		window_left_ram=window_left_ram+window_ram_tmp;
    	}
    }else {
		window_left_ram=window_left_ram+window_ram_tmp;
		if (window_left_ram>=const_window_rigth_ram) {
			window_left_ram=window_left_ram-const_window_rigth_ram;
			if (window_rigth_ram<window_left_ram) 		window_rigth_ram=window_left_ram;
		}
    }
    if (window_rigth_ram==window_left_ram) flag_new_wr=1;

    if (window_left_ram>=const_window_rigth_ram) window_left_ram=window_left_ram-const_window_rigth_ram;

window_left=acknow_num_in.data;
    if (flag_end_stream!=0) {
    	window_rigth=window_left;//+window_size_in.data;
    }else    window_rigth=window_left+window_size_in.data;
    if (window_rigth>=window_left)
    {
        if (window_left>window_middle) {window_middle=window_left;}
    }else
    {
        if (window_middle>window_rigth)
        {
            if (window_left>window_middle) {window_middle=window_left;}
        }
    }
}

void tcp_pak_out(void)
{
volatile unsigned char i;
volatile unsigned int l;
//volatile unsigned long test_m;
 UDWORDbytes k;
	if (flag_tcp_open==0) return;
	do{
		if (window_rigth==window_left) {TCP_STREAM=3;break;}
		if (window_rigth>window_left)
		{
			if ((window_middle+length_data_out)<window_rigth)
			{
				TCP_STREAM=2;
				break;
			}
		}
		if (window_rigth<window_left)
		{
			if ((window_middle+length_data_out)<=0xffffffff)
			{
				TCP_STREAM=2;
				break;
			}else
			{
				if ((window_middle+length_data_out)<window_rigth)
				{
					TCP_STREAM=2;
					break;
				}
			}
		}
		TCP_STREAM=3;
		break;
	}while(1);

	switch (TCP_STREAM)
    {
    if (flag_tcp_open==0) break;

        case 0:
                if (flag_tcp_open==0) break;
                TCP_STREAM=1;
                break;
        case 1:
                if (window_rigth==window_left) {TCP_STREAM=3;break;}
                if (window_rigth>window_left)
                {
                    if ((window_middle+length_data_out)<window_rigth)
                    {
                        TCP_STREAM=2;
                        break;
                    }
                }
                if (window_rigth<window_left)
                {
                    if ((window_middle+length_data_out)<=0xffffffff)
                    {
                        TCP_STREAM=2;
                        break;
                    }else
                    {
                        if ((window_middle+length_data_out)<window_rigth)
                        {
                            TCP_STREAM=2;
                            break;
                        }
                    }
                }
                TCP_STREAM=3;
                break;
        case 2:
				k.data=IORD_32DIRECT(MAC_B_BASE, adr_reg_status);
				if ((k.data&ready_wr_out)==0) break;
			    if (window_middle>=window_left)	l=(unsigned int)((window_middle-window_left)>>10);
			    else l=(unsigned int)((~window_left+1+window_middle)>>10);

			    flag_wr_ram=0;

			    if (window_rigth_ram==window_left_ram) flag_wr_ram=1;
			    else {
			    	if (window_rigth_ram>window_left_ram)
			    	{
			    		{if ((window_rigth_ram-window_left_ram)<=l) flag_wr_ram=1;}
			    	}else
			    	{
			    		{if ((const_window_rigth_ram-window_left_ram+window_rigth_ram)<=l) flag_wr_ram=1;}
			    	}
			    }
				if ((flag_wr_ram!=0)&&((k.data&fifo_stream_ready)==0)) TCP_STREAM=3;
				else {
					if (flag_wr_ram!=0) window_rigth_ram=window_left_ram+l;
					else window_middle_ram=window_left_ram+l;
					if (window_rigth_ram>=const_window_rigth_ram) window_rigth_ram=window_rigth_ram-const_window_rigth_ram;
					if (window_middle_ram>=const_window_rigth_ram) window_middle_ram=window_middle_ram-const_window_rigth_ram;

					if (flag_new_pak_tcp_2==0){

						flag_new_pak_tcp_2=1;

						table_eth[0]=host_mac_l.byte0;
						table_eth[1]=host_mac_l.byte1;
						table_eth[2]=host_mac_l.byte2;
						table_eth[3]=host_mac_l.byte3;
						table_eth[4]=host_mac_h.byte0;
						table_eth[5]=host_mac_h.byte1;
						table_eth[12]=0x08;
						table_eth[13]=0x00;

						table_ip[2]=0x04;//0x02;
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

            sequence_num_out.data=window_middle;
						table_tcp[4]=sequence_num_out.byte3;
						table_tcp[5]=sequence_num_out.byte2;
						table_tcp[6]=sequence_num_out.byte1;
						table_tcp[7]=sequence_num_out.byte0;
						table_tcp[8]=acknow_num_out.byte3;
						table_tcp[9]=acknow_num_out.byte2;
						table_tcp[10]=acknow_num_out.byte1;
						table_tcp[11]=acknow_num_out.byte0;
						table_tcp[13]=0x18;
						table_tcp[16]=0;
						table_tcp[17]=0;

						sum_head_tcp.data=((unsigned long)main_ip.byte0<<8)+((unsigned long)main_ip.byte1)+((unsigned long)main_ip.byte2<<8)+((unsigned long)main_ip.byte3);
						sum_head_tcp.data=sum_head_tcp.data+((unsigned long)host_ip.byte0<<8)+((unsigned long)host_ip.byte1)+((unsigned long)host_ip.byte2<<8)+((unsigned long)host_ip.byte3);
						sum_head_tcp.data=sum_head_tcp.data+6+20+length_data_out;
						IOWR_32DIRECT(MAC_B_BASE,adr_reg_sum_head_tcp,sum_head_tcp.data);


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

					}else{

						sequence_num_out.data=window_middle;

						IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+16,table_ip[2]+(table_ip[3]<<8)+(ident_ip.byte1<<16)+(ident_ip.byte0<<24));
						IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+36,table_tcp[2]+(table_tcp[3]<<8)+(sequence_num_out.byte3<<16)+(sequence_num_out.byte2<<24));
						IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+40,sequence_num_out.byte1+(sequence_num_out.byte0<<8)+(acknow_num_out.byte3<<16)+(acknow_num_out.byte2<<24));
						IOWR_32DIRECT(MAC_B_BASE,adr_reg_tab_beg_pak_out+44,acknow_num_out.byte1+(acknow_num_out.byte0<<8)+(table_tcp[12]<<16)+(table_tcp[13]<<24));
					}
					acknow_num_out_old=acknow_num_out.data;

					IOWR_32DIRECT(MAC_B_BASE,adr_reg_r_b_pak_out,14*4);
					IOWR_32DIRECT(MAC_B_BASE, adr_reg_size_ip,14+20+20+length_data_out);
					IOWR_32DIRECT(MAC_B_BASE, adr_reg_num_byte_tcp,length_data_out);

          while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& fifo_tcp_wr_out_st)!=0) ;

					while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& (busy_wr_ram_data|busy_rd_ram_data))!=0) ;

					IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_wr_ram_data|mask_ena_rd_ram_data);

					if (flag_wr_ram!=0) {
						if (flag_work_timer_1==0)
						{
							flag_work_timer_1=1;
							IOWR_32DIRECT(TIMER_1_BASE, 8,0x00000000);
							IOWR_32DIRECT(TIMER_1_BASE, 4,0x00000005);
							find_acknow_num_in=sequence_num_out.data;
						}
						IOWR_32DIRECT(MAC_B_BASE,adr_reg_address_ram_data,window_rigth_ram);
						IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_wr_ram_data|ena_wr_ram_data|mask_ena_rd_ram_data);
						window_rigth_ram++;
						if (window_rigth_ram>=const_window_rigth_ram) window_rigth_ram=window_rigth_ram-const_window_rigth_ram;
					}
					else {
						IOWR_32DIRECT(MAC_B_BASE,adr_reg_address_ram_data,window_middle_ram);
						IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_rd_ram_data|ena_rd_ram_data|mask_ena_wr_ram_data);
					}
					IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_fifo_tcp_wr_out|fifo_tcp_wr_out|mask_ena_ip_crc|ena_ip_crc);
					IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_out|ena_out);
					while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& busy_r_b_pak_out)!=0) ;
					ident_ip.data++;
				    if (flag_work_timer_0==0)
				    {
			            IOWR_32DIRECT(TIMER_0_BASE, 8,timer_repeat_pak.word0);
				        IOWR_32DIRECT(TIMER_0_BASE, 12,timer_repeat_pak.word1);
				        flag_work_timer_0=1;
				        IOWR_32DIRECT(TIMER_0_BASE, 4,0x00000005);
				     }

					window_middle=window_middle+length_data_out;
					break;
				}
       case 3:
				if ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)&ready_wr_out)==0) break;

                if (acknow_num_out_old==acknow_num_out.data) break;
                acknow_num_out_old=acknow_num_out.data;
                sequence_num_out.data=window_middle;

                flag_new_pak_tcp_2=0;

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


				while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& (busy_wr_ram_data|busy_rd_ram_data))!=0) ;
				IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_wr_ram_data|mask_ena_rd_ram_data);

				IOWR_32DIRECT(MAC_B_BASE,adr_reg_r_b_pak_out,14*4);
                IOWR_32DIRECT(MAC_B_BASE, adr_reg_size_ip,60);
                IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_out|ena_out|mask_fifo_tcp_wr_out);
                while ((IORD_32DIRECT(MAC_B_BASE, adr_reg_status)& busy_r_b_pak_out)!=0) ;
                ident_ip.data++;

          //      TCP_STREAM=1;
                break;


       }

}
