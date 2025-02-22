#ifndef _RANDS_H_
#define _RANDS_H_
#include <cstdlib>

inline float rand(float r)
// returns random float
{
  static const auto randMax = static_cast<float>(RAND_MAX);
  return r * (static_cast<float>(rand()) / randMax);
}

//--------------------------------------------------------------------
inline float randUpper(float r)
// returns larger half of random float
{
  const auto half = r / 2.0f;
  return (half + rand(half));
}

//--------------------------------------------------------------------
inline float randCentered(float r)
// returns random float around 0
{
  const auto half = r / 2.0f;
  return (rand(r) - half);
}
#endif
