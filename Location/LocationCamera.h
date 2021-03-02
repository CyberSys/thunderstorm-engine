//============================================================================================
//	Spirenkov Maxim
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	LocationCamera
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _LocationCamera_H_
#define _LocationCamera_H_


#include "..\common_h\vmodule_api.h"
#include "..\common_h\dx8render.h"
#include "..\common_h\matrix.h"
#include "..\common_h\templates\array.h"
#include "..\common_h\templates\string.h"
#include "PathTracks.h"

#include "CameraFollow.h"

class MODEL;
class Character;
class Location;

class LocationCamera : public ENTITY  
{
	friend CameraFollow;

	enum CameraWorkMode
	{
		cwm_none = 0,
		cwm_follow,		//������ ������������� ��������
		cwm_topos,		//����������� ������ � �������� �������
		cwm_free,		//������ �������� �������� (����������)
	};

//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	LocationCamera();
	virtual ~LocationCamera();

	//�������������
	bool Init();
	//����������
	void Execute(dword delta_time);
	void Realize(dword delta_time);
	//���������
	dword _cdecl ProcessMessage(MESSAGE & message);
	//��������� ��������
	dword AttributeChanged(ATTRIBUTES * apnt);

	void LockFPMode(bool isLock);
	float GetAx();

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	//����������� ������
	bool Set();
	//�������� ����� ������ ������
	void SetMode(CameraWorkMode wm);
	//���������� ������ ��������� �� ����������
	void ExecuteFollow(float dltTime);
	//���������� ������ ���� �� ����
	void ExecuteLook(float dltTime);
	//���������� ������ ����������� �� �����
	void ExecuteTopos(float dltTime);
	//���������� �������� �������� ������
	void ExecuteFree(float dltTime);

	//����������� ������ �� �������� ������� � �����
	bool MoveFollow(CVECTOR & pos, const CVECTOR & cpos, const CVECTOR & to);
	
	//���������� ��� ����� �������
	float Trace(const CVECTOR & src, const CVECTOR & dst);
	bool GetCollideTriangle(TRIANGLE & trg);
	void Clip(PLANE * p, long numPlanes, CVECTOR & cnt, float rad, bool (* fnc)(const CVECTOR * vtx, long num));

	

private:
	VDX8RENDER * rs;
	//����
	ENTITY_ID sea;
	//��������� ������
	float ax;					//���� ������� ������
	float lAx;
	float vAx;					//������� �������� ���������
	float axmin, axmax;			//���� ������� ������
	float teleport_dist;		//������������ ��������� ��� ������� ������ ���������������
	float lookHeight;			//�������� ���� �������� �� ������ (�������������)
	float radius;				//������ ���������� �� ���������
	float rotInertia;			//������� ��������
	float radInertia;			//������� ����������� ��������

	bool isSpecialMode;
	bool isLockFPMode;
	bool isViewExecute;

	ENTITY_ID loc;
	Location * location;

	//����� ������
	CameraWorkMode wmode;
	bool isSleep;
	bool lockAx;

	float dAx, dAy;

	CVECTOR camPos, lookTo;		//������� ��������� ������	
	CVECTOR oldPos, oldLookTo;	//��������� ������� ������ ����������� ������	
	float kMorph;				//������� ���������� ��������

	//��������
	ENTITY_ID chr;				//��������
	ENTITY_ID patchMdl;			//���� ��� ������

	CameraFollow cf;			//������ ��������� �� ����������

	float obstructTime;			//����� ����������� ���������
	CVECTOR obstruct;			//�����

	
	float freeAx, freeAy;		//���� ��������� ������

	bool isLookMode;
	bool isTeleport;

	//������ ����������� �� ���������� �� �����
	CVECTOR fromLook;

	//��������� �������� �� ������ ����������
	Character * character;		//��������� �� ���������
	CVECTOR pos;				//��������� ���������
	float chay;					//����������� ���������
	float lheight;				//������ ���� ��������
	float height;				//������ ���������
	float chradius;				//������ ���������

	//������������ ��������� ����������� (��������)
	struct {
		bool isOn;
		float fMinFov;
		float fMaxFov;
		float fCurFov;
		bool bFogUp;
		float fFogChangeSpeed;
		float fFogTimeCur;
		float fFogTimeMax;
		float fCurAngle;
		float fMaxAngle;
		float fAngleSpeed;
		bool bAngleUp;
	} dynamic_fog;
	void TurnOnDynamicFov(float fSpeed,float fTime, float fRelationMin, float fRelationMax, float fAngSpeed, float fAngMax);
	void ProcessDynamicFov(float fDeltaTime, const CVECTOR &vFrom, const CVECTOR &vTo, CVECTOR &vUp );
	void StoreRestoreDynamicFov(bool bStore);

	// track mode section
	bool m_bTrackMode;
	float m_fTrackCurTime;
	float m_fTrackMaxTime;
	string m_sCurTrackName;
	float m_fPauseTime;

	struct CameraTrackPause
	{
		float trackTime;
		float pauseTime;
	};
	array<CameraTrackPause> m_aTrackPauses;
	long m_nCurPauseIndex;

	bool LoadCameraTrack(const char* pcTrackFile,float fTrackTime);
	void TurnOffTrackCamera();
	void ProcessTrackCamera();
	float TrackPauseProcess();

	PathTracks m_track;
};

inline void LocationCamera::LockFPMode(bool isLock)
{
	isLockFPMode = isLock;
}

inline float LocationCamera::GetAx()
{
	return ax;
}

#endif

