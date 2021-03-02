//============================================================================================
// Spirenkov Maxim, 2003
//============================================================================================
// Quaternion
//============================================================================================

#ifndef _Quaternion_h_
#define _Quaternion_h_


#include "Vector.h"
#include "Matrix.h"

//============================================================================================

struct D3DXQUATERNION;
#pragma pack(push, 1)

//============================================================================================


///����� ������������� �����������
class Quaternion
{
public:
	union
	{
		struct
		{
			///��������� ���������� �� X
			float x;
			///��������� ���������� �� Y
			float y;
			///��������� ���������� �� Z
			float z;
			///��������� ����������
			float w;
		};
		///������������� � ���� �������
		float q[4];
	};

//-----------------------------------------------------------
//������������
//-----------------------------------------------------------
public:
	///���������� ��������� ����������
	Quaternion();
	///�� ����� ��������
	Quaternion(float ax, float ay, float az);
	///�� ����� ��������
	Quaternion(const Vector & ang);
	///�� �����������
	Quaternion(float qx, float qy, float qz, float qw);
	///�� �������
	Quaternion(const Matrix & mtx);

//-----------------------------------------------------------
//���������
//-----------------------------------------------------------
public:
	///���������
	Quaternion & operator - ();
	///����������
	Quaternion & operator = (const Quaternion & q);
	///�������
	Quaternion & operator += (const Quaternion & q);
	///�������
	Quaternion & operator += (float f);
	///�������
	Quaternion & operator -= (const Quaternion & q);
	///�������
	Quaternion & operator -= (float f);
	///��������
	Quaternion & operator *= (float f);
	///��������
	Quaternion & operator *= (const Quaternion & q);
	///���������
	Quaternion & operator /= (float f);

//-----------------------------------------------------------
//����������
//-----------------------------------------------------------
public:
	///���������� ��������� ����������
	void SetIdentity();
	///��������������� �� ����� ��������
	void Set(float ax, float ay, float az);
	///��������������� �� ����� ��������
	void Set(const Vector & ang);
	///��������������� �� �����������
	void Set(float qx, float qy, float qz, float qw);
	///��������������� �� �������
	void Set(const Matrix & mtx);


//-----------------------------------------------------------
//��������������
//-----------------------------------------------------------
public:
	///�������������
	Quaternion & Normalize();
	///�������� ������������ ����������
	Quaternion & Conjugate();
	///������������� ����������
	Quaternion & Inverse();

	///����������� �������� ������������
	Quaternion & SLerp(const Quaternion & q1, const Quaternion & q2, float kBlend);

//-----------------------------------------------------------
//���������
//-----------------------------------------------------------
public:
	///�������� �� ����������� �������
	void GetMatrix(Matrix & mtx) const;
	///�������� ������ �����������
	float GetLength() const;
	///�������� ������� ������ �����������
	float GetLengthSqr() const;

};


//===========================================================
//������������
//===========================================================

//���������� ��������� ����������
mathinline Quaternion::Quaternion()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;
}

//�� ����� ��������
mathinline Quaternion::Quaternion(float ax, float ay, float az)
{
	Set(ax, ay, az);
}

//�� ����� ��������
mathinline Quaternion::Quaternion(const Vector & ang)
{
	Set(ang);
}

//�� �����������
mathinline Quaternion::Quaternion(float qx, float qy, float qz, float qw)
{
	Set(qx, qy, qz, qw);
}

//�� �������
mathinline Quaternion::Quaternion(const Matrix & mtx)
{
	Set(mtx);
}


//===========================================================
//���������
//===========================================================

//���������
mathinline Quaternion & Quaternion::operator - ()
{
	x = -x;
	y = -y;
	z = -z;
	w = -w;
	return *this;
}

//����������
mathinline Quaternion & Quaternion::operator = (const Quaternion & q)
{
	x = q.x;
	y = q.y;
	z = q.z;
	w = q.w;
	return *this;
}

//�������
mathinline Quaternion & Quaternion::operator += (const Quaternion & q)
{
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;	
	return *this;
}

//�������
mathinline Quaternion & Quaternion::operator += (float f)
{
	x += f;
	y += f;
	z += f;
	w += f;
	return *this;
}

//�������
mathinline Quaternion & Quaternion::operator -= (const Quaternion & q)
{
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
	return *this;
}

