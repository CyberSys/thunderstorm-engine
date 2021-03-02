#include "DataColor.h"
#include "../../ICommon/GraphTime.h"
#include "../../ICommon/MemFile.h"
#include "vmodule_api.h"

//�����������/����������
DataColor::DataColor () : ZeroColor(0xFFFFFFFF)
{
}

DataColor::~DataColor ()
{
}

//�������� �������� (������� �����, ����� ����� �����, ��������� �������[0..1])
Color DataColor::GetValue (float Time, float LifeTime, float K_rand)
{
	//����� � ������� ����� ������ �������������...
	Time = (Time / LifeTime);

	uint32_t Count = ColorGraph.size();
	uint32_t StartIndex = 0;
	for (uint32_t n = StartIndex; n < (Count-1); n++)
	{
		float FromTime = ColorGraph[n].Time;
		float ToTime = ColorGraph[n+1].Time;

		//���� ����� � ������ ���������...
		if ((Time >= FromTime)	&& (Time <= ToTime))
		{
			float SegmentDeltaTime = ColorGraph[n+1].Time - ColorGraph[n].Time;
			float ValueDeltaTime = Time - ColorGraph[n].Time;
			float blend_k = 0.0f;
			if (SegmentDeltaTime > 0.001f)	blend_k = ValueDeltaTime / SegmentDeltaTime;

			Color ValueFirstMax = ColorGraph[n].MaxValue;
			Color ValueSecondMax = ColorGraph[n+1].MaxValue;
			Color ValueFirstMin = ColorGraph[n].MinValue;
			Color ValueSecondMin = ColorGraph[n+1].MinValue;

			Color MaxVal;
			MaxVal.Lerp(ValueFirstMax, ValueSecondMax, blend_k);
			Color MinVal;
			MinVal.Lerp(ValueFirstMin, ValueSecondMin, blend_k);

			Color Result;
			Result.Lerp(MinVal, MaxVal, K_rand);

			return Result;
		}
	}

	return ZeroColor;
}

//������������� "�������� �� ���������"
//��� �������, Min=Max=Value
void DataColor::SetDefaultValue (const Color& Value)
{
	ColorGraph.clear();

	ColorVertex pMinVertex;
	pMinVertex.Time = MIN_GRAPH_TIME;
	pMinVertex.MinValue = Value;
	pMinVertex.MaxValue = Value;
	ColorGraph.push_back(pMinVertex);

	ColorVertex pMaxVertex;
	pMinVertex.Time = 1.0f;
	pMinVertex.MinValue = Value;
	pMinVertex.MaxValue = Value;
	ColorGraph.push_back(pMinVertex);

}

//���������� ��������
void DataColor::SetValues (const ColorVertex* Values, uint32_t Count)
{
	ColorGraph.clear();
	for (uint32_t n = 0; n < Count; n++)
	{
		ColorGraph.push_back(Values[n]);
	}
}

//�������� ���-�� ��������
uint32_t DataColor::GetValuesCount ()
{
	return ColorGraph.size();
}


//�������� ���. �������� (�� �������)
const Color& DataColor::GetMinValue (uint32_t Index)
{
	return ColorGraph[Index].MinValue;
}

//�������� ����. �������� (�� �������)
const Color& DataColor::GetMaxValue (uint32_t Index)
{
	return ColorGraph[Index].MaxValue;
}


void DataColor::Load (MemFile* File)
{
	uint32_t dwColorCount = 0;
	File->ReadType(dwColorCount);

	for (uint32_t n = 0; n < dwColorCount; n++)
	{
		float Time = 0.0f;
		File->ReadType(Time);

		Color clrMax;
		File->ReadType(clrMax);

		Color clrMin;
		File->ReadType(clrMin);

		ColorVertex pColor;
		pColor.Time = Time;
		pColor.MinValue = clrMin;
		pColor.MaxValue = clrMax;
		ColorGraph.push_back(pColor);
	}

	static char AttribueName[128];
	uint32_t NameLength = 0;
	File->ReadType(NameLength);
	Assert (NameLength < 128);
	File->Read(AttribueName, NameLength);

	SetName (AttribueName);

}

void DataColor::SetName (const char* szName)
{
	//api->Trace("DataColor::SetName - '%s'", szName);
	Name = szName;
}

const char* DataColor::GetName ()
{
	return Name.c_str();
}

const ColorVertex& DataColor::GetByIndex (uint32_t Index)
{
	return ColorGraph[Index];
}

void DataColor::Write (MemFile* File)
{
	uint32_t dwColorCount = ColorGraph.size();
	File->WriteType(dwColorCount);

	for (uint32_t n = 0; n < dwColorCount; n++)
	{
		float Time = ColorGraph[n].Time;
		File->WriteType(Time);

		Color clrMax = ColorGraph[n].MaxValue;
		File->WriteType(clrMax);

		Color clrMin = ColorGraph[n].MinValue;
		File->WriteType(clrMin);
	}

	//save name
	uint32_t NameLength = Name.size();
	uint32_t NameLengthPlusZero = NameLength+1;
	File->WriteType(NameLengthPlusZero);
	Assert (NameLength < 128);
	File->Write(Name.c_str(), NameLength);
	File->WriteZeroByte();
}

