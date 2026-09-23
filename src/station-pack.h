#ifndef STATION_PACK_H
#define STATION_PACK_H

/* Optional EPST v1 loader. Station presentation remains fallback-safe until
 * the Station owner allocates a runtime consumer for these records. */
#define STATION_PACK_MAX 768
typedef struct {uint8_t system,hub,architecture,arrangement,landmark,windows,traffic,security,service;uint32_t seed;} StationPackRecord;
extern StationPackRecord station_pack_records[STATION_PACK_MAX];
extern unsigned station_pack_count,station_pack_loaded;
static uint32_t station_pack_checksum(const uint8_t *data,unsigned len){uint32_t h=2166136261u;for(unsigned i=0;i<len;i++){h^=data[i];h*=16777619u;}return h;}
static int station_pack_record_valid(const StationPackRecord *r){return r&&r->hub<3&&r->architecture<6&&r->arrangement<8&&r->landmark<8&&r->windows>=4&&r->windows<=13&&r->traffic>=2&&r->traffic<=10&&r->security>=1&&r->security<=7&&r->service>=1&&r->service<=5;}
static int station_pack_load(const char *path){
 FILE *f=fopen(path,"rb");if(!f)return 0;uint8_t header[16],raw[STATION_PACK_MAX*16];StationPackRecord next[STATION_PACK_MAX];unsigned count=0;int ok=1;
 if(fread(header,1,16,f)!=16||memcmp(header,"EPST",4)!=0||header[4]!=1)ok=0;
 if(ok){count=(unsigned)header[6]|((unsigned)header[7]<<8);unsigned bytes=(unsigned)header[8]|((unsigned)header[9]<<8)|((unsigned)header[10]<<16)|((unsigned)header[11]<<24);uint32_t stored=(uint32_t)header[12]|((uint32_t)header[13]<<8)|((uint32_t)header[14]<<16)|((uint32_t)header[15]<<24);if(count==0||count>STATION_PACK_MAX||bytes!=count*16||fread(raw,1,bytes,f)!=bytes||station_pack_checksum(raw,bytes)!=stored)ok=0;}
 if(ok)for(unsigned i=0;i<count;i++){const uint8_t *p=raw+i*16;StationPackRecord r={(uint8_t)p[0],(uint8_t)p[1],(uint8_t)p[2],(uint8_t)p[3],(uint8_t)p[4],(uint8_t)p[5],(uint8_t)p[6],(uint8_t)p[7],(uint8_t)p[8],(uint32_t)p[12]|((uint32_t)p[13]<<8)|((uint32_t)p[14]<<16)|((uint32_t)p[15]<<24)};if(!station_pack_record_valid(&r)){ok=0;break;}next[i]=r;}
 fclose(f);if(!ok){station_pack_loaded=0;station_pack_count=0;return 0;}memcpy(station_pack_records,next,count*sizeof(*next));station_pack_count=count;station_pack_loaded=1;return 1;
}
static const StationPackRecord *station_pack_find(unsigned system,unsigned hub){if(!station_pack_loaded)return 0;for(unsigned i=0;i<station_pack_count;i++)if(station_pack_records[i].system==system&&station_pack_records[i].hub==hub)return &station_pack_records[i];return 0;}
static void station_pack_tests(void){FILE *side=fopen("station.content","rb");int ok=1;if(side){fclose(side);ok=station_pack_load("station.content")&&station_pack_count==768&&station_pack_find(0,0)&&station_pack_find(0,1)&&station_pack_find(0,2);}FILE *out=fopen("station-pack-check.txt","w");if(out){fprintf(out,"RESULT %d failures\n",ok?0:1);fclose(out);}}
#endif
