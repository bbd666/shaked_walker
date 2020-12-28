#include "pch.h"
#include "OXVehicleRayCast.h"

ofstream monFlux("history.txt");

dFloat newTime = 0;

dRaycastVHModel::~dRaycastVHModel()
{

}

dRaycastVHModel::dRaycastVHModel(WindowMain* winctx, const char* const modelName, const dMatrix& location, int linkMaterilID)
	: dModelRootNode(NULL, dGetIdentityMatrix())
	, m_winManager(winctx)
	, Low_Leg_L(NULL)
	, Plantar_L(NULL)
	, Plantar_LNode(NULL)

{
	string tex("Textures//wood6.png");
	l_Hip = 0.25f;
	l_Sacrum = 1.0f;
	l_Up_Leg = 1.0f;
	l_Low_Leg = 1.0f;
	r_bones = 0.07f;
	l_foot = 0.4f;
	w_foot = 0.2f;
	l_toe = 0.2f;
	l_Spine = 0.7f;
	l_Clav = 0.45f;
	l_Up_Arm = 0.8f;
	l_Low_Arm = 0.8f;
	l_Hand = 0.3f;
	l_Neck = 0.3f;
	l_Head = 0.4f;

	glm::vec3 _Pos(glm::vec3(3.0f, 3.0f, 0.f));
	Scale = 1.0f;


	Low_Leg_L = new GeomNewton(m_winManager->aManager);
	Low_Leg_L->SetBodyType(adtDynamic);
	Low_Leg_L->SetTexture0(&tex[0], "Tex0");
	Low_Leg_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Low_Leg_L->SetRollAngle(90.0f, false);
	Low_Leg_L->SetPosition(_Pos.x, _Pos.y, _Pos.z);
	Low_Leg_L->InitNewton(atCapsule, Scale*r_bones, Scale*r_bones, Scale* l_Low_Leg, 10.f);
	m_body = Low_Leg_L->GetBody();
	NewtonBodySetTransformCallback(m_body, NULL);

	NewtonBody* const parentBody = GetBody();
	dMatrix aParenMatrix(dGetIdentityMatrix());
	NewtonBodyGetMatrix(parentBody, &aParenMatrix[0][0]);
	new dCustomSixdof(aParenMatrix, m_body, NULL);


	Plantar_L = new GeomNewton(m_winManager->aManager);
	Plantar_L->SetBodyType(adtDynamic);
	Plantar_L->SetParent(Low_Leg_L);
	Plantar_L->SetTexture0(&tex[0], "Tex0");
	Plantar_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Plantar_L->SetRollAngle(90.0f, false);
	Plantar_L->SetPosition(0.5f, -0.5f, 0);
	Plantar_L->InitNewton(atCapsule, Scale*r_bones, Scale*r_bones, Scale*l_Low_Leg, 10.f);
	NewtonBodySetTransformCallback(Plantar_L->GetBody(), NULL);
	Plantar_LNode = new dModelNode(Plantar_L->GetBody(), dGetIdentityMatrix(), this);




	dMatrix Ankle_LPinMatrix(dGetIdentityMatrix()); 
	Ankle_LPinMatrix = Ankle_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Ankle_LPinMatrix.m_posit = dVector(Low_Leg_L->GetPosition().m_x, Low_Leg_L->GetPosition().m_y - 0.5f, Low_Leg_L->GetPosition().m_z);
	Ankle_L = new dCustomHingeActuator(Ankle_LPinMatrix, Plantar_L->GetBody(), Low_Leg_L->GetBody());
	Ankle_L->SetTargetAngle(-50.0f * dDegreeToRad);
	Ankle_L->EnableMotor(true, 500.0f * dDegreeToRad);
	Ankle_L->SetLimits(-3600.f, 3600.f * dDegreeToRad);
//	Ankle_L->SetAsSpringDamper(true, 200.f, 0.5f);
	m_winManager->aManager->vJointList.push_back(Ankle_L);


	ins11 = dVector(0.3f, 0.f, 0.f);
	ins12 = dVector(-0.3f, 0.f, 0.f);
	m1 = new Muscle(m_winManager->aLineManager, m_winManager->aManager, GetLow_Leg_L(), GetPlantar_L(), ins11, ins12);
	m1->GenerateMesh();

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

float dRaycastVHModel::GetFoot2Floor_R() {
	return Foot2Floor_R;
}

dCustomHingeActuator* dRaycastVHModel::GetAnkle_L() {
	return Ankle_L;
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

void DGVehicleRCManager::OnPreUpdate(dModelRootNode* const model, dFloat timestep, int threadID) const
//pour VS<VS2017 supprimer ,int threadID
{
	dRaycastVHModel* controller = (dRaycastVHModel*)model;
	//printf("DGVehicleRCManager OnPreUpdate \n");


	dVector pos(0.0f);
	dVector Vtemp(0.0f);
	dVector V;

	// scan all  Muscle Elements
	for (auto itr = m_winManager->aManager->vMuscleList.begin();
		itr != m_winManager->aManager->vMuscleList.end(); itr++)
	{
		Muscle* Mobj = (Muscle*)*itr;

		dVector posinsert(0.0f);

		// Get the Body1 connected to the muscle and apply the muscle force
		GeomNewton* gNewton = (GeomNewton*)(Mobj->body1);
		NewtonBody* NBody = gNewton->GetBody();
		Vtemp = Mobj->GetForceElas(timestep);
		V = Mobj->GetInsert1_GlobalRef();
		AddForceAtPos(NBody, &Vtemp.m_x, &V.m_x);


		// Get the Body2 connected and apply the opposite muscle force
		gNewton = (GeomNewton*)(Mobj->body2);
		NBody = (NewtonBody*)gNewton->GetBody();
		Vtemp = Vtemp.Scale(-1.0f);
		V = Mobj->GetInsert2_GlobalRef();
		AddForceAtPos(NBody, &Vtemp.m_x, &V.m_x);

	monFlux << newTime << "  " << Mobj->fSE(Mobj->GetDelta_l()) << "  " <<  Mobj->fCE(Mobj->GetDelta_l(), timestep) << "  " << Mobj->fPE(Mobj->GetDelta_l()) << "  " << Mobj->GetNmax() << "  " << Mobj->GetLCE() << "  " << Mobj->GetDelta_l() << std::endl;
	
	Mobj->SetLCE(Mobj->GetLCE() + Mobj->GetDelta_l());

	}	

	newTime += timestep;
	if (newTime>0.5f){
	controller->GetAnkle_L()->SetTargetAngle((-50.0f + 10.0f * sin((newTime-0.5f) *2.0f*dPi)) * dDegreeToRad);
	}
}

void DGVehicleRCManager::OnPostUpdate(dModelRootNode* const model, dFloat timestep, int threadID) const
//pour VS<VS2017 supprimer ,int threadID

{

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