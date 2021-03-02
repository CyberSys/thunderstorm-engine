//============================================================================================
// Spirenkov Maxim, 2003
//============================================================================================
// Sphere
//============================================================================================

#ifndef _Sphere_h_
#define _Sphere_h_

#pragma pack(push, 1)


#include "Vector.h"
#include "Vector4.h"


///����� ������������� ���� � 3D ������������
class Sphere {
public:
  union {
    struct {
      union {
        struct {
          ///������� �� X
          float x;
          ///������� �� Y
          float y;
          ///������� �� Z
          float z;
        };

        union {
          struct {
            ///�������
            Vector p;
          };

          struct {
            ///�������
            Vector pos;
          };
        };
      };

      union {
        ///������
        float r;
        ///������
        float radius;
      };
    };

    struct {
      ///������������� � ���� Vector4
      Vector4 v4;
    };
  };


  //-----------------------------------------------------------
  //������������
  //-----------------------------------------------------------
public:
  //������ �����������
  Sphere();
  //����������� �����������
  Sphere(const Sphere& s);
  //-----------------------------------------------------------
  //���������
  //-----------------------------------------------------------
public:
  //����� � �����
  bool Intersection(const Vector& p);
  //��������� ����������� ������� � �����
  bool Intersection(const Vector& src, const Vector& dst);
  //��������� ����������� ���� � �����
  bool Intersection(const Vector& orig, const Vector& normdir, float* res);
  //��������� ����������� ����� � �����
  bool Intersection(const Sphere& sph);

  //���������� ����� � ����� � 0 ��������
  void Reset(const Vector& p);
  //�������� � ����������� ����� �����
  void AddPoint(const Vector& p);


  //��������� ����������� ���� � �����
  static bool Intersection(const Vector& orig, const Vector& normdir, const Vector& pos, float r, float* res);
};

//-----------------------------------------------------------
//������������
//-----------------------------------------------------------

//������ �����������
inline Sphere::Sphere() {
  v4 = Vector4();
};

//����������� �����������
inline Sphere::Sphere(const Sphere& s) {
  v4 = s.v4;
};

//===========================================================
//���������
//===========================================================

//����� � �����
inline bool Sphere::Intersection(const Vector& p) {
  return ~(pos - p) <= radius * radius;
}

//��������� ����������� ������� � �����
inline bool Sphere::Intersection(const Vector& src, const Vector& dst) {
  Vector dir = dst - src;
  const float len = dir.Normalize();
  if (len > 1e-10f) {
    float dist;
    if (!Intersection(src, dir, pos, r, &dist)) return false;
    if (dist >= 0.0f) {
      return (dist <= len);
    }
    dir = -dir;
    if (!Intersection(dst, dir, pos, r, &dist)) return false;
    if (dist >= 0.0f) {
      return (dist <= len);
    }
    return false;
  }
  return ~(pos - src) <= radius * radius;
}

//��������� ����������� ���� � �����
inline bool Sphere::Intersection(const Vector& orig, const Vector& normdir, float* res) {
  return Intersection(orig, normdir, pos, r, res);
}

//��������� ����������� ����� � �����
inline bool Sphere::Intersection(const Sphere& sph) {
  return (~(p - sph.p) <= (r + sph.r) * (r + sph.r));
}

//���������� ����� � ����� � 0 ��������
inline void Sphere::Reset(const Vector& p) {
  pos = p;
  r = 0.0f;
}

//�������� � ����������� ����� �����
inline void Sphere::AddPoint(const Vector& p) {
  //������ �� ����� � ������
  const float dx = pos.x - p.x;
  const float dy = pos.y - p.y;
  const float dz = pos.z - p.z;
  float len = dx * dx + dy * dy + dz * dz;
  if (len <= r * r) return;
  len = sqrtf(len);
  //����� ������
  r = (len + r) * 0.5f;
  //����� �������
  len = r / len;
  pos.x = p.x + dx * len;
  pos.y = p.y + dy * len;
  pos.z = p.z + dz * len;
}

//��������� ����������� ���� � �����
inline bool Sphere::Intersection(const Vector& orig, const Vector& normdir, const Vector& pos, float r, float* res) {
  const Vector toCenter = pos - orig;
  const float distToOrtoPlane = normdir | toCenter;
  const float distFromOrtoPlaneToSphere2 = r * r - (~toCenter - distToOrtoPlane * distToOrtoPlane);
  if (distFromOrtoPlaneToSphere2 < 0.0f) return false;
  if (res) {
    *res = distToOrtoPlane - sqrtf(distFromOrtoPlaneToSphere2);
  }
  return true;
}

#pragma pack(pop)

#endif
