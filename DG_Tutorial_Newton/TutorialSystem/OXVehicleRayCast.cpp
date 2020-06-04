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
	if (0) // V1
	{
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
		NewtonBodySetMassMatrix(Thigh_L->GetBody(), masses[5], Ixx[5], Iyy[5], Izz[5]); //set mass matrix  
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
		m_winManager->aManager->vJointList.push_back(Knee_L);

		// CE of muscle
		CE_KL = new GeomNewton(m_winManager->aManager);
		CE_KL->SetBodyType(adtDynamic);
		CE_KL->SetParent(Thigh_L);
		CE_KL->SetTexture0(&tex[0], "Tex0");
		CE_KL->SetDiffuseColor(0.7f, 0.7f, 0.7f);
		CE_KL->SetPosition( 0, r_leg/2, 0.0f);
		CE_KL->InitNewton(atBox, r_leg, r_leg, r_leg, 0.05f);
		NewtonBodySetTransformCallback(CE_KL->GetBody(), NULL);
		CE_KLNode = new dModelNode(CE_KL->GetBody(), dGetIdentityMatrix(), this);

		// Slider
		dMatrix matrix;
		// connect the bodies by a Slider joint
		NewtonBodyGetMatrix(Thigh_L->GetBody(), &matrix[0][0]);
		dCustomSlider* const slider = new dCustomSlider(matrix, CE_KL->GetBody(), Thigh_L->GetBody());

		link = new GeomNewton(m_winManager->aManager);
		link->SetBodyType(adtDynamic);
		link->SetParent(Shank_L);
		link->SetTexture0(&tex[0], "Tex0");
		//Shank_L->SetTurnAngle(30.0f);
		link->SetDiffuseColor(0.7f, 0.7f, 0.7f);
		link->SetPosition(-l_Shank / 2, r_leg, 0.0f);
		link->InitNewton(atCapsule, r_leg/4, r_leg/4, l_Thigh/2 + l_Shank/2, 0.05f);
		NewtonBodySetTransformCallback(link->GetBody(), NULL);
		linkNode = new dModelNode(link->GetBody(), dGetIdentityMatrix(), Shank_LNode);

		//LEFT link JOINT //
		dMatrix link_LPinMatrix(dGetIdentityMatrix());
		link_LPinMatrix = Knee_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
		link_LPinMatrix.m_posit = dVector(_Pos.x + l_Thigh / 2 + l_Shank / 2, _Pos.y + 3*r_leg / 2, _Pos.z);
		dCustomBallAndSocket* link_L = new dCustomBallAndSocket(link_LPinMatrix, Shank_L->GetBody(), link->GetBody());

		//right link JOINT //
		link_LPinMatrix.m_posit = dVector(_Pos.x, _Pos.y + 3*r_leg / 2, _Pos.z);
		dCustomBallAndSocket* link_R = new dCustomBallAndSocket(link_LPinMatrix, CE_KL->GetBody(), link->GetBody());

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		 //MUSCLES //
		 //VAS Left Vasti

		insVAS_L11 = dVector(0.0f, r_leg / 2, 0.0f); // inserire delta x, delta y e delta z da COM del body 1
		insVAS_L12 = dVector(0.0f, r_leg / 2, 0.0f);// inserire delta x, delta y e delta z da COM del body 2
		dMatrix matrix2;
		NewtonBodyGetMatrix(CE_KL->GetBody(), &matrix2[0][0]);
		dVector insVAS_L23 = {0.0f,0.0f,0.0f,0.0f};
		VAS_L2 = new MuscleV2(m_winManager->aLineManager, m_winManager->aManager, GetThigh_L(), GetShank_L(), GetCE_KL(), insVAS_L11, insVAS_L12, insVAS_L23);
		//VAS_L2->GenerateMesh();

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
		NewtonBodySetMassMatrix(Thigh_L->GetBody(), masses[5], Ixx[5], Iyy[5], Izz[5]); //set mass matrix  
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

		m_winManager->aManager->vJointList.push_back(Knee_L);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// MUSCLES //
		// VAS Left Vasti

		insVAS_L11 = dVector(0.0f, r_leg / 2, 0.0f); // inserire delta x, delta y e delta z da COM del body 1
		insVAS_L12 = dVector(0.0f, r_leg / 2, 0.0f);// inserire delta x, delta y e delta z da COM del body 2
		dVector insVAS_L23 = dVector(-l_Shank / 2, 0.05f, 0.0f);// inserire delta x, delta y e delta z da COM del body 2
		dMatrix VAS_LMatrix(dGetIdentityMatrix());
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
	// scan all  Muscle Elements
	for (auto itr = m_winManager->aManager->vMuscleList.begin();
		itr != m_winManager->aManager->vMuscleList.end(); itr++)
	{
		if (0)
		{
			MuscleV2* Mobj = (MuscleV2*)*itr;

			dVector posinsert(0.0f);
			// Get the Body1 connected to the muscle and apply the muscle force
			GeomNewton* gNewton = (GeomNewton*)(Mobj->body1);
			NewtonBody* NBody = gNewton->GetBody();
			Vtemp = Mobj->GetForceMTU_V1(); // Type Hill's force no time integration
			NewtonBodyAddForce(NBody, &Vtemp.m_x); // apply force to COM of body

			// Get the Body2 connected and apply the opposite muscle force
			gNewton = (GeomNewton*)(Mobj->body2);
			NBody = (NewtonBody*)gNewton->GetBody();
			Vtemp.m_x = Vtemp.m_x * (-1);
			NewtonBodyAddForce(NBody, &Vtemp.m_x); // apply force to COM of body
			//NewtonBodyAddForce(NBody, &Vtemp.m_y); // apply force to COM of body
			//NewtonBodyAddForce(NBody, &Vtemp.m_z); // apply force to COM of body
			cout << "Force " << Vtemp.m_x << ' ';
		}

		else {
			Muscle* Mobj = (Muscle*)*itr;

			dVector posinsert(0.0f);
			// Get the Body1 connected to the muscle and apply the muscle force
			GeomNewton* gNewton = (GeomNewton*)(Mobj->body1);
			NewtonBody* NBody = gNewton->GetBody();
			//Vtemp = Mobj->GetForceElas(); // Simple elastic force
			dFloat time = m_winManager->aManager->GetSimulationTime() * pow(10, -6); // simulation time in s
			Vtemp = Mobj->GetForceMTU_V2(timestep, m_player); // type Hill's force 
			NewtonBodyAddForce(NBody, &Vtemp.m_x); // apply force to COM of body
			// Get the Body2 connected and apply the opposite muscle force
			gNewton = (GeomNewton*)(Mobj->body2);
			NBody = (NewtonBody*)gNewton->GetBody();
			Vtemp.m_x = Vtemp.m_x * (-1);
			NewtonBodyAddForce(NBody, &Vtemp.m_x); // apply force to COM of body
			//NewtonBodyAddForce(NBody, &Vtemp.m_y); // apply force to COM of body
			//NewtonBodyAddForce(NBody, &Vtemp.m_z); // apply force to COM of body
			//// Torque calculation
			dCustomHinge* temp;
			temp = (m_player->GetKnee_L());
			thetamk = M_PI - 2.88f;
			dVector arm = { 0.1,0.1,0.056f,0 };
			dVector torque_k(0.0f, 0.0f, 0.0f);
			float ang = temp->GetJointAngle();
			//torque_k.m_z = arm[2] * cos(ang - thetamk) * Vtemp.m_z; // computed acccording to geyer and herr
			torque_k.m_z = arm[2] * Vtemp.m_y;
			//cout << '\t' << torque_k.m_z << '\t' << time << endl;
			// Apply torque to bodies
			GeomNewton* temp2 = controller->GetShank_L();
			torque_k = torque_k.Scale(1.0f);
			NewtonBodyAddTorque(temp2->GetBody(), &torque_k.m_x); //  apply torque to COM of Shank
		}
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

