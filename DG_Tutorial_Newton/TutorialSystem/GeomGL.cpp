/*
* Copyright(c) < 1999 - 2019 > <Dave Gravel, Free Tutorial - Newton Dynamics and more.>
*
* This software is provided 'as-is', without any express or implied
* warranty.In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
*including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions :
*
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software.If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*/
/*
  Tutorial for show how to use newton dynamics api with pure gl write by Dave Gravel - 2019.
  This tutorial if the first version, Some parts is quickly coded and need a better implementation.
  I have try to add the most options possible without have very complex code.
  I can surely update this tutorial later with better features and implementation.
  I think for the first version it already touch pretty much all subject about newton and GL.
  Have Fun...
  Dave Gravel.
*/
#include "pch.h"
#include "GeomGL.h"
#include "WindowGL.h"

GeomBase::GeomBase()
: aDataPointer(NULL),
  aClassID(0),
  IsPicked(false)
  //aMatrix(dGetIdentityMatrix())
  //aVelocity(dVector(0.0f, 0.0f, 0.0f, 0.0f)),
  //aOmega(dVector(0.0f, 0.0f, 0.0f, 0.0f)),
  //aForce(dVector(0.0f, 0.0f, 0.0f, 0.0f))
{
  
}

/*
dMatrix GeomBase::GetMatrix()
{
	return aMatrix;
}
*/

/*
dVector GeomBase::GetVelocity()
{
	return aVelocity;
}

dVector GeomBase::GetOmega()
{
	return aOmega;
}

dVector GeomBase::GetForce()
{
	return aForce;
}
*/

void* GeomBase::GetDataPointer()
{
	return aDataPointer;
}

int GeomBase::GetClassID()
{
	return aClassID;
}

/*
void GeomBase::SetMatrix(dMatrix val)
{
	aMatrix = val;
}
*/

/*
void GeomBase::SetVelocity(dVector val)
{
	aVelocity = val;
}

void GeomBase::SetOmega(dVector val)
{
	aOmega = val;
}

void GeomBase::SetForce(dVector val)
{
	aForce = val;
}
*/

void GeomBase::SetDataPointer(void* val)
{
	aDataPointer = val;
}

void GeomBase::SetClassID(int val)
{
	aClassID = val;
}

GeomBase::~GeomBase()
{

}

/////////////////////////////////////////////////

GeomNewton::GeomNewton(NewtonManager* aMan)
	: aUserData(new GeomBase()),
	aVtx(NULL),
	aVao(0),
	aVbo(0),
	aIbo(0),
	aIndiceCount(0),
	aBufferInited(false),
	aMeshType(atNone),
	aBodyType(adtNone),
	aManager(aMan),
	aSize(glm::vec3(1.0f, 1.0f, 1.0f)),
	aMappingMat(dGetIdentityMatrix()),
	aTrans(dGetIdentityMatrix()),
	aMass(0.0f),
	aBody(NULL),
	aTexture0(0),
	aTexture1(0),
	aTexture2(0),
	aDiffuseColor(dVector(0.65f, 0.65f, 0.65f, 1.0f)),
	aTexTileU(1.0f),
    aTexTileV(1.0f),
	aTurnAngle(0.0f),
    aRollAngle(0.0f),
    aPitchAngle(0.0f), 
	aParentGeom(NULL)
{ 
	aUserData->SetClassID(666);
	aUserData->SetDataPointer(this);
	aManager->vGeomList.push_back(this);
}


void GeomNewton::SetParent(GeomNewton* uParent)
{
	aParentGeom = uParent;
}

void GeomNewton::SetMatrix(dMatrix val)
{
	if (!aParentGeom) {
		aTrans = val;
	}
	else {
		aTrans = val * aParentGeom->GetMatrix();
	}
}

dMatrix GeomNewton::GetMatrix()
{
	return aTrans;
}

void GeomNewton::SetTexTileU(float val)
{
	aTexTileU = val;
}

