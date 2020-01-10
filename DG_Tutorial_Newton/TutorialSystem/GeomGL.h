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
#ifndef GEOMGL_H
#define GEOMGL_H
#pragma once
#include "pch.h"
#include "shader.h"
#include "VertexGL.h"
#include "texture.h"
#include "NewtonManager.h"

enum GeomMeshType
{
	amtNone = -1,
	amtConvex = 0,
	amtCollTree = 1
};

enum GeomBodyType
{
	adtNone = -1,
	adtDynamic = 0,
	adtKinematic = 1
};

enum GeomType
{
	atNone            = -1,
	atBox             = 0,
	atSphere          = 1,
	atCone            = 2,
	atCylinder        = 3,
	atCapsule         = 4,
	atCylinderChamfer = 5
};

// This is only a data pointer object.
// This is not the geom object class.
// This object is use as a pointer for the newton objects userdata.
// It is a helper for retrive the object userdata from newton.
// With this method you can use multiple and different class object as userdata.
// You can use the geomtype for know what is the class in the userdata pointer in coming from newton. 
//
struct GeomBase
{   // public
	GeomBase();
	virtual ~GeomBase();
	//
	//dMatrix GetMatrix();
	//dVector GetVelocity();
	//dVector GetOmega();
	//dVector GetForce();
	void* GetDataPointer();
	int GetClassID();
	//
	//void SetMatrix(dMatrix val);
	//void SetVelocity(dVector val);
	//void SetOmega(dVector val);
	//void SetForce(dVector val);
	void SetDataPointer(void* val);
	void SetClassID(int val);
	//
	bool IsPicked;
  private:
	//dMatrix aMatrix;
	//dVector aVelocity;
	//dVector aOmega;
	//dVector aForce;
	void* aDataPointer;
	int aClassID;
};

struct GeomNewton
{
	// public
	GeomNewton(NewtonManager* aMan);
	virtual ~GeomNewton();
	//
	dMatrix GetMatrix();
	void SetMatrix(dMatrix val);
	void SetDiffuseColor(float cr, float cg, float cb);
	void SetPosition(float px, float py, float pz);
	void SetTransformMatrix(float* pMatrix);
	void SetTexture0(const char* filetex, const char* texname);
	void SetTexture1(const char* filetex, const char* texname);
	void SetTexture2(const char* filetex, const char* texname);
	void SetTexTileU(float val);
	void SetTexTileV(float val);
	void SetTurnAngle(float rAngle, bool rnoLocal = false);
	void SetRollAngle(float rAngle, bool rnoLocal = false);
	void SetPitchAngle(float rAngle, bool rnoLocal = false);
	float GetTurnAngle();
	float GetRollAngle();
	float GetPitchAngle();
	void Render(Shader* cshd, dFloat steptime);
	void SetBodyType(GeomBodyType ctype);
	void InitNewton(GeomType cGeomtype, float cSx, float cSy, float cSz, float cMass = 0.0f);
	NewtonBody* GetBody();
	void SetParent(GeomNewton* uParent);

  private:
	  GeomNewton* aParentGeom;
	  GeomBase* aUserData;
	  MainVertexPTN* aVtx;
	  unsigned int* aIndices;
	  int aIndiceCount;
	  float aMass;
	  float aTexTileU;
	  float aTexTileV;
	  float aTurnAngle;
	  float aRollAngle;
	  float aPitchAngle;
	  NewtonBody* aBody;
	  GLuint aVao;
	  GLuint aVbo;
	  GLuint aIbo;
	  GLuint aTexture0;
	  GLuint aTexture1;
	  GLuint aTexture2;
	  NewtonManager* aManager;
	  glm::vec3 aSize;
	  dMatrix aTrans;
	  dMatrix aMappingMat;
	  dVector aDiffuseColor;
	  GeomType aMeshType;
	  GeomBodyType aBodyType;
	  bool aBufferInited;
	  //
	  void GenerateMesh();
};

// WIP: Very basic assimp mesh loader.
// I use a very basic method for import and load the mesh in this tutorial.
// Later i'm going to add a more complex method with animation and material and texture loading.
// Currently you need to load the texture manually for any meshes by using the mesh id.
//
struct GeomAssimpMesh
{
	GLuint aVao;
	GLuint aVbo;
	GLuint aIbo;
	GLuint aTexture0;
	int aVerticeCount;
	int aIndiceCount;
	int aFaceCount;
	char* aName;
	MainVertexPTN* aVtx;
	unsigned int* aIndices;
};

// I have create a new class for show how to use different userdate with newton.
// This is only a WIP class, later It can become inherited from the original geomnewton class.
// For now it is only a quick and simple class for show some feature with the userdata use.
struct GeomAssimp
{
	GeomAssimp(NewtonManager* aMan);
	virtual ~GeomAssimp();
	//
	void SetMatrix(dMatrix val);
	dMatrix GetMatrix();
	void ImportMeshFile(const char* mfile);
	void FillBufferGL(aiMesh* aImportMesh, GeomAssimpMesh& gMesh);
	void RenderMeshes(Shader* cshd, dFloat steptime);
	void SetPosition(float px, float py, float pz);
	void SetTransformMatrix(float* pMatrix);
	void SetTextureMeshID(int mID, const char* texfile, const char* texname);
	void SetBodyType(GeomBodyType ctype);
	void SetMeshScale(float sx, float sy, float sz);
	void InitNewton(GeomMeshType cGeomMeshtype, float cHulltolerance = 0.01f, float cMass = 0.0f);
	void SetTurnAngle(float rAngle, bool rnoLocal = false);
	void SetRollAngle(float rAngle, bool rnoLocal = false);
	void SetPitchAngle(float rAngle, bool rnoLocal = false);
	float GetTurnAngle();
	float GetRollAngle();
	float GetPitchAngle();
	NewtonBody* GetBody();
private:
	GeomBase* aUserData;
	GeomAssimpMesh* aAssimpMeshes;
	int aMeshCount;
	NewtonManager* aManager;
	NewtonBody* aBody;
    glm::vec3 aScale;
	GeomBodyType aBodyType;
	//
	float aTurnAngle;
	float aRollAngle;
	float aPitchAngle;
	float aMass;
	dMatrix aTrans;
	dVector aDiffuseColor;
	aiScene* aScene;
};

#endif //GEOMGL_H
