#include "GeomCache.h"


//�����������/����������
GeomCache::GeomCache()
{
	pGS = (VGEOMETRY *)api->CreateService("geometry");
	Assert(pGS);
}

GeomCache::~GeomCache()
{
	ResetCache();
}

//�������� ������ � ���
void GeomCache::CacheModel(const char* FileName)
{
	if (GetModel(FileName)) return;

  auto pGeom = pGS->CreateGeometry(FileName, "", 0);
	if (!pGeom) return;

	CachedGeometry CacheEntry;
	CacheEntry.FileName = FileName;
	CacheEntry.pGeom = pGeom;
	Cache.push_back(CacheEntry);
}

//�������� ���
void GeomCache::ResetCache()
{
	for (auto n = 0; n < Cache.size(); n++)
	{
		delete Cache[n].pGeom;
	}

	Cache.clear();
}

//����� ������ �� ����
GEOS* GeomCache::GetModel(const char* FileName)
{
	for (auto n = 0; n < Cache.size(); n++)
	{
		if (Cache[n].FileName == FileName) return Cache[n].pGeom;
	}

	return nullptr;
}

//��������� ���������� �� ����� ������ � ����
bool GeomCache::ValidatePointer(GEOS* pModel)
{
	for (auto n = 0; n < Cache.size(); n++)
	{
		if (Cache[n].pGeom == pModel) return true;
	}

	return false;
}
