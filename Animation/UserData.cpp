//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	Storm engine v2.00
//--------------------------------------------------------------------------------------------
//	UserData
//--------------------------------------------------------------------------------------------
//	�������� � ������ � ���������������� ������
//============================================================================================

#include "UserData.h"

//============================================================================================
//���������������, ������������������
//============================================================================================

UserData::UserData()
{
	data = null;
	numData = 0;
	maxData = 0;	
	strings = null;
	numBytes = 0;
	maxBytes = 0;
}

UserData::~UserData()
{
	if(data) delete data;
	if(strings) delete strings;
}

//--------------------------------------------------------------------------------------------
//������ � �������
//--------------------------------------------------------------------------------------------

//�������� ������
void UserData::AddData(const char * dname, const char * sdata)
{
	if(!dname || !dname[0]) return;
	//�������� ������
	if(numData >= maxData)
	{
		maxData += 16;
		data = (Data *)RESIZE(data, maxData*sizeof(Data));
	}
	//������ �����
	long ldname = strlen(dname) + 1;
	long ldata = strlen(sdata) + 1;
	//��������� ������
	data[numData].hashCode = CalcHashString(dname);
	data[numData].name = numBytes;
	data[numData].data = numBytes + ldname;
	numData++;
	//������ ��� �����
	if(numBytes + ldname + ldata >= maxBytes)
	{
		maxBytes += numBytes + ldname + ldata + 1024;
		strings = (char *)RESIZE(strings, maxBytes);
	}
	//�������� ������
	memcpy(strings + numBytes, dname, ldname);
	memcpy(strings + numBytes + ldname, sdata, ldata);
	numBytes += ldname + ldata;
}

//���������� ������ ������ ����� ���������� ������
void UserData::FreeExtra()
{
	if(numData < maxData)
	{
		maxData = numData;
		data = (Data *)RESIZE(data, maxData*sizeof(Data));
	}
	if(numBytes < maxBytes)
	{
		maxBytes = numBytes;
		strings = (char *)RESIZE(strings, maxBytes);
	}	
}

//����� ���������������� ������
const char * UserData::GetData(const char * dataName)
{
	if(!dataName || !dataName[0]) return null;
	//1 �������� ���� �������� ������
	long hash = CalcHashString(dataName);
	//2 ������ ������ � ����� ���������
	for(long i = 0; i < numData; i++)	
		if(data[i].hashCode == hash)
		{
			if(stricmp(dataName, strings + data[i].name) == 0)
					return strings + data[i].data;
		}	
	return null;
}


//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------

long UserData::CalcHashString(const char * str)
{
  unsigned long hval = 0;
  while(*str != '\0')
  {
	char c = *str++;
	if(c >= 'A' && c <= 'Z') c += 'a' - 'A';
    hval = (hval<<4) + (unsigned long int)c;
    unsigned long g = hval & ((unsigned long int) 0xf << (32 - 4));
    if(g != 0)
		{
			hval ^= g >> (32 - 8);
			hval ^= g;
		}
  }
  return long(hval);
}

