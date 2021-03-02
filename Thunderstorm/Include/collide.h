#ifndef __COLLIDE_H__
#define __COLLIDE_H__


#include "Cvector.h"

#include "../Common_h/vmodule_api.h"
#include "../Common_h/types3d.h"
#include "../Common_h/object.h"
#include "../Common_h/EntityManager.h"


class LOCAL_COLLIDE
{
public:
	virtual ~LOCAL_COLLIDE() {};

	//clip faces into the box and returns num of faces
	virtual long SetBox(const CVECTOR &boxSize, const CMatrix &transform, bool testOnly = false) = 0;

	//returns pointer to clipped face and fills num of vertices in face
	virtual const CVECTOR *GetFace(long &numVertices) = 0;

	//trace a ray inside the box
	virtual float Trace(const CVECTOR &src, const CVECTOR &dst) = 0;

};

class COLLIDE : public SERVICE
{
public:
	virtual ~COLLIDE() = default;

	//creates LocalCollide object which must be deleted after use
	virtual LOCAL_COLLIDE* CreateLocalCollide(EntityManager::layer_index_t idx) = 0;

	virtual float Trace(entid_t entity, const CVECTOR& src, const CVECTOR& dst) = 0;

	virtual float Trace(EntityManager::LayerIterators its, const CVECTOR &src, const CVECTOR &dst, const entid_t * exclude_list, long entities) = 0;

	virtual bool Clip(EntityManager::LayerIterators its, const PLANE *planes, long nplanes, const CVECTOR &center, float radius,
		ADD_POLYGON_FUNC addpoly, const entid_t * exclude_list, long entities) = 0;

	virtual entid_t GetObjectID() = 0;
};

#endif