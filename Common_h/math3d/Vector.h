//============================================================================================
// Spirenkov Maxim, 2003
//============================================================================================
// Vector
//============================================================================================

#ifndef _Vector_h_
#define _Vector_h_

#include <stdlib.h>
#include <math.h>

#pragma pack(push, 1)


///����� ������������� ������� � 3D ������������
class Vector
{
public:
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
		///������������� � ���� �������
		float v[3];
	};

//-----------------------------------------------------------
//������������
//-----------------------------------------------------------
public:
	///������ �����������
	Vector();
	///��������� ������
	Vector(float f);
	///��������� ������
	Vector(double d);
	///��������� ��� ����������
	Vector(float x, float y, float z);
	///��������� ��� ����������
	Vector(const float f[3]);
	///��������� ��� ����������
	Vector(const double d[3]);
	///����������� �����������
	Vector(const Vector & v);
	
//-----------------------------------------------------------
//���������
//-----------------------------------------------------------
public:
	///����� ������� ������ �������
	float operator ~ () const;
	///������� ��������������� ������
	Vector operator ! () const;

	///�������� ������������� ������
	Vector operator - () const;

	///���������
	Vector & operator = (float f);
	///���������
	Vector & operator = (double d);
	///���������
	Vector & operator = (const Vector & v);
	///�������������� �������� � �������������
	Vector & operator += (float f);
	///�������������� �������� � �������������
	Vector & operator += (double d);
	///�������������� �������� � �������������
	Vector & operator += (const Vector & v);
	///�������������� ��������� � �������������
	Vector & operator -= (float f);
	///�������������� ��������� � �������������
	Vector & operator -= (double d);
	///�������������� ��������� � �������������
	Vector & operator -= (const Vector & v);
	///�������������� ��������� � �������������
	Vector & operator *= (float f);
	///�������������� ��������� � �������������
	Vector & operator *= (double d);
	///�������������� ��������� � �������������
	Vector & operator *= (const Vector & v);
	///�������������� ������� � �������������
	Vector & operator /= (float f);
	///�������������� ������� � �������������
	Vector & operator /= (double d);
	///�������������� ������� � �������������
	Vector & operator /= (const Vector & v);
	
	///��������� ������������, ��������� ���������� �� ��� ����������
	Vector & operator |= (const Vector & v);
	///��������� ������������
	Vector & operator ^= (const Vector & v);

//-----------------------------------------------------------
//��������������
//-----------------------------------------------------------
public:
	///������������� ������, � ������� ��� ������ ������
	float Normalize();
	///������������� �� ��������� XZ � �������������
	float Normalize2D();

	///���������� ������ �������, � ������� ������� ������
	float ClampLength(float clampValue);
	
	///���������� ���������� X ����������
	Vector & ClampX(float min, float max);
	///���������� ���������� Y ����������
	Vector & ClampY(float min, float max);
	///���������� ���������� Z ����������
	Vector & ClampZ(float min, float max);
	///���������� ���������� ����������
	Vector & Clamp(float min, float max);

	///��������� � ������� ����������� ����������
	Vector & Min(const Vector & v);
	///��������� � ������� ������������ ����������
	Vector & Max(const Vector & v);


