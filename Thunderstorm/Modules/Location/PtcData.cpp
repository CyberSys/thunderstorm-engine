//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------
//	PtcData
//--------------------------------------------------------------------------------------------
//
//============================================================================================

#include "PtcData.h"
#include "dx9render.h"
#include "storm_assert.h"
#include "vfile_service.h"
#include <core.h>

//============================================================================================
//���������������, ������������������
//============================================================================================

PtcData::PtcData() {
  srand(GetTickCount());
  data = nullptr;
  triangle = nullptr;
  numTriangles = 0;
  vertex = nullptr;
  numVerteces = 0;
  normal = nullptr;
  numNormals = 0;
  min = max = 0.0f;
  map = nullptr;
  l = w = 0;
  ls = ws = 0.0f;
  indeces = nullptr;
  numIndeces = 0;
  table = nullptr;
  lineSize = 0;
  ctriangle = nullptr;
  numClTriangles = 0;
  maxClTriangles = 0;
  dbgTriangles = nullptr;
  dbgEdges = nullptr;
  numSteps = 0;
  materials = nullptr;
  middle = 0.0f;
}

PtcData::~PtcData() {
  delete data;
  delete ctriangle;
  delete dbgTriangles;
  delete dbgEdges;
}

bool PtcData::Load(const char* path) {
  Assert(data == nullptr);
  char* buf = nullptr;
  uint32_t size = 0;
  middle = 0.0f;
  //��������� ������
  if (!fio->LoadFile(path, &buf, &size)) {
    core.Trace("Ptc(\"%s\") -> file not found", path);
    return false;
  }
  //��������� ���� �� ������������
  if (!buf || size < sizeof(PtcHeader)) {
    core.Trace("Ptc(\"%s\") -> invalide file size", path);
    delete buf;
    return false;
  }
  auto& hdr = *(PtcHeader*)buf;
  if (hdr.id != PTC_ID) {
    core.Trace("Ptc(\"%s\") -> invalide file ID", path);
    delete buf;
    return false;
  }
  if (hdr.ver != PTC_VERSION && hdr.ver != PTC_PREVERSION1) {
    core.Trace("Ptc(\"%s\") -> invalide file version", path);
    delete buf;
    return false;
  }
  uint32_t tsize = sizeof(PtcHeader);
  tsize += hdr.numTriangles * sizeof(PtcTriangle);
  tsize += hdr.numVerteces * sizeof(PtcVertex);
  tsize += hdr.numNormals * sizeof(PtcNormal);
  tsize += hdr.mapL * hdr.mapW * sizeof(PtcMap);
  tsize += hdr.numIndeces * sizeof(uint16_t);
  tsize += hdr.lineSize * hdr.numTriangles * sizeof(uint8_t);
  if (hdr.ver == PTC_VERSION) tsize += sizeof(PtcMaterials);
  if (tsize != size) {
    core.Trace("Ptc(\"%s\") -> invalide file size", path);
    delete buf;
    return false;
  }
  if (hdr.numTriangles < 1 ||
    hdr.numVerteces < 3 ||
    hdr.numNormals < 1 ||
    hdr.mapL < 1 ||
    hdr.mapW < 1 ||
    hdr.numIndeces < 1 ||
    hdr.lineSize < 1 ||
    hdr.minX >= hdr.maxX ||
    hdr.minY > hdr.maxY ||
    hdr.minZ >= hdr.maxZ) {
    core.Trace("Ptc(\"%s\") -> invalide file header", path);
    delete buf;
    return false;
  }
  //��������� ��������� ������
  data = buf;
  SFLB_PotectionLoad();
  return true;
}

