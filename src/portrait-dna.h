#ifndef ELITE_PORTRAIT_DNA_H
#define ELITE_PORTRAIT_DNA_H
static inline int portrait_dna_valid(unsigned d){
 if(d<8u)return 1;if(!(d&0x80000000u)||(d&15u)>=10u)return 0;
 unsigned version=(d>>26)&3u;
 /* The previously reserved version-zero bit 30 tags the expanded encoding.
  * Existing four-byte save records and all earlier encodings stay valid. */
 if(version==0u&&(d&0x40000000u))return ((d>>5)&15u)<12u&&((d>>9)&15u)<12u&&((d>>13)&7u)<5u&&((d>>16)&7u)<6u&&((d>>19)&3u)<3u&&((d>>21)&3u)<3u&&((d>>23)&3u)<3u;
 if(version==3u)return !(d&0x03000000u)&&((d>>4)&3u)<2u&&(((d>>6)&7u)|(((d>>19)&1u)<<3))<12u&&(((d>>9)&7u)|(((d>>20)&1u)<<3))<12u&&((d>>12)&3u)<3u&&(((d>>14)&3u)|(((d>>21)&1u)<<2))<6u&&((d>>16)&7u)<3u&&((d>>22)&3u)<3u&&((d>>28)&7u)<3u;
 return (version==2u&&!(d&0x03380030u)&&((d>>12)&3u)<2u&&((d>>22)&3u)<2u&&((d>>16)&7u)<2u&&((d>>28)&7u)<2u)||version==1u||(version==0u&&!(d&0x73000000u));
}
#endif
