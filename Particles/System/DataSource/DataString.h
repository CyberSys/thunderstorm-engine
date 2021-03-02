#ifndef _PARTICLE_DATA_STRING_H_
#define _PARTICLE_DATA_STRING_H_

#include "..\..\icommon\memfile.h"

#include <string>


class DataString
{
	std::string Name;

	std::string Value;

public:

//�����������/����������
	DataString ();
	virtual ~DataString ();

//�������� ��������
	const char* GetValue ();

//���������� ��������
	void SetValue (const char* val);


	void Load (MemFile* File);
	void Write (MemFile* File);


	void SetName (const char* szName);
	const char* GetName ();

};


#endif