//������� ������
#ifndef _XBOX
void PtcData::SFLB_PotectionLoad()
#else
void PtcData::SFLB_PotectionLoad()
#endif
{
  //������
  auto* const buf = static_cast<char*>(data);
  auto& hdr = *(PtcHeader*)buf;
  //������������
  uint32_t tsize = sizeof(PtcHeader);
  triangle = (PtcTriangle*)(buf + tsize);
  numTriangles = hdr.numTriangles;
  //�������
  tsize += hdr.numTriangles * sizeof(PtcTriangle);
  vertex = (PtcVertex*)(buf + tsize);
  numVerteces = hdr.numVerteces;
  //�������
  tsize += hdr.numVerteces * sizeof(PtcVertex);
  normal = (PtcNormal*)(buf + tsize);
  numNormals = hdr.numNormals;
  //����� ��� ���������
  tsize += hdr.numNormals * sizeof(PtcNormal);
  map = (PtcMap*)(buf + tsize);
  l = hdr.mapL;
  w = hdr.mapW;
  min.x = hdr.minX;
  min.y = hdr.minY;
  min.z = hdr.minZ;
  max.x = hdr.maxX;
  max.y = hdr.maxY;
  max.z = hdr.maxZ;
  ls = (max.z - min.z) / l;
  ws = (max.x - min.x) / w;
  tsize += hdr.mapL * hdr.mapW * sizeof(PtcMap);
  indeces = (uint16_t*)(buf + tsize);
  //������ ��� ���������� �����
  tsize += hdr.numIndeces * sizeof(uint16_t);
  table = (uint8_t*)(buf + tsize);
  lineSize = hdr.lineSize;
  //���������
  if (hdr.ver == PTC_VERSION) materials = (PtcMaterials*)(table + lineSize * numTriangles);
  //���� ������� �����
  middle = 0.0f;
  for (long i = 0; i < numVerteces; i++) {
    middle.x += vertex[i].x;
    middle.y += vertex[i].y;
    middle.z += vertex[i].z;
  }
  if (numVerteces) middle *= 1.0f / numVerteces;
}

//���������� ������� �������
long PtcData::FindNode(const CVECTOR& pos, float& y) {
  //������� �� �����
  const auto mapX = static_cast<long>((pos.x - min.x) / ws);
  const auto mapZ = static_cast<long>((pos.z - min.z) / ls);
  if (mapX < 0 || mapX >= w) return -1;
  if (mapZ < 0 || mapZ >= l) return -1;
  //���������� ������������
  PtcMap& m = map[mapZ * w + mapX];
  long node = -1;
  float h = 0.0f;
  float dist = -1.0f;
  for (long i = 0; i < m.size; i++) {
    PtcTriangle& trg = triangle[indeces[m.start + i]];
    //��������� ��������� � �����������
    long j;
    for (j = 0; j < 3; j++) {
      //������� �����
      CVECTOR& vs = *(CVECTOR*)&vertex[trg.i[j]];
      CVECTOR& ve = *(CVECTOR*)&vertex[trg.i[j == 2 ? 0 : j + 1]];
      //������� �����
      const float nx = -(ve.z - vs.z);
      const float nz = (ve.x - vs.x);
      //���� �� ��������� ������������ ��������
      if (pos.x * nx + pos.z * nz > vs.x * nx + vs.z * nz) break;
    }
    if (j == 3) {
      //�������� ������
      const float d = FindHeight(indeces[m.start + i], pos.x, pos.z);
      if (dist >= 0.0f) {
        if (dist > fabsf(pos.y - d)) {
          dist = fabsf(pos.y - d);
          h = d;
          node = indeces[m.start + i];
        }
      }
      else {
        dist = fabsf(pos.y - d);
        h = d;
        node = indeces[m.start + i];
      }
    }
  }
  y = h;
  return node;
}

