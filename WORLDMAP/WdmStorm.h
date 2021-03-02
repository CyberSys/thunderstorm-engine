//============================================================================================
//	Spirenkov Maxim, 2001
//--------------------------------------------------------------------------------------------
//	Sea Dogs II
//--------------------------------------------------------------------------------------------
//	WdmStorm
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _WdmStorm_H_
#define _WdmStorm_H_

#include "WdmRenderObject.h"

class WdmCloud;

class WdmStorm : public WdmRenderObject
{
//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	WdmStorm();
	virtual ~WdmStorm();

	void SetLiveTime(float t);
	float GetLiveTime();
	void GetPosition(float & x, float & z);
	bool IsActive();

	bool CheckIntersection(float x, float z, float r);

	//�������
	virtual void Update(float dltTime);
	virtual void LRender(VDX8RENDER * rs);

	const char * GetID();


public:
	//��������� ����������
	void SetSaveAttribute(ATTRIBUTES * save);
	void DeleteUpdate();

	bool isTornado;

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	//���������� ����������� ������
	void UpdateSaveData();

private:
	CVECTOR pos, dir;

	float isActiveTime;
	float liveTime;
	float liveAlpha;
	float speed;
	bool isBrn, isKl;

	long num;
	WdmCloud * cloud[8];	//��������� �� ������
	CVECTOR cloudPos[8];	//�������
	float rotSpd[8];		//�������� �������� ������ ������

	ATTRIBUTES * saveAttribute;

	//������
	long rainTexture;
	RS_RECT rainRect[48*8];

	static char cloudPosName[16];
	static char rotSpdName[16];
};

inline void WdmStorm::SetLiveTime(float t)
{
	if(t < 1.0f) t = 1.0f;
	liveTime = t;
}

inline float WdmStorm::GetLiveTime()
{
	return liveTime >= 0.0f ? liveTime : 0.0f;	
}

inline void WdmStorm::GetPosition(float & x, float & z)
{
	x = pos.x;
	z = pos.z;
}

inline bool WdmStorm::IsActive()
{
	return isActiveTime <= 0.0f;
}


#endif

