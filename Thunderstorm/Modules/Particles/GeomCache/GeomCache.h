#ifndef _GEOMETRY_CACHE_
#define _GEOMETRY_CACHE_

#include "geometry.h"
#include "geos.h"
#include <string>
#include <vector>


class IGMXScene;

class GeomCache
{
	VGEOMETRY* pGS;

	struct CachedGeometry
	{
		GEOS* pGeom;
		std::string FileName;
	};

	std::vector<CachedGeometry> Cache;

public:

	//�����������/����������
	GeomCache();
	~GeomCache();

	//�������� ������ � ���
	void CacheModel(const char* FileName);

	//�������� ���
	void ResetCache();

	//����� ������ �� ����
	GEOS* GetModel(const char* FileName);

	//��������� ���������� �� ����� ������ � ����
	bool ValidatePointer(GEOS* pModel);
};


#endif
