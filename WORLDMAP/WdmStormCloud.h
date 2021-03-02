//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	WdmStormCloud
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _WdmStormCloud_H_
#define _WdmStormCloud_H_

#include "WdmCloud.h"

class WdmStormCloud : public WdmCloud
{
	struct RainVertex
	{
		float x, y, z;
		float tu, tv;
	};

//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	WdmStormCloud();
	virtual ~WdmStormCloud();

	//�������
	virtual void Update(float dltTime);

	//���������
	virtual void PRender(VDX8RENDER * rs);
	virtual void LRender(VDX8RENDER * rs);

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	virtual void BuildCloud(long n);
	void FillRects();


	//��������� ������
	long curLightning;
	float lightningWaitTime;
	float lightningTime;
	float flashTime;
	long lastColor;
	long lightningColor;

	//������
	long rainTexture;
	float curU, curV;
	CVECTOR rainpos[2048];			//������� �� ��������
	static RainVertex rain[4096];	//������� ������ ��� ���������
};

#endif