void GeomNewton::SetTexTileV(float val)
{
	aTexTileV = val;
}

void GeomNewton::SetTexture0(const char* filetex, const char* texname)
{
	aTexture0 = aManager->vTextureManager->TextureFromFile(filetex, texname);
}

void GeomNewton::SetTexture1(const char* filetex, const char* texname)
{
	aTexture1 = aManager->vTextureManager->TextureFromFile(filetex, texname);
}

void GeomNewton::SetTexture2(const char* filetex, const char* texname)
{
	aTexture2 = aManager->vTextureManager->TextureFromFile(filetex, texname);
}

void GeomNewton::SetDiffuseColor(float cr, float cg, float cb)
{
	aDiffuseColor = dVector(cr, cg, cb, 1.0f);
}

void GeomNewton::SetTransformMatrix(float* pMatrix)
{
	aTrans = dMatrix(pMatrix);
}

void GeomNewton::SetPosition(float px, float py, float pz)
{
	if (!aParentGeom) {
		aTrans.m_posit = dVector(px, py, pz, 1.0f);
	}
	else {
		aTrans = aTrans * aParentGeom->GetMatrix();
		aTrans.m_posit = aParentGeom->GetMatrix().m_posit + dVector(px, py, pz, 1.0f);
	}
}

void GeomNewton::SetBodyType(GeomBodyType ctype)
{
	aBodyType = ctype;
}

void GeomNewton::SetTurnAngle(float rAngle, bool rnoLocal)
{
	aTurnAngle = rAngle * dDegreeToRad;
	//
	if (!rnoLocal) {
		aTrans = (dYawMatrix(aTurnAngle) * aTrans);
	}
	else {
		aTrans = (aTrans * dYawMatrix(aTurnAngle));
	}
	//
}

void GeomNewton::SetRollAngle(float rAngle, bool rnoLocal)
{
	aRollAngle = rAngle * dDegreeToRad;
	//
	if (!rnoLocal) {
		aTrans = (dRollMatrix(aRollAngle) * aTrans);
	}
	else {
		aTrans = (aTrans * dRollMatrix(aRollAngle));
	}
	//
}

void GeomNewton::SetPitchAngle(float rAngle, bool rnoLocal)
{
	aPitchAngle = rAngle * dDegreeToRad;
	//
	if (!rnoLocal) {
		aTrans = (dPitchMatrix(aPitchAngle) * aTrans);
	}
	else {
		aTrans = (aTrans * dPitchMatrix(aPitchAngle));
	}
	//
}

float GeomNewton::GetTurnAngle()
{
	return aTurnAngle * dRadToDegree;
}

float GeomNewton::GetRollAngle()
{
	return aRollAngle * dRadToDegree;
}

float GeomNewton::GetPitchAngle()
{
	return aPitchAngle * dRadToDegree;
}

NewtonBody* GeomNewton::GetBody()
{
	return aBody;
}

