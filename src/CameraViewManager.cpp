#include "CameraViewManager.h"
#include "ed3D.h"
#include "MemoryStream.h"
#include "MathOps.h"
#include <math.h>

#ifdef PLATFORM_PS2
#include <libvu0.h>
#endif
#include "TimeController.h"

#define EDFCAMERA_LOG(level, format, ...) MY_LOG_CATEGORY("edFCamera", level, format, ##__VA_ARGS__)

CameraManager* CameraManager::_gThis = NULL;
CameraManager* g_CameraViewManager_00448e9c = NULL;
edFCamera _gDisplayCamera = { 0 };
edFCamera gSceneCameras[10] = { 0 };

edFCamera CameraManager::_gFrontEndCamera = { 0 };

CameraManager::CameraManager()
{
	//FUN_003fedc0((long)&this->field_0x710);
	_gThis = this;
	this->field_0xa7c = 0;
	Level_ClearInternalData();
	return;
}

void CameraManager::Game_Init()
{
	this->aspectRatio = 1.333333f;
	return;
}

void ActiveCamManager::SetActiveCam(Camera* pCamera)

{
	this->pActiveCamera = pCamera;
	/* WARNING: Could not recover jumptable at 0x003feb1c. Too many branches */
	/* WARNING: Treating indirect jump as call */
	pCamera->ManageEvent(2, 1);
	return;
}

void ActiveCamManager::ClearActiveCam()
{
	this->activeIndex = -1;
	return;
}

ActiveCamManager::ActiveCamManager()
{
	this->pActiveCamera = (Camera*)0x0;
	this->activeIndex = -1;
}

bool ActiveCamManager::SwitchActiveCam(float param_1, Camera* pCamera, SWITCH_MODE switchMode)
{
	bool bVar1;
	Camera* pCVar2;
	int iVar3;
	int iVar4;

	iVar4 = this->activeIndex;
	if (iVar4 < -1) {
		pCVar2 = (Camera*)0x0;
	}
	else {
		if (iVar4 == -1) {
			pCVar2 = this->pActiveCamera;
		}
		else {
			pCVar2 = *(Camera**)(&this->field_0x40 + iVar4 * 0x50);
		}
	}
	if (pCVar2 == pCamera) {
		bVar1 = false;
	}
	else {
		if (switchMode == SWITCH_MODE_B) {
			pCVar2 = (Camera*)0x0;
			if (-2 < iVar4) {
				if (iVar4 == -1) {
					pCVar2 = this->pActiveCamera;
				}
				else {
					IMPLEMENTATION_GUARD();
					pCVar2 = *(Camera**)(&this->field_0x40 + iVar4 * 0x50);
				}
			}
			pCVar2->ManageEvent(1, 1);
			iVar4 = this->activeIndex;
			pCVar2 = (Camera*)0x0;
			if (-2 < iVar4) {
				if (iVar4 == -1) {
					pCVar2 = this->pActiveCamera;
				}
				else {
					IMPLEMENTATION_GUARD();
					pCVar2 = *(Camera**)(&this->field_0x40 + iVar4 * 0x50);
				}
			}
			pCamera->ManageEvent(2, 1);
			bVar1 = false;
			this->activeIndex = -1;
			this->pActiveCamera = pCamera;
		}
		else {
			if (iVar4 == 7) {
				bVar1 = false;
			}
			else {
				IMPLEMENTATION_GUARD(
				if ((iVar4 != -1) &&
					(iVar3 = &this->field_0x0 + this->activeIndex * 0x50,
						*(float*)(iVar3 + 0x4c) <= *(float*)(iVar3 + 0x48) * (Scene::ptable.g_CameraManager_0045167c)->time_0x4)) {
					this->activeIndex = this->activeIndex + -1;
					iVar4 = this->activeIndex;
					pCVar2 = (Camera*)0x0;
					if (-2 < iVar4) {
						if (iVar4 == -1) {
							pCVar2 = this->pActiveCamera;
						}
						else {
							pCVar2 = *(Camera**)(&this->field_0x40 + iVar4 * 0x50);
						}
					}
					if (pCVar2 == pCamera) {
						return false;
					}
				}
				pCVar2 = (Camera*)0x0;
				if (-2 < iVar4) {
					if (iVar4 == -1) {
						pCVar2 = this->pActiveCamera;
					}
					else {
						pCVar2 = *(Camera**)(&this->field_0x40 + iVar4 * 0x50);
					}
				}
				(*(code*)((pCVar2->objBase).pVTable)->ManageEvent)(pCVar2, 1, switchMode, pCamera);
				iVar4 = 0;
				bVar1 = false;
				do {
					iVar3 = this->activeIndex - iVar4;
					iVar4 = iVar4 + 1;
					if (iVar3 < -1) {
						pCVar2 = (Camera*)0x0;
					}
					else {
						if (iVar3 == -1) {
							pCVar2 = this->pActiveCamera;
						}
						else {
							pCVar2 = *(Camera**)((int)this + iVar3 * 0x50 + 0x40);
						}
					}
					if (pCVar2 == pCamera) {
						bVar1 = true;
					}
				} while (iVar4 < this->activeIndex + 2);
				if (!bVar1) {
					iVar4 = this->activeIndex;
					pCVar2 = (Camera*)0x0;
					if (-2 < iVar4) {
						if (iVar4 == -1) {
							pCVar2 = this->pActiveCamera;
						}
						else {
							pCVar2 = *(Camera**)(&this->field_0x40 + iVar4 * 0x50);
						}
					}
					(*(code*)((pCamera->objBase).pVTable)->ManageEvent)(pCamera, 2, switchMode, pCVar2);
				}
				iVar4 = &this->field_0x50 + this->activeIndex * 0x50;
				*(Camera**)(iVar4 + 0x40) = pCamera;
				*(SWITCH_MODE*)(iVar4 + 0x44) = switchMode;
				*(undefined4*)(iVar4 + 0x4c) = 0;
				if (param_1 == 0.0) {
					*(undefined4*)(iVar4 + 0x48) = 0x3fa00000;
				}
				else {
					*(float*)(iVar4 + 0x48) = 1.0 / param_1;
				}
				bVar1 = true;
				this->activeIndex = this->activeIndex + 1)
			}
		}
	}
	return bVar1;
}

char* s_Mouse_Quake_0042ae48 = "Mouse Quake";
char* s_Mouse_Around_0042ae58 = "Mouse Around";
char* s_Around_Special_0042ae68 = "Around Special";
char* s_Debug_Quake_0042ae78 = "Debug Quake";
char* s_Cinematic_0042ae88 = "Cinematic";
char* s_Frontend_0042ae98 = "Frontend";
char* s_Shadow_Sun_0042aea8 = "Shadow Sun";

