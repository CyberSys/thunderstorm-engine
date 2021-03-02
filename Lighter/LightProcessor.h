//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	LightProcessor
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _LightProcessor_H_
#define _LightProcessor_H_

#include "LGeometry.h"
#include "Window.h"
#include "Lights.h"
#include "OctTree.h"

class VDX8RENDER;


class LightProcessor  
{
//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	LightProcessor();
	virtual ~LightProcessor();
	void SetParams(LGeometry * g, Window * win, Lights * lit, OctTree * ot, VDX8RENDER * _rs);
	void UpdateLightsParam();
	
	//��������� ��� ����������
	void Process();

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	//��������� ���������
	void CalcShadows();
	//�������� �����������
	void SmoothShadows();
	//�������� ���������
	void BlurLight();
	//��������� ���������
	void CalcLights(long lit = -1, bool isCos = true, bool isAtt = true, bool isSdw = true);
	//������������ ��������� � ������������ �� �������
	void ApplyTriangleShadows(Triangle & t);


private:
	LGeometry * geometry;
	Window * window;
	Lights * lights;
	VDX8RENDER * rs;
	OctTree * octtree;

	long shadowTriangle;
	long smoothVertex;
	long blurVertex;
};

#endif