//������� pos � to, ���������� ����� ���
long PtcData::Move(long curNode, const CVECTOR& to, CVECTOR& pos, long depth) {
  isSlide = false;
  isBearing = false;
  if (curNode < 0) return -1;
  Assert(curNode < numTriangles);
  //����������� ����������� � 2D
  CVECTOR dir(to.x - pos.x, 0.0f, to.z - pos.z);
  CVECTOR ps = pos;
  long firstNode = curNode;
  //������ ���� � 2D
  float dst = ~dir;
  if (dst == 0.0f || depth > 32) {
    if (to.y != pos.y) {
      float hy = FindHeight(curNode, pos.x, pos.z);
      pos.y = to.y;
      if (pos.y < hy) pos.y = hy;
    }
    return curNode;
  }
  dst = sqrtf(dst);
  dir *= 1.0f / dst;
  //������������ ���������, ���������� ����� ������� �����������
  CVECTOR nd(dir.z, 0.0f, -dir.x);
  float d = (nd | pos);
  //����� ����
  long fromNode = -2; //������ ������
  long loopCounter;
  for (loopCounter = 0; loopCounter < 256; loopCounter++) {
    //��������� ���������� ����� �������� �� ������� ������������
    uint16_t* trg = triangle[curNode].i;
    long j;
    for (j = 0; j < 3; j++) {
      //������� �����
      CVECTOR& vs = *(CVECTOR*)&vertex[trg[j]];
      CVECTOR& ve = *(CVECTOR*)&vertex[trg[j == 2 ? 0 : j + 1]];
      //������� �����
      float nx = -(ve.z - vs.z);
      float nz = (ve.x - vs.x);
      float nl = sqrtf(nx * nx + nz * nz);

      if (!nl) continue;
      nx /= nl;
      nz /= nl;

      //���� �� ��������� ������������ ��������
      if (to.x * nx + to.z * nz > vs.x * nx + vs.z * nz) break;
    }
    if (j == 3) {
      //����� �������������
      pos = to;
      //�������� ������
      float h = FindHeight(curNode, pos.x, pos.z);
      if (pos.y < h) {
        pos.y = h;
        isBearing = true;
      }
      return curNode;
    }
    //����� ����� ����� ������� ��� ������
    short* nb = triangle[curNode].nb;
    long curEdge = -1;
    CVECTOR pnt;
    for (j = 0; j < 3; j++) {
      //��������� ����������� ������ ������
      if (nb[j] == fromNode) continue;
      //������� �����
      CVECTOR& vs = *(CVECTOR*)&vertex[trg[j]];
      CVECTOR& ve = *(CVECTOR*)&vertex[trg[j == 2 ? 0 : j + 1]];
      //��������� �� ������ �����������
      float d1 = (nd | vs) - d;
      float d2 = (nd | ve) - d;
      if (d1 * d2 > 0.0f) continue;
      if (d1 - d2 == 0.0f) continue;
      //���������� �����
      double k = d1 / static_cast<double>(d1 - d2);
      if (k < 0.0f) k = 0.0f;
      if (k > 1.0f) k = 1.0f;
      //����� �����������
      CVECTOR p;
      //k += 0.000000001;
      p.x = static_cast<float>(vs.x + (ve.x - vs.x) * k);
      p.y = static_cast<float>(vs.y + (ve.y - vs.y) * k);
      p.z = static_cast<float>(vs.z + (ve.z - vs.z) * k);
      //�������� ��������� �� ����
      if ((d1 = (dir | (p - ps))) < -0.00001f) continue;
      if ((d2 = (dir | (to - p))) < -0.00001f) continue;
      //����� �����������
      if (nb[j] >= 0) {
        curEdge = j;
        pnt = p;
        break;
      }
      curEdge = j;
      pnt = p;
      if (d1 != 0.0f && d2 != 0.0f) break;
    }
    if (curEdge == -1) {
      isSlide = true;
      return firstNode;

      Assert(false);
      for (j = 0; j < 3; j++) {
        //��������� ����������� ������ ������
        if (nb[j] == fromNode) continue;
        //������� �����
        CVECTOR& vs = *(CVECTOR*)&vertex[trg[j]];
        CVECTOR& ve = *(CVECTOR*)&vertex[trg[j == 2 ? 0 : j + 1]];
        //��������� �� ������ �����������
        float d1 = (nd | vs) - d;
        float d2 = (nd | ve) - d;
        if (d1 * d2 > 0.0f) continue;
        if (d1 - d2 == 0.0f) continue;
        //���������� �����
        double k = d1 / static_cast<double>(d1 - d2);
        if (k < 0.0f) k = 0.0f;
        if (k > 1.0f) k = 1.0f;
        //����� �����������
        CVECTOR p;
        //k += 0.000000001;
        p.x = static_cast<float>(vs.x + (ve.x - vs.x) * k);
        p.y = static_cast<float>(vs.y + (ve.y - vs.y) * k);
        p.z = static_cast<float>(vs.z + (ve.z - vs.z) * k);
        //�������� ��������� �� ����
        d1 = dir | (p - ps);
        d2 = dir | (to - p);
        if (d1 < -0.00001f) continue;
        if (d2 < -0.00001f) continue;
        //����� �����������
        if (nb[j] >= 0) {
          curEdge = j;
          pnt = p;
          break;
        }
        else {
          curEdge = j;
          pnt = p;
          if (d1 != 0.0f && d2 != 0.0f) break;
        }
      }
    }

    Assert(curEdge >= 0);

    if (nb[curEdge] >= 0) {
      //��������� �� ��������� ��� � ���������� ��������
      //pos = pnt;
      fromNode = curNode;
      curNode = nb[curEdge];
    }
    else {
      //����� ����� �������� ���� ���������
      CVECTOR& vs = *(CVECTOR*)&vertex[trg[curEdge]];
      CVECTOR& ve = *(CVECTOR*)&vertex[trg[curEdge == 2 ? 0 : curEdge + 1]];
      //������� �����
      float nx = -(ve.z - vs.z);
      float nz = (ve.x - vs.x);
      //�����������
      float d = nx * nx + nz * nz;
      if (d == 0.0f) {
        //���������� ��������
        core.Trace("Patch have some problem -> triangle edge by zero length");
        //������ ��������
        pos = pnt;
        return curNode;
      }
      d = 1.0f / sqrtf(d);
      nx *= d;
      nz *= d;
      //������ ������ �� ������� ������������
      CVECTOR sld = to - pnt;
      sld.y = 0.0f;
      //��������� ����������
      d = nx * sld.x + nz * sld.z;
      if (d > 0.0f) d += 0.001f;
      if (d < 0.0f) d -= 0.001f;
      //��������
      CVECTOR newPos(to.x - nx * d, to.y, to.z - nz * d);
      if (newPos.x == to.x && newPos.z == to.z) {
        isSlide = true;
        return firstNode;
        Assert(false);
      }
      newPos = pos + (newPos - pos) * 0.99f;
      curNode = Move(firstNode, newPos, pos, depth + 1);
      isSlide = true;
      if (d >= 0.0f) {
        slideDir.x = nx;
        slideDir.y = 0.0f;
        slideDir.z = nz;
      }
      else {
        slideDir.x = -nx;
        slideDir.y = 0.0f;
        slideDir.z = -nz;
      }
      return curNode;
    }
  }
  if (loopCounter >= 256) //~!~
  {
    if (to.y != pos.y) {
      float hy = FindHeight(curNode, pos.x, pos.z);
      pos.y = to.y;
      if (pos.y < hy) pos.y = hy;
    }
    return curNode;
  }
  return -1;
}