void CameraManager::Level_Init(bool bProcessEvents)
{
	float fVar1;
	float fVar2;
	float fVar3;
	Camera* frontend;
	ECameraType EVar4;
	int iVar5;
	int iVar6;
	CameraManager* pCVar7;
	int pEVar8;
	ByteCode newView;

	this->field_0x8 = 0x21000000;
	/* Mouse Quake */
	AddCamera(CT_MouseQuake, &newView, s_Mouse_Quake_0042ae48);
	/* Mouse Around */
	AddCamera(CT_MouseAround, &newView, s_Mouse_Around_0042ae58);
	/* Around Special */
	AddCamera(CT_AroundSpecial, &newView, s_Around_Special_0042ae68);
	/* Debug Quake */
	frontend = AddCamera(CT_MouseQuake, &newView, s_Debug_Quake_0042ae78);
	this->pMouseQuakeCamera_0x4e8 = frontend;
	/* Cinematic */
	AddCamera(CT_Cinematic, &newView, s_Cinematic_0042ae88);
	/* Frontend */
	frontend = AddCamera(CT_Frontend, &newView, s_Frontend_0042ae98);
	this->pFrontendCamera_0x4e4 = (FrontendCameraView*)frontend;
	frontend = this->pFrontendCamera_0x4e4;
	(frontend->lookAt).x = 0.0;
	(frontend->lookAt).y = 0.0;
	(frontend->lookAt).z = 10.0;
	(frontend->lookAt).w = 1.0;
	frontend = this->pFrontendCamera_0x4e4;
	(frontend->transformationMatrix).da = 0.0;
	(frontend->transformationMatrix).db = 0.0;
	(frontend->transformationMatrix).dc = 0.0;
	(frontend->transformationMatrix).dd = 1.0;
	this->pFrontendCamera_0x4e4->field_0x74 = 0.3333333;
	frontend = this->pFrontendCamera_0x4e4;
	_gFrontEndCamera.position.x = (frontend->transformationMatrix).da;
	_gFrontEndCamera.position.y = (frontend->transformationMatrix).db;
	_gFrontEndCamera.position.z = (frontend->transformationMatrix).dc;
	_gFrontEndCamera.position.w = (frontend->transformationMatrix).dd;
	frontend = this->pFrontendCamera_0x4e4;
	_gFrontEndCamera.lookAt.x = (frontend->lookAt).x;
	_gFrontEndCamera.lookAt.y = (frontend->lookAt).y;
	_gFrontEndCamera.lookAt.z = (frontend->lookAt).z;
	_gFrontEndCamera.lookAt.w = (frontend->lookAt).w;
	CameraSet3DPos(&_gFrontEndCamera);
	edFCameraSetSizeRatioFov(0.03f, this->aspectRatio, this->pFrontendCamera_0x4e4->field_0x74 / 1.333333f, &_gFrontEndCamera);
	this->field_0xa00 = Scene::_pinstance->field_0x11c;
	this->field_0xa04 = Scene::_pinstance->count_0x120;
	if (bProcessEvents != false) {
		pEVar8 = 0x0;
		do {
			/* Shadow Sun */
			CCameraShadow* pCameraShadow = (CCameraShadow*)AddCamera(CT_ShadowSun, &newView, s_Shadow_Sun_0042aea8);
			if ((pCameraShadow != (CCameraShadow*)0x0) && (pCameraShadow != (CCameraShadow*)0x0)) {
				EVar4 = pCameraShadow->GetMode();
				if (EVar4 == CT_ShadowSun) {
					this->aCameraShadow[pEVar8] = pCameraShadow;
					pCameraShadow->sceneIndex = pEVar8;
					pCameraShadow->sceneFlags = 1 << (pEVar8 & 0x1fU);
				}
				else {
					this->pShadowSunView_0x4b8 = pCameraShadow;
				}
			}
			pEVar8 = pEVar8 + 1;
		} while (pEVar8 < 10);
	}
	CCameraShadow* pCamera = (CCameraShadow*)this->pInitialView_0x4b4;
	do {
		pCamera->Init();
		pCamera->flags_0xc = pCamera->flags_0xc & 0xffdfffff;
		pCamera = (CCameraShadow*)pCamera->pNextCameraView_0xa4;
	} while (pCamera != (Camera*)0x0);

	for (pCamera = (CCameraShadow*)this->pInitialView_0x4b4;
		(pCamera != (CCameraShadow*)0x0 && ((EVar4 = pCamera->GetMode(), EVar4 != CT_MouseQuake || (pCamera->field_0x8 != -0x9f)))); 
		pCamera = (CCameraShadow*)pCamera->pNextCameraView_0xa4) {
	}

	if (pCamera != (Camera*)0x0) {
		this->cameraStack.SetMainCamera(pCamera);
		this->field_0x710.SetActiveCam(pCamera);
		bool bVar4 = this->field_0x710.SwitchActiveCam(0.0f, pCamera, SWITCH_MODE_B);
		if ((bVar4 == 0) && (pCamera != (Camera*)0x0)) {
			EVar4 = pCamera->GetMode();
			if (EVar4 == CT_ShadowSun) {
				this->pShadowSunView_0x4b8 = (CCameraShadow*)pCamera;
				pCamera->sceneIndex = -1;
				pCamera->sceneFlags = 0;
			}
			else {
				this->pShadowSunView_0x4b8 = pCamera;
			}
		}
		this->field_0x8 = this->field_0x8 & 0xfbffffff;
	}
	this->field_0x4a0 = 0;
	fVar3 = gF32Vertex4Zero.w;
	fVar2 = gF32Vertex4Zero.z;
	fVar1 = gF32Vertex4Zero.y;
	(this->field_0x50).da = gF32Vertex4Zero.x;
	(this->field_0x50).db = fVar1;
	(this->field_0x50).dc = fVar2;
	(this->field_0x50).dd = fVar3;
	fVar3 = gF32Vertex4Zero.w;
	fVar2 = gF32Vertex4Zero.z;
	fVar1 = gF32Vertex4Zero.y;
	(this->shadowCameraLookat).x = gF32Vertex4Zero.x;
	(this->shadowCameraLookat).y = fVar1;
	(this->shadowCameraLookat).z = fVar2;
	(this->shadowCameraLookat).w = fVar3;
	this->field_0xa34 = 0.84f;
	edF32Matrix4SetIdentityHard(&this->matrix_0x10);
	this->field_0x710.ClearActiveCam();
	this->field_0xa78 = 0;
	//iVar5 = 0;
	//pCVar7 = this;
	//do {
	//	*(undefined4*)&pCVar7->field_0xa38 = 0;
	//	*(undefined4*)&pCVar7->field_0xa3c = 0;
	//	iVar5 = iVar5 + 8;
	//	*(undefined4*)&pCVar7->field_0xa40 = 0;
	//	*(undefined4*)&pCVar7->field_0xa44 = 0;
	//	*(undefined4*)&pCVar7->field_0xa48 = 0;
	//	*(undefined4*)&pCVar7->field_0xa4c = 0;
	//	*(undefined4*)&pCVar7->field_0xa50 = 0;
	//	*(undefined4*)&pCVar7->field_0xa54 = 0;
	//	pCVar7 = (CameraManager*)&(pCVar7->matrix_0x10).ba;
	//} while (iVar5 < 0x10);
	iVar5 = 0;
	if (0 < this->loadedCameraCount_0x9f0) {
		do {
			frontend = this->pInitialView_0x4b4;
			iVar6 = 0;
			if (0 < iVar5) {
				do {
					if (frontend->pNextCameraView_0xa4 != (Camera*)0x0) {
						frontend = frontend->pNextCameraView_0xa4;
					}
					iVar6 = iVar6 + 1;
				} while (iVar6 < iVar5);
			}
			EVar4 = frontend->GetMode();
			if ((EVar4 == 0x14) && (iVar6 = 0, pCVar7 = this, (frontend->flags_0xc & 0x200000) != 0)) {
				IMPLEMENTATION_GUARD(
				for (; (*(int*)&pCVar7->field_0xa38 != 0 && (iVar6 < 0x10)); iVar6 = iVar6 + 1) {
					pCVar7 = (CameraManager*)&pCVar7->time_0x4;
				}
				if (iVar6 < 0x10) {
					*(Camera**)(&this->field_0xa38 + iVar6 * 4) = frontend;
					this->field_0xa78 = this->field_0xa78 + 1;
				})
			}
			iVar5 = iVar5 + 1;
		} while (iVar5 < this->loadedCameraCount_0x9f0);
	}
	BuildDisplayMatrix();
	//PTR_CameraManager_00449ee0 = (CameraManager*)0x0;
	//PTR_Singleton_00449ee4 = (AActorBase*)0x0;
	//DAT_00449eec = 0;
	//DAT_00449ff0 = 0;
	return;
}

void CameraManager::Level_Init()
{
	Level_Init(true);
}

void CameraManager::Func_001947e0()
{
	undefined4 uVar1;
	ECameraType EVar2;
	//Actor* pActor;
	long lVar3;
	int iVar4;
	undefined4* puVar5;
	undefined4* puVar6;
	CameraManager** ppCVar7;
	int local_230;
	undefined4 local_22c[67];
	int local_120[68];
	edF32VECTOR4 local_10;

	if ((this->pShadowSunView_0x4b8 != (CCameraShadow*)0x0) &&
		(lVar3 = (long)this->pShadowSunView_0x4b8->GetTarget(), lVar3 != 0)) {
		IMPLEMENTATION_GUARD(); // Check ghidra for function.
	}
}

float CameraManager::Manage_EarthQuake(edF32VECTOR4* param_2)
{
	long lVar1;
	float fVar2;
	float fVar3;
	float t;
	edF32VECTOR4 local_10;

	t = 1.0;
	local_10.x = (this->field_0x480).x;
	local_10.y = (this->field_0x480).y;
	local_10.z = (this->field_0x480).z;
	fVar3 = 0.0;
	if ((this->field_0x4a0 & 1) != 0) {
		IMPLEMENTATION_GUARD();
	}
	return fVar3 * t;
}

void edF32Matrix4TransposeHard(edF32MATRIX4* m0)
{
	edF32MATRIX4 m1 = *m0;
	m0->aa = m1.aa;
	m0->ab = m1.ba;
	m0->ac = m1.ca;
	m0->ad = m1.da;
	m0->ba = m1.ab;
	m0->bb = m1.bb;
	m0->bc = m1.cb;
	m0->bd = m1.db;
	m0->ca = m1.ac;
	m0->cb = m1.bc;
	m0->cc = m1.cc;
	m0->cd = m1.dc;
	m0->da = m1.ad;
	m0->db = m1.bd;
	m0->dc = m1.cd;
	m0->dd = m1.dd;
	return;
}

void CameraManager::ComputeFrustrumPlanes(float param_1, edF32MATRIX4* m0)
{
	float* pfVar1;
	float fVar2;
	float fVar3;
	float fVar4;
	edF32VECTOR4 eStack112;
	edF32VECTOR4 eStack96;
	edF32VECTOR4 eStack80;
	edF32VECTOR4 eStack64;
	edF32VECTOR4 eStack48;
	edF32VECTOR4 eStack32;
	edF32VECTOR4 eStack16;

	edF32Vector4ScaleHard(1.0f / (param_1 / 1.333333f), &eStack16, (edF32VECTOR4*)&m0->ca);
	edF32Vector4ScaleHard(1.0f, &eStack48, (edF32VECTOR4*)&m0->ba);
	edF32Vector4ScaleHard(this->aspectRatio, &eStack32, (edF32VECTOR4*)m0);
	edF32Vector4AddHard(&eStack64, &eStack16, &eStack32);
	edF32Vector4AddHard(&eStack64, &eStack64, &eStack48);
	edF32Vector4AddHard(&eStack80, &eStack16, &eStack32);
	edF32Vector4SubHard(&eStack80, &eStack80, &eStack48);
	edF32Vector4SubHard(&eStack96, &eStack16, &eStack32);
	edF32Vector4AddHard(&eStack96, &eStack96, &eStack48);
	edF32Vector4SubHard(&eStack112, &eStack16, &eStack32);
	edF32Vector4SubHard(&eStack112, &eStack112, &eStack48);
	edF32Vector4CrossProductHard((edF32VECTOR4*)&this->matrix_0x410, &eStack80, &eStack64);
	edF32Vector4NormalizeHard((float*)&this->matrix_0x410, (float*)&this->matrix_0x410);
	edF32Vector4CrossProductHard((edF32VECTOR4*)&(this->matrix_0x410).ba, &eStack96, &eStack112);
	pfVar1 = &(this->matrix_0x410).ba;
	edF32Vector4NormalizeHard(pfVar1, pfVar1);
	edF32Vector4CrossProductHard((edF32VECTOR4*)&(this->matrix_0x410).ca, &eStack64, &eStack96);
	pfVar1 = &(this->matrix_0x410).ca;
	edF32Vector4NormalizeHard(pfVar1, pfVar1);
	edF32Vector4CrossProductHard((edF32VECTOR4*)&(this->matrix_0x410).da, &eStack112, &eStack80);
	pfVar1 = &(this->matrix_0x410).da;
	edF32Vector4NormalizeHard(pfVar1, pfVar1);
	edF32Matrix4TransposeHard(&this->matrix_0x410);
	(this->matrix_0x410).dd = 1.0;
	(this->matrix_0x410).dc = 1.0;
	(this->matrix_0x410).db = 1.0;
	(this->matrix_0x410).da = 1.0;
	fVar4 = m0->cb;
	fVar2 = m0->cc;
	fVar3 = m0->cd;
	(this->field_0x450).x = m0->ca;
	(this->field_0x450).y = fVar4;
	(this->field_0x450).z = fVar2;
	(this->field_0x450).w = fVar3;
	fVar4 = m0->db;
	fVar2 = m0->dc;
	fVar3 = m0->dd;
	(this->field_0x460).x = m0->da;
	(this->field_0x460).y = fVar4;
	(this->field_0x460).z = fVar2;
	(this->field_0x460).w = fVar3;
	return;
}

