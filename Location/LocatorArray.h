//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	LocatorArray
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _LocatorArray_H_
#define _LocatorArray_H_


#include "..\common_h\vmodule_api.h"
#include "..\common_h\matrix.h"


class LocatorArray
{
	struct LocatorInfro
	{
		CMatrix mtx;
		long name;
		long hash;
		float radius;
	};
//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	LocatorArray(const char * groupName);
	virtual ~LocatorArray();

//--------------------------------------------------------------------------------------------
//������ � ��������
//--------------------------------------------------------------------------------------------
public:
	//�������� �������
	void AddLocator(CMatrix & mtx, const char * name = null);
	//�������� ������� ��������
	void SetNewMatrix(long locIndex, CMatrix & mtx);
	//����� ��������� ������� �� ������
	float FindNearesLocator(float x, float y, float z, long * locIndex = null);
	//����� ��������� ������� �� ��������
	long FindNearesLocatorCl(float x, float y, float z, float height2, float & dist);
	//����� ������� �� �����
	long FindByName(const char * locName);
	//�������� ��� ��������
	const char * LocatorName(long locIndex);
	//�������� ���������� ��������
	bool GetLocatorPos(long locIndex, float & x, float & y, float & z);
	//�������� ������� ��������
	bool GetLocatorPos(long locIndex, CMatrix & mtx);
	//��������� ������ �� ������������
	bool IsValidateIndex(long locIndex);
	//���������� ���������
	long Num();
	//��� ��������
	char * Name(long locIndex);
	//�������� ����� ����
	bool CompareGroup(const char * groupName, long ghash);
	//�������� ��� ������
	char * GetGroupName();

	//���������� �������� ������
	void SetLocatorRadius(long locIndex, float radius);
	//�������� ������ ��������
	float GetLocatorRadius(long locIndex);

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
public:
	static long CalcHashString(const char * str);
private:
	//��� ������
	char * group;
	long hash;
	//��������
	LocatorInfro * locator;
	long numLocators;
	//����� ���������
	char * locatorNames;
	long bytesInLNArray;

public:
	bool isVisible;
	float radius;
	float kViewRadius;
	dword color;
	float viewDist;
};

//�������� ��� ��������
inline const char * LocatorArray::LocatorName(long locIndex)
{
	if(locIndex < 0 || locIndex >= numLocators) return null;
	if(locator[locIndex].name < 0) return null;
	return locatorNames + locator[locIndex].name;
}

//�������� ���������� ��������
inline bool LocatorArray::GetLocatorPos(long locIndex, float & x, float & y, float & z)
{
	if(locIndex < 0 || locIndex >= numLocators) return false;
	x = locator[locIndex].mtx.Pos().x;
	y = locator[locIndex].mtx.Pos().y;
	z = locator[locIndex].mtx.Pos().z;
	return true;
}

//�������� ������� ��������
inline bool LocatorArray::GetLocatorPos(long locIndex, CMatrix & mtx)
{
	if(locIndex < 0 || locIndex >= numLocators) return false;
	mtx = locator[locIndex].mtx;
	return true;
}

//��������� ������ �� ������������
inline bool LocatorArray::IsValidateIndex(long locIndex)
{
	return (locIndex < 0 || locIndex >= numLocators);
}

//���������� ���������
inline long LocatorArray::Num()
{
	return numLocators;
}

//��� ��������
inline char * LocatorArray::Name(long locIndex)
{
	Assert(locIndex >= 0 && locIndex < numLocators);
	return locatorNames + locator[locIndex].name;
}

//�������� ��� ������
inline char * LocatorArray::GetGroupName()
{
	return group;
}

//���������� �������� ������
inline void LocatorArray::SetLocatorRadius(long locIndex, float radius)
{
	Assert(locIndex >= 0 && locIndex < numLocators);
	locator[locIndex].radius = radius;
}

//�������� ������ ��������
inline float LocatorArray::GetLocatorRadius(long locIndex)
{
	Assert(locIndex >= 0 && locIndex < numLocators);
	if(locator[locIndex].radius < 0.0f) return radius;
	return locator[locIndex].radius;
}


#endif

