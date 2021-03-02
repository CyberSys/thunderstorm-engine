#include "SinkSplashDefines.h"
#include "TSinkSplash.h"

//--------------------------------------------------------------------
TSinkSplash::TSinkSplash()
	: ps(nullptr)
	  , enabled(false)
	  , sea(nullptr)
	  , time(0)
{
}

//--------------------------------------------------------------------
TSinkSplash::~TSinkSplash()
{
	Release();
}

//--------------------------------------------------------------------
void TSinkSplash::Initialize(INIFILE* _ini, SEA_BASE* _sea)
{
	//ps = new SEPS_PS();
	//ps->Init(_ini, "sink");

	sea = _sea;
}

//--------------------------------------------------------------------
void TSinkSplash::Release()
{
	//if (ps)
	//	delete ps;
}

//--------------------------------------------------------------------
void TSinkSplash::Start(const CVECTOR& _pos, uint16_t* _indexes, SINK_VERTEX* _vertexes, long vOffset)
{
	time = 0;
	center = _pos;
	enabled = true;
	growK = rand(1.5f) + randUpper(0.1f);

  auto midY = sea->WaveXZ(center.x, center.z) + SPLASH_MOVE_Y;
	distortDivider = rand(SPLASH_DISTORT_DIVIDER) + 10.0f;
	//	_VSYSTEM_API->Trace("i=%X, v=%X", _indexes, _vertexes);
  auto stepSize = GRID_LENGTH / GRID_STEPS;
  auto halfSize = GRID_LENGTH / 2.0f;
	for (uint16_t z = 0; z < GRID_STEPS; ++z)
		for (uint16_t x = 0; x < GRID_STEPS; ++x)
		{
			_vertexes->pos.x = center.x + stepSize * x - halfSize;
			_vertexes->pos.z = center.z + stepSize * z - halfSize;
			_vertexes->pos.y = midY;
			_vertexes->tu = ((float)x) / ((float)GRID_STEPS);
			_vertexes->tv = ((float)z) / ((float)GRID_STEPS);
			//_vertexes->color = 0x44FFFF00;
			++_vertexes;
		}

	for (uint16_t z = 0; z < GRID_STEPS - 1; ++z)
		for (uint16_t x = 0; x < GRID_STEPS - 1; ++x)
		{
			*(_indexes++) = vOffset + GRID_STEPS * z + x;
			*(_indexes++) = vOffset + GRID_STEPS * (z + 1) + x;
			*(_indexes++) = vOffset + GRID_STEPS * (z + 1) + x + 1;

			*(_indexes++) = vOffset + GRID_STEPS * z + x;
			*(_indexes++) = vOffset + GRID_STEPS * (z + 1) + x + 1;
			*(_indexes++) = vOffset + GRID_STEPS * z + x + 1;
		}

	//ps->Reset();
}

//--------------------------------------------------------------------
float TSinkSplash::HeightF(uint32_t _time, float _r, float _k)
{
  auto tPhase = ((float)_time) * (PI / SPLASH_FADE_TIME);

  auto fCos = 1 / (_r + SPLASH_START_ARG + _time / distortDivider);
  auto secondPart = ((GRID_LENGTH - _r) / GRID_LENGTH) * (1.0f + fabs(
		sin((-PI * ((float)_time) / SPLASH_FADE_TIME) + PIm2 * _r / GRID_LENGTH)));
  auto k = ((float)_time) / SPLASH_FADE_TIME;
	if (k > 1.0f)
		k = 1.0f;
	return _k * (SPLASH_HEIGHT_MULTIPLIER * (/*(1.0f-k)*fCos*sin(tPhase) +*/ k * secondPart));
}

