//============================================================================================
// Spirenkov Maxim, 2003
//============================================================================================
// Matrix
//============================================================================================

#ifndef _Matrix_h_
#define _Matrix_h_

#include "Vector4.h"
#include "Plane.h"

//============================================================================================

struct D3DXMATRIX;

#pragma pack(push, 1)


//============================================================================================
/*!
<PRE>

�������� �������������        � ���� ����������� �������
                                vx     vy     vz    pos
    0  4  8 12                [0][0] [1][0] [2][0] [3][0]    x
    1  5  9 13                [0][1] [1][1] [2][1] [3][1]    y
    2  6 10 14                [0][2] [1][2] [2][2] [3][2]    z
    3  7 11 15                [0][3] [1][3] [2][3] [3][3]    w
*/
///����� ������� ��� �������������� � 3D.
//============================================================================================

class Matrix
{
public:
	union
	{
		///�������� ������
		float matrix[16];
		///��������� ������
		float m[4][4];
		struct
		{
			///����������� �� X
			Vector vx;
			///������� �������� �� X
			float wx;
			///����������� �� Y
			Vector vy;
			///������� �������� �� Y
			float wy;
			///����������� �� Z
			Vector vz;
			///������� �������� �� Z
			float wz;
			///�������
			Vector pos;
			//����������� ������� ��������
			float w;
		};
	};

//-----------------------------------------------------------
//������������
//-----------------------------------------------------------
public:
	///��������������� ��������� �������
	Matrix();
	///��������������� ������� ��� ����������
	Matrix(bool empty);
	///��������������� ������� �� ����� � ��������
	Matrix(float angX, float angY, float angZ, float x, float y, float z);
	///��������������� ������� �� �����
	Matrix(float angX, float angY, float angZ);
	///��������������� ������� �� ����� � ��������
	Matrix(const Vector & ang, const Vector & pos);
	///��������������� ������� �� �����
	Matrix(const Vector & ang);
	///��������������� ����� �������
	Matrix(const Matrix & matrix);
	///��������������� ��������� ������������ ������ this = m1*m2
	Matrix(const Matrix & m1, const Matrix & m2);

//-----------------------------------------------------------
//���������
//-----------------------------------------------------------
public:
	//��������� ������� ������ �������
	Matrix & operator = (const Matrix & mtx);
	//��������� ������� ������� �����
	Matrix & operator = (float f);
	//��������� ������� ������� �����
	Matrix & operator = (double d);
	//��������� ������� ������� ������
	Matrix & operator = (const Vector & v);

	//����������� �������
	Matrix & operator *= (const Matrix & mtx);

	
//-----------------------------------------------------------
//���������� �������
//-----------------------------------------------------------
public:
	///���������� ��������� �������
	Matrix & SetIdentity();

	///���������� �������
	Matrix & Set(const Matrix & matrix);
	
	///��������� ������� M = rotZ*rotX*rotY*Pos
	Matrix & Build(float angX, float angY, float angZ, float x, float y, float z);
	///��������� ������� M = rotZ*rotX*rotY
	Matrix & Build(float angX, float angY, float angZ);
	///��������� ������� M = rotZ*rotX*rotY*Pos
	Matrix & Build(const Vector & ang, const Vector & pos);
	///��������� ������� M = rotZ*rotX*rotY
	Matrix & Build(const Vector & ang);

	///��������� ������� M = rotX*rotY*rotZ*Pos
	Matrix & BuildXYZ(float angX, float angY, float angZ, float x, float y, float z);

	///��������� ������� �������� ������ X
	Matrix & BuildRotateX(float ang);
	///��������� ������� �������� ������ Y
	Matrix & BuildRotateY(float ang);
	///��������� ������� �������� ������ Z
	Matrix & BuildRotateZ(float ang);
	///��������� ������� �������
	Matrix & BuildPosition(float x, float y, float z);
	///��������� ������� �������
	Matrix & BuildPosition(const Vector & pos);

	///��������� ������� ���������������
	Matrix & BuildScale(float scale);
	///��������� ������� ���������������
	Matrix & BuildScale(float scaleX, float scaleY, float scaleZ);
	///��������� ������� ���������������
	Matrix & BuildScale(const Vector & scale);

	///��������� ������� ��������
	Matrix & BuildProjection(float viewAngle, float vpWidth, float vpHeight, float zNear, float zFar);
	///��������� ������� ������
	bool BuildView(Vector lookFrom, Vector lookTo, Vector upVector);
	///��������� ������� ���������� ������� ���� ����������� �� z � ����������� �����
	bool BuildOrient(Vector zAxisDirection, Vector upVector);
	///��������� ������� ������� ���� ������� ����� ���� ��������� ������ � ����������� �����
	bool BuildOriented(Vector position, Vector lookTo, Vector upVector);
	///��������� ������� ��� �������������� ���������
	Matrix & BuildMirror(float Nx, float Ny, float Nz, float D);
	///��������� ������������� ������� ��������
	Matrix & BuildOrtoProjection(float vpWidth, float vpHeight, float zNear, float zFar);
	//��������� ������� �������� ��� ����
	Matrix & BuildShadowProjection(float viewAngle, float vpWidth, float vpHeight, float zNear, float zFar);

//-----------------------------------------------------------
//�������������� �������
//-----------------------------------------------------------
public:
	///��������� ������ X
	Matrix & RotateX(float ang);
	///��������� ������ Y
	Matrix & RotateY(float ang);
	///��������� ������ Z
	Matrix & RotateZ(float ang);
	///��������� ������ ZXY
	Matrix & Rotate(float angX, float angY, float angZ);
	///��������� ������ ZXY
	Matrix & Rotate(const Vector & ang);
	
	///�����������
	Matrix & Move(float dX, float dY, float dZ);
	///�����������
	Matrix & Move(const Vector & pos);

	///����������������
	Matrix & Scale(float scale);
	///���������������� ������� ��������
	Matrix & Scale3x3(float scale);
	///����������������
	Matrix & Scale(float scaleX, float scaleY, float scaleZ);
	///���������������� ������� ��������
	Matrix & Scale3x3(float scaleX, float scaleY, float scaleZ);
	///����������������
	Matrix & Scale(const Vector & scale);
	///���������������� ������� ��������
	Matrix & Scale3x3(const Vector & scale);
	
