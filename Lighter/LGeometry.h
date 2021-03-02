//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	LGeometry
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _LGeometry_H_
#define _LGeometry_H_

#include "LTypes.h"


class LGeometry  
{
	struct Object
	{
		Object()
		{
			name = null;
			nameReal = null;
			m = null;
			lBufSize = 0;
		};
		char * name;			//���� �� col �����
		char * nameReal;		//��� ��������
		MODEL * m;				//��������� �� ��������
		ENTITY_ID model;		//��������
		long lBufSize;			//������ ������ ������ ��� ���� ��������
	};

//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	LGeometry();
	virtual ~LGeometry();

	//���������� ���� �� �������
	void SetModelsPath(const char * mPath);
	//���������� ���� ��� ������� ������
	void SetLightPath(const char * lPath);
	//�������� ������
	void AddObject(const char * name, ENTITY_ID & model);
	//���������� ������
	bool Process(VDX8RENDER * rs, long numLights);
	//���������� �������
	void DrawNormals(VDX8RENDER * rs);
	//�������� ����� � �������
	void UpdateColors(VDX8RENDER * rs);
	//����������� ��� ����� ��� ������
	float Trace(CVECTOR & src, CVECTOR & dst);
	//��������� ���������
	bool Save();


	Object * object;
	long numObjects;
	long maxObjects;

	Vertex * vrt;
	long numVrt;
	long maxVrt;

	Triangle * trg;
	long numTrg;
	long maxTrg;

	VertexBuffer * vbuffer;
	long numVBuffers;
	long maxVBuffers;

	Shadow * shadows;
	
	CVECTOR min, max;
	float radius;

	bool useColor;

	CVECTOR * drawbuf;

	char modelsPath[512];
	char lightPath[512];
};

#endif

