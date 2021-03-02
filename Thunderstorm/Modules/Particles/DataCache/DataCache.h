#ifndef _DATA_CACHE_
#define _DATA_CACHE_

#include "../system/datasource/datasource.h"
//#include "..\file.h"
#include <vector>

class IParticleManager;

class DataCache
{
	IParticleManager* Master;

	struct LoadedDataSource
	{
		std::string FileName;
		DataSource* pData;

		LoadedDataSource ()
		{
			pData = nullptr;
		}
	};
	
	std::vector<LoadedDataSource> Cache;
	
	void CreateDataSource (void* pBuffer, uint32_t BufferSize, const char* SourceFileName);


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

	uint32_t GetCachedCount ();
	const char* GetCachedNameByIndex (uint32_t Index);


};


#endif