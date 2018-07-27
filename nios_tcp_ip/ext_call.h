#ifndef EXT_CALL_H_
#define EXT_CALL_H_

extern void init(void);
extern void tcp_open ();
extern void ch_summ_ip (unsigned char* );
extern void tcp_pak_out(void);
extern void work_in_pak(void);
extern void tcp_work_in_pak(void);
extern void arp_repl(void);
extern void work_in_pak_bootp(void);
extern void ping_repl(void);
#endif
