#ifndef ELITE_MEGA_CITY_GEOMETRY_H
#define ELITE_MEGA_CITY_GEOMETRY_H
#include <string.h>
Vec3 sub(Vec3 a,Vec3 b);Vec3 mul(Vec3 a,float scale);
float dot(Vec3 a,Vec3 b);float length(Vec3 a);
/* Small convex faceted hulls. The exact same vertices/planes supply drawing
 * and swept collision; the enclosing boxes are only the broad phase. */
enum { MC_BOX,MC_DOME,MC_SPHERE,MC_POD,MC_OCTAGON,MC_BEVEL,MC_PYRAMID,MC_DOCK,MC_RING0,MC_SHAPES=MC_RING0+12 };
#define MC_VERTEX_MAX 50
#define MC_FACE_MAX 48
typedef struct {unsigned char v[4],count,side;Vec3 normal;float plane;} MegaFace;
typedef struct {Vec3 v[MC_VERTEX_MAX];MegaFace f[MC_FACE_MAX];int vertices,faces;} MegaGeometry;
static MegaGeometry mc_geometry[MC_SHAPES];static int mc_geometry_ready;
static inline Vec3 mc_cross(Vec3 a,Vec3 b){return (Vec3){a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};}
static inline int mc_vertex(MegaGeometry *m,Vec3 p){int i=m->vertices++;m->v[i]=p;return i;}
static inline void mc_face(MegaGeometry *m,int a,int b,int c,int d){
 MegaFace *f=&m->f[m->faces++];f->v[0]=a;f->v[1]=b;f->v[2]=c;f->v[3]=d<0?c:d;f->count=d<0?3:4;
 Vec3 n=mc_cross(sub(m->v[b],m->v[a]),sub(m->v[c],m->v[a]));float size=length(n);
 n=mul(n,1.f/fmaxf(.00001f,size));float plane=dot(n,m->v[a]);
 if(plane<0){n=mul(n,-1);plane=-plane;}f->normal=n;f->plane=plane;
 float ax=fabsf(n.x),ay=fabsf(n.y),az=fabsf(n.z);
 f->side=az>=ax&&az>=ay?(n.z<0?0:1):ay>=ax?(n.y<0?2:3):(n.x<0?4:5);
}
static inline int mc_ring(MegaGeometry *m,float y,float r){
 int first=m->vertices;for(int i=0;i<8;i++){float a=i*.7853981634f;mc_vertex(m,(Vec3){cosf(a)*r,y,sinf(a)*r});}return first;
}
static inline void mc_join(MegaGeometry *m,int a,int b){
 for(int i=0;i<8;i++){int j=(i+1)&7;mc_face(m,a+i,a+j,b+j,b+i);}
}
static inline void mc_cap(MegaGeometry *m,int ring,float y){
 int center=mc_vertex(m,(Vec3){0,y,0});for(int i=0;i<8;i++)mc_face(m,center,ring+i,ring+((i+1)&7),-1);
}
static inline const MegaGeometry *mega_geometry_for(int shape){
 if(!mc_geometry_ready){
  mc_geometry_ready=1;
  for(int kind=MC_DOME;kind<MC_SHAPES;kind++){
   MegaGeometry *m=&mc_geometry[kind];
   if(kind>=MC_RING0){
    /* A real continuous annulus made from convex trapezoid sections. Each
       section's origin lies inside it, preserving outward plane clipping. */
    float a=(kind-MC_RING0)*6.2831853f/12,step=3.14159265f/12;
    float cx=cosf(a),cy=sinf(a);
    for(int z=-1;z<=1;z+=2)for(int k=0;k<4;k++){
     float radius=k<2?1.15f:.82f,theta=a+((k==0||k==3)?-step:step);
     mc_vertex(m,(Vec3){cosf(theta)*radius-cx,sinf(theta)*radius-cy,z});
    }
    mc_face(m,0,1,2,3);mc_face(m,4,7,6,5);
    for(int k=0;k<4;k++){int j=(k+1)&3;mc_face(m,k,j,j+4,k+4);}
   }else if(kind==MC_DOME){
    int a=mc_ring(m,-1,1),b=mc_ring(m,0,.8660254f),c=mc_ring(m,.7320508f,.5f),tip=mc_vertex(m,(Vec3){0,1,0});
    mc_join(m,a,b);mc_join(m,b,c);for(int i=0;i<8;i++)mc_face(m,c+i,c+((i+1)&7),tip,-1);mc_cap(m,a,-1);
   }else if(kind==MC_SPHERE){
    int bottom=mc_vertex(m,(Vec3){0,-1,0}),a=mc_ring(m,-.7071068f,.7071068f),b=mc_ring(m,0,1),c=mc_ring(m,.7071068f,.7071068f),tip=mc_vertex(m,(Vec3){0,1,0});
    for(int i=0;i<8;i++)mc_face(m,bottom,a+i,a+((i+1)&7),-1);
    mc_join(m,a,b);mc_join(m,b,c);for(int i=0;i<8;i++)mc_face(m,c+i,c+((i+1)&7),tip,-1);
   }else if(kind==MC_POD){
    /* Y-axis capsule is rotated into Z: wide pressurised habitat body. */
    int bottom=mc_vertex(m,(Vec3){0,-1,0}),a=mc_ring(m,-.62f,1),b=mc_ring(m,.62f,1),tip=mc_vertex(m,(Vec3){0,1,0});
    for(int i=0;i<8;i++){mc_face(m,bottom,a+i,a+((i+1)&7),-1);mc_face(m,b+i,b+((i+1)&7),tip,-1);}mc_join(m,a,b);
    for(int i=0;i<m->vertices;i++){Vec3 p=m->v[i];m->v[i]=(Vec3){p.x,p.z,p.y};}
    /* Recompute normals after the axis permutation. */
    MegaFace saved[MC_FACE_MAX];int count=m->faces;memcpy(saved,m->f,sizeof(saved));m->faces=0;
    for(int i=0;i<count;i++)mc_face(m,saved[i].v[0],saved[i].v[1],saved[i].v[2],saved[i].count==4?saved[i].v[3]:-1);
   }else if(kind==MC_BEVEL||kind==MC_DOCK){
    int a=mc_ring(m,-1,.72f),b=mc_ring(m,-.72f,1),c=mc_ring(m,.72f,1),e=mc_ring(m,1,.72f);
    mc_join(m,a,b);mc_join(m,b,c);mc_join(m,c,e);mc_cap(m,a,-1);mc_cap(m,e,1);
    if(kind==MC_DOCK){
     for(int i=0;i<m->vertices;i++){Vec3 p=m->v[i];m->v[i]=(Vec3){p.x,p.z,p.y};}
     MegaFace saved[MC_FACE_MAX];int count=m->faces;memcpy(saved,m->f,sizeof(saved));m->faces=0;
     for(int i=0;i<count;i++)mc_face(m,saved[i].v[0],saved[i].v[1],saved[i].v[2],saved[i].count==4?saved[i].v[3]:-1);
    }
   }else if(kind==MC_PYRAMID){
    int a=mc_ring(m,-1,1),tip=mc_vertex(m,(Vec3){0,1,0});
    for(int i=0;i<8;i++)mc_face(m,a+i,a+((i+1)&7),tip,-1);mc_cap(m,a,-1);
   }else {int a=mc_ring(m,-1,1),b=mc_ring(m,1,1);mc_join(m,a,b);mc_cap(m,a,-1);mc_cap(m,b,1);}
  }
 }
 return &mc_geometry[shape>MC_BOX&&shape<MC_SHAPES?shape:MC_BOX];
}
#endif
