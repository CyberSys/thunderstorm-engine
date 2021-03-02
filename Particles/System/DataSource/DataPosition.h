#ifndef _PARTICLE_DATA_POSITION_H_
#define _PARTICLE_DATA_POSITION_H_


#include "../../ICommon/MemFile.h"

#include "../../../Common_h/math3D.h"
#include <string>


class DataPosition
{
	std::string Name;


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