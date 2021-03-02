//============================================================================================
// Spirenkov Maxim, 2003
//============================================================================================
// Color
//============================================================================================

#ifndef _Color_h_
#define _Color_h_

#pragma pack(push, 1)


#include "Vector.h"
#include "Vector4.h"


///����� ������������� ����� (float)
class Color
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
					///�������
					float r;
					///������
					float g;
					///�����
					float b;
				};
				union
				{
					struct
					{
						///rgb � �������
						Vector c;
					};
					struct
					{
						///rgb � �������
						Vector color;
					};
				};
			};
			union
			{
				///������������
				float a;
				///������������
				float alpha;
			};
		};
		struct
		{
			///������������� � ���� Vector4
			Vector4 v4;
		};
	};


//-----------------------------------------------------------
//������������
//-----------------------------------------------------------
public:
	///������ �����������
	Color();
	///��������� ������ rgb
	Color(float rgb, float a = 1.0f);
	///��������� ��� ����������
	Color(float r, float g, float b, float a = 1.0f);
	///��������� ��� ����������
	Color(const float f[3], float a = 1.0f);
	///��������� ��� ����������
	Color(const Vector & v, float a = 1.0f);
	///��������� ��� ����������
	Color(const Vector4 & v);
	///�����������
	Color(unsigned long c);
	///�����������
	Color(long c);
	///����������� �����������
	Color(const Color & c);

//-----------------------------------------------------------
//���������
//-----------------------------------------------------------
public:
	///�������� ������������� rgb
	float operator ~ () const;
	///������� ���� � ������������� ������������ 0..1
	Color operator ! () const;

	///�������� ������������� ����
	Color operator - () const;

	///��������� rgb
	Color & operator = (float f);
	///����������� � ���������
	Color & operator = (unsigned long c);
	///��������� rgb
	Color & operator = (const Vector & v);
	///���������
	Color & operator = (const Vector4 & v);
	///���������
	Color & operator = (const Color & c);
	///�������������� �������� � ������������� rgb
	Color & operator += (float f);
	///����������� � �������
	Color & operator += (unsigned long c);
	///�������������� �������� � ������������� rgb
	Color & operator += (const Vector & v);
	///�������������� �������� � �������������
	Color & operator += (const Vector4 & v);
	///�������������� �������� � �������������
	Color & operator += (const Color & c);
	///�������������� ��������� � ������������� rgb
	Color & operator -= (float f);
	///����������� � �������
	Color & operator -= (unsigned long c);
	///�������������� ��������� � ������������� rgb
	Color & operator -= (const Vector & v);
	///�������������� ��������� � �������������
	Color & operator -= (const Vector4 & v);
	///�������������� ��������� � �������������
	Color & operator -= (const Color & c);
	///�������������� ��������� � ������������� rgb
	Color & operator *= (float f);
	///����������� � ��������
	Color & operator *= (unsigned long c);
	///�������������� ��������� � ������������� rgb
	Color & operator *= (const Vector & v);
	///�������������� ��������� � �������������
	Color & operator *= (const Vector4 & v);
	///�������������� ��������� � �������������
	Color & operator *= (const Color & c);
	///�������������� ������� � ������������� rgb
	Color & operator /= (float f);
	///����������� � ���������
	Color & operator /= (unsigned long c);
	///�������������� ������� � ������������� rgb
	Color & operator /= (const Vector & v);
	///�������������� ������� � �������������
	Color & operator /= (const Vector4 & v);
	///�������������� ������� � �������������
	Color & operator /= (const Color & c);
	
	///��������� ������������ rgb, ��������� ���������� �� ��� ����������
	Color & operator |= (const Color & c);

	///�������� ����������� ���� � long
	operator dword () const;

//-----------------------------------------------------------
//��������������
//-----------------------------------------------------------
public:
	///���������� ���������� 0..1
	void Clamp();
	///���������� ����������
	void Clamp(float min, float max);

	///��������� ����������� ����������
	void Min(const Color & c);
	///��������� ������������ ����������
	void Max(const Color & c);

	///������� -1..1
	void Brightness(float br);
	///����� -1..1
	void Gamma(float gm);
	///�������� -1..1
	void Contrast(float cn);
	//�������� ������� �������, ����� � ��������
	void Processed(float br, float gm, float cn);

	///�������� ������������
	//...����� ������������ - ������

