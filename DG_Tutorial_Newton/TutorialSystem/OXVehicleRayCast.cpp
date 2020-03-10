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
	, PlantarC_L(NULL)
	, ToesC_L(NULL)
	, Knee_L(NULL)
	, Ankle_L(NULL)
	, Thigh_R(NULL)
	, Shank_R(NULL)
	, PlantarC_R(NULL)
	, ToesC_R(NULL)
	, Knee_R(NULL)
	, Ankle_R(NULL)
	, Toes_R(NULL)
	, Trunk(NULL)
	, Head(NULL)
	, Arm_R(NULL)
	, Shoulders_arm_R(NULL)
	, Forearm_R(NULL)
	, Farm_arm_R(NULL)
	, Hand_R(NULL)
	, Hand_RNode(NULL)
	, Arm_L(NULL)
	, Shoulders_arm_L(NULL)
	, Forearm_L(NULL)
	, Farm_arm_L(NULL)
	, Hand_L(NULL)
	, Hand_LNode(NULL)
	, ToesC_LNode(NULL)
	, PlantarC_LNode(NULL)
	, Shank_LNode(NULL)
	, ToesC_RNode(NULL)
	, PlantarC_RNode(NULL)
	, Shank_RNode(NULL)
	, Thigh_RNode(NULL)
	, Trunk_Node(NULL)
	, Head_Node(NULL)
	, Arm_RNode(NULL)
	, Forearm_RNode(NULL)
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
	std::vector<float> l = {l_head, l_trunk, l_arm, l_farm, l_hand, l_Thigh, l_Shank, l_foot, l_toe};
	tot_w = 73.0f; // total weight of the subject [kg]
	masses = { 0.0694f * tot_w, 0.4346f * tot_w, 0.0271f * tot_w, 0.0162f * tot_w, 0.00061f * tot_w, 0.1416f * tot_w, 0.0433f * tot_w, 0.0137f * tot_w, 0.0137f * tot_w};
	//       {0 head,			1 trunk,		2 arm,			3 forearm,		4 hand,				5 thigh,		6 shank,			7 plantar AGGIUSTA,		8 toes AGGIUSTA} table 4 de leva technical notes
	// Inertia values for each body
	Ixx = { masses[0] * 0.362f, masses[1] * 0.372f, masses[2] * 0.285f, masses[3] * 0.276f, masses[4] * 0.628f, masses[5] * 0.329f, masses[6] * 0.255f}; //radii of gyration Sagittal (X) %
	Iyy = { masses[0] * 0.376f, masses[1] * 0.347f, masses[2] * 0.269f, masses[3] * 0.265f, masses[4] * 0.513f, masses[5] * 0.329f, masses[6] * 0.249f}; //radii of gyration Transverse (Y) %
	Izz = { masses[0] * 0.312f, masses[1] * 0.191f, masses[2] * 0.158f, masses[3] * 0.121f, masses[4] * 0.401f, masses[5] * 0.149f, masses[6] * 0.103f}; //radii of gyration Longitudinal (Z) %
	// COM distance from the origin of each body (DIRECTION X: IN LOCAL COORDINATE OF THE BODY)
	std::vector<float> percX = {1 - 0.5976f, 1 - 0.4486f, 1 - 0.5772f, 1 - 0.4574f, 1 - 0.7900f, 1 - 0.4095f, 1 - 0.4459f, 1 - 0.5f, 1 - 0.5f}; //table 4 de leva technical notes
	vector<float> DeltaCMX(percX.size(), 0);
	for (int xx = 0; xx < DeltaCMX.size(); xx++)
	{
		DeltaCMX[xx] = (percX[xx] - 0.5f) * l[xx];
	}
	dVector com;

	// LEFT LEG //
	Thigh_L = new GeomNewton(m_winManager->aManager);
	Thigh_L->SetBodyType(adtDynamic);
	Thigh_L->SetTexture0(&tex[0], "Tex0");
	Thigh_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Thigh_L->SetRollAngle(90.0f);
	Thigh_L->SetPosition(_Pos.x - l_hip / 2, _Pos.y + l_Shank + h_foot + l_Thigh / 2, _Pos.z);
	Thigh_L->InitNewton(atCapsule, r_leg, r_leg, l_Thigh, masses[5]);
	m_body = Thigh_L->GetBody();
	NewtonBody* const parentBody = GetBody();
	dMatrix aParenMatrix(dGetIdentityMatrix());
	NewtonBodyGetMatrix(parentBody, &aParenMatrix[0][0]);
	NewtonBodySetMassMatrix(Thigh_L->GetBody(), masses[5], Ixx[5], Iyy[5], Izz[5]); //set mass matrix  
	NewtonBodyGetCentreOfMass(m_body, &com[0]);
	com.m_x = DeltaCMX[5];
	com.m_y = 0.0f;
	com.m_z = 0.0f;
	NewtonBodySetCentreOfMass(Thigh_L->GetBody(), &com[0]); // WIP

	Shank_L = new GeomNewton(m_winManager->aManager);
	Shank_L->SetBodyType(adtDynamic);
	Shank_L->SetParent(Thigh_L);
	Shank_L->SetTexture0(&tex[0], "Tex0");
	Shank_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Shank_L->SetPosition( 0, -(l_Thigh / 2 + l_Shank / 2), 0);
	Shank_L->InitNewton(atCapsule, r_leg, r_leg, l_Shank, masses[6]);
	NewtonBodySetMassMatrix(Shank_L->GetBody(), masses[6], Ixx[6], Iyy[6], Izz[6]); //set mass matrix 
	Shank_LNode = new dModelNode(Shank_L->GetBody(), dGetIdentityMatrix(), this);
	com.m_x = DeltaCMX[6];
	com.m_y = 0.0f;
	com.m_z = 0.0f;
	NewtonBodySetCentreOfMass(Shank_L->GetBody(), &com[0]); // WIP

	// LEFT FOOT //
	PlantarC_L = new GeomNewton(m_winManager->aManager);
	PlantarC_L->SetBodyType(adtDynamic);
	PlantarC_L->SetParent(Shank_L);
	PlantarC_L->SetTexture0(&tex[0], "Tex0");
	PlantarC_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	PlantarC_L->SetPosition(0, -(l_Shank / 2 + h_foot / 2), 0);
	PlantarC_L->InitNewton(atCompound, 2.0f, 0.0f, 0.0f, masses[7]);
	PlantarC_LNode = new dModelNode(PlantarC_L->GetBody(), dGetIdentityMatrix(), Shank_LNode);
	//com.m_x = DeltaCMX[7];
	//com.m_y = 0.0f;
	//com.m_z = 0.0f;
	//NewtonBodySetCentreOfMass(Plantar_L->GetBody(), &com[0]); // WIP

	ToesC_L = new GeomNewton(m_winManager->aManager);
	ToesC_L->SetBodyType(adtDynamic);
	ToesC_L->SetParent(PlantarC_L);
	ToesC_L->SetTexture0(&tex[0], "Tex0");
	ToesC_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	ToesC_L->SetPosition(0.0f, 0.0f, -l_foot);
	ToesC_L->InitNewton(atCompound, 1.0f, 0.0f, 0.0f, masses[1]);
	NewtonBodyGetCentreOfMass(ToesC_L->GetBody(), &com[0]);
	ToesC_LNode = new dModelNode(ToesC_L->GetBody(), dGetIdentityMatrix(), PlantarC_LNode);
	// COM????

	// create knee joint.
	dMatrix Knee_LPinMatrix(dGetIdentityMatrix());
	Knee_LPinMatrix = Knee_LPinMatrix * dYawMatrix(180.0f * dDegreeToRad);
	Knee_LPinMatrix.m_posit = dVector(_Pos.x - l_hip / 2, _Pos.y + l_Shank + h_foot, _Pos.z);
	dCustomHinge* Knee_L = new dCustomHinge(Knee_LPinMatrix, Shank_L->GetBody(), Thigh_L->GetBody());
	Knee_L->EnableLimits(true);
	Knee_L->SetLimits(0.0f * dDegreeToRad, 150.0f * dDegreeToRad); // limit rotation of knee between 0� and 150�
	m_winManager->aManager->vJointList.push_back(Knee_L);

	// create ankle joint.
	dMatrix Ankle_LPinMatrix(dGetIdentityMatrix());
	Ankle_LPinMatrix = Ankle_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Ankle_LPinMatrix.m_posit = dVector(_Pos.x - l_hip / 2, _Pos.y + h_foot, _Pos.z);
	dCustomBallAndSocket* Ankle_L = new dCustomBallAndSocket(Ankle_LPinMatrix, Shank_L->GetBody(), PlantarC_L->GetBody());
	m_winManager->aManager->vJointList.push_back(Ankle_L);
	// LEFT TOES JOINT //
	dMatrix Toes_LPinMatrix(dGetIdentityMatrix());
	Toes_LPinMatrix = Toes_LPinMatrix * dYawMatrix(180.0f * dDegreeToRad);
	Toes_LPinMatrix.m_posit = dVector(_Pos.x - l_hip / 2, _Pos.y + (h_foot / 2), _Pos.z - l_foot);
	dCustomHinge* Toes_L = new dCustomHinge(Toes_LPinMatrix, ToesC_L->GetBody(), PlantarC_L->GetBody());
	m_winManager->aManager->vJointList.push_back(Toes_L);

	// RIGHT LEG //
	Thigh_R = new GeomNewton(m_winManager->aManager);
	Thigh_R->SetBodyType(adtDynamic);
	Thigh_R->SetParent(Thigh_L);
	Thigh_R->SetTexture0(&tex[0], "Tex0");
	Thigh_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Thigh_R->SetPosition(l_hip, 0, 0);
	Thigh_R->InitNewton(atCapsule, r_leg, r_leg, l_Thigh, masses[5]);
	NewtonBodySetMassMatrix(Thigh_R->GetBody(), masses[5], Ixx[5], Iyy[5], Izz[5]); //set mass matrix 
	Thigh_RNode = new dModelNode(Thigh_R->GetBody(), dGetIdentityMatrix(), this);
	com.m_x = DeltaCMX[5];
	com.m_y = 0.0f;
	com.m_z = 0.0f;
	NewtonBodySetCentreOfMass(Thigh_R->GetBody(), &com[0]); // WIP

	Shank_R = new GeomNewton(m_winManager->aManager);
	Shank_R->SetBodyType(adtDynamic);
	Shank_R->SetParent(Thigh_R);
	Shank_R->SetTexture0(&tex[0], "Tex0");
	Shank_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Shank_R->SetPosition(0, -(l_Thigh / 2 + l_Shank / 2), 0);
	Shank_R->InitNewton(atCapsule, r_leg, r_leg, l_Shank, masses[6]);
	NewtonBodySetMassMatrix(Shank_R->GetBody(), masses[6], Ixx[6], Iyy[6], Izz[6]); //set mass matrix 
	Shank_RNode = new dModelNode(Shank_R->GetBody(), dGetIdentityMatrix(), Thigh_RNode);
	com.m_x = DeltaCMX[6];
	com.m_y = 0.0f;
	com.m_z = 0.0f;
	NewtonBodySetCentreOfMass(Shank_R->GetBody(), &com[0]); // WIP

	// RIGHT FOOT //
	PlantarC_R = new GeomNewton(m_winManager->aManager);
	PlantarC_R->SetBodyType(adtDynamic);
	PlantarC_R->SetParent(Shank_R);
	PlantarC_R->SetTexture0(&tex[0], "Tex0");
	PlantarC_R->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	PlantarC_R->SetPosition(0, -(l_Shank / 2 + h_foot / 2), 0);
	PlantarC_R->InitNewton(atCompound, 2.0f, 0.0f, 0.0f, masses[7]);
	PlantarC_RNode = new dModelNode(PlantarC_R->GetBody(), dGetIdentityMatrix(), Shank_RNode);
	//com.m_x = DeltaCMX[7];
	//com.m_y = 0.0f;
	//com.m_z = 0.0f;
	//NewtonBodySetCentreOfMass(Plantar_R->GetBody(), &com[0]); // WIP

	ToesC_R = new GeomNewton(m_winManager->aManager);
	ToesC_R->SetBodyType(adtDynamic);
	ToesC_R->SetParent(PlantarC_R);
	ToesC_R->SetTexture0(&tex[0], "Tex0");
	ToesC_R->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	ToesC_R->SetPosition(0.0f, 0.0f, -l_foot);
	ToesC_R->InitNewton(atCompound, 1.0f, 0.0f, 0.0f, masses[8]); // MODIFICARE
	ToesC_RNode = new dModelNode(ToesC_R->GetBody(), dGetIdentityMatrix(), PlantarC_RNode);
	//com.m_x = DeltaCMX[8];
	//com.m_y = 0.0f;
	//com.m_z = 0.0f;
	//NewtonBodySetCentreOfMass(Toe_R->GetBody(), &com[0]); // WIP

	// RIGHT KNEE JOINT //
	dMatrix Knee_RPinMatrix(dGetIdentityMatrix());
	Knee_RPinMatrix = Knee_RPinMatrix * dYawMatrix(180.0f * dDegreeToRad);
	Knee_RPinMatrix.m_posit = dVector(_Pos.x + l_hip / 2, _Pos.y + l_Shank + h_foot, _Pos.z);
	dCustomHinge* Knee_R = new dCustomHinge(Knee_RPinMatrix, Shank_R->GetBody(), Thigh_R->GetBody());
	Knee_R->EnableLimits(true);
	Knee_R->SetLimits(0.0f * dDegreeToRad, 150.0f * dDegreeToRad); // limit rotation of knee between 0� and 150�
	m_winManager->aManager->vJointList.push_back(Knee_R);
	// RIGHT ANKLE JOINT //
	dMatrix Ankle_RPinMatrix(dGetIdentityMatrix());
	Ankle_RPinMatrix = Ankle_RPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Ankle_RPinMatrix.m_posit = dVector(_Pos.x + l_hip / 2, _Pos.y + h_foot, _Pos.z);
	dCustomBallAndSocket* Ankle_R = new dCustomBallAndSocket(Ankle_RPinMatrix, Shank_R->GetBody(), PlantarC_R->GetBody());
	m_winManager->aManager->vJointList.push_back(Ankle_R);
	// RIGHT TOES JOINT //
	dMatrix ToesC_RPinMatrix(dGetIdentityMatrix());
	ToesC_RPinMatrix = ToesC_RPinMatrix * dYawMatrix(180.0f * dDegreeToRad);
	ToesC_RPinMatrix.m_posit = dVector(_Pos.x + l_hip / 2, _Pos.y + h_foot / 2, _Pos.z - l_foot / 2);
	dCustomHinge* Toes_R = new dCustomHinge(ToesC_RPinMatrix, PlantarC_R->GetBody(), ToesC_R->GetBody());
	m_winManager->aManager->vJointList.push_back(Toes_R);

	// HEAD //
	Head = new GeomNewton(m_winManager->aManager);
	Head->SetBodyType(adtDynamic);
	Head->SetParent(Thigh_L);
	Head->SetTexture0(&tex[0], "Tex0");
	Head->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Head->SetPosition(l_hip / 2, l_Thigh / 2 + + l_trunk + l_head / 2, 0);
	Head->InitNewton(atCapsule, r_leg, r_leg, l_head, masses[0]);
	NewtonBodySetMassMatrix(Head->GetBody(), masses[0], Ixx[0], Iyy[0], Izz[0]); //set mass matrix 
	Head_Node = new dModelNode(Head->GetBody(), dGetIdentityMatrix(), this);
	com.m_x = DeltaCMX[0];
	com.m_y = 0.0f;
	com.m_z = 0.0f;
	NewtonBodySetCentreOfMass(Head->GetBody(), &com[0]); // WIP

	if (0) // if 1 model with trunk compound collision, 0 with rigid joints
	{	
		TrunkCompound = new GeomNewton(m_winManager->aManager);
		TrunkCompound->SetBodyType(adtDynamic);
		TrunkCompound->SetParent(Thigh_L);
		TrunkCompound->SetTexture0(&tex[0], "Tex0");
		TrunkCompound->SetDiffuseColor(1.0f, 1.0f, 1.0f);
		TrunkCompound->SetPosition(l_hip / 2, l_Shank / 2 + l_trunk / 2, 0.0f);
		TrunkCompound->InitNewton(atCompound, 0.0f, 0.0f, 0.0f, masses[1]);
		NewtonBodyGetCentreOfMass(TrunkCompound->GetBody(), &com[0]);
		TrunkCompoundNode = new dModelNode(TrunkCompound->GetBody(), dGetIdentityMatrix(), this);
		//com.m_y = com.m_y + DeltaCMX[1];
		//com.m_z = 0.0f;
		//NewtonBodySetCentreOfMass(TrunkCompound->GetBody(), &com[0]); // WIP

		Trunk = TrunkCompound;
		Trunk_Node = TrunkCompoundNode;
	}
	else
	{ // TRUNK //
		Trunk = new GeomNewton(m_winManager->aManager);
		Trunk->SetBodyType(adtDynamic);
		Trunk->SetParent(Thigh_L);
		Trunk->SetTexture0(&tex[0], "Tex0");
		Trunk->SetDiffuseColor(0.7f, 0.7f, 0.7f);
		Trunk->SetPosition(l_hip / 2, (l_Thigh + l_trunk) / 2, 0);
		Trunk->InitNewton(atCapsule, r_leg, r_leg, l_trunk, masses[1]);
		NewtonBodySetMassMatrix(Trunk->GetBody(), masses[1], Ixx[1], Iyy[1], Izz[1]); //set mass matrix 
		Trunk_Node = new dModelNode(Trunk->GetBody(), dGetIdentityMatrix(), this);
		//com.m_x = DeltaCMX[1];
		//com.m_y = 0.0f;
		//com.m_z = 0.0f;
		//NewtonBodySetCentreOfMass(Trunk->GetBody(), &com[0]); // WIP

		// HIP R //
		Hip_R = new GeomNewton(m_winManager->aManager);
		Hip_R->SetBodyType(adtDynamic);
		Hip_R->SetParent(Trunk);
		Hip_R->SetRollAngle(90.0f);
		Hip_R->SetTexture0(&tex[0], "Tex0");
		Hip_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
		Hip_R->SetPosition(l_hip / 4, (-l_trunk) / 2, 0);
		Hip_R->InitNewton(atCapsule, r_leg, r_leg, l_hip / 2, masses[1]); //mass value??
		Hip_R_Node = new dModelNode(Hip_R->GetBody(), dGetIdentityMatrix(), Trunk_Node);
		// COM????

		// HIP L //
		Hip_L = new GeomNewton(m_winManager->aManager);
		Hip_L->SetBodyType(adtDynamic);
		Hip_L->SetParent(Trunk);
		Hip_L->SetRollAngle(90.0f);
		Hip_L->SetTexture0(&tex[0], "Tex0");
		Hip_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
		Hip_L->SetPosition(-l_hip / 4, (-l_trunk) / 2, 0);
		Hip_L->InitNewton(atCapsule, r_leg, r_leg, l_hip / 2, masses[1]); //mass value??
		Hip_L_Node = new dModelNode(Hip_L->GetBody(), dGetIdentityMatrix(), Trunk_Node);
		// COM????

		// LEFT HIP-LEG JOINT //
		dMatrix Hip_LLMatrix(dGetIdentityMatrix());
		Hip_LLMatrix = Hip_LLMatrix * dYawMatrix(90.0f * dDegreeToRad);
		Hip_LLMatrix.m_posit = dVector(_Pos.x - l_hip / 2, _Pos.y + l_Thigh + l_Shank + h_foot, _Pos.z);
		dCustomBallAndSocket* hip_leg_L = new dCustomBallAndSocket(Hip_LLMatrix, Hip_L->GetBody(), Thigh_L->GetBody());
		m_winManager->aManager->vJointList.push_back(hip_leg_L);

		// RIGHT HIP-LEG JOINT //
		dMatrix Hip_RRMatrix(dGetIdentityMatrix());
		Hip_RRMatrix = Hip_RRMatrix * dYawMatrix(90.0f * dDegreeToRad);
		Hip_RRMatrix.m_posit = dVector(_Pos.x + l_hip / 2, _Pos.y + l_Thigh + l_Shank + h_foot, _Pos.z);
		dCustomBallAndSocket* hip_leg_R = new dCustomBallAndSocket(Hip_RRMatrix, Hip_R->GetBody(), Thigh_R->GetBody());
		m_winManager->aManager->vJointList.push_back(hip_leg_R);

		// RIGHT HIP-TRUNK JOINT // Stiff joint
		dMatrix HipTrunk_RMatrix(dGetIdentityMatrix());
		HipTrunk_RMatrix.m_posit = dVector(_Pos.x, _Pos.y + l_Thigh + l_Shank + h_foot, _Pos.z);
		dCustomHinge* Hip_Trunk_R = new dCustomHinge(HipTrunk_RMatrix, Hip_R->GetBody(), Trunk->GetBody());
		Hip_Trunk_R->SetAsSpringDamper(1, 0.9f, 0.9f, 0.0f);
		m_winManager->aManager->vJointList.push_back(Hip_Trunk_R);

		// LEFT HIP-TRUNK JOINT // Stiff joint
		dMatrix HipTrunk_LMatrix(dGetIdentityMatrix());
		HipTrunk_LMatrix.m_posit = dVector(_Pos.x, _Pos.y + l_Thigh + l_Shank + h_foot, _Pos.z);
		dCustomHinge* Hip_Trunk_L = new dCustomHinge(HipTrunk_LMatrix, Hip_L->GetBody(), Trunk->GetBody());
		Hip_Trunk_L->SetAsSpringDamper(1, 0.9f, 0.9f, 0.0f);
		m_winManager->aManager->vJointList.push_back(Hip_Trunk_L);

		// HEAD-TRUNK JOINT //
		dMatrix shoulder_neckMatrix(dGetIdentityMatrix());
		shoulder_neckMatrix = shoulder_neckMatrix * dYawMatrix(90.0f * dDegreeToRad);
		shoulder_neckMatrix.m_posit = dVector(_Pos.x, _Pos.y + l_Thigh + l_Shank + h_foot + l_trunk, _Pos.z);
		dCustomBallAndSocket* Head_trunk = new dCustomBallAndSocket(shoulder_neckMatrix, Trunk->GetBody(), Head->GetBody());
		m_winManager->aManager->vJointList.push_back(Head_trunk);
	}

	// LEFT HIP-LEG JOINT //
	dMatrix Hip_LLMatrix(dGetIdentityMatrix());
	Hip_LLMatrix = Hip_LLMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Hip_LLMatrix.m_posit = dVector(_Pos.x - l_hip / 2, _Pos.y + l_Thigh + l_Shank + h_foot, _Pos.z);
	dCustomBallAndSocket* hip_leg_L = new dCustomBallAndSocket(Hip_LLMatrix, Trunk->GetBody(), Thigh_L->GetBody());
	m_winManager->aManager->vJointList.push_back(hip_leg_L);

	// RIGHT HIP-LEG JOINT //
	dMatrix Hip_RRMatrix(dGetIdentityMatrix());
	Hip_RRMatrix = Hip_RRMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Hip_RRMatrix.m_posit = dVector(_Pos.x + l_hip / 2, _Pos.y + l_Thigh + l_Shank + h_foot, _Pos.z);
	dCustomBallAndSocket* hip_leg_R = new dCustomBallAndSocket(Hip_RRMatrix, Trunk->GetBody(), Thigh_R->GetBody());
	m_winManager->aManager->vJointList.push_back(hip_leg_R);

	// HEAD-TRUNK JOINT //
	dMatrix shoulder_neckMatrix(dGetIdentityMatrix());
	shoulder_neckMatrix = shoulder_neckMatrix * dYawMatrix(90.0f * dDegreeToRad);
	shoulder_neckMatrix.m_posit = dVector(_Pos.x, _Pos.y + l_Thigh + l_Shank + h_foot + l_trunk, _Pos.z);
	dCustomBallAndSocket* Head_trunk = new dCustomBallAndSocket(shoulder_neckMatrix, Trunk->GetBody(), Head->GetBody());
	m_winManager->aManager->vJointList.push_back(Head_trunk);

	// RIGHT ARM //
	Arm_R = new GeomNewton(m_winManager->aManager);
	Arm_R->SetBodyType(adtDynamic);
	Arm_R->SetParent(Trunk);
	Arm_R->SetTexture0(&tex[0], "Tex0");
	Arm_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Arm_R->SetPosition( l_shoulders / 2, l_trunk / 2 - l_arm / 2, 0);
	Arm_R->InitNewton(atCapsule, r_leg, r_leg, l_arm, masses[2]);
	NewtonBodySetMassMatrix(Arm_R->GetBody(), masses[2], Ixx[2], Iyy[2], Izz[2]); //set mass matrix 
	Arm_RNode = new dModelNode(Arm_R->GetBody(), dGetIdentityMatrix(), Trunk_Node);
	com.m_x = DeltaCMX[2];
	com.m_y = 0.0f;
	com.m_z = 0.0f;
	NewtonBodySetCentreOfMass(Arm_R->GetBody(), &com[0]); // WIP

	// RIGHT SHOULDER-ARM JOINT //
	dMatrix shoulder_arm_RMatrix(dGetIdentityMatrix());
	shoulder_arm_RMatrix = shoulder_arm_RMatrix * dYawMatrix(90.0f * dDegreeToRad);
	shoulder_arm_RMatrix.m_posit = dVector(_Pos.x + l_shoulders / 2, _Pos.y + l_Thigh + l_Shank + h_foot + l_trunk, _Pos.z);
	dCustomBallAndSocket* Shoulders_arm_R = new dCustomBallAndSocket(shoulder_arm_RMatrix, Trunk->GetBody(), Arm_R->GetBody());
	m_winManager->aManager->vJointList.push_back(Shoulders_arm_R);
	// RIGHT FOREARM //
	Forearm_R = new GeomNewton(m_winManager->aManager);
	Forearm_R->SetBodyType(adtDynamic);
	Forearm_R->SetParent(Arm_R);
	Forearm_R->SetTexture0(&tex[0], "Tex0");
	Forearm_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Forearm_R->SetPosition( 0,  - l_arm / 2 - l_farm / 2, 0);
	Forearm_R->InitNewton(atCapsule, r_leg, r_leg, l_farm, masses[3]);
	NewtonBodySetMassMatrix(Forearm_R->GetBody(), masses[3], Ixx[3], Iyy[3], Izz[3]); //set mass matrix 
	Forearm_RNode = new dModelNode(Forearm_R->GetBody(), dGetIdentityMatrix(), Arm_RNode);
	com.m_x = DeltaCMX[3];
	com.m_y = 0.0f;
	com.m_z = 0.0f;
	NewtonBodySetCentreOfMass(Forearm_R->GetBody(), &com[0]); // WIP

	// RIGHT FOREARM-ARM JOINT //
	dMatrix farm_arm_RMatrix(dGetIdentityMatrix());
	farm_arm_RMatrix = farm_arm_RMatrix * dYawMatrix(90.0f * dDegreeToRad);
	farm_arm_RMatrix.m_posit = dVector(_Pos.x + l_shoulders / 2, _Pos.y + l_Thigh + l_Shank + h_foot + l_trunk - l_arm, _Pos.z);
	dCustomHinge* Farm_arm_R = new dCustomHinge(farm_arm_RMatrix, Forearm_R->GetBody(), Arm_R->GetBody());
	m_winManager->aManager->vJointList.push_back(Farm_arm_R);

	// RIGHT HAND //
	Hand_R = new GeomNewton(m_winManager->aManager);
	Hand_R->SetBodyType(adtDynamic);
	Hand_R->SetParent(Forearm_R);
	Hand_R->SetTexture0(&tex[0], "Tex0");
	Hand_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Hand_R->SetPosition(0, -l_hand / 2 - l_farm / 2, 0);
	Hand_R->InitNewton(atCapsule, r_leg, r_leg, l_hand, masses[4]);
	NewtonBodySetMassMatrix(Hand_R->GetBody(), masses[4], Ixx[4], Iyy[4], Izz[4]); //set mass matrix 
	Hand_RNode = new dModelNode(Hand_R->GetBody(), dGetIdentityMatrix(), Forearm_RNode);
	com.m_x = DeltaCMX[4];
	com.m_y = 0.0f;
	com.m_z = 0.0f;
	NewtonBodySetCentreOfMass(Hand_R->GetBody(), &com[0]); // WIP

	// RIGHT FOREARM-HAND JOINT //
	dMatrix hand_arm_RMatrix(dGetIdentityMatrix());
	hand_arm_RMatrix = hand_arm_RMatrix * dYawMatrix(90.0f * dDegreeToRad);
	hand_arm_RMatrix.m_posit = dVector(_Pos.x + l_shoulders / 2, _Pos.y + l_Thigh + l_Shank + h_foot + l_trunk - l_arm - l_farm, _Pos.z);
	dCustomBallAndSocket* arm_hand_R = new dCustomBallAndSocket(hand_arm_RMatrix, Hand_R->GetBody(), Forearm_R->GetBody());
	m_winManager->aManager->vJointList.push_back(Hand_arm_R);

	//// LEFT ARM //
	Arm_L = new GeomNewton(m_winManager->aManager);
	Arm_L->SetBodyType(adtDynamic);
	Arm_L->SetParent(Trunk);
	Arm_L->SetTexture0(&tex[0], "Tex0");
	Arm_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Arm_L->SetPosition(-l_shoulders / 2, l_trunk/2 -l_arm / 2, 0);
	Arm_L->InitNewton(atCapsule, r_leg, r_leg, l_arm, masses[2]);
	NewtonBodySetMassMatrix(Arm_L->GetBody(), masses[2], Ixx[2], Iyy[2], Izz[2]); //set mass matrix 
	Arm_LNode = new dModelNode(Arm_L->GetBody(), dGetIdentityMatrix(), Trunk_Node);
	com.m_x = DeltaCMX[2];
	com.m_y = 0.0f;
	com.m_z = 0.0f;
	NewtonBodySetCentreOfMass(Arm_L->GetBody(), &com[0]); // WIP

	// LEFT SHOULDER-ARM JOINT //
	dMatrix shoulder_arm_LMatrix(dGetIdentityMatrix());
	shoulder_arm_LMatrix = shoulder_arm_LMatrix * dYawMatrix(90.0f * dDegreeToRad);
	shoulder_arm_LMatrix.m_posit = dVector(_Pos.x - l_shoulders / 2, _Pos.y + l_Thigh + l_Shank + h_foot + l_trunk, _Pos.z);
	dCustomBallAndSocket* Shoulders_arm_L = new dCustomBallAndSocket(shoulder_arm_LMatrix, Trunk->GetBody(), Arm_L->GetBody());
	m_winManager->aManager->vJointList.push_back(Shoulders_arm_L);
	// LEFT FOREARM //
	Forearm_L = new GeomNewton(m_winManager->aManager);
	Forearm_L->SetBodyType(adtDynamic);
	Forearm_L->SetParent(Arm_L);
	Forearm_L->SetTexture0(&tex[0], "Tex0");
	Forearm_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Forearm_L->SetPosition(0, -l_arm / 2 - l_farm / 2, 0);
	Forearm_L->InitNewton(atCapsule, r_leg, r_leg, l_farm, masses[3]);
	NewtonBodySetMassMatrix(Forearm_L->GetBody(), masses[3], Ixx[3], Iyy[3], Izz[3]); //set mass matrix 
	Forearm_LNode = new dModelNode(Forearm_L->GetBody(), dGetIdentityMatrix(), Arm_LNode);
	com.m_x = DeltaCMX[3];
	com.m_y = 0.0f;
	com.m_z = 0.0f;
	NewtonBodySetCentreOfMass(Forearm_L->GetBody(), &com[0]); // WIP

	// LEFT FOREARM-ARM JOINT //
	dMatrix farm_arm_LMatrix(dGetIdentityMatrix());
	farm_arm_LMatrix = farm_arm_LMatrix * dYawMatrix(90.0f * dDegreeToRad);
	farm_arm_LMatrix.m_posit = dVector(_Pos.x - l_shoulders / 2, _Pos.y + l_Thigh + l_Shank + h_foot + l_trunk - l_arm, _Pos.z);
	dCustomHinge* Farm_arm_L = new dCustomHinge(farm_arm_LMatrix, Forearm_L->GetBody(), Arm_L->GetBody());
	m_winManager->aManager->vJointList.push_back(Farm_arm_L);

	// LEFT HAND //
	Hand_L = new GeomNewton(m_winManager->aManager);
	Hand_L->SetBodyType(adtDynamic);
	Hand_L->SetParent(Forearm_L);
	Hand_L->SetTexture0(&tex[0], "Tex0");
	Hand_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Hand_L->SetPosition(0, -l_hand / 2 - l_farm / 2, 0);
	Hand_L->InitNewton(atCapsule, r_leg, r_leg, l_hand, masses[4]);
	NewtonBodySetMassMatrix(Hand_L->GetBody(), masses[4], Ixx[4], Iyy[4], Izz[4]); //set mass matrix 
	Hand_LNode = new dModelNode(Hand_L->GetBody(), dGetIdentityMatrix(), Forearm_LNode);
	com.m_x = DeltaCMX[4];
	com.m_y = 0.0f;
	com.m_z = 0.0f;
	NewtonBodySetCentreOfMass(Hand_L->GetBody(), &com[0]); // WIP

	// LEFT FOREARM-HAND JOINT //
	dMatrix hand_arm_LMatrix(dGetIdentityMatrix());
	hand_arm_LMatrix = hand_arm_LMatrix * dYawMatrix(90.0f * dDegreeToRad);
	hand_arm_LMatrix.m_posit = dVector(_Pos.x - l_shoulders / 2, _Pos.y + l_Thigh + l_Shank + h_foot + l_trunk - l_arm - l_farm, _Pos.z);
	dCustomBallAndSocket* arm_hand_L = new dCustomBallAndSocket(hand_arm_LMatrix, Hand_L->GetBody(), Forearm_L->GetBody());
	m_winManager->aManager->vJointList.push_back(Hand_arm_L);

	ins11 = dVector(0.f, 0.f, 0.f);
	ins12 = dVector(0.f, 0.f, 0.f);
	m1 = new Muscle(m_winManager->aLineManager, m_winManager->aManager, GetThigh_L(), GetShank_L(), ins11, ins12);
	m1->GenerateMesh();
	CreateFootScanLine();
}

