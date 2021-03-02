//============================================================================================
// Spirenkov Maxim, 2003
//============================================================================================
// Triangle
//============================================================================================

#ifndef _Triangle_h_
#define _Triangle_h_

#pragma pack(push, 1)


#include "Vector.h"
#include "Plane.h"


///����� ������������� ������������ � 3D ������������
class Triangle
{
public:
	union
	{
		struct
		{
			///������ �������
			Vector p1;
			///������ �������
			Vector p2;
			///������ �������
			Vector p3;
		};
		struct
		{
			///������� � ���� �������
			Vector p[3];
		};
	};


//-----------------------------------------------------------
//������������
//-----------------------------------------------------------
public:	
	///������ �����������
	Triangle();
	Triangle(const Vector * v);

//-----------------------------------------------------------
//���������
//-----------------------------------------------------------
public:
	///�������������� ��������� ������ � �������������
	Triangle & operator *= (const Vector & v);

//-----------------------------------------------------------
//���������
//-----------------------------------------------------------
public:	
	//�������� �������
	Vector GetNormal() const;
	//�������� ������� �����
	Vector GetCenter() const;
	//�������� �����������
	Plane GetPlane() const;
	//��������� ���������� ����� ����� (p[start], p[NextIndex(start)])
	Plane OrtoPlane(long start) const;
	//��������� ���������� ����� ����� (p[0], p[1])
	Plane OrtoPlane01() const;
	//��������� ���������� ����� ����� (p[1], p[2])
	Plane OrtoPlane12() const;
	//��������� ���������� ����� ����� (p[2], p[0])
	Plane OrtoPlane20() const;
	//�������� ���������� ������ ������� ������������
	static long PrevIndex(long index);
	//�������� ��������� ������ ������� ������������
	static long NextIndex(long index);

	//����� ������� ����� � ������������ � ������, ������� � ��������� ������������, true - ������ ������������
	bool FindClosestPoint(const Vector & trgNormal, Vector & pointOnPlane) const;


	//������������� ���������� ������
	Triangle & Transform(const Matrix & mtx);


	enum CoIntersectionResult
	{
		cir_none = 0,		//�� ������������
		cir_deg_cur,			//������� ����������� �����������
		cir_deg_t,			//������ ����������� �����������
		cir_coplanar,		//�� ������������� �� ����� � ����� ���������
		cir_intersection,	//����� � ����� ���������, �������������, �� �� ���������
		cir_equal,			//������������ ���������
	};

	//�������� ������������� �� ����������� � ����� ���������
	CoIntersectionResult IsCoplanarIntersection(const Triangle & t, float intsEps = 0.0000001f) const;

private:
	static long z_sysClipTriangleEdgePlane(Plane plane, Vector src[8], Vector dst[8], long count);
};


//===========================================================
//������������
//===========================================================

//������ �����������
mathinline Triangle::Triangle(){}

///��������� ��� ����������
mathinline Triangle::Triangle(const Vector * v)
{
	p[0] = v[0];
	p[1] = v[1];
	p[2] = v[2];
}


//===========================================================
//���������
//===========================================================

//�������������� ��������� ������ � �������������
mathinline Triangle & Triangle::operator *= (const Vector & v)
{
	p1 *= v;
	p2 *= v;
	p3 *= v;
	return *this;
}

/*!\relates Triangle
�������� ����������� �� �������
*/
mathinline Triangle operator * (const Matrix & mtx, const Triangle & t)
{
	Triangle trg;
	trg.p1 = mtx*t.p1;
	trg.p2 = mtx*t.p2;
	trg.p3 = mtx*t.p3;
	return trg;
}

/*!\relates Triangle
�������� ����������� �� �������
*/
mathinline Triangle operator * (const Triangle & t, const Matrix & mtx)
{
	Triangle trg;
	trg.p1 = mtx*t.p1;
	trg.p2 = mtx*t.p2;
	trg.p3 = mtx*t.p3;
	return trg;
}


//===========================================================
//���������
//===========================================================

//�������� �������
mathinline Vector Triangle::GetNormal() const
{
    return !((p1 - p2) ^ (p1 - p3));
}

//�������� ������� �����
mathinline Vector Triangle::GetCenter() const
{
	return (p1 + p2 + p3)*0.33333333333333f;
}

//�������� �����������
mathinline Plane Triangle::GetPlane() const
{
	return Plane(GetNormal(), p1);	
}

//��������� ���������� ����� ����� (p[start % 2], p[(start + 1) % 2])
mathinline Plane Triangle::OrtoPlane(long start) const
{
	Plane plane;
	//�������
	const Vector & ps = p[start % 3];
	const Vector & pe = p[NextIndex(start)];
	//�������
	plane.n = !(((p3 - p1) ^ (p1 - p2)) ^ (ps - pe));
	//���������
	plane.Move(ps);
	return plane;
}

//��������� ���������� ����� ����� (p[0], p[1])
mathinline Plane Triangle::OrtoPlane01() const
{
	return OrtoPlane(0);
}

//��������� ���������� ����� ����� (p[1], p[2])
mathinline Plane Triangle::OrtoPlane12() const
{
	return OrtoPlane(1);
}

//��������� ���������� ����� ����� (p[2], p[0])
mathinline Plane Triangle::OrtoPlane20() const
{
	return OrtoPlane(2);
}


//�������� ���������� ������ ������� ������������
mathinline long Triangle::PrevIndex(long index)
{
	index--;
	if(index < 0) index = 2;
	if(index > 2) index = 2;
	return index;
}