//�������� ������� � ����
void PtcData::GetNodeNormal(long curNode, CVECTOR& n) const {
  if (curNode >= 0) {
    Assert(curNode < numTriangles);
    PtcNormal& nrm = normal[triangle[curNode].n];
    n.x = nrm.x;
    n.y = nrm.y;
    n.z = nrm.z;
  }
  else {
    n = 0.0f;
  }
}

//����� ����������� ����
bool PtcData::FindPathDir(long curNode, const CVECTOR& cur, long toNode, const CVECTOR& to, long& node,
                          CVECTOR& toPos) {
  numSteps = 0;
  if (curNode < 0 || toNode < 0) return false;
  if (FindPathDir(0, curNode, cur, toNode, to, node, toPos)) return true;
  toPos = to;
  return false;
}

//����� ����������� ����
bool PtcData::FindPathDir(long step, long curNode, const CVECTOR& cur, long toNode, const CVECTOR& to, long& node,
                          CVECTOR& pos) {
  if (step > numSteps) numSteps = step;
  Assert(curNode >= 0);
  Assert(toNode >= 0);
  //���� �����, �� ������ ����� ����������
  if (curNode == toNode || step >= PTCDATA_MAXSTEPS) {
    pos = to;
    node = toNode;
    return true;
  }
  //��������� � ����� ����������� ��������� (�����)
  Assert(curNode < numTriangles);
  Assert(toNode < numTriangles);
  uint8_t* line = table + curNode * lineSize;
  const uint8_t v = (line[toNode >> 2] >> ((toNode & 3) * 2)) & 3;
  if (v == 3) return false;
  //�����
  CVECTOR& vs = *(CVECTOR*)&vertex[triangle[curNode].i[v]];
  CVECTOR& ve = *(CVECTOR*)&vertex[triangle[curNode].i[v + 1 < 3 ? v + 1 : 0]];
  //����� �� �����
  const CVECTOR p = FindEdgePoint(vs, ve, cur, to);
  //�������������� ���������� ����
  const long nb = triangle[curNode].nb[v];
  if (nb < 0) return false;
  if (!FindPathDir(step + 1, nb, p, toNode, to, node, pos)) return false;
  //��������� ����� �� �����
  pos = FindEdgePoint(vs, ve, cur, pos);
  stepPos[step] = pos;
  node = nb;
  return true;
}

