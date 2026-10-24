/* V39 appends navigation provenance to the existing sealed commander. */
static int navigation_stream(FILE*f,NavigationState*n,int load){
 for(int s=0;s<256;s++){uint32_t a=n->visited[s],b=n->supplied[s];if(load){if(!load_u32(f,&a)||!load_u32(f,&b)||(a&~NAV_MASK)||(b&~NAV_MASK))return 0;n->visited[s]=a;n->supplied[s]=b;}else if(!save_u32(f,a)||!save_u32(f,b))return 0;}
 if(load){if(fread(n->scanned,1,256,f)!=256||fread(n->paid,1,256,f)!=256)return 0;for(int i=0;i<256;i++)if(n->scanned[i]&~63u||n->paid[i]&~7u)return 0;}
 else if(fwrite(n->scanned,1,256,f)!=256||fwrite(n->paid,1,256,f)!=256)return 0;
 uint32_t fields[4]={n->waypoint_system,n->waypoint_key,n->search_system,n->search_key};for(int i=0;i<4;i++)if(load){if(!load_u32(f,&fields[i]))return 0;}else if(!save_u32(f,fields[i]))return 0;
 if(fields[0]>255||fields[1]>NAV_KEYS||fields[2]>255||fields[3]>3||(fields[1]>27&&fields[1]<34))return 0;
 if(load){n->waypoint_system=fields[0];n->waypoint_key=fields[1];n->search_system=fields[2];n->search_key=fields[3];}return 1;
}
