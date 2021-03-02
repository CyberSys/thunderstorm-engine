//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	Grass
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _Grass_H_
#define _Grass_H_

#include "..\common_h\vmodule_api.h"
#include "..\common_h\dx8render.h"
#include "grs.h"
#include "supervisor.h"

#define MSG_GRASS_LOAD_DATA			40666
#define MSG_GRASS_SET_TEXTURE		41666
#define MSG_GRASS_SET_PARAM			42666

class GEOS;

class Character;

class Grass : public ENTITY
{
#pragma pack(push, 1)

	struct Vertex
	{
		float x, y, z;
		dword data;
		dword offset;
		float wx, wz;		
		float alpha;
	};

	struct VSConstant
	{
		inline VSConstant()
		{
			x = y = z = 0.0f;
			w = 1.0f;
		};
		inline VSConstant(float _x, float _y, float _z, float _w)
		{
			x = _x; y = _y; z = _z; w = _w;
		};
		float x, y, z, w;
	};

#pragma pack(pop)

	struct AngleInfo
	{
		float dx, dz;			//�����������
		float cs;				//������� ���� ����� ������������ � ����������
	};

	struct GRSMapElementEx
	{
		float x, y, z;			//������� �������� � ����
		union
		{
			struct
			{				
				byte frame;		//����
				byte h;			//������
				byte w;			//������
				byte ang;		//���� ��������
			};
			dword data;
		};
	};

	enum RenderQuality
	{
		rq_full = 0,
		rq_middle = 1,
		rq_low = 2,
		rq_off = 3,
	};


public:
	struct CharacterPos
	{
		CVECTOR pos;			//������� �������
		CVECTOR lastPos;		//����������� �������
		Character * chr;		//��������� �� ���������
		long useCounter;		//������� ������� �� �����
	};

//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	Grass();
	virtual ~Grass();

	//�������������
	bool Init();
	//������
	void Execute(dword delta_time);
	void Realize(dword delta_time);
	//
	dword _cdecl ProcessMessage(MESSAGE &message);
	
	//��������� ������ ��� �����
	bool LoadData(const char * patchName);
	//���������� ��������
	void SetTexture(const char * texName);

	CharacterPos characters[MAX_CHARACTERS];
	long numCharacters;

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	//������ �����
	void RenderBlock(const CVECTOR & camPos, PLANE * plane, long numPlanes, long mx, long mz);
	//�������� �� ��������� �����
	bool VisibleTest(const PLANE * plane, long numPlanes, const CVECTOR & min, const CVECTOR & max);
	//������ �����
	void RenderBlock(GRSMiniMapElement & mme, float kLod);
	//���������� ���������� ������
	void DrawBuffer();
	//�������� ����
	static long GetColor(CVECTOR color);

private:
	//������ �������
	VDX8RENDER * rs;
	//������
	long vb, ib;
	long numPoints;
	//��������
	long texture;

	//���������
	GRSMiniMapElement * miniMap;
	//������� ���������
	long miniX, miniZ;
	//��������� ������� �����
	float startX, startZ;
	//����� � ��������
	GRSMapElementEx * block;
	long numElements;
	//������� ��������� ��� ��������� �����
	AngleInfo angInfo[16];
	//������� ���� �������
	float phase[7];
	//��������� �������� ��������� ���������
	CVECTOR lDir;		//����������� ���������
	CVECTOR lColor;		//���� ���������
	CVECTOR aColor;		//���� ��������� ���������

	long blockChrs[32];	//������� ���������� �������������� ������
	long numBlockChr;	//���������� ���������� �������������� ������

	float lodSelect;	//���������� ��������� ������ ���� (kLod = kLod^lodSelect)
	float winForce;		//���������� �������� ����� 0..1
	CVECTOR winDir;		//��������������� ������������ �����
	
	Vertex * vbuffer;	//��������� �����	

	RenderQuality quality;	//�������� ���������

	float cosPh1, sinPh2, sinPh5, sinPh6, winPow, winF10, kAmpWF, kDirWF, kLitWF;
	float windAng;
	long initForce;

	VSConstant consts[42];

	char textureName[64];

	float m_fDataScale;
	float m_fMaxWidth;
	float m_fMaxHeight;
	float m_fMinVisibleDist;
	float m_fMaxVisibleDist;
	float m_fMinGrassLod;
	
	// boal ��������� �����
	long isGrassLightsOn;
};

#endif
