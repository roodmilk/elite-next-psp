/* Sidecar identity keeps legacy catalogue IDs and every saved roll intact. */
static const DockRoll *db_slot_roll(const Game *g,int slot){if(slot<0||slot>=48)return 0;unsigned ref=g->dockbay.fitted[slot];if(!ref||ref>DB_ITEMS)return 0;const DockRoll *r=&g->dockbay.owned[ref-1];return r->id&&r->base==g->fit[slot]?r:0;}
static float db_slot_power(const Game *g,int slot,int secondary){const DockRoll *r=db_slot_roll(g,slot);return r?(secondary?r->secondary:r->power)*.001f:1.f;}
/* Passive copies share a bounded family benefit; the best fitted roll applies. */
static float db_item_power(const Game *g,int base,int secondary){float best=0;for(int slot=0;slot<48;slot++)if(g->fit[slot]==base)best=fmaxf(best,db_slot_power(g,slot,secondary));return best>0?best:1.f;}
static int db_instance_stored(const Game *g,int i){if(i<0||i>=DB_ITEMS||!g->dockbay.owned[i].id)return 0;for(int s=0;s<48;s++)if(g->dockbay.fitted[s]==(unsigned)i+1)return 0;for(int k=0;k<5;k++)if(k!=g->erl.active_ship)for(int s=0;s<48;s++)if(g->dockbay.fleet[k][s]==(unsigned)i+1)return 0;return 1;}
static int db_owned_free(const Game *g){for(int i=0;i<DB_ITEMS;i++)if(!g->dockbay.owned[i].id)return i;return -1;}
static void db_owned_map(const Game *g,unsigned char *used){memset(used,0,DB_ITEMS);for(int s=0;s<48;s++){unsigned r=g->dockbay.fitted[s];if(r&&r<=DB_ITEMS)used[r-1]=1;}for(int k=0;k<5;k++)if(k!=g->erl.active_ship)for(int s=0;s<48;s++){unsigned r=g->dockbay.fleet[k][s];if(r&&r<=DB_ITEMS)used[r-1]=1;}}
static int db_stored_count(const Game *g){unsigned char used[DB_ITEMS];db_owned_map(g,used);int n=0;for(int i=0;i<DB_ITEMS;i++)n+=g->dockbay.owned[i].id&&!used[i];return n;}
static int db_token_base(const Game *g,int token){int i=token-EQUIPMENT_COUNT;return token>=EQUIPMENT_COUNT&&i<DB_ITEMS&&g->dockbay.owned[i].id?g->dockbay.owned[i].base:token;}
static const char *db_quality(unsigned q){return q==DB_WORN?"WORN":q==DB_RARE?"RARE":q==DB_MODDED?"MODDED":"NORMAL";}
static float db_heat_efficiency(const Game *g){float factor=1;for(int s=0;s<48;s++){int b=g->fit[s];if(b==24||b==TECH_ADVANCED_HEAT_BUFFER)factor/=db_slot_power(g,s,0);else if(b==TECH_EFFICIENT_BOOSTER)factor/=db_slot_power(g,s,1);}return factor<.5f?.5f:factor>2.5f?2.5f:factor;}
