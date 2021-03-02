//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	Sharks
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _Sharks_H_
#define _Sharks_H_

#include "..\common_h\vmodule_api.h"
#include "..\common_h\dx8render.h"
#include "..\common_h\matrix.h"
#include "..\common_h\ship_base.h"
#include "..\common_h\sea_base.h"
#include "..\common_h\island_base.h"
#include "..\common_h\animation.h"

class Sharks : public ENTITY  
{
	struct Vertex
	{
		CVECTOR pos;
		dword color;
		float u, v;
	};

	class Shark : public AnimationEventListener
	{
		static word indeces[];
	public:
		Shark();
		~Shark();

		bool Init(float vp_x, float vp_z, bool isLoadModel = true);

		void Reset(float cam_x, float cam_z);
		void Repulsion(Shark & shr);
		void ShipApply(float x, float z, float r2);
		void Coordination(float cam_x, float cam_z, float dltTime, SEA_BASE * sb, ISLAND_BASE * ib);
		void IslandCollision(ISLAND_BASE * ib, long numPnt, float rad, float frc);
		virtual void Event(Animation * animation, long index, long eventID, AnimationEvent event);
		long GenerateTrack(word * inds, Vertex * vrt, word base, SEA_BASE * sb);

		//����� ����������
		CVECTOR pos;		//������� ����� ����������
		CVECTOR vel;		//����������� �����������
		CVECTOR force;		//���� ����������� ��������
		float yDir;			//��������� � �������� ��� ����������
		float dirTime;		//����� �� ��������� �����
		float accs;			//��������� �����
		CVECTOR rForce;		//��������� ��������� ����
		float rTime;
		//�����
		CVECTOR fforce;		//���� ����������� ��������
		CVECTOR spos;		//������� ��������
		CVECTOR angs;		//���� ��������
		float turn;			//�������� ��������� ����
		float imspd;		//�������� ����������
		float speed;		//�������� �����������
		float shipY;		//����������� �������
		float vBase;		//�������� ����� �����
		ENTITY_ID model;	//�������� �����		
		//��������
		float aniTime;		//����� ������������ ������� ��������
		float jumpTime;		//����� �� ���������� ������������
		bool speedUp;
	};

	friend Shark;

	struct Periscope
	{
		CVECTOR pos;
		float ay;
		float time;
		ENTITY_ID model;
	};

//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	Sharks();
	virtual ~Sharks();

	//�������������
	bool Init();
	//����������
	void Execute(dword delta_time);
	void Realize(dword delta_time);


//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	bool LoadPeriscopeModel();

private:
	VDX8RENDER * rs;
	Shark shark[6];
	long numShakes;
	Periscope periscope;
	float waitPTime;
	CVECTOR camPos;
	long shipcode;
	ENTITY_ID sea;
	ENTITY_ID island;
	long trackTx;
	word indeces[7*10*3];
	Vertex vrt[7*10];
};

#endif