//����� ����� �� �����
CVECTOR PtcData::FindEdgePoint(const CVECTOR& vs, const CVECTOR& ve, const CVECTOR& cur, const CVECTOR& to) {
  CVECTOR pos;
  //������� � �����
  CVECTOR edge = ve - vs;
  edge.y = 0.0f;
  CVECTOR nrm(edge.z, 0.0f, -edge.x);
  const float nl = ~nrm;
  if (nl > 0.0f) {
    //���������
    nrm *= 1.0f / sqrtf(nl);
    const float dist = nrm | vs;
    //��������� ����� ����������
    const float dTo = (to | nrm) - dist;
    if (dTo <= 0.0f) {
      //����� ���������� �� ������
      const float dCur = (cur | nrm) - dist;
      if (dCur - dTo == 0.0f) return (vs + ve) * 0.5f;
      float k = dCur / (dCur - dTo);
      pos = cur + (to - cur) * k;
      //������������ ����� �����������
      CVECTOR p = vs + (ve - vs) * 0.001f;
      k = (ve - p) | (pos - p);
      if (k <= 0.0f) pos = p;
      p = vs + (ve - vs) * 0.999f;
      k = (vs - p) | (pos - p);
      if (k <= 0.0f) pos = p;
    }
    else {
      //����� ���������� ����� ������
      if (((to - cur) | edge) >= 0.0f) {
        //�������� �������� ����� �����
        pos = vs + (ve - vs) * 0.99f;
      }
      else {
        //�������� ��������� ����� �����
        pos = vs + (ve - vs) * 0.01f;
      }
    }
  }
  else {
    pos = (vs + ve) * 0.5f;
  }
  return pos;
}

