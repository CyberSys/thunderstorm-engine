#include "point.h"

// ����������� / ����������
PointEmitter::PointEmitter(ParticleSystem* pSystem) : BaseEmitter(pSystem)
{
}

PointEmitter::~PointEmitter()
{
}

 
//�������� ������� ��� �������� ����� ���������  
Vector PointEmitter::GetNewParticlePosition (float DeltaTime)
{
	//TODO : ���� ������� �� ��������� ������� �����...
	return Vector (0.0f);
}

 
  
  
