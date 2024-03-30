#include "edCollisions.h"
#include "MathOps.h"
#include "port/pointer_conv.h"

GlobalCollisionData gColData;
CollisionTD gColTD;

edColConfig gColConfig;

void edColComputeMatrices(edColPRIM_OBJECT* pPrimObj)
{
	float puVar4;
	float puVar5;
	float puVar6;
	float puVar1;
	float puVar2;
	float puVar3;
	edF32VECTOR4* pfVar1;
	float fVar2;
	float fVar3;
	float fVar4;
	float fVar5;
	float fVar6;
	float fVar7;
	float fVar8;
	float fVar9;
	edF32VECTOR4 local_10;

	pfVar1 = &pPrimObj->field_0x90;

	fVar2 = cosf(pPrimObj->field_0xa0.x);
	fVar4 = sinf(pPrimObj->field_0xa0.x);

	fVar3 = cosf(pPrimObj->field_0xa0.y);
	fVar6 = sinf(pPrimObj->field_0xa0.y);

	fVar7 = cosf(pPrimObj->field_0xa0.z);
	fVar8 = sinf(pPrimObj->field_0xa0.z);

	(pPrimObj->matrix_0x0).aa = pfVar1->x * fVar7 * fVar3;
	(pPrimObj->matrix_0x0).ab = pfVar1->x * fVar3 * fVar8;
	(pPrimObj->matrix_0x0).ac = pfVar1->x * -fVar6;
	(pPrimObj->matrix_0x0).ad = 0.0f;
	(pPrimObj->matrix_0x0).ba = pPrimObj->field_0x90.y * (fVar7 * fVar4 * fVar6 - fVar2 * fVar8);
	fVar9 = fVar7 * fVar2;
	(pPrimObj->matrix_0x0).bb = pPrimObj->field_0x90.y * (fVar6 * fVar8 * fVar4 + fVar9);
	(pPrimObj->matrix_0x0).bc = pPrimObj->field_0x90.y * fVar4 * fVar3;
	(pPrimObj->matrix_0x0).bd = 0.0f;
	fVar5 = fVar6 * fVar9 + fVar8 * fVar4;
	(pPrimObj->matrix_0x0).ca = pPrimObj->field_0x90.z * fVar5;
	(pPrimObj->matrix_0x0).cb = pPrimObj->field_0x90.z * (fVar6 * fVar2 * fVar8 - fVar4 * fVar7);
	(pPrimObj->matrix_0x0).cc = pPrimObj->field_0x90.z * fVar2 * fVar3;
	(pPrimObj->matrix_0x0).cd = 0.0f;
	(pPrimObj->matrix_0x0).da = (pPrimObj->field_0xb0).x;
	(pPrimObj->matrix_0x0).db = (pPrimObj->field_0xb0).y;
	(pPrimObj->matrix_0x0).dc = (pPrimObj->field_0xb0).z;
	(pPrimObj->matrix_0x0).dd = 1.0;

	(pPrimObj->matrix_0x40).aa = (fVar7 * fVar3) / pfVar1->x;
	(pPrimObj->matrix_0x40).ab = (fVar6 * fVar4 * fVar7 + -fVar2 * fVar8) / pPrimObj->field_0x90.y;
	(pPrimObj->matrix_0x40).ac = fVar5 / pPrimObj->field_0x90.z;
	(pPrimObj->matrix_0x40).ad = 0.0f;
	(pPrimObj->matrix_0x40).ba = (fVar3 * fVar8) / pfVar1->x;
	(pPrimObj->matrix_0x40).bb = (fVar9 + fVar4 * fVar6 * fVar8) / pPrimObj->field_0x90.y;
	(pPrimObj->matrix_0x40).bc = (fVar8 * fVar2 * fVar6 + -fVar4 * fVar7) / pPrimObj->field_0x90.z;
	(pPrimObj->matrix_0x40).bd = 0.0f;
	(pPrimObj->matrix_0x40).ca = -fVar6 / pfVar1->x;
	(pPrimObj->matrix_0x40).cb = (fVar4 * fVar3) / pPrimObj->field_0x90.y;
	(pPrimObj->matrix_0x40).cc = (fVar2 * fVar3) / pPrimObj->field_0x90.z;
	(pPrimObj->matrix_0x40).cd = 0.0f;
	(pPrimObj->matrix_0x40).da = 0.0f;
	(pPrimObj->matrix_0x40).db = 0.0f;
	(pPrimObj->matrix_0x40).dc = 0.0f;
	(pPrimObj->matrix_0x40).dd = 1.0;

	local_10.x = -(pPrimObj->field_0xb0).x;
	local_10.y = -(pPrimObj->field_0xb0).y;
	local_10.z = -(pPrimObj->field_0xb0).z;
	local_10.w = 1.0;
	edF32Matrix4MulF32Vector4Hard(&local_10, &pPrimObj->matrix_0x40, &local_10);
	(pPrimObj->matrix_0x40).rowT = local_10;
	return;
}

edColG3D_OBB_TREE* edColLoadStatic(char** pOutData, char* pFileBuffer, uint* param_3, uint bConvertTriangles)
{
	byte bVar1;
	void* pvVar2;
	StaticCollisionEntry* pStaticColEntry;
	edF32TRIANGLE4* peVar3;
	int* piVar4;
	edObbTREE* pObbTree;
	int iVar6;
	float* pfVar7;
	int iVar8;

	pStaticColEntry = (StaticCollisionEntry*)*pOutData;

	if (*param_3 == 0) {
		pStaticColEntry = (StaticCollisionEntry*)&pStaticColEntry->obbTree;
	}

	*pOutData = (char*)(&pStaticColEntry->field_0x0 + pStaticColEntry->field_0x8);
	if (*pOutData == pFileBuffer + *(int*)(pFileBuffer + 8)) {
		*pOutData = (char*)0x0;
	}

	if ((pStaticColEntry->obbTree).bLoaded == 0) {
		(pStaticColEntry->obbTree).bLoaded = 1;

		{
			const int offset = (pStaticColEntry->obbTree).field_0x28;
			if (offset != 0) {
				(pStaticColEntry->obbTree).field_0x28 = STORE_SECTION(pFileBuffer + offset);
			}
		}

		{
			const int offset = (pStaticColEntry->obbTree).aTriangles;
			if (offset != 0) {
				(pStaticColEntry->obbTree).aTriangles = STORE_SECTION(pFileBuffer + offset);
			}
		}

		{
			const int offset = (pStaticColEntry->obbTree).field_0x30;
			if (offset != 0) {
				(pStaticColEntry->obbTree).field_0x30 = STORE_SECTION(pFileBuffer + offset);
			}
		}

		{
			const int offset = (pStaticColEntry->obbTree).aQuads;
			if (offset != 0) {
				(pStaticColEntry->obbTree).aQuads = STORE_SECTION(pFileBuffer + offset);
			}
		}

		{
			const int offset = (pStaticColEntry->obbTree).field_0x3c;
			if (offset != 0) {
				(pStaticColEntry->obbTree).field_0x3c = STORE_SECTION(pFileBuffer + offset);
			}
		}

		{
			const int offset = (pStaticColEntry->obbTree).field_0x38;
			if (offset != 0) {
				(pStaticColEntry->obbTree).field_0x38 = STORE_SECTION(pFileBuffer + offset);
			}
		}

		{
			const int offset = (pStaticColEntry->obbTree).pObbTree;
			if (offset != 0) {
				(pStaticColEntry->obbTree).pObbTree = STORE_SECTION(pFileBuffer + offset);
			}
		}

		edF32TRIANGLE4* pTriangle = LOAD_SECTION_CAST(edF32TRIANGLE4*, (pStaticColEntry->obbTree).aTriangles);
		char* pBase = LOAD_SECTION_CAST(char*, (pStaticColEntry->obbTree).field_0x28);
		for (iVar8 = (pStaticColEntry->obbTree).nbTriangles; iVar8 != 0; iVar8 = iVar8 + -1) {
			pTriangle->p1 = STORE_SECTION(pBase + pTriangle->p1 * 0x10);
			pTriangle->p2 = STORE_SECTION(pBase + pTriangle->p2 * 0x10);
			pTriangle->p3 = STORE_SECTION(pBase + pTriangle->p3 * 0x10);
			pTriangle = pTriangle + 1;
		}

		edF32QUAD4* pQuad = LOAD_SECTION_CAST(edF32QUAD4*, (pStaticColEntry->obbTree).aQuads);

		for (iVar6 = (pStaticColEntry->obbTree).nbQuads; iVar6 != 0; iVar6 = iVar6 + -1) {
			pQuad->p1 = STORE_SECTION(pBase + pQuad->p1 * 0x10);
			pQuad->p2 = STORE_SECTION(pBase + pQuad->p2 * 0x10);
			pQuad->p3 = STORE_SECTION(pBase + pQuad->p3 * 0x10);
			pQuad->p4 = STORE_SECTION(pBase + pQuad->p4 * 0x10);
			pQuad = pQuad + 1;
		}

		edObbTREE* pObbTreeBase = (edObbTREE*)LOAD_SECTION((pStaticColEntry->obbTree).pObbTree);

		pObbTree = pObbTreeBase;
		for (iVar6 = (pStaticColEntry->obbTree).field_0x20; iVar6 != 0; iVar6 = iVar6 + -1) {
			bVar1 = pObbTree->type;
			if (bVar1 == 0xa) {
				pObbTree->field_0x54[0] = STORE_SECTION(((char*)LOAD_SECTION((pStaticColEntry->obbTree).field_0x38) + pObbTree->field_0x54[0] * sizeof(edColPRIM_BOX)));
			}
			else {
				if (bVar1 == 0xb) {
					pObbTree->field_0x54[0] = STORE_SECTION(((char*)LOAD_SECTION((pStaticColEntry->obbTree).field_0x3c) + pObbTree->field_0x54[0] * sizeof(edColPRIM_SPHERE)));
				}
				else {
					if (bVar1 == 8) {
						pObbTree->field_0x54[0] = STORE_SECTION(((char*)LOAD_SECTION((pStaticColEntry->obbTree).aQuads) + pObbTree->field_0x54[0] * sizeof(edF32QUAD4)));
					}
					else {
						if (bVar1 == 5) {
							pObbTree->field_0x54[0] = STORE_SECTION(((char*)LOAD_SECTION((pStaticColEntry->obbTree).field_0x30) + pObbTree->field_0x54[0] * 0x18));
						}
						else {
							if (bVar1 == 4) {
								pObbTree->field_0x54[0] = STORE_SECTION(((char*)LOAD_SECTION((pStaticColEntry->obbTree).aTriangles) + pObbTree->field_0x54[0] * 0x10));
							}
							else {
								iVar8 = 0;
								if (bVar1 == 1) {
									bVar1 = pObbTree->count_0x52;
									for (; iVar8 < (int)(uint)bVar1; iVar8 = iVar8 + 1) {
										edObbTREE* pNext = (edObbTREE*)((char*)pObbTreeBase + (pObbTree->field_0x54[iVar8] * sizeof(edObbTREE)));
										pObbTree->field_0x54[iVar8] = STORE_SECTION(pNext);
									}
								}
							}
						}
					}
				}
			}
			
			pObbTree += 1;
		}
		if (bConvertTriangles != 0) {
			IMPLEMENTATION_GUARD(
			edColConvertToTriangle4Fast(&pStaticColEntry->obbTree);)
		}
	}
	return &pStaticColEntry->obbTree;
}

edColG3D_OBB_TREE* edColLoadStatic(char* pFileBuffer, uint length, uint bConvertTriangles)
{
	edColG3D_OBB_TREE* peVar1;
	uint local_8;
	char* local_4;

	local_8 = 0;
	local_4 = pFileBuffer;
	peVar1 = edColLoadStatic(&local_4, pFileBuffer, &local_8, bConvertTriangles);
	return peVar1;
}

bool edObbIntersect(edObbBOX* pBoxA, edObbBOX* pBoxB)
{
	float fVar1;
	float fVar2;
	float fVar3;
	float fVar4;
	float fVar5;
	float fVar6;
	float fVar7;
	float fVar8;
	float fVar9;
	float fVar10;
	float fVar11;
	float fVar12;
	float fVar13;
	float fVar14;
	float fVar15;
	float fVar16;
	float fVar17;
	float fVar18;
	float fVar19;
	float fVar20;
	float fVar21;
	float fVar23;
	float fVar24;
	float fVar25;
	edF32VECTOR3 fVar22;
	float fVar26;
	float fVar27;
	float fVar28;
	float fVar29;
	float fVar30;
	float fVar31;
	edF32VECTOR3 fVar32;
	edF32VECTOR3 fVar33;
	edF32VECTOR3 fVar34;

	fVar25 = pBoxA->width;
	fVar23 = pBoxA->height;
	fVar24 = pBoxA->depth;
	fVar20 = pBoxB->width;
	fVar21 = pBoxB->height;
	fVar19 = pBoxB->depth;
	fVar22.x = (pBoxA->transform).aa;
	fVar22.y = (pBoxA->transform).ab;
	fVar22.z = (pBoxA->transform).ac;
	fVar32.x = (pBoxB->transform).aa;
	fVar32.y = (pBoxB->transform).ab;
	fVar32.z = (pBoxB->transform).ac;
	fVar33.x = (pBoxB->transform).ba;
	fVar33.y = (pBoxB->transform).bb;
	fVar33.z = (pBoxB->transform).bc;
	fVar34.x = (pBoxB->transform).ca;
	fVar34.y = (pBoxB->transform).cb;
	fVar34.z = (pBoxB->transform).cc;
	fVar1 = (pBoxB->transform).da - (pBoxA->transform).da;
	fVar14 = (pBoxB->transform).db - (pBoxA->transform).db;
	fVar18 = (pBoxB->transform).dc - (pBoxA->transform).dc;
	fVar2 = fVar22.x * fVar32.x + fVar22.y * fVar32.y + fVar22.z * fVar32.z;
	fVar11 = fVar22.y * fVar33.y + fVar22.x * fVar33.x + fVar22.z * fVar33.z;
	fVar15 = fVar22.z * fVar34.z + fVar22.x * fVar34.x + fVar22.y * fVar34.y;
	fVar3 = fVar22.x * fVar1 + fVar22.y * fVar14 + fVar22.z * fVar18;
	fVar4 = fabs(fVar2);
	fVar7 = fabs(fVar11);
	fVar8 = fabs(fVar15);
	if (fabs(fVar3) <= fVar19 * fVar8 + fVar21 * fVar7 + fVar20 * fVar4 + fVar25 + 0.0001f) {
		fVar26 = (pBoxA->transform).ba;
		fVar27 = (pBoxA->transform).bb;
		fVar28 = (pBoxA->transform).bc;
		fVar5 = fVar26 * fVar32.x + fVar27 * fVar32.y + fVar28 * fVar32.z;
		fVar12 = fVar27 * fVar33.y + fVar26 * fVar33.x + fVar28 * fVar33.z;
		fVar16 = fVar28 * fVar34.z + fVar26 * fVar34.x + fVar27 * fVar34.y;
		fVar26 = fVar26 * fVar1 + fVar27 * fVar14 + fVar28 * fVar18;
		fVar27 = fabs(fVar5);
		fVar28 = fabs(fVar12);
		fVar9 = fabs(fVar16);
		if (fabs(fVar26) <= fVar19 * fVar9 + fVar21 * fVar28 + fVar20 * fVar27 + fVar23 + 0.0001f) {
			fVar29 = (pBoxA->transform).ca;
			fVar30 = (pBoxA->transform).cb;
			fVar31 = (pBoxA->transform).cc;
			fVar6 = fVar29 * fVar32.x + fVar30 * fVar32.y + fVar31 * fVar32.z;
			fVar13 = fVar30 * fVar33.y + fVar29 * fVar33.x + fVar31 * fVar33.z;
			fVar17 = fVar31 * fVar34.z + fVar29 * fVar34.x + fVar30 * fVar34.y;
			fVar29 = fVar29 * fVar1 + fVar30 * fVar14 + fVar31 * fVar18;
			fVar30 = fabs(fVar6);
			fVar31 = fabs(fVar13);
			fVar10 = fabs(fVar17);
			if (((((fabs(fVar29) <= fVar19 * fVar10 + fVar21 * fVar31 + fVar20 * fVar30 + fVar24 + 0.0001f) &&
				(fabs(fVar32.x * fVar1 + fVar32.y * fVar14 + fVar32.z * fVar18) <=
					fVar24 * fVar30 + fVar23 * fVar27 + fVar25 * fVar4 + fVar20 + 0.0001f)) &&
				(fabs(fVar33.x * fVar1 + fVar33.y * fVar14 + fVar33.z * fVar18) <=
					fVar24 * fVar31 + fVar23 * fVar28 + fVar25 * fVar7 + fVar21 + 0.0001f)) &&
				(((fabs(fVar34.x * fVar1 + fVar34.y * fVar14 + fVar34.z * fVar18) <=
					fVar24 * fVar10 + fVar23 * fVar9 + fVar25 * fVar8 + fVar19 + 0.0001f &&
					(fabs(fVar29 * fVar5 - fVar26 * fVar6) <=
						fVar19 * fVar7 + fVar21 * fVar8 + fVar24 * fVar27 + fVar23 * fVar30 + 0.0001f + 0.0f)) &&
					((fabs(fVar29 * fVar12 - fVar26 * fVar13) <=
						fVar19 * fVar4 + fVar20 * fVar8 + fVar24 * fVar28 + fVar23 * fVar31 + 0.0001f + 0.0f &&
						((fabs(fVar29 * fVar16 - fVar26 * fVar17) <=
							fVar21 * fVar4 + fVar20 * fVar7 + fVar24 * fVar9 + fVar23 * fVar10 + 0.0001f + 0.0f &&
							(fabs(fVar3 * fVar6 - fVar29 * fVar2) <=
								fVar19 * fVar28 + fVar21 * fVar9 + fVar24 * fVar4 + fVar25 * fVar30 + 0.0001f + 0.0f)))))))) &&
				((fabs(fVar3 * fVar13 - fVar29 * fVar11) <=
					fVar19 * fVar27 + fVar20 * fVar9 + fVar24 * fVar7 + fVar25 * fVar31 + 0.0001f + 0.0f &&
					((((fabs(fVar3 * fVar17 - fVar29 * fVar15) <=
						fVar21 * fVar27 + fVar20 * fVar28 + fVar24 * fVar8 + fVar25 * fVar10 + 0.0001f + 0.0f &&
						(fabs(fVar26 * fVar2 - fVar3 * fVar5) <=
							fVar19 * fVar31 + fVar21 * fVar10 + fVar23 * fVar4 + fVar25 * fVar27 + 0.0001f + 0.0f)) &&
						(fabs(fVar26 * fVar11 - fVar3 * fVar12) <=
							fVar19 * fVar30 + fVar20 * fVar10 + fVar23 * fVar7 + fVar25 * fVar28 + 0.0001f + 0.0f)) &&
						(fabs(fVar26 * fVar15 - fVar3 * fVar16) <=
							fVar21 * fVar30 + fVar20 * fVar31 + fVar23 * fVar8 + fVar25 * fVar9 + 0.0001f + 0.0f)))))) {
				return true;
			}
		}
	}
	return false;
}

static int _gColSizeOfPrims[21] = { 
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x90, 0x90,
	0x90, 0x150, 0x150, 0x150,
	0x0, 0x0, 0x0, 0x1D0,
	0x0 
};

float edColSqrDistancePointTriangle(edF32VECTOR4* v0, edF32TRIANGLE4* t0)
{
	edF32VECTOR4* peVar1;
	edF32VECTOR4* peVar2;
	edF32VECTOR4* peVar3;
	float fVar4;
	float fVar5;
	float fVar6;
	float fVar7;
	float fVar8;
	float fVar9;
	float fVar10;
	float fVar11;
	float fVar12;
	float fVar13;
	float fVar14;
	float fVar15;

	peVar1 = (edF32VECTOR4*)LOAD_SECTION(t0->p1);
	peVar2 = (edF32VECTOR4*)LOAD_SECTION(t0->p2);
	peVar3 = (edF32VECTOR4*)LOAD_SECTION(t0->p3);

	fVar13 = peVar1->x;
	fVar14 = peVar1->y;
	fVar15 = peVar1->z;
	fVar4 = peVar2->x - fVar13;
	fVar11 = peVar2->y - fVar14;
	fVar9 = peVar2->z - fVar15;
	fVar5 = peVar3->x - fVar13;
	fVar12 = peVar3->y - fVar14;
	fVar10 = peVar3->z - fVar15;
	fVar13 = fVar13 - v0->x;
	fVar14 = fVar14 - v0->y;
	fVar15 = fVar15 - v0->z;
	fVar6 = fVar4 * fVar4 + fVar11 * fVar11 + fVar9 * fVar9;
	fVar7 = fVar4 * fVar5 + fVar11 * fVar12 + fVar9 * fVar10;
	fVar8 = fVar5 * fVar5 + fVar12 * fVar12 + fVar10 * fVar10;
	fVar4 = fVar13 * fVar4 + fVar14 * fVar11 + fVar15 * fVar9;
	fVar5 = fVar13 * fVar5 + fVar14 * fVar12 + fVar15 * fVar10;
	fVar13 = fVar13 * fVar13 + fVar14 * fVar14 + fVar15 * fVar15;
	fVar14 = fabs(fVar6 * fVar8 - fVar7 * fVar7);
	fVar11 = fVar7 * fVar5 - fVar8 * fVar4;
	fVar12 = fVar7 * fVar4 - fVar6 * fVar5;
	if (fVar11 + fVar12 <= fVar14) {
		if (fVar11 < 0.0f) {
			if (fVar12 < 0.0f) {
				if (fVar4 < 0.0f) {
					if (fVar6 <= -fVar4) {
						fVar13 = fVar13 + fVar6 + fVar4 * 2.0f;
					}
					else {
						fVar13 = fVar13 + fVar4 * (-fVar4 / fVar6);
					}
				}
				else {
					if (fVar5 < 0.0f) {
						if (fVar8 <= -fVar5) {
							fVar13 = fVar13 + fVar8 + fVar5 * 2.0f;
						}
						else {
							fVar13 = fVar13 + fVar5 * (-fVar5 / fVar8);
						}
					}
				}
			}
			else {
				if (fVar5 < 0.0f) {
					if (fVar8 <= -fVar5) {
						fVar13 = fVar13 + fVar8 + fVar5 * 2.0f;
					}
					else {
						fVar13 = fVar13 + fVar5 * (-fVar5 / fVar8);
					}
				}
			}
		}
		else {
			if (fVar12 < 0.0f) {
				if (fVar4 < 0.0f) {
					if (fVar6 <= -fVar4) {
						fVar13 = fVar13 + fVar6 + fVar4 * 2.0f;
					}
					else {
						fVar13 = fVar13 + fVar4 * (-fVar4 / fVar6);
					}
				}
			}
			else {
				fVar14 = 1.0f / fVar14;
				fVar12 = fVar12 * fVar14;
				fVar11 = fVar11 * fVar14;
				fVar13 = fVar13 + fVar12 * (fVar5 * 2.0f + fVar7 * fVar11 + fVar8 * fVar12) +
					fVar11 * (fVar4 * 2.0f + fVar6 * fVar11 + fVar7 * fVar12);
			}
		}
	}
	else {
		if (fVar11 < 0.0f) {
			fVar11 = fVar8 + fVar5;
			if (fVar7 + fVar4 < fVar11) {
				fVar11 = fVar11 - (fVar7 + fVar4);
				fVar12 = fVar8 + (fVar6 - fVar7 * 2.0f);
				if (fVar12 <= fVar11) {
					fVar13 = fVar13 + fVar6 + fVar4 * 2.0f;
				}
				else {
					fVar11 = fVar11 / fVar12;
					fVar12 = 1.0f - fVar11;
					fVar13 = fVar13 + fVar12 * (fVar5 * 2.0f + fVar7 * fVar11 + fVar8 * fVar12) +
						fVar11 * (fVar4 * 2.0f + fVar6 * fVar11 + fVar7 * fVar12);
				}
			}
			else {
				if (fVar11 <= 0.0f) {
					fVar13 = fVar13 + fVar8 + fVar5 * 2.0f;
				}
				else {
					if (fVar5 < 0.0f) {
						fVar13 = fVar13 + fVar5 * (-fVar5 / fVar8);
					}
				}
			}
		}
		else {
			if (fVar12 < 0.0f) {
				fVar11 = fVar6 + fVar4;
				if (fVar7 + fVar5 < fVar11) {
					fVar11 = fVar11 - (fVar7 + fVar5);
					fVar12 = fVar8 + (fVar6 - fVar7 * 2.0f);
					if (fVar12 <= fVar11) {
						fVar13 = fVar13 + fVar8 + fVar5 * 2.0f;
					}
					else {
						fVar11 = fVar11 / fVar12;
						fVar12 = 1.0f - fVar11;
						fVar13 = fVar13 + fVar11 * (fVar5 * 2.0f + fVar7 * fVar12 + fVar8 * fVar11) +
							fVar12 * (fVar4 * 2.0f + fVar6 * fVar12 + fVar7 * fVar11);
					}
				}
				else {
					if (fVar11 <= 0.0f) {
						fVar13 = fVar13 + fVar6 + fVar4 * 2.0f;
					}
					else {
						if (fVar4 < 0.0f) {
							fVar13 = fVar13 + fVar4 * (-fVar4 / fVar6);
						}
					}
				}
			}
			else {
				fVar11 = ((fVar8 + fVar5) - fVar7) - fVar4;
				if (fVar11 <= 0.0f) {
					fVar13 = fVar13 + fVar8 + fVar5 * 2.0f;
				}
				else {
					fVar12 = fVar8 + (fVar6 - fVar7 * 2.0f);
					if (fVar12 <= fVar11) {
						fVar13 = fVar13 + fVar6 + fVar4 * 2.0f;
					}
					else {
						fVar11 = fVar11 / fVar12;
						fVar12 = 1.0f - fVar11;
						fVar13 = fVar13 + fVar12 * (fVar5 * 2.0f + fVar7 * fVar11 + fVar8 * fVar12) +
							fVar11 * (fVar4 * 2.0f + fVar6 * fVar11 + fVar7 * fVar12);
					}
				}
			}
		}
	}
	return fabs(fVar13);
}

struct edColPRIM_IN {
	edColOBJECT* pColObj;
	edColOBJECT* pOtherColObj;
	edColPRIM_OBJECT* pPrim;
	uint aType;
	edF32VECTOR4* field_0x10;
	edF32VECTOR4* field_0x14;
	edF32VECTOR4* field_0x18;
};

struct edColPRIM_SPHERE_QUAD4_IN : public edColPRIM_IN {
	struct edF32QUAD4* pQuad;
};

struct edColPRIM_BOX_QUAD4_IN : public edColPRIM_IN {
	struct edF32QUAD4* pQuad;
};

struct edF32TRIANGLE4_INFOS {
	edF32VECTOR4 field_0x0;
	float field_0x10;
};

void edColTriangle4GetInfo(edF32TRIANGLE4_INFOS* pInfo, edF32TRIANGLE4* pTriangle)
{
	edF32VECTOR4* peVar1;
	edF32VECTOR4* peVar2;
	float fVar3;
	float fVar4;
	float fVar5;
	float fVar6;
	float fVar7;
	float fVar8;

	peVar1 = LOAD_SECTION_CAST(edF32VECTOR4*, pTriangle->p1);
	peVar2 = LOAD_SECTION_CAST(edF32VECTOR4*, pTriangle->p2);
	fVar3 = peVar2->x - peVar1->x;
	fVar5 = peVar2->y - peVar1->y;
	fVar7 = peVar2->z - peVar1->z;
	peVar2 = LOAD_SECTION_CAST(edF32VECTOR4*, pTriangle->p3);
	fVar4 = peVar2->x - peVar1->x;
	fVar6 = peVar2->y - peVar1->y;
	fVar8 = peVar2->z - peVar1->z;
	(pInfo->field_0x0).x = fVar5 * fVar8 - fVar6 * fVar7;
	(pInfo->field_0x0).y = fVar7 * fVar4 - fVar8 * fVar3;
	(pInfo->field_0x0).z = fVar3 * fVar6 - fVar4 * fVar5;
	(pInfo->field_0x0).w = in_vf0x;
	edF32Vector4NormalizeHard_Fixed(&pInfo->field_0x0, &pInfo->field_0x0);
	pInfo->field_0x10 = -((pInfo->field_0x0).x * peVar1->x + (pInfo->field_0x0).y * peVar1->y + (pInfo->field_0x0).z * peVar1->z);
	return;
}

struct edColRAY_TRIANGLE4_IN {
	edF32VECTOR4* field_0x0;
	edF32VECTOR4* field_0x4;
	edF32TRIANGLE4* pTriangle;
};

const float FLOAT_0044854c = 0.001f;