//����� ����������� � ������
float PtcData::Trace(const CVECTOR& s, const CVECTOR& d) const {
  //������� ����������� �������
  float k = 2.0f;
  CVECTOR pmin = s;
  if (pmin.x > d.x) pmin.x = d.x;
  if (pmin.y > d.y) pmin.y = d.y;
  if (pmin.z > d.z) pmin.z = d.z;
  CVECTOR pmax = s;
  if (pmax.x < d.x) pmax.x = d.x;
  if (pmax.y < d.y) pmax.y = d.y;
  if (pmax.z < d.z) pmax.z = d.z;
  //���������� �� �����
  const long mnX = static_cast<long>((pmin.x - min.x) / ws);
  const long mnZ = static_cast<long>((pmin.z - min.z) / ls);
  const long mxX = static_cast<long>((pmax.x - min.x) / ws);
  const long mxZ = static_cast<long>((pmax.z - min.z) / ls);
  //���������� �� ���� ������������� � ����
  for (long zi = mnZ; zi <= mxZ; zi++) {
    if (zi < 0 || zi >= l) continue;
    for (long xi = mnX; xi <= mxX; xi++) {
      if (xi < 0 || xi >= w) continue;
      PtcMap& m = map[zi * w + xi];
      uint16_t* ids = indeces + m.start;
      for (long i = 0; i < m.size; i++) {
        const float kn = Trace(triangle[ids[i]], s, d);
        if (kn < k) k = kn;
      }
    }
  }
  return k;
}

//��������� ����������� ������������ � ��������
float PtcData::Trace(PtcTriangle& trg, const CVECTOR& s, const CVECTOR& d) const {
  //������� � ������������
  CVECTOR& n = *(CVECTOR*)&normal[trg.n];
  //��������� �� ������������
  const float dst = n | *(CVECTOR*)&vertex[trg.i[0]];
  //��������� ������� �� ���������
  const float d1 = (n | s) - dst;
  const float d2 = (n | d) - dst;
  //���� �� ���������� ���������, �� ��������
  if (d1 * d2 > 0.0f) return 2.0f;
  //����� ����������� ���� � ����������
  float k;
  if (d1 - d2 != 0.0f) k = d1 / (d1 - d2);
  else k = 0.0f;
  if (k < 0.0f) k = 0.0f;
  if (k > 1.0f) k = 1.0f;
  const CVECTOR pnt = s + (d - s) * k;
  //�������
  CVECTOR& v0 = *(CVECTOR*)&vertex[trg.i[0]];
  CVECTOR& v1 = *(CVECTOR*)&vertex[trg.i[1]];
  CVECTOR& v2 = *(CVECTOR*)&vertex[trg.i[2]];
  //�������� ���������� ����� � ������������
  CVECTOR en;
  en = (v1 - v0) ^ n;
  if ((en | pnt) - (en | v0) > 0.0f) return 2.0f;
  en = (v2 - v1) ^ n;
  if ((en | pnt) - (en | v1) > 0.0f) return 2.0f;
  en = (v0 - v2) ^ n;
  if ((en | pnt) - (en | v2) > 0.0f) return 2.0f;
  //������
  return k;
}

//����� ���� ������������� �� ����
void PtcData::FindForce(long curNode, CVECTOR& force) const {
  force = 0.0f;
  if (curNode < 0 || curNode >= numTriangles) return;
  short* nb = triangle[curNode].nb;
  for (long i = 0; i < 3; i++) {
    if (nb[i] >= 0) continue;
    //������� � �����
    const long s = triangle[curNode].i[i];
    const long e = triangle[curNode].i[i < 2 ? i + 1 : 0];
    //������� �����
    CVECTOR& vs = *(CVECTOR*)&vertex[s];
    CVECTOR& ve = *(CVECTOR*)&vertex[e];
    //������� �����
    float nx = (ve.z - vs.z);
    float nz = -(ve.x - vs.x);
    const float nl = sqrtf(nx * nx + nz * nz);
    if (!nl) continue;
    nx /= nl;
    nz /= nl;
    //���� ������������
    force.x += nx;
    force.z += nz;
  }
}

