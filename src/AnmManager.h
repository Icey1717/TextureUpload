#ifndef _ANM_MANAGER_H
#define _ANM_MANAGER_H

#include "Types.h"
#include "LargeObject.h"

struct edAnmMacroAnimator;
class CActor;

typedef void (*AnimationCallback)(edAnmMacroAnimator* pAnmMacroAnimator, CActor* pActor, uint32_t param_3);

PACK(
	struct AnimHeaderPacked {
	int hash;
	int offset;
	float field_0x8;
});

struct edANM_HDR;

struct edAnmAnim {
	static edANM_HDR* LoadFromMem(char* pFileData, int size);
};

struct AnimMatrixData {
	edF32MATRIX4 matrix;
	AnimMatrixData* pNext;
	AnimMatrixData* pPrev;
	uint key_0x48;
	short field_0x4c;
	short usedByCount;
};

struct edAnmMacroAnimator {
	uint flags;
	uint currentAnimDataFlags;
	undefined4 field_0x8;
	float time_0xc;
	float time_0x10;
	float keyStartTime_0x14;
	float keyDuration_0x18;
	struct edANM_HDR* pAnimKeyTableEntry;
	struct edANM_HDR** pKeyDataArray;
	AnimationCallback pFunction;
	CActor* pActor;
	int animationType;
	float field_0x30;

	void Initialize(float param_1, edANM_HDR* pHdr, bool param_4, uint param_5);
	void AnimateDT(float param_1);
	void Animate();
	void UpdateAnimParams();
};

struct edANM_WRTS {
	edF32MATRIX4 matrices[128];
};

struct edAnmManager {
	byte tempMatrixMemoryUsage[4];
	undefined field_0x4;
	undefined field_0x5;
	undefined field_0x6;
	undefined field_0x7;
	undefined field_0x8;
	undefined field_0x9;
	undefined field_0xa;
	undefined field_0xb;
	undefined field_0xc;
	undefined field_0xd;
	undefined field_0xe;
	undefined field_0xf;
	edANM_WRTS tempMatrixBufferArray[4];
	char* pBufferStart_0x8010;
	char* pBufferEnd_0x8014; /* Created by retype action */
	char* pBufferSeekPos_0x8018; /* Created by retype action */

	void Initialize(char* pBufferStart, int size);
	void Reset();
	edF32MATRIX4* GetMatrixBuffer(int count);
	edANM_WRTS* AllocWRTSBuffer();
	void FreeWRTSBuffer(edANM_WRTS* pBuffer);
};

extern edAnmManager TheAnimManager;

struct CAnimationManager : public CObjectManager {
public:
	virtual void Game_Init();
	virtual void Game_Term();
	virtual bool LevelLoading_Manage();
	virtual void Level_ClearAll();
	virtual void Level_Manage();
	virtual void Level_ManagePaused();

	void Level_Create(ByteCode* pMemoryStream);

	AnimMatrixData* aAnimMatrixData;
	edF32MATRIX4* pBufferB_0x8;
	char* pAnimKeyEntryData;
	int levelAnimCount;
	int loadedAnimKeyDataCount;
	edANM_HDR** pAnimKeyTable;

	static AnimationCallback _gLayersCallbacks[4];
};

extern edAnmManager TheAnimManager;

#endif // _ANM_MANAGER_H