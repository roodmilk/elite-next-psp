/* V31 extension; explicit little-endian records included in commander CRC. */
static int rumour_write(FILE *f,const RumourState *r){
 int ok=save_u32(f,1)&&save_u32(f,r->active);
 for(int s=0;ok&&s<256;s++){
  ok=save_u32(f,r->owned[s])&&save_u32(f,r->found[s])&&save_u32(f,r->refunded[s]);
  for(int k=0;ok&&k<RUMOUR_SLOTS;k++)ok=save_u32(f,r->paid[s][k]);
 }return ok;
}
static int rumour_read(FILE *f,RumourState *r){
 uint32_t v=0;memset(r,0,sizeof(*r));
 if(!load_u32(f,&v)||v!=1||!load_u32(f,&r->active)||r->active>256*RUMOUR_SLOTS)return 0;
 for(int s=0;s<256;s++){
  if(!load_u32(f,&v)||(v&~RUMOUR_MASK))return 0;r->owned[s]=v;
  if(!load_u32(f,&v)||(v&~RUMOUR_MASK))return 0;r->found[s]=v;
  if(!load_u32(f,&v)||(v&~RUMOUR_MASK))return 0;r->refunded[s]=v;
  if(r->refunded[s]&~r->owned[s])return 0;
  for(int k=0;k<RUMOUR_SLOTS;k++){
   if(!load_u32(f,&v)||v>60000||(!(r->owned[s]&(1u<<k))&&v))return 0;r->paid[s][k]=v;
  }
 }
 if(r->active){int s=(r->active-1)/RUMOUR_SLOTS,k=(r->active-1)%RUMOUR_SLOTS;
  if(!(r->owned[s]&(1u<<k))&&!(r->found[s]&(1u<<k)))return 0;
  if(r->refunded[s]&(1u<<k))return 0;
 }return 1;
}
