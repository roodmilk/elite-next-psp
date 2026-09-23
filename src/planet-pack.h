#ifndef PLANET_PACK_H
#define PLANET_PACK_H

/* Optional EPWP v1 loader. Packs are an optimization/authoring input only;
 * runtime generation remains the fallback and commander saves never depend on
 * a pack being present. */
#define PLANET_PACK_MAX 768
typedef struct {uint8_t system,body,type,family,terrain_style,palette,sea_level,prop_density,activity_density;uint32_t seed;} PlanetPackRecord;
static PlanetPackRecord planet_pack_records[PLANET_PACK_MAX];
static unsigned planet_pack_count=0,planet_pack_loaded=0;
static uint32_t planet_pack_checksum(const uint8_t *data,unsigned len){uint32_t h=2166136261u;for(unsigned i=0;i<len;i++){h^=data[i];h*=16777619u;}return h;}
static int planet_pack_record_valid(const PlanetPackRecord *r){
 return r&&r->body>=1&&r->body<5&&r->type<=1&&r->family<=4&&r->terrain_style<4&&r->palette<8&&r->prop_density>=35&&r->prop_density<=80&&r->activity_density>=20&&r->activity_density<=80&&(r->type==0?r->family==0:r->family==1+(r->seed%4));
}
static int planet_pack_load(const char *path){
 FILE *f=fopen(path,"rb");if(!f)return 0;uint8_t header[16],raw[PLANET_PACK_MAX*16];PlanetPackRecord next[PLANET_PACK_MAX];unsigned count=0;int ok=1;
 if(fread(header,1,16,f)!=16||memcmp(header,"EPWP",4)!=0||header[4]!=1)ok=0;
 if(ok){count=(unsigned)header[6]|((unsigned)header[7]<<8);unsigned bytes=(unsigned)header[8]|((unsigned)header[9]<<8)|((unsigned)header[10]<<16)|((unsigned)header[11]<<24);uint32_t stored=(uint32_t)header[12]|((uint32_t)header[13]<<8)|((uint32_t)header[14]<<16)|((uint32_t)header[15]<<24);if(count==0||count>PLANET_PACK_MAX||bytes!=count*16||fread(raw,1,bytes,f)!=bytes||planet_pack_checksum(raw,bytes)!=stored)ok=0;}
 if(ok)for(unsigned i=0;i<count;i++){const uint8_t *p=raw+i*16;PlanetPackRecord r={(uint8_t)p[0],(uint8_t)p[1],(uint8_t)p[2],(uint8_t)p[3],(uint8_t)p[4],(uint8_t)p[5],(uint8_t)p[6],(uint8_t)p[7],(uint8_t)p[8],(uint32_t)p[12]|((uint32_t)p[13]<<8)|((uint32_t)p[14]<<16)|((uint32_t)p[15]<<24)};if(!planet_pack_record_valid(&r)){ok=0;break;}next[i]=r;}
 fclose(f);if(!ok){planet_pack_loaded=0;planet_pack_count=0;return 0;}memcpy(planet_pack_records,next,count*sizeof(*next));planet_pack_count=count;planet_pack_loaded=1;return 1;
}
static const PlanetPackRecord *planet_pack_find(unsigned system,unsigned body){if(!planet_pack_loaded)return 0;for(unsigned i=0;i<planet_pack_count;i++)if(planet_pack_records[i].system==system&&planet_pack_records[i].body==body)return &planet_pack_records[i];return 0;}
static void planet_pack_tests(void){
 FILE *side=fopen("planet.content","rb");int sidecar=1;if(side){fclose(side);sidecar=planet_pack_load("planet.content")&&planet_pack_count==768;}
 uint8_t raw[16]={0,1,1,1,0,0,0,40,20,0,0,0,0,0,0,0},head[16]={0};uint32_t sum=planet_pack_checksum(raw,16);memcpy(head,"EPWP",4);head[4]=1;head[6]=1;head[8]=16;memcpy(head+12,&sum,4);FILE *f=fopen("planet-pack-check.bin","wb");if(f){fwrite(head,1,16,f);fwrite(raw,1,16,f);fclose(f);}int good=planet_pack_load("planet-pack-check.bin");const PlanetPackRecord *r=planet_pack_find(0,1);remove("planet-pack-check.bin");FILE *bad=fopen("planet-pack-check.bin","wb");if(bad){fwrite("BAD!",1,4,bad);fclose(bad);}int rejected=!planet_pack_load("planet-pack-check.bin");remove("planet-pack-check.bin");FILE *out=fopen("planet-pack-check.txt","w");if(out){fprintf(out,"RESULT %d failures\n",(sidecar&&good&&r&&r->family==1&&rejected)?0:1);fclose(out);}
}
#endif
