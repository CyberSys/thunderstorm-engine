#ifndef _ModelWraper_H_
#define _ModelWraper_H_

#include "..\common_h\vmodule_api.h"
#include "..\common_h\geometry.h"

class Lights;

class LocModelRealizer : public ENTITY
{
	ENTITY_ID eid_model;
	Lights* lights;
	VDX8RENDER* rs;
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
	void Execute(dword delta_time);
	void Realize(dword delta_time);
	//���������
	dword _cdecl ProcessMessage(MESSAGE & message);
};

#endif
