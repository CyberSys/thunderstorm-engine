#ifndef __GEOMETRY_R__H__
#define __GEOMETRY_R__H__

#include "vmodule_api.h"
#include "dx9render.h"
#include <geometry.h>

//-------------------------------------------------------------------
//animated vertices
//-------------------------------------------------------------------
class GEOMETRY final : public VGEOMETRY
{
	VDX9RENDER * RenderService;
public:
	GEOMETRY();
	bool Init();
	bool LoadState(ENTITY_STATE * state);
	GEOS * CreateGeometry(const char * file_name,const char * light_file_name, long flags, const char *lmPath);
	void DeleteGeometry(GEOS *);
	ANIMATION *LoadAnimation(const char *anim);
	void SetTechnique(const char *name);
	void SetVBConvertFunc(VERTEX_TRANSFORM _transform_func);
	ANIMATION_VB GetAnimationVBDesc(long avb);

	const char *GetTexturePath();
	void SetTexturePath(const char *);

	void SetCausticMode(bool bSet = false);
};


class GEOM_SERVICE_R final : public GEOM_SERVICE 
{
	static IDirect3DVertexDeclaration9 * vertexDecl_;

	VDX9RENDER * RenderService;
	GEOS::ID CurentIndexBuffer;
	GEOS::ID CurentVertexBuffer;
	uint32_t	 CurentVertexBufferSize;
	bool	bCaustic;

public:
	void SetRenderService(VDX9RENDER * render_service);

	HANDLE OpenFile(const char *fname);
	int FileSize(HANDLE file);
	void ReadFile(HANDLE file, void *data, long bytes);
	void CloseFile(HANDLE file);
	void *malloc(long bytes);
	void free(void *ptr);

	GEOS::ID CreateTexture(const char *fname);
	void SetMaterial(const GEOS::MATERIAL &mt);
	void ReleaseTexture(GEOS::ID tex);

	GEOS::ID CreateVertexBuffer(long type, long size);
	void *LockVertexBuffer(GEOS::ID vb);
	void UnlockVertexBuffer(GEOS::ID vb);
	void ReleaseVertexBuffer(GEOS::ID vb);

	GEOS::ID CreateIndexBuffer(long size);
	void *LockIndexBuffer(GEOS::ID ib);
	void UnlockIndexBuffer(GEOS::ID ib);
	void ReleaseIndexBuffer(GEOS::ID ib);

	void SetIndexBuffer(GEOS::ID ibuff);
	void SetVertexBuffer(long vsize, GEOS::ID vbuff);
	void DrawIndexedPrimitive(long minv, long numv, long vrtsize, long startidx, long numtrg);

	GEOS::ID CreateLight(const GEOS::LIGHT);
	void ActivateLight(GEOS::ID n);

	void SetCausticMode(bool bSet = false);
};

//API_SERVICE_START("geometry service")
//	DECLARE_MAIN_SERVICE(GEOMETRY)	
//API_SERVICE_END(GEOMETRY)

#endif