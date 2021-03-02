#ifndef _PARTICLE_DATA_FLOAT_H_
#define _PARTICLE_DATA_FLOAT_H_


#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "../../../common_h/exs.h"
#include "..\..\icommon\memfile.h"
#include <string>


class DataFloat
{
	std::string Name;


	float Value;

public:

//�����������/����������
	DataFloat ();
	virtual ~DataFloat ();

	//�������� ��������
	float GetValue ();

	//���������� ��������
	void SetValue (float val);


	void Load (MemFile* File);
	void Write (MemFile* File);


	void SetName (const char* szName);
	const char* GetName ();

};


#endif