	///������ �������� �������
	Matrix & Inverse();
	///������ �������� ������� �� ������
	Matrix & Inverse(const Matrix & mtx);
	///������ ���������������� �������� �������
	Matrix & InverseWhithScale();
	///���������������� �������
	Matrix & Transposition();
	///���������������� ��������� ��������
	Matrix & Transposition3X3();



//-----------------------------------------------------------
//���������
//-----------------------------------------------------------
public:
	///������� ������ ��������
	Matrix & SetRotate(const Matrix & mtr);
	///����������� ������� � ��������� ��������� � �������
	Matrix & EqMultiply(const Matrix & m1, const Matrix & m2);
	///����������� ������� � ��������� ��������� � ������� m1 != this && m2 != this
	Matrix & EqMultiplyFast(const Matrix & m1, const Matrix & m2);

	///�������� ������� �� �������
	Vector MulVertex(const Vector & v) const;
	///�������� ������� �� �������
	Vector MulNormal(const Vector & v) const;
	///�������� ������� �� ��������� �������
	Vector MulVertexByInverse(const Vector & v) const;
	///�������� ������� �� ��������� �������
	Vector MulNormalByInverse(const Vector & v) const;

	///�������� ������� ������ �� ������� ������
	Vector GetCamPos() const;
	///��������� ������� ��� ���
	bool IsIdentity() const;
	///������������ ������� ��� ���
	bool IsScale() const;

	///������������ ������� (��� ������� ��������)
	Vector4 Projection(Vector vertex, float vphWidth05 = 1.0f, float vphHeight05 = 1.0f) const;
	///������������ ������ ������ (��� ������� ��������)
	void Projection(Vector4 * dstArray, Vector * srcArray, long num, float vphWidth05 = 1.0f, float vphHeight05 = 1.0f, long srcSize = sizeof(Vector), long dstSize = sizeof(Vector4)) const;

	//�������� ���� �� ��������������� ������� ��������
	void GetAngles(float & ax, float & ay, float & az);
	//�������� ���� �� ��������������� ������� ��������
	void GetAngles(Vector & ang);
	
	///����������� �� ��������� ������� ����� ������
	float & operator () (long i, long j);

	///�������� ��������� �� ������� D3D
	operator D3DXMATRIX * () const;

	///�������� ������ ��� ������� X ����������
	Vector4 GetVectorX() const;
	///�������� ������ ��� ������� Y ����������
	Vector4 GetVectorY() const;
	///�������� ������ ��� ������� Z ����������
	Vector4 GetVectorZ() const;
	///�������� ������ ��� ������� W ����������
	Vector4 GetVectorW() const;
};


//===========================================================
//������������
//===========================================================

//��������������� ��������� �������
mathinline Matrix::Matrix()
{
	SetIdentity();
}

///��������������� ������� ��� ����������
mathinline Matrix::Matrix(bool empty)
{
}

//��������������� ������� �� ����� � ��������
mathinline Matrix::Matrix(float angX, float angY, float angZ, float x, float y, float z)
{
	Build(angX, angY, angZ, x, y, z);
}

//��������������� ������� �� �����
mathinline Matrix::Matrix(float angX, float angY, float angZ)
{
	Build(angX, angY, angZ);
}

//��������������� ������� �� ����� � ��������
mathinline Matrix::Matrix(const Vector & ang, const Vector & pos)
{
	Build(ang, pos);
}

//��������������� ������� �� �����
mathinline Matrix::Matrix(const Vector & ang)
{
	Build(ang);
}

//��������������� ����� �������
mathinline Matrix::Matrix(const Matrix & matrix)
{
	Set(matrix);
}

//��������������� ��������� ������������ ������ this = m1*m2
mathinline Matrix::Matrix(const Matrix & m1, const Matrix & m2)
{
	EqMultiply(m1, m2);
}


//===========================================================
//���������
//===========================================================

//��������� ������� ������ �������
mathinline Matrix & Matrix::operator = (const Matrix & mtx)
{
	Set(mtx);
	return *this;
}

//��������� ������� ������� �����
mathinline Matrix & Matrix::operator = (float f)
{
	pos = f;
	return *this;
}

//��������� ������� ������� �����
mathinline Matrix & Matrix::operator = (double d)
{
	pos = d;
	return *this;
}

//��������� ������� ������� ������
mathinline Matrix & Matrix::operator = (const Vector & v)
{
	pos = v;
	return *this;
}

//����������� �������
mathinline Matrix & Matrix::operator *= (const Matrix & mtx)
{
	EqMultiply(*this, mtx);
	return *this;
}

/*!\relates Matrix
����������� �������
*/
mathinline Matrix operator * (const Matrix & m1, const Matrix & m2)
{
	Matrix m;
	m.EqMultiplyFast(m1, m2);
	return m;
}

/*!\relates Matrix
�������� ������ �� �������
*/
mathinline Vector operator * (const Matrix & mtx, const Vector & v)
{
	return mtx.MulVertex(v);
}

/*!\relates Matrix
�������� ������ �� �������
*/
mathinline Vector operator * (const Vector & v, const Matrix & mtx)
{
	return mtx.MulVertex(v);
}
	
//===========================================================
//���������� �������
//===========================================================

//���������� ��������� �������
mathinline Matrix & Matrix::SetIdentity()
{
	_asm
	{
		mov		eax, this
		mov		ecx, 0x3f800000
		xor		ebx, ebx
		mov		[eax + 0*4], ecx
		mov		[eax + 1*4], ebx
		mov		[eax + 2*4], ebx
		mov		[eax + 3*4], ebx
		mov		[eax + 4*4], ebx
		mov		[eax + 5*4], ecx
		mov		[eax + 6*4], ebx
		mov		[eax + 7*4], ebx
		mov		[eax + 8*4], ebx
		mov		[eax + 9*4], ebx
		mov		[eax + 10*4], ecx
		mov		[eax + 11*4], ebx
		mov		[eax + 12*4], ebx
		mov		[eax + 13*4], ebx
		mov		[eax + 14*4], ebx
		mov		[eax + 15*4], ecx
	}
	return *this;
}

