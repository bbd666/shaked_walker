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
	, Plantar_L(NULL)
	, Toe_L(NULL)
	, Knee_L(NULL)
	, Ankle_L(NULL)
	, Flextoe_L(NULL)
	, Thigh_R(NULL)
	, Shank_R(NULL)
	, Plantar_R(NULL)
	, Toe_R(NULL)
	, Knee_R(NULL)
	, Ankle_R(NULL)
	, Flextoe_R(NULL)
	, Hip(NULL)
	, Spine(NULL)
	, Hip_spine(NULL)
	, Shoulders(NULL)
	, Spine_shoulders(NULL)
	, Shoulders_neck(NULL)
	, Neck(NULL)
	, Head(NULL)
	, Neck_head(NULL)
	, Arm_R(NULL)
	, Shoulders_arm_R(NULL)
	, Forearm_R(NULL)
	, Farm_arm_R(NULL)
	, Arm_L(NULL)
	, Shoulders_arm_L(NULL)
	, Forearm_L(NULL)
	, Farm_arm_L(NULL)
	, Toe_LNode(NULL)
	, Plantar_LNode(NULL)
	, Shank_LNode(NULL)
	, Toe_RNode(NULL)
	, Plantar_RNode(NULL)
	, Shank_RNode(NULL)
	, Thigh_RNode(NULL)
	, Hip_Node(NULL)
	, Spine_Node(NULL)
	, Neck_Node(NULL)
	, Head_Node(NULL)
	, Arm_RNode(NULL)
	, Forearm_RNode(NULL)
{
	string tex("Textures//wood6.png");
	glm::vec3 _Pos(glm::vec3(0.0f, 0.25f, 0.0f));
	Scale = 1.0f;

	l_Thigh = 1.0f * Scale;//[m]
	l_Shank = 0.8f * Scale;//[m]
	r_leg = 0.1f * Scale;//[m]
	l_foot = 0.4f * Scale;//[m]
	w_foot = 0.2f * Scale; //[m]
	h_foot = 0.1f * Scale; //[m]
	l_toe = 0.2f * Scale; //[m]
	l_hip = 0.8f * Scale; //[m]
	l_spine = 1.5f * Scale; //[m]
	l_shoulders = 1.0f * Scale; //[m]
	l_neck = 0.3f * Scale; //[m]
	l_arm = 0.8f * Scale; //[m]
	l_farm = 0.5f * Scale; //[m]
	d_head = 0.2f * Scale; //[m]
	tot_w = 70.0f * Scale; // totale weight of the subject [kg]
	masses = { 10.0f * tot_w, 10.0f * tot_w, 10.0f * tot_w, 10.0f * tot_w, 10.0f * tot_w, 10.0f * tot_w, 10.0f * tot_w, 10.0f * tot_w, 10.0f * tot_w, 10.0f * tot_w, 10.0f * tot_w };
	// norm masses of: {0 head,	1 neck,		2 shoulders,	3 arm,			4 forearm,		5 spine,		6 hip,		7 thigh,		8 shank,		9 plantar,		10 toes}
	//
	// LEFT LEG //
	Thigh_L = new GeomNewton(m_winManager->aManager);
	Thigh_L->SetBodyType(adtDynamic);
	Thigh_L->SetTexture0(&tex[0], "Tex0");
	Thigh_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Thigh_L->SetRollAngle(90.0f);
	Thigh_L->SetPosition(_Pos.x - l_hip / 2, _Pos.y + l_Shank + h_foot + l_Thigh / 2, _Pos.z);
	Thigh_L->InitNewton(atCapsule, r_leg, r_leg, l_Thigh, masses[7]);
	m_body= Thigh_L->GetBody();
	NewtonBodySetTransformCallback(m_body, NULL);
	NewtonBody* const parentBody = GetBody();
	dMatrix aParenMatrix(dGetIdentityMatrix());
	NewtonBodyGetMatrix(parentBody, &aParenMatrix[0][0]);
	//NewtonBodySetMassMatrix(aBody, 10.0f, Ixx, Iyy, Izz); set mass matrix WIP

	Shank_L = new GeomNewton(m_winManager->aManager);
	Shank_L->SetBodyType(adtDynamic);
	Shank_L->SetParent(Thigh_L);
	Shank_L->SetTexture0(&tex[0], "Tex0");
	Shank_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Shank_L->SetPosition( 0, -(l_Thigh / 2 + l_Shank / 2), 0);
	Shank_L->InitNewton(atCapsule, r_leg, r_leg, l_Shank, masses[8]);
	NewtonBodySetTransformCallback(Shank_L->GetBody(), NULL);
	Shank_LNode = new dModelNode(Shank_L->GetBody(), dGetIdentityMatrix(), this);

	// LEFT FOOT //
	Plantar_L = new GeomNewton(m_winManager->aManager);
	Plantar_L->SetBodyType(adtDynamic);
	Plantar_L->SetParent(Shank_L);
	Plantar_L->SetTexture0(&tex[0], "Tex0");
	Plantar_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Plantar_L->SetPosition( 0, -(l_Shank / 2 + h_foot / 2), 0);
	Plantar_L->InitNewton(atBox, h_foot, w_foot, l_foot, masses[9]);
	NewtonBodySetTransformCallback(Plantar_L->GetBody(), NULL);
	Plantar_LNode = new dModelNode(Plantar_L->GetBody(), dGetIdentityMatrix(), Shank_LNode);

	Toe_L = new GeomNewton(m_winManager->aManager);
	Toe_L->SetBodyType(adtDynamic);
	Toe_L->SetParent(Plantar_L);
	Toe_L->SetTexture0(&tex[0], "Tex0");
	Toe_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Toe_L->SetPosition( 0, 0, - (l_foot / 2 + l_toe / 2));
	Toe_L->InitNewton(atBox, h_foot, w_foot, l_toe, masses[10]); // MODIFICARE
	NewtonBodySetTransformCallback(Toe_L->GetBody(), NULL);
	Toe_LNode = new dModelNode(Toe_L->GetBody(), dGetIdentityMatrix(), Plantar_LNode);

	// LEFT KNEE JOINT //
	dMatrix Knee_LPinMatrix(dGetIdentityMatrix());
	Knee_LPinMatrix = Knee_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Knee_LPinMatrix.m_posit = dVector(_Pos.x - l_hip / 2, _Pos.y + l_Shank + h_foot, _Pos.z);
	dCustomHinge* Knee_L = new dCustomHinge(Knee_LPinMatrix, Shank_L->GetBody(), Thigh_L->GetBody());
	Knee_L->EnableLimits(true);
	Knee_L->SetLimits(0.0f * dDegreeToRad, 150.0f * dDegreeToRad); // limit rotation of knee between 0° and 150°
	m_winManager->aManager->vJointList.push_back(Knee_L);
	// LEFT ANKLE JOINT //
	dMatrix Ankle_LPinMatrix(dGetIdentityMatrix());
	Ankle_LPinMatrix = Ankle_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Ankle_LPinMatrix.m_posit = dVector(_Pos.x - l_hip / 2, _Pos.y + h_foot, _Pos.z);
	dCustomBallAndSocket* Ankle_L = new dCustomBallAndSocket(Ankle_LPinMatrix, Shank_L->GetBody(), Plantar_L->GetBody());
	m_winManager->aManager->vJointList.push_back(Ankle_L);
	// LEFT TOES JOINT //
	dMatrix Toe_LPinMatrix(dGetIdentityMatrix());
	Toe_LPinMatrix = Toe_LPinMatrix * dYawMatrix(00.0f * dDegreeToRad);
	Toe_LPinMatrix.m_posit = dVector(_Pos.x - l_hip / 2, _Pos.y + h_foot / 2, _Pos.z - l_foot / 2);
	dCustomHinge* Flextoe_L = new dCustomHinge(Toe_LPinMatrix, Plantar_L->GetBody(), Toe_L->GetBody());
	m_winManager->aManager->vJointList.push_back(Flextoe_L);

	// RIGHT LEG //
	Thigh_R = new GeomNewton(m_winManager->aManager);
	Thigh_R->SetBodyType(adtDynamic);
	Thigh_R->SetParent(Thigh_L);
	Thigh_R->SetTexture0(&tex[0], "Tex0");
	Thigh_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Thigh_R->SetPosition(l_hip, 0, 0);
	Thigh_R->InitNewton(atCapsule, r_leg, r_leg, l_Thigh, masses[7]);
	NewtonBodySetTransformCallback(Thigh_R->GetBody(), NULL);
	Thigh_RNode = new dModelNode(Thigh_R->GetBody(), dGetIdentityMatrix(), this);
	//NewtonBodySetMassMatrix(aBody, 10.0f, Ixx, Iyy, Izz); set mass matrix WIP

	Shank_R = new GeomNewton(m_winManager->aManager);
	Shank_R->SetBodyType(adtDynamic);
	Shank_R->SetParent(Thigh_R);
	Shank_R->SetTexture0(&tex[0], "Tex0");
	Shank_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Shank_R->SetPosition(0, -(l_Thigh / 2 + l_Shank / 2), 0);
	Shank_R->InitNewton(atCapsule, r_leg, r_leg, l_Shank, masses[8]);
	NewtonBodySetTransformCallback(Shank_R->GetBody(), NULL);
	Shank_RNode = new dModelNode(Shank_R->GetBody(), dGetIdentityMatrix(), Thigh_RNode);

	// RIGHT FOOT //
	Plantar_R = new GeomNewton(m_winManager->aManager);
	Plantar_R->SetBodyType(adtDynamic);
	Plantar_R->SetParent(Shank_R);
	Plantar_R->SetTexture0(&tex[0], "Tex0");
	Plantar_R->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Plantar_R->SetPosition(0, -(l_Shank / 2 + h_foot / 2), 0);
	Plantar_R->InitNewton(atBox, h_foot, w_foot, l_foot, masses[9]);
	NewtonBodySetTransformCallback(Plantar_R->GetBody(), NULL);
	Plantar_RNode = new dModelNode(Plantar_R->GetBody(), dGetIdentityMatrix(), Shank_RNode);

	Toe_R = new GeomNewton(m_winManager->aManager);
	Toe_R->SetBodyType(adtDynamic);
	Toe_R->SetParent(Plantar_R);
	Toe_R->SetTexture0(&tex[0], "Tex0");
	Toe_R->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Toe_R->SetPosition(0, 0, -(l_foot / 2 + l_toe / 2));
	Toe_R->InitNewton(atBox, h_foot, w_foot, l_toe, masses[10]); // MODIFICARE
	NewtonBodySetTransformCallback(Toe_L->GetBody(), NULL);
	Toe_RNode = new dModelNode(Toe_L->GetBody(), dGetIdentityMatrix(), Plantar_RNode);

	// RIGHT KNEE JOINT //
	dMatrix Knee_RPinMatrix(dGetIdentityMatrix());
	Knee_RPinMatrix = Knee_RPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Knee_RPinMatrix.m_posit = dVector(_Pos.x + l_hip / 2, _Pos.y + l_Shank + h_foot, _Pos.z);
	dCustomHinge* Knee_R = new dCustomHinge(Knee_RPinMatrix, Shank_R->GetBody(), Thigh_R->GetBody());
	Knee_R->EnableLimits(true);
	Knee_R->SetLimits(0.0f * dDegreeToRad, 150.0f * dDegreeToRad); // limit rotation of knee between 0° and 150°
	m_winManager->aManager->vJointList.push_back(Knee_R);
	// RIGHT ANKLE JOINT //
	dMatrix Ankle_RPinMatrix(dGetIdentityMatrix());
	Ankle_RPinMatrix = Ankle_RPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Ankle_RPinMatrix.m_posit = dVector(_Pos.x + l_hip / 2, _Pos.y + h_foot, _Pos.z);
	dCustomBallAndSocket* Ankle_R = new dCustomBallAndSocket(Ankle_RPinMatrix, Shank_R->GetBody(), Plantar_R->GetBody());
	m_winManager->aManager->vJointList.push_back(Ankle_R);
	// RIGHT TOES JOINT //
	dMatrix Toe_RPinMatrix(dGetIdentityMatrix());
	Toe_RPinMatrix = Toe_RPinMatrix * dYawMatrix(00.0f * dDegreeToRad);
	Toe_RPinMatrix.m_posit = dVector(_Pos.x + l_hip / 2, _Pos.y + h_foot / 2, _Pos.z - l_foot / 2);
	dCustomHinge* Flextoe_R = new dCustomHinge(Toe_RPinMatrix, Plantar_R->GetBody(), Toe_R->GetBody());
	m_winManager->aManager->vJointList.push_back(Flextoe_R);

	// HIP //
	Hip = new GeomNewton(m_winManager->aManager);
	Hip->SetBodyType(adtDynamic);
	Hip->SetParent(Thigh_L);
	Hip->SetRollAngle(90.0f);
	Hip->SetTexture0(&tex[0], "Tex0");
	Hip->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Hip->SetPosition(l_hip/2,  l_Thigh / 2, 0);
	Hip->InitNewton(atCapsule, r_leg, r_leg, l_hip, masses[6]);
	NewtonBodySetTransformCallback(Hip->GetBody(), NULL);
	Hip_Node = new dModelNode(Hip->GetBody(), dGetIdentityMatrix(), this);
	// LEFT HIP-LEG JOINT //
	dMatrix Hip_LLMatrix(dGetIdentityMatrix());
	Hip_LLMatrix = Hip_LLMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Hip_LLMatrix.m_posit = dVector(_Pos.x - l_hip / 2, _Pos.y + l_Thigh + l_Shank + h_foot, _Pos.z);
	dCustomBallAndSocket* hip_leg_L = new dCustomBallAndSocket(Hip_LLMatrix, Hip->GetBody(), Thigh_L->GetBody());
	m_winManager->aManager->vJointList.push_back(hip_leg_L);
	// RIGHT HIP-LEG JOINT //
	dMatrix Hip_RRMatrix(dGetIdentityMatrix());
	Hip_RRMatrix = Hip_RRMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Hip_RRMatrix.m_posit = dVector(_Pos.x + l_hip / 2, _Pos.y + l_Thigh + l_Shank + h_foot, _Pos.z);
	dCustomBallAndSocket* hip_leg_R = new dCustomBallAndSocket(Hip_RRMatrix, Hip->GetBody(), Thigh_R->GetBody());
	m_winManager->aManager->vJointList.push_back(hip_leg_R);
	// SPINE //
	Spine = new GeomNewton(m_winManager->aManager);
	Spine->SetBodyType(adtDynamic);
	Spine->SetParent(Hip);
	Spine->SetTexture0(&tex[0], "Tex0");
	Spine->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Spine->SetRollAngle(90.0f);
	Spine->SetPosition(0, l_spine / 2, 0);
	Spine->InitNewton(atCapsule, r_leg, r_leg, l_spine, masses[5]);
	NewtonBodySetTransformCallback(Spine->GetBody(), NULL);
	Spine_Node = new dModelNode(Spine->GetBody(), dGetIdentityMatrix(), Hip_Node);
	// HIP-SPINE JOINT //
	dMatrix Hip_SpineMatrix(dGetIdentityMatrix());
	Hip_SpineMatrix = Hip_SpineMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Hip_SpineMatrix.m_posit = dVector(_Pos.x, _Pos.y + l_Thigh + l_Shank + h_foot, _Pos.z);
	dCustomBallAndSocket* hip_spine = new dCustomBallAndSocket(Hip_SpineMatrix, Hip->GetBody(), Spine->GetBody());
	m_winManager->aManager->vJointList.push_back(hip_spine);
	// SHOULDERS //
	Shoulders = new GeomNewton(m_winManager->aManager);
	Shoulders->SetBodyType(adtDynamic);
	Shoulders->SetParent(Spine);
	Shoulders->SetRollAngle(90.0f);
	Shoulders->SetTexture0(&tex[0], "Tex0");
	Shoulders->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Shoulders->SetPosition( 0, l_spine / 2, 0);
	Shoulders->InitNewton(atCapsule, r_leg, r_leg, l_shoulders, masses[2]);
	NewtonBodySetTransformCallback(Shoulders->GetBody(), NULL);
	Shoulders_Node = new dModelNode(Shoulders->GetBody(), dGetIdentityMatrix(), Spine_Node);
	// SHOULDERS-SPINE JOINT //
	dMatrix shoulder_SpineMatrix(dGetIdentityMatrix());
	shoulder_SpineMatrix = shoulder_SpineMatrix * dYawMatrix(90.0f * dDegreeToRad);
	shoulder_SpineMatrix.m_posit = dVector(_Pos.x, _Pos.y + l_Thigh + l_Shank + h_foot + l_spine, _Pos.z);
	dCustomBallAndSocket* shoulders_spine = new dCustomBallAndSocket(shoulder_SpineMatrix, Shoulders->GetBody(), Spine->GetBody());
	m_winManager->aManager->vJointList.push_back(shoulders_spine);
	// NECK //
	Neck = new GeomNewton(m_winManager->aManager);
	Neck->SetBodyType(adtDynamic);
	Neck->SetParent(Spine);
	Neck->SetTexture0(&tex[0], "Tex0");
	Neck->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Neck->SetPosition( 0, l_neck / 2 + l_spine / 2, 0);
	Neck->InitNewton(atCapsule, r_leg, r_leg, l_neck, masses[1]);
	NewtonBodySetTransformCallback(Neck->GetBody(), NULL);
	Neck_Node = new dModelNode(Neck->GetBody(), dGetIdentityMatrix(), Spine_Node);
	// NECK-SHOULDERS JOINT //
	dMatrix shoulder_neckMatrix(dGetIdentityMatrix());
	shoulder_neckMatrix = shoulder_neckMatrix * dYawMatrix(90.0f * dDegreeToRad);
	shoulder_neckMatrix.m_posit = dVector(_Pos.x, _Pos.y + l_Thigh + l_Shank + h_foot + l_spine, _Pos.z);
	dCustomBallAndSocket* Shoulders_neck = new dCustomBallAndSocket(shoulder_neckMatrix, Shoulders->GetBody(), Neck->GetBody());
	m_winManager->aManager->vJointList.push_back(Shoulders_neck);
	// HEAD //
	Head = new GeomNewton(m_winManager->aManager);
	Head->SetBodyType(adtDynamic);
	Head->SetParent(Neck);
	Head->SetTexture0(&tex[0], "Tex0");
	Head->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Head->SetPosition( 0, l_neck/2 , _Pos.z);
	Head->InitNewton(atSphere, d_head, 0.0f, 0.0f, masses[0]);
	NewtonBodySetTransformCallback(Head->GetBody(), NULL);
	Head_Node = new dModelNode(Head->GetBody(), dGetIdentityMatrix(), Neck_Node);
	// NECK-HEAD JOINT //
	dMatrix neck_headMatrix(dGetIdentityMatrix());
	neck_headMatrix = neck_headMatrix * dYawMatrix(90.0f * dDegreeToRad);
	neck_headMatrix.m_posit = dVector(_Pos.x, _Pos.y + l_neck + l_Thigh + l_Shank + h_foot + l_spine, _Pos.z);
	dCustomBallAndSocket* Heck_head = new dCustomBallAndSocket(neck_headMatrix, Neck->GetBody(), Head->GetBody());
	m_winManager->aManager->vJointList.push_back(Heck_head);

	// RIGHT ARM //
	Arm_R = new GeomNewton(m_winManager->aManager);
	Arm_R->SetBodyType(adtDynamic);
	Arm_R->SetParent(Shoulders);
	Arm_R->SetRollAngle(90.0f);
	Arm_R->SetTexture0(&tex[0], "Tex0");
	Arm_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Arm_R->SetPosition(  l_shoulders / 2, - l_arm / 2, 0);
	Arm_R->InitNewton(atCapsule, r_leg, r_leg, l_arm, masses[3]);
	NewtonBodySetTransformCallback(Arm_R->GetBody(), NULL);
	Arm_RNode = new dModelNode(Arm_R->GetBody(), dGetIdentityMatrix(), Shoulders_Node);
	// RIGHT SHOULDER-ARM JOINT //
	dMatrix shoulder_arm_RMatrix(dGetIdentityMatrix());
	shoulder_arm_RMatrix = shoulder_arm_RMatrix * dYawMatrix(90.0f * dDegreeToRad);
	shoulder_arm_RMatrix.m_posit = dVector(_Pos.x + l_shoulders / 2, _Pos.y + l_Thigh + l_Shank + h_foot + l_spine, _Pos.z);
	dCustomBallAndSocket* Shoulders_arm_R = new dCustomBallAndSocket(shoulder_arm_RMatrix, Shoulders->GetBody(), Arm_R->GetBody());
	m_winManager->aManager->vJointList.push_back(Shoulders_arm_R);
	// RIGHT FOREARM //
	Forearm_R = new GeomNewton(m_winManager->aManager);
	Forearm_R->SetBodyType(adtDynamic);
	Forearm_R->SetParent(Arm_R);
	Forearm_R->SetTexture0(&tex[0], "Tex0");
	Forearm_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Forearm_R->SetPosition( 0,  - l_arm / 2 - l_farm / 2, 0);
	Forearm_R->InitNewton(atCapsule, r_leg, r_leg, l_farm, masses[4]);
	NewtonBodySetTransformCallback(Arm_R->GetBody(), NULL);
	Forearm_RNode = new dModelNode(Forearm_R->GetBody(), dGetIdentityMatrix(), Arm_RNode);
	// RIGHT FOREARM-ARM JOINT //
	dMatrix farm_arm_RMatrix(dGetIdentityMatrix());
	farm_arm_RMatrix = farm_arm_RMatrix * dYawMatrix(90.0f * dDegreeToRad);
	farm_arm_RMatrix.m_posit = dVector(_Pos.x + l_shoulders / 2, _Pos.y + l_Thigh + l_Shank + h_foot + l_spine - l_arm, _Pos.z);
	dCustomHinge* Farm_arm_R = new dCustomHinge(farm_arm_RMatrix, Forearm_R->GetBody(), Arm_R->GetBody());
	m_winManager->aManager->vJointList.push_back(Farm_arm_R);
	//// LEFT ARM //
	Arm_L = new GeomNewton(m_winManager->aManager);
	Arm_L->SetBodyType(adtDynamic);
	Arm_L->SetParent(Shoulders);
	Arm_L->SetRollAngle(90.0f);
	Arm_L->SetTexture0(&tex[0], "Tex0");
	Arm_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Arm_L->SetPosition(-l_shoulders / 2, -l_arm / 2, 0);
	Arm_L->InitNewton(atCapsule, r_leg, r_leg, l_arm, masses[3]);
	NewtonBodySetTransformCallback(Arm_L->GetBody(), NULL);
	Arm_LNode = new dModelNode(Arm_L->GetBody(), dGetIdentityMatrix(), Shoulders_Node);
	// LEFT SHOULDER-ARM JOINT //
	dMatrix shoulder_arm_LMatrix(dGetIdentityMatrix());
	shoulder_arm_LMatrix = shoulder_arm_LMatrix * dYawMatrix(90.0f * dDegreeToRad);
	shoulder_arm_LMatrix.m_posit = dVector(_Pos.x - l_shoulders / 2, _Pos.y + l_Thigh + l_Shank + h_foot + l_spine, _Pos.z);
	dCustomBallAndSocket* Shoulders_arm_L = new dCustomBallAndSocket(shoulder_arm_LMatrix, Shoulders->GetBody(), Arm_L->GetBody());
	m_winManager->aManager->vJointList.push_back(Shoulders_arm_L);
	// LEFT FOREARM //
	Forearm_L = new GeomNewton(m_winManager->aManager);
	Forearm_L->SetBodyType(adtDynamic);
	Forearm_L->SetParent(Arm_L);
	Forearm_L->SetTexture0(&tex[0], "Tex0");
	Forearm_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Forearm_L->SetPosition(0, -l_arm / 2 - l_farm / 2, 0);
	Forearm_L->InitNewton(atCapsule, r_leg, r_leg, l_farm, masses[4]);
	NewtonBodySetTransformCallback(Arm_L->GetBody(), NULL);
	Forearm_LNode = new dModelNode(Forearm_L->GetBody(), dGetIdentityMatrix(), Arm_LNode);
	// LEFT FOREARM-ARM JOINT //
	dMatrix farm_arm_LMatrix(dGetIdentityMatrix());
	farm_arm_LMatrix = farm_arm_LMatrix * dYawMatrix(90.0f * dDegreeToRad);
	farm_arm_LMatrix.m_posit = dVector(_Pos.x - l_shoulders / 2, _Pos.y + l_Thigh + l_Shank + h_foot + l_spine - l_arm, _Pos.z);
	dCustomHinge* Farm_arm_L = new dCustomHinge(farm_arm_LMatrix, Forearm_L->GetBody(), Arm_L->GetBody());
	m_winManager->aManager->vJointList.push_back(Farm_arm_L);

	ins11 = dVector(0.f, 0.f, 0.f);
	ins12 = dVector(0.f, 0.f, 0.f);
	m1 = new Muscle(m_winManager->aLineManager, m_winManager->aManager, GetThigh_L(), GetShank_L(), ins11, ins12);
	m1->GenerateMesh();
	CreateFootScanLine();
}

