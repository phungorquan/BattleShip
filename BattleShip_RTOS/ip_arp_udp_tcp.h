// I comment some function don't use to save MEMORY


unsigned int checksum(unsigned char* buf, unsigned  int len, unsigned char type);
// you must call this function once before you use any of the other functions:
void init_ip_arp_udp_tcp(unsigned char* mymac, unsigned char* myip);//,
   //unsigned char wwwp);
void fill_ip_hdr_checksum(unsigned char* buf);
unsigned char eth_type_is_arp_and_my_ip(unsigned char* buf,unsigned int len);
unsigned char eth_type_is_ip_and_my_ip(unsigned char* buf,unsigned int len);
void make_arp_answer_from_request(unsigned char* buf);
void make_echo_reply_from_request(unsigned char* buf, unsigned int len);
//extern void make_udp_reply_from_request(unsigned char* buf, char* data,unsigned char datalen, unsigned int port);

//extern void make_tcp_synack_from_syn(unsigned char* buf);
//extern void init_len_info(unsigned char* buf);
//extern unsigned int get_tcp_data_pointer(void);
//extern unsigned int fill_tcp_data_p(unsigned char* buf, unsigned int pos,const unsigned char* progmem_s);
//extern unsigned int fill_tcp_data(unsigned char* buf, unsigned int pos,const char* s);
//extern void make_tcp_ack_from_any(unsigned char* buf);
//extern void make_tcp_ack_with_data(unsigned char* buf, unsigned int dlen);
void send_udp_transmit(unsigned char *buf,unsigned char datalen);
void send_udp(unsigned char *buf,char *data,unsigned char datalen,unsigned char sport, const unsigned char *dip, unsigned char dport,const unsigned char *dstmac);
void send_udp_prepare(unsigned char *buf,unsigned int sport, const unsigned char *dip, unsigned int dport,const unsigned char *dstmac);