//���������� �������
mathinline Matrix & Matrix::Set(const Matrix & matrix)
{
	_asm
	{
		mov		esi, matrix
		mov		edi, this
		mov		eax, [esi + 0*4]
		mov		ebx, [esi + 1*4]
		mov		ecx, [esi + 2*4]
		mov		edx, [esi + 3*4]
		mov		[edi + 0*4], eax
		mov		[edi + 1*4], ebx
		mov		[edi + 2*4], ecx
		mov		[edi + 3*4], edx
		mov		eax, [esi + 4*4]
		mov		ebx, [esi + 5*4]
		mov		ecx, [esi + 6*4]
		mov		edx, [esi + 7*4]
		mov		[edi + 4*4], eax
		mov		[edi + 5*4], ebx
		mov		[edi + 6*4], ecx
		mov		[edi + 7*4], edx
		mov		eax, [esi + 8*4]
		mov		ebx, [esi + 9*4]
		mov		ecx, [esi + 10*4]
		mov		edx, [esi + 11*4]
		mov		[edi + 8*4], eax
		mov		[edi + 9*4], ebx
		mov		[edi + 10*4], ecx
		mov		[edi + 11*4], edx
		mov		eax, [esi + 12*4]
		mov		ebx, [esi + 13*4]
		mov		ecx, [esi + 14*4]
		mov		edx, [esi + 15*4]
		mov		[edi + 12*4], eax
		mov		[edi + 13*4], ebx
		mov		[edi + 14*4], ecx
		mov		[edi + 15*4], edx
	}
	return *this;
}

//��������� ������� M = rotZ*rotX*rotY*Pos
mathinline Matrix & Matrix::Build(float angX, float angY, float angZ, float x, float y, float z)
{
	//������ � �������� ����� ��������
	float sinAx = sinf(angX);
	float cosAx = cosf(angX);
	float sinAy = sinf(angY);
	float cosAy = cosf(angY);
	float sinAz = sinf(angZ);
	float cosAz = cosf(angZ);	
	//������ ������� � �������� �������� rz*rx*ry
	m[0][0] = cosAz*cosAy + sinAz*sinAx*sinAy;		//vx.x
	m[0][1] = sinAz*cosAx;							//vx.y
	m[0][2] = cosAz*-sinAy + sinAz*sinAx*cosAy;		//vx.z
	m[0][3] = 0.0f;
	m[1][0] = -sinAz*cosAy + cosAz*sinAx*sinAy;		//vy.x
	m[1][1] = cosAz*cosAx;							//vy.y
	m[1][2] = -sinAz*-sinAy + cosAz*sinAx*cosAy;	//vy.z
	m[1][3] = 0.0f;
	m[2][0] = cosAx*sinAy;							//vz.x
	m[2][1] = -sinAx;								//vz.y
	m[2][2] = cosAx*cosAy;							//vz.z
	m[2][3] = 0.0f;
	m[3][0] = x;									//pos.x
	m[3][1] = y;									//pos.y
	m[3][2] = z;									//pos.z
	m[3][3] = 1.0f;
	return *this;
}

//��������� ������� M = rotZ*rotX*rotY
mathinline Matrix & Matrix::Build(float angX, float angY, float angZ)
{
	Build(angX, angY, angZ, 0.0f, 0.0f, 0.0f);
	return *this;
}

//��������� ������� M = rotZ*rotX*rotY*Pos
mathinline Matrix & Matrix::Build(const Vector & ang, const Vector & pos)
{
	Build(ang.x, ang.y, ang.z, pos.x, pos.y, pos.z);
	return *this;
}

//��������� ������� M = rotZ*rotX*rotY
mathinline Matrix & Matrix::Build(const Vector & ang)
{
	Build(ang.x, ang.y, ang.z, 0.0f, 0.0f, 0.0f);
	return *this;
}


//��������� ������� M = rotX*rotY*rotZ*Pos
mathinline Matrix & Matrix::BuildXYZ(float angX, float angY, float angZ, float x, float y, float z)
{
	//������ � �������� ����� ��������
	float sinAx = sinf(angX);
	float cosAx = cosf(angX);
	float sinAy = sinf(angY);
	float cosAy = cosf(angY);
	float sinAz = sinf(angZ);
	float cosAz = cosf(angZ);
	//������ ������� � �������� �������� rx*ry*rz
	m[0][0] = cosAy*cosAz;
	m[0][1] = cosAy*sinAz;
	m[0][2] = -sinAy;
	m[0][3] = 0.0f;
	m[1][0] = sinAx*sinAy*cosAz-cosAx*sinAz;
	m[1][1] = sinAx*sinAy*sinAz+cosAx*cosAz;
	m[1][2] = sinAx*cosAy;
	m[1][3] = 0.0f;
	m[2][0] = cosAx*sinAy*cosAz+sinAx*sinAz;
	m[2][1] = cosAx*sinAy*sinAz-sinAx*cosAz;
	m[2][2] = cosAx*cosAy;
	m[2][3] = 0.0f;
	m[3][0] = x;
	m[3][1] = y;
	m[3][2] = z;
	m[3][3] = 1.0f;
	return *this;
}

//��������� ������� �������� ������ X
mathinline Matrix & Matrix::BuildRotateX(float ang)
{
	SetIdentity();
	m[1][1] = cosf(ang);
	m[1][2] = sinf(ang);
	m[2][1] = -sinf(ang);	
	m[2][2] = cosf(ang);
	return *this;
}

//��������� ������� �������� ������ Y
mathinline Matrix & Matrix::BuildRotateY(float ang)
{
	SetIdentity();
	m[0][0] = cosf(ang);	
	m[0][2] = -sinf(ang);
	m[2][0] = sinf(ang);
	m[2][2] = cosf(ang);
	return *this;
}

//��������� ������� �������� ������ Z
mathinline Matrix & Matrix::BuildRotateZ(float ang)
{
	SetIdentity();
	m[0][0] = cosf(ang);	
	m[0][1] = sinf(ang);
	m[1][0] = -sinf(ang);
	m[1][1] = cosf(ang);
	return *this;
}

//��������� ������� �������
mathinline Matrix & Matrix::BuildPosition(float x, float y, float z)
{
	SetIdentity();
	m[3][0] = x;
	m[3][1] = y;
	m[3][2] = z;
	return *this;
}