//�������
mathinline Quaternion & Quaternion::operator -= (float f)
{
	x -= f;
	y -= f;
	z -= f;
	w -= f;
	return *this;
}

//��������
mathinline Quaternion & Quaternion::operator *= (float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

//��������
mathinline Quaternion & Quaternion::operator *= (const Quaternion & q)
{
	float a = (w + x)*(q.w + q.x);
	float b = (z - y)*(q.y - q.z);
	float c = (x - w)*(q.y - q.z);
	float d = (y + z)*(q.x - q.w);
	float e = (x + z)*(q.x + q.y);
	float f = (x - z)*(q.x - q.y);
	float g = (w + y)*(q.w - q.z);
	float h = (w - y)*(q.w + q.z);
	x = a - ( e + f + g + h)*0.5f; 
	y =-c + ( e - f + g - h)*0.5f;
	z =-d + ( e - f - g + h)*0.5f;
	w = b + (-e - f + g + h)*0.5f;
	return *this;
}

//���������
mathinline Quaternion & Quaternion::operator /= (float f)
{
	double k = 1.0/f;
	x = float(x*k);
	y = float(y*k);
	z = float(z*k);
	w = float(w*k);
	return *this;
}

/*!\relates Quaternion
�������
*/
mathinline Quaternion operator + (float f, const Quaternion & q)
{
	Quaternion tq(q);
	tq += f;
	return tq;
}

/*!\relates Quaternion
�������
*/
mathinline Quaternion operator + (const Quaternion & q, float f)
{
	Quaternion tq(q);
	tq += f;
	return tq;
}

/*!\relates Quaternion
�������
*/
mathinline Quaternion operator + (const Quaternion & q1, const Quaternion & q2)
{
	Quaternion tq(q1);
	tq += q2;
	return tq;
}

/*!\relates Quaternion
�������
*/
mathinline Quaternion operator - (float f, const Quaternion & q)
{
	Quaternion tq(q);
	tq -= f;
	return tq;
}

/*!\relates Quaternion
�������
*/
mathinline Quaternion operator - (const Quaternion & q, float f)
{
	Quaternion tq(q);
	tq -= f;
	return tq;
}

/*!\relates Quaternion
�������
*/
mathinline Quaternion operator - (const Quaternion & q1, const Quaternion & q2)
{
	Quaternion tq(q1);
	tq -= q2;
	return tq;
}

/*!\relates Quaternion
��������
*/
mathinline Quaternion operator * (float f, const Quaternion & q)
{
	Quaternion tq(q);
	tq *= f;
	return tq;
}

/*!\relates Quaternion
��������
*/
mathinline Quaternion operator * (const Quaternion & q, float f)
{
	Quaternion tq(q);
	tq *= f;
	return tq;
}

/*!\relates Quaternion
��������
*/
mathinline Quaternion operator * (const Quaternion & q1, const Quaternion & q2)
{
	Quaternion tq(q1);
	tq *= q2;
	return tq;
}

/*!\relates Quaternion
���������
*/
mathinline Quaternion operator / (const Quaternion & q, float f)
{
	Quaternion tq(q);
	tq /= f;
	return tq;
}

//===========================================================
//����������
//===========================================================

//���������� ��������� ����������
mathinline void Quaternion::SetIdentity()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;
}

//��������������� �� ����� ��������
mathinline void Quaternion::Set(float ax, float ay, float az)
{
	Set(Matrix(ax, ay, az));
}

//��������������� �� ����� ��������
mathinline void Quaternion::Set(const Vector & ang)
{
	Set(ang.x, ang.y, ang.z);
}

///��������������� �� �����������
mathinline void Quaternion::Set(float qx, float qy, float qz, float qw)
{
	x = qx;
	y = qy;
	z = qz;
	w = qw;	
}

