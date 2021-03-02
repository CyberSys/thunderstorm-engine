//============================================================================================
// Spirenkov Maxim, 2004
//============================================================================================
// Capsule
//============================================================================================

#ifndef _Capsule_h_
#define _Capsule_h_

#pragma pack(push, 1)

//����������� ������������� �������, ����������� � 0
class Capsule
{
public:
	Capsule();
	Capsule(float s, float r);

	//���������� ������ ���� � ��������
	void SetRadius(float r);
	//���������� ��������� ����� �������
	void SetSize(float s);
	//�������� ��������� ������������ �� ������������
	bool Extraction(const Triangle & t, Vector & extPoint, Vector & extVector);

protected:
	float radius;
	float size05;
	Vector up, down;
};

mathinline Capsule::Capsule(){};

mathinline Capsule::Capsule(float s, float r)
{
	SetSize(s);
	SetRadius(r);
};

//���������� ������ ���� � ��������
mathinline void Capsule::SetRadius(float r)
{
	radius = r;
}

//���������� ��������� ����� �������
mathinline void Capsule::SetSize(float s)
{
	size05 = s*0.5f;
	up = Vector(0.0f, size05, 0.0f);
	down = Vector(0.0f, -size05, 0.0f);
}

//�������� ��������� ������������ �� ������������
mathinline bool Capsule::Extraction(const Triangle & t, Vector & extPoint, Vector & extVector)
{
	//��������� ����������� � ����������
	Plane plane = t.GetPlane();
	if(plane.d > 0.0f)
	{
		//����� ������� ������ ���������
		return false;
	}
	float dUp = plane.n.y*size05 - plane.d;
	float dDown = plane.n.y*-size05 - plane.d;
	if(dUp >= radius && dDown >= radius)
	{
		//������� �� ������������ � ����������
		return false;
	}
	//��������� ����� ������ ����� �����������
	Vector pointOnPlane;
	const Vector * nearest;
	float k;
	if(dUp < dDown)
	{
		pointOnPlane = plane.n*-dUp;
		pointOnPlane.y += size05;
		nearest = &up;
		k = 1.0f - dUp;
	}else{
		pointOnPlane = plane.n*-dDown;
		pointOnPlane.y -= size05;
		nearest = &down;
		k = 1.0f - dDown;
	}
	//���� ��������� ����� � ������������
	if(t.FindClosestPoint(plane.n, pointOnPlane))
	{
		//����� ����������� ����� ������ ������������
		extVector = plane.n*k;
		extPoint = pointOnPlane - extVector;
		return true;
	}
	//������� ����� �� ������������ ������� + 1
	static const float safeDist = 1.0f;
	Vector RayOrigin = pointOnPlane - plane.n*(k + safeDist);
	//������� ��� �� �������� ����� � ����������� ������� � ���� �����������
	//��� ������ ������ ������ ��� ������������ ������������� �������� � 0
	//����������� �� �������
	Vector dir2D = plane.n.Get2D();
	double in2D = ~dir2D;
	if(in2D > 1e-20f)
	{
		//�����������;
		dir2D *= (in2D = 1.0f/sqrt(in2D));
		//��� ���������
		float distToOrtoPlane = -(RayOrigin | dir2D);
		float distToEdge2 = radius*radius - (RayOrigin.GetLength2D2() - distToOrtoPlane*distToOrtoPlane);
		if(distToEdge2 <= 0.0f)
		{
			//������� �� ������������ � �������������
			return false;
		}
		float distToCylinder = distToOrtoPlane - sqrtf(distToEdge2);
		if(distToCylinder <= 0.0f)
		{
			//������� �� ������������ � �������������
			return false;
		}
		//���� ���������� �� �������� ����� ������ �����������
		float y = float(plane.n.y*in2D*distToCylinder);
		//����������� � ����������
		if(RayOrigin.y + y > size05)
		{
			//������ � ������� ������
			float dist;
			if(Sphere::Intersection(RayOrigin, plane.n, up, radius, &dist))
			{
				k = k + safeDist - dist;
			}else{
				return false;
			}
		}else
		if(RayOrigin.y + y < -size05)
		{
			//������ � ������ ������
			float dist;
			if(Sphere::Intersection(RayOrigin, plane.n, down, radius, &dist))
			{
				k = k + safeDist - dist;
			}else{
				return false;
			}
		}else{		
			//����������� � ���������
			k = k + safeDist - sqrtf(distToCylinder*distToCylinder + y*y);
			if(k < 0.0f)
			{
				//������� �� ������������ � �������������
				return false;
			}
		}
	}else{
		//��� ������������
		float dist;
		if(Sphere::Intersection(RayOrigin, plane.n, *nearest, radius, &dist))
		{
			k = k + safeDist - dist;
		}else{
			return false;
		}
	}
	extVector = plane.n*k;
	extPoint = pointOnPlane - extVector;
	return true;
}

#pragma pack(pop)

#endif