//��������� ������� �������
mathinline Matrix & Matrix::BuildPosition(const Vector & pos)
{
	BuildPosition(pos.x, pos.y, pos.z);
	return *this;
}

//��������� ������� ���������������
mathinline Matrix & Matrix::BuildScale(float scale)
{
	SetIdentity();
	m[0][0] = scale;
	m[1][1] = scale;
	m[2][2] = scale;
	return *this;
}

//��������� ������� ���������������
mathinline Matrix & Matrix::BuildScale(float scaleX, float scaleY, float scaleZ)
{
	SetIdentity();
	m[0][0] = scaleX;
	m[1][1] = scaleY;
	m[2][2] = scaleZ;
	return *this;
}

//��������� ������� ���������������
mathinline Matrix & Matrix::BuildScale(const Vector & scale)
{
	BuildScale(scale.x, scale.y, scale.z);
	return *this;
}


//��������� ������� ��������
mathinline Matrix & Matrix::BuildProjection(float viewAngle, float vpWidth, float vpHeight, float zNear, float zFar)
{
	//������� ������
	_asm
	{
		mov		eax, this
		xor		ebx, ebx
		xor		edx, edx
		mov		[eax + 0*4], ebx
		mov		[eax + 1*4], edx
		mov		[eax + 2*4], ebx
		mov		[eax + 3*4], edx
		mov		[eax + 4*4], ebx
		mov		[eax + 5*4], edx
		mov		[eax + 6*4], ebx
		mov		[eax + 7*4], edx
		mov		[eax + 8*4], ebx
		mov		[eax + 9*4], edx
		mov		[eax + 10*4], ebx
		mov		[eax + 11*4], edx
		mov		[eax + 12*4], ebx
		mov		[eax + 13*4], edx
		mov		[eax + 14*4], ebx
		mov		[eax + 15*4], edx
	}
	//��������� �������
	double Q = double(zFar)/double(zFar - zNear);
	m[0][0] = float(1.0/tan(viewAngle*0.5));
	m[1][1] = float(1.0/tan((vpHeight/vpWidth)*viewAngle*0.5));
	m[2][2] = float(Q);
	m[2][3] = 1.0f;
	m[3][2] = float(-Q*zNear);
	return *this;
}

///��������� ������������� ������� ��������
mathinline Matrix & Matrix::BuildOrtoProjection(float vpWidth, float vpHeight, float zNear, float zFar)
{
	//������� ������
	_asm
	{
		mov		eax, this
			xor		ebx, ebx
			xor		edx, edx
			mov		[eax + 0*4], ebx
			mov		[eax + 1*4], edx
			mov		[eax + 2*4], ebx
			mov		[eax + 3*4], edx
			mov		[eax + 4*4], ebx
			mov		[eax + 5*4], edx
			mov		[eax + 6*4], ebx
			mov		[eax + 7*4], edx
			mov		[eax + 8*4], ebx
			mov		[eax + 9*4], edx
			mov		[eax + 10*4], ebx
			mov		[eax + 11*4], edx
			mov		[eax + 12*4], ebx
			mov		[eax + 13*4], edx
			mov		[eax + 14*4], ebx
			mov		[eax + 15*4], edx
	}
	//��������� �������
	double Q = 1.0/double(zFar - zNear);
	m[0][0] = 2.0f/vpWidth;
	m[1][1] = 2.0f/vpHeight;
	m[2][2] = float(Q);
	m[3][2] = float(-Q*zNear);
	m[3][3] = 1.0f;
}

//��������� ������� �������� ��� ����
mathinline Matrix & Matrix::BuildShadowProjection(float viewAngle, float vpWidth, float vpHeight, float zNear, float zFar)
{
	//������� ������
	_asm
	{
		mov		eax, this
			xor		ebx, ebx
			xor		edx, edx
			mov		[eax + 0*4], ebx
			mov		[eax + 1*4], edx
			mov		[eax + 2*4], ebx
			mov		[eax + 3*4], edx
			mov		[eax + 4*4], ebx
			mov		[eax + 5*4], edx
			mov		[eax + 6*4], ebx
			mov		[eax + 7*4], edx
			mov		[eax + 8*4], ebx
			mov		[eax + 9*4], edx
			mov		[eax + 10*4], ebx
			mov		[eax + 11*4], edx
			mov		[eax + 12*4], ebx
			mov		[eax + 13*4], edx
			mov		[eax + 14*4], ebx
			mov		[eax + 15*4], edx
	}
	//��������� �������
	double Q = 1.0/double(zFar - zNear);
	m[0][0] = float(1.0/tan(viewAngle*0.5));
	m[1][1] = float(1.0/tan((vpHeight/vpWidth)*viewAngle*0.5));
	m[2][2] = float(Q);
	m[2][3] = 1.0f;
	m[3][2] = float(-Q*zNear);
	return *this;
}

//��������� ������� ������
mathinline bool Matrix::BuildView(Vector lookFrom, Vector lookTo, Vector upVector)
{
	SetIdentity();
	//����������� ������ ���������
	lookTo -= lookFrom;
	if(lookTo.Normalize() == 0.0f)
	{
		//������ ������� ��� ����������� �������
		pos = -lookFrom;
		return false;
	}
	//���������� ������ ����� � ������ �����������
	upVector -= lookTo*(lookTo | upVector);
	//����������� ������ ������������ �����
	if(upVector.Normalize() == 0.0f) upVector.y = 1.0f;
	//���� ������ ������ ������
	Vector v = upVector ^ lookTo;
	if(v.Normalize() != 0.0f)
	{
		//������ ������� ��������
		m[0][0] = v.x;
		m[1][0] = v.y;
		m[2][0] = v.z;
		m[0][1] = upVector.x;
		m[1][1] = upVector.y;
		m[2][1] = upVector.z;
		m[0][2] = lookTo.x;
		m[1][2] = lookTo.y;
		m[2][2] = lookTo.z;	
	}else{
		//������ ������� ��� ����������� �������
		pos = -lookFrom;
		return false;
	}
	//������ �������
	//pos = -MulNormalByInverse(lookFrom);
	pos = -MulNormal(lookFrom);
	return true;
}

