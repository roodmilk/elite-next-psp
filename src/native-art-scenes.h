/* Native PSP art contracts. Keep these dimensions in pixels, never logical UI units. */
#ifndef ELITE_NATIVE_ART_SCENES_H
#define ELITE_NATIVE_ART_SCENES_H

#define NATIVE_ART_SCREEN_W 480
#define NATIVE_ART_SCREEN_H 272
#define NATIVE_ART_SCALE 1
#define NATIVE_ART_STATION_X 6
#define NATIVE_ART_STATION_Y 20
#define NATIVE_ART_STATION_W 340
#define NATIVE_ART_STATION_H 168

typedef struct {
 const char *id;
 int x,y,w,h;
 int safe_top,safe_bottom;
} NativeArtScene;

enum { NATIVE_ART_STATION_ARRIVALS=0, NATIVE_ART_PLANET_APPROACH, NATIVE_ART_CAMPAIGN_DIALOGUE, NATIVE_ART_SCENE_COUNT };

static const NativeArtScene native_art_scenes[NATIVE_ART_SCENE_COUNT]={
 {"station_arrivals",NATIVE_ART_STATION_X,NATIVE_ART_STATION_Y,NATIVE_ART_STATION_W,NATIVE_ART_STATION_H,23,191},
 {"planet_approach",0,24,480,168,24,191},
 {"campaign_dialogue",16,54,448,68,23,191},
};

static const NativeArtScene *native_art_scene(int id){
 if(id<0||id>=NATIVE_ART_SCENE_COUNT)return &native_art_scenes[0];
 return &native_art_scenes[id];
}

#endif
