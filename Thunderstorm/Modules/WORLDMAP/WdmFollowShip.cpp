//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	WdmFollowShip
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#include "WdmFollowShip.h"
#include "WdmIslands.h"

//============================================================================================
//���������������, ������������������
//============================================================================================

WdmFollowShip::WdmFollowShip() {
  isEnemy = true;
  shipType = wdmest_follow;
  mtx.Pos().x = wdmObjects->worldSizeX;
  mtx.Pos().z = wdmObjects->worldSizeZ;
}

WdmFollowShip::~WdmFollowShip() {
}

//��������� ����������
bool WdmFollowShip::Load(const char* modelName) {
  if (!WdmShip::Load(modelName)) return false;
  //���� �������
  float x, z, ay = 0.0f;
  killMe = !GeneratePosition(1.2f * modelRadius, 6.28f, x, z);
  if (wdmObjects->playerShip) {
    //���������� ���
    const double sn = wdmObjects->playerShip->mtx.Pos().x - mtx.Pos().x;
    double cs = wdmObjects->playerShip->mtx.Pos().z - mtx.Pos().z;
    const auto l = sn * sn + cs * cs;
    if (l > 0.0) cs /= sqrt(l);
    if (cs > 1.0) cs = 1.0;
    if (cs < -1.0) cs = -1.0;
    cs = acos(cs);
    if (sn < 0.0) cs = -cs;
    ay = static_cast<float>(cs);
  }
  //������ ��������
  Teleport(x, z, ay);
  return true;
}

//����� ���� ������������� � ������ �����������
void WdmFollowShip::FindMoveForce() {
  CVECTOR dir;
  if (wdmObjects->islands) {
    wdmObjects->islands->FindDirection(mtx.Pos(), static_cast<WdmShip*>(wdmObjects->playerShip)->mtx.Pos(), dir);
  }
  else {
    //������� ������
    dir.x = static_cast<WdmShip*>(wdmObjects->playerShip)->mtx.Pos().x;
    dir.z = static_cast<WdmShip*>(wdmObjects->playerShip)->mtx.Pos().z;
    //����������� �� ������
    dir.x -= mtx.Pos().x;
    dir.z -= mtx.Pos().z;
    auto dl = dir.x * dir.x + dir.z * dir.z;
    if (dl > 1.0f) {
      dl = 1.0f / sqrtf(dl);
      dir.x *= dl;
      dir.z *= dl;
    }
  }
  mx = dir.x;
  mz = dir.z;
}
