#ifndef _GEOMETRY_CACHE_
#define _GEOMETRY_CACHE_

#include "..\..\common_h\vapi.h"
#include "..\..\common_h\geometry.h"
#include "..\..\common_h\geos.h"
#include "..\..\common_h\templates.h"


class IGMXScene;

class GeomCache
{
	
	VGEOMETRY* pGS;

	struct CachedGeometry
	{
		GEOS* pGeom;
		string FileName;
	};

	array<CachedGeometry> Cache;

public:

//�����������/����������
	GeomCache ();
	~GeomCache ();

//�������� ������ � ���
	void CacheModel (const char* FileName);

//�������� ���
	void ResetCache ();

//����� ������ �� ����
	GEOS* GetModel (const char* FileName);

//��������� ���������� �� ����� ������ � ����
	bool ValidatePointer (GEOS* pModel);

};


#endif