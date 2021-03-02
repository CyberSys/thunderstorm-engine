#ifndef PARTICLE_STRUCT
#define PARTICLE_STRUCT


#include "../../common_h/math3d.h"

class DataGraph;
class DataColor;
class DataUV;
class IEmitter;
class GEOS;

struct BB_ParticleData
{
	bool SpeedOriented; //��������������� �� ������� ��������...

//��������� �� ���-�� ��������� ������� ����, ��� �������� �������� ����� ��������� !!!
	DWORD* ActiveCount;

//���������� �������
  Vector PhysPos;

//���� �������� 
  float Angle;

//�������� ������������, ������ � ���.
  float Spin;

//����������� � "����" �������� (�� ���������������)  
	Vector Velocity;

//������� ���� (��� ������������� �� � ��������)
	Vector ExternalForce;

//����� �����  
  float LifeTime;
  
//������� ������ �������  
  float ElapsedTime;
  
//������� �������������, ��� �������� �������� (��� �����)
  Matrix matWorld;
  
//�����  
  float Mass;
  
//fabsf(�����)  
  float UMass;

  
//===========================================================================
//��������� �������/���� ��� �������
  Vector RenderPos;
  float RenderAngle;
	Vector OldRenderPos;
	float OldRenderAngle;

//===========================================================================
	//��������� �� ������
	float CamDistance;
	bool Visible;

//===========================================================================
//�������.........

	DataGraph* Graph_SpinDrag;
	DataGraph* Graph_Drag;
	DataGraph* Graph_Size;
	DataGraph* Graph_Frames;
	DataColor* Graph_Color;
	DataUV* Graph_UV;
	DataGraph* Graph_Transparency;
	DataGraph* Graph_TrackX;
	DataGraph* Graph_TrackY;
	DataGraph* Graph_TrackZ;
	DataGraph* Graph_PhysBlend;
	DataGraph* graph_GravK;
	DataGraph* graph_AddPower;


//===========================================================================
//���������� �������������
	float DragK;
	float SpinDragK;
	float SizeK;
	float ColorK;
	float AlphaK;
	float FrameK;
	float GravKK;
	float AddPowerK;
	float KTrackX;
	float KTrackY;
	float KTrackZ;
	float KPhysBlend;


//===========================================================================
	//��������� �� ������� ������� ��������� � ��������
	IEmitter* AttachedEmitter;

//===============================================
// ID �������� �������� ����������� �������
	DWORD EmitterGUID;
};  



struct MDL_ParticleData
{

	//��������� �� ���-�� ��������� ������� ����, ��� �������� �������� ����� ��������� !!!
	DWORD* ActiveCount;

	//���������� �������
	Vector PhysPos;

	//���� �������� 
	Vector Angle;

	//�������� ������������, ������ � ���.
	Vector Spin;

	//����������� � "����" �������� (�� ���������������)  
	Vector Velocity;

	//������� ���� (��� ������������� �� � ��������)
	Vector ExternalForce;

	//����� �����  
	float LifeTime;

	//������� ������ �������  
	float ElapsedTime;

	//������� �������������, ��� �������� �������� (��� �����)
	Matrix matWorld;

	//�����  
	float Mass;

	//fabsf(�����)  
	float UMass;


	//===========================================================================
	//��������� �������/���� ��� �������
	Vector RenderPos;
	Vector RenderAngle;
	Vector OldRenderPos;
	Vector OldRenderAngle;

	//===========================================================================
	//�������.........

	DataGraph* Graph_SpinDragX;
	DataGraph* Graph_SpinDragY;
	DataGraph* Graph_SpinDragZ;
	DataGraph* Graph_Drag;
	DataGraph* Graph_TrackX;
	DataGraph* Graph_TrackY;
	DataGraph* Graph_TrackZ;
	DataGraph* Graph_PhysBlend;
	DataGraph* graph_GravK;


	//===========================================================================
	//���������� �������������
	float DragK;
	float SpinDragK_X;
	float SpinDragK_Y;
	float SpinDragK_Z;
	float GravKK;
	float KTrackX;
	float KTrackY;
	float KTrackZ;
	float KPhysBlend;



	//===========================================================================
	//��������� �� ������ ��� �������
	GEOS* pScene;

	//===========================================================================
	//��������� �� ������� ������� ��������� � ��������
	IEmitter* AttachedEmitter;

	//===============================================
	// ID �������� �������� ����������� �������
	DWORD EmitterGUID;
};  


#endif