void CameraManager::BuildDisplayMatrix()
{
	float fVar1;
	float fVar2;
	float fVar3;
	float fVar4;
	int iVar5;
	edF32MATRIX4* peVar6;
	edF32MATRIX4* peVar7;

	if (((this->pShadowSunView_0x4b8->flags_0xc & 0x20000) == 0) ||
		((this->field_0x8 & 0x4000000) != 0)) {
		IMPLEMENTATION_GUARD(
		fVar1 = (this->field_0x50).da;
		fVar2 = (this->field_0x50).db;
		fVar3 = (this->field_0x50).dc;
		fVar4 = (this->field_0x50).dd;
		edF32Matrix4FromEulerSoft(&this->field_0x50, (int)&this->field_0xa08, (char*)&PTR_DAT_0042aeb8);
		(this->field_0x50).da = fVar1;
		(this->field_0x50).db = fVar2;
		(this->field_0x50).dc = fVar3;
		(this->field_0x50).dd = fVar4;)
	}
	else {
		edF32Matrix4CopyHard(&this->field_0x50,
			&this->pShadowSunView_0x4b8->transformationMatrix);
	}
	fVar1 = Manage_EarthQuake((edF32VECTOR4*)&(this->field_0x50).da);
	this->field_0x90 = this->field_0x50;
	if ((this->field_0x8 & 0x18000000) == 0x18000000) {
		this->field_0x90 = this->aCameraShadow[0]->transformationMatrix;
	}
	else {
		if ((this->field_0x8 & 0x10000000) != 0) {
			this->field_0x90 = this->pMouseQuakeCamera_0x4e8->transformationMatrix;
		}
	}
	edF32Matrix4GetInverseOrthoHard(&this->field_0x350, &this->field_0x50);
	edF32Vector4ScaleHard(-1.0f, (edF32VECTOR4*)&this->field_0x390, (edF32VECTOR4*)&this->field_0x50);
	edF32Vector4ScaleHard(-1.0f, (edF32VECTOR4*)&(this->field_0x390).ca, (edF32VECTOR4*)&(this->field_0x50).ca);
	fVar4 = (this->field_0x50).bb;
	fVar2 = (this->field_0x50).bc;
	fVar3 = (this->field_0x50).bd;
	(this->field_0x390).ba = (this->field_0x50).ba;
	(this->field_0x390).bb = fVar4;
	(this->field_0x390).bc = fVar2;
	(this->field_0x390).bd = fVar3;
	fVar4 = (this->field_0x50).db;
	fVar2 = (this->field_0x50).dc;
	fVar3 = (this->field_0x50).dd;
	(this->field_0x390).da = (this->field_0x50).da;
	(this->field_0x390).db = fVar4;
	(this->field_0x390).dc = fVar2;
	(this->field_0x390).dd = fVar3;
	edF32Matrix4GetInverseOrthoHard(&this->field_0x3d0, &this->field_0x390);
	_gDisplayCamera.position.x = (this->field_0x90).da;
	_gDisplayCamera.position.y = (this->field_0x90).db;
	_gDisplayCamera.position.z = (this->field_0x90).dc;
	_gDisplayCamera.position.w = (this->field_0x90).dd;
	_gDisplayCamera.lookAt = this->shadowCameraLookat;
	_gDisplayCamera.rotationZ = 0.0;
	edF32Vector4ScaleHard(-1.0f, (edF32VECTOR4*)&_gDisplayCamera.transformMatrix, (edF32VECTOR4*)&this->field_0x90);
	edF32Vector4ScaleHard(-1.0f, (edF32VECTOR4*)&_gDisplayCamera.transformMatrix.ca, (edF32VECTOR4*)&(this->field_0x90).ca);
	_gDisplayCamera.transformMatrix.ba = (this->field_0x90).ba;
	_gDisplayCamera.transformMatrix.bb = (this->field_0x90).bb;
	_gDisplayCamera.transformMatrix.bc = (this->field_0x90).bc;
	_gDisplayCamera.transformMatrix.bd = (this->field_0x90).bd;
	_gDisplayCamera.transformMatrix.da = (this->field_0x90).da;
	_gDisplayCamera.transformMatrix.db = (this->field_0x90).db;
	_gDisplayCamera.transformMatrix.dc = (this->field_0x90).dc;
	_gDisplayCamera.transformMatrix.dd = (this->field_0x90).dd;
	edF32Matrix4GetInverseOrthoHard(&_gDisplayCamera.worldToCamera, &_gDisplayCamera.transformMatrix);
	_gDisplayCamera.calculatedRotation.x = _gDisplayCamera.transformMatrix.aa;
	_gDisplayCamera.calculatedRotation.y = _gDisplayCamera.transformMatrix.ab;
	_gDisplayCamera.calculatedRotation.z = _gDisplayCamera.transformMatrix.ac;
	_gDisplayCamera.calculatedRotation.w = _gDisplayCamera.transformMatrix.ad;
	if ((this->field_0x8 & 0x18000000) == 0x18000000) {
		fVar1 = fVar1 + this->aCameraShadow[0]->field_0x74;
	}
	else {
		if ((this->field_0x8 & 0x10000000) == 0) {
			fVar1 = fVar1 + this->field_0xa34;
		}
		else {
			fVar1 = fVar1 + this->pMouseQuakeCamera_0x4e8->field_0x74;
		}
	}
	edFCameraSetSizeRatioFov(0.03f, this->aspectRatio, fVar1 / 1.333333f, &_gDisplayCamera);
	ComputeFrustrumPlanes(fVar1, &this->field_0x90);
	return;
}

