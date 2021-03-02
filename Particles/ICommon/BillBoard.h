#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_


#include "..\..\common_h\math3d.h"

//�������
struct ParticleBillboard
{
	Vector	vPos;
	float		fSizeX, fSizeY;
	float		fAngle;
	dword		dwColor;
	float		DistanceToCamera;
	bool		Visible;


//��� ������� �����------------------------------------
	//������� ����� ���� � UV
	float		U11;
	float		V11;

	//������ ������ ���� � UV
	float		U12;
	float		V12;

//��� ������� �����------------------------------------
	//������� ����� ���� � UV
	float		U21;
	float		V21;

	//������ ������ ���� � UV
	float		U22;
	float		V22;

//��������� ��������� ����� ������ � ������ ������
	float BlendK;


//�����������-------------------------------------------
	ParticleBillboard()
	{
		DistanceToCamera = 0.0f;
		Visible = false;
	}
};


#endif