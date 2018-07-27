#include "system.h"
#include <io.h>
#include "alt_types.h"
#include "ext_var.h"
#include "ext_call.h"

volatile UDWORDbytes arp_req_ip;
volatile UDWORDbytes arp_req_mac_h;
volatile UDWORDbytes arp_req_mac_l;

volatile unsigned char tempp=0;
volatile unsigned long old_flag;

volatile UWORDbytes host_port_udp;
volatile UWORDbytes main_port_udp;
volatile UWORDbytes length_udp;

volatile unsigned long main_sequence_num_in;

volatile UDWORDbytes source_ip_ping;
volatile unsigned int ident_ping;
volatile unsigned int sequence_ping;
volatile UWORDbytes length_data_ping;
volatile unsigned char flag_in_ping;
volatile UDWORDbytes data_in_ping[20];

volatile unsigned char flag_tcp_port;

void work_in_pak_data(void)
{
    do{
      //  if (data_in[0].data!=0xffffffff) break;
      //  if (data_in[0].word0!=0xffff) break;
        if (data_in[3].data!=0x01000608) break;
        if (data_in[4].data!=0x04060008) break;
        if (data_in[5].word0!=0x0100) break;
        if (data_in[9].word1!=main_ip.word0) break;
        if (data_in[10].word0!=main_ip.word1) break;
        arp_req_mac_l.word0=data_in[5].word1;
        arp_req_mac_l.word1=data_in[6].word0;
        arp_req_mac_h.word0=data_in[6].word1;
        arp_req_ip.data=data_in[7].data;
        flag_find_arp_in=1;
        break;
    }while (1);
    do{
    	if (data_in[3].data!=0x00450008) break;
    	if (data_in[5].byte3!=0x01) break;
    	if (data_in[8].word1!=0x0008) break;
    	if (data_in[7].word1!=main_ip.word0) break;
    	if (data_in[8].word0!=main_ip.word1) break;
        ping_host_mac_h.data=(data_in[2].data>>16)&0x0000ffff;
        ping_host_mac_l.data=((data_in[1].data>>16)&0x0000ffff)|((data_in[2].data<<16)&0xffff0000);
        source_ip_ping.word1=data_in[7].word0;
    	source_ip_ping.word0=data_in[6].word1;
    	ident_ping=data_in[9].word1;
    	sequence_ping=data_in[10].word0;
    	length_data_ping.byte1=data_in[4].byte0;
    	length_data_ping.byte0=data_in[4].byte1;
    	length_data_ping.data=length_data_ping.data-28;
    	flag_in_ping=1;
    	break;
    }while (1);
    if (flag_find_arp_out==1)
    {
        do{
            if (data_in[3].data!=0x01000608) break;
            if (data_in[4].data!=0x04060008) break;
            if (data_in[5].word0!=0x0200) break;
            if (data_in[7].data!=host_ip.data) break;
            host_mac_h.data=(data_in[6].data>>16)&0x0000ffff;
            host_mac_l.data=((data_in[5].data>>16)&0x0000ffff)|((data_in[6].data<<16)&0xffff0000);
            flag_find_arp_out=0;
            flag_find_arp_out_ok=1;
            break;
        } while  (1);
    }

    return;
}


