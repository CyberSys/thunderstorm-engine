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


#include "..\common_h\vmodule_api.h"
#include "..\common_h\dx8render.h"
#include "..\common_h\model.h"



#define BLOTS_NTRGS		32		//������������� � 1 �����
#define BLOTS_MAX		256		//����� �����
#define BLOTS_TIME		120.0f	//����� ����� �����
#define BLOTS_DIST		150.0f	//��������� ��������� � ������


class Blots : public ENTITY  
{

#pragma pack(push, 1)

	struct Blot
	{
		byte isUsed;		//������������� �� ���
		byte lastAlpha;		//��������� �������� �����
		word numTrgs;		//���������� �������������
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
	dword _cdecl ProcessMessage(MESSAGE & message);
	//������
	void Realize(dword delta_time);

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
	
	VDX8RENDER * rs;	
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