void CameraManager::Level_Manage()
{
	SWITCH_MODE switchMode;
	CCameraShadow* pCVar1;
	FrontendCameraView* pFVar2;
	bool bVar3;
	Timer* pTVar4;
	ECameraType EVar5;
	int iVar6;
	float* pfVar7;
	Camera* pCVar8;
	uint* puVar9;
	float fVar10;
	float fVar11;
	undefined4 uVar12;
	float fVar13;

	if (this->pShadowSunView_0x4b8 != (CCameraShadow*)0x0) {
		if ((this->field_0x8 & 0x1000000) == 0) {
			pTVar4 = GetTimer();
			this->time_0x4 = pTVar4->cutsceneDeltaTime;
		}
		else {
			pTVar4 = GetTimer();
			this->time_0x4 = pTVar4->lastFrameTime;
		}
		pCVar8 = this->pInitialView_0x4b4;
		do {
			pCVar8->flags_0xc = pCVar8->flags_0xc & 0xff7fffff;
			pCVar8 = pCVar8->pNextCameraView_0xa4;
		} while (pCVar8 != (Camera*)0x0);
		bVar3 = this->cameraStack.Manage();
		if (bVar3 != false) {
			switchMode = (this->cameraStack).switchMode;
			pCVar1 = (CCameraShadow*)(this->cameraStack).pActiveCamera;
			bVar3 = this->field_0x710.SwitchActiveCam((this->cameraStack).field_0x218, (Camera*)pCVar1, switchMode);
			if ((bVar3 == false) && (pCVar1 != (CCameraShadow*)0x0)) {
				EVar5 = pCVar1->GetMode();
				if (EVar5 == CT_ShadowSun) {
					this->pShadowSunView_0x4b8 = pCVar1;
					pCVar1->sceneIndex = -1;
					pCVar1->sceneFlags = 0;
				}
				else {
					this->pShadowSunView_0x4b8 = pCVar1;
				}
			}
			puVar9 = &this->field_0x8;
			if (switchMode == SWITCH_MODE_B) {
				*puVar9 = *puVar9 & 0xfbffffff;
			}
			else {
				*puVar9 = *puVar9 | 0x4000000;
			}
		}
		Func_001947e0();
		pTVar4 = GetTimer();
		if ((pTVar4->timeScale != 0.0) || ((this->pShadowSunView_0x4b8->flags_0xc & 0x40000) != 0)) {
			/* Camera? */
			//pShadowSunView_0x4b8->Manage();
			if ((this->field_0x8 & 0x4000000) == 0) {
				pCVar1 = this->pShadowSunView_0x4b8;
				if (pCVar1 != (CCameraShadow*)0x0) {
					fVar13 = pCVar1->transformationMatrix.db;
					fVar10 = pCVar1->transformationMatrix.dc;
					fVar11 = pCVar1->transformationMatrix.dd;
					(this->field_0x50).da = pCVar1->transformationMatrix.da;
					(this->field_0x50).db = fVar13;
					(this->field_0x50).dc = fVar10;
					(this->field_0x50).dd = fVar11;
					pCVar1 = this->pShadowSunView_0x4b8;
					fVar13 = pCVar1->lookAt.y;
					fVar10 = pCVar1->lookAt.z;
					fVar11 = pCVar1->lookAt.w;
					(this->shadowCameraLookat).x = pCVar1->lookAt.x;
					(this->shadowCameraLookat).y = fVar13;
					(this->shadowCameraLookat).z = fVar10;
					(this->shadowCameraLookat).w = fVar11;
					/* Focus object camera? */
					this->field_0xa30 = pShadowSunView_0x4b8->GetDistance();
					this->field_0xa34 = pShadowSunView_0x4b8->field_0x74;
					this->field_0xa08.alpha = pShadowSunView_0x4b8->GetAngleAlpha();
					this->field_0xa08.beta = pShadowSunView_0x4b8->GetAngleBeta();
					this->field_0xa08.gamma = pShadowSunView_0x4b8->GetAngleGamma();
				}
			}
			else {
				IMPLEMENTATION_GUARD(
				iVar6 = FUN_003feb30((edF32VECTOR4*)&this->field_0x710);
				if ((iVar6 != 0) &&
					(pCVar1 = (CCameraShadow*)(this->field_0x710).pActiveCamera, pCVar1 != (CCameraShadow*)0x0)) {
					EVar5 = (*((pCVar1->cameraExt).camera.objBase.pVTable)->GetMode)((Camera*)pCVar1);
					if (EVar5 == CT_ShadowSun) {
						this->pShadowSunView_0x4b8 = pCVar1;
						pCVar1->sceneIndex = -1;
						pCVar1->sceneFlags = 0;
					}
					else {
						this->pShadowSunView_0x4b8 = pCVar1;
					}
				}
				puVar9 = &this->field_0x8;
				if ((this->field_0x710).field_0x280 == -1) {
					*puVar9 = *puVar9 & 0xfbffffff;
				}
				else {
					*puVar9 = *puVar9 | 0x4000000;
				}
				if ((this->field_0x710).field_0x280 != -1) {
					pfVar7 = (float*)FUN_003fe590((int)&this->field_0x710);
					fVar13 = pfVar7[1];
					fVar10 = pfVar7[2];
					fVar11 = pfVar7[3];
					(this->shadowCameraLookat).x = *pfVar7;
					(this->shadowCameraLookat).y = fVar13;
					(this->shadowCameraLookat).z = fVar10;
					(this->shadowCameraLookat).w = fVar11;
					fVar13 = pfVar7[5];
					fVar10 = pfVar7[6];
					fVar11 = pfVar7[7];
					(this->field_0x80).x = pfVar7[4];
					(this->field_0x80).y = fVar13;
					(this->field_0x80).z = fVar10;
					(this->field_0x80).w = fVar11;
					*(float*)&this->field_0xa08 = pfVar7[8];
					*(float*)&this->field_0xa0c = pfVar7[9];
					*(float*)&this->field_0xa10 = pfVar7[10];
					*(float*)&this->field_0xa30 = pfVar7[0xb];
					this->field_0xa34 = pfVar7[0xc];
				})
			}
			pCVar8 = this->pInitialView_0x4b4;
			do {
				if (((pCVar8->flags_0xc & 0x200000) != 0) && ((pCVar8->flags_0xc & 2) != 0)) {
					IMPLEMENTATION_GUARD(
					Camera::Func_0019a120(pCVar8);)
				}
				pCVar8 = pCVar8->pNextCameraView_0xa4;
			} while (pCVar8 != (Camera*)0x0);
			//IMPLEMENTATION_GUARD(
			//Func_00194e00(this);)
		}
		if ((this->field_0x8 & 0x10000000) != 0) {
			//IMPLEMENTATION_GUARD(
			//this->pMouseQuakeCamera_0x4e8->Manage();
			//)
		}
		BuildDisplayMatrix();
		if (this->pFrontendCamera_0x4e4 != (FrontendCameraView*)0x0) {
			this->pFrontendCamera_0x4e4->Manage();
			pFVar2 = this->pFrontendCamera_0x4e4;
			_gFrontEndCamera.position.x = pFVar2->transformationMatrix.da;
			_gFrontEndCamera.position.y = pFVar2->transformationMatrix.db;
			_gFrontEndCamera.position.z = pFVar2->transformationMatrix.dc;
			_gFrontEndCamera.position.w = pFVar2->transformationMatrix.dd;
			pFVar2 = this->pFrontendCamera_0x4e4;
			_gFrontEndCamera.lookAt = pFVar2->lookAt;
			CameraSet3DPos(&_gFrontEndCamera);
			edFCameraSetSizeRatioFov (0.03f, this->aspectRatio, this->pFrontendCamera_0x4e4->field_0x74 / 1.333333f, &_gFrontEndCamera);
		}
		ed3DSceneComputeCameraToScreenMatrix(Scene::_scene_handleA, &this->matrix_0x10);
	}
	return;
}

void CameraManager::LevelLoading_Begin()
{
	Level_Init(false);
	return;
}

bool CameraManager::LevelLoading_Manage()
{
	Level_Manage();
	return false;
}

void edFCameraSetSizeRatioFov(float halfFOV, float aspectRatio, float sizeRatio, edFCamera* pCamera)
{
	pCamera->horizontalHalfFOV = halfFOV * aspectRatio;
	pCamera->halfFOV = halfFOV;
	pCamera->verticalHalfFOV = halfFOV / sizeRatio;
	return;
}

void CameraManager::Level_ClearInternalData()
{
	CameraManager* pCVar1;
	edFCamera* m0;
	int iVar2;

	_gDisplayCamera.position.x = 0.0f;
	_gDisplayCamera.position.y = 0.0f;
	_gDisplayCamera.position.z = 0.0f;
	_gDisplayCamera.position.w = 1.0f;
	_gDisplayCamera.lookAt.x = 0.0f;
	_gDisplayCamera.lookAt.y = 0.0f;
	_gDisplayCamera.lookAt.z = 1.0f;
	_gDisplayCamera.lookAt.w = 1.0f;
	_gDisplayCamera.rotationZ = 0.0f;
	edFCameraSetSizeRatioFov(0.0001875f, 1.333333f, 0.25f, &_gDisplayCamera);
	CameraSet3DPos(&_gDisplayCamera);
	_gFrontEndCamera.position.x = 0.0f;
	_gFrontEndCamera.position.y = 0.0f;
	_gFrontEndCamera.position.z = 0.0f;
	_gFrontEndCamera.position.w = 1.0f;
	_gFrontEndCamera.lookAt.x = 0.0f;
	_gFrontEndCamera.lookAt.y = 0.0f;
	_gFrontEndCamera.lookAt.z = 1.0f;
	_gFrontEndCamera.lookAt.w = 1.0f;
	_gFrontEndCamera.rotationZ = 0.0f;
	edFCameraSetSizeRatioFov(0.0001875f, 1.333333f, 0.25f, &_gFrontEndCamera);
	CameraSet3DPos(&_gFrontEndCamera);
	iVar2 = 0;
	m0 = gSceneCameras;
	do {
		(m0->position).x = 0.0f;
		(m0->position).y = 0.0f;
		(m0->position).z = 0.0f;
		(m0->position).w = 1.0f;
		(m0->lookAt).x = 0.0f;
		(m0->lookAt).y = 0.0f;
		(m0->lookAt).z = 1.0f;
		(m0->lookAt).w = 1.0f;
		edFCameraSetSizeRatioFov(0.0001875f, 1.333333f, 0.25f, m0);
		CameraSet3DPos(m0);
		pCVar1->aCameraShadow[iVar2] = (CCameraShadow*)0x0;
		iVar2 = iVar2 + 1;
		m0 = m0 + 1;
	} while (iVar2 < 10);
	this->cameraStack.Reset();
	this->field_0x710.ClearActiveCam();
	this->loadedCameraCount_0x9f0 = 0;
	this->count_0x9f4 = 0;
	this->field_0x9f8 = 0;
	this->count_0x9fc = 0;
	this->pInitialView_0x4b4 = (Camera*)0x0;
	this->pShadowSunView_0x4b8 = (CCameraShadow*)0x0;
	this->pFrontendCamera_0x4e4 = (FrontendCameraView*)0x0;
	this->pMouseQuakeCamera_0x4e8 = (Camera*)0x0;
	this->field_0xa78 = 0;
	this->time_0x4 = 0.0;
	return;
}