//��������� ������� ���������� ������� ���� ����������� �� z � ����������� �����
mathinline bool Matrix::BuildOrient(Vector zAxisDirection, Vector upVector)
{
	//����������� ������ ����������� z
	if(zAxisDirection.Normalize() < 1e-37f || upVector.Normalize() < 1e-37f)
	{
		vx = Vector(1.0f, 0.0f , 0.0f);
		vy = Vector(0.0f, 1.0f , 0.0f);
		vz = Vector(0.0f, 0.0f , 1.0f);
		return false;
	}
	//�������
	vx = zAxisDirection ^ upVector;
	if(vx.Normalize() == 0.0f)
	{
		vx = Vector(1.0f, 0.0f , 0.0f);
		vy = Vector(0.0f, 1.0f , 0.0f);
		vz = Vector(0.0f, 0.0f , 1.0f);
		return false;
	}
	vy = zAxisDirection ^ vx;
	vz = zAxisDirection;
	return true;
}

//��������� ������� ������� ���� ������� ����� ���� ��������� ������ � ����������� �����
mathinline bool Matrix::BuildOriented(Vector position, Vector lookTo, Vector upVector)
{
	//�����������
	lookTo -= position;
	//����������� ������ ����������� z
	if(lookTo.Normalize() == 0.0f || upVector.Normalize() == 0.0f)
	{
		vx = Vector(1.0f, 0.0f , 0.0f); wx = 0.0f;
		vy = Vector(0.0f, 1.0f , 0.0f); wy = 0.0f;
		vz = Vector(0.0f, 0.0f , 1.0f); wz = 0.0f;
		pos = position; w = 1.0f;
		return false;
	}
	//�������
	vx = lookTo ^ upVector; wx = 0.0f;
	if(vx.Normalize() == 0.0f)
	{
		vx = Vector(1.0f, 0.0f , 0.0f); wx = 0.0f;
		vy = Vector(0.0f, 1.0f , 0.0f); wy = 0.0f;
		vz = Vector(0.0f, 0.0f , 1.0f); wz = 0.0f;
		pos = position; w = 1.0f;
		return false;		
	}
	vy = lookTo ^ vx; wy = 0.0f;
	vz = lookTo; wz = 0.0f;
	pos = position; w = 1.0f;
	return true;
}

//��������� ������� ��� �������������� ���������
mathinline Matrix & Matrix::BuildMirror(float Nx, float Ny, float Nz, float D)
{
	m[0][0] = -Nx*2.0f*Nx + 1.0f;
	m[0][1] = -Ny*2.0f*Nx;
	m[0][2] = -Nz*2.0f*Nx;
	m[0][3] = 0.0f;
	m[1][0] = -Nx*2.0f*Ny;
	m[1][1] = -Ny*2.0f*Ny + 1.0f;
	m[1][2] = -Nz*2.0f*Ny;
	m[1][3] = 0.0f;
	m[2][0] = -Nx*2.0f*Nz;
	m[2][1] = -Ny*2.0f*Nz;
	m[2][2] = -Nz*2.0f*Nz + 1.0f;
	m[2][3] = 0.0f;
	m[3][0] = -Nx*2.0f*-D;
	m[3][1] = -Ny*2.0f*-D;
	m[3][2] = -Nz*2.0f*-D;	
	m[3][3] = 1.0f;
	return *this;
}

//-----------------------------------------------------------
//�������������� �������
//-----------------------------------------------------------

//��������� ������ X
mathinline Matrix & Matrix::RotateX(float ang)
{
	Matrix m;
	m.BuildRotateX(ang);
	EqMultiply(Matrix(*this), m);
	return *this;
}

//��������� ������ Y
mathinline Matrix & Matrix::RotateY(float ang)
{
	Matrix m;
	m.BuildRotateY(ang);
	EqMultiply(Matrix(*this), m);
	return *this;
}

//��������� ������ Z
mathinline Matrix & Matrix::RotateZ(float ang)
{
	Matrix m;
	m.BuildRotateZ(ang);
	EqMultiply(Matrix(*this), m);
	return *this;
}

//��������� ������ ZXY
mathinline Matrix & Matrix::Rotate(float angX, float angY, float angZ)
{
	Matrix m;
	m.Build(angX, angY, angZ);
	EqMultiply(Matrix(*this), m);
	return *this;
}

//��������� ������ ZXY
mathinline Matrix & Matrix::Rotate(const Vector & ang)
{
	Matrix m;
	m.Build(ang.x, ang.x, ang.z);
	EqMultiply(Matrix(*this), m);
	return *this;
}

//�����������
mathinline Matrix & Matrix::Move(float dX, float dY, float dZ)
{
	pos.x += dX;
	pos.y += dY;
	pos.z += dZ;
	return *this;
}

//�����������
mathinline Matrix & Matrix::Move(const Vector & pos)
{
	this->pos.x += pos.x;
	this->pos.y += pos.y;
	this->pos.z += pos.z;
	return *this;
}

//����������������
mathinline Matrix & Matrix::Scale(float scale)
{
	Scale(scale, scale, scale);
	return *this;
}

//���������������� ������� ��������
mathinline Matrix & Matrix::Scale3x3(float scale)
{
	Scale3x3(scale, scale, scale);
	return *this;
}

//����������������
mathinline Matrix & Matrix::Scale(float scaleX, float scaleY, float scaleZ)
{
	m[0][0] *= scaleX;
	m[1][0] *= scaleX;
	m[2][0] *= scaleX;
	m[3][0] *= scaleX;
	m[0][1] *= scaleY;
	m[1][1] *= scaleY;
	m[2][1] *= scaleY;
	m[3][1] *= scaleY;
	m[0][2] *= scaleZ;
	m[1][2] *= scaleZ;
	m[2][2] *= scaleZ;
	m[3][2] *= scaleZ;
	return *this;
}

//���������������� ������� ��������
mathinline Matrix & Matrix::Scale3x3(float scaleX, float scaleY, float scaleZ)
{
	m[0][0] *= scaleX;
	m[1][0] *= scaleX;
	m[2][0] *= scaleX;
	m[0][1] *= scaleY;
	m[1][1] *= scaleY;
	m[2][1] *= scaleY;
	m[0][2] *= scaleZ;
	m[1][2] *= scaleZ;
	m[2][2] *= scaleZ;
	return *this;
}

