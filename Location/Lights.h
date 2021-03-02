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

class Lights : public Entity
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
	void ProcessStage(Stage stage, uint32_t delta) override
	{
		switch (stage)
		{
		case Stage::EXECUTE:
			Execute(delta); break;
		case Stage::REALIZE:
			Realize(delta); break;
			/*case Stage::lost_render:
				LostRender(delta); break;
			case Stage::restore_render:
				RestoreRender(delta); break;*/
		}
	}

	//����� ������ ���������
	long FindLight(const char * name);
	//�������� �������� � �������
	void AddLight(long index, const CVECTOR & pos);
	//�������� �������� �������
	bool AddLampModel(const entid_t  lampModel);
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
	std::vector<LightType> types;
	long numTypes;
	long maxTypes;
	//������������ ��������� ���������
	std::vector<Light> lights;
	long numLights;
	long maxLights;
	long lighter_code;

	//���������� ��������� �����
	std::vector<MovingLight> aMovingLight;

	//�������� �������
	entid_t lampModels[16];
	long numLampModels;

	Vertex buf[6*1];

	//��������������� ������ ���������� ��� ���������� �������
	std::vector<lt_elem> aLightsDstSort;
};

#endif

