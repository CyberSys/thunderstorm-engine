//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	CharacterAnimationKipper
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#include "CharacterAnimationKipper.h"
#include "dx9render.h"
#include "EntityManager.h"

//============================================================================================
//���������������, ������������������
//============================================================================================

CharacterAnimationKipper::CharacterAnimationKipper()
{
	asr = nullptr;
	aniMan = nullptr;
	aniWoman = nullptr;
	for(long i = 0; i < sizeof(lockTextures)/sizeof(long); i++) lockTextures[i] = -1;
	numLTextures = 0;
	rs = nullptr;
}

CharacterAnimationKipper::~CharacterAnimationKipper()
{
	delete aniMan;
	delete aniWoman;
	if(rs)
	{
		for(long i = 0; i < sizeof(lockTextures)/sizeof(long); i++) 
			if(lockTextures[i] >= 0) rs->TextureRelease(lockTextures[i]);
	}
}

//�������������
bool CharacterAnimationKipper::Init()
{
	//�������� ��� ������������
	auto& entities = EntityManager::GetEntityIdVector("CharacterAnimationKipper");
	for (auto eid : entities) {
		if (EntityManager::GetEntityPointer(eid) == this)
			continue;

		api->Trace("CharacterAnimationKipper::Init() -> CharacterAnimationKipper already created");
		return false;
	}

	rs = (VDX9RENDER *)api->CreateService("dx9render");
	if(!rs) throw std::exception("No service: dx9render");
	auto* asr = (AnimationService *)api->CreateService("AnimationServiceImp");
	if(!asr) throw std::exception("Anumation service not created!");
	aniMan = asr->CreateAnimation("man");
	aniWoman = asr->CreateAnimation("towngirl");
	//LockTexture("dialog\dialog.tga");
	return true;
}

void CharacterAnimationKipper::LockTexture(const char * texture)
{
	if(numLTextures >= sizeof(lockTextures)/sizeof(long)) return;
	lockTextures[numLTextures++] = rs->TextureCreate(texture);
}

