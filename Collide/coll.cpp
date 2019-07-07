#include "vcollide.h"
#include "../Common_h/object.h"

INTERFACE_FUNCTION
CREATE_SERVICE(COLL)

entid_t last_trace_eid;

COLL::COLL()
{
}
COLL::~COLL()
{
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
LOCAL_COLLIDE *COLL::CreateLocalCollide(const char *layerName)
{
	return new LCOLL(layerName, *api);
}
//----------------------------------------------------------------------------------
//Ray tracing
//----------------------------------------------------------------------------------
float COLL::Trace(entid_t entity, const CVECTOR &src, const CVECTOR &dst)
{
	COLLISION_OBJECT *cob = static_cast<COLLISION_OBJECT*>(api->GetEntityPointer(entity));
	if(static_cast<Entity*>(cob)== nullptr)	return 2.0f;

	last_trace_eid = entity;
	return cob->Trace(src, dst);
}

//----------------------------------------------------------------------------------
//with enclusion list
//----------------------------------------------------------------------------------
float COLL::Trace(walker_t walker, const CVECTOR &src, const CVECTOR &dst, const entid_t * exclude_list, long entities)
{
	float best_res = 2.0f;
	while (const auto eid = walker())
	{
		long e;
		for(e=0; e<entities; e++)
			if(eid==exclude_list[e])	break;
		if(e==entities)
		{
			COLLISION_OBJECT *cob = static_cast<COLLISION_OBJECT*>(api->GetEntityPointer(eid));
			if(cob!= nullptr)
			{
				float res = cob->Trace(src, dst);
				if(res<best_res)
				{
					best_res = res;
					last_trace_eid = eid;
				}
			}
		}
	}

	return best_res;
}


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool COLL::Clip(walker_t walker, const PLANE *planes, long nplanes, const CVECTOR &center, float radius,
	ADD_POLYGON_FUNC addpoly, const entid_t * exclude_list, long entities)
{
	bool retval = false;

	while(const auto eid = walker())
	{
		long e;
		for(e=0; e<entities; e++)
			if(eid==exclude_list[e])	break;
		if(e==entities)
		{
			COLLISION_OBJECT *cob = static_cast<COLLISION_OBJECT*>(api->GetEntityPointer(eid));
			if(cob!= nullptr)
			{
				last_trace_eid = eid;
				if(cob->Clip(planes, nplanes, center, radius, addpoly)==true)
					retval = true;
			}
		}
	}

	return retval;
}


//----------------------------------------------------------------------------------
//get last trace entity id
//----------------------------------------------------------------------------------
entid_t COLL::GetObjectID()
{
	return last_trace_eid;
}
