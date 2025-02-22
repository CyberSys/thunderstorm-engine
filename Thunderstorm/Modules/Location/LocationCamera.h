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


#include "vmodule_api.h"
#include "dx9render.h"
#include "Matrix.h"
#include <vector>
#include <string>
#include "PathTracks.h"

#include "CameraFollow.h"

class MODEL;
class Character;
class Location;

class LocationCamera : public Entity {
  friend CameraFollow;

  enum CameraWorkMode {
    cwm_none = 0,
    cwm_follow,
    //������ ������������� ��������
    cwm_topos,
    //����������� ������ � �������� �������
    cwm_free,
    //������ �������� �������� (����������)
  };

  //--------------------------------------------------------------------------------------------
  //���������������, ������������������
  //--------------------------------------------------------------------------------------------
public:
  LocationCamera();
  virtual ~LocationCamera();

  //�������������
  bool Init() override;
  //����������
  void Execute(uint32_t delta_time);
  void Realize(uint32_t delta_time);
  //���������
  uint64_t ProcessMessage(MESSAGE& message) override;
  //��������� ��������
  uint32_t AttributeChanged(ATTRIBUTES* apnt) override;

  void ProcessStage(Stage stage, uint32_t delta) override {
    switch (stage) {
    case Stage::execute:
      Execute(delta);
      break;
    case Stage::realize:
      Realize(delta);
      break;
      /*case Stage::lost_render:
        LostRender(delta); break;
      case Stage::restore_render:
        RestoreRender(delta); break;*/
    }
  }


  void LockFPMode(bool isLock);
  float GetAx() const;

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
  bool MoveFollow(CVECTOR& pos, const CVECTOR& cpos, const CVECTOR& to);

  //���������� ��� ����� �������
  float Trace(const CVECTOR& src, const CVECTOR& dst) const;
  bool GetCollideTriangle(TRIANGLE& trg) const;
  void Clip(PLANE* p, long numPlanes, CVECTOR& cnt, float rad, bool (* fnc)(const CVECTOR* vtx, long num)) const;


private:
  VDX9RENDER* rs;
  //����
  entid_t sea;
  //��������� ������
  float ax; //���� ������� ������
  float lAx;
  float vAx; //������� �������� ���������
  float axmin, axmax; //���� ������� ������
  float teleport_dist; //������������ ��������� ��� ������� ������ ���������������
  float lookHeight; //�������� ���� �������� �� ������ (�������������)
  float radius; //������ ���������� �� ���������
  float rotInertia; //������� ��������
  float radInertia; //������� ����������� ��������

  bool isSpecialMode;
  bool isLockFPMode;
  bool isViewExecute;

  entid_t loc;
  Location* location;

  //����� ������
  CameraWorkMode wmode;
  bool isSleep;
  bool lockAx;

  float dAx, dAy;

  CVECTOR camPos, lookTo; //������� ��������� ������	
  CVECTOR oldPos, oldLookTo; //��������� ������� ������ ����������� ������	
  float kMorph; //������� ���������� ��������

  //��������
  entid_t chr; //��������
  entid_t patchMdl; //���� ��� ������

  CameraFollow cf; //������ ��������� �� ����������

  float obstructTime; //����� ����������� ���������
  CVECTOR obstruct; //�����


  float freeAx, freeAy; //���� ��������� ������

  bool isLookMode;
  bool isTeleport;

  //������ ����������� �� ���������� �� �����
  CVECTOR fromLook;

  //��������� �������� �� ������ ����������
  Character* character; //��������� �� ���������
  CVECTOR pos; //��������� ���������
  float chay; //����������� ���������
  float lheight; //������ ���� ��������
  float height; //������ ���������
  float chradius; //������ ���������

  //������������ ��������� ����������� (��������)
  //~!~ rearrange this!
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

  void TurnOnDynamicFov(float fSpeed, float fTime, float fRelationMin, float fRelationMax, float fAngSpeed,
                        float fAngMax);
  void ProcessDynamicFov(float fDeltaTime, const CVECTOR& vFrom, const CVECTOR& vTo, CVECTOR& vUp);
  void StoreRestoreDynamicFov(bool bStore);

  // track mode section
  bool m_bTrackMode;
  float m_fTrackCurTime;
  float m_fTrackMaxTime;
  std::string m_sCurTrackName;
  float m_fPauseTime;

  struct CameraTrackPause {
    float trackTime;
    float pauseTime;
  };

  std::vector<CameraTrackPause> m_aTrackPauses;
  long m_nCurPauseIndex;

  bool LoadCameraTrack(const char* pcTrackFile, float fTrackTime);
  void TurnOffTrackCamera();
  void ProcessTrackCamera();
  float TrackPauseProcess();

  PathTracks m_track;
};

inline void LocationCamera::LockFPMode(bool isLock) {
  isLockFPMode = isLock;
}

inline float LocationCamera::GetAx() const {
  return ax;
}

#endif