//��������������� �� �������
mathinline void Quaternion::Set(const Matrix & mtx)
{
	float tr = mtx.m[0][0] + mtx.m[1][1] + mtx.m[2][2];
	if(tr > 0.0f)
	{
		double s = sqrt(tr + 1.0f);
		w = float(s*0.5);
		s = 0.5/s;
		x = float((mtx.m[1][2] - mtx.m[2][1])*s);
		y = float((mtx.m[2][0] - mtx.m[0][2])*s);
		z = float((mtx.m[0][1] - mtx.m[1][0])*s);
	}else{
		long i, j, k;
		if(mtx.m[1][1] > mtx.m[0][0])
		{
			if(mtx.m[2][2] > mtx.m[1][1])
			{
				i = 2;
				j = 0;
				k = 1;
			}else{
				i = 1;
				j = 2;
				k = 0;
			}
		}else
		if(mtx.m[2][2] > mtx.m[0][0])
		{
			i = 2;
			j = 0;
			k = 1;
		}else{
			i = 0;
			j = 1;
			k = 2;
		}
		double s = sqrt((mtx.m[i][i] - (mtx.m[j][j] + mtx.m[k][k])) + 1.0);
		q[i] = float(s*0.5);
		if(s != 0.0) s = 0.5/s;		
		q[j] = float((mtx.m[i][j] + mtx.m[j][i])*s);
		q[k] = float((mtx.m[i][k] + mtx.m[k][i])*s);
		q[3] = float((mtx.m[j][k] - mtx.m[k][j])*s);
	}
}

//===========================================================
//��������������
//===========================================================

//�������������
mathinline Quaternion & Quaternion::Normalize()
{
	double len = double(x)*double(x) + double(y)*double(y) + double(z)*double(z) + double(w)*double(w);
	if(len <= 0.0) return *this;
	len = 1.0/sqrt(len);
	x = float(x*len);
	y = float(y*len);
	z = float(z*len);
	w = float(w*len);
	return *this;
}

//�������� ������������ ����������
mathinline Quaternion & Quaternion::Conjugate()
{
	x = -x;
	y = -y;
	z = -z;
	return *this;
}

//������������� ����������
mathinline Quaternion & Quaternion::Inverse()
{
	Conjugate();
	Normalize();
	return *this;
}

//����������� �������� ������������
mathinline Quaternion & Quaternion::SLerp(const Quaternion & q1, const Quaternion & q2, float kBlend)
{
	//���� ����� �������������
	double cosomega = q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
	double k = 1.0f;
	//��������� �����������
	if(cosomega < 0.0f)
	{
		cosomega = -cosomega;
		k = -1.0f;
	}
	if(cosomega > 1.0) cosomega = 1.0;
	//����������� ������������
	double k0, k1;
	if(1.0 - cosomega > 0.1)
	{
		//slerp
		double omega = acos(cosomega);
		double sinomega = 1.0/sin(omega);
		k0 = sin(omega*(1.0 - kBlend))*sinomega;
		k1 = sin(omega*kBlend)*sinomega;
	}else{
		//lerp
		k0 = 1.0 - kBlend;
		k1 = kBlend;
	}
	k0 *= k;
	//�������������	
	x = float(q1.x*k0 + q2.x*k1);
	y = float(q1.y*k0 + q2.y*k1);
	z = float(q1.z*k0 + q2.z*k1);
	w = float(q1.w*k0 + q2.w*k1);
	return *this;
}

//===========================================================
//���������
//===========================================================

//�������� �� ����������� �������
mathinline void Quaternion::GetMatrix(Matrix & mtx) const
{
	float xx = x*x*2.0f, xy = x*y*2.0f, xz = x*z*2.0f;
	float yy = y*y*2.0f, yz = y*z*2.0f;
	float zz = z*z*2.0f;
	float wx = w*x*2.0f, wy = w*y*2.0f, wz = w*z*2.0f;
	mtx.m[0][0] = 1.0f - (yy + zz);
	mtx.m[0][1] = xy + wz;
	mtx.m[0][2] = xz - wy;
	mtx.m[0][3] = 0.0f;
	mtx.m[1][0] = xy - wz;
	mtx.m[1][1] = 1.0f - (xx + zz);
	mtx.m[1][2] = yz + wx;
	mtx.m[1][3] = 0.0f;
	mtx.m[2][0] = xz + wy;
	mtx.m[2][1] = yz - wx;
	mtx.m[2][2] = 1.0f - (xx + yy);
	mtx.m[2][3] = 0.0f;
	mtx.m[3][0] = 0.0f;
	mtx.m[3][1] = 0.0f;
	mtx.m[3][2] = 0.0f;
	mtx.m[3][3] = 1.0f;
}

//�������� ������ �����������
mathinline float Quaternion::GetLength() const
{
	return sqrtf(x*x + y*y + z*z + w*w);
}

//�������� ������� ������ �����������
mathinline float Quaternion::GetLengthSqr() const
{
	return x*x + y*y + z*z + w*w;
}


#pragma pack(pop)

#endif

