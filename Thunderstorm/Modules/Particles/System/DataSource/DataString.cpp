#include "DataString.h"
#include "vmodule_api.h"


//�����������/����������
DataString::DataString()
{
}

DataString::~DataString()
{
}

//�������� ��������
const char* DataString::GetValue() const {
	return Value.c_str();
}

//���������� ��������
void DataString::SetValue(const char* val)
{
	Value = val;
}

void DataString::Load(MemFile* File)
{
	static char TempString[128];
	File->Read(TempString, 128);
	SetValue(TempString);

	static char AttribueName[128];
	uint32_t NameLength = 0;
	File->ReadType(NameLength);
	Assert(NameLength < 128);
	File->Read(AttribueName, NameLength);

	SetName(AttribueName);
}

void DataString::SetName(const char* szName)
{
	//api->Trace("DataString::SetName - '%s'", szName);
	Name = szName;
}

const char* DataString::GetName() const {
	return Name.c_str();
}

void DataString::Write(MemFile* File) const {
	static char WriteTempString[128];
	memset(WriteTempString, 0, 128);
	strncpy_s(WriteTempString, GetValue(), 128);
	File->Write(WriteTempString, 128);

	//save name
  const uint32_t NameLength = Name.size();
  auto NameLengthPlusZero = NameLength + 1;
	File->WriteType(NameLengthPlusZero);
	Assert(NameLength < 128);
	File->Write(Name.c_str(), NameLength);
	File->WriteZeroByte();
}
