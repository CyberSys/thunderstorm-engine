#include "GeomCache.h"


//�����������/����������
GeomCache::GeomCache () : Cache(_FL_)
{
	pGS = (VGEOMETRY *)api->CreateService("geometry");
	Assert (pGS);
}

GeomCache::~GeomCache ()
{
	ResetCache ();
}

//�������� ������ � ���
void GeomCache::CacheModel (const char* FileName)
{
	if (GetModel(FileName)) return;
	
	GEOS* pGeom = pGS->CreateGeometry(FileName, "", 0);
	if (!pGeom) return;

	CachedGeometry CacheEntry;
	CacheEntry.FileName = FileName;
	CacheEntry.pGeom = pGeom;
	Cache.Add(CacheEntry);
}

//�������� ���
void GeomCache::ResetCache ()
{
	for (int n = 0; n < Cache; n++)
	{
		delete Cache[n].pGeom;
	}

	Cache.DelAll();
}

//����� ������ �� ����
GEOS* GeomCache::GetModel (const char* FileName)
{
	for (int n = 0; n < Cache; n++)
	{
		if (Cache[n].FileName == FileName) return Cache[n].pGeom;
	}

	return NULL;
}

//��������� ���������� �� ����� ������ � ����
bool GeomCache::ValidatePointer (GEOS* pModel)
{
	for (int n = 0; n < Cache; n++)
	{
		if (Cache[n].pGeom == pModel) return true;
	}

	return false;
}
