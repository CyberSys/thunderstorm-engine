#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "../common_h/vmodule_api.h"
#include "../common_h/dx9render.h"
#include "../common_h/collide.h"
#include "../common_h/model.h"

class SHADOW : public ENTITY
{
	VDX9RENDER * rs;
	COLLIDE *col;
	void FindPlanes(const CMatrix &view, const CMatrix &proj);
	PLANE planes[6];
	ENTITY_ID entity;
	void Smooth();
	float shading;
	unsigned long blendValue;
public:
	#define SHADOW_FVF (D3DFVF_XYZ|D3DFVF_TEXTUREFORMAT2|D3DFVF_TEX1)
	struct SHADOW_VERTEX
	{
		CVECTOR pos;
		float tu,tv;
	};
	SHADOW();
	~SHADOW();
	bool Init();
	void Realize(uint32_t Delta_Time);
	uint32_t ProcessMessage(MESSAGE &message);

	void LostRender() override;
	void RestoreRender() override;
};


//API_MODULE_START("SHADOW")
//	CREATE_CLASS(0,SHADOW)
//API_MODULE_END


#endif