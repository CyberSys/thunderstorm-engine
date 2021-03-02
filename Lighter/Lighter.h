//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	Lighter
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _Lighter_H_
#define _Lighter_H_

#include "LightProcessor.h"


class Lighter : public ENTITY  
{
//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	Lighter();
	virtual ~Lighter();

	//�������������
	bool Init();
	//����������
	void Execute(uint32_t delta_time);
	void Realize(uint32_t delta_time);
	//���������
	uint32_t _cdecl ProcessMessage(MESSAGE & message);

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	void MsgAddModel(MESSAGE & message);
	void MsgModelsPath(MESSAGE & message);
	void MsgLightPath(MESSAGE & message);
	void MsgAddLight(MESSAGE & message);
	void PreparingData();

private:

	VDX9RENDER * rs;

	LGeometry geometry;
	OctTree octTree;
	Window window;
	Lights lights;
	LightProcessor lightProcessor;

	long initCounter;
	float waitChange;
	bool isInited, autoTrace, autoSmooth;
	
};

#endif

