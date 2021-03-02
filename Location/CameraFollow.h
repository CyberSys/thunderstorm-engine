//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	CameraFollow
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _CameraFollow_H_
#define _CameraFollow_H_

#include "..\common_h\vmodule_api.h"
#include "..\common_h\matrix.h"
#include "..\common_h\model.h"

class LocationCamera;


class CameraFollow  
{
	struct EdgeInfo
	{
		CVECTOR n;	//������� � ���������
		float d;	//��������� �� ���������
		CVECTOR s;	//������ �����
		CVECTOR e;	//����� �����
	};


//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	CameraFollow();
	virtual ~CameraFollow();
	void SetLocationCamera(LocationCamera * locCamera);

//--------------------------------------------------------------------------------------------	
public:
	//���������� �������
	void Update(float dltTime);

	//����� ������� ������� ������
	void MoveCamera(float dltTime);
	//�������������������� ������� ������
	void BornCamera();
	//��������� ������� ������ ��� ������� ����
	void CalcPosition(float ang, float radius, float dax, CVECTOR & pos);
	//������� ������ � ������� ����������� �� 1���
	float FindRadius(float curAng);
	//������ ������� � ������ �������
	static bool ApplyPoly(const CVECTOR * v, long n);
	static void ApplyPolyFindNearest(EdgeInfo * e, long ne, const CVECTOR & pos);
	//�������� ������
	void ChangeRadius(float dltTime, float radius);
	//����� ��������� ��������� ������ ��� ������� ������������ � ������
	void FindRotAccelCam();

	//��������� ��� � ������ cull � ������� ������
	float Trace(const CVECTOR & src, const CVECTOR & dst);
	//��������� ��� � ������ cull
	float SubTrace(const CVECTOR & src, const CVECTOR & dst);

	void DrawDebug();

//private:
public:
	LocationCamera * lc;

	CVECTOR camPos, lookTo;
	//������ ��������� �� ����������
	float camay;				//������� ����������� ������
	float camradius;			//������� ������
	bool isBrn;					//���� ������������ �� ���� ������ ������ ������
	float rotSpd;				//�������� �������� ������ �� ������ ���������
	float rotAcc;				//��������� ���������	
	float kRadInert;			//����������� ������� ��������� ������� �� �������� ��������
	float kRadInertCur;			//������� ����������� �������
	
	
	static float fndRadius;		//������� ������ ��� ������
	static float fndMaxRadius;	//������������ ������ ��� ������
	static CVECTOR fndCamDir;	//��������������� ����������� �� ������
	static CVECTOR fndCamDirXZ;	//��������������� ����������� �� ������ � XZ
	static CVECTOR fndCamDirY;	//��������������� ����������� �� ������ � Y
	static CVECTOR fndCamPos;	//����� ����������
	static float fndcsAx;		//������� ��������������� ���� �� x
	static float fndkAx;		//1/(1 - cos(ax))
	static float fndcsAy;		//������� ��������������� ���� �� y
	static float fndkAy;		//1/(1 - cos(ay))	
	static PLANE fndPlanes[32];	//�������� ������������ ��������� ������
};

#endif

