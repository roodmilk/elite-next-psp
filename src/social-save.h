/* V23: one global history. V22 is streamed into it without a second large buffer. */
static int social_valid_post(unsigned p,unsigned stamp){
 unsigned kind=p&31,ship=(p>>5)&15;
 return kind>0&&kind<SB_COUNT&&ship<(unsigned)player_ship_count&&(p>>30)!=3&&stamp;
}
static int social_write(FILE *f,const SocialState *s){
 int ok=save_u32(f,s->clock)&&save_u32(f,s->serial)&&save_u32(f,s->count)&&save_u32(f,s->evidence_reaction);
 for(int sys=0;ok&&sys<256;sys++)ok=save_u32(f,s->seen[sys]);
 for(int i=0;ok&&i<SOCIAL_KEEP;i++)ok=save_u32(f,s->post[i])&&save_u32(f,s->stamp[i])&&save_u32(f,s->origin[i]);
 return ok;
}
static int social_read(FILE *f,SocialState *s,int version){
 int ok=load_u32(f,&s->clock)&&load_u32(f,&s->serial);
 if(s->clock>0xfffffff0u)ok=0;
 if(version==22){
  for(int sys=0;ok&&sys<256;sys++){
   uint32_t reactions=0;
   ok=load_u32(f,&s->seen[sys])&&load_u32(f,&reactions);
   if(reactions&~0x3fffu)ok=0;
   for(int r=0;r<7;r++)if(((reactions>>(r*2))&3)==3)ok=0;
   if(sys==7)s->evidence_reaction=reactions&3;
   int empty=0;
   for(int i=0;ok&&i<16;i++){
    uint32_t p=0,stamp=0;ok=load_u32(f,&p)&&load_u32(f,&stamp);
    if(!p){empty=1;if(stamp)ok=0;continue;}
    if(empty||!social_valid_post(p,stamp)){ok=0;break;}
    /* Calendar-dated records first; undated legacy records follow in session order.
     * Equal timestamps retain deterministic system/within-system order. */
    int at=0;
    while(at<(int)s->count){
     unsigned other=s->stamp[at];
     if((stamp>>31)<(other>>31)||((stamp>>31)==(other>>31)&&stamp>other))break;
     at++;
    }
    social_push(s,p,stamp,(unsigned)sys|256u,at);
   }
  }
  return ok;
 }
 if(ok)ok=load_u32(f,&s->count)&&load_u32(f,&s->evidence_reaction);
 if(s->count>SOCIAL_KEEP||s->evidence_reaction>2)ok=0;
 for(int sys=0;ok&&sys<256;sys++)ok=load_u32(f,&s->seen[sys]);
 for(int i=0;ok&&i<SOCIAL_KEEP;i++){
  ok=load_u32(f,&s->post[i])&&load_u32(f,&s->stamp[i])&&load_u32(f,&s->origin[i]);
  if((unsigned)i<s->count){if(!social_valid_post(s->post[i],s->stamp[i])||s->origin[i]>511)ok=0;}
  else if(s->post[i]||s->stamp[i]||s->origin[i])ok=0;
 }
 return ok;
}

