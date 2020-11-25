#include "pch.h"
#include "OXVehicleRayCast.h"

dRaycastVHModel::~dRaycastVHModel()
{

}

dRaycastVHModel::dRaycastVHModel(WindowMain* winctx, const char* const modelName, const dMatrix& location, int linkMaterilID)
	: dModelRootNode(NULL, dGetIdentityMatrix())
	, m_winManager(winctx)
	, Thigh_L(NULL)
	, Shank_L(NULL)
	, Shank_LNode(NULL)
	, Knee_L(NULL)
	, CE_KL(NULL)
	, link(NULL)


{
	string tex("Textures//wood6.png");
	string tex2("Textures//checker.png");
	glm::vec3 _Pos(glm::vec3(0.0f, 0.25f, 0.0f));
	// De leva male

	l_Thigh = 0.4222f;//[m]
	l_Shank = 0.434f;//[m]
	r_leg = 0.03f;//[m] AGGIUSTARE
	l_foot = 0.1f;//[m] AGGIUSTARE
	w_foot = 0.05f; //[m] AGGIUSTARE
	h_foot = 0.05f; //[m] AGGIUSTARE
	l_toe = 0.2f; //[m] AGGIUSTARE
	l_hip = 0.3f; //[m] AGGIUSTARE
	l_trunk = 0.5319f; //[m]
	l_shoulders = 0.45f; //[m] AGGIUSTARE
	l_head = 0.3143f; //[m]
	l_arm = 0.2817f; //[m]
	l_farm = 0.2689f; //[m]
	l_hand = 0.0862; //[m]
	std::vector<float> l = { l_head, l_trunk, l_arm, l_farm, l_hand, l_Thigh, l_Shank, l_foot, l_toe };
	tot_w = 73.0f; // total weight of the subject [kg]
	masses = { 0.0694f * tot_w, 0.4346f * tot_w, 0.0271f * tot_w, 0.0162f * tot_w, 0.00061f * tot_w, 0.1416f * tot_w, 0.0433f * tot_w, 0.0137f * tot_w, 0.0137f * tot_w };
	//       {0 head,			1 trunk,		2 arm,			3 forearm,		4 hand,				5 thigh,		6 shank,			7 plantar AGGIUSTA,		8 toes AGGIUSTA} table 4 de leva technical notes
	// Inertia values for each body
	Ixx = { masses[0] * 0.362f, masses[1] * 0.372f, masses[2] * 0.285f, masses[3] * 0.276f, masses[4] * 0.628f, masses[5] * 0.329f, masses[6] * 0.255f }; //radii of gyration Sagittal (X) %
	Iyy = { masses[0] * 0.376f, masses[1] * 0.347f, masses[2] * 0.269f, masses[3] * 0.265f, masses[4] * 0.513f, masses[5] * 0.329f, masses[6] * 0.249f }; //radii of gyration Transverse (Y) %
	Izz = { masses[0] * 0.312f, masses[1] * 0.191f, masses[2] * 0.158f, masses[3] * 0.121f, masses[4] * 0.401f, masses[5] * 0.149f, masses[6] * 0.103f }; //radii of gyration Longitudinal (Z) %
	// COM distance from the origin of each body (DIRECTION X: IN LOCAL COORDINATE OF THE BODY)
	std::vector<float> percX = { 1 - 0.5976f, 1 - 0.4486f, 1 - 0.5772f, 1 - 0.4574f, 1 - 0.7900f, 1 - 0.4095f, 1 - 0.4459f, 1 - 0.5f, 1 - 0.5f }; //table 4 de leva technical notes
	vector<float> DeltaCMX(percX.size(), 0);
	for (int xx = 0; xx < DeltaCMX.size(); xx++)
	{
		DeltaCMX[xx] = (percX[xx] - 0.5f) * l[xx];
	}
	dVector com;
	// Delta Hill Muscle insertion points
	//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	if (0) // select 1 if you want to use muscle V2 (simple muscle model), or 0 to use version 1 muscle model (type Hill's model)
	{
		// LEFT LEG //
		Thigh_L = new GeomNewton(m_winManager->aManager);
		Thigh_L->SetBodyType(adtDynamic);
		Thigh_L->SetTexture0(&tex[0], "Tex0");
		Thigh_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
		Thigh_L->SetPosition(_Pos.x, _Pos.y + r_leg / 2, _Pos.z);
		//Thigh_L->InitNewton(atCapsule, r_leg, r_leg, l_Thigh, masses[5]);
		Thigh_L->InitNewton(atBox, l_Thigh, r_leg, r_leg, masses[5]);
		m_body = Thigh_L->GetBody();
		NewtonBodySetTransformCallback(m_body, NULL);
		NewtonBody* const parentBody = GetBody();
		dMatrix aParenMatrix(dGetIdentityMatrix());
		NewtonBodyGetMatrix(parentBody, &aParenMatrix[0][0]);
		NewtonBodySetMassMatrix(Thigh_L->GetBody(), 200, Ixx[5], Iyy[5], Izz[5]); //set mass matrix  
		//NewtonBodyGetCentreOfMass(m_body, &com[0]);
		//com.m_x = com.m_x + DeltaCMX[5];
		//NewtonBodySetCentreOfMass(Thigh_L->GetBody(), &com[0]); // WIP

		Shank_L = new GeomNewton(m_winManager->aManager);
		Shank_L->SetBodyType(adtDynamic);
		Shank_L->SetParent(Thigh_L);
		Shank_L->SetTexture0(&tex[0], "Tex0");
		//Shank_L->SetTurnAngle(30.0f);
		Shank_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
		Shank_L->SetPosition(l_Thigh / 2 + l_Shank / 2, 0.0f, 0.0f);
		//Shank_L->InitNewton(atCapsule, r_leg, r_leg, l_Shank, masses[6]);
		Shank_L->InitNewton(atBox, l_Shank, r_leg, r_leg,  masses[6]);
		NewtonBodySetMassMatrix(Shank_L->GetBody(), masses[6], Ixx[6], Iyy[6], Izz[6]); //set mass matrix 
		NewtonBodySetTransformCallback(Shank_L->GetBody(), NULL);
		Shank_LNode = new dModelNode(Shank_L->GetBody(), dGetIdentityMatrix(), this);
		//NewtonBodyGetCentreOfMass(Shank_L->GetBody(), &com[0]);
		//com.m_x = com.m_x + DeltaCMX[6];
		//NewtonBodySetCentreOfMass(Shank_L->GetBody(), &com[0]); // WIP

		 //LEFT KNEE JOINT //
		dMatrix Knee_LPinMatrix(dGetIdentityMatrix());
		Knee_LPinMatrix = Knee_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
		Knee_LPinMatrix.m_posit = dVector(_Pos.x + l_Thigh / 2, _Pos.y + r_leg / 2, _Pos.z);
		Knee_L = new dCustomHinge(Knee_LPinMatrix, Shank_L->GetBody(), Thigh_L->GetBody());
		//Knee_L->EnableLimits(1);
		//Knee_L->SetLimits(0.0, 180.0);
		m_winManager->aManager->vJointList.push_back(Knee_L);

		//AddMuscleV2_Element(Thigh_L, Shank_L); // WORK IN PROGRESS
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//MUSCLES //
		//VAS Left Vasti
		//VAS_L2 = new MuscleV2(m_winManager->aManager, GetShank_L(), GetThigh_L());

	}
	else {
		// LEFT LEG //
		Thigh_L = new GeomNewton(m_winManager->aManager);
		Thigh_L->SetBodyType(adtDynamic);
		Thigh_L->SetTexture0(&tex[0], "Tex0");
		Thigh_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
		Thigh_L->SetPosition(_Pos.x, _Pos.y + r_leg / 2, _Pos.z);
		Thigh_L->InitNewton(atCapsule, r_leg, r_leg, l_Thigh, masses[5]);
		m_body = Thigh_L->GetBody();
		NewtonBodySetTransformCallback(m_body, NULL);
		NewtonBody* const parentBody = GetBody();
		dMatrix aParenMatrix(dGetIdentityMatrix());
		NewtonBodyGetMatrix(parentBody, &aParenMatrix[0][0]);
		NewtonBodySetMassMatrix(Thigh_L->GetBody(), 200, Ixx[5], Iyy[5], Izz[5]); //set mass matrix  
		//NewtonBodyGetCentreOfMass(m_body, &com[0]);
		//com.m_x = com.m_x + DeltaCMX[5];
		//NewtonBodySetCentreOfMass(Thigh_L->GetBody(), &com[0]); // WIP

		Shank_L = new GeomNewton(m_winManager->aManager);
		Shank_L->SetBodyType(adtDynamic);
		Shank_L->SetParent(Thigh_L);
		Shank_L->SetTexture0(&tex[0], "Tex0");
		//Shank_L->SetTurnAngle(30.0f);
		Shank_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
		Shank_L->SetPosition(l_Thigh / 2 + l_Shank / 2, 0.0f, 0.0f);
		Shank_L->InitNewton(atCapsule, r_leg, r_leg, l_Shank, masses[6]);
		NewtonBodySetMassMatrix(Shank_L->GetBody(), masses[6], Ixx[6], Iyy[6], Izz[6]); //set mass matrix 
		NewtonBodySetTransformCallback(Shank_L->GetBody(), NULL);
		Shank_LNode = new dModelNode(Shank_L->GetBody(), dGetIdentityMatrix(), this);
		//NewtonBodyGetCentreOfMass(Shank_L->GetBody(), &com[0]);
		//com.m_x = com.m_x + DeltaCMX[6];
		//NewtonBodySetCentreOfMass(Shank_L->GetBody(), &com[0]); // WIP

		 //LEFT KNEE JOINT //
		dMatrix Knee_LPinMatrix(dGetIdentityMatrix());
		Knee_LPinMatrix = Knee_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
		Knee_LPinMatrix.m_posit = dVector(_Pos.x + l_Thigh / 2, _Pos.y + r_leg / 2, _Pos.z);
		Knee_L = new dCustomHinge(Knee_LPinMatrix, Shank_L->GetBody(), Thigh_L->GetBody());
		Knee_L->EnableLimits(1);
		//Knee_L->SetLimits(0.0, 180.0);
		m_winManager->aManager->vJointList.push_back(Knee_L);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// MUSCLES //
		// VAS Left Vasti

		insVAS_L11 = dVector(0.0f, r_leg / 2, 0.0f); // inserire delta x, delta y e delta z da COM del body 1
		insVAS_L12 = dVector(0.0f, r_leg / 2, 0.0f);// inserire delta x, delta y e delta z da COM del body 2
		dVector insVAS_L23 = dVector(-l_Shank / 2, 0.05f, 0.0f);// inserire delta x, delta y e delta z da COM del body 2
		VAS_L = new Muscle(m_winManager->aLineManager, m_winManager->aManager, GetThigh_L(), GetShank_L(), insVAS_L11, insVAS_L12, insVAS_L23);
		VAS_L->GenerateMesh();
	}
}

