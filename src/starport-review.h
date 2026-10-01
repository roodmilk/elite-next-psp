static int starport_review(void){
 FILE *flag=fopen("starport-review.flag","r");if(!flag)return 0;fclose(flag);
 int failures=starport_layout_tests("starport-layout-tests.txt");
 FILE *f=fopen("starport-review.txt","w");unsigned *saved=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));if(!f||!pixels)return 1;
 fb=pixels+16;for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xa55ac33cu;
 for(int body=1;body<=4;body++){
  field_review_setup(7,body);Vec3 pad=surface_site(&game,1);
  static const int days[]={600,900,1200,1800};float hour=field_local_hour(&game,7,body);game.world_clock=(10-hour+24)*days[(field_seed(7,body)>>24)&3]/24;
  float total=0,worst=0;
  for(int view=0;view<8;view++){
   Vec3 look=pad;game.pos=add(pad,(Vec3){-850,285,-960});
   if(view==1){game.pos=add(pad,(Vec3){160,22,-220});look=add(pad,(Vec3){420,55,-500});}
   if(view==2){game.pos=add(pad,(Vec3){0,22,220});look=add(pad,(Vec3){460,80,500});}
   if(view==3){game.pos=add(pad,(Vec3){220,22,80});look=add(pad,(Vec3){520,50,80});game.world_clock=205-63-(game.bodies[body].seed%40);}
   if(view==4){game.pos=add(pad,(Vec3){FIELD_GARAGE_X,22,110});look=add(pad,(Vec3){FIELD_GARAGE_X,25,-20});}
   if(view==5){game.pos=add(pad,(Vec3){280,22,-130});look=add(pad,(Vec3){630,180,-200});}
   if(view>=6){game.pos=add(pad,(Vec3){160,22,-220});look=add(pad,(Vec3){420,55,-500});high_contrast=view==7;if(view==6)game.world_clock+=days[(field_seed(7,body)>>24)&3]*.5f;}
   game.pos.y+=24;look.y+=24;Vec3 aim=norm(sub(look,game.pos));game.yaw=atan2f(aim.x,aim.z);game.pitch=asinf(aim.y);game.roll=0;
   uint64_t a,b;sceRtcGetCurrentTick(&a);field_review_frame(0);sceRtcGetCurrentTick(&b);float ms=(b-a)*1000.f/sceRtcGetTickResolution();total+=ms;worst=fmaxf(worst,ms);
   char path[64];snprintf(path,sizeof(path),"starport-%d-view-%d.bmp",body,view);dump_native_bmp(path);
  }
  fprintf(f,"Planet %d draw avg %.3fms worst %.3fms (no audio/display)\n",body,total/8,worst);
  for(int kind=1;kind<=3;kind++)for(int step=0;step<=100;step++){
   PlanetPilotCamera c=planet_pilot_sample(kind,step*planet_sequence_duration(kind)/100);
   for(int j=0;j<FIELD_PORT_BUILDINGS;j++){FieldBuilding s=field_port_buildings[j];
    if(fabsf(c.eye.x-pad.x-s.x)<s.w+8&&fabsf(c.eye.z-pad.z-s.z)<s.d+8&&c.eye.y<24+s.height+10){failures++;fprintf(f,"FAIL camera intersects port building %d on world %d\n",j,body);}
   }
  }
 }
 for(int i=0;i<16;i++)if(pixels[i]!=0xa55ac33cu||pixels[STRIDE*H+16+i]!=0xa55ac33cu)failures++;
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);fb=saved;free(pixels);return 1;
}