bool edColIntersectRayTriangle4(float* pOutDistance, edColRAY_TRIANGLE4_IN* pRayTriangleIn)
{
	edF32TRIANGLE4* peVar1;
	edF32VECTOR4* peVar2;
	edF32VECTOR4* peVar3;
	edF32VECTOR4* peVar4;
	edF32VECTOR4* peVar5;
	edF32VECTOR4* peVar6;
	float fVar7;
	float fVar8;
	float fVar9;
	float fVar10;
	float fVar11;
	float fVar12;
	float fVar13;
	float fVar14;
	float fVar15;
	float fVar16;
	float fVar17;
	float fVar18;
	float fVar19;
	float fVar20;
	float fVar21;
	float fVar22;
	float fVar23;
	float fVar24;

	fVar23 = FLOAT_0044854c;
	fVar10 = g_DefaultNearClip_0044851c;
	*pOutDistance = -8888.0;
	peVar1 = pRayTriangleIn->pTriangle;
	peVar2 = pRayTriangleIn->field_0x4;
	peVar3 = pRayTriangleIn->field_0x0;
	peVar4 = LOAD_SECTION_CAST(edF32VECTOR4*, peVar1->p1);
	peVar5 = LOAD_SECTION_CAST(edF32VECTOR4*, peVar1->p2);
	peVar6 = LOAD_SECTION_CAST(edF32VECTOR4*, peVar1->p3);
	fVar14 = peVar4->x;
	fVar15 = peVar4->y;
	fVar16 = peVar4->z;
	fVar17 = peVar2->x;
	fVar18 = peVar2->y;
	fVar19 = peVar2->z;
	fVar7 = peVar5->x - fVar14;
	fVar11 = peVar5->y - fVar15;
	fVar24 = peVar5->z - fVar16;
	fVar8 = peVar6->x - fVar14;
	fVar12 = peVar6->y - fVar15;
	fVar13 = peVar6->z - fVar16;
	fVar20 = fVar18 * fVar13 - fVar12 * fVar19;
	fVar21 = fVar19 * fVar8 - fVar13 * fVar17;
	fVar22 = fVar17 * fVar12 - fVar8 * fVar18;
	fVar9 = fVar7 * fVar20 + fVar11 * fVar21 + fVar24 * fVar22;
	if (fVar23 <= fVar9) {
		fVar14 = peVar3->x - fVar14;
		fVar15 = peVar3->y - fVar15;
		fVar16 = peVar3->z - fVar16;
		fVar10 = fVar14 * fVar20 + fVar15 * fVar21 + fVar16 * fVar22;
		if (fVar10 < 0.0f) {
			return false;
		}
		if (fVar10 <= fVar9) {
			fVar23 = fVar15 * fVar24 - fVar11 * fVar16;
			fVar24 = fVar16 * fVar7 - fVar24 * fVar14;
			fVar14 = fVar14 * fVar11 - fVar7 * fVar15;
			fVar7 = fVar17 * fVar23 + fVar18 * fVar24 + fVar19 * fVar14;
			if ((0.0f <= fVar7) && (fVar10 + fVar7 <= fVar9)) {
			LAB_00250f60:
				*pOutDistance = (fVar8 * fVar23 + fVar12 * fVar24 + fVar13 * fVar14) / fVar9;
				return true;
			}
		}
	}
	else {
		if (fVar9 <= fVar10) {
			fVar14 = peVar3->x - fVar14;
			fVar15 = peVar3->y - fVar15;
			fVar16 = peVar3->z - fVar16;
			fVar10 = fVar14 * fVar20 + fVar15 * fVar21 + fVar16 * fVar22;
			if ((fVar10 <= 0.0f) && (fVar9 <= fVar10)) {
				fVar23 = fVar15 * fVar24 - fVar11 * fVar16;
				fVar24 = fVar16 * fVar7 - fVar24 * fVar14;
				fVar14 = fVar14 * fVar11 - fVar7 * fVar15;
				fVar7 = fVar17 * fVar23 + fVar18 * fVar24 + fVar19 * fVar14;
				if ((fVar7 <= 0.0f) && (fVar9 <= fVar10 + fVar7)) goto LAB_00250f60;
			}
		}
		else {
			*pOutDistance = -9999.0f;
		}
	}
	return false;
}

float FLOAT_004485a4 = 1.0E30f;

bool edObbRayPlaneClip(float param_1, float param_2, float* param_3, float* param_4)

{
	bool bVar1;

	if (0.0f < param_1) {
		bVar1 = false;
		if (param_2 <= param_1 * *param_4) {
			if (param_1 * *param_3 < param_2) {
				*param_3 = param_2 / param_1;
			}
			bVar1 = true;
		}
	}
	else {
		if (param_1 < 0.0f) {
			bVar1 = false;
			if (param_2 <= param_1 * *param_3) {
				if (param_1 * *param_4 < param_2) {
					*param_4 = param_2 / param_1;
				}
				bVar1 = true;
			}
		}
		else {
			bVar1 = param_2 <= 0.0f;
		}
	}
	return bVar1;
}

float edObbIntersectObbRay(edObbBOX* pBox, edF32VECTOR4* param_2, edF32VECTOR4* param_3)
{
	float fVar1;
	float fVar2;
	float fVar3;
	float fVar4;
	float fVar5;
	float fVar6;
	bool bVar7;
	bool bVar8;
	bool bVar9;
	bool bVar10;
	bool bVar11;
	float fVar12;
	bool bVar13;
	bool bVar14;
	bool bVar15;
	bool bVar16;
	bool bVar17;
	bool bVar18;
	float puVar19;
	float fVar19;
	float local_8;
	float local_4;

	fVar12 = FLOAT_004485a4;
	fVar1 = param_2->x - (pBox->transform).da;
	fVar5 = param_2->y - (pBox->transform).db;
	fVar6 = param_2->z - (pBox->transform).dc;
	fVar2 = fVar1 * (pBox->transform).aa + fVar5 * (pBox->transform).ab + fVar6 * (pBox->transform).ac;
	fVar3 = fVar1 * (pBox->transform).ba + fVar5 * (pBox->transform).bb + fVar6 * (pBox->transform).bc;
	fVar1 = fVar1 * (pBox->transform).ca + fVar5 * (pBox->transform).cb + fVar6 * (pBox->transform).cc;
	fVar5 = param_3->x * (pBox->transform).aa + param_3->y * (pBox->transform).ab + param_3->z * (pBox->transform).ac;
	fVar6 = param_3->x * (pBox->transform).ba + param_3->y * (pBox->transform).bb + param_3->z * (pBox->transform).bc;
	fVar4 = param_3->x * (pBox->transform).ca + param_3->y * (pBox->transform).cb + param_3->z * (pBox->transform).cc;
	local_4 = 0.0f;
	local_8 = FLOAT_004485a4;
	fVar19 = 0.0f;
	bVar13 = edObbRayPlaneClip(fVar5, -fVar2 - pBox->width, &local_4, &local_8);
	bVar14 = edObbRayPlaneClip(-fVar5, fVar2 - pBox->width, &local_4, &local_8);
	bVar15 = edObbRayPlaneClip(fVar6, -fVar3 - pBox->height, &local_4, &local_8);
	bVar16 = edObbRayPlaneClip(-fVar6, fVar3 - pBox->height, &local_4, &local_8);
	bVar17 = edObbRayPlaneClip(fVar4, -fVar1 - pBox->depth, &local_4, &local_8);
	bVar18 = edObbRayPlaneClip(-fVar4, fVar1 - pBox->depth, &local_4, &local_8);
	bVar11 = false;
	bVar10 = false;
	bVar9 = false;
	bVar8 = false;
	bVar7 = false;
	if ((bVar13 != false) && (bVar14 != false)) {
		bVar7 = true;
	}
	if ((bVar7) && (bVar15 != false)) {
		bVar8 = true;
	}
	if ((bVar8) && (bVar16 != false)) {
		bVar9 = true;
	}
	if ((bVar9) && (bVar17 != false)) {
		bVar10 = true;
	}
	if ((bVar10) && (bVar18 != false)) {
		bVar11 = true;
	}
	bVar7 = false;
	if (bVar11) {
		bVar8 = true;
		if ((local_4 == fVar19) && (local_8 == fVar12)) {
			bVar8 = false;
		}
		if (bVar8) {
			bVar7 = true;
		}
	}
	if (bVar7) {
		puVar19 = 0.0f;
		if (0.0f < local_4) {
			if (local_8 < FLOAT_004485a4) {
				puVar19 = (float)((int)local_4 * (uint)(local_4 < local_8) | (int)local_8 * (uint)(local_4 >= local_8));
			}
		}
		else {
			puVar19 = -1.0f;
			if (local_8 < FLOAT_004485a4) {
				return 0.0f;
			}
		}
	}
	else {
		puVar19 = -1.0f;
	}
	return puVar19;
}

void edColIntersectRayUnitBoxUnit(edColINFO_OUT* pColInfoOut, edColPRIM_RAY_UNIT_BOX_UNIT_IN* pParams)
{
	edF32VECTOR4* peVar1;
	float fVar2;
	edF32VECTOR4 local_60;
	edObbBOX eStack80;

	pColInfoOut->result = 0;
	edF32Matrix4SetIdentityHard(&eStack80.transform);
	eStack80.width = 0.5f;
	eStack80.height = 0.5f;
	eStack80.depth = 0.5f;
	edF32Vector4NormalizeHard_Fixed(&local_60, pParams->field_0x4);
	fVar2 = edObbIntersectObbRay(&eStack80, pParams->field_0x0, &local_60);

	if (0.0f <= fVar2) {
		local_60.xyz = local_60.xyz * fVar2;

		pColInfoOut->field_0x20.xyz = local_60.xyz + pParams->field_0x0->xyz;
		pColInfoOut->field_0x20.w = local_60.w * fVar2 + pParams->field_0x0->w;

		pColInfoOut->result = 1;
	}

	return;
}

struct edColPRIM_RAY_SPHERE_UNIT_IN {
	edF32VECTOR4* field_0x0;
	edF32VECTOR4* field_0x4;
};

uint edColIntersectRaySphereUnit(edColINFO_OUT* pColInfoOut, edColPRIM_RAY_SPHERE_UNIT_IN* pPrimRaySphereIn)
{
	edF32VECTOR4* peVar1;
	edF32VECTOR4* peVar2;
	edF32VECTOR4* peVar3;
	edF32VECTOR4* peVar4;
	uint uVar5;
	float fVar6;
	float fVar7;
	float fVar8;
	float fVar9;
	float fVar10;
	float fVar11;
	float fVar12;
	float fVar13;
	float fVar14;

	pColInfoOut->result = 0;
	peVar1 = pPrimRaySphereIn->field_0x4;
	fVar8 = peVar1->x * peVar1->x + peVar1->y * peVar1->y + peVar1->z * peVar1->z;
	peVar1 = pPrimRaySphereIn->field_0x4;
	peVar2 = pPrimRaySphereIn->field_0x0;
	peVar3 = pPrimRaySphereIn->field_0x0;
	peVar4 = pPrimRaySphereIn->field_0x0;
	fVar9 = (peVar1->x * peVar2->x + peVar1->y * peVar2->y + peVar1->z * peVar2->z) * 2.0;
	fVar6 = fVar9 * fVar9 - fVar8 * 4.0 * ((peVar3->x * peVar4->x + peVar3->y * peVar4->y + peVar3->z * peVar4->z) - 1.0);
	uVar5 = 0xffffffff;
	if (0.0 <= fVar6) {
		fVar6 = sqrtf(fVar6);
		fVar7 = 1.0 / (fVar8 * 2.0);
		fVar8 = fVar7 * (-fVar9 + fVar6);
		fVar7 = fVar7 * (-fVar9 - fVar6);
		uVar5 = (uint)(0.0 <= fVar7);
		if ((fVar8 < 0.0) || (1.0 < fVar8)) {
			if ((fVar7 < 0.0) || (fVar8 = fVar7, 1.0 < fVar7)) {
				fVar8 = -8888.0;
			}
		}
		else {
			if ((0.0 <= fVar7) && (fVar7 <= 1.0)) {
				uVar5 = 0xffffffff;
				fVar8 = (fVar8 + fVar7) * 0.5;
			}
		}
		if (0.0 <= fVar8) {
			peVar1 = pPrimRaySphereIn->field_0x4;
			fVar6 = peVar1->x;
			fVar9 = peVar1->y;
			fVar7 = peVar1->z;
			fVar10 = peVar1->w;
			peVar1 = pPrimRaySphereIn->field_0x0;
			fVar11 = peVar1->x;
			fVar12 = peVar1->y;
			fVar13 = peVar1->z;
			fVar14 = peVar1->w;
			pColInfoOut->field_0x44 = fVar8 - 1.0;
			(pColInfoOut->field_0x20).x = fVar6 * fVar8 + fVar11;
			(pColInfoOut->field_0x20).y = fVar9 * fVar8 + fVar12;
			(pColInfoOut->field_0x20).z = fVar7 * fVar8 + fVar13;
			(pColInfoOut->field_0x20).w = fVar10 * fVar8 + fVar14;
			pColInfoOut->result = 1;
		}
	}
	return uVar5;
}

static int _gtri_edge[3][2] = {
	{0, 1}, {1, 2}, {2, 0}
};

float FLOAT_004485b0 = 0.1f;

void edColGetNormalInWorldFromLocal(edF32VECTOR4* param_1, edF32MATRIX4* param_2, edF32VECTOR4* param_3)
{
	edF32MATRIX4 eStack64;

	edF32Matrix4GetTransposeHard(&eStack64, param_2);
	edF32Matrix4MulF32Vector4Hard(param_1, &eStack64, param_3);
	param_1->w = 0.0f;
	edF32Vector4NormalizeHard_Fixed(param_1, param_1);
	return;
}

void edColGetWorldVelocity
(edF32VECTOR4* param_1, edF32VECTOR4* param_2, edF32VECTOR4* param_3, edF32VECTOR4* param_4, edF32VECTOR4* param_5)
{
	float fVar1;
	float fVar2;
	float fVar3;
	float fVar4;
	float fVar5;
	float fVar6;
	float fVar7;
	float fVar8;
	float fVar9;

	fVar1 = param_2->x - param_3->x;
	fVar2 = param_2->y - param_3->y;
	fVar3 = param_2->z - param_3->z;
	fVar4 = param_5->x;
	fVar5 = param_5->y;
	fVar7 = param_5->z;
	fVar6 = param_4->y;
	fVar8 = param_4->z;
	fVar9 = param_4->w;
	param_1->x = param_4->x + (fVar5 * fVar3 - fVar2 * fVar7);
	param_1->y = fVar6 + (fVar7 * fVar1 - fVar3 * fVar4);
	param_1->z = fVar8 + (fVar4 * fVar2 - fVar1 * fVar5);
	param_1->w = fVar9 + in_vf0x;
	return;
}

bool edColIntersectSphereUnitTriangle4(edColINFO_OUT* pColInfoOut, edColPRIM_IN* pPrimIn, edF32TRIANGLE4* pTriangle)
{
	edColPRIM_OBJECT* m0;
	edF32VECTOR4* peVar1;
	float fVar2;
	float fVar3;
	float fVar4;
	bool bVar5;
	int iVar6;
	bool lVar7;
	int iVar8;
	int iVar9;
	float fVar10;
	edF32VECTOR4 local_140;
	edF32VECTOR4 local_130;
	edF32VECTOR4 eStack288;
	edF32VECTOR4 local_110;
	edColINFO_OUT eStack256;
	edF32VECTOR4 local_a0;
	edF32VECTOR4 local_90;
	edF32VECTOR4 local_80;
	edF32TRIANGLE4_INFOS local_70;
	float local_50;
	float fStack76;
	float fStack72;
	float fStack68;
	edF32VECTOR4 local_40;
	edF32VECTOR4 local_30;
	edColPRIM_RAY_SPHERE_UNIT_IN local_18;
	edColRAY_TRIANGLE4_IN local_10;
	float local_4;

	pColInfoOut->result = 0;
	m0 = pPrimIn->pPrim;
	fVar10 = edColSqrDistancePointTriangle(&gF32Vertex4Zero, pTriangle);
	if (fVar10 < 1.0f) {
		edColTriangle4GetInfo(&local_70, pTriangle);
		local_40.x = 0.0f - local_70.field_0x0.x;
		local_40.y = 0.0f - local_70.field_0x0.y;
		local_40.z = 0.0f - local_70.field_0x0.z;
		local_40.w = local_70.field_0x0.w;
		local_10.field_0x4 = &local_40;
		local_10.field_0x0 = &gF32Vertex4Zero;
		local_10.pTriangle = pTriangle;
		lVar7 = edColIntersectRayTriangle4(&local_4, &local_10);
		local_80.x = 0.0f;
		iVar8 = 0;
		local_80.y = 0.0f;
		local_80.z = 0.0f;
		local_80.w = 0.0f;
		if (lVar7 != false) {
			local_50 = local_40.x * local_4;
			fStack76 = local_40.y * local_4;
			fStack72 = local_40.z * local_4;
			fStack68 = local_40.w * local_4;
			if (local_50 * local_50 + fStack76 * fStack76 + fStack72 * fStack72 < 1.0) {
				local_80.x = local_50 + 0.0f;
				local_80.y = fStack76 + 0.0f;
				local_80.z = fStack72 + 0.0f;
				local_80.w = fStack68 + 0.0f;
				iVar8 = 1;
				pColInfoOut->field_0x50 = 2;
			}
		}

		iVar9 = 0;
		if (iVar8 == 0) {
			bVar5 = true;
			while (bVar5) {
				local_18.field_0x0 = LOAD_SECTION_CAST(edF32VECTOR4*, pTriangle->points[_gtri_edge[iVar9][0]]);
				peVar1 = LOAD_SECTION_CAST(edF32VECTOR4*, pTriangle->points[_gtri_edge[iVar9][1]]);
				local_a0 = *peVar1 - *local_18.field_0x0;
				local_18.field_0x4 = &local_a0;
				iVar6 = edColIntersectRaySphereUnit(&eStack256, &local_18);
				if (eStack256.result != 0) {
					fVar10 = eStack256.field_0x20.x;
					fVar2 = eStack256.field_0x20.y;
					fVar3 = eStack256.field_0x20.z;
					fVar4 = eStack256.field_0x20.w;
					if (-1 < iVar6) {
						peVar1 = LOAD_SECTION_CAST(edF32VECTOR4*, pTriangle->points[_gtri_edge[iVar9][iVar6]]);
						fVar10 = peVar1->x;
						fVar2 = peVar1->y;
						fVar3 = peVar1->z;
						fVar4 = peVar1->w;
					}
					local_80.w = local_80.w + fVar4;
					local_80.z = local_80.z + fVar3;
					local_80.y = local_80.y + fVar2;
					local_80.x = local_80.x + fVar10;
					iVar8 = iVar8 + 1;
					pColInfoOut->field_0x50 = 1;
				}
				iVar9 = iVar9 + 1;
				bVar5 = iVar9 < 3;
			}
		}

		bVar5 = false;
		if (iVar8 != 0) {
			if (1 < iVar8) {
				fVar10 = 1.0f / (float)iVar8;
				local_80.x = local_80.x * fVar10;
				local_80.y = local_80.y * fVar10;
				local_80.z = local_80.z * fVar10;
			}

			local_80.w = 1.0f;
			edF32Vector4NormalizeHard_Fixed(&local_90, &local_80);
			local_90.x = 0.0f - local_90.x;
			local_90.y = 0.0f - local_90.y;
			local_90.z = 0.0f - local_90.z;
			local_90.w = 0.0f;

			local_110 = local_80;

			edColGetNormalInWorldFromLocal(&local_90, &m0->matrix_0x40, &local_90);
			edF32Vector4NormalizeHard_Fixed(&eStack288, &local_80);
			eStack288.w = 1.0f;
			edF32Matrix4MulF32Vector4Hard(&local_80, &m0->matrix_0x0, &local_80);
			edF32Matrix4MulF32Vector4Hard(&local_130, &m0->matrix_0x0, &eStack288);
			local_140 = local_130 - local_80;

			fVar10 = edF32Vector4GetDistHard(&local_140);
			edF32Vector4NormalizeHard_Fixed(&local_110, &local_110);
			local_110.w = 1.0f;
			edF32Matrix4MulF32Vector4Hard(&local_80, &m0->matrix_0x0, &local_110);
			edColGetWorldVelocity(&local_30, &local_80, pPrimIn->field_0x10, pPrimIn->field_0x14, pPrimIn->field_0x18);
			if (local_30.x * local_90.x + local_30.y * local_90.y + local_30.z * local_90.z < FLOAT_004485b0) {
				bVar5 = true;
				pColInfoOut->field_0x20 = local_80;

				pColInfoOut->field_0x10 = local_90;

				pColInfoOut->field_0x30 = local_30;
				pColInfoOut->field_0x44 = -fVar10;
				pColInfoOut->field_0x48 = 0;
				pColInfoOut->field_0x4c = 1;
				pColInfoOut->result = 1;
			}
			else {
				bVar5 = false;
			}
		}
	}
	else {
		bVar5 = false;
	}
	return bVar5;
}

void edColComputeContactQuad4(edColOBJECT* pColObj, edColOBJECT* pOtherColObj, edColINFO_OUT* pColInfoOut, uint param_4, void* param_5, edColINFO* pColInfo, edF32QUAD4* pQuad, edF32TRIANGLE4* pTriangle)
{
	edF32VECTOR4* peVar1;
	int iVar2;
	edColINFO* peVar3;
	edF32VECTOR4* peVar4;
	edColDbObj_80* pColDbObj;
	float fVar5;
	float fVar6;
	float fVar7;
	edF32TRIANGLE4_INFOS local_90;
	edF32VECTOR4 local_70;
	edF32VECTOR4 local_60;
	edF32VECTOR4 local_40[2];

	if ((gColData.pActiveDatabase)->field_0x4 < (int)(uint)(ushort)gColConfig.field_0x8[gColData.activeDatabaseId + 2]) {
		pColDbObj = (gColData.pActiveDatabase)->field_0x10 + (gColData.pActiveDatabase)->field_0x4;
		pColDbObj->field_0x72 = 0;
		(gColData.pActiveDatabase)->field_0x4 = (gColData.pActiveDatabase)->field_0x4 + 1;
	}
	else {
		pColDbObj = (edColDbObj_80*)0x0;
	}

	if (pColDbObj != (edColDbObj_80*)0x0) {
		pColDbObj->pColObj = pColObj;
		pColDbObj->pOtherColObj = pOtherColObj;

		pColDbObj->field_0x30 = pColInfoOut->field_0x20;
		pColDbObj->field_0x40 = pColInfoOut->field_0x10;
		pColDbObj->location = pColInfoOut->location;

		pColDbObj->field_0x78 = pColInfoOut->field_0x44;
		pColDbObj->quadFlags = pQuad->flags;
		pColDbObj->field_0x73 = (char)pColInfoOut->field_0x50;
		pColDbObj->field_0x60 = (short)param_4;
		pColDbObj->field_0x64 = param_5;
		pColDbObj->field_0x62 = 8;
		pColDbObj->pQuad = pQuad;

		if (gColConfig.field_0x4 != 0) {
			edColTriangle4GetInfo(&local_90, pTriangle);
			pColDbObj->field_0x10 = pColInfoOut->field_0x30;

			if (pColInfo == (edColINFO*)0x0) {
				pColDbObj->field_0x70 = 1;
				pColDbObj->field_0x71 = 1;
			}
			else {
				pColDbObj->field_0x70 = pColInfo->field_0x4a;
				pColDbObj->field_0x71 = pColInfo->field_0x4b;
			}

			pColDbObj->field_0x6c = pColInfo;
			pColObj->field_0x6 = pColObj->field_0x6 + 1;

			if (pOtherColObj != (edColOBJECT*)0x0) {
				pOtherColObj->field_0x6 = pOtherColObj->field_0x6 + 1;
			}

			if (pColInfo != (edColINFO*)0x0) {
				pColInfo->field_0x49 = 1;
			}

			local_60 = local_90.field_0x0;

			peVar4 = LOAD_SECTION_CAST(edF32VECTOR4*, pTriangle->p1);
			peVar1 = LOAD_SECTION_CAST(edF32VECTOR4*, pTriangle->p3);

			local_70.xyz = peVar1->xyz - peVar4->xyz;
			local_70.w = 0.0f;
			edF32Vector4NormalizeHard(&local_70, &local_70);
			peVar4 = local_40;
			local_40[0].x = local_60.y * local_70.z - local_70.y * local_60.z;
			local_40[0].y = local_60.z * local_70.x - local_70.z * local_60.x;
			local_40[0].z = local_60.x * local_70.y - local_70.x * local_60.y;
			local_40[0].w = in_vf0x;
			local_40[1].x = local_60.x;
			local_40[1].y = local_60.y;
			local_40[1].z = local_60.z;
			local_40[1].w = local_60.w;

			if ((pColInfo != (edColINFO*)0x0) && ((pQuad->flags & 0x80000000) == 0)) {
				pColInfo->field_0x44 = 0x0;

				pColInfo->field_0x0.rowX = peVar4[0];
				pColInfo->field_0x0.rowY = peVar4[1];

				pColInfo->field_0x48 = 4;
				pColInfo->field_0x40 = STORE_SECTION(pTriangle);
				pColInfo->field_0x44 = pTriangle->flags;
			}
		}
	}
	return;
}

void edColIntersectSphereQuad4(edColINFO_OUT* pColInfoOut, edColPRIM_SPHERE_QUAD4_IN* pPrimSphereQuadIn)
{
	edF32QUAD4* peVar1;
	edColPRIM_OBJECT* pUVar2;
	bool bVar3;
	uint uVar4;
	edF32MATRIX4* m0;
	int iVar5;
	edF32TRIANGLE4_INFOS local_80;
	edF32TRIANGLE4 local_60;
	uint local_54;
	edF32TRIANGLE4 local_50;
	uint local_44;
	edF32VECTOR4 eStack64;
	edF32VECTOR4 eStack48;
	edF32VECTOR4 eStack32;
	edF32VECTOR4 eStack16;

	uVar4 = 0;
	peVar1 = pPrimSphereQuadIn->pQuad;
	m0 = &(pPrimSphereQuadIn->pPrim)->matrix_0x40;
	edF32Matrix4MulF32Vector4Hard(&eStack64, m0, (edF32VECTOR4*)LOAD_SECTION(peVar1->p1));
	edF32Matrix4MulF32Vector4Hard(&eStack48, m0, (edF32VECTOR4*)LOAD_SECTION(peVar1->p2));
	edF32Matrix4MulF32Vector4Hard(&eStack32, m0, (edF32VECTOR4*)LOAD_SECTION(peVar1->p3));
	edF32Matrix4MulF32Vector4Hard(&eStack16, m0, (edF32VECTOR4*)LOAD_SECTION(peVar1->p4));
	local_44 = peVar1->flags;
	
	local_60.p1 = STORE_SECTION(&eStack64);
	iVar5 = 0;
	local_54 = peVar1->flags;
	local_50.p1 = peVar1->p1;
	do {
		if (iVar5 == 0) {
			local_50.p2 = peVar1->p2;
			local_50.p3 = peVar1->p3;
			local_60.p2 = STORE_SECTION(&eStack48);
			local_60.p3 = STORE_SECTION(&eStack32);
		}
		else {
			local_50.p2 = peVar1->p3;
			local_60.p3 = STORE_SECTION(&eStack16);
			local_50.p3 = peVar1->p4;
			local_60.p2 = STORE_SECTION(&eStack32);
		}
		bVar3 = edColIntersectSphereUnitTriangle4(pColInfoOut, pPrimSphereQuadIn, &local_60);
		if (bVar3 != false) {
			edColTriangle4GetInfo(&local_80, &local_50);
			pColInfoOut->location = local_80.field_0x0;
			pUVar2 = pPrimSphereQuadIn->pPrim;
			edColComputeContactQuad4(pPrimSphereQuadIn->pColObj, pPrimSphereQuadIn->pOtherColObj, pColInfoOut, pPrimSphereQuadIn->aType, pUVar2, &pUVar2->colInfo, peVar1, &local_50);
			uVar4 = uVar4 | pColInfoOut->result;
		}
		iVar5 = iVar5 + 1;
	} while (iVar5 < 2);

	pColInfoOut->result = uVar4;
	return;
}


struct edColARRAY_PRIM_TRI4 {
	edColOBJECT* pColObj;
	int aCount;
	int aType;
	int primSize;
	void* aData;
	edColOBJECT* pOtherColObj;
	int bCount;
	void* bData;
};

struct edColARRAY_PRIM_PRIM {
	edColOBJECT* pColObj;
	int aCount;
	int aType;
	int primSize;
	void* aData;
	edColOBJECT* pOtherColObj;
	int bCount;
	int bType;
	int bPrimSize;
	void* bData;
};

struct edColARRAY_PRIM_QUAD4 {
	edColOBJECT* pColObj;
	int aCount;
	int aType;
	int primSize;
	void* aData;
	edColOBJECT* pOtherColObj;
	int bCount;
	void* bData;
};

struct edColPRIM_BOX_TRI4_IN {
	edColOBJECT* pColObj;
	edColOBJECT* pOtherColObj;
	edColPRIM_OBJECT* pData;
	int aType;
	edF32VECTOR4* field_0x10;
	edF32VECTOR4* field_0x14;
	edF32VECTOR4* field_0x18;
	edF32TRIANGLE4* pTriangle;
};

struct edColPRIM_SPHERE_TRI4_IN {
	edColOBJECT* pColObj;
	edColOBJECT* pOtherColObj;
	edColPRIM_OBJECT* pData;
	int aType;
	edF32VECTOR4* aCentre;
	edF32VECTOR4* field_0x14;
	edF32VECTOR4* field_0x18;
	edF32TRIANGLE4* pTriangle;
};