//-----------------------------------------------------------
//���������
//-----------------------------------------------------------
public:
	//�������� �������������
	float GetIntensity() const;
	//������������� rgb
	Color & Normalize();

	///�������� �������� ��������� ����� �������
	float GetDistance(const Color & c) const;
	///�������� �������� ��������� ����� ������� � ��������
	float GetDistanceSqr(const Color & c) const;

	///��������� ������� ����������������� ��������
	Color & Lerp(const Color & c1, const Color & c2, float kBlend);
	///��������� ������� ����������������� ��������
	Color & LerpA(const Color & c1, const Color & c2, float kBlend);
	//�������� ���� �� �����
	Color & MulColor(float k);
	//�������� ����� �� �����
	Color & MulAlpha(float k);

	///�������� ������� r,b
	Color & SwapRB();

	//�������� ������������ ���� � dword
	unsigned long GetDword() const;


	//�������������� A8R8G8B8 � R5G6B5
	static unsigned short Make565(unsigned long color);
	//�������������� A8R8G8B8 � X1R5G5B5
	static unsigned short Make555(unsigned long color);
	//�������������� A8R8G8B8 � A1R5G5B5
	static unsigned short Make1555(unsigned long color);
	//�������������� A8R8G8B8 � A1R5G5B5
	static unsigned short Make4444(unsigned long color);


};

///������������� ������������� �����
class DColor
{
public:

	union
	{
		struct
		{
			///�����
			unsigned char b;
			///�������
			unsigned char g;
			///�������
			unsigned char r;
			///������������
			unsigned char a;
		};
		union
		{
			///����������� ����
			unsigned long c;
			///����������� ����
			unsigned long color;
		};
	};

//-----------------------------------------------------------
//���������
//-----------------------------------------------------------
public:
	///���������
	DColor & operator = (unsigned long color);
	///���������
	DColor & operator = (long color);

	//�������� long
	operator dword () const;
};
	

//===========================================================
//������������
//===========================================================

//������ �����������
mathinline Color::Color()
{
	r = 0.0f;
	g = 0.0f;
	b = 0.0f;
	a = 1.0f;
}

//��������� ������ rgb
mathinline Color::Color(float rgb, float a)
{
	r = rgb;
	g = rgb;
	b = rgb;
	this->a = a;
}

