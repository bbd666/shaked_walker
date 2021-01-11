#include "pch.h"
#include "OXVehicleRayCast.h"

dRaycastVHModel::~dRaycastVHModel()
{
	 
}

dRaycastVHModel::dRaycastVHModel(WindowMain* winctx, const char* const modelName, const dMatrix& location, int linkMaterilID)
	: dModelRootNode(NULL, dGetIdentityMatrix())
	, m_winManager(winctx)
	, Sacrum(NULL)
	, Spine(NULL)
	, Hip_L(NULL)
	, Up_Leg_L(NULL)
	, Low_Leg_L(NULL)
	, Plantar_L(NULL)
	, Toe_L(NULL)
	, Clav_L(NULL)
	, Up_Arm_L(NULL)
	, Low_Arm_L(NULL)
	, Hand_L(NULL)
	, Clav_R(NULL)
	, Up_Arm_R(NULL)
	, Low_Arm_R(NULL)
	, Hand_R(NULL)
	, Disk1_L(NULL)
	, Rotule_L(NULL)
	, Knee_L(NULL)
	, Ankle_L(NULL)
	, Flextoe_L(NULL)
	, shld_L(NULL)
	, Elb_L(NULL)
	, Wr_L(NULL) 
	, shld_R(NULL)
	, Elb_R(NULL)
	, Wr_R(NULL)
	, Hip_LNode(NULL)
	, Up_Leg_LNode(NULL)
	, Low_Leg_LNode(NULL)
	, Plantar_LNode(NULL)
	, Toe_LNode(NULL)
	, Hip_R(NULL)
	, Up_Leg_R(NULL)
	, Low_Leg_R(NULL)
	, Plantar_R(NULL)
	, Toe_R(NULL)
	, Disk1_R(NULL)
	, Neck(NULL)
	, Head(NULL)
	, Rotule_R(NULL)
	, Knee_R(NULL)
	, Ankle_R(NULL)
	, Flextoe_R(NULL)
	, Hip_RNode(NULL)
	, Up_Leg_RNode(NULL)
	, Low_Leg_RNode(NULL)
	, Plantar_RNode(NULL)
	, Toe_RNode(NULL)
	, Lumbar_Node(NULL)
	, Clav_LNode(NULL)
	, Shoulder_LNode(NULL)
	, Elbow_LNode(NULL)
	, Wrist_LNode(NULL)
	, Clav_RNode(NULL)
	, Shoulder_RNode(NULL)
	, Elbow_RNode(NULL)
	, Neck_Node(NULL)
	, Head_Node(NULL)
	, Heel_L(NULL)
	, Pad1_L(NULL)
	, Pad2_L(NULL)
	, Heel_R(NULL)
	, Pad1_R(NULL)
	, Pad2_R(NULL)
	, Heel_L_Node(NULL)
	, Pad1_L_Node(NULL)
	, Pad2_L_Node(NULL)
	, Heel_R_Node(NULL)
	, Pad1_R_Node(NULL)
	, Pad2_R_Node(NULL)
	, Hl_L(NULL)
	, Pd1_L(NULL)
	, Pd2_L(NULL)
	, Hl_R(NULL)
	, Pd1_R(NULL)
	, Pd2_R(NULL)