//--------------------------------------------------------------------
void TSinkSplash::Process(uint32_t _dTime, uint16_t* _indexes, SINK_VERTEX* _vertexes)
{
	if (!enabled)
		return;

  auto alpha = rand(PI * 2);
  auto rho = randUpper(MAX_RHO);
  auto r = SPLASH_RADIUS;
	static CVECTOR pos;
	static CVECTOR localDir;
	pos = center;
	pos.x += r * cos(alpha);
	pos.z += r * sin(alpha);
	localDir.x = sin(rho) * cos(alpha);
	localDir.y = 1.0f;
	localDir.z = sin(rho) * sin(alpha);
	//localDir += /*(((float) time) / (2 * SPLASH_FADE_TIME))**/this->dir;
	//ps->SetEmitter(pos, localDir);

	time += _dTime;

	if (time > 3 * SPLASH_FADE_TIME)
	{
		enabled = false;
		return;
	}

  auto midY = sea->WaveXZ(center.x, center.z) + SPLASH_MOVE_Y;
  auto stepSize = ((float)GRID_LENGTH) / ((float)GRID_STEPS);
  auto halfSize = GRID_LENGTH / 2.0f;
	CVECTOR curPos;
	int x, z;
	float localHeight, prolongK;

	for (z = 0; z < GRID_STEPS; ++z)
		for (x = 0; x < GRID_STEPS; ++x)
		{
      auto curPosNormed = !CVECTOR(stepSize * x - halfSize, 0.0f, stepSize * z - halfSize);
			prolongK = curPosNormed.x * this->dir.x + curPosNormed.z * this->dir.z;
			curPos.x = (center.x + stepSize * x) - halfSize;
			curPos.z = (center.z + stepSize * z) - halfSize;

			_vertexes->pos.x = center.x + growK * (stepSize * x * prolongK - halfSize);
			_vertexes->pos.z = center.z + growK * (stepSize * z * prolongK - halfSize);
			_vertexes->color = ((((3 * SPLASH_FADE_TIME - (long)time)) << 8) / (3 * SPLASH_FADE_TIME)) << 24;

			if ((x > 0) && (z > 0) && (x < GRID_STEPS - 1) && (z < GRID_STEPS - 1))
			{
				rho = sqrt(
					(curPos.x - center.x) * (curPos.x - center.x) + (curPos.z - center.z) * (curPos.z - center.z));
				localHeight = HeightF(time, rho, (GRID_LENGTH - rho) / GRID_LENGTH);

				_vertexes->pos.x = curPos.x;
				_vertexes->pos.y = midY + growK * localHeight;
				_vertexes->pos.z = curPos.z;
			}
			else
			{
				_vertexes->pos.x = curPos.x;
				_vertexes->pos.y = midY;
				_vertexes->pos.z = curPos.z;
			}

			++_vertexes;
		}
}

//--------------------------------------------------------------------
bool TSinkSplash::Enabled()
{
	return enabled;
}

//--------------------------------------------------------------------
void TSinkSplash::Reset(uint16_t* _indexes, SINK_VERTEX* _vertexes)
{
	time = 0;
	enabled = false;
	/*
		float midY = sea->WaveXZ(center.x, center.z) + SPLASH_MOVE_Y;
		float stepSize = GRID_LENGTH / GRID_STEPS;
		float halfSize = GRID_LENGTH / 2.0f;
		int x,z;
		for (z=0; z < GRID_STEPS; ++z)
		for (x=0; x < GRID_STEPS; ++x)
		{
			_vertexes->pos.x = center.x + stepSize * x - halfSize;
			_vertexes->pos.z = center.z + stepSize * z - halfSize;
			_vertexes->pos.y = midY;
			_vertexes->tu = ((float) x) / ((float) GRID_STEPS);
			_vertexes->tv = ((float) z) / ((float) GRID_STEPS);
			++_vertexes;
		}
	*/
}

//--------------------------------------------------------------------
void TSinkSplash::AdditionalRealize(uint32_t dTime)
{
	//ps->Realize(_dTime);
}

//--------------------------------------------------------------------
