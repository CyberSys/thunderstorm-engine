//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	WdmCameraStdCtrl
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _WdmCameraStdCtrl_H_
#define _WdmCameraStdCtrl_H_

#include "WdmCamera.h"

class WdmCameraStdCtrl : public WdmCamera
{
	//--------------------------------------------------------------------------------------------
	//���������������, ������������������
	//--------------------------------------------------------------------------------------------
public:
	WdmCameraStdCtrl();
	virtual ~WdmCameraStdCtrl();

	//--------------------------------------------------------------------------------------------
	//������� ������������ ������� ������
	//--------------------------------------------------------------------------------------------
public:
	void CtrlProcess(float dltTime) override;
	float MoveLeftRight(float dltTime) override;
	float MoveUpDown(float dltTime) override;
	float RotLeftRight(float dltTime) override;
	float ZoomInOut(float dltTime) override;
	bool CurrentFreeMode() override;
	bool GetHightHeight(float& height) override;

	//--------------------------------------------------------------------------------------------
	//������������
	//--------------------------------------------------------------------------------------------
private:
	bool GetCurFreeMode();
private:
	bool isFree;
	float mdx, mdy, mzoom;
	bool lastFreeMode;
};

#endif
