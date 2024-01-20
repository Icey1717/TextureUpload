#ifndef LIGHT_MANAGER
#define LIGHT_MANAGER

#include "Types.h"
#include "LargeObject.h"

class CActor;
struct ed_3d_hierarchy;
struct S_CHECKPOINT;
struct ByteCode;
struct BaseShape;

struct ZoneHolder {
	ed_zone_3d* zone;
};

struct ed_3D_Light_Config {
	edF32VECTOR4* pLightAmbient;
	edF32MATRIX4* pLightDirections;
	edF32MATRIX4* pLightColorMatrix;
};

struct CLightConfig {
	edF32VECTOR4 lightAmbient;
	edF32MATRIX4 lightDirections;
	edF32MATRIX4 lightColorMatrix;
	ed_3D_Light_Config config;
	float field_0x9c;

	static void Validate(ed_3D_Light_Config* pConfig, bool bDoSomething);
	static float ComputeShadow(ed_3D_Light_Config* pConfig, edF32VECTOR4* param_2);
};

struct LightingContext {
	edF32VECTOR4 position;
	edF32VECTOR4* pLightDirection;
	edF32VECTOR4* pLightAmbient;
	edF32VECTOR4* pLightColor;
	undefined4 field_0x1c;
	float colorMultiplier;
};

class CLight {
public:
	CLight();

	virtual void Manage() { return; }
	virtual void Activate();
	virtual bool DoLighting(LightingContext* pContext) { return false; }
	virtual int GetBaseShape(BaseShape** ppBaseShape) { return 0; }
	virtual void Create(ByteCode* pByteCode) {}

	bool TestIlluminationZones(edF32VECTOR4* pLocation, int id);

	_rgba colour_0x4;
	ushort field_0x8;
	short field_0xa;
	ZoneHolder* field_0xc;
	undefined* field_0x10;
	undefined field_0x14;
	undefined field_0x15;
	undefined field_0x16;
	undefined field_0x17;
	undefined field_0x18;
	undefined field_0x19;
	undefined field_0x1a;
	undefined field_0x1b;
	undefined field_0x1c;
	undefined field_0x1d;
	undefined field_0x1e;
	undefined field_0x1f;
};

class CLightManager : public CObjectManager {

public:
	CLightManager();

	// Begin Manager
	//virtual void Game_Init();
	//virtual void Level_Init();
	//virtual void Level_Term();
	virtual void Level_AddAll(struct ByteCode* pByteCode);
	virtual void Level_Manage();
	virtual void Level_SectorChange(int oldSectorId, int newSectorId);
	// End Manager

	void ComputeLighting(CActor* pActor, uint flags, ed_3D_Light_Config* pConfig, float param_1);
	void ComputeLighting(ed_3d_hierarchy* pHier, edF32VECTOR4* pLocation, uint flags, ed_3D_Light_Config* pConfig, uint param_7, float param_1);
	void ComputeLighting(edF32VECTOR4* pLocation, ed_3D_Light_Config* pConfig);

	void AddLight(ByteCode* pByteCode, CLight** ppOutLight);

	CLight* CreateSimpleLight(ByteCode* pByteCode);
	void Activate(CLight* pLight, int param_3);
	void Reference(CLight* pLight, int param_3, bool param_4, bool param_5, int param_6);

	void BuildSectorList();
	void BuildActiveList();

	float ComputeCollision(CLight* pLightA, CLight* pLightB);

	int field_0x8;
	int lightCount;
	int sectorLightCount;
	int sectorId;
	int activeLightCount;
	int bSectorListDirty;
	int bActiveListDirty;
	int field_0x38;
	ed_3D_Light_Config lightConfig;

	CLight** aLights;
	CLight** aSectorLights;
	CLight** aActiveLights;

	CLightConfig* aLightConfigs;

	int totalLightCount;
	int totalLightConfigs;

	edF32VECTOR4 vector_0xf0;

	edF32VECTOR4 lightAmbient;
	edF32MATRIX4 lightDirections;
	edF32MATRIX4 lightColorMatrix;

	int field_0xe4;
	int field_0xe8;

	float shadowValue;

	int field_0x104;
	int field_0x108;
	int field_0x10c;
	int field_0x110;
};

#endif //LIGHT_MANAGER