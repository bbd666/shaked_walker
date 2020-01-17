#include "pch.h"
#include "OXVehicleRayCast.h"

dRaycastVHModel::~dRaycastVHModel()
{

}

dRaycastVHModel::dRaycastVHModel(WindowMain* winctx, const char* const modelName, const dMatrix& location, int linkMaterilID)
	: dModelRootNode(NULL, dGetIdentityMatrix())
	, m_winManager(winctx)
	, Up_Leg_L(NULL)
	, Low_Leg_L(NULL)
	, Plantar_L(NULL)
	, Toe_L(NULL)
	, Knee_L(NULL)
	, Ankle_L(NULL)
	, Flextoe_L(NULL)
	, Low_Leg_LNode(NULL)
	, Plantar_LNode(NULL)
	, Toe_LNode(NULL)
{
	string tex("Textures//wood6.png");
	l_Up_Leg = 1.0f;
	l_Low_Leg = 1.0f;
	r_leg = 0.1f;
	l_foot = 0.4f;
	w_foot = 0.2f;
	l_toe = 0.2f;
	glm::vec3 _Pos(glm::vec3(3.0f, 3.0f, 0.f));
	Scale = 1.0f;

	Up_Leg_L = new GeomNewton(m_winManager->aManager);
	Up_Leg_L->SetBodyType(adtDynamic);
	Up_Leg_L->SetTexture0(&tex[0], "Tex0");
	Up_Leg_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Up_Leg_L->SetPosition(_Pos.x, _Pos.y, _Pos.z);
	Up_Leg_L->InitNewton(atCapsule, Scale*r_leg, Scale*r_leg, Scale*l_Up_Leg, 10.0f*Scale);
	m_body= Up_Leg_L->GetBody();
	NewtonBodySetTransformCallback(m_body, NULL);
	NewtonBody* const parentBody = GetBody();
	dMatrix aParenMatrix(dGetIdentityMatrix());
	NewtonBodyGetMatrix(parentBody, &aParenMatrix[0][0]);

	Low_Leg_L = new GeomNewton(m_winManager->aManager);
	Low_Leg_L->SetBodyType(adtDynamic);
	Low_Leg_L->SetParent(Up_Leg_L);
	Low_Leg_L->SetTexture0(&tex[0], "Tex0");
	Low_Leg_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Low_Leg_L->SetPosition(Scale * (l_Up_Leg + l_Low_Leg) / 2, 0, 0);
	Low_Leg_L->InitNewton(atCapsule, Scale* r_leg, Scale*  r_leg, Scale* l_Low_Leg, 10.0f);
	NewtonBodySetTransformCallback(Low_Leg_L->GetBody(), NULL);
	Low_Leg_LNode = new dModelNode(Low_Leg_L->GetBody(), dGetIdentityMatrix(), this);

	Plantar_L = new GeomNewton(m_winManager->aManager);
	Plantar_L->SetBodyType(adtDynamic);
	Plantar_L->SetParent(Low_Leg_L);
	Plantar_L->SetTexture0(&tex[0], "Tex0");
	Plantar_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Plantar_L->SetPosition(Scale * ( l_Low_Leg) / 2,  Scale * (l_foot / 4), 0);
	Plantar_L->InitNewton(atBox, Scale*  0.1f, Scale*  l_foot, Scale* w_foot, 10.0f * Scale);
	NewtonBodySetTransformCallback(Plantar_L->GetBody(), NULL);
	Plantar_LNode = new dModelNode(Plantar_L->GetBody(), dGetIdentityMatrix(), Low_Leg_LNode);

	Toe_L = new GeomNewton(m_winManager->aManager);
	Toe_L->SetBodyType(adtDynamic);
	Toe_L->SetParent(Plantar_L);
	Toe_L->SetTexture0(&tex[0], "Tex0");
	Toe_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Toe_L->SetPosition( 0,  Scale * ( l_foot / 2 + l_toe / 2), 0);
	Toe_L->InitNewton(atBox, Scale* 0.1f, Scale*  0.2f, Scale*  0.2f, 10.0f*Scale);
	NewtonBodySetTransformCallback(Toe_L->GetBody(), NULL);
	Toe_LNode = new dModelNode(Toe_L->GetBody(), dGetIdentityMatrix(), Plantar_LNode);

	// create knee joint.
	dMatrix Knee_LPinMatrix(dGetIdentityMatrix());
	Knee_LPinMatrix = Knee_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Knee_LPinMatrix.m_posit = dVector(_Pos.x + Scale * l_Up_Leg / 2, _Pos.y, _Pos.z);
	Knee_L = new dCustomHinge(Knee_LPinMatrix, Low_Leg_L->GetBody(), Up_Leg_L->GetBody());
	m_winManager->aManager->vJointList.push_back(Knee_L);

	// create ankle joint.
	dMatrix Ankle_LPinMatrix(dGetIdentityMatrix());
	Ankle_LPinMatrix = Ankle_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Ankle_LPinMatrix.m_posit = dVector(_Pos.x + Scale * (l_Up_Leg / 2 + l_Low_Leg), _Pos.y, _Pos.z);
	Ankle_L = new dCustomBallAndSocket(Ankle_LPinMatrix,  Plantar_L->GetBody(),Low_Leg_L->GetBody());
	m_winManager->aManager->vJointList.push_back(Ankle_L);

	// create toe joint.
	dMatrix Toe_LPinMatrix(dGetIdentityMatrix());
	Toe_LPinMatrix = Toe_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Toe_LPinMatrix.m_posit = dVector(_Pos.x + Scale * (l_Up_Leg / 2 + l_Low_Leg), _Pos.y + 3 * Scale*  l_foot / 4, _Pos.z);
	Flextoe_L = new dCustomHinge(Toe_LPinMatrix, Plantar_L->GetBody(), Toe_L->GetBody());
	m_winManager->aManager->vJointList.push_back(Flextoe_L);

	ins11 = dVector(0.f, 0.f, 0.f);
	ins12 = dVector(0.f, 0.f, 0.f);
	m1 = new Muscle(m_winManager->aLineManager, m_winManager->aManager, GetUp_Leg_L(), GetLow_Leg_L(), ins11, ins12);
	m1->GenerateMesh();
	CreateFootScanLine();
}

