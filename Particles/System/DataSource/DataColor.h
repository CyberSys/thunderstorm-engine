#ifndef _PARTICLE_DATA_COLOR_H_
#define _PARTICLE_DATA_COLOR_H_


#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "../../../common_h/exs.h"
#include "../../../common_h/templates.h"
#include "..\..\icommon\memfile.h"
#include "..\..\icommon\colorvertex.h"



class DataColor
{
  string Name;

	const Color ZeroColor;

	array<ColorVertex> ColorGraph;

public:

//�����������/����������
	DataColor ();
	virtual ~DataColor ();

//�������� �������� (������� �����, ����� ����� �����, ��������� �������[0..1])
	Color GetValue (float Time, float LifeTime, float K_rand);

//������������� "�������� �� ���������"
//��� �������, Min=Max=Value
	void SetDefaultValue (const Color& Value);

//���������� ��������
	void SetValues (const ColorVertex* Values, DWORD Count);

//�������� ���-�� ��������
	DWORD GetValuesCount ();

//�������� ���. �������� (�� �������)
	const Color& GetMinValue (DWORD Index);

//�������� ����. �������� (�� �������)
	const Color& GetMaxValue (DWORD Index);



	void Load (MemFile* File);
	void Write (MemFile* File);


	void SetName (const char* szName);
	const char* GetName ();


	const ColorVertex& GetByIndex (DWORD Index);

};


#endif