void GeomNewton::GenerateMesh()
{
	int aVerticeCount = 0;
	NewtonCollision* mColl = NULL;
	//
	if (aBodyType == adtNone)
		aBodyType = adtDynamic;
	//
	switch (aMeshType)
	{
    // box
	case atBox : 
		mColl = NewtonCreateBox(aManager->GetWorld(), aSize.x, aSize.y, aSize.z, SERIALIZE_ID_BOX, NULL);
	break;
	// sphere
	case atSphere:
		mColl = NewtonCreateSphere(aManager->GetWorld(), aSize.x, SERIALIZE_ID_SPHERE, NULL);
	break;
	// cone
	case atCone:
		mColl = NewtonCreateCone(aManager->GetWorld(), aSize.x, aSize.y, SERIALIZE_ID_CONE, NULL);
	break;
	// cylinder
	case atCylinder:
		mColl = NewtonCreateCylinder(aManager->GetWorld(), aSize.x, aSize.y, aSize.z, SERIALIZE_ID_CYLINDER, NULL);
	break;
	// capsule
	case atCapsule:
		mColl = NewtonCreateCapsule(aManager->GetWorld(), aSize.x, aSize.y, aSize.z, SERIALIZE_ID_CAPSULE, NULL);
	break;
	// cylinderchamfer
	case atCylinderChamfer:
		mColl = NewtonCreateChamferCylinder(aManager->GetWorld(), aSize.x, aSize.y, SERIALIZE_ID_CHAMFERCYLINDER, NULL);
	break;
	// Need to add some more type like compound, collisiontree, and convex shape...
	default:
		break;
	}
	if (mColl) {	  
	  NewtonMesh* nMesh = NewtonMeshCreateFromCollision(mColl);
	  NewtonMeshCalculateVertexNormals(nMesh, 60.0f * dDegreeToRad);
	  //
	  // The newton mesh shape can have 3 textures.
	  // In this tutorial I use only one texture.
	  // In the mapping info I set 0 as default texture id for all.
	  // If you like to support 3 textures for shape like box.
	  // Or 2 textures for shape like cylinder, 
	  // You need to modify the indices loading part in the code.
	  switch (aMeshType)
	  {
	  case atBox:
		  NewtonMeshApplyBoxMapping(nMesh, 0, 0, 0, &aMappingMat[0][0]);
		  break;
	  case atSphere:
		  NewtonMeshApplySphericalMapping(nMesh, 0, &aMappingMat[0][0]);
		  break;
	  case atCone:
	  case atCylinder:
	  case atCapsule:
	  case atCylinderChamfer:
		  NewtonMeshApplyCylindricalMapping(nMesh, 0, 0, &aMappingMat[0][0]);
		  break;
	  default:
		  break;
	  }
	  
	  //
      aVerticeCount = NewtonMeshGetPointCount(nMesh);
	  //
	  aVtx = new MainVertexPTN[aVerticeCount];
	  //
	  MainVertice* const vert = new MainVertice[aVerticeCount];
	  MainNormal* const norm = new MainNormal[aVerticeCount];
	  MainTexUv* const uvs = new MainTexUv[aVerticeCount];
	  //
	  NewtonMeshGetVertexChannel(nMesh, sizeof(MainVertice), &vert[0].vx);
	  NewtonMeshGetNormalChannel(nMesh, sizeof(MainNormal), &norm[0].nx);
	  NewtonMeshGetUV0Channel(nMesh, sizeof(MainTexUv), &uvs[0].uvx);
	  //
	  for (int i = 0; i < aVerticeCount; i++) {
		  aVtx[i].posit.x = vert[i].vx;
		  aVtx[i].posit.y = vert[i].vy;
		  aVtx[i].posit.z = vert[i].vz;
		  //
		  aVtx[i].normal.x = norm[i].nx;
		  aVtx[i].normal.y = norm[i].ny;
		  aVtx[i].normal.z = norm[i].nz;
		  //
		  aVtx[i].uv.x = uvs[i].uvx * aTexTileU;
		  aVtx[i].uv.y = uvs[i].uvy * aTexTileV;
	  }
	  //
	  void* const geometryHandle = NewtonMeshBeginHandle(nMesh);
	  for (int handle = NewtonMeshFirstMaterial(nMesh, geometryHandle); handle != -1; handle = NewtonMeshNextMaterial(nMesh, geometryHandle, handle)) {
		  int material = NewtonMeshMaterialGetMaterial(nMesh, geometryHandle, handle);
		  int indexCount = NewtonMeshMaterialGetIndexCount(nMesh, geometryHandle, handle);
		  //
		  // It you like to support 2 or 3 textures mode you need to make array of indice count.
		  // And you need to make a array of aIndices buffer.
		  // If newton detecting more texture id set in the mapping info, It load more indices buffer count here.
		  aIndiceCount = indexCount;
		  aIndices = new unsigned int[indexCount];
		  //
		  NewtonMeshMaterialGetIndexStream(nMesh, geometryHandle, handle, (int*)aIndices);
	  }
	  NewtonMeshEndHandle(nMesh, geometryHandle);
	  //
	  NewtonMeshDestroy(nMesh);
	  delete []vert;
	  delete []norm;
	  delete []uvs;
	  //
	  //if (aMass > 0.0f) {
	  if (aBodyType == adtDynamic)
	    aBody = NewtonCreateDynamicBody(aManager->GetWorld(), mColl, &aTrans[0][0]);
	  if (aBodyType == adtKinematic)
	    aBody = NewtonCreateKinematicBody(aManager->GetWorld(), mColl, &aTrans[0][0]);
	  //
	  if (aBody) {
		NewtonBodySetUserData(aBody, aUserData);
		//
		if (aBodyType == adtDynamic) {
			NewtonBodySetMassProperties(aBody, aMass, mColl);
			// WIP: Maybe need some variables for set this values from the class directly...
			NewtonBodySetFreezeState(aBody, 1);
			NewtonBodySetSleepState(aBody, 1);
			NewtonBodySetAutoSleep(aBody, 1);
			//
			if (aMass > 0.0f) {
				NewtonBodySetForceAndTorqueCallback(aBody, NewtonManager::PhysicsApplyGravityForce);
			}
		}
		NewtonBodySetTransformCallback(aBody, NewtonManager::TransformCallback);
		//
	  }
	  //}
	  //
	  NewtonDestroyCollision(mColl);
	  mColl = NULL;
	}
	//
	if (!aBufferInited) {
		glGenVertexArrays(1, &aVao);
		if (aVao) {
			glBindVertexArray(aVao);
			glGenBuffers(1, &aVbo);
			if (aVbo) {
				glBindBuffer(GL_ARRAY_BUFFER, aVbo);
				//
				glBufferData(GL_ARRAY_BUFFER, sizeof(MainVertexPTN) * aVerticeCount, aVtx, GL_STATIC_DRAW);
				//
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MainVertexPTN), (void*)0);
				//
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MainVertexPTN), (void*)(offsetof(MainVertexPTN, MainVertexPTN::uv)));
				//
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(MainVertexPTN), (void*)(offsetof(MainVertexPTN, MainVertexPTN::normal)));
				//
			}
			//
			// If you use 2 textures or 3 textures in the newton mapping remember to loop here other indice buffers.
			// Something like this:
			//glGenBuffers(3, &aIbos);
			// or
			//glGenBuffers(1, &aIbo[i]);
			//if (aIbo[i]) {
			//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, aIbo[i]);
			//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * aIndiceCount[i], aIndices[i], GL_STATIC_DRAW);
			//}
			// The render don't need any modification if you add more indices buffer.
			// You only need to render the VAO, It already containting all buffers.
			// 
			glGenBuffers(1, &aIbo);
			if (aIbo) {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, aIbo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * aIndiceCount, aIndices, GL_STATIC_DRAW);
			}
			//
			glDisableVertexAttribArray(2);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(0);
	        //
			glBindVertexArray(0);
			
			aBufferInited = true;
		}
    }
	//
}

