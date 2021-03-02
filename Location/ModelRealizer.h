#ifndef _ModelWraper_H_
#define _ModelWraper_H_

#include "..\common_h\vmodule_api.h"
#include "..\common_h\geometry.h"

class Lights;

class LocModelRealizer : public ENTITY
{
	ENTITY_ID eid_model;
	Lights* lights;
	VDX9RENDER* rs;
	VGEOMETRY* gs;
	bool bShow;

//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	LocModelRealizer();
	~LocModelRealizer();

	//�������������
	bool Init();
	//����������
	void Execute(uint32_t delta_time);
	void Realize(uint32_t delta_time);
	//���������
	uint32_t _cdecl ProcessMessage(MESSAGE & message);
};

#endif