//����������������
mathinline Matrix & Matrix::Scale(const Vector & scale)
{
	Scale(scale.x, scale.y, scale.z);
	return *this;
}

//���������������� ��������
mathinline Matrix & Matrix::Scale3x3(const Vector & scale)
{
	Scale3x3(scale.x, scale.y, scale.z);
	return *this;
}


//������ �������� �������
mathinline Matrix & Matrix::Inverse()
{
	pos = Vector(-(pos | vx), -(pos | vy), -(pos | vz));
	Transposition3X3();
	return *this;
}

///������ �������� ������� �� ������
mathinline Matrix & Matrix::Inverse(const Matrix & mtx)
{
	pos = Vector(-(mtx.pos | mtx.vx), -(mtx.pos | mtx.vy), -(mtx.pos | mtx.vz));
	_asm
	{
		mov		eax, mtx
		mov		ebx, this
		mov		edx, [eax + 0*4]
		mov		ecx, [eax + 1*4]
		mov		esi, [eax + 2*4]
		mov		[ebx + 0*4], edx
		mov		[ebx + 4*4], ecx
		mov		[ebx + 8*4], esi
		mov		edx, [eax + 4*4]
		mov		ecx, [eax + 5*4]
		mov		esi, [eax + 6*4]
		mov		[ebx + 1*4], edx
		mov		[ebx + 5*4], ecx
		mov		[ebx + 9*4], esi
		mov		edx, [eax + 8*4]
		mov		ecx, [eax + 9*4]
		mov		esi, [eax + 10*4]
		mov		[ebx + 2*4], edx
		mov		[ebx + 6*4], ecx
		mov		[ebx + 10*4], esi
	}
	return *this;
}

///������ ���������������� �������� �������
mathinline Matrix & Matrix::InverseWhithScale()
{
	//������� ��������
	double mtmp[3][3];
	mtmp[0][0] = m[1][1]*m[2][2] - m[1][2]*m[2][1];
	mtmp[0][1] = m[0][2]*m[2][1] - m[0][1]*m[2][2];
	mtmp[0][2] = m[0][1]*m[1][2] - m[0][2]*m[1][1];
	mtmp[1][0] = m[1][2]*m[2][0] - m[1][0]*m[2][2];
	mtmp[1][1] = m[0][0]*m[2][2] - m[0][2]*m[2][0];
	mtmp[1][2] = m[0][2]*m[1][0] - m[0][0]*m[1][2];
	mtmp[2][0] = m[1][0]*m[2][1] - m[1][1]*m[2][0];
	mtmp[2][1] = m[0][1]*m[2][0] - m[0][0]*m[2][1];
	mtmp[2][2] = m[0][0]*m[1][1] - m[0][1]*m[1][0];
	double det = m[0][0]*mtmp[0][0] + m[0][1]*mtmp[1][0] + m[0][2]*mtmp[2][0];
	if(fabs(det) > 1e-200)
	{
		det = 1.0/det;
		m[0][0] = float(det*mtmp[0][0]);
		m[0][1] = float(det*mtmp[0][1]);
		m[0][2] = float(det*mtmp[0][2]);
		m[1][0] = float(det*mtmp[1][0]);
		m[1][1] = float(det*mtmp[1][1]);
		m[1][2] = float(det*mtmp[1][2]);
		m[2][0] = float(det*mtmp[2][0]);
		m[2][1] = float(det*mtmp[2][1]);
		m[2][2] = float(det*mtmp[2][2]);
	}else{
		for(long i = 0; i < 16; i++) matrix[i] = 0.0f;
	}
	//�������
	pos = -(MulNormal(pos));
	return *this;
}

//���������������� �������
mathinline Matrix & Matrix::Transposition()
{
	_asm
	{
		mov		eax, this
		mov		ebx, [eax + 1*4]
		mov		ecx, [eax + 2*4]
		mov		esi, [eax + 4*4]
		mov		edi, [eax + 8*4]
		mov		[eax + 4*4], ebx
		mov		[eax + 8*4], ecx
		mov		[eax + 1*4], esi
		mov		[eax + 2*4], edi
		mov		ebx, [eax + 3*4]
		mov		ecx, [eax + 6*4]
		mov		esi, [eax + 12*4]
		mov		edi, [eax + 9*4]
		mov		[eax + 12*4], ebx
		mov		[eax + 9*4], ecx
		mov		[eax + 3*4], esi
		mov		[eax + 6*4], edi
		mov		ebx, [eax + 7*4]
		mov		ecx, [eax + 11*4]
		mov		esi, [eax + 13*4]
		mov		edi, [eax + 14*4]
		mov		[eax + 13*4], ebx
		mov		[eax + 14*4], ecx
		mov		[eax + 7*4], esi
		mov		[eax + 11*4], edi
	}
	return *this;
}

//���������������� ��������� ��������
mathinline Matrix & Matrix::Transposition3X3()
{
	_asm
	{
		mov		eax, this
		mov		ebx, [eax + 4*4]
		mov		ecx, [eax + 8*4]
		mov		esi, [eax + 1*4]
		mov		edi, [eax + 2*4]
		mov		[eax + 1*4], ebx
		mov		[eax + 2*4], ecx
		mov		[eax + 4*4], esi
		mov		[eax + 8*4], edi
		mov		ebx, [eax + 6*4]
		mov		esi, [eax + 9*4]				
		mov		[eax + 9*4], ebx
		mov		[eax + 6*4], esi
	}
	return *this;
}




//-----------------------------------------------------------
//���������
//-----------------------------------------------------------

//������� ������ ��������
mathinline Matrix & Matrix::SetRotate(const Matrix & mtx)
{
	_asm
	{
		mov		esi, mtx
		mov		edi, this
		mov		ebx, [esi + 0*4]
		mov		ecx, [esi + 1*4]
		mov		edx, [esi + 2*4]
		mov		[edi + 0*4], ebx
		mov		[edi + 1*4], ecx
		mov		[edi + 2*4], edx
		mov		ebx, [esi + 4*4]
		mov		ecx, [esi + 5*4]
		mov		edx, [esi + 6*4]
		mov		[edi + 4*4], ebx
		mov		[edi + 5*4], ecx
		mov		[edi + 6*4], edx
		mov		ebx, [esi + 8*4]
		mov		ecx, [esi + 9*4]
		mov		edx, [esi + 10*4]
		mov		[edi + 8*4], ebx
		mov		[edi + 9*4], ecx
		mov		[edi + 10*4], edx
	}
	return *this;
}


