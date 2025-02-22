#ifndef _RIGGING_DEFINE_H_
#define _RIGGING_DEFINE_H_

#define TEXTURE_RELEASE(rs,tex)	if(tex!=-1 && rs!=NULL) {rs->TextureRelease(tex); tex=-1;}
#define VERTEX_BUFFER_RELEASE(rs,vb) if(vb!=-1 && rs!=NULL) {rs->ReleaseVertexBuffer(vb); vb=-1;}
#define INDEX_BUFFER_RELEASE(rs,ib) if(ib!=-1 && rs!=NULL) {rs->ReleaseIndexBuffer(ib); ib=-1;}

#endif