GeomNewton* dRaycastVHModel::GetThigh_L() {
	return Thigh_L;
}

GeomNewton* dRaycastVHModel::GetShank_L() {
	return Shank_L;
}

GeomNewton* dRaycastVHModel::GetPlantar_L() {
	return Plantar_L;
}

GeomNewton* dRaycastVHModel::GetToe_L() {
	return Toe_L;
}

GeomNewton* dRaycastVHModel::GetThigh_R() {
	return Thigh_R;
}

GeomNewton* dRaycastVHModel::GetShank_R() {
	return Shank_R;
}

GeomNewton* dRaycastVHModel::GetPlantar_R() {
	return Plantar_R;
}

GeomNewton* dRaycastVHModel::GetToe_R() {
	return Toe_R;
}

GeomNewton* dRaycastVHModel::GetHip() {
	return Hip;
}

GeomNewton* dRaycastVHModel::GetSpine() {
	return Spine;
}

GeomNewton* dRaycastVHModel::GetShoulders() {
	return Shoulders;
}

GeomNewton* dRaycastVHModel::GetNeck() {
	return Neck;
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

GeomNewton* dRaycastVHModel::GetArm_L() {
	return Arm_L;
}

GeomNewton* dRaycastVHModel::GetForearm_L() {
	return Forearm_L;
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