GeomNewton* dRaycastVHModel::GetTrunkCompound() {
	return TrunkCompound;
}

GeomNewton* dRaycastVHModel::GetThigh_L() {
	return Thigh_L;
}

GeomNewton* dRaycastVHModel::GetShank_L() {
	return Shank_L;
}

GeomNewton* dRaycastVHModel::GetPlantarC_L() {
	return PlantarC_L;
}

GeomNewton* dRaycastVHModel::GetToesC_L() {
	return ToesC_L;
}

GeomNewton* dRaycastVHModel::GetThigh_R() {
	return Thigh_R;
}

GeomNewton* dRaycastVHModel::GetShank_R() {
	return Shank_R;
}

GeomNewton* dRaycastVHModel::GetPlantarC_R() {
	return PlantarC_R;
}

GeomNewton* dRaycastVHModel::GetToesC_R() {
	return ToesC_R;
}

GeomNewton* dRaycastVHModel::GetTrunk() {
	return Trunk;
}

GeomNewton* dRaycastVHModel::GetHead() {
	return Head;
}


GeomNewton* dRaycastVHModel::GetArm_R() {
	return Arm_R;
}

GeomNewton* dRaycastVHModel::GetForearm_R() {
	return Forearm_R;
}

GeomNewton* dRaycastVHModel::GetHand_R() {
	return Hand_R;
}

GeomNewton* dRaycastVHModel::GetArm_L() {
	return Arm_L;
}

GeomNewton* dRaycastVHModel::GetForearm_L() {
	return Forearm_L;
}

GeomNewton* dRaycastVHModel::GetHand_L() {
	return Hand_L;
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

FootLineIndex_L = m_winManager->aLineManager->AddLine(linepos1, linepos2, linecolor);
}

void dRaycastVHModel::CastFoot_L() {
	NewtonCollision* const collisionA = NewtonBodyGetCollision(PlantarC_L->GetBody());
    dMatrix matrixA;
	NewtonBodyGetMatrix(PlantarC_L->GetBody(), &matrixA[0][0]);

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