void edColComputeContactTriangle4(edColOBJECT* pColObj, edColOBJECT* pOtherColObj, edColINFO_OUT* pColInfoOut, uint param_4, void* param_5,
	edColINFO* pColInfo, edF32TRIANGLE4* pTriangle, int param_8)
{
	edF32VECTOR4* peVar1;
	edF32VECTOR4* peVar2;
	int iVar3;
	edColINFO* peVar4;
	float* pfVar5;
	edColDbObj_80* peVar6;
	float fVar7;
	float fVar8;
	float fVar9;
	undefined4 in_vf0x;
	edF32TRIANGLE4_INFOS local_90;
	edF32VECTOR4 local_70;
	edF32VECTOR4 local_60;
	edF32MATRIX4 local_40;

	if ((gColData.pActiveDatabase)->field_0x4 < gColConfig.field_0x8[gColData.activeDatabaseId + 2]) {
		peVar6 = (gColData.pActiveDatabase)->field_0x10 + (gColData.pActiveDatabase)->field_0x4;
		peVar6->field_0x72 = 0;
		(gColData.pActiveDatabase)->field_0x4 = (gColData.pActiveDatabase)->field_0x4 + 1;
	}
	else {
		peVar6 = (edColDbObj_80*)0x0;
	}

	if (peVar6 != (edColDbObj_80*)0x0) {
		peVar6->pColObj = pColObj;
		peVar6->pOtherColObj = pOtherColObj;

		peVar6->field_0x30 = pColInfoOut->field_0x20;

		peVar6->location = pColInfoOut->location;

		peVar6->field_0x40 = pColInfoOut->field_0x10;
		
		peVar6->field_0x78 = pColInfoOut->field_0x44;
		peVar6->quadFlags = pTriangle->flags;
		peVar6->field_0x73 = (char)pColInfoOut->field_0x50;

		if (param_8 == 0) {
			peVar6->field_0x60 = (short)param_4;
			peVar6->field_0x64 = param_5;
			peVar6->field_0x62 = 4;
			peVar6->pQuad = (edF32QUAD4*)pTriangle;
		}
		else {
			peVar6->field_0x62 = (short)param_4;
			peVar6->pQuad = (edF32QUAD4*)param_5;
			peVar6->field_0x60 = 4;
			peVar6->field_0x64 = pTriangle;
		}

		if (gColConfig.field_0x4 != 0) {
			edColTriangle4GetInfo(&local_90, pTriangle);
			peVar6->field_0x6c = pColInfo;
			if (pColInfo == (edColINFO*)0x0) {
				peVar6->field_0x70 = 1;
				peVar6->field_0x71 = 1;
			}
			else {
				peVar6->field_0x70 = pColInfo->field_0x4a;
				peVar6->field_0x71 = pColInfo->field_0x4b;
			}

			peVar6->field_0x10 = pColInfoOut->field_0x30;

			pColObj->field_0x6 = pColObj->field_0x6 + 1;
			if (pOtherColObj != (edColOBJECT*)0x0) {
				pOtherColObj->field_0x6 = pOtherColObj->field_0x6 + 1;
			}
			if (pColInfo != (edColINFO*)0x0) {
				pColInfo->field_0x49 = 1;
			}

			local_60 = local_90.field_0x0;

			peVar1 = LOAD_SECTION_CAST(edF32VECTOR4*, pTriangle->p1);
			peVar2 = LOAD_SECTION_CAST(edF32VECTOR4*, pTriangle->p3);
			local_70.x = peVar2->x - peVar1->x;
			local_70.y = peVar2->y - peVar1->y;
			local_70.z = peVar2->z - peVar1->z;
			local_70.w = 0.0f;

			edF32Vector4NormalizeHard(&local_70, &local_70);

			local_40.aa = local_60.y * local_70.z - local_70.y * local_60.z;
			local_40.ab = local_60.z * local_70.x - local_70.z * local_60.x;
			local_40.ac = local_60.x * local_70.y - local_70.x * local_60.y;
			local_40.ad = in_vf0x;
			local_40.rowY = local_60;
			local_40.rowZ = local_70;
			local_40.rowT = *LOAD_SECTION_CAST(edF32VECTOR4*, pTriangle->p1);

			if ((pColInfo != (edColINFO*)0x0) && ((pTriangle->flags & 0x80000000) == 0)) {
				pColInfo->field_0x44 = 0x0;

				pColInfo->field_0x0 = local_40;

				pColInfo->field_0x48 = 4;
				pColInfo->field_0x40 = STORE_SECTION(pTriangle);
				pColInfo->field_0x44 = pTriangle->flags;
			}
		}
	}
	return;
}

void edColIntersectSphereTriangle4(edColINFO_OUT* pColInfoOut, edColPRIM_SPHERE_TRI4_IN* pPrimSphereTriIn, int param_3)
{
	edF32TRIANGLE4* pTriangle;
	bool bVar1;
	edF32MATRIX4* m0;
	edF32TRIANGLE4_INFOS local_60;
	edF32TRIANGLE4 local_40;
	edF32VECTOR4 eStack48;
	edF32VECTOR4 eStack32;
	edF32VECTOR4 eStack16;

	pTriangle = pPrimSphereTriIn->pTriangle;
	m0 = &pPrimSphereTriIn->pData->matrix_0x40;
	edF32Matrix4MulF32Vector4Hard(&eStack48, m0, LOAD_SECTION_CAST(edF32VECTOR4*, pTriangle->p1));
	local_40.p2 = STORE_SECTION(&eStack32);
	edF32Matrix4MulF32Vector4Hard(LOAD_SECTION_CAST(edF32VECTOR4*, local_40.p2), m0, LOAD_SECTION_CAST(edF32VECTOR4*, pTriangle->p2));
	local_40.p3 = STORE_SECTION(&eStack16);
	edF32Matrix4MulF32Vector4Hard(LOAD_SECTION_CAST(edF32VECTOR4*, local_40.p3), m0, LOAD_SECTION_CAST(edF32VECTOR4*, pTriangle->p3));
	local_40.p1 = STORE_SECTION(&eStack48);
	local_40.flags = pTriangle->flags;

	bVar1 = edColIntersectSphereUnitTriangle4(pColInfoOut, (edColPRIM_IN*)pPrimSphereTriIn, &local_40);
	if (bVar1 != false) {
		edColTriangle4GetInfo(&local_60, pTriangle);
		pColInfoOut->location = local_60.field_0x0;
		edColComputeContactTriangle4(pPrimSphereTriIn->pColObj, pPrimSphereTriIn->pOtherColObj, pColInfoOut, pPrimSphereTriIn->aType,
			pPrimSphereTriIn->pData, &pPrimSphereTriIn->pData->colInfo, pTriangle, param_3);
	}
	return;
}

struct edColSEGMENT_TRIANGLE4_IN {
	edF32VECTOR4* field_0x0;
	edF32VECTOR4* field_0x4;
	edF32VECTOR4* field_0x8;
	edF32TRIANGLE4* field_0xc;
};

struct edColARRAY_TRI4_PRIM {
	edColOBJECT* pOtherColObj;
	uint bCount;
	int bType;
	int bSize;
	void* bData;
	edColOBJECT* pColObj;
	int bCount2;
	void* aData;
};

