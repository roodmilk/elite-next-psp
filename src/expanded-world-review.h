static int expanded_world_review(void){
 FILE *flag=fopen("expanded-world-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *f=fopen("expanded-world-review.txt","w");if(!f)return 1;int failures=0;
 #define XCHECK(ok,label) do{int pass=(ok);fprintf(f,"%s %s\n",pass?"PASS":"FAIL",label);failures+=!pass;}while(0)
 Game *g=calloc(1,sizeof(Game));if(!g){fclose(f);return 1;}game_init(g);
 int finite=1,dry=1,spread=1,gas_links=1,varied=1,river_worlds=0,bridges=1,bridge_dry_fail=0,bridge_wet_fail=0;
 float biome_low[6]={1e9f,1e9f,1e9f,1e9f,1e9f,1e9f},biome_high[6]={-1e9f,-1e9f,-1e9f,-1e9f,-1e9f,-1e9f};
 for(int sys=0;sys<256;sys++){g->system=sys;system_bodies(g);
  for(int body=1;body<BODY_COUNT;body++){
   g->planet=body;Vec3 pad=surface_site(g,1);int biome=field_profile(g,sys,body).biome;
   float low=1e9f,high=-1e9f;
   for(int ring=1;ring<=3;ring++)for(int bearing=0;bearing<12;bearing++){
    float a=bearing*.5235988f,r=ring*3000.f,x=pad.x+cosf(a)*r,z=pad.z+sinf(a)*r,h=terrain_height(g,x,z);
    finite&=isfinite(h);if(!terrain_is_water(g,x,z)){low=fminf(low,h);high=fmaxf(high,h);}
   }
   if(g->bodies[body].type!=GAS){varied&=high-low>18.f;biome_low[biome]=fminf(biome_low[biome],low);biome_high[biome]=fmaxf(biome_high[biome],high);}
   for(int id=1;id<10;id++)if(id!=6){Vec3 p=surface_poi(g,id);float d=length(sub(p,pad));finite&=isfinite(p.x)&&isfinite(p.y)&&isfinite(p.z);dry&=!terrain_is_water(g,p.x,p.z);spread&=d>1700.f&&d<EVA_FIELD_RADIUS-500.f;if(g->bodies[body].type==GAS)gas_links&=surface_cloud_deck(g,p.x,p.z);}
   int local_bridges=0;for(int i=0;i<6;i++){Vec3 p,cross;if(!terrain_bridge_position(g,i,&p,&cross))continue;local_bridges++;int dry_cross=!terrain_is_water(g,p.x,p.z),wet=0;for(int z=-600;z<=600&&!wet;z+=80)for(int x=-600;x<=600&&!wet;x+=80)if(abs(x)>180||abs(z)>180)wet=terrain_is_water(g,p.x+x,p.z+z);bridge_dry_fail+=!dry_cross;bridge_wet_fail+=!wet;bridges&=dry_cross&&wet;}
   river_worlds+=local_bridges>0;
  }
 }
 XCHECK(EVA_FIELD_RADIUS>=10500.f,"walkable radius is at least five times the original world");
 XCHECK(finite,"all 1024 worlds produce finite terrain and site coordinates");
 XCHECK(dry,"all generated activity sites stand on traversable terrain or cloud decks");
 XCHECK(spread,"activity sites occupy broad inner and outer expedition rings");
 XCHECK(varied,"every solid world has meaningful sampled vertical relief");
 XCHECK(river_worlds>500,"the galaxy contains hundreds of deterministic river or ravine worlds");
 XCHECK(bridges,"river crossings are dry and reconnect to water beyond each bridge");
 XCHECK(gas_links,"gas-giant sites remain connected to the expanded cloud-platform network");
 XCHECK(biome_high[3]-biome_low[3]>biome_high[0]-biome_low[0],"volcanic geology produces stronger relief than ocean country");
 fprintf(f,"INFO river worlds %d / 1024; radius %.0f m\n",river_worlds,EVA_FIELD_RADIUS);
 fprintf(f,"INFO bridge dry failures %d; nearby-water failures %d\n",bridge_dry_fail,bridge_wet_fail);
 for(int i=0;i<6;i++)if(biome_high[i]>biome_low[i])fprintf(f,"INFO biome %d relief %.1f m\n",i,biome_high[i]-biome_low[i]);
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);free(g);
 #undef XCHECK
 return 1;
}