GeomNewton* dRaycastVHModel::GetUp_Leg_L() {
	return Up_Leg_L;
}

GeomNewton* dRaycastVHModel::GetLow_Leg_L() {
	return Low_Leg_L;
}

GeomNewton* dRaycastVHModel::GetPlantar_L() {
	return Plantar_L;
}

float dRaycastVHModel::GetFoot2Floor_L() {
	return Foot2Floor_L;
}

void dRaycastVHModel::CreateFootScanLine() {
glm::vec3 linepos1;
glm::vec3 linepos2;
glm::vec3 linecolor;

linepos1.x = 0; linepos1.y = 0; linepos1.z = 0;
linepos2.x = 2.0f; linepos2.y = 2.0f; linepos2.z = 2.0f;
linecolor.x = 0.0f; linecolor.y = 1.0f; linecolor.z = 0.f;

FootLineIndex_L = m_winManager->aLineManager->AddLine(linepos1, linepos2, linecolor, false);
}

void dRaycastVHModel::CastFoot_L() {
	NewtonCollision* const collisionA = NewtonBodyGetCollision(Plantar_L->GetBody());
    dMatrix matrixA;
	NewtonBodyGetMatrix(Plantar_L->GetBody(), &matrixA[0][0]);

	NewtonCollision* const collisionB = NewtonCreateBox(m_winManager->aManager->GetWorld(), 25.0f, 0.5f, 25.0f, SERIALIZE_ID_BOX, NULL);
	dMatrix matrixB(dGetIdentityMatrix());

	int res = NewtonCollisionClosestPoint(m_winManager->aManager->GetWorld(), collisionA, &matrixA[0][0], collisionB, &matrixB[0][0], &ContactFoot_L[0], &ContactGround_L[0], &NormalFoot_L[0], 0);
	NewtonDestroyCollision(collisionB);
	Foot2Floor_L= abs(ContactFoot_L[1]);

	m_winManager->aLineManager->aLineBuffer[FootLineIndex_L - 1].posit.x = ContactFoot_L[0];
	m_winManager->aLineManager->aLineBuffer[FootLineIndex_L - 1].posit.y = ContactFoot_L[1];
	m_winManager->aLineManager->aLineBuffer[FootLineIndex_L - 1].posit.z = ContactFoot_L[2];
	//aLineBuffer[LineIndex].color = newlinecolor;
	m_winManager->aLineManager->aLineBuffer[FootLineIndex_L - 2].posit.x = this->ContactGround_L[0];
	m_winManager->aLineManager->aLineBuffer[FootLineIndex_L - 2].posit.y = this->ContactGround_L[1];
	m_winManager->aLineManager->aLineBuffer[FootLineIndex_L - 2].posit.z = this->ContactGround_L[2];
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DGVehicleRCManager::DGVehicleRCManager(WindowMain* winctx)
: dModelManager(winctx->aManager->GetWorld()),
  m_player(NULL)
	, m_winManager(winctx)
{
	
}

dModelRootNode* DGVehicleRCManager::CreateWalkerPlayer(const char* const modelName, const dMatrix& location)
{
	dRaycastVHModel* const controller = new dRaycastVHModel(m_winManager, modelName, location, 0);

	// set the model to calculate the local transformation
	controller->SetTransformMode(true);

	// add the model to the manager
	AddRoot(controller);

	m_player = controller;
	return controller;
}

void DGVehicleRCManager::OnPreUpdate(dModelRootNode* const model, dFloat timestep) const
{
	dRaycastVHModel* controller = (dRaycastVHModel*)model;
	//printf("DGVehicleRCManager OnPreUpdate \n");


	dVector pos(0.0f);
	dVector Vtemp(0.0f);

	// scan all  Muscle Elements
for (auto itr = m_winManager->aManager->vMuscleList.begin();
	itr != m_winManager->aManager->vMuscleList.end(); itr++)
{
	Muscle* Mobj = (Muscle*)*itr;

	dVector posinsert(0.0f);
	
	// Get the Body1 connected to the muscle and apply the muscle force
	GeomNewton* gNewton = (GeomNewton*)(Mobj->body1);
	NewtonBody* NBody = gNewton->GetBody();
	Vtemp = Mobj->GetForceElas();
	NewtonBodyAddForce(NBody, &Vtemp.m_x);

	// Get the Body2 connected and apply the opposite muscle force
	gNewton = (GeomNewton*)(Mobj->body2);
	NBody = (NewtonBody*)gNewton->GetBody();
	Vtemp = Mobj->GetForceElas();
	Vtemp = Vtemp.Scale(-1.0f);
	NewtonBodyAddForce(NBody, &Vtemp.m_x);

}
}

void DGVehicleRCManager::OnPostUpdate(dModelRootNode* const model, dFloat timestep) const
{
	//printf("DGVehicleRCManager OnPostUpdate \n");
	dRaycastVHModel* controller = (dRaycastVHModel*)model;
	controller->CastFoot_L();
}

void DGVehicleRCManager::OnUpdateTransform(const dModelNode* const bone, const dMatrix& localMatrix) const
{
	NewtonBody* const body = bone->GetBody();
	GeomBase* aUserData = (GeomBase*)NewtonBodyGetUserData(body);
	//
	//
	if (aUserData->GetClassID() == 666) {
		GeomNewton* aTempGeom = (GeomNewton*)aUserData->GetDataPointer();
		// pour que ici ca fonction tu dois ajouter un simple parent dedans la class GeomNewton
		aTempGeom->SetMatrix(localMatrix);
	}
	//printf("DGVehicleRCManager OnUpdateTransform \n");
}

void DGVehicleRCManager::OnDebug(dModelRootNode* const model, dCustomJoint::dDebugDisplay* const debugContext)
{
	//printf("DGVehicleRCManager OnDebug \n");
}

DGVehicleRCManager::~DGVehicleRCManager()
{

}