//-----------------------------------------------------------
//���������
//-----------------------------------------------------------
public:
	///���������� ����� ��������
	Vector & Set(float x, float y, float z);
	///���������� ����� ��������
	Vector & Set(const Vector & v);

	///�������� ���� ����� ���������
	float GetAngle(const Vector & v) const;
	///�������� �������� ���� ����� ��������� � ��������� XZ
	float GetAngle2D(const Vector & v) const;
	///�������� ���� �������� ������� ������ ��� Y
	float GetAY(float defAngle = 0.0f) const;
	
	///�������� ����� ����� 2D ��������� � ��������� XZ
	float Sin2D(const Vector & v) const;
	///�������� ������� ����� 2D ��������� � ��������� XZ
	float Cos2D(const Vector & v) const;
	///�������� ��������� ������������ �������� � ��������� XZ
	float Cross2D(const Vector & v) const;

	///�������� ������ � ��������� XZ
	Vector Get2D() const;
	///�������� ������ � ��������� XZ
	Vector Get2D(float y) const;
	///������������ ������ � ��������� XZ �� ����
	Vector & Make2D(float ay);


	///�������� ������ �������
	float GetLength() const;
	///�������� ������ ������� � 2D
	float GetLength2D() const;
	///�������� ������� ������ ������� � 2D
	float GetLength2D2() const;
	
	///��������� ������ � ��������� XZ �� ����
	Vector & Rotate(float angle);
	///��������� ������ � ��������� XZ �� ���� �������� cos, sin
	Vector & Rotate(float vcos, float vsin);
	///��������� ������ �� ������� ������� � ��������� XZ �� ���� PI/2
	Vector & Rotate_PI2_CW();
	///��������� ������ ������ ������� ������� � ��������� XZ �� ���� PI/2
	Vector & Rotate_PI2_CCW();

	///��������� ������� ����������������� ��������
	Vector & Lerp(const Vector & v1, const Vector & v2, float kBlend);

	///��������� ��������� ������
	Vector & Reflection(const Vector & normal);

	///��������� ��������� �������� �� ��������� ������������
	Vector & Rand();
	///��������� ��������� �������� �� ��������� ������������ � XZ
	Vector & Rand2D();
	///��������� ���������� ���������� � �������� ABB
	Vector & Rand(const Vector & min, const Vector & max);
	///��������� ���������� ���������� � �������� �����
	Vector & Rand(const Vector & pos, float radius);

	//����������� ������� ������ � �������� ����� �� �������� ���
	bool MoveByStep(const Vector & to, float step);
	//����������� ������� ������ � �������� ����� �� �������� ��� � 2D
	bool MoveByStep2D(const Vector & to, float step);

	//����� ���������� � �����
	bool InBox(const Vector & min, const Vector & max) const;
	//����� ���������� � ����
	bool InSphere(const Vector & pos, float rad) const;
};


//===========================================================
//������������
//===========================================================

//������ �����������
mathinline Vector::Vector(){}

//��������� ������
mathinline Vector::Vector(float f)
{
	x = f;
	y = f;
	z = f;
}

//��������� ������
mathinline Vector::Vector(double d)
{
	x = float(d);
	y = float(d);
	z = float(d);
}

