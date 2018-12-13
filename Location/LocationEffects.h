//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	LocationEffects
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _LocationEffects_H_
#define _LocationEffects_H_


#include "../Common_h/vmodule_api.h"
#include "../Common_h/Matrix.h"

class VDX9RENDER;

#define LFX_SPLASHES_SECT	16

class LocationEffects : public ENTITY  
{

#pragma pack(push, 1)

	struct Vertex
	{
		CVECTOR pos;
		uint32_t color;
		float u, v;
	};

#pragma pack(pop)

	struct Particle
	{
		CVECTOR pos;
		float angle;
		float size;
		float alpha;
	};

	struct ParticleEx : public Particle
	{
		uint32_t color;
		float frame;
	};

	struct ParticleSplash : public Particle
	{
		CVECTOR dir;
		float dAng;
	};

	struct ChrSplash
	{		
		float time;
		float kTime;
		CVECTOR pos;
		ParticleSplash prt[64];
	};

	struct ParticleFly : public ParticleEx
	{
		float ax, ay;
		float kx, ky;
		float a, k;
	};

	struct LampFlys
	{
		CVECTOR pos;
		float radius;
		long start;
		long num;
	};

	struct ParticleSG : public Particle
	{
		CVECTOR spd;		
		float dang;		
		float time;
		float ktime;
	};

//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	LocationEffects();
	virtual ~LocationEffects();


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
	void DrawParticles(void * prts, long num, long size, long texture, const char * tech, bool isEx = false, long numU = 0);


private:
	VDX9RENDER * rs;


	//---------------------------------------------------
	//������ �� ���������
	//---------------------------------------------------
	void CreateSplash(const CVECTOR & pos, float power);
	void ProcessedChrSplash(float dltTime);

	ChrSplash chrSplash[4];
	long chrSplashRefCounter;
	long splashesTxt;

	//---------------------------------------------------
	//���� � �������
	//---------------------------------------------------

	void AddLampFlys(CVECTOR & pos);
	void ProcessedFlys(float dltTime);

	std::vector<LampFlys> flys;
	long numFlys;
	long maxFlys;
	std::vector<ParticleFly> fly;
	long numFly;
	long flyTex;

	//---------------------------------------------------
	//�������� �������
	//---------------------------------------------------
	void SGInited();
	void SGRelease();
	void SGEnvPrt(const CVECTOR & pos, const CVECTOR & ndir);
	void SGBldPrt(const CVECTOR & pos, const CVECTOR & ndir);
	void SGFirePrt(const CVECTOR & pos, const CVECTOR & ndir);
	void ProcessedShotgun(float dltTime);

	ParticleSG smoke[64];
	bool isShgInited;
	long numSmoke;
	long texSmoke;
	ParticleSG flinders[256];
	long numFlinders;
	long texFlinders;
	ParticleSG blood[256];
	long numBlood;
	long texBlood;
	long texHor;

	Vertex buffer[256*6];
};

#endif

