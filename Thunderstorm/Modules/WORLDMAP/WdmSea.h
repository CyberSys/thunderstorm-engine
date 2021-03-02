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
		uint16_t index[3];
	};

	struct Vertex
	{
		float x, y, z;
		uint32_t color;
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
		long index; //������ ������������� �����
		float time; //����� ����� �� 0 �� 1
		float k; //�������� ������� �������
		float phase; //����, �������� ����������� ��������
	};

#pragma pack(pop)

	//--------------------------------------------------------------------------------------------
	//���������������, ������������������
	//--------------------------------------------------------------------------------------------
public:
	WdmSea();
	virtual ~WdmSea();

	void Update(float dltTime) override;

	void PRender(VDX9RENDER* rs) override;
	void LRender(VDX9RENDER* rs) override;


	//--------------------------------------------------------------------------------------------
	//������������
	//--------------------------------------------------------------------------------------------
private:
	//����������
	void Render(VDX9RENDER* rs, const char* tech = nullptr) const;
	//��������� �������������� ���������� ���������
	void PresetMain(VDX9RENDER* rs) const;

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
