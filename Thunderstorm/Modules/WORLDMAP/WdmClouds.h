//============================================================================================
//	Spirenkov Maxim, 2001
//--------------------------------------------------------------------------------------------
//	Sea Dogs II
//--------------------------------------------------------------------------------------------
//	WdmClouds
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _WdmClouds_H_
#define _WdmClouds_H_

#include "WdmRenderObject.h"

class WdmCloud;

#define WDMCLOUDSMAX	32

class WdmClouds : public WdmRenderObject
{
	static IDirect3DVertexDeclaration9* vertexDecl_;
	void CreateVertexDeclaration(VDX9RENDER* rs);

	struct Cloud
	{
		struct Cld
		{
			CVECTOR pos; //�������
			float angle; //���� ��������
			float size; //������
			float alpha; //������������
			uint32_t color; //����
			uint16_t pict; //������ ��������
			uint16_t index; //������ �������������� ��������
			float aspd; //�������� ��������
			CVECTOR dir; //������� ������ ������������� ��������
			CVECTOR rdr; //���������� ������ ���������� ���������
		};

		Cloud();

		//���� ������ ������������ �� ���������� true
		bool Reset(bool isFirstTime = false);
		//�������� ��������� ������
		void Update(float dltTime);
		//��������� ������ ���������������
		long FillRects(RS_RECT* rects, long cnt, float galpha);
		//�������� ����� ����� � ������
		float GetBound(CVECTOR& _center);
		//��������� �������� �������� ������ ���� ���� �����������
		void Kill(const Cloud& cld);

	private:
		Cld cloud[WDMCLOUDSMAX]; //�������
		long count; //����������
		float alpha; //����� ������������
		float lifeTime; //����� �����
		CVECTOR center; //����� ������
		float radius; //������ ������
	};

	//--------------------------------------------------------------------------------------------
	//���������������, ������������������
	//--------------------------------------------------------------------------------------------
public:
	WdmClouds();
	virtual ~WdmClouds();

	//�������
	void Update(float dltTime) override;
	//���������
	void LRender(VDX9RENDER* rs) override;


	long texture, light;
	Cloud clouds[16];
	RS_RECT rects[WDMCLOUDSMAX * 16];
};

#endif
