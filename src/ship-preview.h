/* Centre the real mesh, then fit its bounding sphere with perspective margin.
 * Fixed scale per mesh avoids zoom pumping as a long hull rotates. */
static void ship_preview_layout(int id,Vec3 *centre,float *scale){
 const Mesh *m=&meshes[id];Vec3 lo=m->v[0],hi=lo;
 for(int i=1;i<m->vertices;i++){Vec3 v=m->v[i];lo.x=fminf(lo.x,v.x);lo.y=fminf(lo.y,v.y);lo.z=fminf(lo.z,v.z);hi.x=fmaxf(hi.x,v.x);hi.y=fmaxf(hi.y,v.y);hi.z=fmaxf(hi.z,v.z);}
 *centre=mul(add(lo,hi),.5f);float radius=1;
 for(int i=0;i<m->vertices;i++)radius=fmaxf(radius,length(sub(m->v[i],*centre)));
 /* Projection focal length 240, camera distance 320, screen radius <=36. */
 *scale=(36.f*320.f)/(radius*(240.f+36.f));
}
static void fitted_ship_preview(int id,float yaw,float roll){
 Vec3 centre;float scale;ship_preview_layout(id,&centre,&scale);
 Vec3 position=sub((Vec3){0,0,320},mul(rotate(centre,yaw,roll),scale));
 shipmesh(id,position,yaw,roll,scale,GOLD,1);
}