void work_in_pak(void)
{
	volatile unsigned short k,n;
	volatile unsigned long i;
	volatile unsigned short kk;

	 flag_find_new_pak_tcp=0;
	 if (flag_int==0) return;
	 flag_int=0;
	 if (flag_find_pak_tcp_b!=0){
	//	flag_find_pak_tcp=0;
		flag_find_new_pak_tcp=1;
		if (rflag_in1_pak!=0){
			flags_tcp.byte=IORD_32DIRECT(MAC_B_BASE,adr_reg_flag_tcp_b1);
			flag_tcp_port=(unsigned char)(IORD_32DIRECT(MAC_B_BASE,adr_reg_flag_tcp_b1)>>24);
			length_data_in=(unsigned short)(IORD_32DIRECT(MAC_B_BASE,adr_reg_length_data_in_b1));
			kk=0;
			kk=kk+length_data_in;
			sequence_num_in.data=IORD_32DIRECT(MAC_B_BASE,adr_reg_sequence_num_in_b1);
			if (length_data_in!=0)
			{
				if (main_sequence_num_in==sequence_num_in.data)
				{
					acknow_num_out.data=sequence_num_in.data+length_data_in;
					main_sequence_num_in=sequence_num_in.data+length_data_in;
	            	IOWR_32DIRECT(MAC_B_BASE, adr_reg_begin_adr_read_ram_in,13);
	            	IOWR_32DIRECT(MAC_B_BASE, adr_reg_length_data_fifo_out,length_data_in);
	            	IOWR_32DIRECT(MAC_B_BASE, adr_reg_begin_byte_data_out,2);
	            	IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_wr_fifo_out_ram_b1|ena_wr_fifo_out_ram_b1);
	            	while ((IORD_32DIRECT(MAC_B_BASE,adr_reg_status) & end_wr_fifo_out)==0);
	            	IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_wr_fifo_out_ram_b1);
				}
			}
			if ((flag_syn_tcp!=0)||(flag_fin_tcp!=0)||(flag_rst_tcp!=0))
			{
				sourse_port_in=(unsigned short)((IORD_32DIRECT(MAC_B_BASE,adr_reg_length_data_in_b1))>>16);
				host_ip_syn_in.data=IORD_32DIRECT(MAC_B_BASE,adr_reg_host_ip_syn_b1);
			}
			if (flag_ack_tcp!=0)
			{
				i=IORD_32DIRECT(MAC_B_BASE,adr_reg_acknow_num_in_b1);
				if (i>=acknow_num_in.data){
					i=i-acknow_num_in.data;
				}else{
					i=i+(~acknow_num_in.data)+1;
				}
				if ((i<65536)||(acknow_num_in_old==0)||(flag_tcp_open==0)){
					acknow_num_in_old=acknow_num_in.data;
					acknow_num_in.data=IORD_32DIRECT(MAC_B_BASE,adr_reg_acknow_num_in_b1);
					window_size_in.data=(unsigned int)(IORD_32DIRECT(MAC_B_BASE,adr_reg_window_size_in_b1));
				}
			}
			rflag_in1_pak=0;
			IOWR_32DIRECT(MAC_B_BASE, adr_reg_int_if,mask_int_tcp_pak_in1_if|mask_int_in1_if);
			flag_find_pak_tcp_b=0;
		    IOWR_32DIRECT(MAC_B_BASE, adr_reg_int_ie,mask_int_in_ie|int_in_ie|mask_int_tcp_pak_ie|int_tcp_pak_ie);
		    return;
		}
		if (rflag_in2_pak!=0){
			flags_tcp.byte=IORD_32DIRECT(MAC_B_BASE,adr_reg_flag_tcp_b2);
			flag_tcp_port=(unsigned char)(IORD_32DIRECT(MAC_B_BASE,adr_reg_flag_tcp_b2)>>24);
			length_data_in=(unsigned short)(IORD_32DIRECT(MAC_B_BASE,adr_reg_length_data_in_b2));
			sequence_num_in.data=IORD_32DIRECT(MAC_B_BASE,adr_reg_sequence_num_in_b2);
			if (length_data_in!=0)
			{
				if (main_sequence_num_in==sequence_num_in.data)
				{
					acknow_num_out.data=sequence_num_in.data+length_data_in;
					main_sequence_num_in=sequence_num_in.data+length_data_in;
	            	IOWR_32DIRECT(MAC_B_BASE, adr_reg_begin_adr_read_ram_in,13);
	            	IOWR_32DIRECT(MAC_B_BASE, adr_reg_length_data_fifo_out,length_data_in);
	            	IOWR_32DIRECT(MAC_B_BASE, adr_reg_begin_byte_data_out,2);
	            	IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_wr_fifo_out_ram_b2|ena_wr_fifo_out_ram_b2);
	            	while ((IORD_32DIRECT(MAC_B_BASE,adr_reg_status) & end_wr_fifo_out)==0);
	            	IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_wr_fifo_out_ram_b2);
				}
			}
			if ((flag_syn_tcp!=0)||(flag_fin_tcp!=0)||(flag_rst_tcp!=0))
			{
				sourse_port_in=(unsigned short)((IORD_32DIRECT(MAC_B_BASE,adr_reg_length_data_in_b2))>>16);
				host_ip_syn_in.data=IORD_32DIRECT(MAC_B_BASE,adr_reg_host_ip_syn_b2);
			}
			if (flag_ack_tcp!=0)
			{
				i=IORD_32DIRECT(MAC_B_BASE,adr_reg_acknow_num_in_b2);
				if (i>=acknow_num_in.data){
					i=i-acknow_num_in.data;
				}else{
					i=i+(~acknow_num_in.data)+1;
				}
				if ((i<65536)||(acknow_num_in_old==0)||(flag_tcp_open==0)){
					acknow_num_in_old=acknow_num_in.data;
					acknow_num_in.data=IORD_32DIRECT(MAC_B_BASE,adr_reg_acknow_num_in_b2);
					window_size_in.data=(unsigned int)(IORD_32DIRECT(MAC_B_BASE,adr_reg_window_size_in_b2));
				}
		//		acknow_num_in_old=acknow_num_in.data;
		//		acknow_num_in.data=IORD_32DIRECT(MAC_B_BASE,adr_reg_acknow_num_in_b2);
		//		window_size_in.data=(unsigned int)(IORD_32DIRECT(MAC_B_BASE,adr_reg_window_size_in_b2));
			}
	        rflag_in2_pak=0;
	        IOWR_32DIRECT(MAC_B_BASE, adr_reg_int_if,mask_int_tcp_pak_in2_if|mask_int_in2_if);
			flag_find_pak_tcp_b=0;
		    IOWR_32DIRECT(MAC_B_BASE, adr_reg_int_ie,mask_int_in_ie|int_in_ie|mask_int_tcp_pak_ie|int_tcp_pak_ie);
		    return;
		}
		if (rflag_in3_pak!=0){
			flags_tcp.byte=IORD_32DIRECT(MAC_B_BASE,adr_reg_flag_tcp_b3);
			flag_tcp_port=(unsigned char)(IORD_32DIRECT(MAC_B_BASE,adr_reg_flag_tcp_b3)>>24);
			length_data_in=(unsigned short)(IORD_32DIRECT(MAC_B_BASE,adr_reg_length_data_in_b3));
			sequence_num_in.data=IORD_32DIRECT(MAC_B_BASE,adr_reg_sequence_num_in_b3);
			if (length_data_in!=0)
			{
				if (main_sequence_num_in==sequence_num_in.data)
				{
					acknow_num_out.data=sequence_num_in.data+length_data_in;
					main_sequence_num_in=sequence_num_in.data+length_data_in;
	            	IOWR_32DIRECT(MAC_B_BASE, adr_reg_begin_adr_read_ram_in,13);
	            	IOWR_32DIRECT(MAC_B_BASE, adr_reg_length_data_fifo_out,length_data_in);
	            	IOWR_32DIRECT(MAC_B_BASE, adr_reg_begin_byte_data_out,2);
	            	IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_wr_fifo_out_ram_b3|ena_wr_fifo_out_ram_b3);
	            	while ((IORD_32DIRECT(MAC_B_BASE,adr_reg_status) & end_wr_fifo_out)==0);
	            	IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_ena_wr_fifo_out_ram_b3);
				}
			}
			if ((flag_syn_tcp!=0)||(flag_fin_tcp!=0)||(flag_rst_tcp!=0))
			{
				sourse_port_in=(unsigned short)((IORD_32DIRECT(MAC_B_BASE,adr_reg_length_data_in_b3))>>16);
				host_ip_syn_in.data=IORD_32DIRECT(MAC_B_BASE,adr_reg_host_ip_syn_b3);
			}
			if (flag_ack_tcp!=0)
			{
				i=IORD_32DIRECT(MAC_B_BASE,adr_reg_acknow_num_in_b3);
				if (i>=acknow_num_in.data){
					i=i-acknow_num_in.data;
				}else{
					i=i+(~acknow_num_in.data)+1;
				}
				if ((i<65536)||(acknow_num_in_old==0)||(flag_tcp_open==0)){
					acknow_num_in_old=acknow_num_in.data;
					acknow_num_in.data=IORD_32DIRECT(MAC_B_BASE,adr_reg_acknow_num_in_b3);
					window_size_in.data=(unsigned int)(IORD_32DIRECT(MAC_B_BASE,adr_reg_window_size_in_b3));
				}
		//		acknow_num_in_old=acknow_num_in.data;
		//		acknow_num_in.data=IORD_32DIRECT(MAC_B_BASE,adr_reg_acknow_num_in_b3);
		//		window_size_in.data=(unsigned int)(IORD_32DIRECT(MAC_B_BASE,adr_reg_window_size_in_b3));
			}
	        rflag_in3_pak=0;
	        IOWR_32DIRECT(MAC_B_BASE, adr_reg_int_if,mask_int_tcp_pak_in3_if|mask_int_in3_if);
			flag_find_pak_tcp_b=0;
		    IOWR_32DIRECT(MAC_B_BASE, adr_reg_int_ie,mask_int_in_ie|int_in_ie|mask_int_tcp_pak_ie|int_tcp_pak_ie);
	        return;
		}
	    return;
	}
    if (rflag_in1_pak!=0){
        for (i=adr_reg_tab_in1,k=0;i<(adr_reg_tab_in1+70);i=i+4,k++) data_in[k].data=IORD_32DIRECT(MAC_B_BASE,i);
        rflag_in1_pak=0;
        work_in_pak_data();
        if (flag_in_ping!=0)
        {
        	i=adr_reg_tab_in1+32;
        	k=0;
        	n=length_data_ping.data+8;
        	data_in_ping[k].data=IORD_32DIRECT(MAC_B_BASE,i);
        	k++;i=i+4;n=n-2;
        	while (n>=4)
        	{
        		data_in_ping[k].data=IORD_32DIRECT(MAC_B_BASE,i);
        		k++;i=i+4;n=n-4;
        	}
        	if (n!=0) data_in_ping[k].data=IORD_32DIRECT(MAC_B_BASE,i);
        }
        IOWR_32DIRECT(MAC_B_BASE, adr_reg_int_if,mask_int_in1_if|mask_int_tcp_pak_in1_if);
        IOWR_32DIRECT(MAC_B_BASE, adr_reg_int_ie,mask_int_in_ie|int_in_ie|mask_int_tcp_pak_ie|int_tcp_pak_ie);
        return;
    }
    if (rflag_in2_pak!=0){
        for (i=adr_reg_tab_in2,k=0;i<(adr_reg_tab_in2+70);i=i+4,k++) data_in[k].data=IORD_32DIRECT(MAC_B_BASE,i);
        rflag_in2_pak=0;
        work_in_pak_data();
        if (flag_in_ping!=0)
        {
        	i=adr_reg_tab_in2+32;
        	k=0;
        	n=length_data_ping.data+8;
        	data_in_ping[k].data=IORD_32DIRECT(MAC_B_BASE,i);
        	k++;i=i+4;n=n-2;
        	while (n>=4)
        	{
        		data_in_ping[k].data=IORD_32DIRECT(MAC_B_BASE,i);
        		k++;i=i+4;n=n-4;
        	}
        	if (n!=0) data_in_ping[k].data=IORD_32DIRECT(MAC_B_BASE,i);
        }
        IOWR_32DIRECT(MAC_B_BASE, adr_reg_int_if,mask_int_in2_if|mask_int_tcp_pak_in2_if);
        IOWR_32DIRECT(MAC_B_BASE, adr_reg_int_ie,mask_int_in_ie|int_in_ie|mask_int_tcp_pak_ie|int_tcp_pak_ie);
        return;
    }
    if (rflag_in3_pak!=0){
        for (i=adr_reg_tab_in3,k=0;i<(adr_reg_tab_in3+70);i=i+4,k++) data_in[k].data=IORD_32DIRECT(MAC_B_BASE,i);
        rflag_in3_pak=0;
        work_in_pak_data();
        if (flag_in_ping!=0)
        {
        	i=adr_reg_tab_in3+32;
        	k=0;
        	n=length_data_ping.data+8;
        	data_in_ping[k].data=IORD_32DIRECT(MAC_B_BASE,i);
        	k++;i=i+4;n=n-2;
        	while (n>=4)
        	{
        		data_in_ping[k].data=IORD_32DIRECT(MAC_B_BASE,i);
        		k++;i=i+4;n=n-4;
        	}
        	if (n!=0) data_in_ping[k].data=IORD_32DIRECT(MAC_B_BASE,i);
        }
        IOWR_32DIRECT(MAC_B_BASE, adr_reg_int_if,mask_int_in3_if|mask_int_tcp_pak_in3_if);
        IOWR_32DIRECT(MAC_B_BASE, adr_reg_int_ie,mask_int_in_ie|int_in_ie|mask_int_tcp_pak_ie|int_tcp_pak_ie);
        return;
    }
}