void GeomNewton::InitNewton(GeomType cGeomtype, float cSx, float cSy, float cSz, float cMass)
{
	aMeshType = cGeomtype;
	aSize = glm::vec3(cSx, cSy, cSz);
	aMass = cMass;
	//
	GenerateMesh();
}

void GeomNewton::Render(Shader* cshd, dFloat steptime)
{
	if (!aBufferInited) return;
	//
	if (aVao) {//
		cshd->setNMat4("mat_model", aTrans);
		cshd->setNVec4("diffusecol", aDiffuseColor);
		//
		if (aTexture0) {
			GLuint TextureID = glGetUniformLocation(cshd->ID, "aSamplerTex");
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, aTexture0);
		}

		//
		glBindVertexArray(aVao);
		//
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		//
		glDrawElements(GL_TRIANGLES, aIndiceCount, GL_UNSIGNED_INT, 0);
		//
		//glDisableVertexAttribArray(2);
		//glDisableVertexAttribArray(1);
		//glDisableVertexAttribArray(0);
		//
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	//*/
}

GeomNewton::~GeomNewton()
{
	//
	if ((singletonContextGL) && (aUserData)) {
		if (aUserData->IsPicked)
			singletonContextGL->GetMousePicking()->UnPickBody();
	}
	if (singletonContextGL) {
		NewtonDestroyBody(aBody);
		aBody = NULL;
	}
	//
	if (aUserData)
		delete aUserData;
	//
	delete []aIndices;
	delete []aVtx;
	//
	//PS: slow method, It can request a better implementation if you delete multiple object in runtime...
	///*
	if (!aManager->IsTerminated)
	for (auto itr = aManager->vGeomList.begin();
	  itr != aManager->vGeomList.end(); itr++) {
	  if ((GeomNewton*)*itr == this) {
		  aManager->vGeomList.erase(itr);
		break;
	  }
	}
	//*/
}

