//============================================================================================
// Spirenkov Maxim, 2003
//============================================================================================
// Vector4
//============================================================================================

#ifndef _Vector4_h_
#define _Vector4_h_

#include "Vector.h"

#pragma pack(push, 1)


///����� ������������� ������� c ������� ������������ � 3D ������������
struct Vector4
{
public:
	union
	{
		struct
		{
			union
			{
				struct
				{
					///���������� �� X
					float x;
					///���������� �� Y
					float y;
					///���������� �� Z
					float z;
				};
				struct
				{
					///������������� � ���� �������
					Vector v;
				};
			};
			///������� ����������
			float w;
		};
		struct
		{
			///������������� � ���� �������
			float v4[4];
		};
	};

//-----------------------------------------------------------
//������������
//-----------------------------------------------------------
public:
	///������ �����������
	Vector4();
	///��������� ������
	Vector4(float f);
	///��������� ������
	Vector4(double d);
	///��������� 3 ����������, 1
	Vector4(float x, float y, float z);
	///��������� ��� ����������
	Vector4(float x, float y, float z, float w);
	///��������� 3 ����������, 1
	Vector4(const float f[3]);
	///��������� 3 ����������, 1
	Vector4(const double d[3]);
	///��������� 3 ����������, 1
	Vector4(const Vector & v);
	///����������� �����������
	Vector4(const Vector4 & v);

//-----------------------------------------------------------
//���������
//-----------------------------------------------------------
public:
	///����� ������� ������ �������
	float operator ~ () const;

	///�������� ������������� ������
	Vector4 operator - () const;

	///���������
	Vector4 & operator = (float f);
	///���������
	Vector4 & operator = (double d);
	///���������
	Vector4 & operator = (const Vector & v);
	///���������
	Vector4 & operator = (const Vector4 & v);

//-----------------------------------------------------------
//��������������
//-----------------------------------------------------------
public:
	///������������� w ����������
	void Normalize();

};

//===========================================================
//������������
//===========================================================

//������ �����������
mathinline Vector4::Vector4(){}

//��������� ������
mathinline Vector4::Vector4(float f) : v(f)
{
	w = f;
}

//��������� ������
mathinline Vector4::Vector4(double d)
{
	w = float(d);
}

//��������� 3 ����������, 1
mathinline Vector4::Vector4(float x, float y, float z) : v(x, y, z)
{
	w = 1.0f;	
}

//��������� ��� ����������
mathinline Vector4::Vector4(float x, float y, float z, float w) : v(x, y, z)
{
	this->w = w;
}

//��������� 3 ����������, 1
mathinline Vector4::Vector4(const float f[3]) : v(f[0], f[1], f[2])
{
	w = 1.0f;
}

//��������� 3 ����������, 1
mathinline Vector4::Vector4(const double d[3]) : v(float(d[0]), float(d[1]), float(d[2]))
{
	w = 1.0f;
}

//��������� 3 ����������, 1
mathinline Vector4::Vector4(const Vector & vc) : v(vc)
{
	w = 1.0f;	
}

//����������� �����������
mathinline Vector4::Vector4(const Vector4 & v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}

//===========================================================
//���������
//===========================================================

///����� ������� ������ �������
mathinline float Vector4::operator ~ () const
{
	return x*x + y*y + z*z + w*w;
}

///�������� ������������� ������
mathinline Vector4 Vector4::operator - () const
{
	Vector4 v(*this);
	v.x = -v.x;
	v.y = -v.y;
	v.z = -v.z;
	v.w = -v.w;
	return v;
}

///���������
mathinline Vector4 & Vector4::operator = (float f)
{
	v = f;
	w = 1.0f;
	return *this;
}

///���������
mathinline Vector4 & Vector4::operator = (double d)
{
	v = d;
	w = 1.0f;
	return *this;
}

///���������
mathinline Vector4 & Vector4::operator = (const Vector & v)
{
	this->v = v;
	w = 1.0f;
	return *this;
}

///���������
mathinline Vector4 & Vector4::operator = (const Vector4 & v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}


//===========================================================
//��������������
//===========================================================

//������������� w ����������
mathinline void Vector4::Normalize()
{
	double k = 1.0/w;
	x = float(k*x);
	y = float(k*y);
	z = float(k*z);
	w = 1.0f;
}

#pragma pack(pop)

#endif
