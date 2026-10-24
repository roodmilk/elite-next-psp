/* Native opt-in atmosphere comparisons and handoff safety; no player files. */
static int atmosphere_review(void){
 FILE *flag=fopen("atmosphere-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *f=fopen("atmosphere-review.txt","w");unsigned *saved=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned)),*reference=malloc(STRIDE*H*sizeof(unsigned));if(!f||!pixels||!reference)return 1;
 fb=pixels+16;for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xa55ac33cu;
 int failures=0;
 #define CCHECK(ok,label) do{int pass=(ok);fprintf(f,"%s %s\n",pass?"PASS":"FAIL",label);failures+=!pass;fflush(f);}while(0)
 field_review_setup(7,1);planet_prepare_veil(1);
 rect(0,0,W,H,RGB(30,70,90));memcpy(reference,fb,STRIDE*H*sizeof(unsigned));planet_cloud_cover(0);
 CCHECK(!memcmp(reference,fb,STRIDE*H*sizeof(unsigned)),"zero cover leaves scene unchanged");
 planet_cloud_cover(1);memcpy(reference,fb,STRIDE*H*sizeof(unsigned));rect(0,0,W,H,RGB(210,35,70));planet_cloud_cover(1);
 CCHECK(!memcmp(reference,fb,STRIDE*H*sizeof(unsigned)),"complete cover hides any loading scene");
 rect(0,0,W,H,RGB(30,70,90));planet_cloud_cover_flow(.7f,.3f);
 int centre_clear=1,edge_cloud=0;
 for(int y=0;y<H;y++)for(int x=220;x<260;x++)centre_clear&=fb[y*STRIDE+x]==RGB(30,70,90);
 for(int y=0;y<H;y++)edge_cloud|=fb[y*STRIDE+8]!=RGB(30,70,90);
 CCHECK(centre_clear&&edge_cloud,"arrival opens a clear centre corridor while side cloud banks remain visible");
 dump_native_bmp("cloud-corridor.bmp");
 rect(0,0,W,H,RGB(30,70,90));planet_cloud_cover_flow(1,.5f);memcpy(reference,fb,STRIDE*H*sizeof(unsigned));
 rect(0,0,W,H,RGB(210,35,70));planet_cloud_cover_flow(1,.5f);
 CCHECK(!memcmp(reference,fb,STRIDE*H*sizeof(unsigned)),"outward cloud flow cannot reveal the fully covered loading handoff");
 double cover_total=0;
 for(int frame=0;frame<20;frame++){
  float amount=.05f+frame*.05f;game.time=20+frame*.1f;rect(0,0,W,H,RGB(54,112,180));
  uint64_t a=sceKernelGetSystemTimeWide();planet_cloud_cover(amount);cover_total+=(sceKernelGetSystemTimeWide()-a)*.001;
  if(frame%4==0){char path[64];snprintf(path,sizeof(path),"cloud-cover-%02d.bmp",frame);dump_native_bmp(path);}
 }
 fprintf(f,"SUMMARY cloud compositing avg %.3fms\n",cover_total/20);
 double total=0;float worst=0;int count=0;
 for(int body=1;body<=4;body++)for(int mode=0;mode<4;mode++){
  field_review_setup(7,body);game.surface=0;planet_sequence_begin(mode<2?1:3);
  game.planet_sequence_time=mode==0?.75f:mode==1?3.5f:mode==2?5.2f:7.1f;game.time=20;
  for(int i=0;i<4;i++){uint64_t a=sceKernelGetSystemTimeWide();drawcount=0;rect(0,0,W,H,BG);space();float ms=(sceKernelGetSystemTimeWide()-a)*.001f;total+=ms;worst=fmaxf(worst,ms);count++;}
  char path[64];snprintf(path,sizeof(path),"atmosphere-%d-%d.bmp",body,mode);dump_native_bmp(path);
 }
 fprintf(f,"SUMMARY arrival/ascent render avg %.3fms worst %.3fms\n",total/count,worst);
 int guard=1;for(int i=0;i<16;i++)guard&=pixels[i]==0xa55ac33cu&&pixels[STRIDE*H+16+i]==0xa55ac33cu;
 CCHECK(guard,"framebuffer bounds preserved across cloud layers");
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);fb=saved;free(pixels);free(reference);
 #undef CCHECK
 return 1;
}