Camera* CameraManager::AddCamera(ECameraType type, ByteCode* pMemoryStream, char* objName)
{
	Camera* pCVar1;
	Camera* pCVar2;
	MainCamera* pMVar3;
	//MouseCameraView* pMVar4;
	//VSilverBoomy* pVVar5;
	CameraCinematic* pCinematicCameraView;
	//VExtendedCameraView_110* pVVar6;
	void* pvVar7;
	//FrontendCameraView* pFVar8;
	//DeathCameraViewLocationObject* pDeathCameraView;
	Camera* pCVar9;
	Camera* newObjectPtr;

	newObjectPtr = (Camera*)0x0;
	switch (type) {
	case CT_MouseQuake:
		newObjectPtr = new CCameraMouseQuake();
		break;
	case CT_MouseAround:
		newObjectPtr = new CCameraMouseAroundPerso();
		break;
	case CT_AroundSpecial:
		newObjectPtr = new CCameraMouseAroundPersoSpecial();
		break;
	case 3:
	case 4:
	case CT_KyaWindWall:
	case 6:
	case CT_KyaJamgut:
	case CT_MainCamera:
		newObjectPtr = new MainCamera(type, pMemoryStream);
		break;
	case CT_SilverBoomy:
		IMPLEMENTATION_GUARD(
		pVVar5 = (VSilverBoomy*)operator.new(0x180);
		newObjectPtr = (long)(int)pVVar5;
		if (newObjectPtr != 0) {
			newObjectPtr = VSilverBoomy::Constructor_0018adb0(pVVar5);
		})
		break;
	case CT_IntView:
		IMPLEMENTATION_GUARD(
		pvVar7 = operator.new(0x110);
		newObjectPtr = (long)(int)pvVar7;
		if (newObjectPtr != 0) {
			newObjectPtr = IntViewConstructor ? (newObjectPtr);
		})
		break;
	case CT_ShadowSun:
		newObjectPtr = new CCameraShadow(pMemoryStream);
		break;
	case 0xd:
		// What camera is this?
		IMPLEMENTATION_GUARD();
		newObjectPtr = new CCameraShadow(pMemoryStream);
		break;
	case 0xe:
		IMPLEMENTATION_GUARD(
		pVVar6 = (VExtendedCameraView_110*)operator.new(0x110);
		newObjectPtr = (long)(int)pVVar6;
		if (newObjectPtr != 0) {
			newObjectPtr = VExtendedCameraView_110::Deserialize_0018c630(pVVar6, pMemoryStream);
		})
		break;
	case 0x11:
		IMPLEMENTATION_GUARD(
		pvVar7 = operator.new(0x100);
		newObjectPtr = (long)(int)pvVar7;
		if (newObjectPtr != 0) {
			newObjectPtr = FUN_0018dbe0(newObjectPtr, pMemoryStream, objName);
		})
		break;
	case 0x12:
		IMPLEMENTATION_GUARD(
		pvVar7 = operator.new(0x110);
		newObjectPtr = (long)(int)pvVar7;
		if (newObjectPtr != 0) {
			newObjectPtr = FUN_0018d490(newObjectPtr, (int)pMemoryStream);
		})
		break;
	case 0x13:
		IMPLEMENTATION_GUARD(
		pvVar7 = operator.new(0x120);
		newObjectPtr = (long)(int)pvVar7;
		if (newObjectPtr != 0) {
			newObjectPtr = FUN_0018cb70(newObjectPtr, (int)pMemoryStream);
		})
		break;
	case 0x14:
		IMPLEMENTATION_GUARD(
		pvVar7 = operator.new(0x110);
		newObjectPtr = (long)(int)pvVar7;
		if (newObjectPtr != 0) {
			newObjectPtr = View::DeserializeView_0018c420(newObjectPtr, pMemoryStream);
		})
		break;
	case CT_Cinematic:
		newObjectPtr = new CameraCinematic();
		break;
	case 0x17:
		IMPLEMENTATION_GUARD(
		pFVar8 = (FrontendCameraView*)operator.new(0xc0);
		newObjectPtr = (long)(int)pFVar8;
		if (newObjectPtr != 0) {
			FrontendCameraViewConstructor(pFVar8);
			(pFVar8->viewBase).pVTable = (CameraFunctionData*)&DAT_00440940;
		})
		break;
	case CT_Frontend:
		newObjectPtr = new FrontendCameraView();
		break;
	case CT_Death:
		IMPLEMENTATION_GUARD(
		pDeathCameraView = (DeathCameraViewLocationObject*)operator.new(0x120);
		newObjectPtr = (long)(int)pDeathCameraView;
		if (newObjectPtr != 0) {
			newObjectPtr = DeathViewConstructor(pDeathCameraView);
		})
	}
	pCVar9 = newObjectPtr;
	if (newObjectPtr != (Camera*)0x0) {
		pCVar1 = this->pInitialView_0x4b4;
		if (pCVar1 == (Camera*)0x0) {
			this->pInitialView_0x4b4 = pCVar9;
			g_CameraViewManager_00448e9c = this;
		}
		else {
			pCVar2 = pCVar1->pNextCameraView_0xa4;
			while (pCVar2 != (Camera*)0x0) {
				pCVar1 = pCVar1->pNextCameraView_0xa4;
				pCVar2 = pCVar1->pNextCameraView_0xa4;
			}
			pCVar1->pNextCameraView_0xa4 = pCVar9;
		}
		this->loadedCameraCount_0x9f0 = this->loadedCameraCount_0x9f0 + 1;
	}
	return pCVar9;
}

Camera* CameraManager::GetDefGameCamera(ECameraType type)
{
	Camera* pCVar1;
	ECameraType EVar2;

	for (pCVar1 = this->pInitialView_0x4b4;
		(pCVar1 != (Camera*)0x0 &&
			((EVar2 = pCVar1->GetMode(), type != EVar2 || (pCVar1->field_0x8 != -100))));
		pCVar1 = pCVar1->pNextCameraView_0xa4) {
	}
	return pCVar1;
}

Camera::Camera(ByteCode* pMemoryStream)
{
	CheckpointManagerSubObjB* pCVar1;
	int* piVar2;
	int iVar3;
	uint uVar4;
	EventChunk_24* pEVar5;
	float fVar6;
	edF32VECTOR4 local_10;

	//this->pVTable = (CameraFunctionData*)&ParticleDataBase::VTable_0043a3b0;
	this->field_0x4 = 0xffffffff;
	//this->pVTable = &VTable_0043d130;
	this->specCondition.field_0x0 = (int*)0x0;
	this->pNextCameraView_0xa4 = (Camera*)0x0;
	*(undefined*)&this->field_0xa0 = 0;
	Func_0x34(0);
	fVar6 = pMemoryStream->GetF32();
	this->field_0x74 = fVar6;
	local_10.x = pMemoryStream->GetF32();
	local_10.y = pMemoryStream->GetF32();
	local_10.z = pMemoryStream->GetF32();
	local_10.w = 1.0;
	(this->transformationMatrix).da = local_10.x;
	(this->transformationMatrix).db = local_10.y;
	(this->transformationMatrix).dc = local_10.z;
	(this->transformationMatrix).dd = 1.0f;
	local_10.x = pMemoryStream->GetF32();
	local_10.y = pMemoryStream->GetF32();
	local_10.z = pMemoryStream->GetF32();
	local_10.w = 1.0;
	(this->lookAt).x = local_10.x;
	(this->lookAt).y = local_10.y;
	(this->lookAt).z = local_10.z;
	(this->lookAt).w = 1.0f;
	edF32Vector4SubHard(&local_10, &this->lookAt, (edF32VECTOR4*)&(this->transformationMatrix).da);
	edF32Vector4NormalizeHard((TO_SCE_VECTOR)&local_10, (TO_SCE_VECTOR)&local_10);
	(this->transformationMatrix).ca = local_10.x;
	(this->transformationMatrix).cb = local_10.y;
	(this->transformationMatrix).cc = local_10.z;
	(this->transformationMatrix).cd = local_10.w;
	local_10.x = pMemoryStream->GetF32();
	local_10.y = pMemoryStream->GetF32();
	local_10.z = pMemoryStream->GetF32();
	local_10.w = 0.0;
	(this->transformationMatrix).ba = local_10.x;
	(this->transformationMatrix).bb = local_10.y;
	(this->transformationMatrix).bc = local_10.z;
	(this->transformationMatrix).bd = 0.0;
	edF32Vector4CrossProductHard(&local_10, (edF32VECTOR4*)&(this->transformationMatrix).ba, (edF32VECTOR4*)&(this->transformationMatrix).ca);
	local_10.w = 0.0;
	(this->transformationMatrix).aa = local_10.x;
	(this->transformationMatrix).ab = local_10.y;
	(this->transformationMatrix).ac = local_10.z;
	(this->transformationMatrix).ad = 0.0f;
	iVar3 = pMemoryStream->GetS32();
	this->field_0x4 = iVar3;
	iVar3 = pMemoryStream->GetS32();
	this->field_0x8 = iVar3;
	uVar4 = pMemoryStream->GetU32();
	this->flags_0xc = uVar4 | 0x200000;
	fVar6 = pMemoryStream->GetF32();
	this->field_0x50.x = fVar6;
	fVar6 = pMemoryStream->GetF32();
	this->field_0x50.y = fVar6;
	fVar6 = pMemoryStream->GetF32();
	this->field_0x50.z = fVar6;
	this->field_0x50.w = 0.0f;
	if ((this->flags_0xc & 2) == 0) {
		this->field_0x8c = 0.0f;
		this->switchMode = SWITCH_MODE_F;
		this->field_0x98 = 0.8f;
		this->field_0x94 = 0;
		this->field_0x9c = 0.8f;
	}
	else {
		pEVar5 = (EventChunk_24*)pMemoryStream->GetS32();
		//this->field_0x80 = pEVar5;
		//pCVar1 = (CheckpointManagerSubObjB*)pMemoryStream->currentSeekPos;
		//pMemoryStream->currentSeekPos = (char*)&pCVar1->field_0x4;
		//if (pCVar1->pActor_0x0 != (AActorBase*)0x0) {
		//	pMemoryStream->currentSeekPos = pMemoryStream->currentSeekPos + (int)pCVar1->pActor_0x0 * 4;
		//}
		//this->field_0x84 = pCVar1;
		//piVar2 = (int*)pMemoryStream->currentSeekPos;
		//pMemoryStream->currentSeekPos = (char*)(piVar2 + 1);
		//if (*piVar2 != 0) {
		//	pMemoryStream->currentSeekPos = pMemoryStream->currentSeekPos + *piVar2 * 4;
		//}
		//this->field_0x88 = (float)piVar2;
		fVar6 = pMemoryStream->GetF32();
		this->field_0x8c = fVar6;
		iVar3 = pMemoryStream->GetS32();
		this->switchMode = (SWITCH_MODE)iVar3;
		fVar6 = pMemoryStream->GetF32();
		this->field_0x98 = fVar6;
		iVar3 = pMemoryStream->GetS32();
		this->field_0x94 = iVar3;
		fVar6 = pMemoryStream->GetF32();
		this->field_0x9c = fVar6;
		if (this->field_0x80 == (EventChunk_24*)0xffffffff) {
			this->flags_0xc = this->flags_0xc & 0xfffffffd;
		}
	}
	piVar2 = (int*)pMemoryStream->currentSeekPos;
	pMemoryStream->currentSeekPos = (char*)(piVar2 + 1);
	if (*piVar2 != 0) {
		pMemoryStream->currentSeekPos = pMemoryStream->currentSeekPos + *piVar2 * 0x10;
	}
	(this->specCondition).field_0x0 = (int*)piVar2;
	return;
}

