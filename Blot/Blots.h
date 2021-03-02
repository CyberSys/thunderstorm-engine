//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	Blots
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _Blots_H_
#define _Blots_H_


#include "../Common_h/vmodule_api.h"
#include "../Common_h/dx9render.h"
#include "../Common_h/model.h"


#define BLOTS_NTRGS		32		//������������� � 1 �����
#define BLOTS_MAX		256		//����� �����
#define BLOTS_TIME		120.0f	//����� ����� �����
#define BLOTS_DIST		150.0f	//��������� ��������� � ������


class Blots : public ENTITY  
{

#pragma pack(push, 1)

	struct Blot
	{
		uint8_t isUsed;		//������������� �� ���
		uint8_t lastAlpha;		//��������� �������� �����
		uint16_t numTrgs;		//���������� �������������
		float liveTime;		//����� �����
		long startIndex;	//��������� ������ � �������
		long rnd;
		CVECTOR pos, dir;
	};

	struct Vertex
	{
		CVECTOR pos;
		long c;
		float u, v;
	};

#pragma pack(pop)

//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	Blots();
	virtual ~Blots();

	//�������������
	bool Init();
	//���������
	uint32_t ProcessMessage(MESSAGE & message);
	//������
	void Realize(uint32_t delta_time);

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	//�������� ���������
	void Hit(MESSAGE & message);
	//�������� �����
	void AddBlot(long i, long rnd, const CVECTOR & pos, const CVECTOR & dir, float time);
	//
	void SetNodesCollision(NODE * n, bool isSet);
	//��������� ��������� �����
	void SaveBlot(long i);
	//��������� ��������� �����
	void LoadBlot(long i);

private:
	
	VDX9RENDER * rs;	
	ENTITY_ID model;
	long textureID;
	ATTRIBUTES * blotsInfo;
	ATTRIBUTES * pCharAttributeRoot;

	Blot blot[BLOTS_MAX];
	Vertex vrt[3*BLOTS_NTRGS*BLOTS_MAX];
	long useVrt;

	long updateBlot;

	static bool AddPolygon(const CVECTOR * v, long nv);
	static CVECTOR clipTriangles[3*BLOTS_NTRGS];
	static long numClipTriangles;
	static CVECTOR dir, normal;
};

#endif