//��������� ��� ����������
mathinline Vector::Vector(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

//��������� ��� ����������
mathinline Vector::Vector(const float f[3])
{
	x = f[0];
	y = f[1];
	z = f[2];
}

//��������� ��� ����������
mathinline Vector::Vector(const double d[3])
{
	x = float(d[0]);
	y = float(d[1]);
	z = float(d[2]);
}

//����������� �����������
mathinline Vector::Vector(const Vector & v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}


//===========================================================
//���������
//===========================================================

//����� ������� ������ �������
mathinline float Vector::operator ~ () const
{
	return x*x + y*y + z*z;
}

//������� ��������������� ������
mathinline Vector Vector::operator ! () const
{
	Vector v(*this);
	float length = v.Normalize();
	//assert(fabsf(length) > 1e-20);
	return v;
}

mathinline Vector Vector::operator - () const
{
	Vector v(*this);
	v.x = -v.x;
	v.y = -v.y;
	v.z = -v.z;
	return v;
}

//���������
mathinline Vector & Vector::operator = (float f)
{
	x = f;
	y = f;
	z = f;
	return *this;
}

//���������
mathinline Vector & Vector::operator = (double d)
{
	x = float(d);
	y = float(d);
	z = float(d);
	return *this;
}

//���������
mathinline Vector & Vector::operator = (const Vector & v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

//�������������� �������� � �������������
mathinline Vector & Vector::operator += (float f)
{
	x += f;
	y += f;
	z += f;
	return *this;
}

//�������������� �������� � �������������
mathinline Vector & Vector::operator += (double d)
{
	x += float(d);
	y += float(d);
	z += float(d);
	return *this;
}

//�������������� �������� � �������������
mathinline Vector & Vector::operator += (const Vector & v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

//�������������� ��������� � �������������
mathinline Vector & Vector::operator -= (float f)
{
	x -= f;
	y -= f;
	z -= f;
	return *this;
}

//�������������� ��������� � �������������
mathinline Vector & Vector::operator -= (double d)
{
	x -= float(d);
	y -= float(d);
	z -= float(d);
	return *this;
}

//�������������� ��������� � �������������
mathinline Vector & Vector::operator -= (const Vector & v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

//�������������� ��������� � �������������
mathinline Vector & Vector::operator *= (float f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

//�������������� ��������� � �������������
mathinline Vector & Vector::operator *= (double d)
{
	x *= float(d);
	y *= float(d);
	z *= float(d);
	return *this;
}

//�������������� ��������� � �������������
mathinline Vector & Vector::operator *= (const Vector & v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

//�������������� ������� � �������������
mathinline Vector & Vector::operator /= (float f)
{
	double d = 1.0/f;
	x = float(x*d);
	y = float(y*d);
	z = float(z*d);
	return *this;
}

//�������������� ������� � �������������
mathinline Vector & Vector::operator /= (double d)
{
	d = 1.0/d;
	x = float(x*d);
	y = float(y*d);
	z = float(z*d);
	return *this;
}

//�������������� ������� � �������������
mathinline Vector & Vector::operator /= (const Vector & v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}


//��������� ������������, ��������� ���������� �� ��� ����������
mathinline Vector & Vector::operator |= (const Vector & v)
{
	x = y = z = v.x*x + v.y*y + v.z*z;
	return *this;
}

//��������� ������������
mathinline Vector & Vector::operator ^= (const Vector & v)
{
	float tx = y * v.z - z * v.y;
	float ty = z * v.x - x * v.z;
	float tz = x * v.y - y * v.x;
	x = tx;
	y = ty;
	z = tz;
	return *this;
}

/*!\relates Vector
�������
*/
mathinline Vector operator + (const Vector & v, float f)
{
	Vector tv(v);
	tv += f;
	return tv;
}

/*!\relates Vector
�������
*/
mathinline Vector operator + (float f, const Vector & v)
{
	Vector tv(v);
	tv += f;
	return tv;
}

/*!\relates Vector
�������
*/
mathinline Vector operator + (const Vector & v, double d)
{
	Vector tv(v);
	tv += d;
	return tv;
}

/*!\relates Vector
�������
*/
mathinline Vector operator + (double d, const Vector & v)
{
	Vector tv(v);
	tv += d;
	return tv;
}

/*!\relates Vector
�������
*/
mathinline Vector operator + (const Vector & v1, const Vector & v2)
{
	Vector tv(v1);
	tv += v2;
	return tv;
}

/*!\relates Vector
�������
*/
mathinline Vector operator - (const Vector & v, float f)
{
	Vector tv(v);
	tv -= f;
	return tv;
}

/*!\relates Vector
�������
*/
mathinline Vector operator - (float f, const Vector & v)
{
	Vector tv(v);
	tv -= f;
	return tv;
}

/*!\relates Vector
�������
*/
mathinline Vector operator - (const Vector & v, double d)
{
	Vector tv(v);
	tv -= d;
	return tv;
}

/*!\relates Vector
�������
*/
mathinline Vector operator - (double d, const Vector & v)
{
	Vector tv(v);
	tv -= d;
	return tv;
}

/*!\relates Vector
�������
*/
mathinline Vector operator - (const Vector & v1, const Vector & v2)
{
	Vector tv(v1);
	tv -= v2;
	return tv;
}

/*!\relates Vector
��������
*/
mathinline Vector operator * (const Vector & v, float f)
{
	Vector tv(v);
	tv *= f;
	return tv;
}

/*!\relates Vector
��������
*/
mathinline Vector operator * (float f, const Vector & v)
{
	Vector tv(v);
	tv *= f;
	return tv;
}

/*!\relates Vector
��������
*/
mathinline Vector operator * (const Vector & v, double d)
{
	Vector tv(v);
	tv *= d;
	return tv;
}

/*!\relates Vector
��������
*/
mathinline Vector operator * (double d, const Vector & v)
{
	Vector tv(v);
	tv *= d;
	return tv;
}

/*!\relates Vector
��������
*/
mathinline Vector operator * (const Vector & v1, const Vector & v2)
{
	Vector tv(v1);
	tv *= v2;
	return tv;
}

/*!\relates Vector
���������
*/
mathinline Vector operator / (const Vector & v, float f)
{
	Vector tv(v);
	tv /= f;
	return tv;
}

/*!\relates Vector
���������
*/
mathinline Vector operator / (float f, const Vector & v)
{
	Vector tv(f);
	tv /= v;
	return tv;
}

/*!\relates Vector
���������
*/
mathinline Vector operator / (const Vector & v, double d)
{
	Vector tv(v);
	tv /= d;
	return tv;
}

/*!\relates Vector
���������
*/
mathinline Vector operator / (double d, const Vector & v)
{
	Vector tv(d);
	tv /= v;
	return tv;
}

/*!\relates Vector
���������
*/
mathinline Vector operator / (const Vector & v1, const Vector & v2)
{
	Vector tv(v1);
	tv /= v2;
	return tv;
}

/*!\relates Vector
��������� ������������
*/
mathinline float operator | (const Vector & v1, const Vector & v2)
{	
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

/*!\relates Vector
��������� ������������
*/
mathinline Vector operator ^ (const Vector & v1, const Vector & v2)
{
	Vector v;
	v.x = v1.y*v2.z - v1.z*v2.y;
	v.y = v1.z*v2.x - v1.x*v2.z;
	v.z = v1.x*v2.y - v1.y*v2.x;
	return v;
}

/*!\relates Vector
�������� �������������
*/
mathinline bool operator > (const Vector & v1, const Vector & v2)
{
	if(v1.x > v2.x && v1.y > v2.y && v1.z > v2.z) return true;	
	return false;
}

/*!\relates Vector
�������� �������������
*/
mathinline bool operator >= (const Vector & v1, const Vector & v2)
{
	if(v1.x >= v2.x && v1.y >= v2.y && v1.z >= v2.z) return true;	
	return false;
}

/*!\relates Vector
�������� �������������
*/
mathinline bool operator < (const Vector & v1, const Vector & v2)
{
	if(v1.x < v2.x && v1.y < v2.y && v1.z < v2.z) return true;	
	return false;
}

/*!\relates Vector
�������� �������������
*/
mathinline bool operator <= (const Vector & v1, const Vector & v2)
{
	if(v1.x <= v2.x && v1.y <= v2.y && v1.z <= v2.z) return true;	
	return false;
}

/*!\relates Vector
�������� �������������
*/
mathinline bool operator == (const Vector & v1, const Vector & v2)
{
	if(fabsf(v1.x - v2.x) < 1e-34f && fabsf(v1.y - v2.y) < 1e-34f && fabsf(v1.z - v2.z) < 1e-34f) return true;
	return false;
}

/*!\relates Vector
�������� �������������
*/
mathinline bool operator != (const Vector & v1, const Vector & v2)
{	
	return !(v1 == v2);
}

//===========================================================
//��������������
//===========================================================

//������������� ������, � ������� ��� ������ ������
mathinline float Vector::Normalize()
{
	double len = sqrt(double(x)*double(x) + double(y)*double(y) + double(z)*double(z));
	if(len > 0.0)
	{
		double k = 1.0/len;
		x = float(x*k);
		y = float(y*k);
		z = float(z*k);
	}
	return float(len);
}

//������������� �� ��������� XZ � �������������
mathinline float Vector::Normalize2D()
{
	y = 0.0f;
	double len = sqrt(double(x)*double(x) + double(z)*double(z));
	if(len > 0.0)
	{
		double k = 1.0/len;
		x = float(x*k);
		z = float(z*k);
	}
	return float(len);
}

//���������� ������ �������, � ������� ������� ������
mathinline float Vector::ClampLength(float clampValue)
{
	if(clampValue <= 0.0f)
	{
		x = y = z = 0.0f;
		return 0.0f;
	}
	double len = sqrt(x*x + y*y + z*z);
	if(len > clampValue)
	{
		double k = clampValue/len;
		x = float(x*k);
		y = float(y*k);
		z = float(z*k);
		return clampValue;
	}
	return float(len);
}

//���������� ���������� X ����������
mathinline Vector & Vector::ClampX(float min, float max)
{
	if(x < min) x = min;
	if(x > max) x = max;
	return *this;
}

//���������� ���������� Y ����������
mathinline Vector & Vector::ClampY(float min, float max)
{
	if(y < min) y = min;
	if(y > max) y = max;
	return *this;
}

//���������� ���������� Z ����������
mathinline Vector & Vector::ClampZ(float min, float max)
{
	if(z < min) z = min;
	if(z > max) z = max;
	return *this;
}

//���������� ���������� ����������
mathinline Vector & Vector::Clamp(float min, float max)
{
	if(x < min) x = min;
	if(x > max) x = max;
	if(y < min) y = min;
	if(y > max) y = max;
	if(z < min) z = min;
	if(z > max) z = max;
	return *this;
}

//��������� � ������� ����������� ����������
mathinline Vector & Vector::Min(const Vector & v)
{
	if(x > v.x) x = v.x;
	if(y > v.y) y = v.y;
	if(z > v.z) z = v.z;
	return *this;
}

//��������� � ������� ������������ ����������
mathinline Vector & Vector::Max(const Vector & v)
{
	if(x < v.x) x = v.x;
	if(y < v.y) y = v.y;
	if(z < v.z) z = v.z;
	return *this;
}


//===========================================================
//���������
//===========================================================

//���������� ����� ��������
mathinline Vector & Vector::Set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	return *this;
}

//���������� ����� ��������
mathinline Vector & Vector::Set(const Vector & v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

//�������� ���� ����� ���������
mathinline float Vector::GetAngle(const Vector & v) const
{
	double len = double(x)*double(x) + double(y)*double(y) + double(z)*double(z);
	len *= double(x)*double(x) + double(y)*double(y) + double(z)*double(z);
	if(len <= 0.0) return 0.0f;
	double cs = (x*v.x + y*v.y + z*v.z)/sqrt(len);
	if(cs > 1.0) cs = 1.0;
	if(cs < -1.0) cs = -1.0;
	return float(acos(cs));
}

///�������� �������� ���� ����� ��������� � ��������� XZ
mathinline float Vector::GetAngle2D(const Vector & v) const
{
	double len = double(x)*double(x) + double(z)*double(z);
	len *= double(v.x)*double(v.x) + double(v.z)*double(v.z);
	if(len <= 0.0) return 0.0f;
	len = (x*v.x + z*v.z)/sqrt(len);
	if(len > 1.0) len = 1.0;
	if(len < -1.0) len = -1.0;
	len = acos(len);
	if(z*v.x - x*v.z < 0) len = -len;
	return float(len);
}

//�������� ���� �������� ������� ������ ��� Y
mathinline float Vector::GetAY(float defAngle) const
{
	double len = double(x)*double(x) + double(z)*double(z);
	if(len > 0.00000000001)
	{
		len = z/sqrt(len);
		if(len > 1.0) len = 1.0;
		if(len < -1.0) len = -1.0;
		len = acos(len);
	}else return defAngle;
	if(x < 0) len = -len;
	return float(len);
}

//�������� ����� ����� 2D ��������� � ��������� XZ
mathinline float Vector::Sin2D(const Vector & v) const
{
	double len = double(x)*double(x) + double(z)*double(z);
	len *= double(v.x)*double(v.x) + double(v.z)*double(v.z);
	if(len <= 0.0) return 0.0f;
	len = (z*v.x - x*v.z)/sqrt(len);
	return float(len);
}

//�������� ������� ����� 2D ��������� � ��������� XZ
mathinline float Vector::Cos2D(const Vector & v) const
{
	double len = double(x)*double(x) + double(z)*double(z);
	len *= double(v.x)*double(v.x) + double(v.z)*double(v.z);
	if(len <= 0.0) return 1.0f;
	len = (x*v.x + z*v.z)/sqrt(len);
	return float(len);
}

//�������� ��������� ������������ �������� � ��������� XZ
mathinline float Vector::Cross2D(const Vector & v) const
{
	return z*v.x - x*v.z;
}

//�������� ������ � ��������� XZ
mathinline Vector Vector::Get2D() const
{
	return Vector(x, 0.0f, z);
}

///�������� ������ � ��������� XZ
mathinline Vector Vector::Get2D(float y) const
{
	return Vector(x, y, z);
}

//������������ ������ � ��������� XZ �� ����
mathinline Vector & Vector::Make2D(float ay)
{
	x = sinf(ay);
	y = 0.0f;
	z = cosf(ay);
	return *this;
}

//�������� ������ �������
mathinline float Vector::GetLength() const
{
	return float(sqrt(double(x)*double(x) + double(y)*double(y) + double(z)*double(z)));
}

//�������� ������ ������� � 2D
mathinline float Vector::GetLength2D() const
{
	return float(sqrt(double(x)*double(x) + double(z)*double(z)));
}

///�������� ������� ������ ������� � 2D
mathinline float Vector::GetLength2D2() const
{
	return x*x + z*z;
}

//��������� ������ � ��������� XZ �� ����
mathinline Vector & Vector::Rotate(float angle)
{
	return Rotate(cosf(angle), sinf(angle));
}

//��������� ������ � ��������� XZ �� ���� �������� cos, sin
mathinline Vector & Vector::Rotate(float vcos, float vsin)
{
	float tx = x*vcos + z*vsin;
	float tz = z*vcos - x*vsin;
	x = tx; z = tz;
	return *this;
}

//��������� ������ �� ������� ������� � ��������� XZ �� ���� PI/2
mathinline Vector & Vector::Rotate_PI2_CW()
{
	float t = x;
	x = z;
	z = -t;
	return *this;
}

//��������� ������ ������ ������� ������� � ��������� XZ �� ���� PI/2
mathinline Vector & Vector::Rotate_PI2_CCW()
{
	float t = x;
	x = -z;
	z = t;
	return *this;
}

//��������� ������������������������ ��������
mathinline Vector & Vector::Lerp(const Vector & v1, const Vector & v2, float kBlend)
{
	x = v1.x + (v2.x - v1.x)*kBlend;
	y = v1.y + (v2.y - v1.y)*kBlend;
	z = v1.z + (v2.z - v1.z)*kBlend;
	return *this;
}

//��������� ��������� ������
mathinline Vector & Vector::Reflection(const Vector & normal)
{
	float k = -2.0f*(x*normal.x + y*normal.y + z*normal.z);
	x += normal.x*k;
	y += normal.y*k;
	z += normal.z*k;
	return *this;
}

//��������� ��������� �������� �� ��������� ������������
mathinline Vector & Vector::Rand()
{
	Make2D(rand()*((2.0f*3.141592654f)/RAND_MAX));
	y = z; z = 0.0f;
	return Rotate(rand()*((2.0f*3.141592654f)/RAND_MAX));
}

//��������� ��������� �������� �� ��������� ������������ � XZ
mathinline Vector & Vector::Rand2D()
{
	return Make2D(rand()*((2.0f*3.141592654f)/RAND_MAX));
}

///��������� ���������� ���������� � �������� ABB
mathinline Vector & Vector::Rand(const Vector & min, const Vector & max)
{
	x = min.x + rand()*((max.x - min.x)*(1.0f/RAND_MAX));
	y = min.y + rand()*((max.y - min.y)*(1.0f/RAND_MAX));
	z = min.z + rand()*((max.z - min.z)*(1.0f/RAND_MAX));
	return *this;
}

///��������� ���������� ���������� � �������� �����
mathinline Vector & Vector::Rand(const Vector & pos, float radius)
{
	Rand();
	*this *= rand()*(radius*(1.0f/RAND_MAX));
	return *this;
}

//����������� ������� ������ � �������� ����� �� �������� ���
mathinline bool Vector::MoveByStep(const Vector & to, float step)
{
	if(step <= 0.0f) return false;
	//��������� �� �����
	float dx = to.x - x;
	float dy = to.y - y;
	float dz = to.z - z;
	float dist = dx*dx + dy*dy + dz*dz;
	if(dist < 1e-20f)
	{
		x = to.x;
		y = to.y;
		z = to.z;
		return false;
	}
	dist = sqrtf(dist);
	//�������� � �����
	if(step >= dist)
	{
		x = to.x;
		y = to.y;
		z = to.z;
		return true;
	}
	step /= dist;
	x += dx*step;
	y += dy*step;
	z += dz*step;
	return true;
}

//����������� ������� ������ � �������� ����� �� �������� ��� � 2D
mathinline bool Vector::MoveByStep2D(const Vector & to, float step)
{
	if(step <= 0.0f) return false;
	//��������� �� �����
	float dx = to.x - x;
	float dz = to.z - z;
	float dist = dx*dx + dz*dz;
	if(dist < 1e-20f)
	{
		x = to.x;
		z = to.z;
		return false;
	}
	dist = sqrtf(dist);
	//�������� � �����
	if(step >= dist)
	{
		x = to.x;
		z = to.z;
		return true;
	}
	step /= dist;
	x += dx*step;
	z += dz*step;
	return true;
}

//����� ���������� � �����
mathinline bool Vector::InBox(const Vector & min, const Vector & max) const
{
	if(y < min.y || y > max.y) return false;
	if(x < min.x || x > max.x) return false;
	if(z < min.z || z > max.z) return false;
	return true;
}

//����� ���������� � ����
mathinline bool Vector::InSphere(const Vector & pos, float rad) const
{
	double d = (x - pos.x)*(x - pos.x);
	d += (y - pos.y)*(y - pos.y);
	d += (z - pos.z)*(z - pos.z);
	return d < double(rad)*double(rad);
}

#pragma pack(pop)


#endif
