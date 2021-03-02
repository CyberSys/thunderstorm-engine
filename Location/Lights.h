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

#include "../Common_h/vmodule_api.h"
#include "../Common_h/Matrix.h"
#include "../Common_h/dx9render.h"
#include "../Common_h/collide.h"
#include <vector>

class Lights : public ENTITY
{
	//�������� ��������� ���������
	struct LightType
	{
		char * name;
		D3DLIGHT9 dxLight;
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
		uint32_t color;
		float u, v;
	};

	//��� ���������� ��� �� ���������
	struct lt_elem
	{
		long idx;
		float dst;
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
	void Execute(uint32_t delta_time);
	//��������� �����
	void Realize(uint32_t delta_time);

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
	void SetCharacterLights(const CVECTOR * const pos = nullptr);

	//��������� ������������� ��� ��������� ��������� ���������
	void DelCharacterLights();

	//�������� ���� ����������
	void UpdateLightTypes(long i);

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	VDX9RENDER * rs;
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
	std::vector<MovingLight> aMovingLight;

	//�������� �������
	ENTITY_ID lampModels[16];
	long numLampModels;

	Vertex buf[6*1];

	//��������������� ������ ���������� ��� ���������� �������
	std::vector<lt_elem> aLightsDstSort;
};

#endif