Camera::Camera()
{
	this->field_0x4 = 0xffffffff;
	(this->specCondition).field_0x0 = (int*)0x0;
	this->pNextCameraView_0xa4 = (Camera*)0x0;
	edF32Matrix4SetIdentityHard(&this->transformationMatrix);
	this->flags_0xc = 0x200000;
	this->field_0x50.x = 0.0f;
	this->field_0x50.y = 0.0f;
	this->field_0x50.z = 0.0f;
	this->field_0x50.w = 0.0f;
	*(undefined*)&this->field_0xa0 = 0;
	this->field_0x4 = 0xffffffff;
	this->field_0x8 = -1;
	this->field_0x8c = 0.0f;
	Func_0x34(0x0);
	this->switchMode = SWITCH_MODE_A;
	this->field_0x98 = 0.8f;
	this->field_0x94 = 0;
	this->field_0x9c = 0.8f;
	return;
}

void Camera::Init()
{
	EventManager* pEVar1;
	//EventChunk_24* pEVar2;
	//CheckpointManagerSubObjB* pCVar3;
	//Actor* pAVar4;
	edF32VECTOR3 local_10;

	pEVar1 = Scene::ptable.g_EventManager_006f5080;
	edF32Matrix4ToEulerSoft(&this->transformationMatrix, &local_10, "ZXY");
	SetAngleAlpha(local_10.x);
	SetAngleBeta(local_10.y);
	SetAngleGamma(local_10.z);
	if ((this->flags_0xc & 2) != 0) {
		IMPLEMENTATION_GUARD(
		this->field_0x7c = 0;
		pEVar2 = (EventChunk_24*)0x0;
		if (this->field_0x80 != (EventChunk_24*)0xffffffff) {
			pEVar2 = edEventGetChunkZone(pEVar1->activeEventChunkID_0x8, (int)this->field_0x80);
		}
		this->field_0x80 = pEVar2;
		pCVar3 = this->field_0x84;
		for (pAVar4 = pCVar3->pActor_0x0; pAVar4 != (Actor*)0x0; pAVar4 = (Actor*)((int)&pAVar4[-1].data.field_0x158 + 3))
		{
			pCVar3 = (CheckpointManagerSubObjB*)&pCVar3->field_0x4;
			LinkCheckpointActor_00119a00(pCVar3);
		}
		pCVar3 = (CheckpointManagerSubObjB*)this->field_0x88;
		for (pAVar4 = pCVar3->pActor_0x0; pAVar4 != (Actor*)0x0; pAVar4 = (Actor*)((int)&pAVar4[-1].data.field_0x158 + 3))
		{
			pCVar3 = (CheckpointManagerSubObjB*)&pCVar3->field_0x4;
			LinkCheckpointActor_00119a00(pCVar3);
		})
	}
	return;
}

bool Camera::Manage()
{
	Actor* pActorA;
	bool returnVal;
	Actor* pAVar1;
	void* actorAddrA;

	if ((this->flags_0xc & 0x800000) == 0) {
		this->flags_0xc = this->flags_0xc | 0x800000;
		actorAddrA = this->GetTarget();
		if (actorAddrA != 0) {
			IMPLEMENTATION_GUARD(
			pActorA = (Actor*)(*(code*)((this->objBase).pVTable)->GetTarget)(this);
			pAVar1 = (pActorA->actorBase).data.actorFieldG;
			if (pAVar1 == (Actor*)0x0) {
				pAVar1 = (Actor*)(*(code*)((this->objBase).pVTable)->GetTarget)(this);
				pAVar1 = GetActorMatrixSubObjFunc_001010f0(pAVar1);
			}
			(*((this->objBase).pVTable)->SetTarget)(this, pAVar1);)
		}
		returnVal = false;
	}
	else {
		returnVal = true;
	}
	return returnVal;
}

void Camera::ManageEvent(int param_2, int param_3)
{
	int* piVar1;
	int iVar2;
	uint* puVar3;
	int iVar4;
	int iVar5;
	float fVar6;

	if (param_2 == 1) {
		fVar6 = this->field_0x8c;
		if ((this->flags_0xc & 2) != 0) {
			iVar5 = 0;
			iVar4 = 0;
			IMPLEMENTATION_GUARD(
			while (true) {
				piVar1 = (int*)this->field_0x88;
				iVar2 = 0;
				if (piVar1 != (int*)0x0) {
					iVar2 = *piVar1;
				}
				if (iVar2 <= iVar5) break;
				edEventComputeZoneAgainstVertex
				((Actor*)this->field_0x84->field_0x4, *(Actor**)((int)piVar1 + iVar4 + 4), 0x2a,
					(ActorCompareStruct*)(uint)(0.0 < fVar6));
				iVar4 = iVar4 + 4;
				iVar5 = iVar5 + 1;
			})
		}
	}
	else {
		if (param_2 == 2) {
			fVar6 = this->field_0x8c;
			if ((this->flags_0xc & 2) != 0) {
				IMPLEMENTATION_GUARD(
				iVar5 = 0;
				iVar4 = 0;
				while (true) {
					piVar1 = (int*)this->field_0x88;
					iVar2 = 0;
					if (piVar1 != (int*)0x0) {
						iVar2 = *piVar1;
					}
					if (iVar2 <= iVar5) break;
					edEventComputeZoneAgainstVertex
					((Actor*)this->field_0x84->field_0x4, *(Actor**)((int)piVar1 + iVar4 + 4), 0x29,
						(ActorCompareStruct*)(uint)(0.0 < fVar6));
					iVar4 = iVar4 + 4;
					iVar5 = iVar5 + 1;
				})
			}
		}
		else {
			if (param_2 == 0) {
				puVar3 = &this->flags_0xc;
				if (param_3 == 0) {
					*puVar3 = *puVar3 & 0xffefffff;
				}
				else {
					*puVar3 = *puVar3 | 0x100000;
				}
			}
		}
	}
	return;
}

float edF32Vector4GetDistHard(edF32VECTOR4* v0)
{
	return sqrtf(v0->x * v0->x + v0->y * v0->y + v0->z * v0->z) + 0.0;
}

float Camera::GetDistance()
{
	float fVar1;
	edF32VECTOR4 eStack16;

	edF32Vector4SubHard(&eStack16, &this->lookAt, (edF32VECTOR4*)&(this->transformationMatrix).da);
	fVar1 = edF32Vector4GetDistHard(&eStack16);
	return fVar1;
}

float GetAngleXFromVector(edF32VECTOR4* v0)
{
	float pitch;
	float fVar1;

	pitch = atan2f(-v0->y, sqrt(v0->z * v0->z + v0->x * v0->x));
	/* Second Param: 6.283185 (2 Pi) */
	fVar1 = fmodf(pitch, 6.283185);
	return fVar1;
}

float GetAngleYFromVector(edF32VECTOR4* v0)
{
	float fVar1;

	fVar1 = atan2f(v0->x, v0->z);
	fVar1 = fmodf(fVar1, 6.283185);
	return fVar1;
}

float Camera::GetAngleAlpha()
{
	return GetAngleXFromVector((edF32VECTOR4*)&(this->transformationMatrix).ca);
}

float Camera::GetAngleBeta()
{
	return GetAngleYFromVector((edF32VECTOR4*)&(this->transformationMatrix).ca);
}

float Camera::GetAngleGamma()
{
	return 0.0f;
}

