#ifndef POINT_EMITTER_H
#define POINT_EMITTER_H

#include "base.h"


class PointEmitter : public BaseEmitter
{

protected:

 
//�������� ������� ��� �������� ����� ���������  
  virtual Vector GetNewParticlePosition (float DeltaTime);

public:
 
	// ����������� / ����������
  PointEmitter(ParticleSystem* pSystem);
  virtual ~PointEmitter();
  
  


};

#endif