dCustomHinge* dRaycastVHModel::GetKnee_L() {
	return Knee_L;
}

GeomNewton* dRaycastVHModel::GetThigh_L() {
	return Thigh_L;
}

GeomNewton* dRaycastVHModel::GetShank_L() {
	return Shank_L;
}

GeomNewton* dRaycastVHModel::GetCE_KL() {
	return CE_KL;
}

// WORK IN PROGRESS
//GeomNewton* dRaycastVHModel::AddMuscleV2_Element(GeomNewton* body1, GeomNewton*body2) {
//	string tex("Textures//wood6.png");
//	// CE of muscle
//	GeomNewton* CE = new GeomNewton(m_winManager->aManager);
//	CE->SetBodyType(adtDynamic);
//	CE->SetParent(body1);
//	CE->SetTexture0(&tex[0], "Tex0");
//	CE->SetDiffuseColor(0.7f, 0.7f, 0.7f);
//	CE->SetPosition(l_Thigh / 2, 2 * r_leg, 0.0f);
//	CE->InitNewton(atBox, r_leg, r_leg / 2, r_leg, 0.05f);
//	NewtonBodySetTransformCallback(CE_KL->GetBody(), NULL);
//	dModelNode* CE_Node = new dModelNode(CE_KL->GetBody(), dGetIdentityMatrix(), this);
//
//	////right CE JOINT //
//	// connect CE to Thigh with 4 dof link: spherical + slider
//	dMatrix matrix0;
//	//dVector size = {1.0f, 1.0f, 1.0f};
//	dMatrix pinMatrix(dGrammSchmidt(dVector(0.0f, 1.0f, 0.0f, 0.0f)));// Only around y. Why ?
//
//	// Limits
//	//const dFloat yawLimit = 180.0f * dDegreeToRad;
//	const dFloat rollLimit = 180.0f * dDegreeToRad;
//	//const dFloat pitchLimit = 180.0f * dDegreeToRad;
//	dVector linearlimits = { 0.0f, 0.1f, 0.0f };
//
//	// Create 6 dof link
//	NewtonBodyGetMatrix(CE_KL->GetBody(), &matrix0[0][0]);
//	pinMatrix.m_posit = matrix0.m_posit + dVector(-l_Thigh / 2, .0f, .0f, 0.0f);
//	dCustomSixdof* const joint0 = new dCustomSixdof(pinMatrix, CE_KL->GetBody(), Thigh_L->GetBody());
//
//	// Set limits
//	joint0->SetLinearLimits(linearlimits.Scale(-1), linearlimits);
//
//	//joint0->SetYawLimits(-yawLimit, yawLimit);
//	//joint0->SetPitchLimits(-pitchLimit, pitchLimit);
//	joint0->SetRollLimits(-rollLimit, rollLimit);
//
//	// Push to joint list
//	m_winManager->aManager->vJointList.push_back(joint0);
//
//	////Left CE JOINT //
//	// connect CE to Thigh with 4 dof link: spherical + slider
//	dMatrix matrix2;
//	dMatrix pinMatrix2(dGrammSchmidt(dVector(0.0f, 1.0f, 0.0f, 0.0f)));// ???
//
//
//	// Create 6 dof link
//	NewtonBodyGetMatrix(CE_KL->GetBody(), &matrix2[0][0]);
//	pinMatrix2.m_posit = matrix2.m_posit + dVector(l_Shank / 2, .0f, .0f, 0.0f);
//	dCustomSixdof* const joint1 = new dCustomSixdof(pinMatrix2, CE_KL->GetBody(), Shank_L->GetBody());
//
//	// Set limits
//	joint1->SetLinearLimits(linearlimits.Scale(-1), linearlimits);
//
//	//joint0->SetYawLimits(-yawLimit, yawLimit);
//	//joint0->SetPitchLimits(-pitchLimit, pitchLimit);
//	joint1->SetRollLimits(-rollLimit, rollLimit);
//
//	// Push to joint list
//	m_winManager->aManager->vJointList.push_back(joint1);
//
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DGVehicleRCManager::DGVehicleRCManager(WindowMain* winctx)
	: dModelManager(winctx->aManager->GetWorld()),
	m_player(NULL)
	, m_winManager(winctx)
	, temp(NULL)
{}
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

	float thetamk;
	// scan all  Muscle V1 Elements
	for (auto itr = m_winManager->aManager->vMuscleList.begin();
		itr != m_winManager->aManager->vMuscleList.end(); itr++)
	{
		Muscle* Mobj = (Muscle*)*itr;

		// Get the Body2 connected to the muscle and apply the muscle force
		GeomNewton* gNewton = (GeomNewton*)(Mobj->body2);
		NewtonBody* NBody = gNewton->GetBody();
		//Vtemp = Mobj->GetForceElas(); // Simple elastic force
		double F = Mobj->GetForceMTU_V1(timestep, m_player); // type Hill's force 
		//NewtonBodyAddForce(NBody, &Vtemp.m_x); // apply force to COM of body

		//// Torque calculation // shold be generalized for each muscle. now it is ok only for this example
		dCustomHinge* temp;
		temp = (m_player->GetKnee_L());
		thetamk = M_PI - 2.88f; //caso vas e knee
		dVector arm = { 0.1,0.1,0.056f,0 }; // same as in muscle vas for l mtu calculation
		float ang = temp->GetJointAngle();
		float tau = (arm[2] * F * cos(ang - thetamk)); // Check
		float F_p = tau / 0.2;

		//Adding perpendicular force to shank COM
		dVector pinAxisVector(0.0f), newDir(0.0f);
		GeomNewton* gShank = m_player->GetShank_L();

		dMatrix shankMatrix = gShank->GetMatrix();
		NewtonBody* NShank = gShank->GetBody();

		dVector shankVect = (shankMatrix.m_posit - temp->GetMatrix0().m_posit); // temp is dCustomHinge
		shankVect = shankVect.Normalize();

		pinAxisVector = temp->GetPinAxis();
		newDir = shankVect.CrossProduct(pinAxisVector).Scale(F_p);

		NewtonBodyAddForce(NShank, &newDir[0]); // apply perpendicular force to shank

		dFloat angknee = temp->GetJointAngle();
		/*if (angknee <= 0.0 * dDegreeToRad)
			cout << '10!!' << endl;
		if (angknee <= -10.0*dDegreeToRad)
			cout << '10!!' << endl;
		if (angknee <= -20.0 * dDegreeToRad)
			cout << '20!!' << endl;
		if (angknee <= -30.0 * dDegreeToRad)
			cout << '30!!' << endl;
		if (angknee <= -40.0 * dDegreeToRad)
			cout << '40!!' << endl;
		if (angknee <= -50.0 * dDegreeToRad)
			cout << '50!!' << endl;
		if (angknee <= -60.0 * dDegreeToRad)
			cout << '60!!' << endl;
		if (angknee <= -70.0 * dDegreeToRad)
			cout << '70!!' << endl;
		if (angknee <= -80.0 * dDegreeToRad)
			cout << '80!!' << endl;*/
	/*	if (angknee <= -90.0 * dDegreeToRad)
			cout << '90!!' << endl;*/
	}
	// Scan all Muscle V2 elements
	for (auto itr = m_winManager->aManager->vMuscleV2List.begin();
		itr != m_winManager->aManager->vMuscleV2List.end(); itr++)
	{
		MuscleV2* Mobj = (MuscleV2*)*itr;
		// Get the Body1 connected to the muscle and apply the muscle force
		GeomNewton* gNewton = (GeomNewton*)(Mobj->body1);
		NewtonBody* NBody = gNewton->GetBody();
		//float F = Mobj->GetForceMTU_V2(timestep); // Simple muscle

		//Vtemp = { -F, 0.0f, 0.0f };
		//NewtonBodyAddForce(NBody, &Vtemp.m_x); // apply force to COM of body to generate torque
	}
}

void DGVehicleRCManager::OnPostUpdate(dModelRootNode* const model, dFloat timestep) const
{
	//printf("DGVehicleRCManager OnPostUpdate \n");
	dRaycastVHModel* controller = (dRaycastVHModel*)model;
	//controller->CastFoot_L();


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