//��������� ��� ����������
mathinline Color::Color(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

//��������� ��� ����������
mathinline Color::Color(const float f[3], float a)
{
	r = f[0];
	g = f[1];
	b = f[2];
	this->a = a;
}

//��������� ��� ����������
mathinline Color::Color(const Vector & v, float a)
{
	r = v.x;
	g = v.y;
	b = v.z;
	this->a = a;
}

//��������� ��� ����������
mathinline Color::Color(const Vector4 & v)
{
	r = v.x;
	g = v.y;
	b = v.z;
	a = v.w;
}

//�����������
mathinline Color::Color(unsigned long c)
{
	*this = c;
}

///�����������
mathinline Color::Color(long c)
{
	*this = (dword)c;
}

//����������� �����������
mathinline Color::Color(const Color & c)
{
	r = c.r;
	g = c.g;
	b = c.b;
	a = c.a;
}


//===========================================================
//���������
//===========================================================

//�������� ������������� rgb
mathinline float Color::operator ~ () const
{
	return GetIntensity();
}

//������� ���� � ������������� ������������ 0..1
mathinline Color Color::operator ! () const
{
	Color c(*this);
	c.Clamp();
	return c;
}

//�������� ������������� ����
mathinline Color Color::operator - () const
{
	Color c(*this);
	c.r = -c.r;
	c.g = -c.g;
	c.b = -c.b;
	c.a = -c.a;
	return c;
}

//��������� rgb
mathinline Color & Color::operator = (float f)
{
	r = f;
	g = f;
	b = f;
	a = 1.0f;
	return *this;
}

//��������� rgb
mathinline Color & Color::operator = (const Vector & v)
{
	r = v.x;
	g = v.y;
	b = v.z;
	a = 1.0f;
	return *this;
}

//���������
mathinline Color & Color::operator = (const Vector4 & v)
{
	r = v.x;
	g = v.y;
	b = v.z;
	a = v.w;
	return *this;
}

//����������� � ���������
mathinline Color & Color::operator = (unsigned long c)
{
	r = ((unsigned char)(c >> 16))*(1.0f/255.0f);
	g = ((unsigned char)(c >> 8))*(1.0f/255.0f);
	b = ((unsigned char)(c >> 0))*(1.0f/255.0f);
	a = ((unsigned char)(c >> 24))*(1.0f/255.0f);
	return *this;
}

//���������
mathinline Color & Color::operator = (const Color & c)
{
	r = c.r;
	g = c.g;
	b = c.b;
	a = c.a;
	return *this;
}

//�������������� �������� � ������������� rgb
mathinline Color & Color::operator += (float f)
{
	r += f;
	g += f;
	b += f;
	return *this;
}

//����������� � �������
mathinline Color & Color::operator += (unsigned long c)
{
	Color clr(c);
	*this += clr;
	return *this;
}

//�������������� �������� � ������������� rgb
mathinline Color & Color::operator += (const Vector & v)
{
	r += v.x;
	g += v.y;
	b += v.z;
	return *this;
}

//�������������� �������� � �������������
mathinline Color & Color::operator += (const Vector4 & v)
{
	r += v.x;
	g += v.y;
	b += v.z;
	a += v.w;
	return *this;
}

//�������������� �������� � �������������
mathinline Color & Color::operator += (const Color & c)
{
	r += c.r;
	g += c.g;
	b += c.b;
	a += c.a;
	return *this;
}

//�������������� ��������� � ������������� rgb
mathinline Color & Color::operator -= (float f)
{
	r -= f;
	g -= f;
	b -= f;
	return *this;
}

///����������� � �������
mathinline Color & Color::operator -= (unsigned long c)
{
	Color clr(c);
	*this = clr;
	return *this;
}

//�������������� ��������� � ������������� rgb
mathinline Color & Color::operator -= (const Vector & v)
{
	r -= v.x;
	g -= v.y;
	b -= v.z;
	return *this;
}

//�������������� ��������� � �������������
mathinline Color & Color::operator -= (const Vector4 & v)
{
	r -= v.x;
	g -= v.y;
	b -= v.z;
	a -= v.w;
	return *this;
}

//�������������� ��������� � �������������
mathinline Color & Color::operator -= (const Color & c)
{
	r -= c.r;
	g -= c.g;
	b -= c.b;
	a -= c.a;
	return *this;
}

//�������������� ��������� � ������������� rgb
mathinline Color & Color::operator *= (float f)
{
	r *= f;
	g *= f;
	b *= f;
	return *this;
}

///����������� � ��������
mathinline Color & Color::operator *= (unsigned long c)
{
	Color clr(c);
	*this *= clr;
	return *this;
}

//�������������� ��������� � ������������� rgb
mathinline Color & Color::operator *= (const Vector & v)
{
	r *= v.x;
	g *= v.y;
	b *= v.z;
	return *this;
}

//�������������� ��������� � �������������
mathinline Color & Color::operator *= (const Vector4 & v)
{
	r *= v.x;
	g *= v.y;
	b *= v.z;
	a *= v.w;
	return *this;
}

//�������������� ��������� � �������������
mathinline Color & Color::operator *= (const Color & c)
{
	r *= c.r;
	g *= c.g;
	b *= c.b;
	a *= c.a;
	return *this;
}

//�������������� ������� � ������������� rgb
mathinline Color & Color::operator /= (float f)
{
	f = 1.0f/f;
	r *= f;
	g *= f;
	b *= f;
	return *this;
}

//����������� � ���������
mathinline Color & Color::operator /= (unsigned long c)
{
	Color clr(c);
	*this /= clr;
	return *this;
}

//�������������� ������� � ������������� rgb
mathinline Color & Color::operator /= (const Vector & v)
{
	r /= v.x;
	g /= v.y;
	b /= v.z;
	return *this;
}

//�������������� ������� � �������������
mathinline Color & Color::operator /= (const Vector4 & v)
{
	r /= v.x;
	g /= v.y;
	b /= v.z;
	a /= v.w;
	return *this;
}

//�������������� ������� � �������������
mathinline Color & Color::operator /= (const Color & c)
{
	r /= c.r;
	g /= c.g;
	b /= c.b;
	a /= c.a;
	return *this;
}


//��������� ������������ rgb, ��������� ���������� �� ��� ����������
mathinline Color & Color::operator |= (const Color & c)
{
	r = g= b = a = r*c.r + g*c.g + b*c.b;
	return *this;
}

//�������� ����������� ���� � long
mathinline Color::operator dword () const
{
	Color c(*this);
	c.Clamp();
	return long(c.GetDword());
}


/*!\relates Color
�������������� �������� � ������������� rgb
*/
mathinline Color operator + (const Color & c, float f)
{
	Color clr(c);
	clr += f;
	return clr;
}

/*!\relates Color
�������������� �������� � ������������� rgb
*/
mathinline Color operator + (float f, const Color & c)
{
	Color clr(c);
	clr += f;
	return clr;
}

/*!\relates Color
����������� � �������
*/
mathinline Color operator + (const Color & c, unsigned long cl)
{
	Color clr(c);
	clr += cl;
	return clr;
}

/*!\relates Color
����������� � �������
*/
mathinline Color operator + (unsigned long cl, const Color & c)
{
	Color clr(c);
	clr += cl;
	return clr;
}

/*!\relates Color
�������������� �������� � ������������� rgb
*/
mathinline Color operator + (const Color & c, const Vector & v)
{
	Color clr(c);
	clr += v;
	return clr;
}

/*!\relates Color
�������������� �������� � ������������� rgb
*/
mathinline Color operator + (const Vector & v, const Color & c)
{
	Color clr(c);
	clr += v;
	return clr;
}

/*!\relates Color
�������������� �������� � �������������
*/
mathinline Color operator + (const Color & c, const Vector4 & v)
{
	Color clr(c);
	clr += v;
	return clr;
}

/*!\relates Color
�������������� �������� � �������������
*/
mathinline Color operator + (const Vector4 & v, const Color & c)
{
	Color clr(c);
	clr += v;
	return clr;
}

/*!\relates Color
�������������� �������� � �������������
*/
mathinline Color operator + (const Color & c1, const Color & c2)
{
	Color clr(c1);
	clr += c2;
	return clr;
}

/*!\relates Color
�������������� ��������� � ������������� rgb
*/
mathinline Color operator - (const Color & c, float f)
{
	Color clr(c);
	clr -= f;
	return clr;
}

/*!\relates Color
�������������� ��������� � ������������� rgb
*/
mathinline Color operator - (float f, const Color & c)
{
	Color clr(f);
	clr -= f;
	return clr;
}

/*!\relates Color
����������� � �������
*/
mathinline Color operator - (const Color & c, unsigned long cl)
{
	Color clr(c);
	clr -= cl;
	return clr;
}

/*!\relates Color
����������� � �������
*/
mathinline Color operator - (unsigned long cl, const Color & c)
{
	Color clr(cl);
	clr -= c;
	return clr;
}

/*!\relates Color
�������������� ��������� � ������������� rgb
*/
mathinline Color operator - (const Color & c, const Vector & v)
{
	Color clr(c);
	clr -= v;
	return clr;
}

/*!\relates Color
�������������� ��������� � ������������� rgb
*/
mathinline Color operator - (const Vector & v, const Color & c)
{
	Color clr(v);
	clr -= c;
	return clr;
}

/*!\relates Color
�������������� ��������� � �������������
*/
mathinline Color operator - (const Color & c, const Vector4 & v)
{
	Color clr(c);
	clr -= v;
	return clr;
}

/*!\relates Color
�������������� ��������� � �������������
*/
mathinline Color operator - (const Vector4 & v, const Color & c)
{
	Color clr(v);
	clr -= c;
	return clr;
}

/*!\relates Color
�������������� ��������� � �������������
*/
mathinline Color operator - (const Color & c1, const Color & c2)
{
	Color c(c1);
	c -= c2;
	return c;
}

/*!\relates Color
�������������� ��������� � ������������� rgb
*/
mathinline Color operator * (const Color & c, float f)
{
	Color clr(c);
	clr *= f;
	return clr;
}

/*!\relates Color
�������������� ��������� � ������������� rgb
*/
mathinline Color operator * (float f, const Color & c)
{
	Color clr(c);
	clr *= f;
	return clr;
}

/*!\relates Color
����������� � ��������
*/
mathinline Color operator * (const Color & c, unsigned long cl)
{
	Color clr(c);
	clr *= cl;
	return clr;
}

/*!\relates Color
����������� � ��������
*/
mathinline Color operator * (unsigned long cl, const Color & c)
{
	Color clr(c);
	clr *= cl;
	return clr;
}

/*!\relates Color
�������������� ��������� � ������������� rgb
*/
mathinline Color operator * (const Color & c, const Vector & v)
{
	Color clr(c);
	clr *= v;
	return clr;
}

/*!\relates Color
�������������� ��������� � ������������� rgb
*/
mathinline Color operator * (const Vector & v, const Color & c)
{
	Color clr(c);
	clr *= v;
	return clr;
}

/*!\relates Color
�������������� ��������� � �������������
*/
mathinline Color operator * (const Color & c, const Vector4 & v)
{
	Color clr(c);
	clr *= v;
	return clr;
}

/*!\relates Color
�������������� ��������� � �������������
*/
mathinline Color operator * (const Vector4 & v, const Color & c)
{
	Color clr(c);
	clr *= v;
	return clr;
}

/*!\relates Color
�������������� ��������� � �������������
*/
mathinline Color operator * (const Color & c1, const Color & c2)
{
	Color clr(c1);
	clr *= c2;
	return clr;
}

/*!\relates Color
�������������� ������� � ������������� rgb
*/
mathinline Color operator / (const Color & c, float f)
{
	Color clr(c);
	clr /= f;
	return clr;
}

/*!\relates Color
�������������� ������� � ������������� rgb
*/
mathinline Color operator / (float f, const Color & c)
{
	Color clr(f);
	clr /= c;
	return clr;
}


/*!\relates Color
����������� � ���������
*/
mathinline Color operator / (const Color & c, unsigned long cl)
{
	Color clr(c);
	clr /= cl;
	return clr;
}

/*!\relates Color
����������� � ���������
*/
mathinline Color operator / (unsigned long cl, const Color & c)
{
	Color clr(cl);
	clr /= c;
	return clr;
}

/*!\relates Color
�������������� ������� � ������������� rgb
*/
mathinline Color operator / (const Color & c, const Vector & v)
{
	Color clr(c);
	clr /= v;
	return clr;
}

/*!\relates Color
�������������� ������� � ������������� rgb
*/
mathinline Color operator / (const Vector & v, const Color & c)
{
	Color clr(v);
	clr /= c;
	return clr;
}

/*!\relates Color
�������������� ������� � �������������
*/
mathinline Color operator / (const Color & c, const Vector4 & v)
{
	Color clr(c);
	clr /= v;
	return clr;
}

/*!\relates Color
�������������� ������� � �������������
*/
mathinline Color operator / (const Vector4 & v, const Color & c)
{
	Color clr(v);
	clr /= c;
	return clr;
}

/*!\relates Color
�������������� ������� � �������������
*/
mathinline Color operator / (const Color & c1, const Color & c2)
{
	Color clr(c1);
	clr /= c2;
	return clr;
}

/*!\relates Color
��������� ������������ rgb, ��������� ���������� �� ��� ����������
*/
mathinline Color operator | (const Color & c1, const Color & c2)
{
	Color clr(c1);
	clr |= c2;
	return clr;
}


//===========================================================
//��������������
//===========================================================

//���������� ���������� 0..1
mathinline void Color::Clamp()
{
	if(r < 0.0f) r = 0.0f;
	if(r > 1.0f) r = 1.0f;
	if(g < 0.0f) g = 0.0f;
	if(g > 1.0f) g = 1.0f;
	if(b < 0.0f) b = 0.0f;
	if(b > 1.0f) b = 1.0f;
	if(a < 0.0f) a = 0.0f;
	if(a > 1.0f) a = 1.0f;
}

//���������� ����������
mathinline void Color::Clamp(float min, float max)
{
	if(r < min) r = min;
	if(r > max) r = max;
	if(g < min) g = min;
	if(g > max) g = max;
	if(b < min) b = min;
	if(b > max) b = max;
	if(a < min) a = min;
	if(a > max) a = max;
}

//��������� ����������� ����������
mathinline void Color::Min(const Color & c)
{
	if(r > c.r) r = c.r;
	if(g > c.g) g = c.g;
	if(b > c.b) b = c.b;
	if(a > c.a) a = c.a;
}

//��������� ������������ ����������
mathinline void Color::Max(const Color & c)
{
	if(r < c.r) r = c.r;
	if(g < c.g) g = c.g;
	if(b < c.b) b = c.b;
	if(a < c.a) a = c.a;
}

//������� -1..1
mathinline void Color::Brightness(float br)
{
	if(br > 1.0f) br = 1.0f;
	if(br < -1.0f) br = -1.0f;
	*this += br;
	Clamp();
}

//����� -1..1
mathinline void Color::Gamma(float gm)
{
	Clamp();
	if(gm > 0.0f)
	{
		if(gm > 1.0f) gm = 1.0f;
		gm = 1.0f + gm*9.0f;
		r = powf(r, gm);
		g = powf(g, gm);
		b = powf(b, gm);
	}else
	if(gm < 0.0f)
	{
		if(gm < -1.0f) gm = -1.0f;
		gm = 1.0f + gm*0.9f;
		r = powf(r, gm*10.0f);
		g = powf(g, gm*10.0f);
		b = powf(b, gm*10.0f);
	}
}

//�������� -1..1
mathinline void Color::Contrast(float cn)
{
	if(cn > 1.0f) cn = 1.0f;
	if(cn < -1.0f) cn = -1.0f;
	cn *= 5.0f;
	r = (r - 0.5f)*cn + 0.5f;
	g = (g - 0.5f)*cn + 0.5f;
	b = (b - 0.5f)*cn + 0.5f;
	Clamp();

}

//�������� ������� �������, ����� � ��������
mathinline void Color::Processed(float br, float gm, float cn)
{
	Contrast(cn);
	Gamma(gm);
	Brightness(br);
}


//===========================================================
//���������
//===========================================================

//�������� �������������
mathinline float Color::GetIntensity() const
{
	return r*0.299f + g*0.587f + b*0.114f;
}

//������������� rgb
mathinline Color & Color::Normalize()
{
	float min = r;
	float max = r;
	if(min > g) min = g;
	if(max < g) max = g;
	if(min > b) min = b;
	if(max < b) max = b;
	if(min > 0.0f) min = 0.0f;
	float dlt = max - min;
	r -= min;
	g -= min;
	b -= min;
	if(dlt > 1.0f)
	{
		dlt = 1.0f/dlt;
		r *= dlt;
		g *= dlt;
		b *= dlt;
	}
	Clamp();
	return *this;
}

//�������� �������� ��������� ����� �������
mathinline float Color::GetDistance(const Color & c) const
{
	return sqrtf(GetDistanceSqr(c));
}

//�������� �������� ��������� ����� ������� � ��������
mathinline float Color::GetDistanceSqr(const Color & c) const
{
	float dr = (r - c.r)*0.299f;
	float dg = (g - c.g)*0.587f;
	float db = (b - c.b)*0.114f;
	return dr*dr + dg*dg + db*db;
}

//��������� ������� ����������������� ��������
mathinline Color & Color::Lerp(const Color & c1, const Color & c2, float kBlend)
{
	r = c1.r + (c2.r - c1.r)*kBlend;
	g = c1.g + (c2.g - c1.g)*kBlend;
	b = c1.b + (c2.b - c1.b)*kBlend;
	return *this;
}

//��������� ������� ����������������� ��������
mathinline Color & Color::LerpA(const Color & c1, const Color & c2, float kBlend)
{
	r = c1.r + (c2.r - c1.r)*kBlend;
	g = c1.g + (c2.g - c1.g)*kBlend;
	b = c1.b + (c2.b - c1.b)*kBlend;
	a = c1.a + (c2.a - c1.a)*kBlend;
	return *this;
}

//�������� ���� �� �����
mathinline Color & Color::MulColor(float k)
{
	r *= k;
	g *= k;
	b *= k;
	return *this;
}

//�������� ����� �� �����
mathinline Color & Color::MulAlpha(float k)
{
	a *= k;
	return *this;
}

//�������� ������� r,b
mathinline Color & Color::SwapRB()
{
	float t = r;
	r = b;
	b = t;
	return *this;
}

//�������� ������������ ���� � dword
mathinline unsigned long Color::GetDword() const
{
	long l;
	DColor color;
	float k = 255.0f;
	_asm
	{
		mov		eax, this
		fld		[eax]this.r
		fld		k
		fmul
		fistp	l
		mov		ebx, l
		mov		color.r, bl
		fld		[eax]this.g
		fld		k
		fmul
		fistp	l
		mov		ebx, l
		mov		color.g, bl
		fld		[eax]this.b
		fld		k
		fmul
		fistp	l
		mov		ebx, l
		mov		color.b, bl
		fld		[eax]this.a
		fld		k
		fmul
		fistp	l
		mov		ebx, l
		mov		color.a, bl
	};
	return color.c;
}

//�������������� A8R8G8B8 � R5G6B5
mathinline unsigned short Color::Make565(unsigned long color)
{
	//   11111000 11111100 11111000
    //           11111 111111 11111
	unsigned long b = (color >> 3) & 0x1f;
	unsigned long g = (color >> 5) & 0x7e0;
	unsigned long r = (color >> 8) & 0xf800;
	return (unsigned short)(r | g | b);
}

//�������������� A8R8G8B8 � X1R5G5B5
mathinline unsigned short Color::Make555(unsigned long color)
{
	//   11111000 11111000 11111000
	//           011111 11111 11111
	unsigned long b = (color >> 3) & 0x1f;
	unsigned long g = (color >> 6) & 0x3e0;
	unsigned long r = (color >> 9) & 0x7c00;
	return (unsigned short)(r | g | b);
}

//�������������� A8R8G8B8 � A1R5G5B5
mathinline unsigned short Color::Make1555(unsigned long color)
{
	//   11111000 11111000 11111000
	//          1 11111 11111 11111
	unsigned long b = (color >> 3) & 0x1f;
	unsigned long g = (color >> 6) & 0x3e0;
	unsigned long r = (color >> 9) & 0x7c00;
	unsigned long a = (color >> 16) & 0x8000;
	return (unsigned short)(r | g | b | a);
}

//�������������� A8R8G8B8 � A1R5G5B5
mathinline unsigned short Color::Make4444(unsigned long color)
{
	//  11110000 11111000 11111000 11111000
	//                  1111 1111 1111 1111
	unsigned long b = (color >> 4) & 0xf;
	unsigned long g = (color >> 8) & 0xf0;
	unsigned long r = (color >> 12) & 0xf00;
	unsigned long a = (color >> 16) & 0xf000;
	return (unsigned short)(r | g | b | a);
}

//===========================================================
//DColor
//===========================================================

//���������
mathinline DColor & DColor::operator = (unsigned long color)
{
	c = color;
	return *this;
}

//���������
mathinline DColor & DColor::operator = (long color)
{
	c = (unsigned long)color;
	return *this;
}

//�������� long
mathinline DColor::operator dword () const
{
	return long(c);
}



#pragma pack(pop)

#endif
