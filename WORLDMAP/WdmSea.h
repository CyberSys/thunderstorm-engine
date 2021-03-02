//============================================================================================
//	Spirenkov Maxim, 2001
//--------------------------------------------------------------------------------------------
//	Sea Dogs II
//--------------------------------------------------------------------------------------------
//	WdmSea
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _WdmSea_H_
#define _WdmSea_H_

#include "WdmRenderObject.h"

class WdmSea : public WdmRenderObject  
{
#pragma pack(push, 1)

	struct Triangle
	{
		word index[3];
	};

	struct Vertex
	{
		float x, y, z;
		dword color;
		float tu, tv;
	};

	struct WhiteHorses
	{
		float x;
		float z;
		float angle;
		float size;
		float speed;
		float curTime;
		float kTime;
		long textureIndex;
	};

	struct Flare
	{
		long index;		//������ ������������� �����
		float time;		//����� ����� �� 0 �� 1
		float k;		//�������� ������� �������
		float phase;	//����, �������� ����������� ��������
	};

#pragma pack(pop)

//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	WdmSea();
	virtual ~WdmSea();

	void Update(float dltTime);

	virtual void PRender(VDX8RENDER * rs);
	virtual void LRender(VDX8RENDER * rs);


//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	//����������
	void Render(VDX8RENDER * rs, const char * tech = null);
	//��������� �������������� ���������� ���������
	void PresetMain(VDX8RENDER * rs);

private:
	//������ ��� ���������
	long vb, ib;
	//�������
	WhiteHorses wh[16];
	//Textures
	long baseTexture;
	long aniTextures[32];
	float aniFrame;
	long whiteHorses[1];
	float seaSizeX;
	float seaSizeZ;
	float aniTiling;
	//�������
	Flare flare[1024];
	RS_RECT flareRect[1024];
	long flareCount;
	long flareTexture;
	float flareCerateCounter;
};

#endif

