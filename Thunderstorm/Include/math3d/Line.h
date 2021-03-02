//============================================================================================
// Spirenkov Maxim, 2003
//============================================================================================
// Line
//============================================================================================

#ifndef _Line_h_
#define _Line_h_

#pragma pack(push, 1)

///����� ������������� ����� � 3D ������������
class Line
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
					///��������� ������� �����
					Vector p1;
				};

				struct
				{
					///��������� ������� �����
					Vector src;
				};

				struct
				{
					///��������� ������� �����
					Vector start;
				};

				struct
				{
					///��������� ������� �����
					Vector s;
				};
			};

			union
			{
				struct
				{
					///�������� ������� �����
					Vector p2;
				};

				struct
				{
					///�������� ������� �����
					Vector dst;
				};

				struct
				{
					///�������� ������� �����
					Vector end;
				};

				struct
				{
					///�������� ������� �����
					Vector e;
				};
			};
		};

		struct
		{
			///������������� � ���� ������� �� 0->1
			Vector p[2];
		};
	};

	//-----------------------------------------------------------
	//������������
	//-----------------------------------------------------------
public:
	///����������� �����������
	Line(const Line& l);
	///��������� ������
	Line(float f);
	///��������� ���������
	Line(const Vector& start, const Vector& end);


	//-----------------------------------------------------------
	//���������
	//-----------------------------------------------------------
public:
	//����� ����� ����������� � ����������
	bool Intersection(Plane& plane) const;
	//����� ����� ����������� � ����������
	bool Intersection(Plane& plane, Vector& point) const;
	//����� ����� ����������� � ������, ���������� ����� �����
	bool IntersectionLines(const Line& line, Vector& point, float eps = 0.001f) const;
	//����� ����� ����������� � ������, ���������� ����� �����
	bool IntersectionLinesXZ(const Line& line, Vector& point) const;
	//����� ���������� �� ����� �� ������, ���������� ����� �����
	float DistanceToLine(const Vector& point) const;
	//����� ���������� �� ����� �� ������, ���������� ����� �����
	float DistanceToLineXZ(const Vector& point) const;

	//�������� ��������������� ����������� ������
	Vector Direction() const;
};

//===========================================================
//������������
//===========================================================

//����������� �����������
inline Line::Line(const Line& l)
{
	p1 = l.p1;
	p2 = l.p2;
}

//��������� ������
inline Line::Line(float f)
{
	p1 = 0.0f;
	p2 = 0.0f;
}

//��������� ���������
inline Line::Line(const Vector& start, const Vector& end)
{
	p1 = start;
	p2 = end;
}

//===========================================================
//���������
//===========================================================

//����� ����� ����������� � ����������
inline bool Line::Intersection(Plane& plane) const
{
	return plane.Intersection(p1, p2);
}

//����� ����� ����������� � ����������
inline bool Line::Intersection(Plane& plane, Vector& point) const
{
	return plane.Intersection(p1, p2, point);
}

//����� ����� ����������� ������, ���������� ����� �����
inline bool Line::IntersectionLines(const Line& line, Vector& point, float eps) const
{
	//������������ ������
	Vector dir1 = dst - src;
	Vector dir2 = line.dst - line.src;
	//��������� ����������� line � ���������������� �������� ����� �������
	Plane plane((dir1 ^ dir2) ^ dir2);
	if (plane.normal.Normalize() <= 1e-30f) return false;
	plane.Move(line.src);
	//����� ����������� �������
	float k;
	if (!plane.IntersectionLine(src, dst, k)) return false;
	Vector p = src + (dst - src) * k;
	k = line.DistanceToLine(p);
	if (k < 0.0f || k > eps) return false;
	point = p;
	return true;
}

//����� ����� ����������� � ������, ���������� ����� �����
inline bool Line::IntersectionLinesXZ(const Line& line, Vector& point) const
{
	Line l1 = *this;
	l1.src.y = 0.0f;
	l1.dst.y = 0.0f;
	Line l2 = line;
	l2.src.y = 0.0f;
	l2.dst.y = 0.0f;
	return l1.IntersectionLines(l2, point);
}

//����� ���������� �� ����� �� ������, ���������� ����� �����
inline float Line::DistanceToLine(const Vector& point) const
{
	Plane plane(dst - src);
	if (plane.n.Normalize() <= 1e-30f) return -1.0f;
	plane.Move(point);
	float k;
	if (!plane.IntersectionLine(src, dst, k)) return -1.0f;
	return (src + (dst - src) * k - point).GetLength();
}

//����� ���������� �� ����� �� ������, ���������� ����� �����
inline float Line::DistanceToLineXZ(const Vector& point) const
{
	Vector p = point;
	p.y = 0.0f;
	Line l1 = *this;
	l1.src.y = 0.0f;
	l1.dst.y = 0.0f;
	return l1.DistanceToLine(p);
}

//�������� ��������������� ����������� ������
inline Vector Line::Direction() const
{
	return !(dst - src);
}

#pragma pack(pop)

#endif
