#ifndef ANIMATION_H
#define ANIMATION_H

#include "Types.h"
#include "AnmManager.h"
#include "EdenLib/edAnim/AnmSkeleton.h"

struct edAnmAnim;

struct edAnmStage {
	int animMode;
	undefined4 field_0x4;
	float field_0x8;
	edF32MATRIX4* pFrameMatrixData;
	edF32MATRIX3* field_0x10;
	struct AnimMatrix* pAnimMatrix;
	struct edANM_WRTS* pRelativeTransformMatrixBuffer;
	edAnmSkeleton anmSkeleton;
	edANM_HDR* pKeyData;
	int field_0x24;
	undefined4 bLoop;
	float field_0x2c;
	float field_0x30;
	float field_0x34;
	float field_0x38;
	float field_0x3c;
	float field_0x40;
	undefined field_0x44;
	undefined field_0x45;
	undefined field_0x46;
	undefined field_0x47;
	undefined field_0x48;
	undefined field_0x49;
	undefined field_0x4a;
	undefined field_0x4b;
	bool field_0x4c;
	bool field_0x4d;
	undefined field_0x4e;
	undefined field_0x4f;

	void SetActor(edANM_SKELETON* pSkeleton);
	void SetDestinationWRTS(edANM_WRTS* pMatrixBuffer, int count);
	void BlendToDestWRTS(float alpha, edF32MATRIX4* m0, edF32MATRIX4* m1);
	void ToonWRTSToGlobalMatrices(uchar mode);
	static bool ComputeAnimParams(float param_1, float param_2, float param_3, float* param_4, bool param_5, int loopType);
	void SetAnim(edANM_HDR* pKeyData);
	void SetAnimLoop(edANM_HDR* pKeyData);
	void SetTime(float time);
	void AnimToWRTS();

};

union edANM_HDR_Internal {
	float asTime;
	int asKey;
};

struct edANM_HDR {
	uint flags;
	edANM_HDR_Internal field_0x4;
	edANM_HDR_Internal keyIndex_0x8;
};

struct AnimDesc {
	int animType;
	edANM_HDR* pHdrA;
	uint flags;
	int maxKey_0xc;
	edANM_HDR* pHdrB;
	int origAnimType;
	edAnmMacroAnimator state;
	float field_0x4c;
	int animMode;
};


struct edAnmStateDesc {
	int field_0x0;
	int animType;
	edANM_HDR* pHdrA;
	int flags;
	int maxKey_0x10;
	edANM_HDR* pHdrB;
	int origAnimType;
};

struct edAnmLayer {
	void Reset();
	void AnimateDT(float deltaTime);
	bool MorphingDT(float playTime);
	bool PlayingDT(float playTime);
	void SetRawAnim(edANM_HDR* pHdr, uint flags, uint type);
	bool MorphingInitDT(edAnmStateDesc* pNewAnimation);
	bool MorphingStartDT();

	void SetAnim(edAnmStateDesc* pDesc);
	int field_0x0;
	float field_0x4;
	int animPlayState;
	undefined4 field_0xc;
	AnimDesc currentAnimDesc;
	AnimDesc nextAnimDesc;
	int field_0xb8;
	float field_0xbc;
	AnimationCallback pFunction_0xc0;
	struct CActor* pActor;
	edANM_HDR** pAnimManagerKeyData;
	uint field_0xcc;
	float playRate;
	float field_0xd4;
};

struct edAnmMetaAnimator {
	int layerCount;
	edAnmLayer* aAnimData;

	void SetLayerProperties(int count);
	void SetLayerResourceArray(int index, edANM_HDR** pAnimManagerKeyData);
	void AnimateDT(float deltaTime);
};

struct edAnmBinMetaAnimator : public edAnmMetaAnimator {
	int* pAnimKeyEntryData;
	void SetAnimStatesBinary(void* pAnimKeyEntryData);
	void SetLayerMacroAnimCallback(int index, AnimationCallback callback);
	void SetLayerMacroAnimUserParams(int index, CActor* pActor);
	void SetLayerTimeWarper(float value, int index);
	void SetAnim(int animType, int origAnimType);
};

struct CAnimation {
	CAnimation();

	bool UpdateCurSkeleton(CActor* pActor);
	void Create(CActor* pActor, uint count, edAnmLayer* pAnimData, int layerCount);
	void StopEventTrack(int state);
	void Manage(float deltaTime, CActor* pActor, int bHasFlag, int bPlayingAnimation);
	void PlayAnim(CActor* pActor, int animType, int origAnimType);

	edAnmBinMetaAnimator anmBinMetaAnimator;
	edAnmSkeleton anmSkeleton;
	AnimMatrixData* pMatrixData_0x10;
	undefined4 field_0x14;
	undefined field_0x18;
	undefined field_0x19;
	undefined field_0x1a;
	undefined field_0x1b;
	undefined field_0x1c;
	undefined field_0x1d;
	undefined field_0x1e;
	undefined field_0x1f;
	undefined field_0x20;
	undefined field_0x21;
	undefined field_0x22;
	undefined field_0x23;
	edF32MATRIX3* pAnimMatrix;
	undefined4 field_0x28;
	uint count_0x2c;
	int currentAnimType_0x30;
	uint field_0x34;
	undefined4 field_0x38;
	undefined field_0x3c;
	undefined field_0x3d;
	undefined field_0x3e;
	undefined field_0x3f;
	undefined field_0x40;
	undefined field_0x41;
	undefined field_0x42;
	undefined field_0x43;
	undefined field_0x44;
	undefined field_0x45;
	undefined field_0x46;
	undefined field_0x47;
	undefined field_0x48;
	undefined field_0x49;
	undefined field_0x4a;
	undefined field_0x4b;
	undefined field_0x4c;
	undefined field_0x4d;
	undefined field_0x4e;
	undefined field_0x4f;
	undefined field_0x50;
	undefined field_0x51;
	undefined field_0x52;
	undefined field_0x53;
	int count_0x54;
};

extern edAnmStage TheAnimStage;

#endif // ANIMATION_H