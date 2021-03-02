#ifndef InterfaceBaseManager_h
#define InterfaceBaseManager_h

#include "..\..\common_h\dx9render.h"
#include "..\..\common_h\defines.h"

class BIImageRender;
class BI_ManagerBase;

class BI_MousePointerBase
{
public:
	BI_MousePointerBase() = default;
	virtual ~BI_MousePointerBase() = default;

	virtual void Update() = 0;
};

class BI_ManagerNodeBase
{
public:
	BI_ManagerNodeBase(){}
	virtual ~BI_ManagerNodeBase(){}

	virtual void Update() = 0;

	virtual long Event(const char* pcEventName) = 0;

	virtual bool IsPointInside(float fX, float fY) = 0;
	virtual bool IsMouseClickReaction() = 0;
	virtual bool IsMouseMoveReaction() = 0;
};

class BI_ManagerBase : public ENTITY
{
public:
	BI_ManagerBase(){}
	~BI_ManagerBase(){}

	VDX9RENDER* GetDX9Render() {return m_pRS;}
	BIImageRender* GetImageRender() {return m_pImgRender;}
	BI_MousePointerBase* GetMouse() {return m_pMouse;}

	virtual BI_ManagerNodeBase* CreateImageNode(const char* texture, const FRECT& uv, const RECT& pos, uint32_t color, long nPrioritet) = 0;
	virtual BI_ManagerNodeBase* CreateStringNode(const char* text, const char* font, uint32_t color, float scale, const RECT& pos, long nHAlign, long nVAlign, long prioritet) = 0;
	virtual void DeleteNode(BI_ManagerNodeBase* pNod) = 0;

protected:
	VDX9RENDER *m_pRS;
	BIImageRender *m_pImgRender;
	BI_MousePointerBase *m_pMouse;
};

#endif
