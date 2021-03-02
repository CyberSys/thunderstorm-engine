#include "DataBool.h"
#include "vmodule_api.h"

#pragma warning (disable : 4800)

//�����������/����������
DataBool::DataBool()
{
	Value = false;
}

DataBool::~DataBool()
{
}

//�������� ��������
bool DataBool::GetValue() const {
	return Value;
}

//���������� ��������
void DataBool::SetValue(bool val)
{
	Value = val;
}

void DataBool::Load(MemFile* File)
{
	uint32_t dwValue = 0;
	File->ReadType(dwValue);
	SetValue(dwValue);

	static char AttribueName[128];
	uint32_t NameLength = 0;
	File->ReadType(NameLength);
	Assert(NameLength < 128);
	File->Read(AttribueName, NameLength);

	SetName(AttribueName);
}


void DataBool::SetName(const char* szName)
{
	//api->Trace("DataBool::SetName - '%s'", szName);
	Name = szName;
}

const char* DataBool::GetName() const {
	return Name.c_str();
}

void DataBool::Write(MemFile* File) const {
	uint32_t dwValue = GetValue();
	File->WriteType(dwValue);


	//save name
  const uint32_t NameLength = Name.size();
  auto NameLengthPlusZero = NameLength + 1;
	File->WriteType(NameLengthPlusZero);
	Assert(NameLength < 128);
	File->Write(Name.c_str(), NameLength);
	File->WriteZeroByte();
}