////////////////////////////////////////////////////////////

GeomAssimp::GeomAssimp(NewtonManager* aMan)
	:aMeshCount(0),
	 aScene(NULL),
	 aManager(aMan),
	 aUserData(new GeomBase()),
	 aTrans(dGetIdentityMatrix()),
	 aDiffuseColor(dVector(1.0f, 1.0f, 1.0f, 1.0f)),
	 aBody(NULL),
	 aMass(0.0f),
	 aBodyType(adtNone),
	 aScale(glm::vec3(1.0f, 1.0f, 1.0f)),
	 aTurnAngle(0.0f),
	 aRollAngle(0.0f),
	 aPitchAngle(0.0f)
{
	aUserData->SetClassID(777);
	aUserData->SetDataPointer(this);
	aManager->vAssMeshList.push_back(this);
}

void GeomAssimp::SetMeshScale(float sx, float sy, float sz)
{
	aScale = glm::vec3(sx, sy, sz);
}

void GeomAssimp::ImportMeshFile(const char* mfile)
{
	int flags = (aiProcess_OptimizeMeshes|
		         aiProcess_OptimizeGraph |
		         aiProcess_Triangulate |
		         aiProcess_GenNormals |
		         //aiProcess_GenSmoothNormals |
		         aiProcess_LimitBoneWeights |
		         aiProcess_ValidateDataStructure |
		         aiProcess_GenUVCoords |
		         aiProcess_TransformUVCoords |
		         aiProcess_RemoveComponent |
		         aiProcess_CalcTangentSpace |
		         aiProcess_RemoveRedundantMaterials |
		         aiProcess_SortByPType
		); 
	aScene = (aiScene*)aiImportFile(mfile, flags);
	if (aScene) {
		aMeshCount = aScene->mNumMeshes;
		if ((aMeshCount > 0) || (aScene->HasAnimations())) {
			aAssimpMeshes = new GeomAssimpMesh[aMeshCount];
			//
			for (int i = 0; i < aMeshCount; i++) {
				FillBufferGL(aScene->mMeshes[i], aAssimpMeshes[i]);
			}
		}
		//
		aiReleaseImport(aScene);
		aScene = NULL;
	}
}

