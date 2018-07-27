#include "system.h"
#include <io.h>
#include "alt_types.h"
#include "sys/alt_irq.h"
#include "ext_var.h"
#include "ext_call.h"
#include "altera_vic_irq.h"
#include "altera_vic_regs.h"

volatile union long_bits flag1;
volatile int edge_capture;
volatile UDWORDbytes data_in[40];
volatile UWORDbytes port_tcp;
volatile UWORDbytes main_port_tcp_1;
volatile UWORDbytes main_port_tcp_2;
volatile UWORDbytes main_port_tcp;
volatile UWORDbytes host_port_tcp;
volatile unsigned char prior_b1,prior_b2,prior_b3;
volatile UDWORDbytes temp_reg_if;
volatile unsigned long temp_reg_ie;
volatile unsigned char rflag_in1_pak=0;
volatile unsigned char rflag_in2_pak=0;
volatile unsigned char rflag_in3_pak=0;
volatile unsigned long timer_main=0;
volatile unsigned char flag_find_pak_tcp_b=0;
volatile unsigned char flag_int=0;
volatile unsigned char ttt=0;
volatile unsigned char ttt1=0;
volatile unsigned char ttt2=0;
volatile unsigned char e1,e2,e3;
volatile unsigned char flag_timer_1=0;
volatile unsigned char flag_work_timer_1=0;

static void timer_0_interrupts(void* context)
{
    flag_timer_0=1;
    flag_work_timer_0=0;
    IOWR_32DIRECT(TIMER_0_BASE, 0,0);
    IOWR_32DIRECT(TIMER_0_BASE, 4,0);
    return;
}
static void timer_1_interrupts(void* context)
{
    flag_timer_1=1;
    flag_work_timer_1=0;
    IOWR_32DIRECT(TIMER_1_BASE, 0,0);
    IOWR_32DIRECT(TIMER_1_BASE, 4,0);
    return;
}

static void timer_2_interrupts(void* context)	//òàéìåð 10 ìñ
{
   IOWR_32DIRECT(TIMER_2_BASE, 0,0);
   if (timer_main!=0) timer_main--;
   if (timer_tcp_open!=0) timer_tcp_open--;
   return;
}

static void mac_interrupts(void* context)
{
    IOWR_32DIRECT(MAC_B_BASE, adr_reg_int_ie,mask_int_in_ie|mask_int_tcp_pak_ie);
    flag_int=1;
    temp_reg_if.data=IORD_32DIRECT(MAC_B_BASE,adr_reg_int_if);
    prior_b1=temp_reg_if.byte1;
    prior_b2=temp_reg_if.byte2;
    prior_b3=temp_reg_if.byte3;

    if (prior_b1>=prior_b2)
       {   if (prior_b1>=prior_b3) {
			   rflag_in1_pak=1;
			   if ((temp_reg_if.data & int_tcp_pak_in1_if)!=0) flag_find_pak_tcp_b=1;
			}
           else {
        	   rflag_in3_pak=1;
        	   if ((temp_reg_if.data&int_tcp_pak_in3_if)!=0) flag_find_pak_tcp_b=1;
        	   }
       }else
       {
    	   if (prior_b2>=prior_b3) {
    		   rflag_in2_pak=1;
    		   if ((temp_reg_if.data&int_tcp_pak_in2_if)!=0) flag_find_pak_tcp_b=1;
    		   }
           else  {
        	   rflag_in3_pak=1;
        	   if ((temp_reg_if.data&int_tcp_pak_in3_if)!=0) flag_find_pak_tcp_b=1;
        	   }
       }
}


static void init_int_mac()
{
    void* edge_capture_ptr = (void*) &edge_capture;
	alt_ic_isr_register( MAC_B_IRQ_INTERRUPT_CONTROLLER_ID,MAC_B_IRQ,mac_interrupts,edge_capture_ptr,0x0 );
}

static void init_int_timer_0()
{
	void* edge_capture_ptr_0 = (void*) &edge_capture;
	alt_ic_isr_register( TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID,TIMER_0_IRQ,timer_0_interrupts,edge_capture_ptr_0,0x0 );
}

static void init_int_timer_1()
{
	void* edge_capture_ptr_1 = (void*) &edge_capture;
	alt_ic_isr_register( TIMER_1_IRQ_INTERRUPT_CONTROLLER_ID,TIMER_1_IRQ,timer_1_interrupts,edge_capture_ptr_1,0x0 );
}

static void init_int_timer_2()
{
	void* edge_capture_ptr_2 = (void*) &edge_capture;
	alt_ic_isr_register( TIMER_2_IRQ_INTERRUPT_CONTROLLER_ID,TIMER_2_IRQ,timer_2_interrupts,edge_capture_ptr_2,0x0 );
}

int main (void) __attribute__ ((weak, alias ("alt_main")));

int alt_main (void)
{

	IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_eth_ready|mask_connect_ok|mask_connect_err);
	IOWR_32DIRECT(MAC_B_BASE, adr_reg_upr,mask_connect_port_1|mask_connect_port_2);

	IOWR_32DIRECT(MAC_B_BASE, adr_reg_int_ie,0xffff0000);
	ALTERA_VIC_IRQ_INSTANCE ( VIC_0, vic_0);
	alt_vic_irq_init(&vic_0);
    init_int_mac();
    init_int_timer_0();
    init_int_timer_1();
    init_int_timer_2();
    alt_irq_enable_all(NIOS2_STATUS_PIE_MSK);
    IOWR_32DIRECT(TIMER_2_BASE, 4,0x00000007);

    timer_main=400;
    while (timer_main!=0);

	ident_ip.data=1;
    flag1.data=0;
    timer_repeat_pak.data=10000;
    sequence_num_out.data=0x11111111;

    TCP_OPEN=0;

    do{
    	init();
    	arp_repl();
    	work_in_pak();
    	ping_repl();
    	tcp_open();
    	tcp_work_in_pak();
    	tcp_pak_out();
    }while(1);


  return 0;
}