//����� ���� ������������� �� ����
void PtcData::FindForce(long curNode, const CVECTOR& pos, float dist, CVECTOR& force) const {
  force = 0.0f;
  if (curNode < 0 || curNode >= numTriangles) return;
  short* nb = triangle[curNode].nb;
  for (long i = 0; i < 3; i++) {
    if (nb[i] >= 0) continue;
    //������� � �����
    const long s = triangle[curNode].i[i];
    const long e = triangle[curNode].i[i < 2 ? i + 1 : 0];
    //������� �����
    CVECTOR& vs = *(CVECTOR*)&vertex[s];
    CVECTOR& ve = *(CVECTOR*)&vertex[e];
    //������� �����
    float nx = (ve.z - vs.z);
    float nz = -(ve.x - vs.x);
    const float nl = sqrtf(nx * nx + nz * nz);
    if (nl < 1e-10f) continue;
    nx /= nl;
    nz /= nl;
    //��������� �� �����
    float d = pos.x * nx + pos.z * nz - vs.x * nx - vs.z * nz;
    if (d >= dist) continue;
    //���� ������������
    if (d < 0.5f) d = 0.5f;
    d = 1.0f / d;
    force.x += nx * d;
    force.z += nz * d;
  }
}

//�������� �������� ����
const char* PtcData::GetMaterial(long curNode) {
  if (!materials) return nullptr;
  if (curNode < 0 || curNode >= numTriangles) return nullptr;
  const long mtl = triangle[curNode].mtl;
  if (mtl >= 15 || mtl < 0 || mtl >= materials->numMaterials) return nullptr;
  return materials->material[mtl];
}

/*
//�������� ������������ ������������ ������ �������
PtcData::Triangle * PtcData::GetTriangles(float x, float z, float sx, float sz, long & num)
{
	num = 0;
	numClTriangles = 0;
	float minX = x - sx*0.5f;
	float minZ = z - sz*0.5f;
	float maxX = x + sx*0.5f;
	float maxZ = z + sz*0.5f;
	long mnX = long((minX - min.x)/ws);
	long mnZ = long((minZ - min.z)/ls);
	long mxX = long((maxX - min.x)/ws);
	long mxZ = long((maxZ - min.z)/ls);
	for(long zi = mnZ; zi <= mxZ; zi++)
	{
		if(zi < 0 || zi >= l) continue;
		for(long xi = mnX; xi <= mxX; xi++)
		{
			if(xi < 0 || xi >= w) continue;
			PtcMap & m = map[zi*w + xi];
			uint16_t * ids = indeces + m.start;
			for(long i = 0; i < m.size; i++) AddClTriangle(ids[i]);
		}
	}
	num = numClTriangles;
	return ctriangle;
}

//�������� ����������� � �����
inline void PtcData::AddClTriangle(long i)
{
	for(long j = 0; j < numClTriangles; j++)
		if(ctriangle[j].index == i) return;
	if(numClTriangles >= maxClTriangles)
	{
		maxClTriangles += 64;
		ctriangle = (Triangle *)RESIZE(ctriangle, maxClTriangles*sizeof(Triangle));
	}
	Triangle & ct = ctriangle[numClTriangles++];
	uint16_t * idx = triangle[i].i;
	ct.index = i;
	ct.v[0].x = vertex[idx[0]].x;
	ct.v[0].y = vertex[idx[0]].y;
	ct.v[0].z = vertex[idx[0]].z;
	ct.v[1].x = vertex[idx[1]].x;
	ct.v[1].y = vertex[idx[1]].y;
	ct.v[1].z = vertex[idx[1]].z;
	ct.v[2].x = vertex[idx[2]].x;
	ct.v[2].y = vertex[idx[2]].y;
	ct.v[2].z = vertex[idx[2]].z;
	ct.n.x = normal[triangle[i].n].x;
	ct.n.y = normal[triangle[i].n].y;
	ct.n.z = normal[triangle[i].n].z;
}*/