//�������� ��������� ������ ������� ������������
mathinline long Triangle::NextIndex(long index)
{
	index++;
	if(index < 0) index = 0;
	if(index > 2) index = 0;
	return index;
}

//����� ������� ����� � ������������ � ������, ������� � ��������� ������������, true - ������ ������������
mathinline bool Triangle::FindClosestPoint(const Vector & trgNormal, Vector & pointOnPlane) const
{
	const Vector * cPoint = null;
	for(long i = 0; i < 3; i++)
	{
		//�����
		const Vector & ps = p[i];
		const Vector & pe = p[i + 1 < 3 ? i + 1 : 0];
		Vector edge = pe - ps;
		//���� ����������� ����� ������������ ������ �����, �� ������ ������ ���
		float edgeLen = edge.Normalize();
		if(edgeLen < 1e-37f) return false;
		//������������� ���������
		Plane orto(edge ^ trgNormal, ps);
		//���������� ��������� �����
		float distToEdge = orto.Dist(pointOnPlane);
		if(distToEdge > 0.0f)
		{
			//���������� ������ �������� (pointOnPlane - ps) �� �����
			float prjLength = edge | (pointOnPlane - ps);
			if(prjLength < 0.0f)
			{
				cPoint = &ps;
			}else
			if(prjLength > edgeLen)
			{
				cPoint = &pe;
			}else{
				pointOnPlane = ps + (pe - ps)*(prjLength/edgeLen);
				return false;
			}
		}
	}
	if(cPoint)
	{
		pointOnPlane = *cPoint;
		return false;
	}
	return true;
}

//������������� ���������� ������
mathinline Triangle & Triangle::Transform(const Matrix & mtx)
{
	p1 = mtx.MulVertex(p1);
	p2 = mtx.MulVertex(p2);
	p3 = mtx.MulVertex(p3);
	return *this;
}

//�������� ������������� �� ����������� � ����� ���������
mathinline Triangle::CoIntersectionResult Triangle::IsCoplanarIntersection(const Triangle & t, float intsEps) const
{
	//�������� ������� �������������
	if(~(p1 - p2) < intsEps*intsEps || ~(p2 - p3) < intsEps*intsEps || ~(p3 - p1) < intsEps*intsEps) return cir_deg_cur;
	if(~(t.p1 - t.p2) < intsEps*intsEps || ~(t.p2 - t.p3) < intsEps*intsEps || ~(t.p3 - t.p1) < intsEps*intsEps) return cir_deg_t;
	//������� �������
	Vector n = (p1 - p2) ^ (p1 - p3);
	if(n.Normalize() < 0.0000001f) return cir_deg_cur;
	Vector nt = (t.p1 - t.p2) ^ (t.p1 - t.p3);
	if(nt.Normalize() < 0.0000001f) return cir_deg_t;
	//�������� �������������
	float cs = n | nt;
	static const float cosMin = cosf(0.5f*3.141592654f/180.0f);
	if(cs < cosMin) return cir_none;
	//��������� ����������
	float d = n | p1;
	float dt = n | t.p1;
	if(fabs(d - dt) > intsEps) return cir_none;
	//�������� �� ����������
	if(~(p1 - t.p1) + ~(p2 - t.p2) + ~(p3 - t.p3) < intsEps*intsEps) return cir_equal;
	if(~(p2 - t.p1) + ~(p3 - t.p2) + ~(p1 - t.p3) < intsEps*intsEps) return cir_equal;
	if(~(p3 - t.p1) + ~(p1 - t.p2) + ~(p2 - t.p3) < intsEps*intsEps) return cir_equal;
	//�������� �� ����������� �������������
	static Vector poly1[8], poly2[8];
	poly1[0] = t.p1; poly1[1] = t.p2; poly1[2] = t.p3;
	long count = 3;
	count = z_sysClipTriangleEdgePlane(Plane(!(n ^ (p1 - p2)), p1), poly1, poly2, count);
	if(!count) return cir_coplanar;
	count = z_sysClipTriangleEdgePlane(Plane(!(n ^ (p2 - p3)), p2), poly2, poly1, count);
	if(!count) return cir_coplanar;
	count = z_sysClipTriangleEdgePlane(Plane(!(n ^ (p3 - p1)), p3), poly1, poly2, count);
	if(!count) return cir_coplanar;
	for(long s = 0; s < count; s++)
	{
		long e = s + 1 < count ? s + 1 : 0;
		float dist = ~(poly2[e] - poly2[s]);
		if(dist < intsEps*intsEps) return cir_coplanar;
	}
	return cir_intersection;
}

mathinline long Triangle::z_sysClipTriangleEdgePlane(Plane plane, Vector src[8], Vector dst[8], long count)
{
	float ds = plane*src[0], de;
	for(long s = 0, c = 0; s < count; s++, ds = de)
	{
		//���� � �������, ��������� �������
		if(ds <= 0.0f) dst[c++] = src[s];		
		//������ ����������
		long e = s + 1 < count ? s + 1 : 0;
		//��������� �� ���������
		de = plane*src[e];
		//���� � ����� �������, �� ����������
		if(ds*de >= 0.0f) continue;
		//���� �����������
		dst[c++] = src[s] + (src[e] - src[s])*(ds/(ds - de));
	}
	if(c < 3) c = 0;
	return c;
}

#pragma pack(pop)

#endif