void CCameraExt::Init()
{
	EventManager* pEVar1;
	//EventChunk_24* pEVar2;
	//CheckpointManagerSubObjB* pCVar3;
	//Actor* pAVar4;
	edF32VECTOR3 local_10;

	pEVar1 = Scene::ptable.g_EventManager_006f5080;
	edF32Matrix4ToEulerSoft(&this->transformationMatrix, &local_10, "ZXY");
	SetAngleAlpha(local_10.x);
	SetAngleBeta(local_10.y);
	SetAngleGamma(local_10.z);
	if ((this->flags_0xc & 2) != 0) {
		IMPLEMENTATION_GUARD(
			this->field_0x7c = 0;
		pEVar2 = (EventChunk_24*)0x0;
		if (this->field_0x80 != (EventChunk_24*)0xffffffff) {
			pEVar2 = edEventGetChunkZone(pEVar1->activeEventChunkID_0x8, (int)this->field_0x80);
		}
		this->field_0x80 = pEVar2;
		pCVar3 = this->field_0x84;
		for (pAVar4 = pCVar3->pActor_0x0; pAVar4 != (Actor*)0x0; pAVar4 = (Actor*)((int)&pAVar4[-1].data.field_0x158 + 3))
		{
			pCVar3 = (CheckpointManagerSubObjB*)&pCVar3->field_0x4;
			LinkCheckpointActor_00119a00(pCVar3);
		}
		pCVar3 = (CheckpointManagerSubObjB*)this->field_0x88;
		for (pAVar4 = pCVar3->pActor_0x0; pAVar4 != (Actor*)0x0; pAVar4 = (Actor*)((int)&pAVar4[-1].data.field_0x158 + 3))
		{
			pCVar3 = (CheckpointManagerSubObjB*)&pCVar3->field_0x4;
			LinkCheckpointActor_00119a00(pCVar3);
		})
	}
	return;
}

void* CCameraExt::GetTarget()
{
	return this->pActorView;
}

float CCameraExt::GetDistance()
{
	return this->distance;
}

void CCameraExt::SetDistance(float distance)
{
	this->distance = distance;
	return;
}

float CCameraExt::GetAngleAlpha()
{
	return this->angles.alpha;
}

float CCameraExt::GetAngleBeta()
{
	return this->angles.beta;
}

float CCameraExt::GetAngleGamma()
{
	return this->angles.gamma;
}

void CCameraExt::SetAngleAlpha(float angle)
{
	(this->angles).alpha = angle;
	return;
}

void CCameraExt::SetAngleBeta(float angle)
{
	(this->angles).beta = angle;
	return;
}

void CCameraExt::SetAngleGamma(float angle)
{
	(this->angles).gamma = angle;
	return;
}

ECameraType Camera::GetMode()
{
	return CT_Frontend;
}

void Camera::Func_0x34(undefined4 param_2)
{
	this->field_0x70 = param_2;
}

edF32VECTOR4 Vector3_00472250 = { 0 };

CameraView::CameraView(struct ByteCode* pMemoryStream)
	: Camera(pMemoryStream)
{
	float fVar1;
	float fVar2;

	this->field_0xb0 = 0;
	this->field_0xc0 = 0;
	fVar2 = Vector3_00472250.z;
	fVar1 = Vector3_00472250.y;
	this->field_0xb4 = Vector3_00472250.x;
	this->field_0xb8 = fVar1;
	this->field_0xbc = fVar2;
	return;
}

MainCamera::MainCamera(ECameraType type, ByteCode* pMemoryStream)
	: CameraView(pMemoryStream)
{
	ECameraType EVar1;
	long lVar2;
	astruct_8* puVar3;

	this->field_0x340 = 0;
	subObj_8.Init(pMemoryStream);
	lVar2 = subObj_12.Init(pMemoryStream);
	puVar3 = &this->subObj_8;
	if (lVar2 == 0) {
		puVar3->field_0x0 = puVar3->field_0x0 & 0xfffdffff;
	}
	else {
		puVar3->field_0x0 = puVar3->field_0x0 | 0x20000;
	}
	SetMode(type);
	EVar1 = GetMode();
	if (((EVar1 == 3) && ((this->subObj_8).field_0x2c < 1.22173f)) && (1.22173f < (this->subObj_8).field_0x30)) {
		(this->subObj_8).field_0x30 = 1.22173f;
	}
	flags_0xc = flags_0xc | 0x20000;
	return;
}

void MainCamera::SetMode(ECameraType type)
{
	uint uVar1;
	bool bVar2;
	bool bVar3;
	bool bVar4;
	bool bVar5;
	bool bVar6;
	bool bVar7;
	bool bVar8;

	if (type == CT_MainCamera) {
		uVar1 = flags_0xc;
		bVar2 = (uVar1 & 0x40) != 0;
		bVar3 = (uVar1 & 4) != 0;
		bVar4 = (uVar1 & 8) != 0;
		bVar5 = (uVar1 & 0x10) != 0;
		bVar6 = (uVar1 & 0x20) != 0;
		bVar7 = (uVar1 & 0x100) != 0;
		bVar8 = (uVar1 & 0x200) != 0;
		if ((byte)(bVar3 + bVar2 + bVar4 + bVar5 + bVar6 + bVar7 + bVar8) < 2) {
			if (bVar3) {
				type = (ECameraType)3;
			}
			else {
				if (bVar4) {
					type = CT_KyaJamgut;
				}
				else {
					if (bVar5) {
						type = (ECameraType)8;
					}
					else {
						if (bVar6) {
							type = (ECameraType)9;
						}
						else {
							if (bVar2) {
								type = (ECameraType)4;
							}
							else {
								if (bVar7) {
									type = CT_KyaWindWall;
								}
								else {
									type = (ECameraType)6;
									if (!bVar8) {
										type = (ECameraType)3;
									}
								}
							}
						}
					}
				}
			}
		}
		else {
			type = (ECameraType)3;
		}
	}
	this->cameraType_0xd4 = type;
	return;
}

ECameraType MainCamera::GetMode()
{
	return this->cameraType_0xd4;
}

void astruct_8::Init(struct ByteCode* pMemoryStream)
{
	uint uVar1;
	int iVar2;
	float fVar3;

	uVar1 = pMemoryStream->GetU32();
	this->field_0x0 = uVar1;
	uVar1 = pMemoryStream->GetS32();
	this->field_0x4 = uVar1;
	uVar1 = pMemoryStream->GetS32();
	this->field_0x8 = uVar1;
	fVar3 = pMemoryStream->GetF32();
	this->field_0x18 = fVar3;
	fVar3 = this->field_0x18 * 0.01745329 * 0.5;
	this->field_0x18 =
		sinf(fVar3) /
		cosf(fVar3);
	iVar2 = pMemoryStream->GetS32();
	if (iVar2 == 0) {
		this->field_0x20 = 2.0;
		this->field_0xc = 0.25;
		this->field_0x10 = 0.25;
		this->field_0x14 = 0.0;
		this->field_0x1c = 0.0;
	}
	else {
		if (iVar2 == 1) {
			fVar3 = pMemoryStream->GetF32();
			this->field_0x20 = fVar3;
			fVar3 = pMemoryStream->GetF32();
			this->field_0xc = fVar3;
			fVar3 = pMemoryStream->GetF32();
			this->field_0x10 = fVar3;
			fVar3 = pMemoryStream->GetF32();
			this->field_0x14 = fVar3;
			fVar3 = pMemoryStream->GetF32();
			this->field_0x1c = fVar3 * 0.01745329;
		}
	}
	fVar3 = pMemoryStream->GetF32();
	this->field_0x28 = fVar3 * 0.01745329;
	fVar3 = pMemoryStream->GetF32();
	this->field_0x2c = fVar3 * 0.01745329;
	fVar3 = pMemoryStream->GetF32();
	this->field_0x30 = fVar3 * 0.01745329;
	this->field_0x34 = 0.8726646;
	this->field_0x38 = 2.443461;
	this->field_0x3c = 2.443461;
	fVar3 = pMemoryStream->GetF32();
	this->field_0x40 = fVar3 * 0.01745329;
	fVar3 = pMemoryStream->GetF32();
	this->field_0x44 = fVar3 * 0.01745329;
	fVar3 = pMemoryStream->GetF32();
	this->field_0x48 = fVar3 * 0.01745329;
	this->field_0x4c = 1.047198;
	this->field_0x50 = 3.490659;
	this->field_0x54 = 3.490659;
	fVar3 = pMemoryStream->GetF32();
	this->field_0x58 = fVar3;
	fVar3 = pMemoryStream->GetF32();
	this->field_0x5c = fVar3;
	fVar3 = pMemoryStream->GetF32();
	this->field_0x60 = fVar3;
	this->field_0x64 = 4.0;
	this->field_0x68 = 8.0;
	this->field_0x6c = 10.0;
	fVar3 = this->field_0x5c;
	if (fVar3 < 2.5) {
		this->field_0x24 = 2.5;
	}
	else {
		this->field_0x24 = fVar3 + (this->field_0x58 - fVar3) * 0.5;
	}
	return;
}

bool astruct_12::Init(struct ByteCode* pMemoryStream)
{
	uint uVar1;
	float fVar2;

	uVar1 = pMemoryStream->GetU32();
	if (uVar1 == 1) {
		fVar2 = pMemoryStream->GetF32();
		this->field_0x0 = fVar2;
		pMemoryStream->GetF32();
		this->field_0x4 = 0.0;
		fVar2 = pMemoryStream->GetF32();
		this->field_0x8 = fVar2;
		pMemoryStream->GetF32();
		pMemoryStream->GetF32();
		this->field_0xc = 0.1;
		fVar2 = pMemoryStream->GetF32();
		this->field_0x10 = fVar2 * 0.01745329;
		pMemoryStream->GetF32();
		this->field_0x14 = 0.0;
		fVar2 = pMemoryStream->GetF32();
		this->field_0x18 = fVar2 * 0.01745329;
		fVar2 = pMemoryStream->GetF32();
		this->field_0x1c = fVar2;
		fVar2 = pMemoryStream->GetF32();
		this->field_0x20 = fVar2;
		pMemoryStream->GetF32();
		pMemoryStream->GetF32();
	}
	else {
		this->field_0x0 = 0.5;
		this->field_0x4 = 0.0;
		this->field_0x8 = 4.0;
		this->field_0xc = 0.1;
		this->field_0x10 = 0.0;
		this->field_0x14 = 0.0;
		this->field_0x18 = 3.141593;
		this->field_0x1c = 0.0;
		this->field_0x20 = 8.0;
	}
	return uVar1 == 1;
}

