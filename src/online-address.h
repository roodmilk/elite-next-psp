/* Configuration is read before PSP_NET_MODULE_INET is loaded. Never call
 * sceNetInetInetAddr/inet_addr here: an unlinked import returns 0x8002013A,
 * which would otherwise become destination 58.1.2.128 on real hardware. */
#ifndef ELITE_ONLINE_ADDRESS_H
#define ELITE_ONLINE_ADDRESS_H
#include <stdint.h>
#include <string.h>
static int online_ipv4_parse(const char *text,uint32_t *address){
 unsigned char octets[4];
 if(!text||!address)return 0;
 for(int i=0;i<4;i++){
  unsigned value=0,digits=0;
  while(*text>='0'&&*text<='9'){
   value=value*10u+(unsigned)(*text++-'0');
   if(++digits>3||value>255)return 0;
  }
  if(!digits)return 0;octets[i]=(unsigned char)value;
  if(i<3){if(*text++!='.')return 0;}else if(*text)return 0;
 }
 /* Copy wire-order bytes directly; independent of CPU endianness and of
  * networking-module initialization. Invalid input leaves output untouched. */
 memcpy(address,octets,sizeof(octets));return 1;
}
#endif