{
	string tex("Textures//wood6.png");
	l_Hip = 0.25f;
	l_Sacrum = 0.4f;
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
	r_Pad = 0.05f;
	h_sphere = 0.03f;

	glm::vec3 _Pos(glm::vec3(3.0f, 3.0f, 0.f));
	Scale = 1.0f;
	float hip_angle = 15.0f;
	float clav_angle = 20.0f;


	Sacrum = new GeomNewton(m_winManager->aManager);
	Sacrum->SetBodyType(adtDynamic);
	Sacrum->SetTexture0(&tex[0], "Tex0");
	Sacrum->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Sacrum->SetRollAngle(90.0f ,false);
	Sacrum->SetPosition(_Pos.x, _Pos.y, _Pos.z);
	Sacrum->InitNewton(atCapsule, Scale*r_bones, Scale*r_bones, Scale*l_Sacrum, 10.0f*Scale);
	m_body = Sacrum->GetBody();
	NewtonBodySetTransformCallback(m_body, NULL);
	NewtonBody* const parentBody = GetBody();
	dMatrix aParenMatrix(dGetIdentityMatrix());
	NewtonBodyGetMatrix(parentBody, &aParenMatrix[0][0]);

	Spine = new GeomNewton(m_winManager->aManager);
	Spine->SetBodyType(adtDynamic);
	Spine->SetParent(Sacrum);
	Spine->SetTexture0(&tex[0], "Tex0");
	Spine->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Spine->SetPosition(0, Scale * ( l_Sacrum  + l_Spine) /2 + 2 * Scale * r_bones, 0);
	Spine->InitNewton(atCapsule, Scale * r_bones, Scale * r_bones, Scale * l_Spine, 10.0f * Scale);
	NewtonBodySetTransformCallback(Spine->GetBody(), NULL);
	Lumbar_Node = new dModelNode(Spine->GetBody(), dGetIdentityMatrix(), this);

	Neck = new GeomNewton(m_winManager->aManager);
	Neck->SetBodyType(adtDynamic);
	Neck->SetParent(Spine);
	Neck->SetTexture0(&tex[0], "Tex0");
	Neck->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Neck->SetPosition(0, Scale* (l_Neck + l_Spine) / 2 + 2*Scale* r_bones, 0);
	Neck->InitNewton(atCapsule, Scale* r_bones, Scale* r_bones, Scale* l_Neck, 10.0f * Scale);
	NewtonBodySetTransformCallback(Neck->GetBody(), NULL);
	Neck_Node = new dModelNode(Neck->GetBody(), dGetIdentityMatrix(), Lumbar_Node);

	Head = new GeomNewton(m_winManager->aManager);
	Head->SetBodyType(adtDynamic);
	Head->SetParent(Neck);
	Head->SetTexture0(&tex[0], "Tex0");
	Head->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Head->SetPosition(0, Scale* (l_Head + l_Neck) / 2 + 2*Scale * r_bones, 0);
	Head->InitNewton(atCapsule, Scale* r_bones, Scale* r_bones, Scale* l_Head, 10.0f * Scale);
	NewtonBodySetTransformCallback(Head->GetBody(), NULL);
	Head_Node = new dModelNode(Head->GetBody(), dGetIdentityMatrix(), Neck_Node);

	//Left lower limb 
	
	Hip_L = new GeomNewton(m_winManager->aManager);
	Hip_L->SetBodyType(adtDynamic);
	Hip_L->SetParent(Sacrum);
	Hip_L->SetTexture0(&tex[0], "Tex0");
	Hip_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Hip_L->SetRollAngle(90.0f+hip_angle, false);
	Hip_L->SetPosition(Scale*l_Hip*cos(hip_angle * dDegreeToRad)/2+ 2* Scale * r_bones,-Scale*l_Sacrum/ 2+ Scale* l_Hip*sin(hip_angle * dDegreeToRad)/2,0);
	Hip_L->InitNewton(atCapsule, Scale*r_bones, Scale*r_bones, Scale*l_Hip, 10.0f*Scale);
	NewtonBodySetTransformCallback(Hip_L->GetBody(), NULL);
	Hip_LNode = new dModelNode(Hip_L->GetBody(), dGetIdentityMatrix(), this);
	 
	Up_Leg_L = new GeomNewton(m_winManager->aManager);
	Up_Leg_L->SetBodyType(adtDynamic);
	Up_Leg_L->SetParent(Hip_L);
	Up_Leg_L->SetTexture0(&tex[0], "Tex0");
	Up_Leg_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Up_Leg_L->SetRollAngle(-90.0f - hip_angle, false);
	Up_Leg_L->SetPosition(Scale * l_Hip / 2*cos(hip_angle* dDegreeToRad) , sin(hip_angle * dDegreeToRad)* Scale * l_Hip / 2 -Scale * l_Up_Leg / 2, 0);
	Up_Leg_L->InitNewton(atCapsule, Scale*r_bones, Scale*r_bones, Scale*l_Up_Leg, 10.0f*Scale);
	NewtonBodySetTransformCallback(Up_Leg_L->GetBody(), NULL);
	Up_Leg_LNode = new dModelNode(Up_Leg_L->GetBody(), dGetIdentityMatrix(), Hip_LNode);

	Low_Leg_L = new GeomNewton(m_winManager->aManager);
	Low_Leg_L->SetBodyType(adtDynamic);
	Low_Leg_L->SetParent(Up_Leg_L);
	Low_Leg_L->SetTexture0(&tex[0], "Tex0");
	Low_Leg_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Low_Leg_L->SetPosition(0,-Scale * (l_Up_Leg + l_Low_Leg) / 2 ,  0);
	Low_Leg_L->InitNewton(atCapsule, Scale* r_bones, Scale*  r_bones, Scale* l_Low_Leg, 10.0f);
	NewtonBodySetTransformCallback(Low_Leg_L->GetBody(), NULL);
	Low_Leg_LNode = new dModelNode(Low_Leg_L->GetBody(), dGetIdentityMatrix(), Up_Leg_LNode);
	 
	Plantar_L = new GeomNewton(m_winManager->aManager);
	Plantar_L->SetBodyType(adtDynamic);
	Plantar_L->SetParent(Low_Leg_L);
	Plantar_L->SetTexture0(&tex[0], "Tex0");
	Plantar_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Plantar_L->SetPitchAngle(90.0f, false);
	Plantar_L->SetPosition (0,-Scale * ( l_Low_Leg) / 2 -2* Scale * r_bones, -Scale * (l_foot / 4));
	Plantar_L->InitNewton(atBox, Scale*  0.1f, Scale*  l_foot, Scale* w_foot, 10.f * Scale);
	NewtonBodySetTransformCallback(Plantar_L->GetBody(), NULL);
	Plantar_LNode = new dModelNode(Plantar_L->GetBody(), dGetIdentityMatrix(), Low_Leg_LNode);

	Toe_L = new GeomNewton(m_winManager->aManager);
	Toe_L->SetBodyType(adtDynamic);
	Toe_L->SetParent(Plantar_L);
	Toe_L->SetTexture0(&tex[0], "Tex0");
	Toe_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Toe_L->SetPosition( 0,  0,-Scale * ( l_foot / 2 + l_toe / 2));
	Toe_L->InitNewton(atBox, Scale* 0.1f, Scale*  0.2f, Scale*  0.2f, 1.0f*Scale);
	NewtonBodySetTransformCallback(Toe_L->GetBody(), NULL);
	Toe_LNode = new dModelNode(Toe_L->GetBody(), dGetIdentityMatrix(), Plantar_LNode);

	//Right lower limb

	Hip_R = new GeomNewton(m_winManager->aManager);
	Hip_R->SetBodyType(adtDynamic);
	Hip_R->SetParent(Sacrum);
	Hip_R->SetTexture0(&tex[0], "Tex0");
	Hip_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Hip_R->SetRollAngle(-90.0f - hip_angle, false);
	Hip_R->SetPosition(-Scale* l_Hip* cos(hip_angle* dDegreeToRad) / 2 - 2* Scale * r_bones, -Scale * l_Sacrum / 2 + Scale * l_Hip * sin(hip_angle * dDegreeToRad) / 2, 0);
	Hip_R->InitNewton(atCapsule, Scale* r_bones, Scale* r_bones, Scale* l_Hip, 10.0f * Scale);
	NewtonBodySetTransformCallback(Hip_R->GetBody(), NULL);
	Hip_RNode = new dModelNode(Hip_R->GetBody(), dGetIdentityMatrix(), this);

	Up_Leg_R = new GeomNewton(m_winManager->aManager);
	Up_Leg_R->SetBodyType(adtDynamic);
	Up_Leg_R->SetParent(Hip_R);
	Up_Leg_R->SetTexture0(&tex[0], "Tex0");
	Up_Leg_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Up_Leg_R->SetRollAngle(90.0f + hip_angle, false);
	Up_Leg_R->SetPosition(-Scale* l_Hip / 2 * cos(hip_angle * dDegreeToRad) , sin(hip_angle* dDegreeToRad)* Scale* l_Hip / 2 - Scale * l_Up_Leg / 2, 0);
	Up_Leg_R->InitNewton(atCapsule, Scale* r_bones, Scale* r_bones, Scale* l_Up_Leg, 10.0f * Scale);
	NewtonBodySetTransformCallback(Up_Leg_R->GetBody(), NULL);
	Up_Leg_RNode = new dModelNode(Up_Leg_R->GetBody(), dGetIdentityMatrix(), Hip_RNode);

	Low_Leg_R = new GeomNewton(m_winManager->aManager);
	Low_Leg_R->SetBodyType(adtDynamic);
	Low_Leg_R->SetParent(Up_Leg_R);
	Low_Leg_R->SetTexture0(&tex[0], "Tex0");
	Low_Leg_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Low_Leg_R->SetPosition(0, -Scale * (l_Up_Leg + l_Low_Leg) / 2 , 0);
	Low_Leg_R->InitNewton(atCapsule, Scale* r_bones, Scale* r_bones, Scale* l_Low_Leg, 10.0f);
	NewtonBodySetTransformCallback(Low_Leg_R->GetBody(), NULL);
	Low_Leg_RNode = new dModelNode(Low_Leg_R->GetBody(), dGetIdentityMatrix(), Up_Leg_RNode);

	Plantar_R = new GeomNewton(m_winManager->aManager);
	Plantar_R->SetBodyType(adtDynamic);
	Plantar_R->SetParent(Low_Leg_R);
	Plantar_R->SetTexture0(&tex[0], "Tex0");
	Plantar_R->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Plantar_R->SetPitchAngle(90.0f, false);
	Plantar_R->SetPosition(0, -Scale * (l_Low_Leg) / 2 -2* Scale * r_bones, -Scale * (l_foot / 4));
	Plantar_R->InitNewton(atBox, Scale * 0.1f, Scale* l_foot, Scale* w_foot, 10.0f * Scale);
	NewtonBodySetTransformCallback(Plantar_R->GetBody(), NULL);
	Plantar_RNode = new dModelNode(Plantar_R->GetBody(), dGetIdentityMatrix(), Low_Leg_RNode);

	Toe_R = new GeomNewton(m_winManager->aManager);
	Toe_R->SetBodyType(adtDynamic);
	Toe_R->SetParent(Plantar_R);
	Toe_R->SetTexture0(&tex[0], "Tex0");
	Toe_R->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Toe_R->SetPosition(0, 0, -Scale * (l_foot / 2 + l_toe / 2));
	Toe_R->InitNewton(atBox, Scale * 0.1f, Scale * 0.2f, Scale * 0.2f, 1.f * Scale);
	NewtonBodySetTransformCallback(Toe_R->GetBody(), NULL);
	Toe_RNode = new dModelNode(Toe_R->GetBody(), dGetIdentityMatrix(), Plantar_RNode); 

	// Left upper limb

	Clav_L = new GeomNewton(m_winManager->aManager);
	Clav_L->SetBodyType(adtDynamic);
	Clav_L->SetParent(Spine);
	Clav_L->SetTexture0(&tex[0], "Tex0");
	Clav_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Clav_L->SetRollAngle(90.0f + clav_angle, false);
	Clav_L->SetPosition(Scale* l_Clav* cos(clav_angle* dDegreeToRad) / 2 + 2 * Scale * r_bones, Scale * l_Spine / 2 + Scale * l_Clav * sin(clav_angle * dDegreeToRad) / 2, 0);
	Clav_L->InitNewton(atCapsule, Scale* r_bones, Scale* r_bones, Scale* l_Clav, 10.0f * Scale);
	NewtonBodySetTransformCallback(Clav_L->GetBody(), NULL);
	Clav_LNode = new dModelNode(Clav_L->GetBody(), dGetIdentityMatrix(), Lumbar_Node);

	Up_Arm_L = new GeomNewton(m_winManager->aManager);
	Up_Arm_L->SetBodyType(adtDynamic);
	Up_Arm_L->SetParent(Clav_L);
	Up_Arm_L->SetTexture0(&tex[0], "Tex0");
	Up_Arm_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Up_Arm_L->SetRollAngle(-90.0f  - clav_angle, false);
	Up_Arm_L->SetPosition(Scale * l_Clav * cos(clav_angle * dDegreeToRad) / 2, sin(clav_angle* dDegreeToRad)* Scale* l_Clav / 2 - Scale * l_Up_Arm / 2, 0);
	Up_Arm_L->InitNewton(atCapsule, Scale* r_bones, Scale* r_bones, Scale* l_Up_Arm, 10.0f * Scale);
	NewtonBodySetTransformCallback(Up_Arm_L->GetBody(), NULL);
	Shoulder_LNode = new dModelNode(Up_Arm_L->GetBody(), dGetIdentityMatrix(), Clav_LNode);

	Low_Arm_L = new GeomNewton(m_winManager->aManager);
	Low_Arm_L->SetBodyType(adtDynamic);
	Low_Arm_L->SetParent(Up_Arm_L);
	Low_Arm_L->SetTexture0(&tex[0], "Tex0");
	Low_Arm_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Low_Arm_L->SetPosition(0, -Scale * (l_Up_Arm + l_Low_Arm) / 2, 0);
	Low_Arm_L->InitNewton(atCapsule, Scale* r_bones, Scale* r_bones, Scale* l_Low_Arm, 10.0f);
	NewtonBodySetTransformCallback(Low_Arm_L->GetBody(), NULL);
	Elbow_LNode = new dModelNode(Low_Arm_L->GetBody(), dGetIdentityMatrix(), Shoulder_LNode);

	Hand_L = new GeomNewton(m_winManager->aManager);
	Hand_L->SetBodyType(adtDynamic);
	Hand_L->SetParent(Low_Arm_L);
	Hand_L->SetTexture0(&tex[0], "Tex0");
	Hand_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Hand_L->SetPitchAngle(90.0f, false);
	Hand_L->SetPosition(0, -Scale * (l_Low_Arm+ l_Hand) / 2,0);
	Hand_L->InitNewton(atCapsule, Scale* r_bones, Scale* r_bones, Scale* l_Hand, 10.0f * Scale);
	NewtonBodySetTransformCallback(Hand_L->GetBody(), NULL);
	Wrist_LNode = new dModelNode(Hand_L->GetBody(), dGetIdentityMatrix(), Elbow_LNode);

	// Right upper limb

	Clav_R = new GeomNewton(m_winManager->aManager);
	Clav_R->SetBodyType(adtDynamic);
	Clav_R->SetParent(Spine);
	Clav_R->SetTexture0(&tex[0], "Tex0");
	Clav_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Clav_R->SetRollAngle(90.0f - clav_angle, false);
	Clav_R->SetPosition(-Scale* l_Clav* cos(clav_angle* dDegreeToRad) / 2 - 2 * Scale * r_bones, Scale* l_Spine / 2 + Scale * l_Clav * sin(clav_angle * dDegreeToRad) / 2, 0);
	Clav_R->InitNewton(atCapsule, Scale* r_bones, Scale* r_bones, Scale* l_Clav, 10.0f * Scale);
	NewtonBodySetTransformCallback(Clav_R->GetBody(), NULL);
	Clav_RNode = new dModelNode(Clav_R->GetBody(), dGetIdentityMatrix(), Lumbar_Node);

	Up_Arm_R = new GeomNewton(m_winManager->aManager);
	Up_Arm_R->SetBodyType(adtDynamic);
	Up_Arm_R->SetParent(Clav_R);
	Up_Arm_R->SetTexture0(&tex[0], "Tex0");
	Up_Arm_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Up_Arm_R->SetRollAngle(-90.0f + clav_angle, false);
	Up_Arm_R->SetPosition(-Scale* l_Clav* cos(clav_angle* dDegreeToRad) / 2, sin(clav_angle* dDegreeToRad)* Scale* l_Clav / 2 - Scale * l_Up_Arm / 2, 0);
	Up_Arm_R->InitNewton(atCapsule, Scale* r_bones, Scale* r_bones, Scale* l_Up_Arm, 10.0f * Scale);
	NewtonBodySetTransformCallback(Up_Arm_R->GetBody(), NULL);
	Shoulder_RNode = new dModelNode(Up_Arm_R->GetBody(), dGetIdentityMatrix(), Clav_RNode);

	Low_Arm_R = new GeomNewton(m_winManager->aManager);
	Low_Arm_R->SetBodyType(adtDynamic);
	Low_Arm_R->SetParent(Up_Arm_R);
	Low_Arm_R->SetTexture0(&tex[0], "Tex0");
	Low_Arm_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Low_Arm_R->SetPosition(0, -Scale * (l_Up_Arm + l_Low_Arm) / 2, 0);
	Low_Arm_R->InitNewton(atCapsule, Scale* r_bones, Scale* r_bones, Scale* l_Low_Arm, 10.0f);
	NewtonBodySetTransformCallback(Low_Arm_R->GetBody(), NULL);
	Elbow_RNode = new dModelNode(Low_Arm_R->GetBody(), dGetIdentityMatrix(), Shoulder_RNode);

	Hand_R = new GeomNewton(m_winManager->aManager);
	Hand_R->SetBodyType(adtDynamic);
	Hand_R->SetParent(Low_Arm_R);
	Hand_R->SetTexture0(&tex[0], "Tex0");
	Hand_R->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Hand_R->SetPitchAngle(90.0f, false);
	Hand_R->SetPosition(0, -Scale * (l_Low_Arm + l_Hand) / 2, 0);
	Hand_R->InitNewton(atCapsule, Scale* r_bones, Scale* r_bones, Scale* l_Hand, 10.0f * Scale);
	NewtonBodySetTransformCallback(Hand_R->GetBody(), NULL);
	Wrist_RNode = new dModelNode(Hand_R->GetBody(), dGetIdentityMatrix(), Elbow_RNode);
	
    
	// Foot pads

	Heel_L = new GeomNewton(m_winManager->aManager);
	Heel_L->SetBodyType(adtDynamic);
	Heel_L->SetParent(Plantar_L);
	Heel_L->SetTexture0(&tex[0], "Tex0");
	Heel_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Heel_L->SetPitchAngle(90.0f, false);
	Heel_L->SetPosition(0, -Scale * h_sphere, Scale * (l_foot / 2));
	Heel_L->InitNewton(atSphere, r_Pad, r_Pad, r_Pad, 10.0f * Scale);
	NewtonBodySetTransformCallback(Heel_L->GetBody(), NULL);
	Heel_L_Node = new dModelNode(Heel_L->GetBody(), dGetIdentityMatrix(), Plantar_LNode);

	Pad1_L = new GeomNewton(m_winManager->aManager);
	Pad1_L->SetBodyType(adtDynamic);
	Pad1_L->SetParent(Toe_L);
	Pad1_L->SetTexture0(&tex[0], "Tex0");
	Pad1_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Pad1_L->SetPitchAngle(90.0f, false);
	Pad1_L->SetPosition( Scale* w_foot / 2, -Scale * h_sphere,-Scale * (l_foot / 4));
	Pad1_L->InitNewton(atSphere, r_Pad, r_Pad, r_Pad, 10.0f * Scale);
	NewtonBodySetTransformCallback(Pad1_L->GetBody(), NULL);
	Pad1_L_Node = new dModelNode(Pad1_L->GetBody(), dGetIdentityMatrix(), Toe_LNode);

	Pad2_L = new GeomNewton(m_winManager->aManager);
	Pad2_L->SetBodyType(adtDynamic);
	Pad2_L->SetParent(Toe_L);
	Pad2_L->SetTexture0(&tex[0], "Tex0");
	Pad2_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Pad2_L->SetPitchAngle(90.0f, false);
	Pad2_L->SetPosition( -Scale* w_foot / 2, -Scale * h_sphere, -Scale * (l_foot / 4));
	Pad2_L->InitNewton(atSphere, r_Pad, r_Pad, r_Pad, 10.0f * Scale);
	NewtonBodySetTransformCallback(Pad2_L->GetBody(), NULL);
	Pad2_L_Node = new dModelNode(Pad2_L->GetBody(), dGetIdentityMatrix(), Toe_LNode);

	Heel_R = new GeomNewton(m_winManager->aManager);
	Heel_R->SetBodyType(adtDynamic);
	Heel_R->SetParent(Plantar_R);
	Heel_R->SetTexture0(&tex[0], "Tex0");
	Heel_R->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Heel_R->SetPitchAngle(90.0f, false);
	Heel_R->SetPosition(0, -Scale * h_sphere, Scale* (l_foot / 2));
	Heel_R->InitNewton(atSphere, r_Pad, r_Pad, r_Pad, 10.0f * Scale);
	NewtonBodySetTransformCallback(Heel_R->GetBody(), NULL);
	Heel_R_Node = new dModelNode(Heel_R->GetBody(), dGetIdentityMatrix(), Plantar_RNode);

	Pad1_R = new GeomNewton(m_winManager->aManager);
	Pad1_R->SetBodyType(adtDynamic);
	Pad1_R->SetParent(Toe_R);
	Pad1_R->SetTexture0(&tex[0], "Tex0");
	Pad1_R->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Pad1_R->SetPitchAngle(90.0f, false);
	Pad1_R->SetPosition(Scale* w_foot / 2, -Scale * h_sphere, -Scale * (l_foot / 4));
	Pad1_R->InitNewton(atSphere, r_Pad, r_Pad, r_Pad, 10.0f * Scale);
	NewtonBodySetTransformCallback(Pad1_R->GetBody(), NULL);
	Pad1_R_Node = new dModelNode(Pad1_R->GetBody(), dGetIdentityMatrix(), Toe_RNode);

	Pad2_R = new GeomNewton(m_winManager->aManager);
	Pad2_R->SetBodyType(adtDynamic);
	Pad2_R->SetParent(Toe_R);
	Pad2_R->SetTexture0(&tex[0], "Tex0");
	Pad2_R->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Pad2_R->SetPitchAngle(90.0f, false);
	Pad2_R->SetPosition(-Scale * w_foot / 2, -Scale * h_sphere, -Scale * (l_foot / 4));
	Pad2_R->InitNewton(atSphere, r_Pad, r_Pad, r_Pad, 10.0f * Scale);
	NewtonBodySetTransformCallback(Pad2_R->GetBody(), NULL);
	Pad2_R_Node = new dModelNode(Pad2_R->GetBody(), dGetIdentityMatrix(), Toe_RNode);
	

	//Pad joints

	//Left foot
	dMatrix Heel_L_PinMatrix(dGetIdentityMatrix());
	Heel_L_PinMatrix.m_posit = dVector(0, -Scale * h_sphere, Scale * (l_foot / 2),1.0f);
	Hl_L = new dCustomDoubleHinge(Heel_L_PinMatrix, Heel_L->GetBody(), Plantar_L->GetBody());
	Hl_L->SetMassIndependentSpringDamper(true,0.01, 1.e20f, 1.e15f);
	Hl_L->SetMassIndependentSpringDamper1(true,0.01, 1.e20f, 1.e15f);
	m_winManager->aManager->vJointList.push_back(Hl_L);

	dMatrix Pad1_L_PinMatrix(dGetIdentityMatrix());
	Pad1_L_PinMatrix.m_posit = dVector(Scale * w_foot / 2, -Scale * h_sphere, -Scale * (l_foot / 4) ,1.0f);
	Pd1_L = new dCustomDoubleHinge(Pad1_L_PinMatrix, Pad1_L->GetBody(), Toe_L->GetBody());
	Pd1_L->SetMassIndependentSpringDamper(true, 0.01, 1.e20f, 1.e15f);
	Pd1_L->SetMassIndependentSpringDamper1(true, 0.01, 1.e20f, 1.e15f);
	m_winManager->aManager->vJointList.push_back(Pd1_L);

	dMatrix Pad2_L_PinMatrix(dGetIdentityMatrix());
	Pad2_L_PinMatrix.m_posit = dVector(-Scale * w_foot / 2, -Scale * h_sphere, -Scale * (l_foot / 4),1.0f);
	Pd2_L = new dCustomDoubleHinge(Pad2_L_PinMatrix, Pad2_L->GetBody(), Toe_L->GetBody());
	Pd2_L->SetMassIndependentSpringDamper(true, 0.01, 1.e20f, 1.e15f);
	Pd2_L->SetMassIndependentSpringDamper1(true, 0.01, 1.e20f, 1.e15f);
	m_winManager->aManager->vJointList.push_back(Pd2_L);

	//Right foot
	dMatrix Heel_R_PinMatrix(dGetIdentityMatrix());
	Heel_R_PinMatrix.m_posit = dVector(0, -Scale * h_sphere, Scale * (l_foot / 2),1.0f);
	Hl_R = new dCustomDoubleHinge(Heel_R_PinMatrix, Heel_R->GetBody(), Plantar_R->GetBody());
	Hl_R->SetMassIndependentSpringDamper(true, 0.01, 1.e20f, 1.e15f);
	Hl_R->SetMassIndependentSpringDamper1(true, 0.01, 1.e20f, 1.e15f);
	m_winManager->aManager->vJointList.push_back(Hl_R);

	dMatrix Pad1_R_PinMatrix(dGetIdentityMatrix());
	Pad1_R_PinMatrix.m_posit = dVector(Scale * w_foot / 2, -Scale * h_sphere, -Scale * (l_foot / 4),1.0f);
	Pd1_R = new dCustomDoubleHinge(Pad1_R_PinMatrix, Pad1_R->GetBody(), Toe_R->GetBody());
	Pd1_R->SetMassIndependentSpringDamper(true, 0.01, 1.e20f, 1.e15f);
	Pd1_R->SetMassIndependentSpringDamper1(true, 0.01, 1.e20f, 1.e15f);
	m_winManager->aManager->vJointList.push_back(Pd1_R);

	dMatrix Pad2_R_PinMatrix(dGetIdentityMatrix());
	Pad2_R_PinMatrix.m_posit = dVector(-Scale * w_foot / 2, -Scale * h_sphere, -Scale * (l_foot / 4),1.0f);
	Pd2_R = new dCustomDoubleHinge(Pad2_R_PinMatrix, Pad2_R->GetBody(), Toe_R->GetBody());
	Pd2_R->SetMassIndependentSpringDamper(true, 0.01, 1.e20f, 1.e15f);
	Pd2_R->SetMassIndependentSpringDamper1(true, 0.01, 1.e20f, 1.e15f);
	m_winManager->aManager->vJointList.push_back(Pd2_R); 
	
	//trunk joints

	// create Lumbar joint. 
	dMatrix Spine_PinMatrix(dYawMatrix(90.0f * dDegreeToRad));
	Spine_PinMatrix.m_posit = dVector(_Pos.x , _Pos.y + Scale * l_Sacrum  / 2 + Scale * r_bones, _Pos.z,1.0f);
	Disk2 = new dCustomDoubleHinge(Spine_PinMatrix, Spine->GetBody(), Sacrum->GetBody());
	Disk2->SetMassIndependentSpringDamper(true,0.3,  1.e6f, 1.e4f);
	Disk2->SetMassIndependentSpringDamper1(true,0.3,  1.e6f, 1.e4f);
	m_winManager->aManager->vJointList.push_back(Disk2);

	// create Neck joint. 
	//dMatrix Neck_PinMatrix(dYawMatrix(90.0f * dDegreeToRad));
	dMatrix Neck_PinMatrix(dGetIdentityMatrix());
	Neck_PinMatrix.m_posit = dVector(Neck->GetPosition().m_x, Neck->GetPosition().m_y - Scale * l_Neck / 2 -  Scale * r_bones, Neck->GetPosition().m_z,1.0f);
	Nck = new dCustomDoubleHinge(Neck_PinMatrix, Neck->GetBody(), Spine->GetBody());
	Nck->SetMassIndependentSpringDamper(true, 0.01, 1000.f, 10.f);
	Nck->SetMassIndependentSpringDamper1(true, 0.01, 1000.f, 10.f);
	m_winManager->aManager->vJointList.push_back(Nck);

	// create Head joint. 
	dMatrix Head_PinMatrix(dYawMatrix(90.0f * dDegreeToRad));
	Head_PinMatrix.m_posit = dVector(Head->GetPosition().m_x, Head->GetPosition().m_y - Scale * l_Head / 2 -  Scale * r_bones, Head->GetPosition().m_z,1.0f);
	Hd = new dCustomDoubleHinge(Head_PinMatrix, Head->GetBody(), Neck->GetBody());
	Hd->SetMassIndependentSpringDamper(true, 0.01, 1000.f, 10.f);
	Hd->SetMassIndependentSpringDamper1(true, 0.01, 1000.f, 10.f);
	m_winManager->aManager->vJointList.push_back(Hd);

	// Left upper limb joints

	// create sternum joint.

	dMatrix Sternum_PinMatrix(dYawMatrix(90.0f * dDegreeToRad));

	Sternum_PinMatrix.m_posit = dVector(Spine->GetPosition().m_x + Scale * r_bones, Spine->GetPosition().m_y+ Scale * l_Spine / 2, Spine->GetPosition().m_z,1.0f);
	Strn_L = new dCustomDoubleHinge(Sternum_PinMatrix, Clav_L->GetBody(), Spine->GetBody());
	Strn_L->SetMassIndependentSpringDamper(true, 0.01, 1.e6f, 1.e4f);
	Strn_L->SetMassIndependentSpringDamper1(true, 0.01, 1.e6f, 1.e4f);
	m_winManager->aManager->vJointList.push_back(Strn_L);

	// create Shoulder joint. 
	dMatrix Shoulder_LPinMatrix(dGetIdentityMatrix());
	Shoulder_LPinMatrix = Shoulder_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Shoulder_LPinMatrix.m_posit = dVector(Up_Arm_L->GetPosition().m_x, Up_Arm_L->GetPosition().m_y + Scale * l_Up_Arm / 2, Up_Arm_L->GetPosition().m_z,1.0f);
	shld_L = new dCustomBallAndSocket(Shoulder_LPinMatrix,Up_Arm_L->GetBody(), Clav_L->GetBody() );
	m_winManager->aManager->vJointList.push_back(shld_L);

	// create elbow joint. 
	dMatrix Elbow_LPinMatrix(dGetIdentityMatrix());
	Elbow_LPinMatrix = Elbow_LPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Elbow_LPinMatrix.m_posit = dVector(Low_Arm_L->GetPosition().m_x, Low_Arm_L->GetPosition().m_y + Scale * l_Low_Arm / 2, Low_Arm_L->GetPosition().m_z,1.0f);
	Elb_L = new dCustomHinge(Elbow_LPinMatrix, Low_Arm_L->GetBody(), Up_Arm_L->GetBody());
	Elb_L->EnableLimits(true);
	Elb_L->SetLimits(0.f * dDegreeToRad, 180.f);	
	m_winManager->aManager->vJointList.push_back(Elb_L);

	// create wrist joint.
	dMatrix Wrist_LPinMatrix(dGetIdentityMatrix());
	Wrist_LPinMatrix = Wrist_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Wrist_LPinMatrix.m_posit = dVector(Hand_L->GetPosition().m_x, Hand_L->GetPosition().m_y + Scale * l_Hand / 2, Hand_L->GetPosition().m_z,1.0f);
	Wr_L = new dCustomBallAndSocket(Wrist_LPinMatrix, Hand_L->GetBody(), Low_Arm_L->GetBody());
	m_winManager->aManager->vJointList.push_back(Wr_L);

	// Right upper limb joints

	// create sternum joint. 

	Sternum_PinMatrix.m_posit = dVector(Spine->GetPosition().m_x - Scale * r_bones, Spine->GetPosition().m_y + Scale * l_Spine / 2, Spine->GetPosition().m_z,1.0f);
	Strn_R = new dCustomDoubleHinge(Sternum_PinMatrix, Clav_R->GetBody(), Spine->GetBody());
	Strn_R->SetMassIndependentSpringDamper(true, 0.01, 1.e6f, 1.e4f);
	Strn_R->SetMassIndependentSpringDamper1(true, 0.01, 1.e6f, 1.e4f);
	m_winManager->aManager->vJointList.push_back(Strn_R);


	// create Shoulder joint. 
	dMatrix Shoulder_RPinMatrix(dGetIdentityMatrix());
	Shoulder_RPinMatrix = Shoulder_RPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Shoulder_RPinMatrix.m_posit = dVector(Up_Arm_R->GetPosition().m_x, Up_Arm_R->GetPosition().m_y + Scale * l_Up_Arm / 2, Up_Arm_R->GetPosition().m_z,1.0f);
	shld_R = new dCustomBallAndSocket(Shoulder_RPinMatrix, Up_Arm_R->GetBody(),Clav_R->GetBody());
	m_winManager->aManager->vJointList.push_back(shld_R);

	// create elbow joint. 
	dMatrix Elbow_RPinMatrix(dGetIdentityMatrix());
	Elbow_RPinMatrix = Elbow_RPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Elbow_RPinMatrix.m_posit = dVector(Low_Arm_R->GetPosition().m_x, Low_Arm_R->GetPosition().m_y + Scale * l_Low_Arm / 2, Low_Arm_R->GetPosition().m_z,1.0f);
	Elb_R = new dCustomHinge(Elbow_RPinMatrix, Low_Arm_R->GetBody(), Up_Arm_R->GetBody());
	Elb_R->EnableLimits(true);
	Elb_R->SetLimits(0.f * dDegreeToRad, 180.f);
	m_winManager->aManager->vJointList.push_back(Elb_R);

	// create wrist joint.
	dMatrix Wrist_RPinMatrix(dGetIdentityMatrix());
	Wrist_RPinMatrix = Wrist_RPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Wrist_RPinMatrix.m_posit = dVector(Hand_R->GetPosition().m_x, Hand_R->GetPosition().m_y + Scale * l_Hand / 2, Hand_R->GetPosition().m_z,1.0f);
	Wr_R = new dCustomBallAndSocket(Wrist_RPinMatrix, Hand_R->GetBody(), Low_Arm_R->GetBody());
	m_winManager->aManager->vJointList.push_back(Wr_R);



	//Left lower limb joints

	// create Sacrum joint. 
	dMatrix Sacrum_PinMatrix(dYawMatrix(90.0f * dDegreeToRad));
	Sacrum_PinMatrix.m_posit = dVector(_Pos.x + Scale * r_bones, _Pos.y - Scale * l_Sacrum / 2, _Pos.z,1.0f);
	Disk1_L = new dCustomDoubleHinge(Sacrum_PinMatrix, Hip_L->GetBody(), Sacrum->GetBody());
	Disk1_L->SetMassIndependentSpringDamper(true,0.01,  1.e6f, 1.e4f);
	Disk1_L->SetMassIndependentSpringDamper1(true, 0.01, 1.e6f, 1.e4f);
	m_winManager->aManager->vJointList.push_back(Disk1_L);

	// create hip joint. 
	dMatrix Hip_LPinMatrix(dGetIdentityMatrix());
	Hip_LPinMatrix = Hip_LPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Hip_LPinMatrix.m_posit = dVector(Up_Leg_L->GetPosition().m_x , Up_Leg_L->GetPosition().m_y + Scale * l_Up_Leg / 2, Up_Leg_L->GetPosition().m_z,1.0f);
	Rotule_L = new dCustomDoubleHinge(Hip_LPinMatrix, Up_Leg_L->GetBody(), Hip_L->GetBody());
	Rotule_L->EnableLimits(true);
	Rotule_L->EnableLimits1(true);
	Rotule_L->SetLimits(-40.f * dDegreeToRad,120.f * dDegreeToRad);
	Rotule_L->SetLimits1(-5.f * dDegreeToRad, 30.f * dDegreeToRad);
	m_winManager->aManager->vJointList.push_back(Rotule_L);

	// create knee joint. 
	dMatrix Knee_LPinMatrix(dGetIdentityMatrix());
	Knee_LPinMatrix = Knee_LPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Knee_LPinMatrix.m_posit = dVector(Low_Leg_L->GetPosition().m_x, Low_Leg_L->GetPosition().m_y + Scale * l_Low_Leg / 2 , Low_Leg_L->GetPosition().m_z,1.0f);
	Knee_L = new dCustomHinge(Knee_LPinMatrix, Low_Leg_L->GetBody(), Up_Leg_L->GetBody());
	Knee_L->EnableLimits(true);
	Knee_L->SetLimits(-180.f * dDegreeToRad,0.f);
	m_winManager->aManager->vJointList.push_back(Knee_L);

	// create ankle joint.
	dMatrix Ankle_LPinMatrix(dGetIdentityMatrix()); //flexion twist
	//Ankle_LPinMatrix = Ankle_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);//supination twist
	Ankle_LPinMatrix = Ankle_LPinMatrix * dRollMatrix(90.0f * dDegreeToRad);//torsion twist
	Ankle_LPinMatrix.m_posit = dVector(Low_Leg_L->GetPosition().m_x, Low_Leg_L->GetPosition().m_y - Scale * l_Low_Leg / 2- Scale * r_bones, Low_Leg_L->GetPosition().m_z,1.0f);
	Ankle_L = new dCustomBallAndSocket(Ankle_LPinMatrix,  Plantar_L->GetBody(),Low_Leg_L->GetBody());
	Ankle_L->EnableTwist(true);
	Ankle_L->SetTwistLimits(-15.f * dDegreeToRad,15.f * dDegreeToRad);
	Ankle_L->EnableCone(true);
	Ankle_L->SetConeLimits(40.f * dDegreeToRad );
	m_winManager->aManager->vJointList.push_back(Ankle_L);

	// create toe joint.
	dMatrix Toe_LPinMatrix(dGetIdentityMatrix());
	Toe_LPinMatrix = Toe_LPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Toe_LPinMatrix.m_posit = dVector(Plantar_L->GetPosition().m_x, Plantar_L->GetPosition().m_y, Plantar_L->GetPosition().m_z- Scale * l_foot/2,1.0f);
	Flextoe_L = new dCustomHinge(Toe_LPinMatrix, Plantar_L->GetBody(), Toe_L->GetBody());
	//Flextoe_L->SetMassIndependentSpringDamper(true, 0.01, 1.e5f, 1.e4f);
	Flextoe_L->SetAsSpringDamper(true,  1.e3f, 1.e2f);
	m_winManager->aManager->vJointList.push_back(Flextoe_L);

	//Right lower limb joints
	
	// create Sacrum joint. 
	Sacrum_PinMatrix.m_posit = dVector(_Pos.x - Scale * r_bones, _Pos.y - Scale * l_Sacrum / 2, _Pos.z,1.0f);
	Disk1_R = new dCustomDoubleHinge(Sacrum_PinMatrix, Hip_R->GetBody(), Sacrum->GetBody());
	Disk1_R->SetMassIndependentSpringDamper(true, 0.01, 1.e6f, 1.e4f);
	Disk1_R->SetMassIndependentSpringDamper1(true, 0.01, 1.e6f, 1.e4f);
	m_winManager->aManager->vJointList.push_back(Disk1_L);

	// create hip joint. 
	dMatrix Hip_RPinMatrix(dGetIdentityMatrix());
	Hip_RPinMatrix = Hip_RPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Hip_RPinMatrix.m_posit = dVector(Up_Leg_R->GetPosition().m_x , Up_Leg_R->GetPosition().m_y + Scale * l_Up_Leg / 2, Up_Leg_R->GetPosition().m_z,1.0f);
	Rotule_R = new dCustomDoubleHinge(Hip_RPinMatrix, Up_Leg_R->GetBody(), Hip_R->GetBody());
	Rotule_R->EnableLimits(true);
	Rotule_R->EnableLimits1(true);
	Rotule_R->SetLimits(-40.f * dDegreeToRad, 120.f * dDegreeToRad);
	Rotule_R->SetLimits1(-30.f * dDegreeToRad, 5.0f * dDegreeToRad);
	m_winManager->aManager->vJointList.push_back(Rotule_R);

	// create knee joint. 
	dMatrix Knee_RPinMatrix(dGetIdentityMatrix());
	Knee_RPinMatrix = Knee_RPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Knee_RPinMatrix.m_posit = dVector(Low_Leg_R->GetPosition().m_x, Low_Leg_R->GetPosition().m_y + Scale * l_Low_Leg / 2 , Low_Leg_R->GetPosition().m_z,1.0f);
	Knee_R = new dCustomHinge(Knee_RPinMatrix, Low_Leg_R->GetBody(), Up_Leg_R->GetBody());
	Knee_R->EnableLimits(true);
	Knee_R->SetLimits(-180.f * dDegreeToRad, 0);
	m_winManager->aManager->vJointList.push_back(Knee_R);

	// create ankle joint.
	dMatrix Ankle_RPinMatrix(dGetIdentityMatrix());
	Ankle_RPinMatrix = Ankle_RPinMatrix * dRollMatrix(90.0f * dDegreeToRad);
	Ankle_RPinMatrix.m_posit = dVector(Low_Leg_R->GetPosition().m_x, Low_Leg_R->GetPosition().m_y - Scale * l_Low_Leg / 2- Scale * r_bones, Low_Leg_R->GetPosition().m_z,1.0f);
	Ankle_R = new dCustomBallAndSocket(Ankle_RPinMatrix, Plantar_R->GetBody(), Low_Leg_R->GetBody());
	Ankle_R->EnableTwist(true);
	Ankle_R->SetTwistLimits(-15.f * dDegreeToRad, 15.f * dDegreeToRad);
	Ankle_R->EnableCone(true);
	Ankle_R->SetConeLimits(40.f * dDegreeToRad);
	m_winManager->aManager->vJointList.push_back(Ankle_R);

	// create toe joint.
	dMatrix Toe_RPinMatrix(dGetIdentityMatrix());
	Toe_RPinMatrix = Toe_RPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Toe_RPinMatrix.m_posit = dVector(Plantar_R->GetPosition().m_x, Plantar_R->GetPosition().m_y, Plantar_R->GetPosition().m_z - Scale * l_foot / 2,1.0f);
	Flextoe_R = new dCustomHinge(Toe_RPinMatrix, Plantar_R->GetBody(), Toe_R->GetBody());
	//Flextoe_R->SetMassIndependentSpringDamper(true, 0.01, 1.e5f, 1.e4f);
	Flextoe_R->SetAsSpringDamper(true,  1.e3f, 1.e2f);
	m_winManager->aManager->vJointList.push_back(Flextoe_R);
	
	ins11 = dVector(0.f, 0.f, 0.f);
	ins12 = dVector(0.f, 0.f, 0.f);
/*	m1 = new Muscle(m_winManager->aLineManager, m_winManager->aManager, GetUp_Leg_L(), GetLow_Leg_L(), ins11, ins12);
	m1->GenerateMesh();*/

	FootLineIndex_L = CreateFootScanLine();
	FootLineIndex_R = CreateFootScanLine();
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

float dRaycastVHModel::GetFoot2Floor_R() {
	return Foot2Floor_R;
}

int dRaycastVHModel::CreateFootScanLine() {
glm::vec3 linepos1;
glm::vec3 linepos2;
glm::vec3 linecolor;

linepos1.x = 0; linepos1.y = 0; linepos1.z = 0;
linepos2.x = 2.0f; linepos2.y = 2.0f; linepos2.z = 2.0f;
linecolor.x = 0.0f; linecolor.y = 1.0f; linecolor.z = 0.f;

return m_winManager->aLineManager->AddLine(linepos1, linepos2, linecolor);
}



void dRaycastVHModel::CastFoot(const char* const Laterality) {
	GeomNewton* GNewton;
	int FLindex;
	if (Laterality == "L")
	{
		GNewton = Plantar_L; FLindex = FootLineIndex_L;
	}
	else 
		{
			GNewton = Plantar_R;  FLindex = FootLineIndex_R;
		}

	NewtonCollision* const collisionA = NewtonBodyGetCollision(GNewton->GetBody());
	dMatrix matrixA;
	NewtonBodyGetMatrix(GNewton->GetBody(), &matrixA[0][0]);

	NewtonCollision* const collisionB = NewtonCreateBox(m_winManager->aManager->GetWorld(), 25.0f, 0.5f, 25.0f, SERIALIZE_ID_BOX, NULL);
	dMatrix matrixB(dGetIdentityMatrix());

	int res = NewtonCollisionClosestPoint(m_winManager->aManager->GetWorld(), collisionA, &matrixA[0][0], collisionB, &matrixB[0][0], &ContactFoot[0], &ContactGround[0], &NormalFoot[0], 0);
	NewtonDestroyCollision(collisionB);
	if (Laterality == "L") { Foot2Floor_L = abs(ContactFoot[1]); } else { Foot2Floor_R = abs(ContactFoot[1]); }

	m_winManager->aLineManager->aLineBuffer[FLindex - 1].posit.x = ContactFoot[0];
	m_winManager->aLineManager->aLineBuffer[FLindex - 1].posit.y = ContactFoot[1];
	m_winManager->aLineManager->aLineBuffer[FLindex - 1].posit.z = ContactFoot[2];
	//aLineBuffer[LineIndex].color = newlinecolor;
	m_winManager->aLineManager->aLineBuffer[FLindex - 2].posit.x = this->ContactGround[0];
	m_winManager->aLineManager->aLineBuffer[FLindex - 2].posit.y = this->ContactGround[1];
	m_winManager->aLineManager->aLineBuffer[FLindex - 2].posit.z = this->ContactGround[2];
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
	printf("DGVehicleRCManager OnPreUpdate \n");


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


}
}

void DGVehicleRCManager::OnPostUpdate(dModelRootNode* const model, dFloat timestep, int threadID) const
//pour VS<VS2017 supprimer ,int threadID

{
	printf("DGVehicleRCManager OnPostUpdate \n");
	dRaycastVHModel* controller = (dRaycastVHModel*)model;
	controller->CastFoot("L");
	controller->CastFoot("R");
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