#ifndef _PARTICLE_DATA_GRAPH_H_
#define _PARTICLE_DATA_GRAPH_H_

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "../../../common_h/exs.h"
#include "../../../common_h/templates.h"
#include "..\..\icommon\memfile.h"

#include "..\..\icommon\GraphVertex.h"


class DataGraph
{
  string Name;


//� ������ ������� ��������� ��� �������� ��������
	float MaxCachedTime;
	float MinCachedTime;
//����� ��� ������ � ����� �������
	DWORD MaxCachedIndex;
	DWORD MinCachedIndex;

	array<GraphVertex> MinGraph;
	array<GraphVertex> MaxGraph;


	void ResetCachedTime ();

	float GetMinAtTime (float Time, float LifeTime);
	float GetMaxAtTime (float Time, float LifeTime);


	bool bRelative;
	bool bNegative;
	

public:

//�����������/����������
	DataGraph ();
	virtual ~DataGraph ();


	//����������/�������� ����� ���� ������������� �������� � ������� ��� ���...
	void SetNegative (bool _bNegative);
	bool GetNegative ();

	//����������/�������� ������������� ������ ��� ���...
	void SetRelative (bool _bRelative);
	bool GetRelative ();


//�������� �������� (������� �����, ��������� �������[0..1])
	float GetValue (float Time, float LifeTime, float K_rand);
	float GetRandomValue (float Time, float LifeTime);


//���������� ��������...
	void SetValues (const GraphVertex* MinValues, DWORD MinValuesSize, const GraphVertex* MaxValues, DWORD MaxValuesSize);

	//������������� "�������� �� ���������"
	void SetDefaultValue (float MaxValue, float MinValue);


//�������� ���-�� � ������� ��������
	DWORD GetMinCount ();

//�������� ���-�� � ������� ���������
	DWORD GetMaxCount ();

//�������� �������� �� ������� �� ������� ��������
	const GraphVertex& GetMinVertex (DWORD Index);

//�������� �������� �� ������� �� ������� ���������
	const GraphVertex& GetMaxVertex (DWORD Index);


	void Load (MemFile* File);
	void Write (MemFile* File);
	
	void SetName (const char* szName);
	const char* GetName ();


	float GetMaxTime ();


	void ConvertRadToDeg ();
	void ConvertDegToRad ();
	void MultiplyBy (float Val);
	void Clamp (float MinValue, float MaxValue);
	void Reverse ();  //Graphs = 1.0f - Graph
	void NormalToPercent ();
	void PercentToNormal ();
	void NormalToAlpha ();
	void AlphaToNormal ();




};


#endif