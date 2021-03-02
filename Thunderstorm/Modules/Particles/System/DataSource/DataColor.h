#pragma once

#include "../../ICommon/MemFile.h"
#include "../../ICommon/ColorVertex.h"
#include <string>
#include <vector>

class DataColor {
  std::string Name;

  const Color ZeroColor;

  std::vector<ColorVertex> ColorGraph;

public:

  //�����������/����������
  DataColor();
  virtual ~DataColor();

  //�������� �������� (������� �����, ����� ����� �����, ��������� �������[0..1])
  Color GetValue(float Time, float LifeTime, float K_rand);

  //������������� "�������� �� ���������"
  //��� �������, Min=Max=Value
  void SetDefaultValue(const Color& Value);

  //���������� ��������
  void SetValues(const ColorVertex* Values, uint32_t Count);

  //�������� ���-�� ��������
  uint32_t GetValuesCount() const;

  //�������� ���. �������� (�� �������)
  const Color& GetMinValue(uint32_t Index);

  //�������� ����. �������� (�� �������)
  const Color& GetMaxValue(uint32_t Index);


  void Load(MemFile* File);
  void Write(MemFile* File);


  void SetName(const char* szName);
  const char* GetName() const;


  const ColorVertex& GetByIndex(uint32_t Index);
};