void GeomAssimp::InitNewton(GeomMeshType cGeomMeshtype, float cHulltolerance, float cMass)
{
	if (aBodyType == adtNone)
		aBodyType = adtDynamic;
	aMass = cMass;

	int aTotalVerticeCount = 0;
	MainVertice* artri;
	int vid = 0;
	//
	NewtonCollision* aMeshColl = NULL;
	//
	switch (cGeomMeshtype)
	{
	case amtNone:
		break;
	case amtConvex:
		for (int i = 0; i < aMeshCount; i++) {
			aTotalVerticeCount += aAssimpMeshes[i].aVerticeCount;
		}
		//
		artri = new MainVertice[aTotalVerticeCount];
		//
		for (int i = 0; i < aMeshCount; i++) {
			for (int u = 0; u < aAssimpMeshes[i].aVerticeCount; u++) {
				artri[vid].vx = aAssimpMeshes[i].aVtx[u].posit.x;
				artri[vid].vy = aAssimpMeshes[i].aVtx[u].posit.y;
				artri[vid].vz = aAssimpMeshes[i].aVtx[u].posit.z;
				//
				vid++;
			}
		}
		aMeshColl = NewtonCreateConvexHull(aManager->GetWorld(), aTotalVerticeCount, &artri[0].vx, sizeof(MainVertice), cHulltolerance, SERIALIZE_ID_CONVEXHULL, NULL);
		//
		delete [] artri;
		break;
	case amtCollTree:
		aMeshColl = NewtonCreateTreeCollision(aManager->GetWorld(), SERIALIZE_ID_TREE);
		NewtonTreeCollisionBeginBuild(aMeshColl);
		//
		for (int u = 0; u < aMeshCount; u++) {
			for (int i = 0; i < aAssimpMeshes[u].aFaceCount; i++) {
				MainVertice face[3];
				int index = i * 3;
				//
				face[0].vx = aAssimpMeshes[u].aVtx[index + 0].posit.x;
				face[0].vy = aAssimpMeshes[u].aVtx[index + 0].posit.y;
				face[0].vz = aAssimpMeshes[u].aVtx[index + 0].posit.z;
				//
				face[1].vx = aAssimpMeshes[u].aVtx[index + 1].posit.x;
				face[1].vy = aAssimpMeshes[u].aVtx[index + 1].posit.y;
				face[1].vz = aAssimpMeshes[u].aVtx[index + 1].posit.z;
				//
				face[2].vx = aAssimpMeshes[u].aVtx[index + 2].posit.x;
				face[2].vy = aAssimpMeshes[u].aVtx[index + 2].posit.y;
				face[2].vz = aAssimpMeshes[u].aVtx[index + 2].posit.z;
				//
				// You can set some special face id here for detect in runtime collision.
				// This feature is not implemented for this tutorial..
				int matID = 0;
				NewtonTreeCollisionAddFace(aMeshColl, 3, &face[0].vx, sizeof(MainVertice), matID);
			}
		}
		NewtonTreeCollisionEndBuild(aMeshColl, 1);
		break;
	default:
		break;
	}
	//
	if (aBodyType == adtDynamic)
		aBody = NewtonCreateDynamicBody(aManager->GetWorld(), aMeshColl, &aTrans[0][0]);
	if (aBodyType == adtKinematic)
		aBody = NewtonCreateKinematicBody(aManager->GetWorld(), aMeshColl, &aTrans[0][0]);
	//
	if (aBody) {
		NewtonBodySetUserData(aBody, aUserData);
		//
		if ((aBodyType == adtDynamic) && (cGeomMeshtype != amtCollTree)) {
			NewtonBodySetMassProperties(aBody, aMass, aMeshColl);
			// WIP: Maybe need some variables for set this values from the class directly...
			NewtonBodySetFreezeState(aBody, 1);
			NewtonBodySetSleepState(aBody, 1);
			NewtonBodySetAutoSleep(aBody, 1);
			//
			if (aMass > 0.0f) {
				NewtonBodySetForceAndTorqueCallback(aBody, NewtonManager::PhysicsApplyGravityForce);
			}
		}
		NewtonBodySetTransformCallback(aBody, NewtonManager::TransformCallback);
		//
	}
	if (aMeshColl) {
		NewtonDestroyCollision(aMeshColl);
		aMeshColl = NULL;
	}
}

void GeomAssimp::SetMatrix(dMatrix val)
{
	aTrans = val;
}

void GeomAssimp::SetPosition(float px, float py, float pz)
{
	aTrans.m_posit = dVector(px, py, pz, 1.0f);
}

void GeomAssimp::SetTransformMatrix(float* pMatrix)
{
	aTrans = dMatrix(pMatrix);
}

