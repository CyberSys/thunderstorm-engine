//============================================================================================
// Spirenkov Maxim, 2003
//============================================================================================
// Plane
//============================================================================================

#ifndef _Plane_h_
#define _Plane_h_

#pragma pack(push, 1)

#include "Vector.h"

///����� ������������� ��������� � 3D ������������
class Plane
{
public:
	union
	{
		struct
		{
			///�������
			Vector n;
		};
		struct
		{
			///�������
			Vector normal;
		};
		struct
		{
			///�������
			Vector N;
		};
	};
	union
	{
		///��������� �� ������
		float d;
		///��������� �� ������
		float dist;
		///��������� �� ������
		float D;
	};

//-----------------------------------------------------------
//������������
//-----------------------------------------------------------
public:
	///������ �����������
	Plane();
	///������ �����������
	Plane(float Nx, float Ny, float Nz);
	///������ �����������
	Plane(const Vector & normal);
	///������� ���������
	Plane(const Vector & normal, const Vector & point);
	///����������� �����������
	Plane(const Plane & plane);

//-----------------------------------------------------------
//��������������
//-----------------------------------------------------------
public:
	///�������������
	Plane & Normalize();
	///����������� ��������� � �������� �����
	Plane & Move(const Vector & point);


//-----------------------------------------------------------
//���������
//-----------------------------------------------------------
public:
	//����� ��������� �� ��������� (*)
	float Dist(const Vector & point) const;
	//��������� �� ����������� ������� � ���������
	bool Intersection(const Vector & src, const Vector & dst) const;
	//����� ����� ����������� ������� � ���������
	bool Intersection(const Vector & src, const Vector & dst, Vector & res) const;
	//��������� �� ����������� ����� � ���������
	bool IntersectionLine(const Vector & src, const Vector & dst, float & k) const;

};


//===========================================================
//������������
//===========================================================

///������ �����������
mathinline Plane::Plane()
{
}

///������ �����������
mathinline Plane::Plane(float Nx, float Ny, float Nz)
{
	N.x = Nx; N.y = Ny; N.z = Nz; D = 0.0f;
}

///������ �����������
mathinline Plane::Plane(const Vector & normal)
{
	N = normal;
}

///������� ���������
mathinline Plane::Plane(const Vector & normal, const Vector & point)
{
	N = normal; D = normal | point;
}

///����������� �����������
mathinline Plane::Plane(const Plane & plane)
{
	N = plane.N; D = plane.D;
}


//===========================================================
//���������
//===========================================================

/*!\relates Plane
��������� �� ����� �� ���������
*/
mathinline float operator * (const Vector & point, const Plane & plane)
{
	return (plane.N | point) - plane.D;
}

/*!\relates Plane
��������� �� ����� �� ���������
*/
mathinline float operator * (const Plane & plane, const Vector & point)
{
	return (plane.N | point) - plane.D;
}


//===========================================================
//��������������
//===========================================================

///�������������
mathinline Plane & Plane::Normalize()
{
	float d = normal.Normalize();
	if(d != 0.0f) dist /= d; else dist = 0.0f;
	return *this;
}

///����������� ��������� � �������� �����
mathinline Plane & Plane::Move(const Vector & point)
{
	D = (N | point);
	return *this;
}

//===========================================================
//���������
//===========================================================

//����� ��������� �� ��������� (*)
mathinline float Plane::Dist(const Vector & point) const
{
	return *this*point;
}

//��������� �� ����������� ������� � ���������
mathinline bool Plane::Intersection(const Vector & src, const Vector & dst) const
{
	float dsrc = *this*src;
	float ddst = *this*dst;
	return (dsrc*ddst <= 0.0f);
}

//����� ����� ����������� ������� � ���������
mathinline bool Plane::Intersection(const Vector & src, const Vector & dst, Vector & res) const
{
	float dsrc = *this*src;
	float ddst = *this*dst;
	if(dsrc*ddst > 0.0f) return false;
	ddst = dsrc - ddst;
	res = src;
	if(ddst != 0.0f) res += (dst - src)*dsrc/ddst;	
	return true;
}

//��������� �� ����������� ����� � ���������
mathinline bool Plane::IntersectionLine(const Vector & src, const Vector & dst, float & k) const
{
	float dsrc = *this*src;
	float ddst = *this*dst;
	ddst = dsrc - ddst;
	if(fabsf(ddst) <= 1e-30f) return false;
	k = dsrc/ddst;
	return true;
}

#pragma pack(pop)

#endif
