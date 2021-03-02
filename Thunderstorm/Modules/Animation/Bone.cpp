//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	Storm engine v2.00
//--------------------------------------------------------------------------------------------
//	Bone
//--------------------------------------------------------------------------------------------
//	����� ������, ���������� ���� ��������
//============================================================================================

#include "Bone.h"

#include "vmodule_api.h"

//============================================================================================
//���������������, ������������������
//============================================================================================

Bone::Bone()
{
	parent = nullptr;
	ang = nullptr;
	pos = nullptr;
	numFrames = 0;
}

Bone::~Bone()
{
	delete ang;
	delete pos;
}


//������� ������� ����� ������ ��������
void Bone::SetNumFrames(long num, CVECTOR& sPos, bool isRoot)
{
	delete ang;
	delete pos;
	ang = nullptr;
	pos = nullptr;
	numFrames = num;
	if (numFrames <= 0)
	{
		numFrames = 0;
		return;
	}
#ifdef ANI_COMPRESS_ENABLE
	ang = new COMP_QUATERNION[num];
	memset(ang, 0, numFrames * sizeof(ang[0]));
#else
	ang = new D3DXQUATERNION[num];
	memset(ang, 0, numFrames*sizeof(ang[0]));
#endif


	if (isRoot)
	{
		pos = new CVECTOR[num];
		memset(pos, 0, numFrames * sizeof(CVECTOR));
	}
	pos0 = sPos;
}

//���������� ������� ��������
void Bone::SetPositions(const CVECTOR* pArray, long numPos)
{
	Assert(numPos == numFrames);
	Assert(pArray);
	Assert(pos);
	memcpy(pos, pArray, numFrames * sizeof(CVECTOR));
}

//=====================================================================================

#ifdef ANI_COMPRESS_ENABLE

//-------------------------------------
//Compressed animation
//-------------------------------------

//���������� ���� ��������
void Bone::SetAngles(const D3DXQUATERNION* aArray, long numAng)
{
	Assert(numAng == numFrames);
	Assert(aArray);
	Assert(ang);
	for (long i = 0; i < numAng; i++)
	{
    auto x = Clamp(aArray[i].x, "Animation is break: qt.x < -1.0f or qt.x > 1.0f !!!");
    auto y = Clamp(aArray[i].y, "Animation is break: qt.y < -1.0f or qt.y > 1.0f !!!");
    auto z = Clamp(aArray[i].z, "Animation is break: qt.z < -1.0f or qt.z > 1.0f !!!");
    auto w = Clamp(aArray[i].w, "Animation is break: qt.w < -1.0f or qt.w > 1.0f !!!");
		x = float(asin(x) / (PI * 0.5)) * 32767.0f;
		y = float(asin(y) / (PI * 0.5)) * 32767.0f;
		z = float(asin(z) / (PI * 0.5)) * 32767.0f;
		w = float(asin(w) / (PI * 0.5)) * 32767.0f;
		ang[i].x = short(long(x));
		ang[i].y = short(long(y));
		ang[i].z = short(long(z));
		ang[i].w = short(long(w));
	}
}

inline void Bone::GetFrame(long f, D3DXQUATERNION& qt)
{
	qt.x = sinf((ang[f].x * (1.0f / 32767.0f)) * PI * 0.5f);
	qt.y = sinf((ang[f].y * (1.0f / 32767.0f)) * PI * 0.5f);
	qt.z = sinf((ang[f].z * (1.0f / 32767.0f)) * PI * 0.5f);
	qt.w = sinf((ang[f].w * (1.0f / 32767.0f)) * PI * 0.5f);
}

#else

//-------------------------------------
//Uncompressed animation
//-------------------------------------

//���������� ���� ��������
void Bone::SetAngles(const D3DXQUATERNION * aArray, long numAng)
{
	Assert(numAng == numFrames);
	Assert(aArray);
	Assert(ang);
	memcpy(ang, aArray, numFrames*sizeof(*ang));

}

inline void Bone::GetFrame(long f, D3DXQUATERNION & qt)
{
	qt = ang[f];
}


#endif


inline float Bone::Clamp(float v, const char* str)
{
  auto isErr = false;
	if (v < -1.0f)
	{
		v = -1.0f;
		isErr = true;
	}
	if (v > 1.0f)
	{
		v = 1.0f;
		isErr = true;
	}
	if (isErr && str) api->Trace(str);
	return v;
}


//=====================================================================================


//���������������� ��������� �������
void Bone::BuildStartMatrix()
{
	if (numFrames == 0 || !ang) return;
	CMatrix inmtx;
	D3DXQUATERNION a;
	GetFrame(0, a);
	D3DXMatrixRotationQuaternion(inmtx, &a);
	inmtx.Pos() = pos0;
	if (parent) start.EqMultiply(inmtx, parent->start);
	else start = inmtx;
}

//--------------------------------------------------------------------------------------------
//������ � ������
//--------------------------------------------------------------------------------------------


//�������� ����� ��������
void Bone::BlendFrame(long frame, float kBlend, D3DXQUATERNION& res)
{
	if (numFrames <= 0) return;
  auto f0 = frame;
  auto f1 = frame + 1;
	if (f0 >= numFrames || f1 >= numFrames)
	{
		GetFrame(numFrames - 1, res);
		return;
	}
	D3DXQUATERNION q0, q1;
	GetFrame(f0, q0);
	GetFrame(f1, q1);
	if (kBlend < 0.0f) kBlend = 0.0f;
	if (kBlend > 1.0f) kBlend = 1.0f;
	D3DXQuaternionSlerp(&res, &q0, &q1, kBlend);
}

//������� ������� ��� ����������� ���������
void Bone::BuildMatrix()
{
	//��������
	//matrix = a.BuildMatrix();
	D3DXMatrixRotationQuaternion(matrix, &a);
	matrix.Pos() = p;
	//�������� �� ������������ �������
	if (parent) matrix.EqMultiply(CMatrix(matrix), parent->matrix);
}