void GeomAssimp::SetBodyType(GeomBodyType ctype)
{
	aBodyType = ctype;
}

void GeomAssimp::SetTextureMeshID(int mID, const char* texfile, const char* texname)
{
	if ((mID >= 0) || (mID <= aMeshCount)) {
		aAssimpMeshes[mID].aTexture0 = aManager->vTextureManager->TextureFromFile(texfile, texname);
#if defined(_DEBUG) && defined(_MSC_VER)
		printf("texture name: %s \n", (char*)texname);
#endif
	}
}

void GeomAssimp::FillBufferGL(aiMesh* aImportMesh, GeomAssimpMesh& gMesh)
{
	gMesh.aName = (char*)aImportMesh->mName.C_Str();
	gMesh.aVerticeCount = aImportMesh->mNumVertices;
	gMesh.aFaceCount = aImportMesh->mNumFaces;
	gMesh.aIndiceCount = (aImportMesh->mNumFaces * 3);
	//
#if defined(_DEBUG) && defined(_MSC_VER)
	printf("mesh name: %s vertices: %d faces: %d indices: %d \n", gMesh.aName, aImportMesh->mNumVertices, gMesh.aFaceCount, gMesh.aIndiceCount);
#endif
	//
	gMesh.aVtx = new MainVertexPTN[gMesh.aVerticeCount];
	gMesh.aIndices = new unsigned int[gMesh.aIndiceCount];
	//
	for (int i = 0; i < gMesh.aVerticeCount; i++) {
		// This aScale method don't work for animation.
		// You need to apply the scale on the matrix with the animation or for better result you do the scale in shader.
		// It can work only for mesh dynamic or static with this demo.
		// Later in the other tutorial update I can add animation with a better scale method.
		gMesh.aVtx[i].posit.x = aImportMesh->mVertices[i].x * aScale.x;
		gMesh.aVtx[i].posit.y = aImportMesh->mVertices[i].y * aScale.y;
		gMesh.aVtx[i].posit.z = aImportMesh->mVertices[i].z * aScale.z;
		//
		gMesh.aVtx[i].normal.x = aImportMesh->mNormals[i].x;
		gMesh.aVtx[i].normal.y = aImportMesh->mNormals[i].y;
		gMesh.aVtx[i].normal.z = aImportMesh->mNormals[i].z;
		//
		if (aImportMesh->mTextureCoords[0]) {
			gMesh.aVtx[i].uv.x = aImportMesh->mTextureCoords[0][i].x;
			gMesh.aVtx[i].uv.y = (1.0f - aImportMesh->mTextureCoords[0][i].y);
		}
		else {
			gMesh.aVtx[i].uv.x = 0.0f;
			gMesh.aVtx[i].uv.y = 0.0f;
		}
	}
	//
	for (int f = 0; f < int(aImportMesh->mNumFaces); f++)
	{
		aiFace *face = aImportMesh->mFaces + f;
		//
		gMesh.aIndices[f * 3 + 0] = face->mIndices[0];
		gMesh.aIndices[f * 3 + 1] = face->mIndices[1];
		gMesh.aIndices[f * 3 + 2] = face->mIndices[2];
	}
	//
	glGenVertexArrays(1, &gMesh.aVao);
	if (gMesh.aVao) {
		glBindVertexArray(gMesh.aVao);
		glGenBuffers(1, &gMesh.aVbo);
		if (gMesh.aVbo) {
			glBindBuffer(GL_ARRAY_BUFFER, gMesh.aVbo);
			//
			glBufferData(GL_ARRAY_BUFFER, sizeof(MainVertexPTN) * gMesh.aVerticeCount, gMesh.aVtx, GL_STATIC_DRAW);
			//
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MainVertexPTN), (void*)0);
			//
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MainVertexPTN), (void*)(offsetof(MainVertexPTN, MainVertexPTN::uv)));
			//
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(MainVertexPTN), (void*)(offsetof(MainVertexPTN, MainVertexPTN::normal)));
			//
		}
		glGenBuffers(1, &gMesh.aIbo);
		if (gMesh.aIbo) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gMesh.aIbo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * gMesh.aIndiceCount, gMesh.aIndices, GL_STATIC_DRAW);
		}
		//
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		//
		glBindVertexArray(0);
	}
}

