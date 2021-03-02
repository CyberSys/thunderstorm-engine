#ifndef _PARTICLE_DATA_POSITION_H_
#define _PARTICLE_DATA_POSITION_H_


#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "../../../common_h/exs.h"
#include "../../../common_h/templates.h"
#include "..\..\icommon\memfile.h"

#include "..\..\..\common_h\math3d.h"


class DataPosition
{
  string Name;


	Vector Value;

public:

//�����������/����������
	DataPosition ();
	virtual ~DataPosition ();

//�������� �������� (������� �����, ��������� �������[0..1])
	const Vector& GetValue ();

//���������� ��������
	void SetValue (const Vector& val);


	void Load (MemFile* File);
	void Write (MemFile* File);

	void SetName (const char* szName);
	const char* GetName ();

};


#endif