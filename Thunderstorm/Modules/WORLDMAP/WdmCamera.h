//============================================================================================
//	Spirenkov Maxim, 2001
//--------------------------------------------------------------------------------------------
//	Sea Dogs II
//--------------------------------------------------------------------------------------------
//	WdmCamera
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _WdmCamera_H_
#define _WdmCamera_H_

#include "dx9render.h"

class WdmCamera {
  //--------------------------------------------------------------------------------------------
  //���������������, ������������������
  //--------------------------------------------------------------------------------------------
public:
  WdmCamera();
  virtual ~WdmCamera();

  //--------------------------------------------------------------------------------------------
  //����������� ������
  //--------------------------------------------------------------------------------------------
public:
  void Init(float defAy, float defHeight = -1.0f);
  void Move(float dltTime, VDX9RENDER* rs);

  //--------------------------------------------------------------------------------------------
  //������� ��� ����������
  //--------------------------------------------------------------------------------------------
protected:
  virtual void CtrlProcess(float dltTime) = 0;
  virtual float MoveLeftRight(float dltTime) = 0;
  virtual float MoveUpDown(float dltTime) = 0;
  virtual float RotLeftRight(float dltTime) = 0;
  virtual float ZoomInOut(float dltTime) = 0;
  virtual bool CurrentFreeMode() = 0;
  virtual bool GetHightHeight(float& height) = 0;

  //--------------------------------------------------------------------------------------------
  //
  //--------------------------------------------------------------------------------------------
public:
  CVECTOR pos;
  CVECTOR ang;
  float realHeight;
  float hgh;

  bool lock;

  //--------------------------------------------------------------------------------------------
  //������������
  //--------------------------------------------------------------------------------------------
private:
  void CheckRange();

private:
  float scrollSpeed;
  float rotateSpeed;
  float elasticAy;

  bool isFreeMode;
  float lastAng;
  CVECTOR bndPos;
  float kBlendPos;

  bool isInit;
};

#endif