//��������� ������ ����� �� ��������� ������������
inline float PtcData::FindHeight(long trgID, float x, float z) {
  Assert(trgID >= 0 && trgID < numTriangles);
  CVECTOR& n = *(CVECTOR*)&normal[triangle[trgID].n];
  float d = n | *(CVECTOR*)&vertex[triangle[trgID].i[0]];
  d = static_cast<float>((d - n.x * x - n.z * z) / static_cast<double>(n.y) + 0.0001);
  return d;
}

//���������� ���������
void PtcData::DebugDraw(VDX9RENDER* rs, float dltTime) {
  if (numTriangles <= 0) return;
  if (!dbgTriangles) {
    dbgTriangles = new DbgVertex[numTriangles * 3];
    for (long i = 0; i < numTriangles; i++) {
      dbgTriangles[i * 3 + 0].x = vertex[triangle[i].i[0]].x;
      dbgTriangles[i * 3 + 0].y = vertex[triangle[i].i[0]].y;
      dbgTriangles[i * 3 + 0].z = vertex[triangle[i].i[0]].z;
      dbgTriangles[i * 3 + 0].c = 0xff00008f;
      dbgTriangles[i * 3 + 1].x = vertex[triangle[i].i[1]].x;
      dbgTriangles[i * 3 + 1].y = vertex[triangle[i].i[1]].y;
      dbgTriangles[i * 3 + 1].z = vertex[triangle[i].i[1]].z;
      dbgTriangles[i * 3 + 1].c = dbgTriangles[i * 3 + 0].c;
      dbgTriangles[i * 3 + 2].x = vertex[triangle[i].i[2]].x;
      dbgTriangles[i * 3 + 2].y = vertex[triangle[i].i[2]].y;
      dbgTriangles[i * 3 + 2].z = vertex[triangle[i].i[2]].z;
      dbgTriangles[i * 3 + 2].c = dbgTriangles[i * 3 + 0].c;
    }
  }
  if (!dbgEdges) {
    dbgEdges = new DbgVertex[numTriangles * 3 * 2];
    for (long i = 0; i < numTriangles; i++) {
      for (long j = 0; j < 3; j++) {
        float y = 0.05f;
        if (triangle[i].nb[j] >= 0 && triangle[i].nb[j] < i) y += 0.01f;
        dbgEdges[i * 6 + j * 2 + 0].x = vertex[triangle[i].i[j]].x;
        dbgEdges[i * 6 + j * 2 + 0].y = vertex[triangle[i].i[j]].y + 0.001f;
        dbgEdges[i * 6 + j * 2 + 0].z = vertex[triangle[i].i[j]].z;
        dbgEdges[i * 6 + j * 2 + 0].c = triangle[i].nb[j] >= 0 ? 0xff008f00 : 0xff8f0000;
        dbgEdges[i * 6 + j * 2 + 1].x = vertex[triangle[i].i[j + 1 < 3 ? j + 1 : 0]].x;
        dbgEdges[i * 6 + j * 2 + 1].y = vertex[triangle[i].i[j + 1 < 3 ? j + 1 : 0]].y + 0.001f;
        dbgEdges[i * 6 + j * 2 + 1].z = vertex[triangle[i].i[j + 1 < 3 ? j + 1 : 0]].z;
        dbgEdges[i * 6 + j * 2 + 1].c = dbgEdges[i * 6 + j * 2 + 0].c;
      }
    }
  }
  const char* tech = "DbgPatchViewZ";
  rs->SetTransform(D3DTS_WORLD, CMatrix());
  rs->DrawPrimitiveUP(D3DPT_TRIANGLELIST, D3DFVF_XYZ | D3DFVF_DIFFUSE, numTriangles, dbgTriangles, sizeof(DbgVertex),
                      tech);
  rs->DrawPrimitiveUP(D3DPT_LINELIST, D3DFVF_XYZ | D3DFVF_DIFFUSE, numTriangles * 3, dbgEdges, sizeof(DbgVertex),
                      tech);
}