void GeomAssimp::SetTurnAngle(float rAngle, bool rnoLocal)
{
	aTurnAngle = rAngle * dDegreeToRad;
	//
	if (!rnoLocal) {
		aTrans = (dYawMatrix(aTurnAngle) * aTrans);
	}
	else {
		aTrans = (aTrans * dYawMatrix(aTurnAngle));
	}
	//
}

void GeomAssimp::SetRollAngle(float rAngle, bool rnoLocal)
{
	aRollAngle = rAngle * dDegreeToRad;
	//
	if (!rnoLocal) {
		aTrans = (dRollMatrix(aRollAngle) * aTrans);
	}
	else {
		aTrans = (aTrans * dRollMatrix(aRollAngle));
	}
	//
}

void GeomAssimp::SetPitchAngle(float rAngle, bool rnoLocal)
{
	aPitchAngle = rAngle * dDegreeToRad;
	//
	if (!rnoLocal) {
		aTrans = (dPitchMatrix(aPitchAngle) * aTrans);
	}
	else {
		aTrans = (aTrans * dPitchMatrix(aPitchAngle));
	}
	//
}

float GeomAssimp::GetTurnAngle()
{
	return aTurnAngle * dRadToDegree;
}

float GeomAssimp::GetRollAngle()
{
	return aRollAngle * dRadToDegree;
}

float GeomAssimp::GetPitchAngle()
{
	return aPitchAngle * dRadToDegree;
}

void GeomAssimp::RenderMeshes(Shader* cshd, dFloat steptime)
{
	for (int i = 0; i < aMeshCount; i++) {
		if (aAssimpMeshes[i].aVao) {//
			cshd->setNMat4("mat_model", aTrans);
			cshd->setNVec4("diffusecol", aDiffuseColor);
			//
			if (aAssimpMeshes[i].aTexture0) {
				GLuint TextureID = glGetUniformLocation(cshd->ID, "aSamplerTex");
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, aAssimpMeshes[i].aTexture0);
			}

			//
			glBindVertexArray(aAssimpMeshes[i].aVao);
			//
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			//
			glDrawElements(GL_TRIANGLES, aAssimpMeshes[i].aIndiceCount, GL_UNSIGNED_INT, 0);
			//
			//glDisableVertexAttribArray(2);
			//glDisableVertexAttribArray(1);
			//glDisableVertexAttribArray(0);
			//
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

dMatrix GeomAssimp::GetMatrix()
{
	return aTrans;
}

NewtonBody* GeomAssimp::GetBody()
{
	return aBody;
}

GeomAssimp::~GeomAssimp()
{
	// Currently not working, I have only start to implement it.
	// This is need when you delete a object and it is picked by the mouse.
	// I try to finish this part of code on next tutorial update.
	if ((singletonContextGL) && (aBody) && (aUserData)) {
		if (aUserData->IsPicked)
			singletonContextGL->GetMousePicking()->UnPickBody();
		NewtonDestroyBody(aBody);
		aBody = NULL;
	}
	for (int i = 0; i < aMeshCount; i++) {
		delete[] aAssimpMeshes[i].aVtx;
		delete[] aAssimpMeshes[i].aIndices;
	}
	//PS: slow method, It can request a better implementation if you delete multiple object in runtime...
	///*
	if (!aManager->IsTerminated)
	for (auto itr = aManager->vAssMeshList.begin();
		itr != aManager->vAssMeshList.end(); itr++) {
		if ((GeomAssimp*)*itr == this) {
			aManager->vAssMeshList.erase(itr);
			break;
		}
	}
	delete[] aAssimpMeshes;
	//*/
	if (aUserData)
		delete aUserData;
}