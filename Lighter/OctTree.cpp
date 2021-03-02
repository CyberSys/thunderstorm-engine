//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	OctTree
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#include "OctTree.h"

//============================================================================================

#define LLOT_MAX	128

OctTree::OTNode::OTNode(CVECTOR & _min, CVECTOR & _max)
{
	for(long i = 0; i < 8; i++) node[i] = null;
	min = _min; max = _max;
	vrt = NEW Vertex * [LLOT_MAX];
	num = 0;
}

OctTree::OTNode::~OTNode()
{
	if(vrt) delete vrt;
	for(long i = 0; i < 8; i++) if(node[i]) delete node[i];
}

//============================================================================================
//���������������, ������������������
//============================================================================================

OctTree::OctTree()
{
	root = null;
	verts = null;
	numVerts = 0;
	maxVerts = 0;
	vrt = null;
	numVrt = 0;
}

OctTree::~OctTree()
{
	if(root) delete root;
	if(verts) delete verts;
}

//���������������� ������
void OctTree::Init(LGeometry * g)
{	
	vrt = g->vrt;
	numVrt = g->numVrt;
	root = NEW OTNode(g->min, g->max);
	for(long i = 0; i < numVrt; i++) AddVertex(root, vrt + i);
	Optimize(root);
	/*
	for(i = 0; i < numVrt; i++)
	{
		long num = Check(root, vrt + i, 0);
		Assert(num == 1);
	}
	*/
}

long OctTree::Check(OTNode * node, Vertex * v, long num)
{
	if(node->vrt)
	{
		for(long i = 0; i < node->num; i++) if(node->vrt[i] == v) num++;
	}else{
		for(long i = 0; i < 8; i++) if(node->node[i]) num += Check(node->node[i], v, 0);
	}
	return num;
}

bool OctTree::AddVertex(OTNode * node, Vertex * v)
{
	CVECTOR & min = node->min;
	CVECTOR & max = node->max;
	if(v->p.x < min.x || v->p.x > max.x) return false;
	if(v->p.y < min.y || v->p.y > max.y) return false;
	if(v->p.z < min.z || v->p.z > max.z) return false;
	if(node->vrt)
	{
		if(node->num < LLOT_MAX)
		{
			//��������� � ������� ���
			node->vrt[node->num++] = v;
			return true;
		}else{
			//������������, ���� ������������ �� �����
			CVECTOR cnt = (node->min + node->max)*0.5f;
			node->node[0] = NEW OTNode(CVECTOR(min.x, min.y, min.z), CVECTOR(cnt.x, cnt.y, cnt.z));
			node->node[1] = NEW OTNode(CVECTOR(min.x, min.y, cnt.z), CVECTOR(cnt.x, cnt.y, max.z));
			node->node[2] = NEW OTNode(CVECTOR(cnt.x, min.y, cnt.z), CVECTOR(max.x, cnt.y, max.z));
			node->node[3] = NEW OTNode(CVECTOR(cnt.x, min.y, min.z), CVECTOR(max.x, cnt.y, cnt.z));
			node->node[4] = NEW OTNode(CVECTOR(min.x, cnt.y, min.z), CVECTOR(cnt.x, max.y, cnt.z));
			node->node[5] = NEW OTNode(CVECTOR(min.x, cnt.y, cnt.z), CVECTOR(cnt.x, max.y, max.z));
			node->node[6] = NEW OTNode(CVECTOR(cnt.x, cnt.y, cnt.z), CVECTOR(max.x, max.y, max.z));
			node->node[7] = NEW OTNode(CVECTOR(cnt.x, cnt.y, min.z), CVECTOR(max.x, max.y, cnt.z));
			for(long n = 0; n < node->num; n++)
			{
				for(long c = 0; c < 8; c++)
				{
					if(AddVertex(node->node[c], node->vrt[n])) break;
				}
				Assert(c < 8);
			}
			delete node->vrt;
			node->vrt = null;
			node->num = 0;
		}
	}
	//��������� �����
	for(long c = 0; c < 8; c++)
	{
		if(AddVertex(node->node[c], v)) break;
	}
	Assert(c < 8);	
	return true;
}

//����������� ������
void OctTree::Optimize(OTNode * node)
{
	if(!node->vrt)
	{
		for(long i = 0; i < 8; i++)
		{
			if(!node->node[i]) continue;
			if(node->node[i]->vrt)
			{
				if(node->node[i]->num <= 0)
				{
					delete node->node[i];
					node->node[i] = null;
				}
			}else Optimize(node->node[i]);			
		}
	}
}

//����� ������� � �������� �������
void OctTree::FindVerts(CVECTOR & pos, float r)
{
	numVerts = 0;
	vertsPos = pos;
	vertsR = r*r;
	r += 0.000001f;
	vertsPosMin = pos - CVECTOR(r);
	vertsPosMax = pos + CVECTOR(r);
	if(root) FindVerts(root);
}

//�����
void OctTree::FindVerts(OTNode * node)
{
	CVECTOR & min = node->min;
	CVECTOR & max = node->max;
	//��������������� ��������
	if(vertsPosMin.x > max.x) return;
	if(vertsPosMax.x < min.x) return;
	if(vertsPosMin.y > max.y) return;
	if(vertsPosMax.y < min.y) return;
	if(vertsPosMin.z > max.z) return;
	if(vertsPosMax.z < min.z) return;
	//�������� ��������

	//���� ��� ������ ������ �������� � �����
	if(node->vrt == null)
	{
		for(long i = 0; i < 8; i++) if(node->node[i]) FindVerts(node->node[i]);
	}else{
		for(long i = 0; i < node->num; i++)
		{
			float r = ~(node->vrt[i]->p - vertsPos);
			if(r < vertsR)
			{
				if(numVerts >= maxVerts)
				{
					maxVerts += 1024;
					verts = (OctFndVerts *)RESIZE(verts, maxVerts*sizeof(OctFndVerts));
				}
				verts[numVerts].v = node->vrt[i];
				verts[numVerts++].r2 = r;
			}
		}
	}
}