edF32VECTOR4 gF32Vertex4Zero = { 0 };
edF32VECTOR4 gF32Vector4Zero = { 0 };

CameraCinematic::CameraCinematic()
{
	(this->field_0x74) = 0.6f;
	(this->transformationMatrix.da) = gF32Vertex4Zero.x;
	(this->transformationMatrix.db) = gF32Vertex4Zero.y;
	(this->transformationMatrix.dc) = gF32Vertex4Zero.z;
	(this->transformationMatrix.dd) = gF32Vertex4Zero.w;
	(this->lookAt.x) = gF32Vertex4Zero.x;
	(this->lookAt.y) = gF32Vertex4Zero.y;
	(this->lookAt.z) = gF32Vertex4Zero.z;
	(this->lookAt.w) = gF32Vertex4Zero.w;
	ResetEvent();
	(this->field_0x8) = -100;
	(this->flags_0xc) = (this->flags_0xc) | 0x37c;
	(this->switchMode) = SWITCH_MODE_B;
	(this->field_0x98) = 0.8f;
	(this->field_0x94) = 0;
	(this->field_0x9c) = 0.8f;
	(this->field_0xb0).x = 0.0f;
	(this->field_0xb0).y = 0.0f;
	(this->field_0xb0).z = 0.0f;
	return;
}


void CameraCinematic::SetTransform(edF32MATRIX4* transformMatrix)
{
	float fVar1;
	int iVar2;
	edF32MATRIX4* peVar3;
	edF32MATRIX4* peVar4;
	edF32VECTOR4 local_10;

	iVar2 = 8;
	peVar3 = &transformationMatrix;
	peVar4 = transformMatrix;
	do {
		iVar2 = iVar2 + -1;
		fVar1 = peVar4->ab;
		peVar3->aa = peVar4->aa;
		peVar4 = (edF32MATRIX4*)&peVar4->ac;
		peVar3->ab = fVar1;
		peVar3 = (edF32MATRIX4*)&peVar3->ac;
	} while (0 < iVar2);
	edF32Matrix4ToEulerSoft(transformMatrix, &this->field_0xb0, "ZXY");
	edF32Vector4AddHard(&local_10, (edF32VECTOR4*)&this->transformationMatrix.da,
		(edF32VECTOR4*)&transformMatrix->ca);
	this->lookAt.x = local_10.x;
	this->lookAt.y = local_10.y;
	this->lookAt.z = local_10.z;
	this->lookAt.w = local_10.w;
	return;
}

ECameraType CameraCinematic::GetMode(void)
{
	return CT_Cinematic;
}

void CameraSet3DPos(edFCamera* pCamera)
{
	float fVar1;
	float fVar2;
	float fVar3;
	edF32VECTOR4 rowDiffVector;
	edF32VECTOR4 rotationVector;
	edF32MATRIX4 transformedMatrix;

	EDFCAMERA_LOG(LogLevel::Info, "CameraSet3DPos Updating position for camera: %p", pCamera);
	EDFCAMERA_LOG(LogLevel::Info, "Position: x=%f, y=%f, z=%f", pCamera->position.x, pCamera->position.y, pCamera->position.z);
	EDFCAMERA_LOG(LogLevel::Info, "Look At : x=%f, y=%f, z=%f", pCamera->lookAt.x, pCamera->lookAt.y, pCamera->lookAt.z);

	transformedMatrix = gF32Matrix4Unit;

	rowDiffVector.x = (pCamera->position).x - (pCamera->lookAt).x;
	rowDiffVector.y = (pCamera->position).y - (pCamera->lookAt).y;
	rowDiffVector.z = (pCamera->position).z - (pCamera->lookAt).z;
	rowDiffVector.w = (pCamera->position).w - (pCamera->lookAt).w;
	edF32Vector4NormalizeHard((float*)&rowDiffVector, (float*)&rowDiffVector);
	GetAnglesFromVector(&rotationVector, &rowDiffVector);
	if (pCamera->rotationZ != 0.0) {
		edF32Matrix4RotateZHard(pCamera->rotationZ, &transformedMatrix, &transformedMatrix);
	}
	edF32Matrix4RotateXHard(rotationVector.x, &transformedMatrix, &transformedMatrix);
	edF32Matrix4RotateYHard(rotationVector.y, &transformedMatrix, &transformedMatrix);
	edF32Matrix4CopyHard(&pCamera->transformMatrix, &transformedMatrix);
	fVar3 = (pCamera->position).y;
	fVar1 = (pCamera->position).z;
	fVar2 = (pCamera->position).w;
	(pCamera->transformMatrix).da = (pCamera->position).x;
	(pCamera->transformMatrix).db = fVar3;
	(pCamera->transformMatrix).dc = fVar1;
	(pCamera->transformMatrix).dd = fVar2;
	edF32Matrix4GetInverseOrthoHard(&pCamera->worldToCamera, &pCamera->transformMatrix);
	fVar3 = (pCamera->transformMatrix).ab;
	fVar1 = (pCamera->transformMatrix).ac;
	fVar2 = (pCamera->transformMatrix).ad;
	(pCamera->calculatedRotation).x = (pCamera->transformMatrix).aa;
	(pCamera->calculatedRotation).y = fVar3;
	(pCamera->calculatedRotation).z = fVar1;
	(pCamera->calculatedRotation).w = fVar2;
	return;
}

edF32VECTOR3 gF32Vector3Zero = {};

CCameraExt::CCameraExt()
{
	float fVar1;
	float fVar2;

	this->pActorView = (Actor*)0x0;
	this->distance = 0.0;
	fVar2 = gF32Vector3Zero.z;
	fVar1 = gF32Vector3Zero.y;
	(this->angles).alpha = gF32Vector3Zero.x;
	(this->angles).beta = fVar1;
	(this->angles).gamma = fVar2;
	return;
}

CCameraShadow::CCameraShadow(ByteCode* pByteCode)
{
	int iVar1;

	this->field_0x16cc = 0;
	this->field_0x16c4 = 0;
	//iVar1 = GameDListPatch_Register((CObject*)this, 0x2ee, 0);
	//this->patchRegister = iVar1;
	return;
}

void CCameraShadow::Init()
{
	CCameraExt::Init();
	//(*(code*)((this->cameraExt).camera.objBase.pVTable)->field_0x30)(this, 0);
	*(undefined*)&this->field_0xd0 = 0;
	SetDistance(40.0f);
	SetAngleGamma(1.5708f);
	this->field_0x16e4 = 0.5735765f;
	this->field_0x16e0 = 0.8191521f;
	*(undefined*)&this->field_0x1700 = 0;
	return;
}

ECameraType CCameraShadow::GetMode()
{
	return CT_ShadowSun;
}

CCameraMouse::CCameraMouse()
{
	this->field_0xd4 = -1;
	this->field_0xd0 = 1.0;
	this->flags_0xc = this->flags_0xc | 0x40000;
}

CCameraMouseQuake::CCameraMouseQuake()
{
	this->field_0x74 = 0.6f;
}

ECameraType CCameraMouseQuake::GetMode()
{
	return CT_MouseQuake;
}

edF32VECTOR4 gF32Vector4UnitZ = { 0.0f, 0.0f, 1.0f, 0.0f };

void CCameraMouseQuake::Init()
{
	float fVar1;
	float fVar2;
	float fVar3;

	CCameraExt::Init();
	this->SetDistance(6.0f);
	this->SetAngleAlpha(0.0f);
	this->SetAngleBeta(0.0f);
	this->SetAngleGamma(0.0f);
	this->transformationMatrix.da = gF32Vertex4Zero.x;
	this->transformationMatrix.db = gF32Vertex4Zero.y;
	this->transformationMatrix.dc = gF32Vertex4Zero.z;
	this->transformationMatrix.dd = gF32Vertex4Zero.w;
	this->lookAt.x = 0.0;
	this->lookAt.y = 0.0;
	this->lookAt.z = 1.0;
	this->lookAt.w = 1.0;
	fVar3 = gF32Vector4UnitZ.w;
	fVar2 = gF32Vector4UnitZ.z;
	fVar1 = gF32Vector4UnitZ.y;
	this->transformationMatrix.ca = gF32Vector4UnitZ.x;
	this->transformationMatrix.cb = fVar1;
	this->transformationMatrix.cc = fVar2;
	this->transformationMatrix.cd = fVar3;
	this->flags_0xc = this->flags_0xc | 0x20000;
	this->field_0x8 = -0x9f;
	return;
}

CCameraMouseAroundPerso::CCameraMouseAroundPerso()
{
	this->isSpecial = 0;
}

ECameraType CCameraMouseAroundPerso::GetMode()
{
	ECameraType EVar1;

	EVar1 = CT_AroundSpecial;
	if (this->isSpecial == 0) {
		EVar1 = CT_MouseAround;
	}
	return EVar1;
}

void CCameraMouseAroundPerso::Init()
{
	CCameraExt::Init();
	this->field_0x74 = 0.84;
	(this->field_0xe0).x = 0.0;
	(this->field_0xe0).y = 1.4;
	(this->field_0xe0).z = 0.0;
	(this->field_0xe0).w = 0.0;
	this->flags_0xc = this->flags_0xc | 0x20001;
	this->field_0x8 = -0x9f;
	return;
}

CCameraMouseAroundPersoSpecial::CCameraMouseAroundPersoSpecial()
{
	this->isSpecial = 1;
}
