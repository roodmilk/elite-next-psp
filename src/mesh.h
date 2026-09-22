#ifndef MESH_H
#define MESH_H
typedef struct { float x,y,z; } Vec3;
typedef struct { int a,b,c; Vec3 normal; } MeshTri;
typedef struct { const char *name; int vertices,triangles; const Vec3 *v; const MeshTri *t; } Mesh;
extern const Mesh meshes[];
extern const int mesh_count;
#endif
