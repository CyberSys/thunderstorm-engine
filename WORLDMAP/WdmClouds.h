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
	struct Cloud
	{
		struct Cld
		{
			CVECTOR pos;	//�������
			float angle;	//���� ��������
			float size;		//������
			float alpha;	//������������
			dword color;	//����
			word pict;		//������ ��������
			word index;		//������ �������������� ��������
			float aspd;		//�������� ��������
			CVECTOR dir;	//������� ������ ������������� ��������
			CVECTOR rdr;	//���������� ������ ���������� ���������
		};

		Cloud();
		~Cloud();

		//���� ������ ������������ �� ���������� true
		bool Reset(bool isFirstTime = false);
		//�������� ��������� ������
		void Update(float dltTime);
		//��������� ������ ���������������
		long FillRects(RS_RECT * rects, long cnt, float galpha);
		//�������� ����� ����� � ������
		float GetBound(CVECTOR & _center);
		//��������� �������� �������� ������ ���� ���� �����������
		void Kill(const Cloud & cld);

	private:
		Cld cloud[WDMCLOUDSMAX];		//�������
		long count;						//����������
		float alpha;					//����� ������������
		float lifeTime;					//����� �����
		CVECTOR center;					//����� ������
		float radius;					//������ ������
	};

//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	WdmClouds();
	virtual ~WdmClouds();

	//�������
	virtual void Update(float dltTime);
	//���������
	virtual void LRender(VDX8RENDER * rs);
	

	long texture, light;
	Cloud clouds[16];
	RS_RECT rects[WDMCLOUDSMAX*16];
};

#endif

