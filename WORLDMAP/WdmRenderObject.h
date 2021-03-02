//============================================================================================
//	Spirenkov Maxim, 2001
//--------------------------------------------------------------------------------------------
//	Sea Dogs II
//--------------------------------------------------------------------------------------------
//	WdmRenderObject
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _WdmRenderObject_H_
#define _WdmRenderObject_H_


#include "../Common_h/dx9render.h"

class WdmRenderObject
{
//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	WdmRenderObject();
	virtual ~WdmRenderObject();

	//�������
	virtual void Update(float dltTime){};
	//���������
	virtual void PRender(VDX9RENDER * rs){};
	virtual void MRender(VDX9RENDER * rs){};
	virtual void LRender(VDX9RENDER * rs){};

	static void DeleteAllObjects();

	bool killMe;
	bool isEnablePause;


//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	WdmRenderObject * prevObject;
	WdmRenderObject * nextObject;
	static WdmRenderObject * firstObject;
};

#endif

