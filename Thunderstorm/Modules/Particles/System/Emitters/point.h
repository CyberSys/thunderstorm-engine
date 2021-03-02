#ifndef POINT_EMITTER_H
#define POINT_EMITTER_H

#include "base.h"


class PointEmitter : public BaseEmitter {
protected:


  //�������� ������� ��� �������� ����� ���������  
  Vector GetNewParticlePosition(float DeltaTime) override;

public:

  // ����������� / ����������
  PointEmitter(ParticleSystem* pSystem);
  virtual ~PointEmitter();
};

#endif
