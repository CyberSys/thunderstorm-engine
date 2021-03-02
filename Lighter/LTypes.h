
#ifndef _LighterTypes_H_
#define _LighterTypes_H_


#include "..\common_h\vmodule_api.h"
#include "..\common_h\matrix.h"
#include "..\common_h\model.h"


struct Light
{
	enum Type
	{
		t_none,
		t_amb,
		t_sun,
		t_sky,
		t_point,
		t_group,
	};

	CVECTOR color;		//���� ���������
	CVECTOR p;			//������� ��������� ��������� ��� ����������� ������������� ���������
	float att0;			//��������� D^0
	float att1;			//��������� D^1
	float att2;			//��������� D^2
	float range;		//������ ��������
	char * group;		//������ � ������� ����������� ��������
	//��������� ���������
	float cosine;		//���������� ��������
	float shadow;		//���������� ����
	float bright;		//������� ����
	float contr;		//������������� ����
	float gamma;		//����� ����
	float curgm;		//������� ��� ��������� �����
	float curct;		//������� ��� ��������� ��������
	//	
	Type type;			//��� ���������
	bool isOn;			//�������� �� ��������
	bool isMark;		//��� ���������� ������
};


struct Shadow
{
	double v;				//����������� ��������
	double nrm;				//���������� ������������
	double sm;				//���������� ��������
	float cs;				//������� ���� ������� � ����������� �� ��������
	float att;				//���������� ���������
	float dst;				//��������� �� ���������
	float csatt;			//���������� ���������
	float shw;				//���������� ��������� ����
	CVECTOR c;				//������� ���� ���������
};


struct Vertex
{
	enum Flag
	{
		f_zero = 0,			//�������������
		f_inv = 1,			//������������� �� �������
		f_set = 2,			//���� �� ������� �������� �������
		f_bug = 4,			//���� ������
	};

	CVECTOR p;				//�������
	CVECTOR n;				//�������
	CVECTOR c;				//����
	CVECTOR bc;				//���� ��� �����������
	CVECTOR mc;				//�������� �����
	dword alpha;			//����� ��������
	Shadow * shadow;		//�������� ��������� �� ������� ���������
	long flags;				//�����
	long vbid;				//����� ������
	long addr;				//������������� ����� ����� � ������
	long obj;				//������ �������� �����������
	long cindex;			//������ � �������� ������ ������ �������
};

struct Triangle
{
	CVECTOR n;				//�������
	float sq;				//�������
	long i[3];				//������� ������
};

struct VertexBuffer
{
	long vbID;				//������������� ������� ������
	long start;				//������ ������ � ���������� �������
};

struct OctFndVerts
{
	Vertex * v;				//�������
	float r2;				//������� ���������
};

#endif
