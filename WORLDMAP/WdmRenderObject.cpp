//============================================================================================
//	Spirenkov Maxim, 2001
//--------------------------------------------------------------------------------------------
//	Sea Dogs II
//--------------------------------------------------------------------------------------------
//	WdmRenderObject
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#include "WdmRenderObject.h"

WdmRenderObject * WdmRenderObject::firstObject = nullptr;

//============================================================================================
//���������������, ������������������
//============================================================================================

WdmRenderObject::WdmRenderObject()
{
	killMe = false;
	isEnablePause = true;
	if(firstObject)
	{
		for(prevObject = firstObject; prevObject->nextObject; prevObject = prevObject->nextObject);
		prevObject->nextObject = this;
		nextObject = nullptr;
	}else{
		firstObject = this;
		prevObject = nullptr;
		nextObject = nullptr;
	}
}

WdmRenderObject::~WdmRenderObject()
{
	if(prevObject == nullptr)
	{
		Assert(WdmRenderObject::firstObject == this);
		WdmRenderObject::firstObject = nextObject;
		if(nextObject) nextObject->prevObject = nullptr;
	}else{
		Assert(WdmRenderObject::firstObject != this);
		prevObject->nextObject = nextObject;
		if(nextObject) nextObject->prevObject = prevObject;
	}
}

void WdmRenderObject::DeleteAllObjects()
{
	while(firstObject) delete firstObject;
}