#ifndef _GI_EDITOR_H
#define _GI_EDITOR_H

#include "editor_defines.h"

class XINTERFACE;
class CXI_IMAGE;
class GIEditorList;
class CINODE;
class VDX9RENDER;

class GIEditor : public GIEditorObject
{
public:
	GIEditor(XINTERFACE* pInterface);
	~GIEditor();

	void Release();

	void Render();
	bool ProcessControl();

	bool IsShowMode();
	void MakeShowMode(bool bShow);

	void SetEditNode(CINODE* pNode);

	void ReCreate();
	void AddNode(CINODE* pNode);
	void DelNode(CINODE* pNode);

	void DrawSizeBox();

protected:
	bool m_bShowMode;
	CINODE* m_pEditableNode;
	float m_fLastKeyPressTime;

	CXI_IMAGE* m_pImageBack;

	GIEditorList* m_pNodeList;
	bool m_bSubNameOn;
	GIEditorList* m_pSubNameList;

public:
	VDX9RENDER* m_pRS;
	XINTERFACE* m_pGIOwner;

	void ChangeNodeName();
	void ChangeSubNodeName();
};

#endif