int triBoxOverlap(edF32TRIANGLE4* pTriangle)
{
	edF32VECTOR4* peVar1;
	edF32VECTOR4* peVar2;
	edF32VECTOR4* peVar3;
	bool bVar4;
	int iVar5;
	float fVar6;
	float fVar7;
	float fVar8;
	float fVar9;
	float fVar10;
	float fVar11;
	float fVar12;
	float local_60[4];
	float local_50[4];
	edF32VECTOR4 local_40;
	float local_30;
	float local_2c;
	float local_28;
	float fStack36;
	edF32VECTOR4 local_20;
	edF32VECTOR4 local_10;

	peVar1 = LOAD_SECTION_CAST(edF32VECTOR4*, pTriangle->p1);
	peVar2 = LOAD_SECTION_CAST(edF32VECTOR4*, pTriangle->p2);
	peVar3 = LOAD_SECTION_CAST(edF32VECTOR4*, pTriangle->p3);

	fVar8 = peVar2->x - peVar1->x;
	fVar9 = peVar2->y - peVar1->y;
	fVar7 = peVar2->z - peVar1->z;

	local_20.x = peVar3->x - peVar2->x;
	local_20.y = peVar3->y - peVar2->y;
	local_20.z = peVar3->z - peVar2->z;
	local_20.w = peVar3->w - peVar2->w;

	local_40.x = fVar9 * local_20.z - local_20.y * fVar7;
	local_40.y = fVar7 * local_20.x - local_20.z * fVar8;
	local_40.z = fVar8 * local_20.y - local_20.x * fVar9;

	edF32Vector4NormalizeHard_Fixed(&local_40, &local_40);

	fVar12 = -(local_40.x * peVar1->x + local_40.y * peVar1->y + local_40.z * peVar1->z);
	fVar6 = fVar7 * peVar1->y - fVar9 * peVar1->z;
	fVar10 = fVar7 * peVar3->y - fVar9 * peVar3->z;
	fVar11 = fVar10;

	if (fVar10 <= fVar6) {
		fVar11 = fVar6;
		fVar6 = fVar10;
	}

	fVar10 = fabs(fVar9) * 0.5f + fabs(fVar7) * 0.5f;
	if ((fVar10 < fVar6) || (fVar11 < -fVar10)) {
		iVar5 = 0;
	}
	else {
		fVar6 = -fVar7 * peVar1->x + fVar8 * peVar1->z;
		fVar10 = -fVar7 * peVar3->x + fVar8 * peVar3->z;
		fVar11 = fVar10;

		if (fVar10 <= fVar6) {
			fVar11 = fVar6;
			fVar6 = fVar10;
		}

		fVar7 = fabs(fVar8) * 0.5f + fabs(fVar7) * 0.5f;
		if ((fVar7 < fVar6) || (fVar11 < -fVar7)) {
			iVar5 = 0;
		}
		else {
			fVar6 = fVar9 * peVar2->x - fVar8 * peVar2->y;
			fVar11 = fVar9 * peVar3->x - fVar8 * peVar3->y;
			fVar7 = fVar6;
			if (fVar6 <= fVar11) {
				fVar7 = fVar11;
				fVar11 = fVar6;
			}

			fVar8 = fabs(fVar8) * 0.5f + fabs(fVar9) * 0.5f;
			if ((fVar8 < fVar11) || (fVar7 < -fVar8)) {
				iVar5 = 0;
			}
			else {
				fVar9 = local_20.z * peVar1->y - local_20.y * peVar1->z;
				fVar7 = local_20.z * peVar3->y - local_20.y * peVar3->z;
				fVar8 = fVar7;

				if (fVar7 <= fVar9) {
					fVar8 = fVar9;
					fVar9 = fVar7;
				}

				fVar7 = fabs(local_20.y) * 0.5f + fabs(local_20.z) * 0.5f;
				if ((fVar7 < fVar9) || (fVar8 < -fVar7)) {
					iVar5 = 0;
				}
				else {
					fVar9 = -local_20.z * peVar1->x + local_20.x * peVar1->z;
					fVar7 = -local_20.z * peVar3->x + local_20.x * peVar3->z;
					fVar8 = fVar7;
					if (fVar7 <= fVar9) {
						fVar8 = fVar9;
						fVar9 = fVar7;
					}

					fVar7 = fabs(local_20.x) * 0.5f + fabs(local_20.z) * 0.5f;
					if ((fVar7 < fVar9) || (fVar8 < -fVar7)) {
						iVar5 = 0;
					}
					else {
						fVar9 = local_20.y * peVar1->x - local_20.x * peVar1->y;
						fVar7 = local_20.y * peVar2->x - local_20.x * peVar2->y;
						fVar8 = fVar7;
						if (fVar7 <= fVar9) {
							fVar8 = fVar9;
							fVar9 = fVar7;
						}

						fVar7 = fabs(local_20.x) * 0.5f + fabs(local_20.y) * 0.5f;
						if ((fVar7 < fVar9) || (fVar8 < -fVar7)) {
							iVar5 = 0;
						}
						else {
							local_30 = peVar1->x - peVar3->x;
							local_2c = peVar1->y - peVar3->y;
							local_28 = peVar1->z - peVar3->z;
							fStack36 = peVar1->w - peVar3->w;
							fVar9 = local_28 * peVar1->y - local_2c * peVar1->z;
							fVar7 = local_28 * peVar2->y - local_2c * peVar2->z;
							fVar8 = fVar7;
							if (fVar7 <= fVar9) {
								fVar8 = fVar9;
								fVar9 = fVar7;
							}

							fVar7 = fabs(local_2c) * 0.5f + fabs(local_28) * 0.5f;
							if ((fVar7 < fVar9) || (fVar8 < -fVar7)) {
								iVar5 = 0;
							}
							else {
								fVar9 = -local_28 * peVar1->x + local_30 * peVar1->z;
								fVar7 = -local_28 * peVar2->x + local_30 * peVar2->z;
								fVar8 = fVar7;
								if (fVar7 <= fVar9) {
									fVar8 = fVar9;
									fVar9 = fVar7;
								}

								fVar7 = fabs(local_30) * 0.5f + fabs(local_28) * 0.5f;
								if ((fVar7 < fVar9) || (fVar8 < -fVar7)) {
									iVar5 = 0;
								}
								else {
									fVar7 = local_2c * peVar2->x - local_30 * peVar2->y;
									fVar9 = local_2c * peVar3->x - local_30 * peVar3->y;
									fVar8 = fVar7;
									if (fVar7 <= fVar9) {
										fVar8 = fVar9;
										fVar9 = fVar7;
									}

									fVar7 = fabs(local_30) * 0.5f + fabs(local_2c) * 0.5f;
									if ((fVar7 < fVar9) || (fVar8 < -fVar7)) {
										iVar5 = 0;
									}
									else {
										fVar9 = peVar1->x;
										fVar8 = peVar2->x;
										if (fVar9 <= fVar8) {
											fVar8 = fVar9;
										}
										fVar7 = peVar2->x;
										if (fVar7 <= fVar9) {
											fVar7 = fVar9;
										}
										fVar9 = peVar3->x;
										if (fVar8 <= fVar9) {
											fVar9 = fVar8;
										}
										fVar8 = peVar3->x;
										if (fVar8 <= fVar7) {
											fVar8 = fVar7;
										}

										if ((0.5f < fVar9) || (fVar8 < -0.5f)) {
											iVar5 = 0;
										}
										else {
											fVar9 = peVar1->y;
											fVar8 = peVar2->y;
											if (fVar9 <= fVar8) {
												fVar8 = fVar9;
											}
											fVar7 = peVar2->y;
											if (fVar7 <= fVar9) {
												fVar7 = fVar9;
											}
											fVar9 = peVar3->y;
											if (fVar8 <= fVar9) {
												fVar9 = fVar8;
											}
											fVar8 = peVar3->y;
											if (fVar8 <= fVar7) {
												fVar8 = fVar7;
											}

											if ((0.5f < fVar9) || (fVar8 < -0.5f)) {
												iVar5 = 0;
											}
											else {
												fVar9 = peVar1->z;
												fVar8 = peVar2->z;
												if (fVar9 <= fVar8) {
													fVar8 = fVar9;
												}
												fVar7 = peVar2->z;
												if (fVar7 <= fVar9) {
													fVar7 = fVar9;
												}
												fVar9 = peVar3->z;
												if (fVar8 <= fVar9) {
													fVar9 = fVar8;
												}
												fVar8 = peVar3->z;
												if (fVar8 <= fVar7) {
													fVar8 = fVar7;
												}

												if ((0.5f < fVar9) || (fVar8 < -0.5f)) {
													iVar5 = 0;
												}
												else {
													for (iVar5 = 0; iVar5 < 3; iVar5 = iVar5 + 1) {
														if (0.0f < (&local_40.x)[iVar5]) {
															local_50[iVar5] = -0.5f;
															local_60[iVar5] = 0.5f;
														}
														else {
															local_50[iVar5] = 0.5f;
															local_60[iVar5] = -0.5f;
														}
													}

													if (0.0f < fVar12 + local_40.x * local_50[0] + local_40.y * local_50[1] +
														local_40.z * local_50[2]) {
														bVar4 = false;
													}
													else {
														bVar4 = false;
														if (0.0f <= fVar12 + local_40.x * local_60[0] + local_40.y * local_60[1] +
															local_40.z * local_60[2]) {
															bVar4 = true;
														}
													}

													if (bVar4) {
														iVar5 = 1;
													}
													else {
														iVar5 = 0;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return iVar5;
}

int _gcube_edge[12][2] = {
	{ 0, 1 },
	{ 1, 3 },
	{ 3, 2 },
	{ 2, 0 },
	{ 4, 5 },
	{ 5, 7 },
	{ 7, 6 },
	{ 6, 4 },
	{ 0, 4 },
	{ 1, 5 },
	{ 2, 6 },
	{ 3, 7 },
};


edF32VECTOR4 _gcube_tri_normal[8] = {
	{ 0.5f,  0.5f,  0.5f, 1.0f},
	{ 0.5f,  0.5f, -0.5f, 1.0f},
	{ 0.5f, -0.5f,  0.5f, 1.0f},
	{ 0.5f, -0.5f, -0.5f, 1.0f},
	{-0.5f,  0.5f,  0.5f, 1.0f},
	{-0.5f,  0.5f, -0.5f, 1.0f},
	{-0.5f, -0.5f,  0.5f, 1.0f},
	{-0.5f, -0.5f, -0.5f, 1.0f}
};

int _gcube_tri[12][3] = {
	{ 0x0, 0x2, 0x3 },
	{ 0x0, 0x3, 0x1 },
	{ 0x4, 0x6, 0x2 },
	{ 0x4, 0x2, 0x0 },
	{ 0x5, 0x7, 0x6 },
	{ 0x5, 0x6, 0x4 },
	{ 0x1, 0x3, 0x7 },
	{ 0x1, 0x7, 0x5 },
	{ 0x0, 0x1, 0x5 },
	{ 0x0, 0x5, 0x4 },
	{ 0x2, 0x6, 0x7 },
	{ 0x2, 0x7, 0x3 },
};

void edColIntersectSegmentTriangle4(edColINFO_OUT* pColInfoOut, edColSEGMENT_TRIANGLE4_IN* pSegmentTriangleIn)
{
	edF32TRIANGLE4* peVar1;
	edF32VECTOR4* peVar2;
	edF32VECTOR4* peVar3;
	float fVar4;
	float fVar5;
	float fVar6;
	float fVar7;
	float fVar8;
	float fVar9;
	float fVar10;
	float fVar11;
	float fVar12;
	uint uVar13;
	float fVar14;
	float fVar15;
	float fVar16;
	float fVar17;
	float fVar18;
	float fVar19;
	float fVar20;
	float fVar21;
	float fVar22;
	float local_50;
	float fStack76;
	float fStack72;

	uVar13 = 0;
	pColInfoOut->field_0x44 = FLOAT_004485a4;
	pColInfoOut->field_0x4c = 0;
	pColInfoOut->result = 0;
	peVar1 = pSegmentTriangleIn->field_0xc;
	peVar2 = LOAD_SECTION_CAST(edF32VECTOR4*, peVar1->p1);
	peVar3 = LOAD_SECTION_CAST(edF32VECTOR4*, peVar1->p2);
	fVar15 = peVar3->x - peVar2->x;
	fVar5 = peVar3->y - peVar2->y;
	fVar9 = peVar3->z - peVar2->z;
	peVar2 = LOAD_SECTION_CAST(edF32VECTOR4*, peVar1->p1);
	peVar3 = LOAD_SECTION_CAST(edF32VECTOR4*, peVar1->p3);
	fVar16 = peVar3->x - peVar2->x;
	fVar6 = peVar3->y - peVar2->y;
	fVar10 = peVar3->z - peVar2->z;
	peVar2 = pSegmentTriangleIn->field_0x0;
	peVar3 = pSegmentTriangleIn->field_0x4;
	fVar17 = peVar3->w;
	fVar19 = peVar2->w;
	fVar18 = peVar3->x - peVar2->x;
	fVar7 = peVar3->y - peVar2->y;
	fVar11 = peVar3->z - peVar2->z;
	(pColInfoOut->field_0x10).x = fVar5 * fVar10 - fVar6 * fVar9;
	(pColInfoOut->field_0x10).y = fVar9 * fVar16 - fVar10 * fVar15;
	(pColInfoOut->field_0x10).z = fVar15 * fVar6 - fVar16 * fVar5;
	(pColInfoOut->field_0x10).w = in_vf0x;
	fVar20 = fVar7 * fVar10 - fVar6 * fVar11;
	fVar21 = fVar11 * fVar16 - fVar10 * fVar18;
	fVar22 = fVar18 * fVar6 - fVar16 * fVar7;
	fVar14 = fVar15 * fVar20 + fVar5 * fVar21 + fVar9 * fVar22;

	if (FLOAT_0044854c < fVar14) {
		peVar2 = LOAD_SECTION_CAST(edF32VECTOR4*, peVar1->p1);
		peVar3 = pSegmentTriangleIn->field_0x0;
		fVar4 = peVar3->x - peVar2->x;
		fVar8 = peVar3->y - peVar2->y;
		fVar12 = peVar3->z - peVar2->z;
		fVar20 = fVar4 * fVar20 + fVar8 * fVar21 + fVar12 * fVar22;

		if (fVar20 < 0.0f) {
			return;
		}

		if (fVar14 < fVar20) {
			return;
		}

		local_50 = fVar8 * fVar9 - fVar5 * fVar12;
		fStack76 = fVar12 * fVar15 - fVar9 * fVar4;
		fStack72 = fVar4 * fVar5 - fVar15 * fVar8;
		fVar15 = fVar18 * local_50 + fVar7 * fStack76 + fVar11 * fStack72;

		if (fVar15 < 0.0f) {
			return;
		}

		if (fVar14 < fVar20 + fVar15) {
			return;
		}
	}
	else {
		if (g_DefaultNearClip_0044851c <= fVar14) {
			return;
		}

		peVar2 = LOAD_SECTION_CAST(edF32VECTOR4*, peVar1->p1);
		peVar3 = pSegmentTriangleIn->field_0x0;
		fVar4 = peVar3->x - peVar2->x;
		fVar8 = peVar3->y - peVar2->y;
		fVar12 = peVar3->z - peVar2->z;
		fVar20 = fVar4 * fVar20 + fVar8 * fVar21 + fVar12 * fVar22;

		if (0.0f < fVar20) {
			return;
		}

		if (fVar20 < fVar14) {
			return;
		}

		local_50 = fVar8 * fVar9 - fVar5 * fVar12;
		fStack76 = fVar12 * fVar15 - fVar9 * fVar4;
		fStack72 = fVar4 * fVar5 - fVar15 * fVar8;
		fVar15 = fVar18 * local_50 + fVar7 * fStack76 + fVar11 * fStack72;

		if (0.0f < fVar15) {
			return;
		}

		if (fVar20 + fVar15 < fVar14) {
			return;
		}
	}

	fVar14 = (fVar16 * local_50 + fVar6 * fStack76 + fVar10 * fStack72) / fVar14;

	if (fVar14 != -8888.0f) {
		edF32Vector4NormalizeHard_Fixed(&pColInfoOut->field_0x10, &pColInfoOut->field_0x10);

		peVar2 = LOAD_SECTION_CAST(edF32VECTOR4*, peVar1->p1);
		peVar3 = pSegmentTriangleIn->field_0x0;
		pColInfoOut->field_0x44 =
			(peVar3->x - peVar2->x) * (pColInfoOut->field_0x10).x + (peVar3->y - peVar2->y) * (pColInfoOut->field_0x10).y +
			(peVar3->z - peVar2->z) * (pColInfoOut->field_0x10).z;

		(pColInfoOut->field_0x20).x = fVar18 * fVar14;
		(pColInfoOut->field_0x20).y = fVar7 * fVar14;
		(pColInfoOut->field_0x20).z = fVar11 * fVar14;
		(pColInfoOut->field_0x20).w = (fVar17 - fVar19) * fVar14;

		peVar2 = pSegmentTriangleIn->field_0x0;
		fVar15 = peVar2->y;
		fVar16 = peVar2->z;
		fVar18 = peVar2->w;
		(pColInfoOut->field_0x20).x = peVar2->x + (pColInfoOut->field_0x20).x;
		(pColInfoOut->field_0x20).y = fVar15 + (pColInfoOut->field_0x20).y;
		(pColInfoOut->field_0x20).z = fVar16 + (pColInfoOut->field_0x20).z;
		(pColInfoOut->field_0x20).w = fVar18 + (pColInfoOut->field_0x20).w;

		peVar2 = pSegmentTriangleIn->field_0x8;
		fVar18 = peVar2->y;
		fVar15 = peVar2->z;
		fVar16 = peVar2->w;
		(pColInfoOut->field_0x30).x = peVar2->x;
		(pColInfoOut->field_0x30).y = fVar18;
		(pColInfoOut->field_0x30).z = fVar15;
		(pColInfoOut->field_0x30).w = fVar16;

		if ((0.0f <= fVar14) && (fVar14 <= 1.0f)) {
			uVar13 = 1;
		}
		pColInfoOut->field_0x4c = 1;
		pColInfoOut->result = uVar13;
	}

	return;
}

void edColIntersectBoxTriangle4(edColINFO_OUT* pColInfoOut, edColPRIM_BOX_TRI4_IN* pParams, int param_3)
{
	edColPRIM_OBJECT* m0;
	edF32TRIANGLE4* pTriangle;
	int iVar1;
	int iVar2;
	int iVar3;
	edF32MATRIX4* m0_00;
	float fVar4;
	edF32TRIANGLE4_INFOS local_1d0;
	edF32VECTOR4 local_1b0;
	edF32TRIANGLE4_INFOS eStack416;
	edF32TRIANGLE4 local_180;
	edColINFO_OUT eStack368;
	edColSEGMENT_TRIANGLE4_IN local_110;
	edColINFO_OUT eStack256;
	edColSEGMENT_TRIANGLE4_IN local_a0;
	edF32VECTOR4 local_90;
	edF32VECTOR4 local_80;
	edF32TRIANGLE4_INFOS local_70;
	edF32TRIANGLE4 local_50;
	edF32VECTOR4 aeStack64[3];
	edF32VECTOR4 local_10;

	local_50.p2 = STORE_SECTION(aeStack64 + 1);
	local_50.p3 = STORE_SECTION(aeStack64 + 2);
	pColInfoOut->result = 0;
	local_1b0.w = 0.0f;
	m0 = pParams->pData;
	local_50.p1 = STORE_SECTION(aeStack64);
	pTriangle = pParams->pTriangle;
	m0_00 = &m0->matrix_0x40;
	local_50.flags = pTriangle->flags;

	edF32Matrix4MulF32Vector4Hard(aeStack64 + 0, m0_00, LOAD_SECTION_CAST(edF32VECTOR4*, pTriangle->p1));
	edF32Matrix4MulF32Vector4Hard(aeStack64 + 1, m0_00, LOAD_SECTION_CAST(edF32VECTOR4*, pTriangle->p2));
	edF32Matrix4MulF32Vector4Hard(aeStack64 + 2, m0_00, LOAD_SECTION_CAST(edF32VECTOR4*, pTriangle->p3));

	iVar1 = triBoxOverlap(&local_50);
	if (iVar1 != 0) {
		edColTriangle4GetInfo(&local_70, &local_50);
		iVar1 = 0;
		for (iVar2 = 0; iVar2 < 0xc; iVar2 = iVar2 + 1) {
			local_a0.field_0x0 = _gcube_tri_normal + _gcube_edge[iVar2][0];
			local_a0.field_0xc = &local_50;
			local_a0.field_0x8 = &gF32Vector4Zero;
			local_a0.field_0x4 = _gcube_tri_normal + _gcube_edge[iVar2][1];
			edColIntersectSegmentTriangle4(&eStack256, &local_a0);
			if (eStack256.result != 0) {
				iVar3 = _gcube_edge[iVar2][0];
				fVar4 = local_70.field_0x10 +
					_gcube_tri_normal[iVar3].x * local_70.field_0x0.x + _gcube_tri_normal[iVar3].y * local_70.field_0x0.y +
					_gcube_tri_normal[iVar3].z * local_70.field_0x0.z;
				if (fVar4 < 0.0) {
					if (fVar4 < local_1b0.w) {
						local_80.x = _gcube_tri_normal[iVar3].x;
						local_80.y = _gcube_tri_normal[iVar3].y;
						iVar1 = iVar1 + 1;
						local_80.z = _gcube_tri_normal[iVar3].z;
						local_80.w = _gcube_tri_normal[iVar3].w;
						local_90.x = local_70.field_0x0.x;
						local_90.y = local_70.field_0x0.y;
						local_90.z = local_70.field_0x0.z;
						local_90.w = local_70.field_0x0.w;
						pColInfoOut->field_0x50 = 2;
						local_1b0.w = fVar4;
					}
				}
				else {
					iVar3 = _gcube_edge[iVar2][1];
					fVar4 = local_70.field_0x10 +
						_gcube_tri_normal[iVar3].x * local_70.field_0x0.x + _gcube_tri_normal[iVar3].y * local_70.field_0x0.y
						+ _gcube_tri_normal[iVar3].z * local_70.field_0x0.z;
					if (fVar4 < local_1b0.w) {
						local_80.x = _gcube_tri_normal[iVar3].x;
						local_80.y = _gcube_tri_normal[iVar3].y;
						iVar1 = iVar1 + 1;
						local_80.z = _gcube_tri_normal[iVar3].z;
						local_80.w = _gcube_tri_normal[iVar3].w;
						local_90.x = local_70.field_0x0.x;
						local_90.y = local_70.field_0x0.y;
						local_90.z = local_70.field_0x0.z;
						local_90.w = local_70.field_0x0.w;
						pColInfoOut->field_0x50 = 2;
						local_1b0.w = fVar4;
					}
				}
			}
		}
		if (iVar1 == 0) {
			for (iVar2 = 0; iVar2 < 3; iVar2 = iVar2 + 1) {
				for (iVar3 = 0; iVar3 < 0xc; iVar3 = iVar3 + 1) {
					local_180.p1 = STORE_SECTION(_gcube_tri_normal + _gcube_tri[iVar3][0]);
					local_180.p2 = STORE_SECTION(_gcube_tri_normal + _gcube_tri[iVar3][1]);
					local_180.p3 = STORE_SECTION(_gcube_tri_normal + _gcube_tri[iVar3][2]);
					edColTriangle4GetInfo(&eStack416, &local_180);
					local_110.field_0xc = &local_180;
					local_110.field_0x0 = aeStack64 + _gtri_edge[iVar2][0];
					local_110.field_0x8 = &gF32Vector4Zero;
					local_110.field_0x4 = aeStack64 + _gtri_edge[iVar2][1];
					edColIntersectSegmentTriangle4(&eStack368, &local_110);
					if (eStack368.result != 0) {
						local_80.x = eStack368.field_0x20.x;
						local_80.y = eStack368.field_0x20.y;
						local_80.z = eStack368.field_0x20.z;
						local_80.w = eStack368.field_0x20.w;

						local_90.x = local_70.field_0x0.x;
						local_90.y = local_70.field_0x0.y;
						local_90.z = local_70.field_0x0.z;
						local_90.w = local_70.field_0x0.w;

						iVar1 = iVar1 + 1;
						local_1b0.w = local_70.field_0x10 + eStack368.field_0x20.x * local_70.field_0x0.x + eStack368.field_0x20.y * local_70.field_0x0.y + eStack368.field_0x20.z * local_70.field_0x0.z;
						pColInfoOut->field_0x50 = 1;
					}
				}
			}
		}
		if (iVar1 != 0) {
			edF32Matrix4MulF32Vector4Hard(&local_80, &m0->matrix_0x0, &local_80);

			local_1b0.x = local_90.x * local_1b0.w;
			local_1b0.y = local_90.y * local_1b0.w;
			local_1b0.z = local_90.z * local_1b0.w;
			local_1b0.w = local_90.w * local_1b0.w;

			edF32Matrix4MulF32Vector4Hard(&local_1b0, &m0->matrix_0x0, &local_1b0);
			edColGetNormalInWorldFromLocal(&local_90, m0_00, &local_90);
			edColGetWorldVelocity(&local_10, &local_80, pParams->field_0x10, pParams->field_0x14, pParams->field_0x18);

			if (local_10.x * local_90.x + local_10.y * local_90.y + local_10.z * local_90.z < FLOAT_004485b0) {
				(pColInfoOut->field_0x20).x = local_80.x;
				(pColInfoOut->field_0x20).y = local_80.y;
				(pColInfoOut->field_0x20).z = local_80.z;
				(pColInfoOut->field_0x20).w = local_80.w;

				(pColInfoOut->field_0x10).x = local_90.x;
				(pColInfoOut->field_0x10).y = local_90.y;
				(pColInfoOut->field_0x10).z = local_90.z;
				(pColInfoOut->field_0x10).w = local_90.w;

				(pColInfoOut->field_0x30).x = local_10.x;
				(pColInfoOut->field_0x30).y = local_10.y;
				(pColInfoOut->field_0x30).z = local_10.z;
				(pColInfoOut->field_0x30).w = local_10.w;

				fVar4 = edF32Vector4GetDistHard(&local_1b0);
				pColInfoOut->field_0x44 = -fVar4;
				pColInfoOut->field_0x48 = 0;
				pColInfoOut->field_0x4c = 1;
				pColInfoOut->result = 1;

				edColTriangle4GetInfo(&local_1d0, pTriangle);

				(pColInfoOut->location).x = local_1d0.field_0x0.x;
				(pColInfoOut->location).y = local_1d0.field_0x0.y;
				(pColInfoOut->location).z = local_1d0.field_0x0.z;
				(pColInfoOut->location).w = local_1d0.field_0x0.w;

				edColComputeContactTriangle4(pParams->pColObj, pParams->pOtherColObj, pColInfoOut, pParams->aType, pParams->pData, &pParams->pData->colInfo, pParams->pTriangle, param_3);
			}
		}
	}
	return;
}

uint edColArrayObjectPrimPenatratingArrayTriangles4(edColARRAY_PRIM_TRI4* pParams)
{
	int iVar1;
	int iVar2;
	edF32VECTOR4* peVar3;
	int iVar4;
	edColOBJECT* peVar5;
	uint uVar6;
	edF32VECTOR4* peVar7;
	edColPRIM_OBJECT* pUVar8;
	edColPRIM_OBJECT* pUVar9;
	edColINFO_OUT eStack160;
	edColPRIM_SPHERE_TRI4_IN local_40;
	edColPRIM_BOX_TRI4_IN local_20;

	iVar1 = pParams->primSize;
	uVar6 = 0;
	iVar2 = pParams->bCount;
	peVar3 = (edF32VECTOR4*)pParams->bData;
	pUVar8 = (edColPRIM_OBJECT*)pParams->aData;
	iVar4 = pParams->aType;
	peVar5 = pParams->pColObj;
	pUVar9 = (edColPRIM_OBJECT*)((char*)pUVar8 + pParams->aCount * iVar1);
	if ((iVar4 == 0xa) || (iVar4 == 0xd)) {
		local_20.pOtherColObj = pParams->pOtherColObj;
		local_20.aType = pParams->aType;
		local_20.pColObj = peVar5;

		for (; pUVar8 < pUVar9; pUVar8 = (edColPRIM_OBJECT*)((char*)pUVar8 + iVar1)) {
			local_20.field_0x10 = &pUVar8->matrix_0x0.rowT;
			local_20.field_0x14 = &pUVar8->field_0xc0;
			local_20.field_0x18 = &pUVar8->field_0xd0;
			local_20.pData = pUVar8;

			for (peVar7 = peVar3; peVar7 < peVar3 + iVar2; peVar7 = peVar7 + 1) {
				local_20.pTriangle = (edF32TRIANGLE4*)peVar7;

				edColIntersectBoxTriangle4(&eStack160, &local_20, 0);

				uVar6 = uVar6 | eStack160.result;
				peVar5->colResult = uVar6;
			}
		}
	}
	else {
		if ((iVar4 == 0xb) || (iVar4 == 0xe)) {
			local_40.pOtherColObj = pParams->pOtherColObj;
			local_40.aType = pParams->aType;
			local_40.pColObj = peVar5;
			for (; pUVar8 < pUVar9; pUVar8 = (edColPRIM_OBJECT*)((char*)pUVar8 + iVar1)) {
				local_40.aCentre = &pUVar8->matrix_0x0.rowT;
				local_40.field_0x14 = &pUVar8->field_0xc0;
				local_40.field_0x18 = &pUVar8->field_0xd0;
				local_40.pData = pUVar8;

				for (peVar7 = peVar3; peVar7 < peVar3 + iVar2; peVar7 = peVar7 + 1) {
					local_40.pTriangle = (edF32TRIANGLE4*)peVar7;

					edColIntersectSphereTriangle4(&eStack160, &local_40, 0);

					uVar6 = uVar6 | eStack160.result;
					peVar5->colResult = uVar6;
				}
			}
		}
		else {
			uVar6 = 0;
		}
	}
	return uVar6;
}

void edColIntersectBoxQuad4(edColINFO_OUT* pColInfoOut, edColPRIM_BOX_QUAD4_IN* pPrimBoxQuadIn)
{
	edF32QUAD4* pQuad;
	edColPRIM_OBJECT* m0;
	edColPRIM_OBJECT* peVar1;
	int iVar2;
	int iVar3;
	int iVar4;
	edF32MATRIX4* m0_00;
	float fVar5;
	float fVar6;
	uint local_200;
	edF32TRIANGLE4_INFOS local_1f0;
	edF32VECTOR4 local_1d0;
	edF32TRIANGLE4_INFOS eStack448;
	edF32TRIANGLE4 local_1a0;
	edColINFO_OUT eStack400;
	edColSEGMENT_TRIANGLE4_IN local_130;
	edColINFO_OUT eStack288;
	edColSEGMENT_TRIANGLE4_IN local_c0;
	edF32TRIANGLE4_INFOS local_b0;
	edF32TRIANGLE4 local_90;
	edF32TRIANGLE4 local_80;
	edF32VECTOR4 local_70;
	edF32VECTOR4 local_60;
	edF32VECTOR4 eStack80;
	edF32VECTOR4 eStack64;
	edF32VECTOR4 eStack48;
	edF32VECTOR4 local_20;
	edF32VECTOR4* local_10[4];

	iVar3 = 0;
	pColInfoOut->result = 0;
	fVar6 = 0.0f;
	local_200 = 0;
	pQuad = pPrimBoxQuadIn->pQuad;
	m0 = pPrimBoxQuadIn->pPrim;
	m0_00 = &m0->matrix_0x40;
	edF32Matrix4MulF32Vector4Hard(&eStack80, m0_00, LOAD_SECTION_CAST(edF32VECTOR4*, pQuad->p1));
	edF32Matrix4MulF32Vector4Hard(&eStack64, m0_00, LOAD_SECTION_CAST(edF32VECTOR4*, pQuad->p2));
	edF32Matrix4MulF32Vector4Hard(&eStack48, m0_00, LOAD_SECTION_CAST(edF32VECTOR4*, pQuad->p3));
	edF32Matrix4MulF32Vector4Hard(&local_20, m0_00, LOAD_SECTION_CAST(edF32VECTOR4*, pQuad->p4));
	local_90.p1 = STORE_SECTION(&eStack80);
	local_80.p1 = pQuad->p1;
	local_10[0] = LOAD_SECTION_CAST(edF32VECTOR4*, local_90.p1);

	for (iVar4 = 0; iVar4 < 2; iVar4 = iVar4 + 1) {
		local_90.p2 = STORE_SECTION(&eStack48);

		if (iVar4 == 0) {
			local_90.p2 = STORE_SECTION(&eStack64);
			local_90.p3 = STORE_SECTION(&eStack48);
			local_80.p2 = pQuad->p2;
			local_80.p3 = pQuad->p3;
		}
		else {
			local_90.p3 = STORE_SECTION(&local_20);
			local_80.p2 = pQuad->p3;
			local_80.p3 = pQuad->p4;
		}

		local_10[1] = LOAD_SECTION_CAST(edF32VECTOR4*, local_90.p2);
		local_10[2] = LOAD_SECTION_CAST(edF32VECTOR4*, local_90.p3);

		iVar2 = triBoxOverlap(&local_90);
		if (iVar2 != 0) {
			edColTriangle4GetInfo(&local_b0, &local_90);

			for (iVar4 = 0; iVar4 < 0xc; iVar4 = iVar4 + 1) {
				local_c0.field_0x0 = _gcube_tri_normal + _gcube_edge[iVar4][0];
				local_c0.field_0xc = &local_90;
				local_c0.field_0x8 = &gF32Vector4Zero;
				local_c0.field_0x4 = _gcube_tri_normal + _gcube_edge[iVar4][1];
				edColIntersectSegmentTriangle4(&eStack288, &local_c0);
				if (eStack288.result != 0) {
					iVar2 = _gcube_edge[iVar4][0];
					fVar5 = local_b0.field_0x10 +
						_gcube_tri_normal[iVar2].x * local_b0.field_0x0.x + _gcube_tri_normal[iVar2].y * local_b0.field_0x0.y
						+ _gcube_tri_normal[iVar2].z * local_b0.field_0x0.z;
					if (fVar5 < 0.0) {
						if (fVar5 < fVar6) {
							local_60.x = _gcube_tri_normal[iVar2].x;
							local_60.y = _gcube_tri_normal[iVar2].y;
							iVar3 = iVar3 + 1;
							local_60.z = _gcube_tri_normal[iVar2].z;
							local_60.w = _gcube_tri_normal[iVar2].w;
							local_70.x = local_b0.field_0x0.x;
							local_70.y = local_b0.field_0x0.y;
							local_70.z = local_b0.field_0x0.z;
							local_70.w = local_b0.field_0x0.w;
							pColInfoOut->field_0x50 = 2;
							fVar6 = fVar5;
						}
					}
					else {
						iVar2 = _gcube_edge[iVar4][1];
						fVar5 = local_b0.field_0x10 +
							_gcube_tri_normal[iVar2].x * local_b0.field_0x0.x +
							_gcube_tri_normal[iVar2].y * local_b0.field_0x0.y +
							_gcube_tri_normal[iVar2].z * local_b0.field_0x0.z;
						if (fVar5 < fVar6) {
							local_60.x = _gcube_tri_normal[iVar2].x;
							local_60.y = _gcube_tri_normal[iVar2].y;
							iVar3 = iVar3 + 1;
							local_60.z = _gcube_tri_normal[iVar2].z;
							local_60.w = _gcube_tri_normal[iVar2].w;
							local_70.x = local_b0.field_0x0.x;
							local_70.y = local_b0.field_0x0.y;
							local_70.z = local_b0.field_0x0.z;
							local_70.w = local_b0.field_0x0.w;
							pColInfoOut->field_0x50 = 2;
							fVar6 = fVar5;
						}
					}
				}
			}
			if (iVar3 == 0) {
				for (iVar4 = 0; iVar4 < 3; iVar4 = iVar4 + 1) {
					for (iVar2 = 0; iVar2 < 0xc; iVar2 = iVar2 + 1) {
						local_1a0.p1 = STORE_SECTION(_gcube_tri_normal + _gcube_tri[iVar2][0]);
						local_1a0.p2 = STORE_SECTION(_gcube_tri_normal + _gcube_tri[iVar2][1]);
						local_1a0.p3 = STORE_SECTION(_gcube_tri_normal + _gcube_tri[iVar2][2]);
						edColTriangle4GetInfo(&eStack448, &local_1a0);
						local_130.field_0xc = &local_1a0;
						local_130.field_0x0 = local_10[_gtri_edge[iVar4][0]];
						local_130.field_0x4 = local_10[_gtri_edge[iVar4][1]];
						local_130.field_0x8 = &gF32Vector4Zero;
						edColIntersectSegmentTriangle4(&eStack400, &local_130);
						if (eStack400.result != 0) {
							local_60.x = eStack400.field_0x20.x;
							local_60.y = eStack400.field_0x20.y;
							local_60.z = eStack400.field_0x20.z;
							local_60.w = eStack400.field_0x20.w;
							local_70.x = local_b0.field_0x0.x;
							local_70.y = local_b0.field_0x0.y;
							local_70.z = local_b0.field_0x0.z;
							local_70.w = local_b0.field_0x0.w;
							iVar3 = iVar3 + 1;
							fVar6 = local_b0.field_0x10 +
								eStack400.field_0x20.x * local_b0.field_0x0.x + eStack400.field_0x20.y * local_b0.field_0x0.y +
								eStack400.field_0x20.z * local_b0.field_0x0.z;
							pColInfoOut->field_0x50 = 1;
						}
					}
				}
			}
			if (iVar3 != 0) {
				edF32Matrix4MulF32Vector4Hard(&local_60, (edF32MATRIX4*)m0, &local_60);
				local_1d0.x = local_70.x * fVar6;
				local_1d0.y = local_70.y * fVar6;
				local_1d0.z = local_70.z * fVar6;
				local_1d0.w = local_70.w * fVar6;
				edF32Matrix4MulF32Vector4Hard(&local_1d0, (edF32MATRIX4*)m0, &local_1d0);
				edColGetNormalInWorldFromLocal(&local_70, m0_00, &local_70);
				edColGetWorldVelocity
				(&local_20, &local_60, pPrimBoxQuadIn->field_0x10, pPrimBoxQuadIn->field_0x14,
					pPrimBoxQuadIn->field_0x18);
				if (local_20.x * local_70.x + local_20.y * local_70.y + local_20.z * local_70.z < FLOAT_004485b0) {
					(pColInfoOut->field_0x20).x = local_60.x;
					(pColInfoOut->field_0x20).y = local_60.y;
					(pColInfoOut->field_0x20).z = local_60.z;
					(pColInfoOut->field_0x20).w = local_60.w;
					(pColInfoOut->field_0x10).x = local_70.x;
					(pColInfoOut->field_0x10).y = local_70.y;
					(pColInfoOut->field_0x10).z = local_70.z;
					(pColInfoOut->field_0x10).w = local_70.w;
					(pColInfoOut->field_0x30).x = local_20.x;
					(pColInfoOut->field_0x30).y = local_20.y;
					(pColInfoOut->field_0x30).z = local_20.z;
					(pColInfoOut->field_0x30).w = local_20.w;
					fVar5 = edF32Vector4GetDistHard(&local_1d0);
					pColInfoOut->field_0x44 = -fVar5;
					pColInfoOut->field_0x48 = 0;
					pColInfoOut->field_0x4c = 1;
					pColInfoOut->result = 1;
					edColTriangle4GetInfo(&local_1f0, &local_80);
					(pColInfoOut->location).x = local_1f0.field_0x0.x;
					(pColInfoOut->location).y = local_1f0.field_0x0.y;
					(pColInfoOut->location).z = local_1f0.field_0x0.z;
					(pColInfoOut->location).w = local_1f0.field_0x0.w;
					peVar1 = pPrimBoxQuadIn->pPrim;
					edColComputeContactQuad4
					(pPrimBoxQuadIn->pColObj, pPrimBoxQuadIn->pOtherColObj, pColInfoOut,
						pPrimBoxQuadIn->aType, peVar1, &peVar1->colInfo, pQuad, &local_80);
					local_200 = local_200 | pColInfoOut->result;
				}
			}
		}
	}
	pColInfoOut->result = local_200;
	return;
}

uint edColArrayObjectPrimPenatratingArrayQuads4(edColARRAY_PRIM_QUAD4* pParams)
{
	int primSize;
	int nbQuads;
	edF32QUAD4* aQuads;
	edColOBJECT* pColObj;
	char* pCurrentPrim;
	uint result;
	edF32QUAD4* pCurrentQuad;
	char* pPrimEnd;
	edColPRIM_BOX_QUAD4_IN primBoxQuadIn;
	edColPRIM_SPHERE_QUAD4_IN primSphereQuadIn;
	edColINFO_OUT colInfoOut;

	result = 0;
	primSize = pParams->primSize;
	nbQuads = pParams->bCount;
	aQuads = (edF32QUAD4*)pParams->bData;
	pCurrentPrim = (char*)pParams->aData;
	pColObj = pParams->pColObj;
	pPrimEnd = pCurrentPrim + (pParams->aCount * primSize);

	if (pParams->aType == 0xd) {
		primBoxQuadIn.pOtherColObj = pParams->pOtherColObj;
		primBoxQuadIn.aType = pParams->aType;
		primBoxQuadIn.pColObj = pColObj;

		COLLISION_LOG(LogLevel::VeryVerbose, "edColArrayObjectPrimPenatratingArrayQuads4 box count: 0x{:x} quad count: 0x{:x}", pParams->aCount, nbQuads);

		for (; pCurrentPrim < pPrimEnd; pCurrentPrim = pCurrentPrim + primSize) {
			edColPRIM_OBJECT* pPrim = reinterpret_cast<edColPRIM_OBJECT*>(pCurrentPrim);
			primBoxQuadIn.field_0x10 = &(pPrim->matrix_0x0).rowT;
			primBoxQuadIn.field_0x14 = &pPrim->field_0xc0;
			primBoxQuadIn.field_0x18 = &pPrim->field_0xd0;
			primBoxQuadIn.pPrim = pPrim;

			for (pCurrentQuad = aQuads; pCurrentQuad < aQuads + nbQuads; pCurrentQuad = pCurrentQuad + 1) {
				primBoxQuadIn.pQuad = pCurrentQuad;
				edColIntersectBoxQuad4(&colInfoOut, &primBoxQuadIn);
				result = result | colInfoOut.result;

				COLLISION_LOG(LogLevel::VeryVerbose, "edColArrayObjectPrimPenatratingArrayQuads4 intersect result: 0x{:x} result: 0x{:x}", colInfoOut.result, result);
				pColObj->colResult = result;
			}
		}
	}
	else {
		if (pParams->aType == 0xe) {
			primSphereQuadIn.pOtherColObj = pParams->pOtherColObj;
			primSphereQuadIn.aType = pParams->aType;
			primSphereQuadIn.pColObj = pColObj;

			COLLISION_LOG(LogLevel::VeryVerbose, "edColArrayObjectPrimPenatratingArrayQuads4 sphere count: 0x{:x} quad count: 0x{:x}", pParams->aCount, nbQuads);

			for (; pCurrentPrim < pPrimEnd; pCurrentPrim = pCurrentPrim + primSize) {
				edColPRIM_OBJECT* pPrim = reinterpret_cast<edColPRIM_OBJECT*>(pCurrentPrim);
				primSphereQuadIn.field_0x10 = &(pPrim->matrix_0x0).rowT;
				primSphereQuadIn.field_0x14 = &pPrim->field_0xc0;
				primSphereQuadIn.field_0x18 = &pPrim->field_0xd0;
				primSphereQuadIn.pPrim = pPrim;

				COLLISION_LOG(LogLevel::VeryVerbose, "edColArrayObjectPrimPenatratingArrayQuads4 {} {} {}", 
					primSphereQuadIn.field_0x10->ToString(), primSphereQuadIn.field_0x14->ToString(), primSphereQuadIn.field_0x18->ToString());

				for (pCurrentQuad = aQuads; pCurrentQuad < aQuads + nbQuads; pCurrentQuad = pCurrentQuad + 1) {
					primSphereQuadIn.pQuad = pCurrentQuad;
					edColIntersectSphereQuad4(&colInfoOut, &primSphereQuadIn);
					result = result | colInfoOut.result;

					COLLISION_LOG(LogLevel::VeryVerbose, "edColArrayObjectPrimPenatratingArrayQuads4 intersect result: 0x{:x} result: 0x{:x}", colInfoOut.result, result);
					pColObj->colResult = result;
				}
			}
		}
		else {
			result = 0;
		}
	}
	return result;
}

struct edColPRIM_BOX_SPHERE_IN {
	edColOBJECT* pOtherColObj;
	edColPRIM_OBJECT* pPrimObj;
	int bType;
	edF32VECTOR4* bCentre;
	edF32VECTOR4* field_0x10;
	edF32VECTOR4* field_0x14;
	edColOBJECT* pColObj;
	edColPRIM_OBJECT* aData;
	int aType;
	edF32VECTOR4* aCentre;
	edF32VECTOR4* field_0x28;
	edF32VECTOR4* field_0x2c;
};

int edColIntersectSphereUnitTriangle4Box(edColINFO_OUT* pColInfoOut, edColPRIM_SPHERE_TRI4_IN* pParams)
{
	edColPRIM_OBJECT* m0;
	edF32VECTOR4* peVar1;
	bool bVar2;
	int iVar3;
	int iVar4;
	float fVar5;
	edF32VECTOR4 local_140;
	edF32VECTOR4 local_130;
	edF32VECTOR4 eStack288;
	edF32VECTOR4 local_110;
	edColINFO_OUT eStack256;
	edF32VECTOR4 local_a0;
	edF32VECTOR4 local_90;
	edF32VECTOR4 local_80;
	edF32TRIANGLE4_INFOS local_70;
	float local_50;
	float fStack76;
	float fStack72;
	float fStack68;
	edF32VECTOR4 local_40;
	edF32VECTOR4 local_30;
	edColPRIM_RAY_SPHERE_UNIT_IN local_18;
	edColRAY_TRIANGLE4_IN local_10;
	float local_4;

	pColInfoOut->result = 0;
	m0 = pParams->pData;
	fVar5 = edColSqrDistancePointTriangle(&gF32Vertex4Zero, pParams->pTriangle);
	iVar4 = 0;

	if (fVar5 < 1.0f) {
		edColTriangle4GetInfo(&local_70, pParams->pTriangle);
		local_40.x = 0.0f - local_70.field_0x0.x;
		local_40.y = 0.0f - local_70.field_0x0.y;
		local_40.z = 0.0f - local_70.field_0x0.z;
		local_40.w = local_70.field_0x0.w;
		local_10.field_0x4 = &local_40;
		local_10.field_0x0 = &gF32Vertex4Zero;
		local_10.pTriangle = pParams->pTriangle;

		bVar2 = edColIntersectRayTriangle4(&local_4, &local_10);
		local_80.x = 0.0f;
		iVar3 = 0;
		local_80.y = 0.0f;
		local_80.z = 0.0f;
		local_80.w = 0.0f;

		if (bVar2 != false) {
			local_50 = local_40.x * local_4;
			fStack76 = local_40.y * local_4;
			fStack72 = local_40.z * local_4;
			fStack68 = local_40.w * local_4;

			if (local_50 * local_50 + fStack76 * fStack76 + fStack72 * fStack72 < 1.0f) {
				local_80.x = local_50 + 0.0f;
				local_80.y = fStack76 + 0.0f;
				local_80.z = fStack72 + 0.0f;
				local_80.w = fStack68 + 0.0f;
				iVar3 = 1;
				pColInfoOut->field_0x50 = 2;
			}
		}

		iVar4 = 0;
		if (iVar3 == 0) {
			bVar2 = true;
			while (bVar2) {
				local_18.field_0x0 = LOAD_SECTION_CAST(edF32VECTOR4*, pParams->pTriangle->points[_gtri_edge[iVar4][0]]);
				peVar1 = LOAD_SECTION_CAST(edF32VECTOR4*, pParams->pTriangle->points[_gtri_edge[iVar4][1]]);
				local_a0.x = peVar1->x - (local_18.field_0x0)->x;
				local_a0.y = peVar1->y - (local_18.field_0x0)->y;
				local_a0.z = peVar1->z - (local_18.field_0x0)->z;
				local_a0.w = peVar1->w - (local_18.field_0x0)->w;
				local_18.field_0x4 = &local_a0;

				edColIntersectRaySphereUnit(&eStack256, &local_18);

				if (eStack256.result != 0) {
					local_80.x = local_80.x + eStack256.field_0x20.x;
					local_80.y = local_80.y + eStack256.field_0x20.y;
					local_80.z = local_80.z + eStack256.field_0x20.z;
					local_80.w = local_80.w + eStack256.field_0x20.w;
					iVar3 = iVar3 + 1;
					pColInfoOut->field_0x50 = 1;
				}

				iVar4 = iVar4 + 1;
				bVar2 = iVar4 < 3;
			}
		}

		iVar4 = 0;
		if (iVar3 != 0) {
			fVar5 = 1.0f / (float)iVar3;
			local_80.x = local_80.x * fVar5;
			local_80.y = local_80.y * fVar5;
			local_80.z = local_80.z * fVar5;
			local_80.w = 1.0f;
			edF32Vector4NormalizeHard_Fixed(&local_90, &local_80);
			local_90.x = 0.0f - local_90.x;
			local_90.y = 0.0f - local_90.y;
			local_90.z = 0.0f - local_90.z;
			local_90.w = 0.0f;
			local_110.x = local_80.x;
			local_110.y = local_80.y;
			local_110.z = local_80.z;
			local_110.w = local_80.w;
			edColGetNormalInWorldFromLocal(&local_90, &m0->matrix_0x40, &local_90);
			edF32Vector4NormalizeHard_Fixed(&eStack288, &local_80);
			eStack288.w = 1.0f;
			edF32Matrix4MulF32Vector4Hard(&local_80, &m0->matrix_0x0, &local_80);
			edF32Matrix4MulF32Vector4Hard(&local_130, &m0->matrix_0x0, &eStack288);
			local_140.x = local_130.x - local_80.x;
			local_140.y = local_130.y - local_80.y;
			local_140.z = local_130.z - local_80.z;
			local_140.w = local_130.w - local_80.w;
			fVar5 = edF32Vector4GetDistHard(&local_140);
			edF32Vector4NormalizeHard_Fixed(&local_110, &local_110);
			local_110.w = 1.0f;
			edF32Matrix4MulF32Vector4Hard(&local_80, &m0->matrix_0x0, &local_110);
			edColGetWorldVelocity(&local_30, &local_80, pParams->aCentre, pParams->field_0x14, pParams->field_0x18);
			if (local_30.x * local_90.x + local_30.y * local_90.y + local_30.z * local_90.z < FLOAT_004485b0) {
				iVar4 = 1;
				(pColInfoOut->field_0x20).x = local_80.x;
				(pColInfoOut->field_0x20).y = local_80.y;
				(pColInfoOut->field_0x20).z = local_80.z;
				(pColInfoOut->field_0x20).w = local_80.w;
				(pColInfoOut->field_0x10).x = local_90.x;
				(pColInfoOut->field_0x10).y = local_90.y;
				(pColInfoOut->field_0x10).z = local_90.z;
				(pColInfoOut->field_0x10).w = local_90.w;
				(pColInfoOut->field_0x30).x = local_30.x;
				(pColInfoOut->field_0x30).y = local_30.y;
				(pColInfoOut->field_0x30).z = local_30.z;
				(pColInfoOut->field_0x30).w = local_30.w;
				pColInfoOut->field_0x44 = -fVar5;
				pColInfoOut->field_0x48 = 0;
				pColInfoOut->field_0x4c = 1;
				pColInfoOut->result = 1;
			}
			else {
				iVar4 = 0;
			}
		}
	}
	return iVar4;
}

void edColComputeContactPrim(edColOBJECT* pColObjA, edColOBJECT* pColObjB, edColINFO_OUT* pColInfoOut, uint aType, void* pPrimA, edColINFO* pColInfo, uint bType, edColPRIM_OBJECT* pPrimB)
{
	int iVar1;
	edColINFO* peVar2;
	edF32MATRIX4* peVar3;
	edColDbObj_80* peVar4;
	float fVar5;
	float fVar6;
	float fVar7;
	edF32VECTOR4 local_70;
	edF32VECTOR4 local_60;
	edF32MATRIX4 local_40;

	if ((gColData.pActiveDatabase)->field_0x4 < gColConfig.field_0x8[gColData.activeDatabaseId + 2]) {
		peVar4 = (gColData.pActiveDatabase)->field_0x10 + (gColData.pActiveDatabase)->field_0x4;
		peVar4->field_0x72 = 0;
		(gColData.pActiveDatabase)->field_0x4 = (gColData.pActiveDatabase)->field_0x4 + 1;
	}
	else {
		peVar4 = (edColDbObj_80*)0x0;
	}

	if (peVar4 != (edColDbObj_80*)0x0) {
		peVar4->pColObj = pColObjA;
		peVar4->pOtherColObj = pColObjB;
	
		peVar4->field_0x30 = pColInfoOut->field_0x20;
		peVar4->field_0x40 = pColInfoOut->field_0x10;
		peVar4->location = pColInfoOut->location;

		peVar4->field_0x78 = pColInfoOut->field_0x44;
		peVar4->quadFlags = pPrimB->flags_0x80;
		peVar4->field_0x73 = (char)pColInfoOut->field_0x50;
		peVar4->field_0x60 = (short)aType;
		peVar4->field_0x64 = pPrimA;
		peVar4->field_0x62 = (short)bType;
		peVar4->pQuad = (edF32QUAD4*)pPrimB;

		if (gColConfig.field_0x4 != 0) {
			peVar4->field_0x10 = pColInfoOut->field_0x30;

			if (pColInfo == (edColINFO*)0x0) {
				peVar4->field_0x70 = 1;
				peVar4->field_0x71 = 1;
			}
			else {
				peVar4->field_0x70 = pColInfo->field_0x4a;
				peVar4->field_0x71 = pColInfo->field_0x4b;
			}

			peVar4->field_0x6c = pColInfo;
			pColObjA->field_0x6 = pColObjA->field_0x6 + 1;

			if (pColObjB != (edColOBJECT*)0x0) {
				pColObjB->field_0x6 = pColObjB->field_0x6 + 1;
			}

			if ((pColInfo != (edColINFO*)0x0) && ((pPrimB->flags_0x80 & 0x80000000) == 0)) {
				pColInfo->field_0x49 = 1;
				local_60.x = (pColInfoOut->field_0x10).x;
				local_70.x = (pColInfoOut->field_0x10).y;
				local_70.z = (pColInfoOut->field_0x10).z;
				local_60.w = (pColInfoOut->field_0x10).w;
				local_70.w = 0.0f;
				local_70.y = -local_60.x;
				local_60.y = local_70.x;
				local_60.z = local_70.z;
				edF32Vector4NormalizeHard(&local_70, &local_70);

				local_40.aa = local_60.y * local_70.z - local_70.y * local_60.z;
				local_40.ab = local_60.z * local_70.x - local_70.z * local_60.x;
				local_40.ac = local_60.x * local_70.y - local_70.x * local_60.y;
				local_40.ad = in_vf0x;
				local_40.ba = local_60.x;
				local_40.bb = local_60.y;
				local_40.bc = local_60.z;
				local_40.bd = local_60.w;
				local_40.ca = local_70.x;
				local_40.cb = local_70.y;
				local_40.cc = local_70.z;
				local_40.cd = local_70.w;
				local_40.da = (pColInfoOut->field_0x20).x;
				local_40.db = (pColInfoOut->field_0x20).y;
				local_40.dc = (pColInfoOut->field_0x20).z;
				local_40.dd = (pColInfoOut->field_0x20).w;
				pColInfo->field_0x44 = 0x0;

				peVar2->field_0x0 = local_40;


				pColInfo->field_0x48 = (byte)bType;
				pColInfo->field_0x40 = STORE_SECTION(pPrimB);
				pColInfo->field_0x44 = pPrimB->flags_0x80;
			}
		}
	}
	return;
}

void edColIntersectBoxSphere(edColINFO_OUT* pColInfoOut, edColPRIM_BOX_SPHERE_IN* pParams, int param_3)
{
	edF32VECTOR4* peVar1;
	int iVar2;
	edF32VECTOR4* peVar3;
	edF32VECTOR4* peVar4;
	int iVar5;
	uint uVar6;
	int iVar7;
	float fVar8;
	edF32VECTOR4 local_130;
	edF32VECTOR4 local_120;
	edColPRIM_SPHERE_TRI4_IN eStack272;
	edF32TRIANGLE4 local_f0;
	edF32VECTOR4 local_e0 = {};
	edF32VECTOR4 local_d0 = {};
	edF32VECTOR4 aeStack192[8];
	edF32MATRIX4 eStack64;

	uVar6 = 0;
	iVar5 = 0;

	edF32Matrix4MulF32Matrix4Hard(&eStack64, &pParams->pPrimObj->matrix_0x0, &pParams->aData->matrix_0x40);

#if 0
	peVar3 = (edF32VECTOR4*)&DAT_00000010;
	peVar4 = &local_d0;
	peVar1 = peVar4;
	while (peVar1 != (edF32VECTOR4*)0x0) {
		*(undefined*)&peVar4->x = 0;
		peVar4 = (edF32VECTOR4*)((int)&peVar4->x + 1);
		peVar3 = (edF32VECTOR4*)((int)&peVar3[-1].w + 3);
		peVar1 = peVar3;
	}
	peVar3 = (edF32VECTOR4*)&DAT_00000010;
	peVar4 = &local_e0;
	peVar1 = peVar4;
	while (peVar1 != (edF32VECTOR4*)0x0) {
		*(undefined*)&peVar4->x = 0;
		peVar4 = (edF32VECTOR4*)((int)&peVar4->x + 1);
		peVar3 = (edF32VECTOR4*)((int)&peVar3[-1].w + 3);
		peVar1 = peVar3;
	}
#endif

	for (iVar7 = 0; iVar7 < 8; iVar7 = iVar7 + 1) {
		edF32Matrix4MulF32Vector4Hard(aeStack192 + iVar7, &eStack64, _gcube_tri_normal + iVar7);
	}

	for (iVar7 = 0; iVar7 < 0xc; iVar7 = iVar7 + 1) {
		local_f0.flags = pParams->pPrimObj->flags_0x80;
		eStack272.pTriangle = &local_f0;

		local_f0.p1 = STORE_SECTION(aeStack192 + _gcube_tri[iVar7][0]);
		local_f0.p2 = STORE_SECTION(aeStack192 + _gcube_tri[iVar7][1]);
		local_f0.p3 = STORE_SECTION(aeStack192 + _gcube_tri[iVar7][2]);

		eStack272.aCentre = pParams->aCentre;
		eStack272.field_0x14 = pParams->field_0x28;
		eStack272.field_0x18 = pParams->field_0x2c;
		eStack272.pData = pParams->aData;

		iVar2 = edColIntersectSphereUnitTriangle4Box(pColInfoOut, &eStack272);

		if (iVar2 != 0) {
			iVar5 = iVar5 + 1;
			local_e0.x = local_e0.x + (pColInfoOut->field_0x10).x;
			local_e0.y = local_e0.y + (pColInfoOut->field_0x10).y;
			local_e0.z = local_e0.z + (pColInfoOut->field_0x10).z;
			local_e0.w = local_e0.w + (pColInfoOut->field_0x10).w;
			local_d0.x = local_d0.x + (pColInfoOut->field_0x20).x;
			local_d0.y = local_d0.y + (pColInfoOut->field_0x20).y;
			local_d0.z = local_d0.z + (pColInfoOut->field_0x20).z;
			local_d0.w = local_d0.w + (pColInfoOut->field_0x20).w;
			uVar6 = uVar6 | pColInfoOut->result;
		}
	}

	if (iVar5 != 0) {
		if (1 < iVar5) {
			fVar8 = 1.0f / (float)iVar5;
			local_e0.x = local_e0.x * fVar8;
			local_e0.y = local_e0.y * fVar8;
			local_e0.z = local_e0.z * fVar8;
			local_e0.w = local_e0.w * fVar8;
			local_d0.x = local_d0.x * fVar8;
			local_d0.y = local_d0.y * fVar8;
			local_d0.z = local_d0.z * fVar8;
			local_d0.w = local_d0.w * fVar8;
			edF32Vector4NormalizeHard_Fixed(&pColInfoOut->field_0x10, &local_e0);
			(pColInfoOut->field_0x20).x = local_d0.x;
			(pColInfoOut->field_0x20).y = local_d0.y;
			(pColInfoOut->field_0x20).z = local_d0.z;
			(pColInfoOut->field_0x20).w = local_d0.w;
		}

		if (param_3 == 0) {
			(pColInfoOut->field_0x10).x = 0.0f - (pColInfoOut->field_0x10).x;
			(pColInfoOut->field_0x10).y = 0.0f - (pColInfoOut->field_0x10).y;
			(pColInfoOut->field_0x10).z = 0.0f - (pColInfoOut->field_0x10).z;
			(pColInfoOut->field_0x10).w = (pColInfoOut->field_0x10).w;
		}

		uVar6 = uVar6 | pColInfoOut->result;
		edColGetWorldVelocity(&local_120, &pColInfoOut->field_0x20, pParams->bCentre, pParams->field_0x10, pParams->field_0x14);
		edColGetWorldVelocity(&local_130, &pColInfoOut->field_0x20, pParams->aCentre, pParams->field_0x28, pParams->field_0x2c);
		peVar4 = &pColInfoOut->field_0x30;
		peVar4->x = local_120.x - local_130.x;
		(pColInfoOut->field_0x30).y = local_120.y - local_130.y;
		(pColInfoOut->field_0x30).z = local_120.z - local_130.z;
		(pColInfoOut->field_0x30).w = local_120.w - local_130.w;
		if (param_3 != 0) {
			edF32Vector4GetNegHard(peVar4, peVar4);
			edColComputeContactPrim(pParams->pColObj, pParams->pOtherColObj, pColInfoOut, pParams->aType, pParams->aData,
				&pParams->aData->colInfo, pParams->bType, pParams->pPrimObj);

			pColInfoOut->result = uVar6;
			return;
		}

		edColComputeContactPrim(pParams->pOtherColObj, pParams->pColObj, pColInfoOut, pParams->bType, pParams->pPrimObj,
			&pParams->pPrimObj->colInfo, pParams->aType, pParams->aData);
	}
	pColInfoOut->result = uVar6;
	return;
}

edF32VECTOR4 v_null$1614 = { 0.0f, 0.0f, 0.0f, 0.0f };

struct edColPRIM_SPHERE_SPHERE_IN {
	edColOBJECT* pColObj;
	int aType;
	edColPRIM_OBJECT* aPrim;
	edF32VECTOR4* aVecA;
	edF32VECTOR4* aVecB;
	edF32VECTOR4* aVecC;
	edColOBJECT* pOtherColObj;
	int bType;
	edColPRIM_OBJECT* bPrim;
	edF32VECTOR4* bVecA;
	edF32VECTOR4* bVecB;
	edF32VECTOR4* bVecC;
};

const float M_NEG_PI = -3.1415927f;
const float M_NEG_PI_2 = -1.5707964f;

edF32VECTOR4 gColSphereVertices[128];

int gColNbSphereVertices;

void edColIntersectSphereSphere(edColINFO_OUT* pColInfoOut, edColPRIM_SPHERE_SPHERE_IN* pPrimSphereSphereIn)
{
	edColPRIM_OBJECT* aPrim;
	edColPRIM_OBJECT* bPrim;
	edF32VECTOR4* peVar2;
	edF32VECTOR4* peVar3;
	edF32VECTOR4* peVar4;
	int iVar5;
	int iVar6;
	int iVar7;
	float fVar8;
	float fVar9;
	float fVar10;
	float fVar11;
	float fVar12;
	float fVar13;
	float fVar14;
	edF32VECTOR4 local_130;
	edF32VECTOR4 local_120;
	edF32VECTOR4 eStack272;
	edF32VECTOR4 local_100;
	edF32VECTOR4 local_f0;
	edF32VECTOR4 eStack224;
	edF32VECTOR4 local_d0;
	edF32VECTOR4 local_c0;
	edF32VECTOR4 local_b0;
	edF32VECTOR4 local_a0;
	edF32VECTOR4 local_90 = {};
	edF32MATRIX4 eStack128;
	edF32MATRIX4 eStack64;

	iVar6 = 0;

	static int first_time$1108 = 1;

	if (first_time$1108 != 0) {
		static float dx$1109 = 0.3926991f;
		static float dy$1110 = 0.3926991f;
		fVar11 = M_NEG_PI;
		for (iVar7 = 0; iVar7 < 0x10; iVar7 = iVar7 + 1) {
			fVar12 = M_NEG_PI_2;
			for (iVar5 = 0; iVar5 < 8; iVar5 = iVar5 + 1) {
				fVar13 = cosf(fVar11);
				fVar14 = cosf(fVar12);
				fVar8 = sinf(fVar12);
				fVar9 = sinf(fVar11);
				fVar10 = cosf(fVar12);
				gColSphereVertices[iVar6].x = fVar13 * fVar14;
				gColSphereVertices[iVar6].y = fVar8;
				gColSphereVertices[iVar6].z = fVar9 * fVar10;
				gColSphereVertices[iVar6].w = 1.0f;
				fVar12 = fVar12 + dy$1110;
				iVar6 = iVar6 + 1;
			}
			fVar11 = fVar11 + dx$1109;
		}
		first_time$1108 = 0;
		gColNbSphereVertices = iVar6;
	}

#if 0
	peVar3 = (edF32VECTOR4*)&DAT_00000010;
	peVar4 = &local_90;
	peVar2 = peVar4;
	while (peVar2 != (edF32VECTOR4*)0x0) {
		*(undefined*)&peVar4->x = 0;
		peVar4 = (edF32VECTOR4*)((int)&peVar4->x + 1);
		peVar3 = (edF32VECTOR4*)((int)&peVar3[-1].w + 3);
		peVar2 = peVar3;
	}
#endif

	pColInfoOut->result = 0;
	iVar6 = 0;

	aPrim = pPrimSphereSphereIn->aPrim;
	bPrim = pPrimSphereSphereIn->bPrim;

	fVar11 = edF32Vector4GetDistHard(&aPrim->matrix_0x0.rowX);
	fVar12 = edF32Vector4GetDistHard(&aPrim->matrix_0x0.rowY);
	fVar13 = edF32Vector4GetDistHard(&aPrim->matrix_0x0.rowZ);

	fVar13 = fVar13 * fVar11 * fVar12;

	fVar11 = edF32Vector4GetDistHard(&bPrim->matrix_0x0.rowX);
	fVar12 = edF32Vector4GetDistHard(&bPrim->matrix_0x0.rowY);
	fVar14 = edF32Vector4GetDistHard(&bPrim->matrix_0x0.rowZ);

	if (fVar14 * fVar11 * fVar12 < fVar13) {
		edF32Matrix4MulF32Matrix4Hard(&eStack64, &bPrim->matrix_0x0, &aPrim->matrix_0x40);

		for (iVar7 = 0; iVar7 < gColNbSphereVertices; iVar7 = iVar7 + 1) {
			edF32Matrix4MulF32Vector4Hard(&local_a0, &eStack64, gColSphereVertices + iVar7);

			if (local_a0.x * local_a0.x + local_a0.y * local_a0.y + local_a0.z * local_a0.z < 1.0f) {
				local_90 = local_90 + local_a0;
				iVar6 = iVar6 + 1;
			}
		}
		if (iVar6 != 0) {
			fVar11 = 1.0f / (float)iVar6;
			local_90.xyz = local_90.xyz * fVar11;
			local_90.w = 1.0f;

			edColGetNormalInWorldFromLocal(&local_b0, &pPrimSphereSphereIn->aPrim->matrix_0x40, &local_90);

			local_b0.x = 0.0f - local_b0.x;
			local_b0.y = 0.0f - local_b0.y;
			local_b0.z = 0.0f - local_b0.z;
			aPrim = pPrimSphereSphereIn->aPrim;
			edF32Vector4NormalizeHard_Fixed(&eStack224, &local_90);
			eStack224.w = 1.0f;

			edF32Matrix4MulF32Vector4Hard(&local_90, &aPrim->matrix_0x0, &local_90);
			edF32Matrix4MulF32Vector4Hard(&local_f0, &aPrim->matrix_0x0, &eStack224);

			local_100 = local_f0 - local_90;

			fVar12 = edF32Vector4GetDistHard(&local_100);

			pColInfoOut->location = local_b0;
			pColInfoOut->field_0x10 = pColInfoOut->location;
			pColInfoOut->field_0x20 = local_90;

			edColGetWorldVelocity
			(&local_c0, &pColInfoOut->field_0x20, pPrimSphereSphereIn->aVecA, pPrimSphereSphereIn->aVecB,
				pPrimSphereSphereIn->aVecC);

			edColGetWorldVelocity
			(&local_d0, &pColInfoOut->field_0x20, pPrimSphereSphereIn->bVecA, pPrimSphereSphereIn->bVecB,
				pPrimSphereSphereIn->bVecC);

			(pColInfoOut->field_0x30).x = local_c0.x - local_d0.x;
			(pColInfoOut->field_0x30).y = local_c0.y - local_d0.y;
			(pColInfoOut->field_0x30).z = local_c0.z - local_d0.z;
			(pColInfoOut->field_0x30).w = local_c0.w - local_d0.w;
			pColInfoOut->field_0x44 = -fVar12;
			pColInfoOut->result = 1;

			edColComputeContactPrim
			(pPrimSphereSphereIn->pColObj, pPrimSphereSphereIn->pOtherColObj, pColInfoOut, pPrimSphereSphereIn->aType,
				pPrimSphereSphereIn->aPrim, &pPrimSphereSphereIn->aPrim->colInfo, pPrimSphereSphereIn->bType,
				pPrimSphereSphereIn->bPrim);
		}
	}
	else {
		edF32Matrix4MulF32Matrix4Hard(&eStack128, &aPrim->matrix_0x0, &bPrim->matrix_0x40);

		for (iVar7 = 0; iVar7 < gColNbSphereVertices; iVar7 = iVar7 + 1) {
			edF32Matrix4MulF32Vector4Hard(&local_a0, &eStack128, gColSphereVertices + iVar7);

			if (local_a0.x * local_a0.x + local_a0.y * local_a0.y + local_a0.z * local_a0.z < 1.0f) {
				local_90 = local_90 + local_a0;
				iVar6 = iVar6 + 1;
			}
		}

		if (iVar6 != 0) {
			fVar11 = 1.0f / (float)iVar6;
			local_90.xyz = local_90.xyz * fVar11;
			local_90.w = 1.0f;

			edColGetNormalInWorldFromLocal(&local_b0, &pPrimSphereSphereIn->bPrim->matrix_0x40, &local_90);

			aPrim = pPrimSphereSphereIn->bPrim;
			edF32Vector4NormalizeHard_Fixed(&eStack272, &local_90);
			eStack272.w = 1.0f;
			edF32Matrix4MulF32Vector4Hard(&local_90, &aPrim->matrix_0x0, &local_90);
			edF32Matrix4MulF32Vector4Hard(&local_120, &aPrim->matrix_0x0, &eStack272);

			local_130 = local_120 - local_90;
			fVar12 = edF32Vector4GetDistHard(&local_130);
			pColInfoOut->location = local_b0;
			pColInfoOut->field_0x10 = pColInfoOut->location;
			pColInfoOut->field_0x20 = local_90;
			edColGetWorldVelocity
			(&local_c0, &pColInfoOut->field_0x20, pPrimSphereSphereIn->aVecA, pPrimSphereSphereIn->aVecB,
				pPrimSphereSphereIn->aVecC);
			edColGetWorldVelocity
			(&local_d0, &pColInfoOut->field_0x20, pPrimSphereSphereIn->bVecA, pPrimSphereSphereIn->bVecB,
				pPrimSphereSphereIn->bVecC);

			pColInfoOut->field_0x30 = local_c0 - local_d0;
			pColInfoOut->field_0x44 = -fVar12;
			pColInfoOut->result = 1;
			edColComputeContactPrim
			(pPrimSphereSphereIn->pColObj, pPrimSphereSphereIn->pOtherColObj, pColInfoOut, pPrimSphereSphereIn->aType,
				pPrimSphereSphereIn->aPrim, &pPrimSphereSphereIn->aPrim->colInfo, pPrimSphereSphereIn->bType,
				pPrimSphereSphereIn->bPrim);
		}
	}
	return;
}

uint edColArrayObjectPrimsPenatratingArrayPrims(edColARRAY_PRIM_PRIM* pParams)
{
	int pPrimSizeA;
	int primSizeB;
	edColOBJECT* peVar3;
	edColOBJECT* peVar4;
	char* pPrimEndB;
	uint uVar6;
	char* pPrimA;
	uint uVar8;
	char* pPrimB;
	char* pPrimEnd;
	//edColPRIM_BOX_BOX_IN local_f0;
	edColPRIM_SPHERE_SPHERE_IN local_c0;
	edColPRIM_BOX_SPHERE_IN local_90;
	edColINFO_OUT local_60;

	uVar8 = 0;
	pPrimSizeA = pParams->primSize;
	primSizeB = pParams->bPrimSize;
	peVar3 = pParams->pColObj;
	peVar4 = pParams->pOtherColObj;
	pPrimA = reinterpret_cast<char*>(pParams->aData);
	pPrimEndB = reinterpret_cast<char*>(pParams->bData) + (pParams->bCount * primSizeB);
	uVar6 = pParams->aType << 8 | pParams->bType;
	pPrimEnd = pPrimA + (pParams->aCount * pPrimSizeA);
	local_90.pOtherColObj = peVar4;

	if ((uVar6 == 0xb13) || (uVar6 == 0xe13)) {
		IMPLEMENTATION_GUARD(
		local_90.field_0x8 = pParams->bType;
		local_90.field_0x18 = pParams->pColObj;
		local_90.field_0x20 = pParams->aType;
		for (; pPrimA < pPrimEnd; pPrimA = (edF32MATRIX4*)((int)&pPrimA->aa + pPrimSizeA)) {
			local_90.aCentre = (edF32VECTOR4*)&pPrimA->da;
			if (pParams->aType == 0xe) {
				local_90.field_0x28 = (edF32VECTOR4*)(pPrimA + 3);
				local_90.field_0x2c = (edF32VECTOR4*)&pPrimA[3].ba;
			}
			else {
				local_90.field_0x28 = &v_null$1614;
				local_90.field_0x2c = &v_null$1614;
			}
			local_90.field_0x1c = pPrimA;
			for (pPrimB = (edF32MATRIX4*)pParams->bData; pPrimB < pPrimEndB; pPrimB = (edF32MATRIX4*)((int)&pPrimB->aa + primSizeB)
				) {
				local_90.bCentre = (edF32VECTOR4*)&pPrimB->da;
				local_90.field_0x10 = (edF32VECTOR4*)(pPrimB + 3);
				if ((pParams->bType == 0xd) || (pParams->bType == 0x13)) {
					local_90.field_0x14 = (edF32VECTOR4*)&pPrimB[3].ba;
				}
				else {
					local_90.field_0x10 = &v_null$1614;
					local_90.field_0x14 = &v_null$1614;
				}
				local_90.field_0x4 = pPrimB;
				edColIntersectBoxSphere(&local_60, &local_90, 1);
				uVar8 = uVar8 | local_60.result;
			}
			peVar3->colResult = uVar8;
			if (peVar4 != (edColOBJECT*)0x0) {
				peVar4->colResult = uVar8;
			}
		})
	}
	else {
		if ((((uVar6 == 0xb0a) || (uVar6 == 0xb0d)) || (uVar6 == 0xe0a)) || (uVar6 == 0xe0d)) {
			local_90.bType = pParams->bType;
			local_90.pColObj = pParams->pColObj;
			local_90.aType = pParams->aType;

			for (; pPrimA < pPrimEnd; pPrimA = pPrimA + pPrimSizeA) {
				edColPRIM_OBJECT* pPrim = reinterpret_cast<edColPRIM_OBJECT*>(pPrimA);

				local_90.aCentre = &pPrim->matrix_0x0.rowT;

				if (pParams->aType == 0xe) {
					local_90.field_0x28 = &pPrim->field_0xc0;
					local_90.field_0x2c = &pPrim->field_0xd0;
				}
				else {
					local_90.field_0x28 = &v_null$1614;
					local_90.field_0x2c = &v_null$1614;
				}

				local_90.aData = pPrim;

				for (pPrimB = reinterpret_cast<char*>(pParams->bData); pPrimB < pPrimEndB; pPrimB = pPrimB + primSizeB) {
					edColPRIM_OBJECT* pPrim = reinterpret_cast<edColPRIM_OBJECT*>(pPrimB);

					local_90.bCentre = &pPrim->matrix_0x0.rowT;

					local_90.field_0x10 = &pPrim->field_0xc0;

					if ((pParams->bType == 0xd) || (pParams->bType == 0x13)) {
						local_90.field_0x14 = &pPrim->field_0xd0;
					}
					else {
						local_90.field_0x10 = &v_null$1614;
						local_90.field_0x14 = &v_null$1614;
					}

					local_90.pPrimObj = pPrim;
					edColIntersectBoxSphere(&local_60, &local_90, 1);
					uVar8 = uVar8 | local_60.result;
				}

				peVar3->colResult = uVar8;

				if (peVar4 != (edColOBJECT*)0x0) {
					peVar4->colResult = uVar8;
				}
			}
		}
		else {
			if (((uVar6 == 0xa0b) || (uVar6 == 0xa0e)) ||
				((uVar6 == 0x130b || (((uVar6 == 0x130e || (uVar6 == 0xd0b)) || (uVar6 == 0xd0e)))))) {
				local_90.bType = pParams->aType;
				local_90.pColObj = pParams->pOtherColObj;
				local_90.aType = pParams->bType;
				local_90.pOtherColObj = peVar3;
				for (; pPrimA < pPrimEnd; pPrimA = pPrimA + pPrimSizeA) {
					edColPRIM_OBJECT* pPrim = reinterpret_cast<edColPRIM_OBJECT*>(pPrimA);

					local_90.bCentre = &pPrim->matrix_0x0.rowT;
					local_90.field_0x10 = &pPrim->field_0xc0;

					if ((pParams->aType == 0xd) || (pParams->aType == 0x13)) {
						local_90.field_0x14 = &pPrim->field_0xd0;
					}
					else {
						local_90.field_0x10 = &v_null$1614;
						local_90.field_0x14 = &v_null$1614;
					}

					local_90.pPrimObj = pPrim;

					for (pPrimB = reinterpret_cast<char*>(pParams->bData); pPrimB < pPrimEndB; pPrimB = pPrimB + primSizeB) {
						edColPRIM_OBJECT* pPrim = reinterpret_cast<edColPRIM_OBJECT*>(pPrimB);
						local_90.aCentre = &pPrim->matrix_0x0.rowT;

						if (pParams->bType == 0xe) {
							local_90.field_0x28 = &pPrim->field_0xc0;
							local_90.field_0x2c = &pPrim->field_0xd0;
						}
						else {
							local_90.field_0x28 = &v_null$1614;
							local_90.field_0x2c = &v_null$1614;
						}

						local_90.aData = pPrim;
						edColIntersectBoxSphere(&local_60, &local_90, 0);
						local_60.location = local_60.field_0x10;

						uVar8 = uVar8 | local_60.result;
					}
					peVar3->colResult = uVar8;
					if (peVar4 != (edColOBJECT*)0x0) {
						peVar4->colResult = uVar8;
					}
				}
			}
			else {
				if (((uVar6 == 0x1313) || (uVar6 == 0x130d)) ||
					((uVar6 == 0x130a || (((uVar6 == 0xd13 || (uVar6 == 0xd0d)) || (uVar6 == 0xd0a)))))) {
					IMPLEMENTATION_GUARD(
					local_f0.field_0x4 = pParams->aType;
					local_f0.field_0x1c = pParams->bType;
					local_f0.field_0x0 = peVar3;
					local_f0.field_0x18 = peVar4;
					for (; pPrimA < pPrimEnd; pPrimA = (edF32MATRIX4*)((int)&pPrimA->aa + pPrimSizeA)) {
						local_f0.field_0xc = &pPrimA->da;
						local_f0.field_0x10 = pPrimA + 3;
						local_f0.field_0x14 = &pPrimA[3].ba;
						local_f0.field_0x8 = pPrimA;
						for (pPrimB = (edF32MATRIX4*)pParams->bData; pPrimB < pPrimEndB;
							pPrimB = (edF32MATRIX4*)((int)&pPrimB->aa + primSizeB)) {
							local_f0.field_0x24 = &pPrimB->da;
							local_f0.field_0x28 = (edF32VECTOR4*)(pPrimB + 3);
							if ((pParams->bType == 0xd) || (pParams->bType == 0x13)) {
								local_f0.field_0x2c = (edF32VECTOR4*)&pPrimB[3].ba;
							}
							else {
								local_f0.field_0x28 = &v_null$1614;
								local_f0.field_0x2c = &v_null$1614;
							}
							local_f0.field_0x20 = pPrimB;
							edColIntersectBoxBox(&local_60, &local_f0, 0);
							uVar8 = uVar8 | local_60.result;
						}
						peVar3->colResult = uVar8;
						if (peVar4 != (edColOBJECT*)0x0) {
							peVar4->colResult = uVar8;
						}
					})
				}
				else {
					if ((uVar6 == 0xe0b) || (uVar6 == 0xe0e)) {
						local_c0.aType = pParams->aType;
						local_c0.bType = pParams->bType;
						local_c0.pColObj = peVar3;
						local_c0.pOtherColObj = peVar4;

						for (; pPrimA < pPrimEnd; pPrimA = pPrimA + pPrimSizeA) {
							edColPRIM_OBJECT* pPrim = reinterpret_cast<edColPRIM_OBJECT*>(pPrimA);

							local_c0.aVecA = &pPrim->matrix_0x0.rowT;
							local_c0.aVecB = &pPrim->field_0xc0;
							local_c0.aVecC = &pPrim->field_0xd0;
							local_c0.aPrim = pPrim;

							for (pPrimB = reinterpret_cast<char*>(pParams->bData); pPrimB < pPrimEndB; pPrimB = pPrimB + primSizeB) {
								edColPRIM_OBJECT* pPrimB = reinterpret_cast<edColPRIM_OBJECT*>(pPrimB);

								local_c0.bVecA = &pPrimB->matrix_0x0.rowT;

								if (pParams->bType == 0xe) {
									local_c0.bVecB = &pPrimB->field_0xc0;
									local_c0.bVecC = &pPrimB->field_0xd0;
								}
								else {
									local_c0.bVecB = &v_null$1614;
									local_c0.bVecC = &v_null$1614;
								}
								local_c0.bPrim = pPrimB;

								edColIntersectSphereSphere(&local_60, &local_c0);
								uVar8 = uVar8 | local_60.result;
							}
							peVar3->colResult = uVar8;
							if (peVar4 != (edColOBJECT*)0x0) {
								peVar4->colResult = uVar8;
							}
						}
					}
					else {
						uVar8 = 0;
					}
				}
			}
		}
	}
	return uVar8;
}

uint edColArrayObjectTriangles4PenatratingPrims(edColARRAY_TRI4_PRIM* pParams)
{
	int bSize;
	edF32TRIANGLE4* peVar2;
	int iVar3;
	int iVar4;
	uint uVar5;
	char* pPrimStart;
	char* peVar7;
	edF32TRIANGLE4* peVar8;
	uint uVar9;
	edColPRIM_BOX_TRI4_IN local_a0;
	edColPRIM_SPHERE_TRI4_IN local_80;
	edColINFO_OUT eStack96;

	uVar9 = 0;
	bSize = pParams->bSize;
	peVar2 = (edF32TRIANGLE4*)pParams->aData;
	iVar3 = pParams->bCount2;
	iVar4 = pParams->bType;

	if ((iVar4 == 0xa) || (iVar4 == 0xd)) {
		uVar5 = pParams->bCount;
		peVar7 = reinterpret_cast<char*>(pParams->bData);
		pPrimStart = peVar7;

		for (; peVar7 < pPrimStart + (uVar5 * bSize); peVar7 = peVar7 + bSize) {
			edColPRIM_OBJECT* pPrim = reinterpret_cast<edColPRIM_OBJECT*>(peVar7);
			local_a0.field_0x10 = &pPrim->matrix_0x0.rowT;
			local_a0.pColObj = pParams->pColObj;
			local_a0.field_0x14 = &pPrim->field_0xc0;
			local_a0.field_0x18 = &pPrim->field_0xd0;
			local_a0.pOtherColObj = pParams->pOtherColObj;
			local_a0.aType = pParams->bType;
			local_a0.pData = pPrim;

			for (peVar8 = peVar2; peVar8 < peVar2 + iVar3; peVar8 = peVar8 + 1) {
				local_a0.pTriangle = peVar8;
				edColIntersectBoxTriangle4(&eStack96, &local_a0, 1);
				eStack96.field_0x10.x = 0.0f - eStack96.field_0x10.x;
				eStack96.field_0x10.y = 0.0f - eStack96.field_0x10.y;
				eStack96.field_0x10.z = 0.0f - eStack96.field_0x10.z;
				uVar9 = uVar9 | eStack96.result;
			}
		}
	}
	else {
		if ((iVar4 == 0xb) || (iVar4 == 0xe)) {
			uVar5 = pParams->bCount;
			peVar7 = reinterpret_cast<char*>(pParams->bData);
			pPrimStart = peVar7;

			for (; peVar7 < pPrimStart + (uVar5 * bSize); peVar7 = peVar7 + bSize) {
				edColPRIM_OBJECT* pPrim = reinterpret_cast<edColPRIM_OBJECT*>(peVar7);
				local_80.aCentre = &pPrim->matrix_0x0.rowT;
				local_80.pColObj = pParams->pColObj;
				local_80.field_0x14 = &pPrim->field_0xc0;
				local_80.field_0x18 = &pPrim->field_0xd0;
				local_80.pOtherColObj = pParams->pOtherColObj;
				local_80.aType = pParams->bType;
				local_80.pData = pPrim;

				for (peVar8 = peVar2; peVar8 < peVar2 + iVar3; peVar8 = peVar8 + 1) {
					local_80.pTriangle = peVar8;
					edColIntersectSphereTriangle4(&eStack96, &local_80, 1);
					eStack96.field_0x10.x = 0.0f - eStack96.field_0x10.x;
					eStack96.field_0x10.y = 0.0f - eStack96.field_0x10.y;
					eStack96.field_0x10.z = 0.0f - eStack96.field_0x10.z;
					uVar9 = uVar9 | eStack96.result;
				}
			}
		}
		else {
			uVar9 = 0;
		}
	}

	return uVar9;
}

uint edObbTreeIntersectObbTree(edColINFO_OBBTREE_OBBTREE* pColInfoObbTree, edObbTREE_DYN* pObbTreeA, edObbTREE_DYN* pObbTreeB)
{
	byte bVar1;
	edObbTREE_DYN* peVar2;
	edObbTREE_DYN* peVar3;
	undefined4 uVar4;
	uint uVar5;
	bool lVar6;
	uint uVar7;
	int iVar8;
#if 0
	ProfileObject* pPVar9;
#endif
	uint uVar10;
	undefined4* puVar11;
	edColPrimEntry* puVar12;
	uint uVar13;
	float** ppfVar14;
	ushort* puVar15;
	edObbTREE_DYN** ppeVar16;
	int iVar17;
	int iVar18;
	ushort* puVar19;
	edObbTREE_DYN** ppeVar20;
	float** ppfVar21;
	int iVar22;
	float fVar23;
	int local_1054;
	int intersectCountB;
	int intersectCountA;
	undefined4 local_fec;
	undefined4 local_fe8;
	float* local_fe0;
	float* local_fdc;
	float* local_fd8;
	float* local_fd4;
	float* local_fd0;
	int local_fcc;
	int local_fc8;
	undefined4 local_fc4;
	edColARRAY_PRIM_QUAD4 local_f70;
	edColARRAY_PRIM_TRI4 local_e90;
	edColARRAY_PRIM_PRIM local_fc0;
	edColARRAY_TRI4_PRIM local_f50;
	IMPLEMENTATION_GUARD_LOG(
	edColARRAY_VERT_IND_PRIM local_f90;
	edColARRAY_PRIM_TRI4_FAST local_f30;
	edColARRAY_PRIM_TRI4_FAST local_f00;
	edColARRAY_PRIM_TRI4 local_ed0;
	edColARRAY_PRIM_TRI4 local_eb0;
	edColARRAY_VERT_IND_QUAD4 local_e70;)
	undefined local_e50[16];
	uint local_e40;
	edF32TRIANGLE4* local_e3c;
	IntersectResTestA aIntersectResultsA[100];
	edColPrimEntry aIntersectResultsB[100];
	edObbTREE_DYN* local_340[2][100];
	IMPLEMENTATION_GUARD_LOG(
	edColRAY_TRIANGLE4_IN local_20;)
	int local_10[3];
	float local_4;

	//uVar5 = prof_obb_col;
	uVar13 = 0;
	uVar10 = 0;
	local_340[0][0] = pObbTreeB;
	local_10[0] = 1;
	intersectCountA = 0;
	local_10[1] = 0;
	intersectCountB = 0;
#if 0
	if (gColConfig.bCreateProfileObj != 0) {
		if ((prof_obb_col & 1) == 0) {
			uVar7 = prof_obb_col >> 4;
			if (ProfileManager.ProfileManager[uVar7].field_0x34 != 0) {
				ProfileManager.ProfileManager[uVar7].field_0x8 = 0;
				ProfileManager.ProfileManager[uVar7].field_0x20 = 0;
				ProfileManager.ProfileManager[uVar7].field_0x34 = 0;
			}
			ProfileManager.ProfileManager[uVar5 >> 4].field_0x0 = (long)PCR1 & 0xffffffff;
			ProfileManager.ProfileManager[uVar5 >> 4].field_0x20 = ProfileManager.ProfileManager[uVar5 >> 4].field_0x20 + 1;
		}
		else {
			uVar7 = prof_obb_col >> 4;
			if (ProfileManager.g_ProfileObjA_0041ed40[uVar7].field_0x34 != 0) {
				ProfileManager.g_ProfileObjA_0041ed40[uVar7].field_0x8 = 0;
				ProfileManager.g_ProfileObjA_0041ed40[uVar7].field_0x20 = 0;
				ProfileManager.g_ProfileObjA_0041ed40[uVar7].field_0x34 = 0;
			}
			ProfileManager.g_ProfileObjA_0041ed40[uVar5 >> 4].field_0x0 = (long)PCR1 & 0xffffffff;
			ProfileManager.g_ProfileObjA_0041ed40[uVar5 >> 4].field_0x20 =
				ProfileManager.g_ProfileObjA_0041ed40[uVar5 >> 4].field_0x20 + 1;
		}
	}
#endif

	// Initial BB tests.
	COLLISION_LOG(LogLevel::VeryVerbose, "\nedObbTreeIntersectObbTree");

	iVar8 = 0;
	do {
		uVar5 = uVar10 != 0 ^ 1;
		ppeVar16 = local_340[0] + (iVar8 + uVar10) * 0x14;
		local_10[uVar5] = 0;
		ppeVar20 = local_340[uVar5];

		COLLISION_LOG(LogLevel::VeryVerbose, "\nedObbTreeIntersectObbTree flip: {} count: {}", uVar5, local_10[uVar10]);

		for (iVar8 = 0; iVar8 < local_10[uVar10]; iVar8 = iVar8 + 1) {
			peVar2 = *ppeVar16;
			iVar18 = 0;

			COLLISION_LOG(LogLevel::VeryVerbose, "edObbTreeIntersectObbTree type: {} (0x{:x})", peVar2->type, *(uint*)&peVar2->bbox.transform.aa);

			if (peVar2->type == 1) {

				COLLISION_LOG(LogLevel::VeryVerbose, "edObbTreeIntersectObbTree count: {}", peVar2->count_0x52);
				bVar1 = peVar2->count_0x52;
				for (; iVar18 < bVar1; iVar18 = iVar18 + 1) {

					COLLISION_LOG(LogLevel::VeryVerbose, "edObbTreeIntersectObbTree checking: {}", iVar18);

					edObbTREE_DYN* pLinkedTree = (edObbTREE_DYN*)LOAD_SECTION(peVar2->field_0x54[iVar18]);
					lVar6 = edObbIntersect(&pObbTreeA->bbox, &pLinkedTree->bbox);

					COLLISION_LOG(LogLevel::VeryVerbose, "edObbTreeIntersectObbTree edObbIntersect result: {}", lVar6);

					if (lVar6 != 0) {
						uVar5 = uVar10 != 0 ^ 1;
						if (local_10[uVar5] < 100) {
							*ppeVar20 = pLinkedTree;
							ppeVar20 = ppeVar20 + 1;
							local_10[uVar5] = local_10[uVar5] + 1;

							COLLISION_LOG(LogLevel::VeryVerbose, "edObbTreeIntersectObbTree flip index: {} new count: {}", uVar5, local_10[uVar5]);
						}
					}
				}
			}
			else {
				lVar6 = edObbIntersect(&pObbTreeA->bbox, &peVar2->bbox);

				COLLISION_LOG(LogLevel::VeryVerbose, "edObbTreeIntersectObbTree edObbIntersect result: {}", lVar6);

				if ((lVar6 != 0) && (intersectCountA < 100)) {
					aIntersectResultsA[intersectCountA].pObbTree = peVar2;
					aIntersectResultsA[intersectCountA].sub.type = peVar2->type;
					aIntersectResultsA[intersectCountA].sub.count = peVar2->count_0x52;
					aIntersectResultsA[intersectCountA].sub.pData = LOAD_SECTION(peVar2->field_0x54[0]);
					intersectCountA = intersectCountA + 1;

					COLLISION_LOG(LogLevel::VeryVerbose, "edObbTreeIntersectObbTree new intersect count: {} (type: 0x{:x} - count: 0x{:x})", intersectCountA, peVar2->type, peVar2->count_0x52);
				}
			}
			ppeVar16 = ppeVar16 + 1;
		}

		uVar10 = uVar10 != 0 ^ 1;
		iVar8 = uVar10 << 2;
	} while (local_10[uVar10] != 0);

	COLLISION_LOG(LogLevel::VeryVerbose, "\nedObbTreeIntersectObbTree Process A Results");

	// Go through first set of results.
	for (iVar8 = 0; iVar8 < intersectCountA; iVar8 = iVar8 + 1) {
		local_340[uVar10][0] = pObbTreeA;
		local_10[uVar10] = 1;
		local_10[uVar10 != 0 ^ 1] = 0;
		peVar2 = aIntersectResultsA[iVar8].pObbTree;
		do {
			uVar5 = uVar10 != 0 ^ 1;
			ppeVar16 = local_340[uVar10];
			local_10[uVar5] = 0;
			ppeVar20 = local_340[uVar5];
			for (iVar18 = 0; iVar18 < local_10[uVar10]; iVar18 = iVar18 + 1) {
				peVar3 = *ppeVar16;
				iVar17 = 0;
				if (peVar3->type == 1) {
					bVar1 = peVar3->count_0x52;
					for (; iVar17 < bVar1; iVar17 = iVar17 + 1) {
						edObbTREE_DYN* pLinkedTree = (edObbTREE_DYN*)LOAD_SECTION(peVar3->field_0x54[iVar17]);

						lVar6 = edObbIntersect(&peVar2->bbox, &pLinkedTree->bbox);
						if (lVar6 != 0) {
							uVar5 = uVar10 != 0 ^ 1;
							if (local_10[uVar5] < 100) {
								*ppeVar20 = pLinkedTree;
								ppeVar20 = ppeVar20 + 1;
								local_10[uVar5] = local_10[uVar5] + 1;
							}
						}
						bVar1 = peVar3->count_0x52;
					}
				}
				else {
					lVar6 = edObbIntersect(&peVar2->bbox, &peVar3->bbox);
					if ((lVar6 != 0) && (intersectCountB < 100)) {
						aIntersectResultsB[intersectCountB].a.type = peVar3->type;
						aIntersectResultsB[intersectCountB].a.count = peVar3->count_0x52;
						aIntersectResultsB[intersectCountB].a.pData = LOAD_SECTION(peVar3->field_0x54[0]);

						aIntersectResultsB[intersectCountB].b = aIntersectResultsA[iVar8].sub;

						intersectCountB = intersectCountB + 1;

						COLLISION_LOG(LogLevel::VeryVerbose, "edObbTreeIntersectObbTree new intersect B count: {} (type: 0x{:x} - count: 0x{:x})", intersectCountB, peVar3->type, peVar3->count_0x52);

					}
				}

				ppeVar16 = ppeVar16 + 1;
			}
			uVar10 = uVar10 != 0 ^ 1;
		} while (local_10[uVar10] != 0);
	}

#if 0
	if (gColConfig.bCreateProfileObj != 0) {
		uVar5 = prof_obb_col >> 4;
		if ((prof_obb_col & 1) == 0) {
			ProfileManager.ProfileManager[uVar5].field_0x0 = (long)PCR1 - ProfileManager.ProfileManager[uVar5].field_0x0;
			ProfileManager.ProfileManager[uVar5].field_0x8 =
				ProfileManager.ProfileManager[uVar5].field_0x8 + ProfileManager.ProfileManager[uVar5].field_0x0;
			pPVar9 = ProfileManager.ProfileManager + uVar5;
			if (pPVar9->field_0x10 < ProfileManager.ProfileManager[uVar5].field_0x0) {
				pPVar9->field_0x10 = pPVar9->field_0x0;
			}
		}
		else {
			ProfileManager.g_ProfileObjA_0041ed40[uVar5].field_0x0 =
				(long)PCR1 - ProfileManager.g_ProfileObjA_0041ed40[uVar5].field_0x0;
			ProfileManager.g_ProfileObjA_0041ed40[uVar5].field_0x8 =
				ProfileManager.g_ProfileObjA_0041ed40[uVar5].field_0x8 +
				ProfileManager.g_ProfileObjA_0041ed40[uVar5].field_0x0;
			pPVar9 = ProfileManager.g_ProfileObjA_0041ed40 + uVar5;
			if (pPVar9->field_0x10 < ProfileManager.g_ProfileObjA_0041ed40[uVar5].field_0x0) {
				pPVar9->field_0x10 = pPVar9->field_0x0;
			}
		}
	}
	uVar5 = prof_prim_col;
	if (gColConfig.bCreateProfileObj != 0) {
		if ((prof_prim_col & 1) == 0) {
			uVar10 = prof_prim_col >> 4;
			if (ProfileManager.ProfileManager[uVar10].field_0x34 != 0) {
				ProfileManager.ProfileManager[uVar10].field_0x8 = 0;
				ProfileManager.ProfileManager[uVar10].field_0x20 = 0;
				ProfileManager.ProfileManager[uVar10].field_0x34 = 0;
			}
			ProfileManager.ProfileManager[uVar5 >> 4].field_0x0 = (long)PCR1 & 0xffffffff;
			ProfileManager.ProfileManager[uVar5 >> 4].field_0x20 = ProfileManager.ProfileManager[uVar5 >> 4].field_0x20 + 1;
		}
		else {
			uVar10 = prof_prim_col >> 4;
			if (ProfileManager.g_ProfileObjA_0041ed40[uVar10].field_0x34 != 0) {
				ProfileManager.g_ProfileObjA_0041ed40[uVar10].field_0x8 = 0;
				ProfileManager.g_ProfileObjA_0041ed40[uVar10].field_0x20 = 0;
				ProfileManager.g_ProfileObjA_0041ed40[uVar10].field_0x34 = 0;
			}
			ProfileManager.g_ProfileObjA_0041ed40[uVar5 >> 4].field_0x0 = (long)PCR1 & 0xffffffff;
			ProfileManager.g_ProfileObjA_0041ed40[uVar5 >> 4].field_0x20 =
				ProfileManager.g_ProfileObjA_0041ed40[uVar5 >> 4].field_0x20 + 1;
		}
	}
#endif

	// Go through results.
	puVar12 = aIntersectResultsB;
	for (iVar8 = 0; iVar8 < intersectCountB; iVar8 = iVar8 + 1) {
		uVar5 = (uint)(puVar12->a).type << 8 | (uint)(puVar12->b).type;
		if (uVar5 == 0x1008) {
			IMPLEMENTATION_GUARD(
			uVar5 = puVar12->a->pData;
			uVar10 = uVar5 + (uint)puVar12[1] * 0x60;
			for (; uVar5 < uVar10; uVar5 = uVar5 + 0x60) {
				fVar23 = *(float*)(uVar5 + 0x44);
				ppfVar14 = *(float***)(puVar12 + 6);
				iVar18 = -1;
				iVar17 = 0;
				ppfVar21 = ppfVar14 + (uint)puVar12[5] * 5;
				for (; ppfVar14 < ppfVar21; ppfVar14 = ppfVar14 + 5) {
					local_fe0 = *ppfVar14;
					for (iVar22 = 0; iVar22 < 2; iVar22 = iVar22 + 1) {
						if (iVar22 == 0) {
							local_fdc = ppfVar14[1];
							local_fd8 = ppfVar14[2];
						}
						else {
							local_fdc = ppfVar14[2];
							local_fd8 = ppfVar14[3];
						}
						local_fd4 = ppfVar14[4];
						local_20.field_0x0 = (float*)(uVar5 + 0x10);
						local_20.field_0x4 = (float*)(uVar5 + 0x30);
						local_20.field_0x8 = &local_fe0;
						edColIntersectRayTriangle4(&local_4, &local_20);
						if ((((local_4 != -8888.0) && (local_4 != -9999.0)) && (0.0 <= local_4)) &&
							((local_4 < fVar23 || ((fVar23 < 0.0 && (local_4 <= *(float*)(uVar5 + 0x40))))))) {
							fVar23 = local_4;
							iVar18 = iVar17;
							local_1054 = iVar22;
						}
					}
					iVar17 = iVar17 + 1;
				}
				*(float*)(uVar5 + 0x44) = fVar23;
				if (((0.0 <= fVar23) && (-1 < iVar18)) && (*(int*)(puVar12 + 6) != 0)) {
					puVar11 = (undefined4*)(*(int*)(puVar12 + 6) + iVar18 * 0x14);
					if (local_1054 == 0) {
						local_fec = puVar11[1];
						local_fe8 = puVar11[2];
					}
					else {
						local_fec = puVar11[2];
						local_fe8 = puVar11[3];
					}
					uVar4 = puVar11[4];
					*(undefined4*)(uVar5 + 0x50) = *puVar11;
					*(undefined4*)(uVar5 + 0x54) = local_fec;
					*(undefined4*)(uVar5 + 0x58) = local_fe8;
					*(undefined4*)(uVar5 + 0x5c) = uVar4;
				}
			})
		}
		else {
			if (uVar5 == 0x1005) {
				IMPLEMENTATION_GUARD(
				uVar5 = *(uint*)(puVar12 + 2);
				uVar10 = uVar5 + (uint)puVar12[1] * 0x60;
				for (; uVar5 < uVar10; uVar5 = uVar5 + 0x60) {
					fVar23 = *(float*)(uVar5 + 0x44);
					puVar15 = *(ushort**)(puVar12 + 6);
					puVar19 = puVar15 + (uint)puVar12[5] * 0xc;
					for (; puVar15 < puVar19; puVar15 = puVar15 + 0xc) {
						local_20.field_0x0 = (float*)(uVar5 + 0x10);
						local_20.field_0x4 = (float*)(uVar5 + 0x30);
						local_20.field_0x8 = &local_fd0;
						local_fd0 = (float*)(*(int*)(pColInfoObbTree->field_0x8 + 0x28) + (uint)*puVar15 * 0x10);
						local_fcc = *(int*)(pColInfoObbTree->field_0x8 + 0x28) + (uint)puVar15[1] * 0x10;
						local_fc8 = *(int*)(pColInfoObbTree->field_0x8 + 0x28) + (uint)puVar15[2] * 0x10;
						local_fc4 = *(undefined4*)(puVar15 + 8);
						edColIntersectRayTriangle4(&local_4, &local_20);
						if ((((local_4 != -8888.0) && (local_4 != -9999.0)) && (0.0 <= local_4)) &&
							((local_4 < fVar23 || ((fVar23 < 0.0 && (local_4 <= *(float*)(uVar5 + 0x40))))))) {
							*(float**)(uVar5 + 0x50) = local_fd0;
							*(int*)(uVar5 + 0x54) = local_fcc;
							*(int*)(uVar5 + 0x58) = local_fc8;
							*(undefined4*)(uVar5 + 0x5c) = local_fc4;
							fVar23 = local_4;
						}
					}
					*(float*)(uVar5 + 0x44) = fVar23;
				})
			}
			else {
				if (uVar5 == 0x1004) {
					IMPLEMENTATION_GUARD(
					uVar5 = *(uint*)(puVar12 + 2);
					uVar10 = uVar5 + (uint)puVar12[1] * 0x60;
					for (; uVar5 < uVar10; uVar5 = uVar5 + 0x60) {
						fVar23 = *(float*)(uVar5 + 0x44);
						ppfVar14 = *(float***)(puVar12 + 6);
						iVar18 = -1;
						iVar17 = 0;
						ppfVar21 = ppfVar14 + (uint)puVar12[5] * 4;
						for (; ppfVar14 < ppfVar21; ppfVar14 = ppfVar14 + 4) {
							local_20.field_0x4 = (float*)(uVar5 + 0x30);
							local_20.field_0x0 = (float*)(uVar5 + 0x10);
							local_20.field_0x8 = ppfVar14;
							edColIntersectRayTriangle4(&local_4, &local_20);
							if (((local_4 != -8888.0) && (local_4 != -9999.0)) &&
								((0.0 <= local_4 && ((local_4 < fVar23 || ((fVar23 < 0.0 && (local_4 <= *(float*)(uVar5 + 0x40))))))))
								) {
								fVar23 = local_4;
								iVar18 = iVar17;
							}
							iVar17 = iVar17 + 1;
						}
						*(float*)(uVar5 + 0x44) = fVar23;
						if ((0.0 <= fVar23) && (-1 < iVar18)) {
							if (*(int*)(puVar12 + 6) != 0) {
								puVar11 = (undefined4*)(*(int*)(puVar12 + 6) + iVar18 * 0x10);
								*(undefined4*)(uVar5 + 0x50) = *puVar11;
								*(undefined4*)(uVar5 + 0x54) = puVar11[1];
								*(undefined4*)(uVar5 + 0x58) = puVar11[2];
								*(undefined4*)(uVar5 + 0x5c) = puVar11[3];
							}
						}
					})
				}
				else {
					if ((((((uVar5 == 0x130a) || (uVar5 == 0x130d)) || (uVar5 == 0x1313)) ||
						((uVar5 == 0x130b || (uVar5 == 0x130e)))) || (uVar5 == 0xd0a)) ||
						(((((uVar5 == 0xd0d || (uVar5 == 0xd13)) ||
							((uVar5 == 0xd0b || (((uVar5 == 0xd0e || (uVar5 == 0xe0a)) || (uVar5 == 0xe0d)))))) ||
							((uVar5 == 0xe13 || (uVar5 == 0xe0b)))) || (uVar5 == 0xe0e)))) {
						COLLISION_LOG(LogLevel::VeryVerbose, "edObbTreeIntersectObbTree edColArrayObjectPrimsPenatratingArrayPrims 0x{:x}", uVar5);
						local_fc0.pColObj = pColInfoObbTree->pColObj;
						local_fc0.aData = (puVar12->a).pData;
						local_fc0.aCount = (puVar12->a).count;
						local_fc0.primSize = _gColSizeOfPrims[(puVar12->a).type];
						local_fc0.aType = (puVar12->a).type;
						local_fc0.pOtherColObj = pColInfoObbTree->pOtherColObj;
						local_fc0.bData = (puVar12->b).pData;
						local_fc0.bCount = (puVar12->b).count;
						local_fc0.bPrimSize = _gColSizeOfPrims[(puVar12->b).type];
						local_fc0.bType = (puVar12->b).type;

						uVar5 = edColArrayObjectPrimsPenatratingArrayPrims(&local_fc0);
						uVar13 = uVar13 | uVar5;
					}
					else {
						if (((uVar5 == 0x30d) || (uVar5 == 0x30e)) || ((uVar5 == 0x30a || (uVar5 == 0x30b)))) {
							IMPLEMENTATION_GUARD(
							local_f90.field_0x0 = pColInfoObbTree->field_0x0;
							local_f90.field_0x4 = (char*)(uint)puVar12[1];
							local_f90.field_0x8 = *(char**)(puVar12 + 2);
							local_f90.field_0xc = pColInfoObbTree->pOtherColObj;
							local_f90.field_0x1c = *(char**)(puVar12 + 6);
							local_f90.field_0x10 = (char*)(uint)puVar12[5];
							local_f90.field_0x18 = *(char**)(&_gColSizeOfPrims + (uint)puVar12[4] * 4);
							local_f90.field_0x14 = (char*)(uint)puVar12[4];
							uVar5 = edColArrayVerticesIndexPenatratingArrayPrims(&local_f90, (int)pColInfoObbTree->field_0xc);
							uVar13 = uVar13 | uVar5;)
						}
						else {
							if ((uVar5 == 0xd08) || (uVar5 == 0xe08)) {
								COLLISION_LOG(LogLevel::VeryVerbose, "edObbTreeIntersectObbTree edColArrayObjectPrimPenatratingArrayQuads4 0x{:x}", uVar5);
								local_f70.pColObj = pColInfoObbTree->pColObj;
								local_f70.aData = puVar12->a.pData;
								local_f70.aCount = puVar12->a.count;
								local_f70.primSize = _gColSizeOfPrims[(puVar12->a).type];
								local_f70.aType = puVar12->a.type;
								local_f70.pOtherColObj = pColInfoObbTree->pOtherColObj;
								local_f70.bData = puVar12->b.pData;
								local_f70.bCount = puVar12->b.count;

								COLLISION_LOG(LogLevel::VeryVerbose, "edObbTreeIntersectObbTree aCount: 0x{:x} aType: 0x{:x} bCount: 0x{:x} primSize: 0x{:x}", 
									local_f70.aCount, local_f70.aType, local_f70.bCount, local_f70.primSize);

								uVar5 = edColArrayObjectPrimPenatratingArrayQuads4(&local_f70);
								COLLISION_LOG(LogLevel::VeryVerbose, "edObbTreeIntersectObbTree edColArrayObjectPrimPenatratingArrayQuads4 result: 0x{:x}", uVar5);
								uVar13 = uVar13 | uVar5;
							}
							else {
								if ((((uVar5 == 0x40d) || (uVar5 == 0x40e)) || (uVar5 == 0x40a)) || (uVar5 == 0x40b)) {
									local_f50.pOtherColObj = pColInfoObbTree->pOtherColObj;
									local_f50.bData = (puVar12->b).pData;
									local_f50.bCount = (puVar12->b).count;
									local_f50.bSize = _gColSizeOfPrims[(puVar12->b).type];
									local_f50.bType = (puVar12->b).type;
									local_f50.pColObj = pColInfoObbTree->pColObj;
									local_f50.aData = (puVar12->a).pData;
									local_f50.bCount2 = (ushort)(puVar12->b).count;
									uVar5 = edColArrayObjectTriangles4PenatratingPrims(&local_f50);
									uVar13 = uVar13 | uVar5;
								}
								else {
									if (uVar5 == 0xe05) {
										IMPLEMENTATION_GUARD(
										local_f30.field_0x0 = (int)pColInfoObbTree->field_0x0;
										local_f30.field_0x10 = *(uint*)(puVar12 + 2);
										local_f30.field_0x4 = ZEXT24(puVar12[1]);
										local_f30.field_0xc = *(int*)(&_gColSizeOfPrims + (uint)*puVar12 * 4);
										local_f30.field_0x8 = ZEXT24(*puVar12);
										local_f30.field_0x18 = (int)pColInfoObbTree->pOtherColObj;
										local_f30.field_0x20 = *(uint*)(puVar12 + 6);
										local_f30.field_0x1c = ZEXT24(puVar12[5]);
										local_f30.field_0x14 = (int)pColInfoObbTree->field_0x8;
										uVar5 = edColArrayObjectFastPrimPenatratingArrayTriangles4Fast(&local_f30);
										uVar13 = uVar13 | uVar5;)
									}
									else {
										if (uVar5 == 0x1305) {
											IMPLEMENTATION_GUARD(
											local_f00.field_0x0 = (int)pColInfoObbTree->field_0x0;
											local_f00.field_0x10 = *(uint*)(puVar12 + 2);
											local_f00.field_0x4 = ZEXT24(puVar12[1]);
											local_f00.field_0xc = *(int*)(&_gColSizeOfPrims + (uint)*puVar12 * 4);
											local_f00.field_0x8 = ZEXT24(*puVar12);
											local_f00.field_0x18 = (int)pColInfoObbTree->pOtherColObj;
											local_f00.field_0x20 = *(uint*)(puVar12 + 6);
											local_f00.field_0x1c = ZEXT24(puVar12[5]);
											local_f00.field_0x14 = (int)pColInfoObbTree->field_0x8;
											uVar5 = edColArrayObjectFastPrimPenatratingArrayTriangles4(&local_f00);
											uVar13 = uVar13 | uVar5;)
										}
										else {
											if (uVar5 == 0x1308) {
												IMPLEMENTATION_GUARD(
												local_ed0.field_0x0 = (int)pColInfoObbTree->field_0x0;
												local_ed0.field_0x10 = *(edF32MATRIX4**)(puVar12 + 2);
												local_ed0.field_0x4 = ZEXT24(puVar12[1]);
												local_ed0.field_0xc = *(int*)(&_gColSizeOfPrims + (uint)*puVar12 * 4);
												local_ed0.field_0x8 = ZEXT24(*puVar12);
												local_ed0.field_0x14 = (int)pColInfoObbTree->pOtherColObj;
												local_ed0.field_0x1c = *(edF32VECTOR4**)(puVar12 + 6);
												local_ed0.field_0x18 = ZEXT24(puVar12[5]);
												uVar5 = edColArrayObjectFastPrimPenatratingArrayTriangles4(&local_ed0);
												uVar13 = uVar13 | uVar5;)
											}
											else {
												if (uVar5 == 0x1304) {
													IMPLEMENTATION_GUARD(
													local_eb0.field_0x0 = (int)pColInfoObbTree->field_0x0;
													local_eb0.field_0x10 = *(edF32MATRIX4**)(puVar12 + 2);
													local_eb0.field_0x4 = ZEXT24(puVar12[1]);
													local_eb0.field_0xc = *(int*)(&_gColSizeOfPrims + (uint)*puVar12 * 4);
													local_eb0.field_0x8 = ZEXT24(*puVar12);
													local_eb0.field_0x14 = (int)pColInfoObbTree->pOtherColObj;
													local_eb0.field_0x1c = *(edF32VECTOR4**)(puVar12 + 6);
													local_eb0.field_0x18 = ZEXT24(puVar12[5]);
													uVar5 = edColArrayObjectFastPrimPenatratingArrayTriangles4(&local_eb0);
													uVar13 = uVar13 | uVar5;)
												}
												else {
													if ((uVar5 == 0xd04) || (uVar5 == 0xe04)) {
														local_e90.pColObj = pColInfoObbTree->pColObj;
														local_e90.aData = puVar12->a.pData;
														local_e90.aCount = puVar12->a.count;
														local_e90.primSize = _gColSizeOfPrims[(puVar12->a).type];
														local_e90.aType = puVar12->a.type;
														local_e90.pOtherColObj = pColInfoObbTree->pOtherColObj;
														local_e90.bData = puVar12->b.pData;
														local_e90.bCount = puVar12->b.count;
														uVar5 = edColArrayObjectPrimPenatratingArrayTriangles4(&local_e90);
														uVar13 = uVar13 | uVar5;
													}
													else {
														if (uVar5 == 0x308) {
															IMPLEMENTATION_GUARD(
															local_e70.field_0x0 = (int)pColInfoObbTree->field_0x0;
															local_e70.field_0xc = (int)pColInfoObbTree->pOtherColObj;
															local_e70.field_0x8 = *(int**)(puVar12 + 2);
															local_e70.field_0x14 = *(int*)(puVar12 + 6);
															local_e70.field_0x4 = ZEXT24(puVar12[1]);
															local_e70.field_0x10 = ZEXT24(puVar12[5]);
															uVar5 = edColArrayVerticesIndexPenatratingArrayQuads4
															(&local_e70, (int)pColInfoObbTree->field_0xc);
															uVar13 = uVar13 | uVar5;)
														}
														else {
															if (uVar5 == 0x304) {
																IMPLEMENTATION_GUARD(
																local_e50._0_4_ = pColInfoObbTree->field_0x0;
																local_e50._8_4_ = *(int**)(puVar12 + 2);
																local_e3c = *(edF32TRIANGLE4**)(puVar12 + 6);
																local_e50._12_4_ = pColInfoObbTree->pOtherColObj;
																local_e50._4_4_ = (uint)puVar12[1];
																local_e40 = (uint)puVar12[5];
																uVar5 = edColArrayVerticesIndexPenatratingArrayTriangles4
																((edColARRAY_VERT_IND_TRI4*)local_e50, (int)pColInfoObbTree->field_0xc
																);
																uVar13 = uVar13 | uVar5;)
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		puVar12 = puVar12 + 1;
	}

#if 0
	if (gColConfig.bCreateProfileObj != 0) {
		uVar5 = prof_prim_col >> 4;
		if ((prof_prim_col & 1) == 0) {
			ProfileManager.ProfileManager[uVar5].field_0x0 = (long)PCR1 - ProfileManager.ProfileManager[uVar5].field_0x0;
			ProfileManager.ProfileManager[uVar5].field_0x8 =
				ProfileManager.ProfileManager[uVar5].field_0x8 + ProfileManager.ProfileManager[uVar5].field_0x0;
			pPVar9 = ProfileManager.ProfileManager + uVar5;
			if (pPVar9->field_0x10 < ProfileManager.ProfileManager[uVar5].field_0x0) {
				pPVar9->field_0x10 = pPVar9->field_0x0;
			}
		}
		else {
			ProfileManager.g_ProfileObjA_0041ed40[uVar5].field_0x0 =
				(long)PCR1 - ProfileManager.g_ProfileObjA_0041ed40[uVar5].field_0x0;
			ProfileManager.g_ProfileObjA_0041ed40[uVar5].field_0x8 =
				ProfileManager.g_ProfileObjA_0041ed40[uVar5].field_0x8 +
				ProfileManager.g_ProfileObjA_0041ed40[uVar5].field_0x0;
			pPVar9 = ProfileManager.g_ProfileObjA_0041ed40 + uVar5;
			if (pPVar9->field_0x10 < ProfileManager.g_ProfileObjA_0041ed40[uVar5].field_0x0) {
				pPVar9->field_0x10 = pPVar9->field_0x0;
			}
		}
	}
#endif
	return uVar13;
}

edColConfig* edColGetConfig(void)
{
	return &gColConfig;
}

void edColInit(void)
{
	edColOBJECT* peVar1;
	edColDbObj_80* peVar2;
	undefined* puVar3;
	byte* pbVar4;
	uint uVar5;
	void* pvVar6;
	int iVar7;
	edColDatabase* peVar8;
	int iVar9;

	if (gColConfig.bSetMemFlags != 0) {
		edMemSetFlags(gColConfig.heapID_A, 0x100);
	}

	gColTD.field_0x4 = (undefined*)edMemAlloc(gColConfig.heapID_A, gColConfig.field_0x18 * 0xc0);
	memset(gColTD.field_0x4, 0, gColConfig.field_0x18 * 0xc0);

	gColTD.aPrim = (edColPrimEntry*)edMemAlloc(gColConfig.heapID_A, gColConfig.nbPrimEntries * sizeof(edColPrimEntry));
	memset(gColTD.aPrim, 0, gColConfig.nbPrimEntries * sizeof(edColPrimEntry));

	gColTD.field_0x14 = (undefined*)edMemAlloc(gColConfig.heapID_A, gColConfig.field_0x1c << 5);
	memset(gColTD.field_0x14, 0, gColConfig.field_0x1c << 5);

	if (gColConfig.bSetMemFlags != 0) {
		edMemClearFlags(gColConfig.heapID_A, 0x100);
	}

	gColData.aDatabases = (edColDatabase*)edMemAlloc(gColConfig.heapID_B, gColConfig.databaseCount * sizeof(edColDatabase));
	memset(gColData.aDatabases, 0, gColConfig.databaseCount * sizeof(edColDatabase));

	gColData.pActiveDatabase = gColData.aDatabases;
	for (iVar9 = 0; iVar9 < (int)(uint)gColConfig.databaseCount; iVar9 = iVar9 + 1) {
		peVar8 = gColData.aDatabases + iVar9;

		peVar8->aColObj = (edColOBJECT*)edMemAlloc(gColConfig.heapID_B, gColConfig.field_0x8[iVar9] * sizeof(edColOBJECT));
		memset(peVar8->aColObj, 0, gColConfig.field_0x8[iVar9] * sizeof(edColOBJECT));

		peVar8->field_0x10 = (edColDbObj_80*)edMemAlloc(gColConfig.heapID_B, gColConfig.field_0x8[iVar9 + 2] * sizeof(edColDbObj_80));
		memset(peVar8->field_0x10, 0, gColConfig.field_0x8[iVar9 + 2] * sizeof(edColDbObj_80));

		peVar8->aPrim = (edColPrimEntry*)edMemAlloc(gColConfig.heapID_B, gColConfig.field_0x8[iVar9 + 4] * sizeof(edColPrimEntry));
		memset(peVar8->aPrim, 0, gColConfig.field_0x8[iVar9 + 4] * sizeof(edColPrimEntry));

		peVar8->field_0x1c = (undefined*)edMemAlloc(gColConfig.heapID_B, gColConfig.field_0x8[iVar9 + 6] * 0xc0);
		memset(peVar8->field_0x1c, 0, gColConfig.field_0x8[iVar9 + 6] * 0xc0);

		if (gColConfig.field_0x1 == 0) {
			peVar8->field_0x20 = (byte*)edMemAlloc(gColConfig.heapID_B, gColConfig.field_0x8[iVar9] * gColConfig.field_0x8[iVar9]);
			uVar5 = gColConfig.field_0x8[iVar9];
			memset(peVar8->field_0x20, 0, uVar5 * uVar5);

			for (iVar7 = 0; uVar5 = gColConfig.field_0x8[iVar9], iVar7 < (int)(uVar5 * uVar5);
				iVar7 = iVar7 + 1) {
				peVar8->field_0x20[iVar7] = 1;
			}

			peVar8->curObjId = 0;
		}
		else {
			peVar8->field_0x20 = (byte*)0x0;
			peVar8->curObjId = 0;
		}

		peVar8->field_0x4 = 0;
	}

	edMemSetFlags(TO_HEAP(H_MAIN), 0x8000);
	gColData.field_0x1c = (uint)gColConfig.field_0x3 << 4;
	pvVar6 = edMemAlloc(TO_HEAP(H_MAIN), gColData.field_0x1c);
	gColData.field_0x18 = (undefined*)((uint)pvVar6 | 0x20000000);
	edMemClearFlags(TO_HEAP(H_MAIN), 0x8000);

	gColTD.field_0x0 = 0;
	gColData.bInitialized = 1;
	gColTD.pCurDatabase = gColData.aDatabases;

#if 0
	if (gColConfig.bCreateProfileObj != 0) {
		/* OBB_TEST */
		prof_obb_col = edProfileNew(1, 0x80, 0x80, 0x80, s_OBB_TEST_00430060);
		/* PRIM_TEST */
		prof_prim_col = edProfileNew(1, 0x80, 0x40, 0x40, s_PRIM_TEST_00430070);
		/* FAST_TEST */
		prof_fast_col = edProfileNew(1, 0x40, 0x40, 0x80, s_FAST_TEST_00430080);
	}
#endif
	return;
}

void edColSetDataBase(int databaseId)
{
	gColData.activeDatabaseId = (byte)databaseId;
	gColData.pActiveDatabase = gColData.aDatabases + databaseId;
	return;
}

void edColBegin(int param_1, int databaseId)
{
	gColData.pActiveDatabase = gColData.aDatabases + databaseId;
	gColData.activeDatabaseId = (byte)databaseId;
	gColTD.field_0x0 = 1;
	gColTD.field_0x1 = (byte)param_1;
	gColTD.field_0x8 = gColTD.field_0x4;
	gColTD.pCurPrim = gColTD.aPrim;
	gColTD.field_0x18 = gColTD.field_0x14;
	gColTD.pCurColObj = (gColData.pActiveDatabase)->aColObj + (gColData.pActiveDatabase)->curObjId;
	gColTD.field_0x38 = 0;
	gColTD.curObjId = (gColData.pActiveDatabase)->curObjId;
	gColTD.nbPrim = 0;
	gColTD.field_0x44 = 0;
	return;
}

void edColObjectSetCollisionsType(byte param_1, byte param_2, byte param_3)
{
	(gColTD.pCurColObj)->colType_0x0 = param_1;
	(gColTD.pCurColObj)->colType_0x1 = param_2;
	(gColTD.pCurColObj)->colType_0x2 = param_3;
	return;
}

void edColCollisionAddPrim(int aType, int aCount, void* param_3, void* aData, int bType, int bCount, void* bData)
{
	((gColTD.pCurPrim)->a).pData = aData;
	((gColTD.pCurPrim)->a).type = (ushort)aType;
	((gColTD.pCurPrim)->a).count = (short)aCount;
	((gColTD.pCurPrim)->b).pData = bData;
	((gColTD.pCurPrim)->b).type = (ushort)bType;
	((gColTD.pCurPrim)->b).count = (short)bCount;
	gColTD.pCurPrim = gColTD.pCurPrim + 1;
	gColTD.nbPrim = gColTD.nbPrim + 1;
	return;
}

edColOBJECT* edColEnd(edDynOBJECT* pDynObj)
{
	undefined8 uVar1;
	int iVar2;
	ushort* puVar4;
	undefined4 uVar5;
	undefined4 uVar6;
	undefined4 uVar7;
	undefined4 uVar8;
	int iVar9;
	edColPrimEntry* pIVar10;
	undefined8* puVar11;
	undefined4 uVar12;
	undefined4 uVar13;
	undefined4 uVar14;
	int iVar15;
	undefined8* puVar16;
	undefined4* puVar17;
	undefined4* puVar18;

#if 0
	undefined local_70[112];
	undefined* puVar3;

	puVar3 = local_70;
	iVar2 = 0x70;
	if ((undefined*)register0x000001d0 != (undefined*)0x70) {
		do {
			*puVar3 = 0;
			iVar2 = iVar2 + -1;
			puVar3 = puVar3 + 1;
		} while (iVar2 != 0);
	}
#endif

	(gColTD.pCurColObj)->pDynObj = STORE_SECTION(pDynObj);

	if (gColTD.field_0x38 != 0) {
		IMPLEMENTATION_GUARD(
		(gColTD.pCurColObj)->nbPrim = (short)gColTD.field_0x38;
		(gColTD.pCurColObj)->field_0x18 =
			(gColData.pActiveDatabase)->field_0x1c + (gColData.pActiveDatabase)->field_0xc * 0xc0;
		(gColData.pActiveDatabase)->field_0xc = (gColData.pActiveDatabase)->field_0xc + gColTD.field_0x38;
		if (gColTD.field_0x1 == 2) {
			(gColTD.pCurColObj)->field_0x1c =
				(gColData.pActiveDatabase)->field_0x1c + (gColData.pActiveDatabase)->field_0xc * 0xc0;
			(gColData.pActiveDatabase)->field_0xc = (gColData.pActiveDatabase)->field_0xc + gColTD.field_0x38;
		})
	}

	if (gColTD.nbPrim != 0) {
		(gColTD.pCurColObj)->nbPrimUsed = (short)gColTD.nbPrim;

		(gColTD.pCurColObj)->pPrim = STORE_SECTION((gColData.pActiveDatabase)->aPrim + (gColData.pActiveDatabase)->curPrimId);

		(gColData.pActiveDatabase)->curPrimId = (gColData.pActiveDatabase)->curPrimId + gColTD.nbPrim;
	}

	(gColTD.pCurColObj)->field_0x4 = 1;
	(gColTD.pCurColObj)->field_0x5 = 1;

	for (iVar2 = 0; iVar2 < gColTD.field_0x38; iVar2 = iVar2 + 1) {
		IMPLEMENTATION_GUARD(
		iVar15 = iVar2 * 0xc0;
		iVar9 = 3;
		puVar16 = (undefined8*)(gColTD.field_0x4 + iVar15);
		puVar18 = (undefined4*)((gColTD.pCurColObj)->field_0x18 + iVar15);
		puVar11 = puVar16;
		puVar17 = puVar18;
		do {
			uVar1 = *puVar11;
			uVar5 = *(undefined4*)(puVar11 + 1);
			uVar7 = *(undefined4*)((int)puVar11 + 0xc);
			iVar9 = iVar9 + -1;
			*puVar17 = (int)uVar1;
			puVar17[1] = (int)((ulong)uVar1 >> 0x20);
			puVar17[2] = uVar5;
			puVar17[3] = uVar7;
			puVar11 = puVar11 + 2;
			puVar17 = puVar17 + 4;
		} while (0 < iVar9);
		uVar12 = *(undefined4*)((int)puVar16 + 0x34);
		uVar13 = *(undefined4*)(puVar16 + 7);
		uVar14 = *(undefined4*)((int)puVar16 + 0x3c);
		puVar11 = puVar16 + 10;
		uVar5 = *(undefined4*)(puVar16 + 8);
		uVar7 = *(undefined4*)((int)puVar16 + 0x44);
		uVar6 = *(undefined4*)(puVar16 + 9);
		uVar8 = *(undefined4*)((int)puVar16 + 0x4c);
		puVar17 = puVar18 + 0x14;
		iVar9 = 7;
		puVar18[0xc] = *(undefined4*)(puVar16 + 6);
		puVar18[0xd] = uVar12;
		puVar18[0xe] = uVar13;
		puVar18[0xf] = uVar14;
		puVar18[0x10] = uVar5;
		puVar18[0x11] = uVar7;
		puVar18[0x12] = uVar6;
		puVar18[0x13] = uVar8;
		do {
			uVar1 = *puVar11;
			uVar5 = *(undefined4*)(puVar11 + 1);
			uVar7 = *(undefined4*)((int)puVar11 + 0xc);
			iVar9 = iVar9 + -1;
			*puVar17 = (int)uVar1;
			puVar17[1] = (int)((ulong)uVar1 >> 0x20);
			puVar17[2] = uVar5;
			puVar17[3] = uVar7;
			puVar11 = puVar11 + 2;
			puVar17 = puVar17 + 4;
		} while (0 < iVar9);
		puVar17 = (undefined4*)((gColTD.pCurColObj)->field_0x1c + iVar15);
		if ((gColTD.pCurColObj)->field_0x1c != 0) {
			iVar9 = 3;
			puVar16 = (undefined8*)(gColTD.field_0x4 + iVar15);
			puVar18 = puVar17;
			puVar11 = puVar16;
			do {
				uVar1 = *puVar11;
				uVar5 = *(undefined4*)(puVar11 + 1);
				uVar7 = *(undefined4*)((int)puVar11 + 0xc);
				iVar9 = iVar9 + -1;
				*puVar18 = (int)uVar1;
				puVar18[1] = (int)((ulong)uVar1 >> 0x20);
				puVar18[2] = uVar5;
				puVar18[3] = uVar7;
				puVar11 = puVar11 + 2;
				puVar18 = puVar18 + 4;
			} while (0 < iVar9);
			uVar12 = *(undefined4*)((int)puVar16 + 0x34);
			uVar13 = *(undefined4*)(puVar16 + 7);
			uVar14 = *(undefined4*)((int)puVar16 + 0x3c);
			puVar11 = puVar16 + 10;
			uVar5 = *(undefined4*)(puVar16 + 8);
			uVar7 = *(undefined4*)((int)puVar16 + 0x44);
			uVar6 = *(undefined4*)(puVar16 + 9);
			uVar8 = *(undefined4*)((int)puVar16 + 0x4c);
			puVar18 = puVar17 + 0x14;
			iVar9 = 7;
			puVar17[0xc] = *(undefined4*)(puVar16 + 6);
			puVar17[0xd] = uVar12;
			puVar17[0xe] = uVar13;
			puVar17[0xf] = uVar14;
			puVar17[0x10] = uVar5;
			puVar17[0x11] = uVar7;
			puVar17[0x12] = uVar6;
			puVar17[0x13] = uVar8;
			do {
				uVar1 = *puVar11;
				uVar5 = *(undefined4*)(puVar11 + 1);
				uVar7 = *(undefined4*)((int)puVar11 + 0xc);
				iVar9 = iVar9 + -1;
				*puVar18 = (int)uVar1;
				puVar18[1] = (int)((ulong)uVar1 >> 0x20);
				puVar18[2] = uVar5;
				puVar18[3] = uVar7;
				puVar11 = puVar11 + 2;
				puVar18 = puVar18 + 4;
			} while (0 < iVar9);
		})
	}

	// Copy prims.
	for (iVar2 = 0; iVar2 < gColTD.nbPrim; iVar2 = iVar2 + 1) {
		pIVar10 = gColTD.aPrim + iVar2;
		edColPrimEntry* pPrimStart = (edColPrimEntry*)LOAD_SECTION((gColTD.pCurColObj)->pPrim);
		edColPrimEntry* peVar4 = pPrimStart + iVar2;
		*peVar4 = *pIVar10;
	}

	(gColTD.pCurColObj)->field_0x4 = 1;
	(gColTD.pCurColObj)->field_0x5 = 1;
	(gColTD.pCurColObj)->field_0xc = (gColData.pActiveDatabase)->curObjId;
	gColTD.pCurColObj = gColTD.pCurColObj + 1;
	(gColData.pActiveDatabase)->curObjId = (gColData.pActiveDatabase)->curObjId + 1;
	gColTD.field_0x0 = 0;
	return gColTD.pCurColObj + -1;
}

uint VectorFunc_00254520(edObbTREE_DYN* pObbTree, edF32VECTOR4* param_2, edF32VECTOR4* param_3)
{
	float fVar1;
	float fVar2;
	float fVar4;
	float fVar5;
	float fVar6;
	float fVar7;
	float fVar8;
	float fVar9;
	float fVar10;
	float fVar11;
	float fVar12;
	float fVar13;
	float fVar14;
	float fVar15;
	float fVar16;
	float fVar17;
	float fVar18;
	float fVar19;
	float fVar20;
	float fVar21;
	float fVar22;
	float fVar23;

	fVar4 = (pObbTree->bbox).transform.ac;
	fVar5 = (pObbTree->bbox).transform.ba;
	fVar6 = (pObbTree->bbox).transform.bb;
	fVar7 = (pObbTree->bbox).transform.bc;
	fVar8 = (pObbTree->bbox).transform.ca;
	fVar9 = (pObbTree->bbox).transform.cb;
	fVar10 = (pObbTree->bbox).transform.cc;
	fVar13 = (pObbTree->bbox).transform.da;
	fVar14 = (pObbTree->bbox).transform.db;
	fVar15 = (pObbTree->bbox).transform.dc;
	fVar16 = param_2->x;
	fVar17 = param_2->y;
	fVar18 = param_2->z;
	fVar19 = param_2->w;
	fVar20 = param_3->x;
	fVar21 = param_3->y;
	fVar22 = param_3->z;
	fVar23 = param_3->w;
	fVar12 = (pObbTree->bbox).transform.ab;
	fVar11 = (pObbTree->bbox).transform.aa;
	fVar1 = 0.0 - (fVar11 * fVar13 + fVar12 * fVar14 + fVar4 * fVar15);
	fVar2 = 0.0 - (fVar5 * fVar13 + fVar6 * fVar14 + fVar7 * fVar15);
	fVar15 = 0.0 - (fVar8 * fVar13 + fVar9 * fVar14 + fVar10 * fVar15);
	fVar13 = fVar11 * fVar16 + fVar12 * fVar17 + fVar4 * fVar18 + fVar1 * fVar19;
	fVar14 = fVar5 * fVar16 + fVar6 * fVar17 + fVar7 * fVar18 + fVar2 * fVar19;
	fVar16 = fVar8 * fVar16 + fVar9 * fVar17 + fVar10 * fVar18 + fVar15 * fVar19;
	fVar1 = fVar11 * fVar20 + fVar12 * fVar21 + fVar4 * fVar22 + fVar1 * fVar23;
	fVar2 = fVar5 * fVar20 + fVar6 * fVar21 + fVar7 * fVar22 + fVar2 * fVar23;
	fVar15 = fVar8 * fVar20 + fVar9 * fVar21 + fVar10 * fVar22 + fVar15 * fVar23;
	fVar12 = (pObbTree->bbox).width;
	fVar11 = (pObbTree->bbox).height;
	fVar4 = (pObbTree->bbox).depth;
	return (uint)(fVar12 < fVar13) + (((uint)(fVar13 < -fVar12) << 0x17) >> 0x16) +
		(((uint)(fVar11 < fVar14) << 0x17) >> 0x15) + (((uint)(fVar14 < -fVar11) << 0x17) >> 0x14) +
		(((uint)(fVar4 < fVar16) << 0x17) >> 0x13) + (((uint)(fVar16 < -fVar4) << 0x17) >> 0x12) &
		(uint)(fVar12 < fVar1) + (((uint)(fVar1 < -fVar12) << 0x17) >> 0x16) +
		(((uint)(fVar11 < fVar2) << 0x17) >> 0x15) + (((uint)(fVar2 < -fVar11) << 0x17) >> 0x14) +
		(((uint)(fVar4 < fVar15) << 0x17) >> 0x13) + (((uint)(fVar15 < -fVar4) << 0x17) >> 0x12);
}

int INT_ARRAY_00448910[2] = { 1, 0 };

struct edColPRIM_RAY_UNIT_SPHERE_UNIT_IN {
	edF32VECTOR4* field_0x0;
	edF32VECTOR4* field_0x4;
};

void edColIntersectRayUnitSphereUnit(edColINFO_OUT* pColInfoOut, edColPRIM_RAY_UNIT_SPHERE_UNIT_IN* pParams)
{
	edF32VECTOR4* peVar1;
	edF32VECTOR4* peVar2;
	edF32VECTOR4* peVar3;
	float fVar4;
	float fVar5;
	float fVar6;
	edF32VECTOR4 local_30;
	edF32VECTOR4 local_20;
	edF32VECTOR4 local_10;

	pColInfoOut->result = 0;

	edF32Vector4NormalizeHard_Fixed(&local_10, pParams->field_0x4);

	peVar1 = pParams->field_0x0;
	peVar2 = pParams->field_0x0;
	peVar3 = pParams->field_0x0;
	fVar6 = (peVar1->x * local_10.x + peVar1->y * local_10.y + peVar1->z * local_10.z) * -2.0f;
	fVar4 = (peVar2->x * peVar3->x + peVar2->y * peVar3->y + peVar2->z * peVar3->z) - 1.0f;
	if (0.0f <= fVar6 * fVar6 - fVar4 * 4.0f) {
		fVar5 = (fVar6 - sqrtf(fVar6 * fVar6 - fVar4 * 4.0f)) * 0.5f;
		fVar4 = (fVar6 + sqrtf(fVar6 * fVar6 - fVar4 * 4.0f)) * 0.5f;
		if ((fVar5 <= 0.0f) || (fVar4 <= 0.0f)) {
			if ((fVar5 < 0.0f) && (fVar4 < 0.0f)) {
				return;
			}
			if (fVar5 <= fVar4) {
				fVar5 = fVar4;
			}
		}
		else {
			if (fVar4 <= fVar5) {
				fVar5 = fVar4;
			}
		}
		peVar1 = pParams->field_0x0;
		local_20.x = local_10.x * fVar5 + peVar1->x;
		local_20.y = local_10.y * fVar5 + peVar1->y;
		local_20.z = local_10.z * fVar5 + peVar1->z;
		local_20.w = 1.0f;
		fVar4 = edF32Vector4NormalizeHard_Fixed(&local_30, &local_20);
		pColInfoOut->field_0x44 = fVar4 - 1.0f;
		(pColInfoOut->field_0x20).x = local_20.x;
		(pColInfoOut->field_0x20).y = local_20.y;
		(pColInfoOut->field_0x20).z = local_20.z;
		(pColInfoOut->field_0x20).w = local_20.w;
		(pColInfoOut->field_0x10).x = local_30.x;
		(pColInfoOut->field_0x10).y = local_30.y;
		(pColInfoOut->field_0x10).z = local_30.z;
		(pColInfoOut->field_0x10).w = local_30.w;
		pColInfoOut->result = 1;
	}
	return;
}

float edObbIntersectObbTreeRayPrim(void** pOutHit, uint* pOutType, edObbTREE_DYN* pObbTree, edColRAY_OBB_IN* pRay)
{
	byte bVar1;
	edF32VECTOR4* pDirection;
	edF32VECTOR4* pLocation;
	edObbTREE_DYN* peVar4;
	bool bVar5;
	uint uVar6;
	int iVar7;
	uint uVar8;
	edObbTREE_DYN* peVar9;
	int iVar10;
	int iVar11;
	float fVar12;
	float puVar13;
	float fVar13;
	float fVar14;
	float fVar15;
	float fVar16;
	float fVar17;
	float fVar18;
	float fVar19;
	float fVar20;
	float fVar21;
	float fVar22;
	float fVar23;
	float fVar24;
	float fVar25;
	float fVar26;
	float fVar27;
	edF32VECTOR4 local_610;
	float local_600;
	float fStack1532;
	float fStack1528;
	float fStack1524;
	float local_5f0;
	float fStack1516;
	float fStack1512;
	float fStack1508;
	float local_5e0;
	float fStack1500;
	float fStack1496;
	float fStack1492;
	undefined auStack1488[32];
	float local_5b0;
	float fStack1452;
	float fStack1448;
	float fStack1444;
	int local_590;
	edF32VECTOR4 local_570;
	float local_560;
	float fStack1372;
	float fStack1368;
	float fStack1364;
	float local_550;
	float fStack1356;
	float fStack1352;
	float fStack1348;
	float local_540;
	float fStack1340;
	float fStack1336;
	float fStack1332;
	undefined auStack1328[32];
	float local_510;
	float fStack1292;
	float fStack1288;
	float fStack1284;
	int local_4f0;
	edF32VECTOR4 local_4d0;
	float local_4c0;
	float fStack1212;
	float fStack1208;
	float fStack1204;
	float local_4b0;
	float fStack1196;
	float fStack1192;
	float fStack1188;
	edF32VECTOR4 local_4a0;
	//TraceData48 TStack1168;
	edF32VECTOR4 local_430;
	float local_420;
	float fStack1052;
	float fStack1048;
	float fStack1044;
	edF32VECTOR4 local_410;
	edF32VECTOR4 local_400;
	//TraceData48 TStack1008;
	undefined local_390[20];
	float local_37c;
	float local_378;
	edF32VECTOR4 endLocation;
	edObbTREE_DYN* local_360[202];
	float* local_38;
	float* local_34;
	float* local_30;
	float* local_2c;
	edF32VECTOR4* local_28;
	float* local_24;
	edF32VECTOR4* local_20[2];
	int local_18[2];
	edColRAY_TRIANGLE4_IN local_10;
	float local_4;

	puVar13 = -1.0f;
	uVar8 = 0;
	local_4 = -1.0f;
	local_18[0] = INT_ARRAY_00448910[0];
	local_18[1] = INT_ARRAY_00448910[1];
	fVar12 = pRay->lengthA;
	pDirection = pRay->pDirection;
	pLocation = pRay->pLocation;
	endLocation.xyz = (pDirection->xyz * fVar12) + pLocation->xyz;
	endLocation.w = 1.0f;
	local_360[0] = pObbTree;
	uVar6 = VectorFunc_00254520(pObbTree, pRay->pLocation, &endLocation);
	if (uVar6 == 0) {
		bVar5 = false;
		do {
			iVar11 = 0;
			local_18[bVar5 ^ 1] = 0;
			while (iVar11 < local_18[uVar8]) {
				peVar4 = local_360[uVar8 * 100 + iVar11];
				bVar1 = peVar4->type;
				if (bVar1 == 1) {
					for (iVar7 = 0; iVar7 < peVar4->count_0x52; iVar7 = iVar7 + 1) {
						uVar6 = VectorFunc_00254520(LOAD_SECTION_CAST(edObbTREE_DYN*, peVar4->field_0x54[iVar7]), pRay->pLocation, &endLocation);
						if (uVar6 == 0) {
							uVar6 = uVar8 != 0 ^ 1;
							if (local_18[uVar6] < 100) {
								local_360[uVar6 * 100 + local_18[uVar6]] = LOAD_SECTION_CAST(edObbTREE_DYN*, peVar4->field_0x54[iVar7]);
								local_18[uVar6] = local_18[uVar6] + 1;
							}
						}
					}
					iVar11 = iVar11 + 1;
				}
				else {
					if (bVar1 == 0xd) {
						edColPRIM_OBJECT* pPrim = LOAD_SECTION_CAST(edColPRIM_OBJECT*, peVar4->field_0x54[0]);
						for (iVar7 = 0; iVar7 < peVar4->count_0x52; iVar7 = iVar7 + 1) {
							pDirection = pRay->pDirection;
							pLocation = pRay->pLocation;

							fVar22 = pDirection->x;
							fVar24 = pDirection->y;
							fVar26 = pDirection->z;
							float fVar28 = pDirection->w;
							float fVar30 = pLocation->x;
							float fVar32 = pLocation->y;
							float fVar34 = pLocation->z;
							float fVar36 = pLocation->w;

							fVar13 = (pPrim->matrix_0x40).aa;
							fVar14 = (pPrim->matrix_0x40).ab;
							fVar15 = (pPrim->matrix_0x40).ac;
							fVar16 = (pPrim->matrix_0x40).ad;
							fVar17 = (pPrim->matrix_0x40).ba;
							fVar18 = (pPrim->matrix_0x40).bb;
							fVar19 = (pPrim->matrix_0x40).bc;
							fVar20 = (pPrim->matrix_0x40).bd;
							fVar21 = (pPrim->matrix_0x40).ca;
							fVar23 = (pPrim->matrix_0x40).cb;
							fVar25 = (pPrim->matrix_0x40).cc;
							fVar27 = (pPrim->matrix_0x40).cd;
							float fVar29 = (pPrim->matrix_0x40).da;
							float fVar31 = (pPrim->matrix_0x40).db;
							float fVar33 = (pPrim->matrix_0x40).dc;
							float fVar35 = (pPrim->matrix_0x40).dd;

							edF32VECTOR4 local_5e0;
							edF32VECTOR4 local_5f0;

							local_5e0.x = fVar13 * fVar22 + fVar17 * fVar24 + fVar21 * fVar26 + fVar29 * fVar28;
							local_5e0.y = fVar14 * fVar22 + fVar18 * fVar24 + fVar23 * fVar26 + fVar31 * fVar28;
							local_5e0.z = fVar15 * fVar22 + fVar19 * fVar24 + fVar25 * fVar26 + fVar33 * fVar28;
							local_5e0.w = fVar16 * fVar22 + fVar20 * fVar24 + fVar27 * fVar26 + fVar35 * fVar28;
							local_5f0.x = fVar13 * fVar30 + fVar17 * fVar32 + fVar21 * fVar34 + fVar29 * fVar36;
							local_5f0.y = fVar14 * fVar30 + fVar18 * fVar32 + fVar23 * fVar34 + fVar31 * fVar36;
							local_5f0.z = fVar15 * fVar30 + fVar19 * fVar32 + fVar25 * fVar34 + fVar33 * fVar36;
							local_5f0.w = fVar16 * fVar30 + fVar20 * fVar32 + fVar27 * fVar34 + fVar35 * fVar36;

							edColPRIM_RAY_UNIT_BOX_UNIT_IN local_38;
							local_38.field_0x4 = &local_5f0;
							local_38.field_0x0 = &local_5e0;

							edColINFO_OUT eStack1488;

							edColIntersectRayUnitBoxUnit(&eStack1488, &local_38);

							if (eStack1488.result != 0) {
								local_600 = (pPrim->matrix_0x0).aa * eStack1488.field_0x20.x +
									(pPrim->matrix_0x0).ba * eStack1488.field_0x20.y +
									(pPrim->matrix_0x0).ca * eStack1488.field_0x20.z +
									(pPrim->matrix_0x0).da * eStack1488.field_0x20.w;
								fStack1532 = (pPrim->matrix_0x0).ab * eStack1488.field_0x20.x +
									(pPrim->matrix_0x0).bb * eStack1488.field_0x20.y +
									(pPrim->matrix_0x0).cb * eStack1488.field_0x20.z +
									(pPrim->matrix_0x0).db * eStack1488.field_0x20.w;
								fStack1528 = (pPrim->matrix_0x0).ac * eStack1488.field_0x20.x +
									(pPrim->matrix_0x0).bc * eStack1488.field_0x20.y +
									(pPrim->matrix_0x0).cc * eStack1488.field_0x20.z +
									(pPrim->matrix_0x0).dc * eStack1488.field_0x20.w;
								fStack1524 = (pPrim->matrix_0x0).ad * eStack1488.field_0x20.x +
									(pPrim->matrix_0x0).bd * eStack1488.field_0x20.y +
									(pPrim->matrix_0x0).cd * eStack1488.field_0x20.z +
									(pPrim->matrix_0x0).dd * eStack1488.field_0x20.w;

								pLocation = pRay->pLocation;

								local_610.x = local_600 - pLocation->x;
								local_610.y = fStack1532 - pLocation->y;
								local_610.z = fStack1528 - pLocation->z;
								local_610.w = fStack1524 - pLocation->w;

								local_4 = edF32Vector4GetDistHard(&local_610);
								if ((local_4 < puVar13) || ((puVar13 < 0.0f && (local_4 <= pRay->lengthA)))) {
									*pOutHit = pPrim;
									*pOutType = 10;
									puVar13 = local_4;
								}
							}

							pPrim = pPrim + 1;
						}
					}
					else {
						if (bVar1 == 0xa) {
							edColPRIM_BOX* pBox = LOAD_SECTION_CAST(edColPRIM_BOX*, peVar4->field_0x54[0]);
							for (iVar7 = 0; iVar7 < peVar4->count_0x52; iVar7 = iVar7 + 1) {
								pDirection = pRay->pDirection;
								pLocation = pRay->pLocation;

								fVar22 = pDirection->x;
								fVar24 = pDirection->y;
								fVar26 = pDirection->z;
								float fVar28 = pDirection->w;
								float fVar30 = pLocation->x;
								float fVar32 = pLocation->y;
								float fVar34 = pLocation->z;
								float fVar36 = pLocation->w;

								fVar13 = (pBox->matrix_0x40).aa;
								fVar14 = (pBox->matrix_0x40).ab;
								fVar15 = (pBox->matrix_0x40).ac;
								fVar16 = (pBox->matrix_0x40).ad;
								fVar17 = (pBox->matrix_0x40).ba;
								fVar18 = (pBox->matrix_0x40).bb;
								fVar19 = (pBox->matrix_0x40).bc;
								fVar20 = (pBox->matrix_0x40).bd;
								fVar21 = (pBox->matrix_0x40).ca;
								fVar23 = (pBox->matrix_0x40).cb;
								fVar25 = (pBox->matrix_0x40).cc;
								fVar27 = (pBox->matrix_0x40).cd;
								float fVar29 = (pBox->matrix_0x40).da;
								float fVar31 = (pBox->matrix_0x40).db;
								float fVar33 = (pBox->matrix_0x40).dc;
								float fVar35 = (pBox->matrix_0x40).dd;

								edF32VECTOR4 local_5e0;
								edF32VECTOR4 local_5f0;

								local_5e0.x = fVar13 * fVar22 + fVar17 * fVar24 + fVar21 * fVar26 + fVar29 * fVar28;
								local_5e0.y = fVar14 * fVar22 + fVar18 * fVar24 + fVar23 * fVar26 + fVar31 * fVar28;
								local_5e0.z = fVar15 * fVar22 + fVar19 * fVar24 + fVar25 * fVar26 + fVar33 * fVar28;
								local_5e0.w = fVar16 * fVar22 + fVar20 * fVar24 + fVar27 * fVar26 + fVar35 * fVar28;
								local_5f0.x = fVar13 * fVar30 + fVar17 * fVar32 + fVar21 * fVar34 + fVar29 * fVar36;
								local_5f0.y = fVar14 * fVar30 + fVar18 * fVar32 + fVar23 * fVar34 + fVar31 * fVar36;
								local_5f0.z = fVar15 * fVar30 + fVar19 * fVar32 + fVar25 * fVar34 + fVar33 * fVar36;
								local_5f0.w = fVar16 * fVar30 + fVar20 * fVar32 + fVar27 * fVar34 + fVar35 * fVar36;

								edColPRIM_RAY_UNIT_BOX_UNIT_IN local_38;
								local_38.field_0x4 = &local_5f0;
								local_38.field_0x0 = &local_5e0;

								edColINFO_OUT eStack1488;

								edColIntersectRayUnitBoxUnit(&eStack1488, &local_38);

								if (eStack1488.result != 0) {
									local_600 = (pBox->matrix_0x0).aa * eStack1488.field_0x20.x +
										(pBox->matrix_0x0).ba * eStack1488.field_0x20.y +
										(pBox->matrix_0x0).ca * eStack1488.field_0x20.z +
										(pBox->matrix_0x0).da * eStack1488.field_0x20.w;
									fStack1532 = (pBox->matrix_0x0).ab * eStack1488.field_0x20.x +
										(pBox->matrix_0x0).bb * eStack1488.field_0x20.y +
										(pBox->matrix_0x0).cb * eStack1488.field_0x20.z +
										(pBox->matrix_0x0).db * eStack1488.field_0x20.w;
									fStack1528 = (pBox->matrix_0x0).ac * eStack1488.field_0x20.x +
										(pBox->matrix_0x0).bc * eStack1488.field_0x20.y +
										(pBox->matrix_0x0).cc * eStack1488.field_0x20.z +
										(pBox->matrix_0x0).dc * eStack1488.field_0x20.w;
									fStack1524 = (pBox->matrix_0x0).ad * eStack1488.field_0x20.x +
										(pBox->matrix_0x0).bd * eStack1488.field_0x20.y +
										(pBox->matrix_0x0).cd * eStack1488.field_0x20.z +
										(pBox->matrix_0x0).dd * eStack1488.field_0x20.w;

									pLocation = pRay->pLocation;

									local_610.x = local_600 - pLocation->x;
									local_610.y = fStack1532 - pLocation->y;
									local_610.z = fStack1528 - pLocation->z;
									local_610.w = fStack1524 - pLocation->w;

									local_4 = edF32Vector4GetDistHard(&local_610);
									if ((local_4 < puVar13) || ((puVar13 < 0.0f && (local_4 <= pRay->lengthA)))) {
										*pOutHit = pBox;
										*pOutType = 10;
										puVar13 = local_4;
									}
								}

								pBox = pBox + 1;
							}
						}
						else {
							if (bVar1 == 0xe) {
								IMPLEMENTATION_GUARD(
								peVar9 = peVar4->field_0x54;
								for (iVar7 = 0; iVar7 < (int)(uint)peVar4->count_0x52; iVar7 = iVar7 + 1) {
									pDirection = pRay->pDirection;
									pLocation = pRay->pLocation;
									fVar20 = pLocation->x;
									fVar21 = pLocation->y;
									fVar22 = pLocation->z;
									fVar23 = pLocation->w;
									fVar24 = pDirection->x;
									fVar25 = pDirection->y;
									fVar26 = pDirection->z;
									fVar27 = pDirection->w;
									fVar12 = (peVar9->bbox).width;
									fVar13 = (peVar9->bbox).height;
									fVar14 = (peVar9->bbox).depth;
									fVar15 = (peVar9->bbox).field_0x4c;
									fVar16 = (peVar9->matrix_0x70).aa;
									fVar17 = (peVar9->matrix_0x70).ab;
									fVar18 = (peVar9->matrix_0x70).ac;
									fVar19 = (peVar9->matrix_0x70).ad;
									local_4a0.x = fVar12 * fVar20 + *(float*)&peVar9->field_0x50 * fVar21 +
										*(float*)&peVar9->field_0x60 * fVar22 + fVar16 * fVar23;
									local_4a0.y = fVar13 * fVar20 + (float)peVar9->field_0x54 * fVar21 +
										*(float*)&peVar9->field_0x64 * fVar22 + fVar17 * fVar23;
									local_4a0.z = fVar14 * fVar20 + *(float*)&peVar9->field_0x58 * fVar21 +
										*(float*)&peVar9->field_0x68 * fVar22 + fVar18 * fVar23;
									local_4a0.w = fVar15 * fVar20 + *(float*)&peVar9->field_0x5c * fVar21 +
										*(float*)&peVar9->field_0x6c * fVar22 + fVar19 * fVar23;
									local_4b0 = fVar12 * fVar24 + *(float*)&peVar9->field_0x50 * fVar25 +
										*(float*)&peVar9->field_0x60 * fVar26 + fVar16 * fVar27;
									fStack1196 = fVar13 * fVar24 + (float)peVar9->field_0x54 * fVar25 +
										*(float*)&peVar9->field_0x64 * fVar26 + fVar17 * fVar27;
									fStack1192 = fVar14 * fVar24 + *(float*)&peVar9->field_0x58 * fVar25 +
										*(float*)&peVar9->field_0x68 * fVar26 + fVar18 * fVar27;
									fStack1188 = fVar15 * fVar24 + *(float*)&peVar9->field_0x5c * fVar25 +
										*(float*)&peVar9->field_0x6c * fVar26 + fVar19 * fVar27;
									local_24 = &local_4b0;
									local_28 = &local_4a0;
									edColIntersectRayUnitSphereUnit(&TStack1168, &local_28);
									if (TStack1168.field_0x40 != 0) {
										local_4c0 = (peVar9->bbox).transform.aa * TStack1168.field_0x20 +
											(peVar9->bbox).transform.ba * TStack1168.field_0x24 +
											(peVar9->bbox).transform.ca * TStack1168.field_0x28 +
											(peVar9->bbox).transform.da * TStack1168.field_0x2c;
										fStack1212 = (peVar9->bbox).transform.ab * TStack1168.field_0x20 +
											(peVar9->bbox).transform.bb * TStack1168.field_0x24 +
											(peVar9->bbox).transform.cb * TStack1168.field_0x28 +
											(peVar9->bbox).transform.db * TStack1168.field_0x2c;
										fStack1208 = (peVar9->bbox).transform.ac * TStack1168.field_0x20 +
											(peVar9->bbox).transform.bc * TStack1168.field_0x24 +
											(peVar9->bbox).transform.cc * TStack1168.field_0x28 +
											(peVar9->bbox).transform.dc * TStack1168.field_0x2c;
										fStack1204 = (peVar9->bbox).transform.ad * TStack1168.field_0x20 +
											(peVar9->bbox).transform.bd * TStack1168.field_0x24 +
											(peVar9->bbox).transform.cd * TStack1168.field_0x28 +
											(peVar9->bbox).transform.dd * TStack1168.field_0x2c;
										pDirection = pRay->pLocation;
										local_4d0.x = local_4c0 - pDirection->x;
										local_4d0.y = fStack1212 - pDirection->y;
										local_4d0.z = fStack1208 - pDirection->z;
										local_4d0.w = fStack1204 - pDirection->w;
										local_4 = (undefined*)edF32Vector4GetDistHard(&local_4d0);
										if ((local_4 < puVar13) ||
											((puVar13 < 0.0 && (local_4 <= pRay->lengthA)))) {
											*pOutHit = peVar9;
											*pOutType = 0xb;
											puVar13 = local_4;
										}
									}
									peVar9 = (edObbTREE_DYN*)&peVar9[1].matrix_0x70.ca;
								})
							}
							else {
								if (bVar1 == 0xb) {
									edColPRIM_SPHERE* pSphere = LOAD_SECTION_CAST(edColPRIM_SPHERE*, peVar4->field_0x54[0]);
									for (iVar7 = 0; iVar7 < peVar4->count_0x52; iVar7 = iVar7 + 1) {
										pDirection = pRay->pDirection;
										pLocation = pRay->pLocation;

										float fVar29 = pLocation->x;
										float fVar30 = pLocation->y;
										float fVar31 = pLocation->z;
										float fVar32 = pLocation->w;
										float fVar33 = pDirection->x;
										float fVar34 = pDirection->y;
										float fVar35 = pDirection->z;
										float fVar36 = pDirection->w;

										fVar13 = (pSphere->matrix_0x40).aa;
										fVar14 = (pSphere->matrix_0x40).ab;
										fVar15 = (pSphere->matrix_0x40).ac;
										fVar16 = (pSphere->matrix_0x40).ad;
										fVar17 = (pSphere->matrix_0x40).ba;
										fVar18 = (pSphere->matrix_0x40).bb;
										fVar19 = (pSphere->matrix_0x40).bc;
										fVar20 = (pSphere->matrix_0x40).bd;
										fVar21 = (pSphere->matrix_0x40).ca;
										fVar22 = (pSphere->matrix_0x40).cb;
										fVar23 = (pSphere->matrix_0x40).cc;
										fVar24 = (pSphere->matrix_0x40).cd;
										fVar25 = (pSphere->matrix_0x40).da;
										fVar26 = (pSphere->matrix_0x40).db;
										fVar27 = (pSphere->matrix_0x40).dc;
										float fVar28 = (pSphere->matrix_0x40).dd;

										local_400.x = fVar13 * fVar29 + fVar17 * fVar30 + fVar21 * fVar31 + fVar25 * fVar32;
										local_400.y = fVar14 * fVar29 + fVar18 * fVar30 + fVar22 * fVar31 + fVar26 * fVar32;
										local_400.z = fVar15 * fVar29 + fVar19 * fVar30 + fVar23 * fVar31 + fVar27 * fVar32;
										local_400.w = fVar16 * fVar29 + fVar20 * fVar30 + fVar24 * fVar31 + fVar28 * fVar32;
										local_410.x = fVar13 * fVar33 + fVar17 * fVar34 + fVar21 * fVar35 + fVar25 * fVar36;
										local_410.y = fVar14 * fVar33 + fVar18 * fVar34 + fVar22 * fVar35 + fVar26 * fVar36;
										local_410.z = fVar15 * fVar33 + fVar19 * fVar34 + fVar23 * fVar35 + fVar27 * fVar36;
										local_410.w = fVar16 * fVar33 + fVar20 * fVar34 + fVar24 * fVar35 + fVar28 * fVar36;

										edColPRIM_RAY_UNIT_SPHERE_UNIT_IN local_20;
										edColINFO_OUT eStack1008;

										local_20.field_0x4 = &local_410;
										local_20.field_0x0 = &local_400;
										edColIntersectRayUnitSphereUnit(&eStack1008, &local_20);

										if (eStack1008.result != 0) {
											local_420 = (pSphere->matrix_0x0).aa * eStack1008.field_0x20.x +
												(pSphere->matrix_0x0).ba * eStack1008.field_0x20.y +
												(pSphere->matrix_0x0).ca * eStack1008.field_0x20.z +
												(pSphere->matrix_0x0).da * eStack1008.field_0x20.w;
											fStack1052 = (pSphere->matrix_0x0).ab * eStack1008.field_0x20.x +
												(pSphere->matrix_0x0).bb * eStack1008.field_0x20.y +
												(pSphere->matrix_0x0).cb * eStack1008.field_0x20.z +
												(pSphere->matrix_0x0).db * eStack1008.field_0x20.w;
											fStack1048 = (pSphere->matrix_0x0).ac * eStack1008.field_0x20.x +
												(pSphere->matrix_0x0).bc * eStack1008.field_0x20.y +
												(pSphere->matrix_0x0).cc * eStack1008.field_0x20.z +
												(pSphere->matrix_0x0).dc * eStack1008.field_0x20.w;
											fStack1044 = (pSphere->matrix_0x0).ad * eStack1008.field_0x20.x +
												(pSphere->matrix_0x0).bd * eStack1008.field_0x20.y +
												(pSphere->matrix_0x0).cd * eStack1008.field_0x20.z +
												(pSphere->matrix_0x0).dd * eStack1008.field_0x20.w;

											pDirection = pRay->pLocation;
											local_430.x = local_420 - pDirection->x;
											local_430.y = fStack1052 - pDirection->y;
											local_430.z = fStack1048 - pDirection->z;
											local_430.w = fStack1044 - pDirection->w;
											local_4 = edF32Vector4GetDistHard(&local_430);

											if ((local_4 < puVar13) || ((puVar13 < 0.0f && (local_4 <= pRay->lengthA)))) {
												*pOutHit = (edObbTREE_DYN*)pSphere;
												*pOutType = 0xb;
												puVar13 = local_4;
											}
										}

										pSphere = pSphere + 1;
									}
								}
								else {
									if (bVar1 == 8) {
										edF32QUAD4* pQuad = LOAD_SECTION_CAST(edF32QUAD4*, peVar4->field_0x54[0]);

										for (iVar7 = 0; iVar7 < peVar4->count_0x52; iVar7 = iVar7 + 1) {

											edF32TRIANGLE4 triangle;

											for (iVar10 = 0; iVar10 < 2; iVar10 = iVar10 + 1) {
												if (iVar10 == 0) {
													triangle.p1 = pQuad->p1;
													triangle.p2 = pQuad->p2;
													triangle.p3 = pQuad->p3;
												}
												else {
													triangle.p1 = pQuad->p1;
													triangle.p2 = pQuad->p3;
													triangle.p3 = pQuad->p4;
												}

												triangle.flags = pQuad->flags;
												local_10.pTriangle = &triangle;
												local_10.field_0x0 = pRay->pLocation;
												local_10.field_0x4 = pRay->pDirection;

												edColIntersectRayTriangle4(&local_4, &local_10);

												if ((((local_4 != -8888.0f) && (local_4 != -9999.0f)) && (0.0f <= local_4)) &&
													((local_4 < puVar13 || ((puVar13 < 0.0f && (local_4 <= pRay->lengthA)))))) {
													*pOutHit = pQuad;
													*pOutType = 8;
													puVar13 = local_4;
												}
											}

											pQuad = pQuad + 1;
										}
									}
									else {
										if (bVar1 == 5) {
											IMPLEMENTATION_GUARD(
											peVar9 = peVar4->field_0x54;
											for (iVar7 = 0; iVar7 < (int)(uint)peVar4->count_0x52; iVar7 = iVar7 + 1) {
												local_10.pTriangle = (edF32TRIANGLE4*)(local_390 + 0x10);
												local_390._16_4_ =
													(float)(*(int*)(pRay->field_0xc + 0x28) +
														(uint) * (ushort*)&(peVar9->bbox).transform.aa * 0x10);
												local_37c = (float)(*(int*)(pRay->field_0xc + 0x28) +
													(uint) * (ushort*)((int)&(peVar9->bbox).transform.aa + 2) * 0x10);
												local_378 = (float)(*(int*)(pRay->field_0xc + 0x28) +
													(uint) * (ushort*)&(peVar9->bbox).transform.ab * 0x10);
												local_10.field_0x0 = pRay->pLocation;
												local_10.field_0x4 = pRay->pDirection;
												edColIntersectRayTriangle4((float*)&local_4, &local_10);
												if ((((local_4 != -8888.0) && (local_4 != -9999.0)) && (0.0 <= local_4)) &&
													((local_4 < puVar13 ||
														((puVar13 < 0.0 && (local_4 <= pRay->lengthA)))))) {
													*pOutHit = peVar9;
													*pOutType = 5;
													puVar13 = local_4;
												}
												peVar9 = (edObbTREE_DYN*)&(peVar9->bbox).transform.bc;
											})
										}
										else {
											if (bVar1 == 4) {
												edF32TRIANGLE4* pTriangle = LOAD_SECTION_CAST(edF32TRIANGLE4*, peVar4->field_0x54[0]);
												for (iVar7 = 0; iVar7 < peVar4->count_0x52; iVar7 = iVar7 + 1) {
													local_10.field_0x0 = pRay->pLocation;
													local_10.field_0x4 = pRay->pDirection;
													local_10.pTriangle = pTriangle;

													edColIntersectRayTriangle4(&local_4, &local_10);

													if ((((local_4 != -8888.0f) && (local_4 != -9999.0f)) && (0.0f <= local_4))
														&& ((local_4 < puVar13 || ((puVar13 < 0.0f && (local_4 <= pRay->lengthA)))))) {
														*pOutHit = pTriangle;
														*pOutType = 4;
														puVar13 = local_4;
													}

													pTriangle = pTriangle + 1;
												}
											}
										}
									}
								}
							}
						}
					}
					iVar11 = iVar11 + 1;
				}
			}
			uVar8 = uVar8 != 0 ^ 1;
			bVar5 = uVar8 != 0;
		} while (local_18[uVar8] != 0);
	}
	return puVar13;
}