#ifndef _DATA_CACHE_
#define _DATA_CACHE_

#include "..\system\datasource\datasource.h"
//#include "..\..\common_h\file.h"
#include "..\..\common_h\templates.h"

class IParticleManager;

class DataCache
{
	IParticleManager* Master;

	struct LoadedDataSource
	{
		string FileName;
		DataSource* pData;

		LoadedDataSource ()
		{
			pData = NULL;
		}
	};
	
	array<LoadedDataSource> Cache;
	
	void CreateDataSource (void* pBuffer, DWORD BufferSize, const char* SourceFileName);


public:

//�����������/����������
	DataCache (IParticleManager* pManager);
	~DataCache ();

//�������� � ��� ������ ��� �������
	void CacheSystem (const char* FileName);

//�������� ���
	void ResetCache ();

//�������� ��������� �� ������ ��� ������� ���������
	DataSource* GetParticleSystemDataSource (const char* FileName);

//��������� ��������� �� ����������
	bool ValidatePointer (DataSource* pData);

	DWORD GetCachedCount ();
	const char* GetCachedNameByIndex (DWORD Index);


};


#endif