/* V35 additive stream. No raw structure padding, no rerolled statistics. */
static int db_words(FILE *f,void *data,int words,int reading){unsigned char *p=data;for(int i=0;i<words;i++){uint32_t u;if(reading){if(!load_u32(f,&u))return 0;memcpy(p+i*4,&u,4);}else{memcpy(&u,p+i*4,4);if(!save_u32(f,u))return 0;}}return 1;}
static int dockbay_stream(FILE *f,DockBay *d,int reading){
 if(!db_words(f,&d->ready,8,reading))return 0;
 for(int i=0;i<DB_SELLERS;i++)if(!db_words(f,&d->seller[i],7,reading))return 0;
 for(int i=0;i<DB_ADS;i++)if(!db_words(f,&d->ad[i],DB_AD_WORDS,reading))return 0;
 for(int i=0;i<DB_ITEMS;i++)if(!db_words(f,&d->owned[i],8,reading))return 0;
 if(!db_words(f,d->fitted,48,reading)||!db_words(f,d->fleet,5*48,reading)||!db_words(f,&d->px,10,reading))return 0;
 if(!db_words(f,&d->journal_n,2,reading))return 0;
 for(int i=0;i<DB_RECEIPTS;i++)if(!db_words(f,&d->journal[i],DB_AD_WORDS,reading))return 0;
 return 1;
}
static int db_roll_valid(const DockRoll *r){if(cosmetic_item(r->base)&&(r->quality!=DB_NORMAL||r->power!=1000||r->secondary!=1000))return 0;return r->id&&equip_slot_for(r->base)>=0&&r->quality<=DB_RARE&&r->generation==1&&r->power>=400&&r->power<=2000&&r->secondary>=400&&r->secondary<=2000&&r->value<=10000000&&(r->quality!=DB_NORMAL||(r->power==1000&&r->secondary==1000))&&(r->quality!=DB_WORN||(r->power<=1000&&r->secondary<=1000))&&(r->quality<DB_MODDED||(r->power>=1000&&r->secondary>=1000))&&(!fit_is_active_weapon(r->base)||r->power*r->secondary<=2000000);}
static int db_ad_valid(const DockAd *a){
 if(!a->id||a->id>=0x40000000u||a->seller>=DB_SELLERS||a->system>255||a->site>5||a->kind>DB_BUY_CARGO||a->state>DB_REPORTED||a->flags>4095||a->outcome>3||a->encounter>=20||a->stock>a->original||!a->original||a->original>100000||a->price>100000000u/a->original||a->price>10000000||a->receipt_paid>100000000||a->receipt_qty>a->original||a->receipt_seen>1||a->visits>7)return 0;
 if(a->kind==DB_MODULE?(!db_roll_valid(&a->roll)||a->roll.base!=a->item||a->stock>1):a->item>=GOODS)return 0;
 if(!isfinite(a->x)||!isfinite(a->y)||!isfinite(a->z)||fabsf(a->x)>1.e8f||fabsf(a->y)>1.e8f||fabsf(a->z)>1.e8f)return 0;
 for(int k=0;k<3;k++)if(!isfinite(a->actor_health[k])||!isfinite(a->actor_shield[k])||a->actor_health[k]<0||a->actor_health[k]>1000||a->actor_shield[k]<0||a->actor_shield[k]>1000)return 0;return 1;
}
static int dockbay_valid(const DockBay *d){
 if(d->ready>1||d->route>DB_ADS||d->receipt_ad>DB_ADS||d->resume>1||d->resume_system>255||d->dirty>1)return 0;
 for(int i=0;i<DB_SELLERS;i++){const DockSeller *s=&d->seller[i];if(s->trusted>1||s->blocked>1||s->trusted&&(s->meetings<5||s->blocked))return 0;}
 if(d->journal_n>DB_RECEIPTS||d->journal_next>=DB_RECEIPTS)return 0;
 for(int i=0;i<DB_ADS;i++)if(d->ready){if(!db_ad_valid(&d->ad[i]))return 0;for(int j=0;j<i;j++)if(d->ad[i].id==d->ad[j].id)return 0;}
 for(int i=0;i<DB_ITEMS;i++){const DockRoll *r=&d->owned[i];if(r->id&&!db_roll_valid(r))return 0;for(int j=0;r->id&&j<i;j++)if(r->id==d->owned[j].id)return 0;}
 for(int i=0;i<(int)d->journal_n;i++)if(!db_ad_valid(&d->journal[i])||!(d->journal[i].flags&DB_RECEIPT))return 0;
 for(int i=0;i<48;i++)if(d->fitted[i]>DB_ITEMS||d->fitted[i]&&!d->owned[d->fitted[i]-1].id)return 0;
 for(int k=0;k<5;k++)for(int i=0;i<48;i++)if(d->fleet[k][i]>DB_ITEMS||d->fleet[k][i]&&!d->owned[d->fleet[k][i]-1].id)return 0;
 const float *v=&d->px;for(int i=0;i<10;i++)if(!isfinite(v[i])||fabsf(v[i])>1.e8f)return 0;
 return 1;
}
/* Ordinary legacy equipment becomes a normal instance, retaining catalogue resale. */
static void dockbay_migrate(Game *g){
 DockBay *d=&g->dockbay;for(int k=-1;k<g->erl.fleet_n;k++){if(k==g->erl.active_ship)continue;uint8_t *fit=k<0?g->fit:g->erl.fleet[k].fit;uint32_t *refs=k<0?d->fitted:d->fleet[k];for(int s=0;s<48;s++)if(fit[s]!=FIT_EMPTY&&!refs[s]){int i=db_owned_free(g);if(i<0)return;d->owned[i]=(DockRoll){++d->serial,fit[s],DB_NORMAL,1,0,1000,1000,0};refs[s]=i+1;}}
 for(int b=1;b<EQUIPMENT_COUNT;b++)if(equip_slot_for(b)>=0)while(g->module_store[b]){int i=db_owned_free(g);if(i<0)return;d->owned[i]=(DockRoll){++d->serial,b,DB_NORMAL,1,0,1000,1000,0};g->module_store[b]--;}
}
static int dockbay_fit_valid(const DockBay *d,const uint8_t *fit,const ErlCareer *e){
 unsigned char used[DB_ITEMS]={0};for(int k=-1;k<e->fleet_n;k++){if(k==e->active_ship)continue;const uint32_t *refs=k<0?d->fitted:d->fleet[k];const uint8_t *bank=k<0?fit:e->fleet[k].fit;unsigned char bases[EQUIPMENT_COUNT]={0};int charms=0,dashboards=0;
 for(int s=0;s<48;s++){unsigned r=refs[s];if(r){if(r>DB_ITEMS||used[r-1]||!d->owned[r-1].id||d->owned[r-1].base!=bank[s])return 0;used[r-1]=1;}if(bank[s]!=FIT_EMPTY){if(charm_item(bank[s])&&++charms>1)return 0;if(dashboard_item(bank[s])&&++dashboards>1)return 0;if(bank[s]>=EQUIPMENT_COUNT)return 0;if(bases[bank[s]]&&(!r||!refs[bases[bank[s]]-1]))return 0;bases[bank[s]]=s+1;}}
 }return 1;
}
static void dockbay_checkpoint(Game *g){DockBay *d=&g->dockbay;d->resume=d->ready&&!g->docked&&g->planet<0&&!g->dock_stage&&!g->jump;d->resume_system=g->system;d->px=g->pos.x;d->py=g->pos.y;d->pz=g->pos.z;d->yaw=g->yaw;d->pitch=g->pitch;d->roll=g->roll;d->speed=g->speed;d->hull=g->hull;d->shield=g->energy;d->heat=g->heat;
 for(int n=0;n<NPC_COUNT;n++){NPC *actor=&g->npc[n];if(!actor->dockbay_id||actor->dockbay_id&0x40000000u||actor->route_leg>2)continue;for(int i=0;i<DB_ADS;i++)if(d->ad[i].id==(actor->dockbay_id&0x3fffffffu)){int k=actor->route_leg;d->ad[i].actor_health[k]=fmaxf(0,actor->health);d->ad[i].actor_shield[k]=fmaxf(0,actor->shield);if(!actor->alive)d->ad[i].visits|=1u<<k;break;}}
}
static void dockbay_resume(Game *g){DockBay *d=&g->dockbay;g->dockbay_npc=-1;g->dockbay_live_system=-1;if(!d->resume||d->resume_system!=(unsigned)g->system||g->erl.event_kind)return;g->docked=0;g->dock_stage=0;g->planet=g->approach=-1;g->surface=0;g->pos=(Vec3){d->px,d->py,d->pz};g->yaw=d->yaw;g->pitch=d->pitch;g->roll=d->roll;g->speed=0;g->hull=d->hull;g->energy=d->shield;g->heat=d->heat;g->boost=0;g->police_grace=5;dockbay_tick(g,0);}
