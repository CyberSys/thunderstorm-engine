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


#include "..\common_h\dx8render.h"

#include "WdmObjects.h"

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
	virtual void PRender(VDX8RENDER * rs){};
	virtual void MRender(VDX8RENDER * rs){};
	virtual void LRender(VDX8RENDER * rs){};

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

