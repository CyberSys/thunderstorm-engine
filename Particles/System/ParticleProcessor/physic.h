#ifndef SIMPLE_PHYSIC_MATH
#define SIMPLE_PHYSIC_MATH



///
/// ================================================================
/// Poisition - ������� � ������� ���������� �������
/// Velocity - ������� ��������
/// Forces - ���� ����������� �� �������
/// UMass - fabsf(�����)
/// Drag - ������������� ����� (0 ��� ������������� .. 1 ������ ���������)
/// TimeScale - ������������ �������...
__forceinline void SolvePhysic (Vector& Position, Vector& Velocity, const Vector& Forces, float UMass, float Drag, float TimeScale)
{
/*
	Drag = 1.0f - Drag;
	if (Drag < 0.0f) Drag = 0.0f;
	if (Drag > 1.0f) Drag = 1.0f;
*/
	Vector Acceleration = Vector (0.0f);
	if (UMass) Acceleration = (Forces / UMass);

	Velocity += Acceleration * TimeScale;

	Position += ((Velocity * Drag) * TimeScale);
}

/// �������� � ������� ����� ���� ����������...
__forceinline void AddGravityForce (Vector& Forces, float Mass, float GravK)
{
	GravK *= 0.01f;
	if (GravK < 0) GravK = 0.0f;
	if (GravK > 1.0f) GravK = 1.0f;

	Forces += Vector (0.0f, -9.8f*Mass*GravK, 0.0f);
}

#endif