//����������� ������� � ��������� ��������� � �������
mathinline Matrix & Matrix::EqMultiply(const Matrix & m1, const Matrix & m2)
{
	Matrix m;
	m.EqMultiplyFast(m1, m2);
	Set(m);
	return *this;
}
	
//����������� ������� � ��������� ��������� � ������� m1 != this && m2 != this
mathinline Matrix & Matrix::EqMultiplyFast(const Matrix & m1, const Matrix & m2)
{
	m[0][0] = m2.m[0][0]*m1.m[0][0] + m2.m[1][0]*m1.m[0][1] + m2.m[2][0]*m1.m[0][2] + m2.m[3][0]*m1.m[0][3];
	m[0][1] = m2.m[0][1]*m1.m[0][0] + m2.m[1][1]*m1.m[0][1] + m2.m[2][1]*m1.m[0][2] + m2.m[3][1]*m1.m[0][3];
	m[0][2] = m2.m[0][2]*m1.m[0][0] + m2.m[1][2]*m1.m[0][1] + m2.m[2][2]*m1.m[0][2] + m2.m[3][2]*m1.m[0][3];
	m[0][3] = m2.m[0][3]*m1.m[0][0] + m2.m[1][3]*m1.m[0][1] + m2.m[2][3]*m1.m[0][2] + m2.m[3][3]*m1.m[0][3];
	m[1][0] = m2.m[0][0]*m1.m[1][0] + m2.m[1][0]*m1.m[1][1] + m2.m[2][0]*m1.m[1][2] + m2.m[3][0]*m1.m[1][3];
	m[1][1] = m2.m[0][1]*m1.m[1][0] + m2.m[1][1]*m1.m[1][1] + m2.m[2][1]*m1.m[1][2] + m2.m[3][1]*m1.m[1][3];
	m[1][2] = m2.m[0][2]*m1.m[1][0] + m2.m[1][2]*m1.m[1][1] + m2.m[2][2]*m1.m[1][2] + m2.m[3][2]*m1.m[1][3];
	m[1][3] = m2.m[0][3]*m1.m[1][0] + m2.m[1][3]*m1.m[1][1] + m2.m[2][3]*m1.m[1][2] + m2.m[3][3]*m1.m[1][3];
	m[2][0] = m2.m[0][0]*m1.m[2][0] + m2.m[1][0]*m1.m[2][1] + m2.m[2][0]*m1.m[2][2] + m2.m[3][0]*m1.m[2][3];
	m[2][1] = m2.m[0][1]*m1.m[2][0] + m2.m[1][1]*m1.m[2][1] + m2.m[2][1]*m1.m[2][2] + m2.m[3][1]*m1.m[2][3];
	m[2][2] = m2.m[0][2]*m1.m[2][0] + m2.m[1][2]*m1.m[2][1] + m2.m[2][2]*m1.m[2][2] + m2.m[3][2]*m1.m[2][3];
	m[2][3] = m2.m[0][3]*m1.m[2][0] + m2.m[1][3]*m1.m[2][1] + m2.m[2][3]*m1.m[2][2] + m2.m[3][3]*m1.m[2][3];
	m[3][0] = m2.m[0][0]*m1.m[3][0] + m2.m[1][0]*m1.m[3][1] + m2.m[2][0]*m1.m[3][2] + m2.m[3][0]*m1.m[3][3];
	m[3][1] = m2.m[0][1]*m1.m[3][0] + m2.m[1][1]*m1.m[3][1] + m2.m[2][1]*m1.m[3][2] + m2.m[3][1]*m1.m[3][3];
	m[3][2] = m2.m[0][2]*m1.m[3][0] + m2.m[1][2]*m1.m[3][1] + m2.m[2][2]*m1.m[3][2] + m2.m[3][2]*m1.m[3][3];
	m[3][3] = m2.m[0][3]*m1.m[3][0] + m2.m[1][3]*m1.m[3][1] + m2.m[2][3]*m1.m[3][2] + m2.m[3][3]*m1.m[3][3];
	return *this;
}


//�������� ������� �� �������
mathinline Vector Matrix::MulVertex(const Vector & v) const
{
	Vector tv;
	tv.x = m[0][0]*v.x + m[1][0]*v.y + m[2][0]*v.z + m[3][0];
	tv.y = m[0][1]*v.x + m[1][1]*v.y + m[2][1]*v.z + m[3][1];
	tv.z = m[0][2]*v.x + m[1][2]*v.y + m[2][2]*v.z + m[3][2];
	return tv;
}

//�������� ������� �� �������
mathinline Vector Matrix::MulNormal(const Vector & v) const
{
	Vector tv;
	tv.x = m[0][0]*v.x + m[1][0]*v.y + m[2][0]*v.z;
	tv.y = m[0][1]*v.x + m[1][1]*v.y + m[2][1]*v.z;
	tv.z = m[0][2]*v.x + m[1][2]*v.y + m[2][2]*v.z;
	return tv;
}

//�������� ������� �� ��������� �������
mathinline Vector Matrix::MulVertexByInverse(const Vector & v) const
{
	Vector tv;
	tv.x = m[0][0]*(v.x - m[3][0]) + m[0][1]*(v.y - m[3][1]) + m[0][2]*(v.z - m[3][2]);
	tv.y = m[1][0]*(v.x - m[3][0]) + m[1][1]*(v.y - m[3][1]) + m[1][2]*(v.z - m[3][2]);
	tv.z = m[2][0]*(v.x - m[3][0]) + m[2][1]*(v.y - m[3][1]) + m[2][2]*(v.z - m[3][2]);
	return tv;
}

//�������� ������� �� ��������� �������
mathinline Vector Matrix::MulNormalByInverse(const Vector & v) const
{
	Vector tv;
	tv.x = m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z;
	tv.y = m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z;
	tv.z = m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z;
	return tv;
}

