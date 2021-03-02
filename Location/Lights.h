//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	Lights
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _Lights_H_
#define _Lights_H_

#include "..\common_h\vmodule_api.h"
#include "..\common_h\matrix.h"
#include "..\common_h\dx8render.h"
#include "..\common_h\collide.h"
#include "LocationEffects.h"
#include "..\common_h\templates\array.h"

class Lights : public ENTITY  
{
	//�������� ��������� ���������
	struct LightType
	{
		char * name;
		D3DLIGHT8 dxLight;
		D3DCOLORVALUE color;
		float flicker;
		float flickerSlow;
		float freq;
		float freqSlow;
		float p;
		float pSlow;
		float coronaRange;
		float coronaRange2;
		float invCoronaRange;
		float coronaSize;
		long corona;
	};

	//��������
	struct Light
	{
		D3DCOLORVALUE color;	//������� ���� ���������
		D3DVECTOR pos;			//������� ���������
		float time;				//����� � ������� ���������� ��������� ��������� �������������
		float timeSlow;			//����� � ������� ���������� ��������� ��������������� �������������
		float itens;			//��������� �������������
		float itensSlow;		//����������� ��������������� �������������
		float itensDlt;			//�������� ��������������� �������������
		float i;				//�������������� �������������
		long type;				//������ ���� ���������
		float corona;			//������������ ������
	};

	//�����������(��������������) ��������
	struct MovingLight
	{
		long id;
		long light;
	};

	struct Vertex
	{
		CVECTOR pos;
		dword color;
		float u, v;
	};


//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	Lights();
	virtual ~Lights();

	//�������������
	bool Init();
	//����������
	void Execute(dword delta_time);
	//��������� �����
	void Realize(dword delta_time);

	//����� ������ ���������
	long FindLight(const char * name);
	//�������� �������� � �������
	void AddLight(long index, const CVECTOR & pos);
	//�������� �������� �������
	bool AddLampModel(const ENTITY_ID & lampModel);
	//
	void DelAllLights();

	//�������� ���������� ��������
	long AddMovingLight(const char* type, const CVECTOR& pos);
	//��������� ���������� �������� � ����� �������
	void UpdateMovingLight(long id, const CVECTOR& pos);
	//������� ���������� ��������
	void DelMovingLight(long id);

	//���������� ��� ��������� ��������� ���������
	void SetCharacterLights(const CVECTOR & pos);
	//��������� ������������� ��� ��������� ��������� ���������
	void DelCharacterLights();
	//���������� �� �� ��������� ��� � ��� ���������� �������
	void SetCharacterLights();

	//�������� ���� ����������
	void UpdateLightTypes(long i);

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	VDX8RENDER * rs;
	COLLIDE * collide;
	//������������� ��������� ���������
	struct{bool set; long light;} lt[8];
	//���� ���������� ���������
	LightType * types;
	long numTypes;
	long maxTypes;
	//������������ ��������� ���������
	Light * lights;
	long numLights;
	long maxLights;
	long lighter_code;

	//���������� ��������� �����
	array<MovingLight> aMovingLight;

	//�������� �������
	ENTITY_ID lampModels[16];
	long numLampModels;

	Vertex buf[6*1];
};

#endif

