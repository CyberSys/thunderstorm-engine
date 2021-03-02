//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	Storm engine v2.00
//--------------------------------------------------------------------------------------------
//	Bone
//--------------------------------------------------------------------------------------------
//	����� ������, ���������� ���� ��������
//============================================================================================

#ifndef _Bone_H_
#define _Bone_H_

//============================================================================================

#include "..\Common_h\Matrix.h"
#include "..\Common_h\d_types.h"
#include "..\Common_h\memop.h"
#include "..\Common_h\exs.h"


#include <d3dx8.h>
//============================================================================================

#ifndef PI
#define PI	3.141592653589793238f
#endif

#define ANI_COMPRESS_ENABLE

//============================================================================================

class Bone
{

#pragma pack(push, 1)

	struct COMP_QUATERNION
	{
		short x, y, z;
		short w;
	};

#pragma pack(pop)


//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	Bone();
	virtual ~Bone();
	//���������� ��������
	void SetParent(Bone * parentBone);
	//������� ������� ����� ������ ��������
	void SetNumFrames(long num, CVECTOR & sPos, bool isRoot = false);
	//���������� ������� ��������
	void SetPositions(const CVECTOR * pArray, long numPos);
	//���������� ���� ��������
	void SetAngles(const D3DXQUATERNION * aArray, long numAng);
	//���������������� ��������� �������
	void BuildStartMatrix();


//--------------------------------------------------------------------------------------------
//������ � ������
//--------------------------------------------------------------------------------------------
public:	
	//�������� ����� ��������
	void BlendFrame(long frame, float kBlend, D3DXQUATERNION & res);
	//void BlendFrame(float frame);
	//������� ������� ��� ����������� ���������
	void BuildMatrix();
	//������� ������� ��� 0 �����
	//void BuildMatrixZero();
	//�������� ������� ���������
	CMatrix & Matrix();
	//�������� ���������� ������� ���������
	CMatrix & StartMatrix();

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	void GetFrame(long f, D3DXQUATERNION & qt);
	float Clamp(float v, const char * str);
	//�������� ������������ �������
	float LerpPos(float a, float b, float k);
	//�������� ������������ ����
	float LerpAng(float a, float b, float k);

public:
	Bone * parent;			//������������ �����

#ifdef ANI_COMPRESS_ENABLE
	COMP_QUATERNION * ang;	//���������������� ���� ������ ��������
#else
	D3DXQUATERNION * ang;	//���� ������ ��������
#endif

	CVECTOR * pos;			//������� ������ ��������
	long numFrames;			//���������� ������ ��������
	CVECTOR pos0;			//������� �����, ���� ��� ������� ������ ��������

	CVECTOR p;				//������� ����� � ��������� �����������
	D3DXQUATERNION a;				//���� �������� ����� � ��������� �����������
	CMatrix matrix;			//������� ��������� �����
	CMatrix start;			//������� 0 �����
};

//============================================================================================
//inline
//============================================================================================

//���������� ��������
inline void Bone::SetParent(Bone * parentBone)
{
	parent = parentBone;
}

//�������� ������� ���������
inline CMatrix & Bone::Matrix()
{
	return matrix;
}

//�������� ���������� ������� ���������
inline CMatrix & Bone::StartMatrix()
{
	return start;
}

//�������� ������������ �������
inline float Bone::LerpPos(float a, float b, float k)
{
	return a + k*(b - a);
}

//�������� ������������ ����
inline float Bone::LerpAng(float a, float b, float k)
{
	float delta = b - a;
	if(delta < -PI) delta = 2*PI + delta;
	if(delta >  PI) delta =-2*PI + delta;
	return a + k*delta;	
}


#endif