///�������� ������� ������ �� ������� ������
mathinline Vector Matrix::GetCamPos() const
{
	return -MulNormalByInverse(pos);
}

//��������� ������� ��� ���
mathinline bool Matrix::IsIdentity() const
{
	const float eps = 1e-4f;
	if(fabs(m[0][0] - 1.0f) > eps) return false;
	if(fabs(m[0][1] - 0.0f) > eps) return false;
	if(fabs(m[0][2] - 0.0f) > eps) return false;
	if(fabs(m[0][3] - 0.0f) > eps) return false;
	if(fabs(m[1][0] - 0.0f) > eps) return false;
	if(fabs(m[1][1] - 1.0f) > eps) return false;
	if(fabs(m[1][2] - 0.0f) > eps) return false;
	if(fabs(m[1][3] - 0.0f) > eps) return false;
	if(fabs(m[2][0] - 0.0f) > eps) return false;
	if(fabs(m[2][1] - 0.0f) > eps) return false;
	if(fabs(m[2][2] - 1.0f) > eps) return false;
	if(fabs(m[2][3] - 0.0f) > eps) return false;
	if(fabs(m[3][0] - 0.0f) > eps) return false;
	if(fabs(m[3][1] - 0.0f) > eps) return false;
	if(fabs(m[3][2] - 0.0f) > eps) return false;
	if(fabs(m[3][3] - 1.0f) > eps) return false;
	return true;
}

//������������ ������� ��� ���
mathinline bool Matrix::IsScale() const
{
	const float eps = 1e-4f;
	if(fabsf(~vx - 1.0f) > eps) return true;
	if(fabsf(~vy - 1.0f) > eps) return true;
	if(fabsf(~vz - 1.0f) > eps) return true;
	if(fabsf(w - 1.0f) > eps) return true;
	return false;
}

//������������ ������� (��� ������� ��������)
mathinline Vector4 Matrix::Projection(Vector vertex, float vphWidth05, float vphHeight05) const
{
	Vector4 res;
	//����������� �������
	res.x = m[0][0]*vertex.x + m[1][0]*vertex.y + m[2][0]*vertex.z + m[3][0];
	res.y = m[0][1]*vertex.x + m[1][1]*vertex.y + m[2][1]*vertex.z + m[3][1];
	res.z = m[0][2]*vertex.x + m[1][2]*vertex.y + m[2][2]*vertex.z + m[3][2];
	res.w = m[0][3]*vertex.x + m[1][3]*vertex.y + m[2][3]*vertex.z + m[3][3];
	//���������� ������������
	float w = 1.0f/res.w;
	//�����������
	res.x = (1.0f + res.x*w)*vphWidth05;
	res.y = (1.0f - res.y*w)*vphHeight05;
	res.z *= w;
	res.w = w;
	return res;
}

//������������ ������ ������ (��� ������� ��������)
mathinline void Matrix::Projection(Vector4 * dstArray, Vector * srcArray, long num, float vphWidth05, float vphHeight05, long srcSize, long dstSize) const
{
	for(; num > 0; num--)
	{
		//����������� �������
		dstArray->x = m[0][0]*srcArray->x + m[1][0]*srcArray->y + m[2][0]*srcArray->z + m[3][0];
		dstArray->y = m[0][1]*srcArray->x + m[1][1]*srcArray->y + m[2][1]*srcArray->z + m[3][1];
		dstArray->z = m[0][2]*srcArray->x + m[1][2]*srcArray->y + m[2][2]*srcArray->z + m[3][2];
		dstArray->w = m[0][3]*srcArray->x + m[1][3]*srcArray->y + m[2][3]*srcArray->z + m[3][3];
		//���������� ������������
		float w = 1.0f/dstArray->w;
		//�����������
		dstArray->x = (1.0f + dstArray->x*w)*vphWidth05;
		dstArray->y = (1.0f - dstArray->y*w)*vphHeight05;
		dstArray->z *= w;
		dstArray->w = w;
		//��������� �� ��������� �������
		srcArray = (Vector *)((char *)srcArray + srcSize);
		dstArray = (Vector4 *)((char *)dstArray + dstSize);
	}
}

//�������� ���� �� ��������������� ������� ��������
mathinline void Matrix::GetAngles(float & ax, float & ay, float & az)
{	
	if(vz.y < 1.0f)
	{
		if(vz.y > -1.0f)
		{
			ax = (float)asin(-vz.y);
			ay = (float)atan2(vz.x, vz.z);
			az = (float)atan2(vx.y, vy.y);
			return;
		}else{
			ax = 3.141592654f*0.5f;
			ay = 0.0f;
			az = (float)atan2(vx.z, vx.x);
		}
	}else{
		ax = -3.141592654f*0.5f;
		ay = 0.0f;
		az = (float)-atan2(vx.z, vx.x);
	}
}

//�������� ���� �� ��������������� ������� ��������
mathinline void Matrix::GetAngles(Vector & ang)
{
	GetAngles(ang.x, ang.y, ang.z);
}

//����������� �� ��������� ������� ����� ������
mathinline float & Matrix::operator () (long i, long j)
{
	return m[i][j];
}

//�������� ��������� �� ������� D3D
mathinline Matrix::operator D3DXMATRIX * () const
{
	return ((D3DXMATRIX*)matrix);
}

//�������� ������ ��� ������� X ����������
mathinline Vector4 Matrix::GetVectorX() const
{
	return Vector4(m[0][0], m[1][0], m[2][0], m[3][0]);
}

//�������� ������ ��� ������� Y ����������
mathinline Vector4 Matrix::GetVectorY() const
{
	return Vector4(m[0][1], m[1][1], m[2][1], m[3][1]);
}

//�������� ������ ��� ������� Z ����������
mathinline Vector4 Matrix::GetVectorZ() const
{
	return Vector4(m[0][2], m[1][2], m[2][2], m[3][2]);
}

//�������� ������ ��� ������� W ����������
mathinline Vector4 Matrix::GetVectorW() const
{
	return Vector4(m[0][3], m[1][3], m[2][3], m[3][3]);
}

#pragma pack(pop)

#endif
