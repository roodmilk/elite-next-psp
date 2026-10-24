/* DockBay domain: presentation never transfers goods or invents a seller. */
static uint32_t db_hash(uint32_t x){x^=x>>16;x*=0x7feb352du;x^=x>>15;x*=0x846ca68bu;return x^(x>>16);}
static const char *db_names[DB_SELLERS]={"MOSS_77","AUNTIE_JET","BOLT_DAD","RHEA VOSS","FINCH & CO","ADA QUILL","MILO WREN","NORI VALE","PIP COPPER","TESS REED","ORIN BELL","LUMA GREY","KIP MARLOW","SABLE FINN","JUNO PAX","RIN ASH","VEL MERCER","DAX FROST","BEA NORTH","SOL PARK","DOT LENNOX","NIX STARR","IVY KEEN","ARGO LYNN"};
const char *dockbay_seller_name(int i){return i>=0&&i<DB_SELLERS?db_names[i]:"NO SELLER";}
static const int db_encounter_bases[20]={24,10,2,TECH_ADVANCED_HEAT_BUFFER,6,0,12,TECH_EFFICIENT_BOOSTER,9,TECH_BOOST_INJECTORS,38,6,1,32,27,2,24,10,TECH_EFFICIENT_HYPERDRIVE,38};
static int db_module_price(int base){const TechModule *m=tech_module(base);if(m)return m->price_u*10;return base==24?6500:base==2?4000:base==1?2200:base==10?3500:base==38?11500:base==6?6000:base==27?24000:base==32?7500:12000;}
void dockbay_roll_generate(DockRoll *r,unsigned seed,int base,int quality,int distant){
 r->base=base;r->quality=quality;r->generation=1;r->seed=seed;
 r->power=quality==DB_WORN?550+seed%301:quality==DB_NORMAL?1000:quality==DB_MODDED?1100+seed%401:1700+seed%301;
 r->secondary=1000;if((seed&3)==0&&(fit_is_active_weapon(base)||base==TECH_EFFICIENT_BOOSTER))r->secondary=quality==DB_WORN?800+seed%151:quality>=DB_MODDED?1100+(seed>>8)%201:1000;
 if(fit_is_active_weapon(base)&&r->power*r->secondary>2000000)r->power=2000000/r->secondary;
 int price=db_module_price(base);float factor=quality==DB_WORN?.35f:quality==DB_NORMAL?.75f:quality==DB_MODDED?1.7f:3.8f;
 if(distant)factor=quality==DB_RARE?4.f:.28f;else if(seed%9==0)factor*=.42f;
 r->value=(unsigned)fmaxf(10,price*factor);
}
int dockbay_storage_capacity(const Game *g){int n=0;for(int c=0;c<6;c++)n+=fit_capacity(g->ship,c);n*=2;return n<12?12:n>40?40:n;}
int dockbay_storage_used(const Game *g){int n=db_stored_count(g);for(int i=1;i<EQUIPMENT_COUNT;i++)n+=g->module_store[i];return n;}
int dockbay_cargo_free(const Game *g,int item){return cargo_capacity_for(g,item)-cargo_used(g);}
/* Roll once, then retain the price, coordinates and outcome through every visit. */
static void db_new_ad(Game *g,DockAd *a,int seller,int encounter,int system,int site){
 DockBay *d=&g->dockbay;memset(a,0,sizeof(*a));a->id=++d->serial;a->seller=seller;a->system=system;a->site=site;a->encounter=encounter;
 if(encounter==1)a->site=0;else if(encounter==6)a->site=2;else if(encounter==17)a->site=3;
 uint32_t h=db_hash(a->id*731u+d->seller[seller].seed);a->kind=encounter==5||encounter==6?DB_CARGO:encounter==8?DB_BUY_CARGO:DB_MODULE;a->item=db_encounter_bases[encounter];a->stock=a->kind==DB_MODULE?1:80+h%321;a->original=a->stock;a->revision=1;a->roll.id=++d->serial;
 if(a->site==4){int rings=0;for(int b=1;b<system_body_slots(system);b++)rings|=system_body_type_slot(system,b)==GAS;if(!rings)a->site=0;}
 int distant=distance_ly(g,g->system,system)>18.f,quality=encounter==13||encounter==7?DB_WORN:encounter==3||encounter==14||(distant&&h%8==0)?DB_RARE:distant||encounter==9?DB_MODDED:h%4==0?DB_MODDED:DB_NORMAL;
 dockbay_roll_generate(&a->roll,h,a->item,quality,distant);a->price=a->kind==DB_MODULE?a->roll.value:(unsigned)fmaxf(4,galactic_price(a->item)*(a->kind==DB_BUY_CARGO?(distant?2.8f:1.3f):(distant?.18f:.65f)));
 /* Encounter flavour and appearance do not predict danger. Trusted pilots never betray. */
 unsigned chance=db_hash(h^0xfacadeu)%100;a->outcome=chance<4?1:chance<8?2:chance<11?3:0;
 if(d->seller[seller].trusted&&a->outcome!=3)a->outcome=0;
 for(int k=0;k<3;k++){a->actor_health[k]=k?90:140;a->actor_shield[k]=k?40:80;}
}
void dockbay_init(Game *g){
 DockBay *d=&g->dockbay;if(d->ready)return;d->ready=1;d->serial=1000;g->dockbay_npc=-1;g->dockbay_live_system=-1;
 for(int i=0;i<DB_SELLERS;i++){DockSeller *s=&d->seller[i];s->id=i+1;s->seed=db_hash(0xDB398u+i*97);s->portrait=s->seed;}
 for(int i=0;i<DB_ADS;i++){
  uint32_t h=db_hash(391u+i*731u);db_new_ad(g,&d->ad[i],i%DB_SELLERS,i==1?5:i==2?7:i==3?2:i%20,i<4?g->system:i<12?(g->system+i*3)%256:(h>>8)&255,i%6);
 }
 /* A travelling shop's second advert is real shared stock at the same rendezvous. */
 d->ad[34].system=d->ad[10].system;d->ad[34].site=d->ad[10].site;
 d->dirty=1;
}
static int db_danger(const Game *g,const DockAd *a){return a->outcome&&!(a->flags&DB_DANGER_DONE)&&!(a->outcome<3&&g->dockbay.seller[a->seller].trusted);}
static void db_count_meeting(Game *g,DockAd *a){
 if(a->stock||a->flags&DB_COUNTED||a->outcome==1||a->outcome==2||db_danger(g,a))return;
 DockSeller *s=&g->dockbay.seller[a->seller];a->flags|=DB_COUNTED;if(!a->meeting)a->meeting=a->id;if(s->counted_meeting==a->meeting)return;s->counted_meeting=a->meeting;s->meetings++;
 if(s->meetings>=5){s->trusted=1;s->blocked=0;for(int i=0;i<DB_ADS;i++)if(g->dockbay.ad[i].seller==a->seller&&g->dockbay.ad[i].outcome<3)g->dockbay.ad[i].outcome=0;}
 g->dockbay.dirty=1;
}
static void db_archive(Game *g,const DockAd *a){DockBay *d=&g->dockbay;d->journal[d->journal_next]=*a;d->journal_next=(d->journal_next+1)%DB_RECEIPTS;if(d->journal_n<DB_RECEIPTS)d->journal_n++;}
/* Only an explicitly finished transaction can create a new listing. Unpaid lots survive. */
void dockbay_restock(Game *g){
 DockBay *d=&g->dockbay;if(!d->ready)return;
 for(int i=0;i<DB_ADS;i++){DockAd *a=&d->ad[i];if(a->state!=DB_COMPLETE||!a->receipt_seen||db_danger(g,a))continue;
  int present=0;for(int n=0;n<NPC_COUNT;n++)if(g->npc[n].alive&&g->npc[n].dockbay_id==a->id)present=1;if(present)continue;
  int seller=a->seller,site=a->site,sys=a->system;unsigned old=a->id;db_new_ad(g,a,seller,19,sys,site);
  /* Regulars bring a different useful supported family, with a new permanent roll. */
  static const int stock[]={24,10,6,38,1,TECH_EFFICIENT_BOOSTER,TECH_BOOST_INJECTORS,TECH_EFFICIENT_HYPERDRIVE};a->item=stock[d->seller[seller].meetings%8];dockbay_roll_generate(&a->roll,db_hash(old),a->item,d->seller[seller].trusted?DB_RARE:DB_MODDED,0);a->price=a->roll.value;
  if(d->route==(unsigned)i+1)d->route=0;d->dirty=1;
 }
}
int dockbay_shop_next(Game *g,int index,int npc){
 if(index<0||index>=DB_ADS||dockbay_actor(g,npc)!=index)return -1;DockAd *a=&g->dockbay.ad[index];
 for(int step=1;step<DB_ADS;step++){int k=(index+step)%DB_ADS;DockAd *b=&g->dockbay.ad[k];if(b->seller!=a->seller||b->system!=a->system||b->site!=a->site||b->state>=DB_COMPLETE||!b->stock||db_danger(g,a))continue;
  b->meeting=a->meeting?a->meeting:a->id;b->x=a->x;b->y=a->y;b->z=a->z;b->actor_health[0]=g->npc[npc].health;b->actor_shield[0]=g->npc[npc].shield;b->flags|=DB_POINT|DB_WATCH;g->npc[npc].dockbay_id=b->id;g->dockbay.route=k+1;g->dockbay.dirty=1;return k;
 }return -1;
}
int dockbay_ready(const Game *g){return g->dockbay.ready&&g->dockbay.route>0&&g->dockbay.route<=DB_ADS&&g->dockbay.ad[g->dockbay.route-1].system==(unsigned)g->system&&g->dockbay.ad[g->dockbay.route-1].state<DB_COMPLETE;}
Vec3 dockbay_point(const Game *g,int index){
 if(index<0||index>=DB_ADS)return g->pos;const DockAd *a=&g->dockbay.ad[index];if(a->flags&DB_POINT)return (Vec3){a->x,a->y,a->z};
 Vec3 p=hub_position(g,a->site%HUB_COUNT);float half=station_half_for(g,a->site%HUB_COUNT),angle=(g->dockbay.seller[a->seller].seed%19)*.33069396f;
 p=add(p,(Vec3){cosf(angle)*(half*5+6000),1800+sinf(angle)*1500,sinf(angle)*(half*5+6000)});
 if(a->site==2||a->site==3)for(int i=0;i<DEBRIS_COUNT;i++)if(g->debris[i].alive&&(a->site==2?g->debris[i].rock:g->debris[i].wreck)){
  Vec3 candidate=add(g->debris[i].pos,(Vec3){700,500,600});int safe=1;for(int hub=0;hub<HUB_COUNT;hub++)if(length(sub(candidate,hub_position(g,hub)))<station_half_for(g,hub)*4+2500)safe=0;
  if(safe){p=candidate;break;}
 }
 if(a->site==4)for(int b=1;b<BODY_COUNT;b++)if(body_active(&g->bodies[b])&&g->bodies[b].type==GAS){p=planet_ring_point(&g->bodies[b],angle,(planet_ring_inner(&g->bodies[b])+planet_ring_outer(&g->bodies[b]))*.5f);p.y+=500;break;}
 /* Move an overlapping candidate outward without snapping into a landing path. */
 for(int tries=0;tries<12;tries++){int clear=1;for(int b=0;b<BODY_COUNT;b++)if(body_active(&g->bodies[b])&&length(sub(p,g->bodies[b].pos))<g->bodies[b].radius+5000){p=add(g->bodies[b].pos,mul(norm(sub(p,g->bodies[b].pos)),g->bodies[b].radius+6500));clear=0;}if(clear)break;}
 for(int hub=0;hub<HUB_COUNT;hub++){Vec3 centre=hub_position(g,hub);float clear=station_half_for(g,hub)*4+2500;if(length(sub(p,centre))<clear){Vec3 away=norm(sub(p,centre));if(length(away)<.1f)away=(Vec3){1,.4f,0};p=add(centre,mul(away,clear+1000));}}
 return p;
}
int dockbay_route(Game *g,int index,int replace,char *reason,int size){
 dockbay_init(g);if(index<0||index>=DB_ADS)return 0;DockAd *a=&g->dockbay.ad[index];
 if(a->state>=DB_COMPLETE){snprintf(reason,size,"This deal has ended.");return 0;}
 if(g->erl.event_kind||race_ready(g)||g->dock_stage){snprintf(reason,size,"Finish this flight operation first.");return 0;}
 int jumps=0,hop=a->system==(unsigned)g->system?g->system:route_next_hop(g,a->system,&jumps);
 if(hop<0){snprintf(reason,size,"No route with your current fuel and drive.");return 0;}
 if(g->route_goal>=0&&g->route_goal!=(int)a->system&&!replace){snprintf(reason,size,"Replace your current navigation route?");return -1;}
 if(g->dockbay.route&&g->dockbay.route!=(unsigned)index+1){DockAd *old=&g->dockbay.ad[g->dockbay.route-1];int danger=db_danger(g,old)&&(old->flags&DB_REVEALED);
  if(danger&&!g->docked&&old->system==(unsigned)g->system&&length(sub(g->pos,dockbay_point(g,g->dockbay.route-1)))<24000){snprintf(reason,size,"Break away from the attackers before setting a new meeting.");return 0;}
  if(danger){old->flags|=DB_DANGER_DONE|DB_ESCAPED;if(old->outcome==1&&old->state<DB_COMPLETE)old->state=DB_CANCELLED;}
  for(int n=0;n<NPC_COUNT;n++)if((g->npc[n].dockbay_id&0x3fffffffu)==old->id){NPC *actor=&g->npc[n];if(!(actor->dockbay_id&0x40000000u)&&actor->route_leg<3){old->actor_health[actor->route_leg]=fmaxf(0,actor->health);old->actor_shield[actor->route_leg]=fmaxf(0,actor->shield);}actor->alive=0;actor->dockbay_id=0;}
  g->dockbay_npc=-1;g->dockbay_depart=0;
 }
 g->dockbay.route=index+1;a->flags|=DB_WATCH;if(a->state<DB_ROUTED)a->state=DB_ROUTED;g->dockbay.dirty=1;
 if(a->system!=(unsigned)g->system){route_set_goal(g,a->system);g->destination=hop;}else{Vec3 p=dockbay_point(g,index);a->x=p.x;a->y=p.y;a->z=p.z;a->flags|=DB_POINT;}
 snprintf(reason,size,a->system==(unsigned)g->system?"NAV DATA RECEIVED / LOCAL MEETING":"NAV DATA RECEIVED / %d JUMPS",jumps);return 1;
}
int dockbay_actor(const Game *g,int npc){
 if(npc<0||npc>=NPC_COUNT)return -1;uint32_t id=g->npc[npc].dockbay_id;if(!id)return -1;
 for(int i=0;i<DB_ADS;i++)if(g->dockbay.ad[i].id==id&&g->dockbay.ad[i].system==(unsigned)g->system)return i;return -1;
}
const char *dockbay_contact_unavailable(const Game *g,int npc){
 int i=dockbay_actor(g,npc);if(i<0||!g->npc[npc].alive)return "Approach the seller's meeting beacon.";
 if(g->docked||g->dead||g->dock_stage||g->jump>0||g->planet>=0||race_ready(g))return "Contact unavailable during this operation.";
 if(length(sub(g->npc[npc].pos,g->pos))>600)return "Move within 600 m of the seller.";
 if(g->dockbay.ad[i].state>=DB_COMPLETE)return "This deal has ended.";
 if(g->npc[npc].player_tag>0)return "The seller is defending their ship.";
 return 0;
}
int dockbay_hail(Game *g,int npc,char *reason,int size){
 int i=dockbay_actor(g,npc);if(i<0)return -1;DockAd *a=&g->dockbay.ad[i];
 const char *unavailable=dockbay_contact_unavailable(g,npc);if(unavailable){snprintf(reason,size,"%s",unavailable);return -2;}
 if(g->dockbay.seller[a->seller].trusted&&a->outcome<3)a->outcome=0;
 if(db_danger(g,a)&&(a->outcome==1||a->outcome==3)){dockbay_reveal(g,i);snprintf(reason,size,"Incoming weapons fire. Break contact!");return -3;}
 g->npc[npc].name_known=1;a->state=a->state==DB_PARTIAL?DB_PARTIAL:DB_MET;g->dockbay.dirty=1;snprintf(reason,size,"There you are! Have a look. The price is just as advertised.");return i;
}
int dockbay_check(const Game *g,int index,unsigned revision,int quantity,char *reason,int size){
 if(index<0||index>=DB_ADS){snprintf(reason,size,"No offer selected.");return 0;}const DockAd *a=&g->dockbay.ad[index];
 int actor=-1;for(int n=0;n<NPC_COUNT;n++)if(g->npc[n].alive&&g->npc[n].dockbay_id==a->id){actor=n;break;}
 if(actor<0||a->system!=(unsigned)g->system||length(sub(g->npc[actor].pos,g->pos))>600||g->docked||g->dead||g->dock_stage||g->jump>0||g->planet>=0||g->erl.event_kind||race_ready(g)){snprintf(reason,size,"Seller unavailable or outside 600 m.");return 0;}
 if(g->npc[actor].player_tag>0){snprintf(reason,size,"Seller is defending their ship.");return 0;}
 if(a->revision!=revision||a->state!=DB_MET&&a->state!=DB_PARTIAL||!a->stock){snprintf(reason,size,"Offer changed. Review it again.");return 0;}
 if(db_danger(g,a)&&(a->outcome==1||a->outcome==3)){snprintf(reason,size,"Transfer cancelled. Incoming attackers!");return 0;}
 if(quantity<1||quantity>(int)a->stock||a->kind==DB_MODULE&&quantity!=1){snprintf(reason,size,"Choose an available quantity.");return 0;}
 if(a->price>100000000u/(unsigned)quantity){snprintf(reason,size,"This batch exceeds the balance limit.");return 0;}int cost=a->price*quantity;
 if(a->kind==DB_BUY_CARGO){if(g->cargo[a->item]-mission_cargo_reserved(g,a->item)<quantity){snprintf(reason,size,"Not enough unreserved cargo.");return 0;}if(g->credits>100000000-cost){snprintf(reason,size,"Commander balance limit reached.");return 0;}}
 else if(g->credits<cost){snprintf(reason,size,"Not enough units.");return 0;}
 if(a->kind==DB_MODULE&&dockbay_storage_used(g)>=dockbay_storage_capacity(g)){snprintf(reason,size,"ITEM STORAGE FULL");return 0;}
 if(a->kind==DB_CARGO&&goods[a->item].unit=='t'&&cargo_used(g)+quantity>cargo_capacity_for(g,a->item)){snprintf(reason,size,"Not enough free cargo hold.");return 0;}
 if(a->kind==DB_MODULE&&db_owned_free(g)<0){snprintf(reason,size,"Equipment records are full.");return 0;}return 1;
}
int dockbay_trade(Game *g,int index,unsigned revision,int quantity,char *reason,int size){
 if(!dockbay_check(g,index,revision,quantity,reason,size))return 0;DockAd *a=&g->dockbay.ad[index];int cost=a->price*quantity;
 /* All checks complete: this is the sole money/stock commit point. */
 if(a->kind==DB_MODULE){int free=db_owned_free(g);if(free<0){snprintf(reason,size,"Equipment records are full.");return 0;}g->dockbay.owned[free]=a->roll;}
 else g->cargo[a->item]+=a->kind==DB_BUY_CARGO?-quantity:quantity;
 g->credits+=a->kind==DB_BUY_CARGO?cost:-cost;a->stock-=quantity;a->revision++;a->state=a->stock?DB_PARTIAL:DB_COMPLETE;
 a->receipt_serial=++g->dockbay.receipt_serial;a->receipt_qty=quantity;a->receipt_paid=cost;a->receipt_seen=0;a->flags|=DB_RECEIPT;g->dockbay.receipt_ad=index+1;g->dockbay.dirty=1;
 db_count_meeting(g,a);db_archive(g,a);g->dockbay.resume=1;
 g->cue=SFX_UI;snprintf(reason,size,"Trade complete.");return 1;
}
int dockbay_reveal(Game *g,int index){
 if(index<0||index>=DB_ADS)return 0;DockAd *a=&g->dockbay.ad[index];if(!db_danger(g,a))return 0;
 a->flags|=DB_REVEALED;if(a->outcome==2)a->flags|=DB_POST_ATTACK;a->revision++;g->dockbay.dirty=1;g->cue=SFX_ALERT;return 1;
}
/* Wreck goods come from unpaid stock. A completed purchase can never drop again. */
int dockbay_wreck(Game *g,int npc){
 int index=dockbay_actor(g,npc);if(index<0)return 0;DockAd *a=&g->dockbay.ad[index];NPC *n=&g->npc[npc];Vec3 drift=mul(n->dir,12);spawn_debris(g,n->pos,drift,-1,80,1);
 if(a->kind==DB_MODULE&&a->stock){int pod=spawn_debris(g,add(n->pos,(Vec3){70,20,0}),drift,-1,1,0);if(pod>=0){g->debris[pod].dockbay_id=a->id;a->stock=0;a->flags|=DB_LOOT_PENDING;}}
 if(a->kind==DB_CARGO)for(int k=0;k<4&&a->stock;k++){int qty=(a->stock+3-k)/(4-k),pod=spawn_debris(g,add(n->pos,(Vec3){70+k*55.f,20,k*40.f}),drift,a->item,qty,0);if(pod>=0)a->stock-=qty;}
 g->dockbay.dirty=1;return 1;
}
int dockbay_salvage(Game *g,int debris){
 if(debris<0||debris>=DEBRIS_COUNT||!g->debris[debris].alive||!g->debris[debris].dockbay_id)return 0;Debris *pod=&g->debris[debris];
 if(g->docked||g->dead||g->jump>0||g->dock_stage||g->planet>=0||length(sub(pod->pos,g->pos))>tech_tractor_range(g)){message(g,"Move within tractor range of the module pod.");return 1;}
 for(int i=0;i<DB_ADS;i++){DockAd *a=&g->dockbay.ad[i];if(a->id!=pod->dockbay_id)continue;
  if(a->flags&DB_LOOT_TAKEN||!(a->flags&DB_LOOT_PENDING)){pod->alive=0;return 1;}
  int free=db_owned_free(g);if(free<0||dockbay_storage_used(g)>=dockbay_storage_capacity(g)){message(g,"ITEM STORAGE FULL. The module remains outside.");return 1;}
  g->dockbay.owned[free]=a->roll;a->flags=(a->flags|DB_LOOT_TAKEN)&~DB_LOOT_PENDING;pod->alive=0;g->dockbay.dirty=1;g->cue=SFX_SCAN;message(g,"Recovered the seller's actual module into Item Storage.");return 1;
 }return 0;
}
int dockbay_report(Game *g,int index){if(index<0||index>=DB_ADS)return 0;DockAd *a=&g->dockbay.ad[index];if(!(a->flags&DB_REVEALED)||a->outcome==3)return 0;a->state=DB_REPORTED;g->dockbay.seller[a->seller].blocked=1;g->dockbay.dirty=1;return 1;}
static void dockbay_killed(Game *g,int npc){NPC *n=&g->npc[npc];if(!n->dockbay_id||n->dockbay_id&0x40000000u)return;for(int i=0;i<DB_ADS;i++){DockAd *a=&g->dockbay.ad[i];if(a->id!=(n->dockbay_id&0x3fffffffu)||n->route_leg>2)continue;a->visits|=1u<<n->route_leg;a->actor_health[n->route_leg]=0;a->actor_shield[n->route_leg]=0;if(!n->route_leg&&a->state<DB_COMPLETE)a->state=DB_CANCELLED;g->dockbay.dirty=1;return;}}
void dockbay_departure(Game *g,int index){if(index<0||index>=DB_ADS)return;DockAd *a=&g->dockbay.ad[index];a->receipt_seen=1;g->dockbay.dirty=1;if(!a->stock&&!db_danger(g,a))g->dockbay_depart=.001f;}
static int db_free_actor(const Game *g){for(int n=35;n>=0;n--)if(!g->npc[n].alive&&!g->npc[n].dockbay_id&&g->npc[n].bounty_slot<0&&g->npc[n].traveller<0&&expedition_npc(g,n)<0&&n!=8&&n!=20&&n!=32)return n;return -1;}
static int db_spawn_actor(Game *g,DockAd *a,int k,int companion){
 int slot=db_free_actor(g);if(slot<0)return -1;if(!a->meeting)a->meeting=a->id;NPC *n=&g->npc[slot];memset(n,0,sizeof(*n));n->alive=1;n->bounty_slot=n->traveller=-1;n->mega_hull=-1;n->dockbay_id=a->id|(companion?0x40000000u:k?0x80000000u:0);n->route_leg=k;n->vessel_seed=g->dockbay.seller[a->seller].seed+k*73;n->role=companion?TRADERS:k?PIRATES:TRADERS;n->mesh=mesh_id(k&&!companion?"SIDEWINDER":a->kind==DB_MODULE?"COBRA MK 3":"PYTHON");n->scale=a->kind!=DB_MODULE&&!k?1.7f:1;n->radius=30;for(int vertex=0;vertex<meshes[n->mesh].vertices;vertex++)n->radius=fmaxf(n->radius,length(meshes[n->mesh].v[vertex])*n->scale);n->health=companion?100:a->actor_health[k];n->shield=companion?60:a->actor_shield[k];n->pos=add(dockbay_point(g,a-g->dockbay.ad),(Vec3){k?((k&1)?-850:850):0,k*180,k?600:0});n->dir=norm(sub(g->pos,n->pos));n->target=-1;return slot;
}
/* Attackers use the real NPC pool, damage, collision, weapons and destruction rules. */
static void db_attack_flight(Game *g,NPC *n,int slot,float dt){
 Vec3 aim=sub(g->pos,n->pos);float distance=length(aim);Vec3 toward=norm(aim),desired=toward;
 if(distance<230)desired=norm(add(mul(toward,-1),(Vec3){.45f,.2f,.3f}));
 else if(distance<1000)desired=norm(add(toward,(Vec3){sinf(g->time*.6f+slot)*.35f,cosf(g->time*.4f+slot)*.16f,0}));
 float turn=fminf(1,dt*1.8f);n->dir=norm(add(mul(n->dir,1-turn),mul(desired,turn)));n->cruise=distance>1200?330:distance<230?260:200;n->pos=add(n->pos,mul(n->dir,n->cruise*dt));n->target=-2;n->cooldown-=dt;
 if(dt>0&&distance<1500&&dot(n->dir,toward)>.85f&&n->cooldown<=0){n->cooldown=1.25f+slot%3*.15f;n->flash=.12f;g->fire_bearing_time[slot]=2.5f;g->attacked=3;player_damage(g,4,"Weapons fire is striking your shields.");}
}
void dockbay_tick(Game *g,float dt){
 DockBay *d=&g->dockbay;if(!d->ready)return;
 if(g->dockbay_live_system!=g->system){g->dockbay_live_system=g->system;g->dockbay_npc=-1;g->dockbay_depart=0;}
 int index=d->route?d->route-1:-1;if(index<0||index>=DB_ADS)return;DockAd *a=&d->ad[index];
 if(a->system!=(unsigned)g->system)return;
 int slot=g->dockbay_npc;if(slot>=0&&(slot>=NPC_COUNT||g->npc[slot].dockbay_id!=a->id)){g->dockbay_npc=-1;slot=-1;}
 if(slot>=0&&!g->npc[slot].alive){a->visits|=1;a->actor_health[0]=0;if(a->state<DB_COMPLETE)a->state=DB_CANCELLED;a->revision++;d->dirty=1;g->dockbay_npc=-1;slot=-1;if(!(a->flags&DB_REVEALED)){a->flags|=DB_DANGER_DONE;return;}}
 if(g->dead||g->docked||g->dock_stage||g->jump>0||g->planet>=0||race_ready(g)||g->erl.event_kind)return;
 Vec3 point=dockbay_point(g,index);if(!(a->flags&DB_POINT)){a->x=point.x;a->y=point.y;a->z=point.z;a->flags|=DB_POINT;d->dirty=1;}
 if(slot<0&&!(a->visits&1)&&(dockbay_ready(g)||a->flags&DB_REVEALED&&db_danger(g,a)||a->state==DB_COMPLETE&&!a->receipt_seen)&&length(sub(point,g->pos))<12000){
  slot=db_spawn_actor(g,a,0,0);if(slot<0)return;g->dockbay_npc=slot;
 }
 /* A saved wreck retains its one real unpaid module, even after reload. */
 if(a->flags&DB_LOOT_PENDING&&!(a->flags&DB_LOOT_TAKEN)&&length(sub(point,g->pos))<12000){int present=0;for(int k=0;k<DEBRIS_COUNT;k++)present|=g->debris[k].alive&&g->debris[k].dockbay_id==a->id;if(!present){int pod=spawn_debris(g,add(point,(Vec3){70,20,0}),(Vec3){0,0,0},-1,1,0);if(pod>=0)g->debris[pod].dockbay_id=a->id;}}
 int danger=db_danger(g,a)&&a->flags&DB_REVEALED;
 if(danger)for(int k=1;k<3;k++)if(!(a->visits&(1u<<k))){int found=-1;for(int n=0;n<NPC_COUNT;n++)if(g->npc[n].dockbay_id==(a->id|0x80000000u)&&g->npc[n].route_leg==k){found=n;break;}if(found<0)found=db_spawn_actor(g,a,k,0);if(found>=0&&!g->npc[found].alive){a->visits|=1u<<k;a->actor_health[k]=0;d->dirty=1;}}
 if(slot>=0&&a->encounter==18&&!danger)for(int k=1;k<3;k++){int present=0;for(int n=0;n<NPC_COUNT;n++)if(g->npc[n].dockbay_id==(a->id|0x40000000u)&&g->npc[n].route_leg==k)present=1;if(!present)db_spawn_actor(g,a,k,1);}
 for(int j=0;j<NPC_COUNT;j++){NPC *n=&g->npc[j];if((n->dockbay_id&0x3fffffffu)!=a->id||!n->alive)continue;int k=n->route_leg;n->flash=fmaxf(0,n->flash-dt);
  int hostile=danger&&(a->outcome!=3||n->dockbay_id&0x80000000u);if(hostile||n->player_tag>0){n->role=PIRATES;db_attack_flight(g,n,j,dt);}
  else if(g->dockbay_depart>0){n->cruise=fminf(380,n->cruise+dt*100);n->dir=norm(add(n->dir,mul((Vec3){.1f,.04f,1},dt)));n->pos=add(n->pos,mul(n->dir,n->cruise*dt));}
  else {n->cruise=0;float t=fminf(1,dt*1.8f);n->dir=norm(add(mul(n->dir,1-t),mul(norm(sub(g->pos,n->pos)),t)));}
  if(dt>0&&length(sub(n->pos,g->pos))<n->radius+15&&g->collision<=0){Vec3 away=norm(sub(g->pos,n->pos));if(length(away)<.1f)away=(Vec3){0,0,-1};g->pos=add(n->pos,mul(away,n->radius+20));g->speed=0;g->boost=0;g->collision=2;collision_damage(g,5,"Ship collision. Shields damaged.");hit(g,j,10,1);}
  if(!(n->dockbay_id&0x40000000u)&&k<3){a->actor_health[k]=n->health;a->actor_shield[k]=n->shield;}
 }
 if(danger){unsigned needed=a->outcome==3?6:7;int escaped=length(sub(point,g->pos))>24000;
  if((a->visits&needed)==needed||escaped){a->flags|=DB_DANGER_DONE|(escaped?DB_ESCAPED:DB_ASSISTED);if(a->outcome==1&&a->state<DB_COMPLETE)a->state=DB_CANCELLED;db_count_meeting(g,a);d->dirty=1;for(int n=0;n<NPC_COUNT;n++)if((g->npc[n].dockbay_id&0x3fffffffu)==a->id&&g->npc[n].role==PIRATES){g->npc[n].alive=0;g->npc[n].dockbay_id=0;}if(slot>=0&&a->outcome!=3)g->dockbay_npc=-1;}
 }
 if(g->dockbay_depart>0){g->dockbay_depart+=dt;if(g->dockbay_depart>18){for(int n=0;n<NPC_COUNT;n++)if((g->npc[n].dockbay_id&0x3fffffffu)==a->id){g->npc[n].alive=0;g->npc[n].dockbay_id=0;}g->dockbay_npc=-1;d->route=0;g->dockbay_depart=0;}}
}
