//============================================================================================
//	Spirenkov Maxim, 2002
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	PtcData
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _PtcData_H_
#define _PtcData_H_

#include "..\common_h\vmodule_api.h"
#include "ptc.h"


#define PTCDATA_MAXSTEPS	32

class VDX8RENDER;


class PtcData
{
public:
	struct Triangle
	{
		long index;
		CVECTOR v[3];
		CVECTOR n;
	};

	struct DbgVertex
	{
		float x, y, z;
		dword c;
	};

//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	PtcData();
	virtual ~PtcData();

	//��������� ����
	bool Load(const char * path);
	//���������� ������� �������
	long FindNode(const CVECTOR & pos, float & y);
	//������� pos � to, ���������� ����� ���
	long Move(long curNode, const CVECTOR & to, CVECTOR & pos, long depth = 0);
	//�������� ������� � ����
	void GetNodeNormal(long curNode, CVECTOR & n);
	//����� ����������� ����
	bool FindPathDir(long curNode, const CVECTOR & cur, long toNode, const CVECTOR & to, long & node, CVECTOR & toPos);
	//����� ����������� � ������
	float Trace(const CVECTOR & s, const CVECTOR & d);
	//����� ���� ������������� �� ����
	void FindForce(long curNode, CVECTOR & force);
	//�������� �������� ����
	const char * GetMaterial(long curNode);

	//�������� ������������ ������������ ������ �������
	Triangle * GetTriangles(float x, float z, float sx, float sz, long & num);


	//���������� ���������� �����������
	bool isSlide;		//��������
	CVECTOR slideDir;	//����������� ����������
	bool isBearing;		//���� �� �����

	CVECTOR stepPos[PTCDATA_MAXSTEPS];
	long numSteps;

	CVECTOR middle;

	//���������� ���������
	void DebugDraw(VDX8RENDER * rs, float dltTime);


//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	//������� ������
#ifndef _XBOX
	void __declspec(dllexport) __cdecl SFLB_PotectionLoad();
#else
	void SFLB_PotectionLoad();
#endif
	//��������� ������ ����� �� ��������� ������������
	float FindHeight(long trgID, float x, float z);
	//����� ����������� ����
	bool FindPathDir(long step, long curNode, const CVECTOR & cur, long toNode, const CVECTOR & to, long & node, CVECTOR & pos);
	//����� ����� �� �����
	CVECTOR FindEdgePoint(const CVECTOR & vs, const CVECTOR & ve, const CVECTOR & cur, const CVECTOR & to);
	//��������� ����������� ������������ � ��������
	float Trace(PtcTriangle & trg, const CVECTOR & s, const CVECTOR & d);
	//�������� ����������� � �����
	void AddClTriangle(long i);

//private:
public:
	//���� ������
	void * data;

	//���������
	PtcTriangle * triangle;	//������������ ���������
	long numTriangles;		//���������� ������������� ���������
	
	PtcVertex * vertex;		//������� ���������
	long numVerteces;		//���������� ������ ���������
	
	PtcNormal * normal;		//������� ������������� ���������
	long numNormals;		//���������� ��������� ���������

	PtcMaterials * materials;//���������

	//������ ��� ���������
	CVECTOR min, max;		//�� ��� ������������
	
	PtcMap * map;			//����� �������� ������
	long l, w;				//������ ����� �������� ������
	float ls, ws;			//������ ������ ��������
	
	word * indeces;			//�������
	long numIndeces;		//���������� ��������

	//������ ��� ���������� �����
	byte * table;			//������� �����������
	long lineSize;			//������ ������

	//������������ ����� ��������
	Triangle * ctriangle;
	long numClTriangles;
	long maxClTriangles;

	//���������� ����������
	DbgVertex * dbgTriangles;
	DbgVertex * dbgEdges;
};

#endif

