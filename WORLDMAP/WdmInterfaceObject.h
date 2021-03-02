//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	WdmInterfaceObject
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _WdmInterfaceObject_H_
#define _WdmInterfaceObject_H_

#include "WdmRenderObject.h"

class WdmInterfaceObject : public WdmRenderObject  
{
protected:
	struct Vertex
	{
		float x, y, z, rhw;
		dword color;
		float tu, tv;
		float tu1, tv1;
	};



//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	WdmInterfaceObject();
	virtual ~WdmInterfaceObject();

//--------------------------------------------------------------------------------------------
//��������� ��� ������������� �������
//--------------------------------------------------------------------------------------------
protected:
	//��������� 6 ������ ������������ ��������������
	static void FillRectCoord(Vertex * vrt, float x, float y, float w, float h, float ang = 0.0f);
	//��������� 6 ������ ����������� ������������ ��������������
	static void FillRectUV(Vertex * vrt, float tu, float tv, float tw, float th, float ang = 0.0f);
	//��������� 6 ������ ����������� ������������ ��������������
	static void FillRectUV1(Vertex * vrt, float tu, float tv, float tw, float th, float ang = 0.0f);
	//��������� 6 ������ ������
	static void FillRectColor(Vertex * vrt, dword color = 0xffffffff);
	//���������� ������ ���������������
	static void DrawRects(Vertex * vrt, long numRects, char * techName = null);
	
	//��������� 18 ������ �������������� ��������������
	static void FillSRectCoord(Vertex * vrt, float x, float y, float w, float h, float ws);
	//��������� 18 ������ ����������� ������������ �������������� ��������������
	static void FillSRectUV(Vertex * vrt, float tu, float tv, float tw, float th, float ts);
	//��������� 18 ������ ������ �������������� ��������������
	static void FillSRectColor(Vertex * vrt, dword color = 0xffffffff);

	//������� ����� � �������� ������� �� ����������� � � �������� ������
	static void Print(long font, long color, float xleft, float xright, float y, const char * format, ...);

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
protected:
	static char stringBuffer[1024];
private:
	
};

#endif

