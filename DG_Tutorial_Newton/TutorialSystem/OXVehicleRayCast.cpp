#include "pch.h"
#include "OXVehicleRayCast.h"
#include <tinyxml.h>

ofstream monFlux("history.txt");

dFloat newTime = 0;

// ----------------------------------------------------------------------
// STDOUT dump and indenting utility functions
// ----------------------------------------------------------------------
const unsigned int NUM_INDENTS_PER_SPACE = 2;

const char* getIndent(unsigned int numIndents)
{
	static const char* pINDENT = "                                      + ";
	static const unsigned int LENGTH = strlen(pINDENT);
	unsigned int n = numIndents * NUM_INDENTS_PER_SPACE;
	if (n > LENGTH) n = LENGTH;

	return &pINDENT[LENGTH - n];
}

// same as getIndent but no "+" at the end
const char* getIndentAlt(unsigned int numIndents)
{
	static const char* pINDENT = "                                        ";
	static const unsigned int LENGTH = strlen(pINDENT);
	unsigned int n = numIndents * NUM_INDENTS_PER_SPACE;
	if (n > LENGTH) n = LENGTH;

	return &pINDENT[LENGTH - n];
}

int dRaycastVHModel::dump_attribs_to_stdout(TiXmlElement* pElement, std::vector<float> &vector, unsigned int indent)
{
	if (!pElement) return 0;

	TiXmlAttribute* pAttrib = pElement->FirstAttribute();
	int i = 0;
	int ival;
	double dval;
	const char* pIndent = getIndent(indent);
	printf("\n");
	while (pAttrib)
	{
		if (pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS) printf("%s%s: value=[%1.4f]", pIndent, pAttrib->Name(), dval);
		
	
		vector.push_back(dval);
		printf("\n");
		i++;
		pAttrib = pAttrib->Next();
	}
	return i;
}

void dRaycastVHModel::dump_to_stdout(TiXmlNode* pParent, unsigned int indent)
{
	if (!pParent) return;

	TiXmlNode* pChild;
	TiXmlText* pText;
	int t = pParent->Type();
	printf("%s", getIndent(indent));
	int num=0;

	switch (t)
	{
	case TiXmlNode::DOCUMENT:
		printf("Document");
		break;

	case TiXmlNode::ELEMENT:
	{
		printf("Element [%s]", pParent->Value());
		TIXML_STRING sParameter = pParent->ValueTStr();

		if (sParameter == "Scale")
		{
			num = dump_attribs_to_stdout(pParent->ToElement(), v_scale, indent + 1);
		}
		else if (sParameter == "Lengths")
		{
			num = dump_attribs_to_stdout(pParent->ToElement(), v_lengths, indent + 1);
		}
		else if (sParameter == "Weight")
		{
			num = dump_attribs_to_stdout(pParent->ToElement(), v_weight, indent + 1);
		}
		else if (sParameter == "Masses")
		{
			num = dump_attribs_to_stdout(pParent->ToElement(), v_masses, indent + 1);
		}
		else if (sParameter == "Ixx")
		{
			num = dump_attribs_to_stdout(pParent->ToElement(), v_ixx, indent + 1);
		}
		else if (sParameter == "Iyy")
		{
			num = dump_attribs_to_stdout(pParent->ToElement(), v_iyy, indent + 1);
		}
		else if (sParameter == "Izz")
		{
			num = dump_attribs_to_stdout(pParent->ToElement(), v_izz, indent + 1);
		}
		else if (sParameter == "COM")
		{
			num = dump_attribs_to_stdout(pParent->ToElement(), v_com, indent + 1);
		}
		else if (sParameter == "Angles")
		{
			num = dump_attribs_to_stdout(pParent->ToElement(), v_angles, indent + 1);
		}
		else if (sParameter == "X1")
		{
			num = dump_attribs_to_stdout(pParent->ToElement(), v_x1, indent + 1);
		}
		else if (sParameter == "Y1")
		{
			num = dump_attribs_to_stdout(pParent->ToElement(), v_y1, indent + 1);
		}
		else if (sParameter == "Z1")
		{
			num = dump_attribs_to_stdout(pParent->ToElement(), v_z1, indent + 1);
		}
		else if (sParameter == "X2")
		{
			num = dump_attribs_to_stdout(pParent->ToElement(), v_x2, indent + 1);
		}
		else if (sParameter == "Y2")
		{
			num = dump_attribs_to_stdout(pParent->ToElement(), v_y2, indent + 1);
		}
		else if (sParameter == "Z2")
		{
			num = dump_attribs_to_stdout(pParent->ToElement(), v_z2, indent + 1);
		}
		else
		{
			printf("ERROR IN XML. PLEASE CHECK \n");
		}

		switch (num)
		{
		case 0:  printf(" (No attributes)"); break;
		case 1:  printf("%s1 attribute", getIndentAlt(indent)); break;
		default: printf("%s%d attributes", getIndentAlt(indent), num); break;
		}
		break;
	}
	case TiXmlNode::COMMENT:
		printf("Comment: [%s]", pParent->Value());
		break;

	case TiXmlNode::UNKNOWN:
		printf("Unknown");
		break;

	case TiXmlNode::TEXT:
		pText = pParent->ToText();
		printf("Text: [%s]", pText->Value());
		break;

	case TiXmlNode::DECLARATION:
		printf("Declaration");
		break;
	default:
		break;
	}
	printf("\n");
	for (pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
	{
		dump_to_stdout(pChild, indent + 1);
	}
}

// load the named file and dump its structure to STDOUT
void dRaycastVHModel::dump_to_stdout(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
		//printf("\n%s:\n", pFilename);
		dump_to_stdout(&doc); 
	}
	else
	{
		printf("Failed to load file \"%s\"\n", pFilename);
	}
}

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
	, joint1(NULL)

{
	// LOADING DATA FROM XML//
	TiXmlDocument doc("DummyGeometricProperties.xml");
	bool loadOkay = doc.LoadFile();
	(dump_to_stdout(&doc)); //parse the xml and load parameters
	if (v_scale.size() != 1 || v_lengths.size() != 15 || v_weight.size() != 1 || v_masses.size() != 8 || v_ixx.size() != 8 || v_iyy.size() != 8 || v_izz.size() != 8 || v_com.size() != 8 || v_angles.size() != 2)
		std::cout << "ERROR IN XML" << std::endl;
	// Saving data in the program//
	string tex("Textures//wood6.png");
	Scale = v_scale[0]; // Scale factor of dummy

	// LENGTH DATA
	l_Up_Leg	= Scale * v_lengths[0];
	l_Low_Leg	= Scale * v_lengths[1];
	l_plantar	= Scale * v_lengths[2];		// taken from Chadefaux et al
	l_Spine		= Scale * v_lengths[3];		//assumed to be upper part of trunk
	l_Up_Arm	= Scale * v_lengths[4];
	l_Low_Arm	= Scale * v_lengths[5];
	l_Hand		= Scale * v_lengths[6];
	l_Head		= Scale * v_lengths[7];
	l_Hip		= Scale * v_lengths[8];		//half length of hip
	l_Sacrum	= Scale * v_lengths[9];		//assumed to be Lower part of trunk
	w_foot		= l_plantar/2.0f;	
	l_toe		= Scale * v_lengths[10];	//taken from Chadefaux et al
	h_foot		= Scale * v_lengths[11];
	l_Neck		= Scale * v_lengths[12];
	l_Clav		= Scale * v_lengths[13];	//half length of shoulder width
	r_bones		= Scale * v_lengths[14];
	r_Pad		= w_foot/4.0f;
	h_sphere	= h_foot / 2;

	std::vector<float> l = {l_Head, l_Spine, l_Up_Arm, l_Low_Arm, l_Hand, l_Up_Leg, l_Low_Leg, l_plantar };
	
	// MASSE DATA
	float tot_w = Scale * v_weight[0]; //private variable?
	std::vector<float> masses  = { v_masses[0] * tot_w, v_masses[1] * tot_w, v_masses[2] * tot_w, v_masses[3] * tot_w, v_masses[4] * tot_w, v_masses[5] * tot_w, v_masses[6] * tot_w, v_masses[7] * tot_w};
	
	//INERTIA DATA
	//	     				 {		0 head,			1 trunk,		 2 uparm,		3 forearm,	     4 hand,			5 thigh,		 6 shank,		  7 foot } table 4 de leva technical notes
	std::vector<float> Ixx = { masses[0] * v_ixx[0], masses[1] * v_ixx[1], masses[2] * v_ixx[2], masses[3] * v_ixx[3], masses[4] * v_ixx[4], masses[5] * v_ixx[5], masses[6] * v_ixx[6], masses[7] * v_ixx[7] }; //radii of gyration Sagittal (X) %
	std::vector<float> Iyy = { masses[0] * v_iyy[0], masses[1] * v_iyy[1], masses[2] * v_iyy[2], masses[3] * v_iyy[3], masses[4] * v_iyy[4], masses[5] * v_iyy[5], masses[6] * v_iyy[6], masses[7] * v_iyy[7] }; //radii of gyration Transverse (Y) %
	std::vector<float> Izz = { masses[0] * v_izz[0], masses[1] * v_izz[1], masses[2] * v_izz[2], masses[3] * v_izz[3], masses[4] * v_izz[4], masses[5] * v_izz[5], masses[6] * v_izz[6], masses[7] * v_izz[7] }; //radii of gyration Longitudinal (Z) %
	
	// COM distance from the origin of each body (DIRECTION X: IN LOCAL COORDINATE OF THE BODY)
	//std::vector<float> comY = { 0.5976f, 0.4486f, 0.5772f, 0.4574f, 0.7900f, 0.4095f, 0.4459f, 0.4415f }; //table 4 de leva technical notes
	vector<float> DeltaCM(v_com.size(), 0);

	for (int ii = 0; ii < DeltaCM.size(); ii++)
		DeltaCM[ii] = (0.5f - v_com[ii]) * l[ii];

	glm::vec3 _Pos(glm::vec3(0.0f, l_Sacrum/2+ l_Up_Leg+ l_Low_Leg+ h_foot+l_Hip+0.25f, 0.f)); // INITIAL POSITION  OF THE DUMMY IN THE SCENE X (lateral) Y(vertical) Z(front)

	float hip_angle = v_angles[0];
	float clav_angle = v_angles[1];


	// BODIES OF THE DUMMY DEFINITION
	Sacrum = new GeomNewton(m_winManager->aManager);
	Sacrum->SetBodyType(adtDynamic);
	Sacrum->SetTexture0(&tex[0], "Tex0");
	Sacrum->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Sacrum->SetRollAngle(90.0f ,false);
	Sacrum->SetPosition(_Pos.x, _Pos.y, _Pos.z);
	Sacrum->InitNewton(atCapsule, r_bones, r_bones,l_Sacrum, 10.0f);
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
	Spine->SetPosition(0, ( l_Sacrum  + l_Spine) /2 + 2 * r_bones, 0);
	Spine->InitNewton(atCapsule, r_bones, r_bones, l_Spine, 10.0f);
	NewtonBodySetTransformCallback(Spine->GetBody(), NULL);
	Lumbar_Node = new dModelNode(Spine->GetBody(), dGetIdentityMatrix(), this);
	NewtonBodySetMassMatrix(Spine->GetBody(), masses[1], Ixx[1], Iyy[1], Izz[1]);
	dVector com;
	NewtonBodyGetCentreOfMass(Spine->GetBody(), &com[0]);
	com.m_y += DeltaCM[1];
	NewtonBodySetCentreOfMass(Spine->GetBody(), &com[0]); // WIP

	Neck = new GeomNewton(m_winManager->aManager);
	Neck->SetBodyType(adtDynamic);
	Neck->SetParent(Spine);
	Neck->SetTexture0(&tex[0], "Tex0");
	Neck->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Neck->SetPosition(0,   (l_Neck + l_Spine) / 2 + 2* r_bones, 0);
	Neck->InitNewton(atCapsule, r_bones,  r_bones, l_Neck, 10.0f );
	NewtonBodySetTransformCallback(Neck->GetBody(), NULL);
	Neck_Node = new dModelNode(Neck->GetBody(), dGetIdentityMatrix(), Lumbar_Node);

	Head = new GeomNewton(m_winManager->aManager);
	Head->SetBodyType(adtDynamic);
	Head->SetParent(Neck);
	Head->SetTexture0(&tex[0], "Tex0");
	Head->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Head->SetPosition(0, (l_Head + l_Neck) / 2 + 2 * r_bones, 0);
	Head->InitNewton(atCapsule,  r_bones,  r_bones,  l_Head, 10.0f);
	NewtonBodySetTransformCallback(Head->GetBody(), NULL);
	Head_Node = new dModelNode(Head->GetBody(), dGetIdentityMatrix(), Neck_Node);
	NewtonBodySetMassMatrix(Head->GetBody(), masses[0], Ixx[0], Iyy[0], Izz[0]);
	NewtonBodyGetCentreOfMass(Head->GetBody(), &com[0]);
	com.m_y = com.m_y + DeltaCM[0];
	NewtonBodySetCentreOfMass(Head->GetBody(), &com[0]); // WIP

	//Left lower limb

	Hip_L = new GeomNewton(m_winManager->aManager);
	Hip_L->SetBodyType(adtDynamic);
	Hip_L->SetParent(Sacrum);
	Hip_L->SetTexture0(&tex[0], "Tex0");
	Hip_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Hip_L->SetRollAngle(90.0f+hip_angle, false);
	Hip_L->SetPosition(l_Hip*cos(hip_angle * dDegreeToRad)/2+ 2 * r_bones,-l_Sacrum/ 2+  l_Hip*sin(hip_angle * dDegreeToRad)/2,0);
	Hip_L->InitNewton(atCapsule, r_bones, r_bones, l_Hip, 10.0f);
	NewtonBodySetTransformCallback(Hip_L->GetBody(), NULL);
	Hip_LNode = new dModelNode(Hip_L->GetBody(), dGetIdentityMatrix(), this);

	Up_Leg_L = new GeomNewton(m_winManager->aManager);
	Up_Leg_L->SetBodyType(adtDynamic);
	Up_Leg_L->SetParent(Hip_L);
	Up_Leg_L->SetTexture0(&tex[0], "Tex0");
	Up_Leg_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Up_Leg_L->SetRollAngle(-90.0f - hip_angle, false);
	Up_Leg_L->SetPosition( l_Hip / 2*cos(hip_angle* dDegreeToRad) , sin(hip_angle * dDegreeToRad)*  l_Hip / 2 - l_Up_Leg / 2, 0);
	Up_Leg_L->InitNewton(atCapsule, r_bones,r_bones, l_Up_Leg, 10.0f);
	NewtonBodySetTransformCallback(Up_Leg_L->GetBody(), NULL);
	Up_Leg_LNode = new dModelNode(Up_Leg_L->GetBody(), dGetIdentityMatrix(), Hip_LNode);
	NewtonBodySetMassMatrix(Up_Leg_L->GetBody(), masses[5], Ixx[5], Iyy[5], Izz[5]);
	NewtonBodyGetCentreOfMass(Up_Leg_L->GetBody(), &com[0]);
	com.m_y = com.m_y + DeltaCM[5];
	NewtonBodySetCentreOfMass(Up_Leg_L->GetBody(), &com[0]); // WIP

	Low_Leg_L = new GeomNewton(m_winManager->aManager);
	Low_Leg_L->SetBodyType(adtDynamic);
	Low_Leg_L->SetParent(Up_Leg_L);
	Low_Leg_L->SetTexture0(&tex[0], "Tex0");
	Low_Leg_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Low_Leg_L->SetPosition(0,- (l_Up_Leg + l_Low_Leg) / 2 ,  0);
	Low_Leg_L->InitNewton(atCapsule,  r_bones,   r_bones,  l_Low_Leg, 10.0f);
	NewtonBodySetTransformCallback(Low_Leg_L->GetBody(), NULL);
	Low_Leg_LNode = new dModelNode(Low_Leg_L->GetBody(), dGetIdentityMatrix(), Up_Leg_LNode);
	NewtonBodySetMassMatrix(Low_Leg_L->GetBody(), masses[6], Ixx[6], Iyy[6], Izz[6]);
	NewtonBodyGetCentreOfMass(Low_Leg_L->GetBody(), &com[0]);
	com.m_y = com.m_y + DeltaCM[6];
	NewtonBodySetCentreOfMass(Low_Leg_L->GetBody(), &com[0]); // WIP

	Plantar_L = new GeomNewton(m_winManager->aManager);
	Plantar_L->SetBodyType(adtDynamic);
	Plantar_L->SetParent(Low_Leg_L);
	Plantar_L->SetTexture0(&tex[0], "Tex0");
	Plantar_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Plantar_L->SetPitchAngle(90.0f, false);
	Plantar_L->SetPosition (0,-( l_Low_Leg) / 2 -2*   r_bones, -  (l_plantar / 4));
	Plantar_L->InitNewton(atBox, h_foot,    l_plantar,   w_foot, 10.f  ); // CHECK
	NewtonBodySetTransformCallback(Plantar_L->GetBody(), NULL);
	Plantar_LNode = new dModelNode(Plantar_L->GetBody(), dGetIdentityMatrix(), Low_Leg_LNode);

	Toe_L = new GeomNewton(m_winManager->aManager);
	Toe_L->SetBodyType(adtDynamic);
	Toe_L->SetParent(Plantar_L);
	Toe_L->SetTexture0(&tex[0], "Tex0");
	Toe_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Toe_L->SetPosition( 0,  0,-  ( l_plantar / 2 + l_toe / 2));
	Toe_L->InitNewton(atBox, h_foot, l_toe, w_foot, 1.0f); // CHECK
	NewtonBodySetTransformCallback(Toe_L->GetBody(), NULL);
	Toe_LNode = new dModelNode(Toe_L->GetBody(), dGetIdentityMatrix(), Plantar_LNode);

	//Right lower limb

	Hip_R = new GeomNewton(m_winManager->aManager);
	Hip_R->SetBodyType(adtDynamic);
	Hip_R->SetParent(Sacrum);
	Hip_R->SetTexture0(&tex[0], "Tex0");
	Hip_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Hip_R->SetRollAngle(-90.0f - hip_angle, false);
	Hip_R->SetPosition(-  l_Hip* cos(hip_angle* dDegreeToRad) / 2 - 2*   r_bones, -  l_Sacrum / 2 +   l_Hip * sin(hip_angle * dDegreeToRad) / 2, 0);
	Hip_R->InitNewton(atCapsule,   r_bones,   r_bones,   l_Hip, 10.0f  );
	NewtonBodySetTransformCallback(Hip_R->GetBody(), NULL);
	Hip_RNode = new dModelNode(Hip_R->GetBody(), dGetIdentityMatrix(), this);

	Up_Leg_R = new GeomNewton(m_winManager->aManager);
	Up_Leg_R->SetBodyType(adtDynamic);
	Up_Leg_R->SetParent(Hip_R);
	Up_Leg_R->SetTexture0(&tex[0], "Tex0");
	Up_Leg_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Up_Leg_R->SetRollAngle(90.0f + hip_angle, false);
	Up_Leg_R->SetPosition(-  l_Hip / 2 * cos(hip_angle * dDegreeToRad) , sin(hip_angle* dDegreeToRad)*   l_Hip / 2 -   l_Up_Leg / 2, 0);
	Up_Leg_R->InitNewton(atCapsule,   r_bones,   r_bones,   l_Up_Leg, 10.0f  );
	NewtonBodySetTransformCallback(Up_Leg_R->GetBody(), NULL);
	Up_Leg_RNode = new dModelNode(Up_Leg_R->GetBody(), dGetIdentityMatrix(), Hip_RNode);
	NewtonBodySetMassMatrix(Up_Leg_R->GetBody(), masses[5], Ixx[5], Iyy[5], Izz[5]);
	NewtonBodyGetCentreOfMass(Up_Leg_R->GetBody(), &com[0]);
	com.m_y = com.m_y + DeltaCM[5];
	NewtonBodySetCentreOfMass(Up_Leg_R->GetBody(), &com[0]); // WIP

	Low_Leg_R = new GeomNewton(m_winManager->aManager);
	Low_Leg_R->SetBodyType(adtDynamic);
	Low_Leg_R->SetParent(Up_Leg_R);
	Low_Leg_R->SetTexture0(&tex[0], "Tex0");
	Low_Leg_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Low_Leg_R->SetPosition(0, -  (l_Up_Leg + l_Low_Leg) / 2 , 0);
	Low_Leg_R->InitNewton(atCapsule,   r_bones,   r_bones,   l_Low_Leg, 10.0f);
	NewtonBodySetTransformCallback(Low_Leg_R->GetBody(), NULL);
	Low_Leg_RNode = new dModelNode(Low_Leg_R->GetBody(), dGetIdentityMatrix(), Up_Leg_RNode);
	NewtonBodySetMassMatrix(Low_Leg_R->GetBody(), masses[6], Ixx[6], Iyy[6], Izz[6]);
	NewtonBodySetTransformCallback(Low_Leg_R->GetBody(), NULL);
	Low_Leg_RNode = new dModelNode(Low_Leg_R->GetBody(), dGetIdentityMatrix(), Up_Leg_RNode);
	NewtonBodySetMassMatrix(Low_Leg_R->GetBody(), masses[6], Ixx[6], Iyy[6], Izz[6]);
	NewtonBodyGetCentreOfMass(Low_Leg_R->GetBody(), &com[0]);
	com.m_y = com.m_y + DeltaCM[6];
	NewtonBodySetCentreOfMass(Low_Leg_R->GetBody(), &com[0]); // WIP

	Plantar_R = new GeomNewton(m_winManager->aManager);
	Plantar_R->SetBodyType(adtDynamic);
	Plantar_R->SetParent(Low_Leg_R);
	Plantar_R->SetTexture0(&tex[0], "Tex0");
	Plantar_R->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Plantar_R->SetPitchAngle(90.0f, false);
	Plantar_R->SetPosition(0, -  (l_Low_Leg) / 2 -2*   r_bones, -  (l_plantar / 4));
	Plantar_R->InitNewton(atBox, h_foot,   l_plantar,   w_foot, 10.0f  ); // CHECK
	NewtonBodySetTransformCallback(Plantar_R->GetBody(), NULL);
	Plantar_RNode = new dModelNode(Plantar_R->GetBody(), dGetIdentityMatrix(), Low_Leg_RNode);

	Toe_R = new GeomNewton(m_winManager->aManager);
	Toe_R->SetBodyType(adtDynamic);
	Toe_R->SetParent(Plantar_R);
	Toe_R->SetTexture0(&tex[0], "Tex0");
	Toe_R->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Toe_R->SetPosition(0, 0, -  (l_plantar / 2 + l_toe / 2));
	Toe_R->InitNewton(atBox, h_foot, l_toe,    w_foot, 1.f  );// CHECK
	NewtonBodySetTransformCallback(Toe_R->GetBody(), NULL);
	Toe_RNode = new dModelNode(Toe_R->GetBody(), dGetIdentityMatrix(), Plantar_RNode); 

	// Left upper limb

	Clav_L = new GeomNewton(m_winManager->aManager);
	Clav_L->SetBodyType(adtDynamic);
	Clav_L->SetParent(Spine);
	Clav_L->SetTexture0(&tex[0], "Tex0");
	Clav_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Clav_L->SetRollAngle(90.0f + clav_angle, false);
	Clav_L->SetPosition(  l_Clav* cos(clav_angle* dDegreeToRad) / 2 + 2 *   r_bones,   l_Spine / 2 +   l_Clav * sin(clav_angle * dDegreeToRad) / 2, 0);
	Clav_L->InitNewton(atCapsule,   r_bones,   r_bones,   l_Clav, 10.0f  );
	NewtonBodySetTransformCallback(Clav_L->GetBody(), NULL);
	Clav_LNode = new dModelNode(Clav_L->GetBody(), dGetIdentityMatrix(), Lumbar_Node);

	Up_Arm_L = new GeomNewton(m_winManager->aManager);
	Up_Arm_L->SetBodyType(adtDynamic);
	Up_Arm_L->SetParent(Clav_L);
	Up_Arm_L->SetTexture0(&tex[0], "Tex0");
	Up_Arm_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Up_Arm_L->SetRollAngle(-90.0f  - clav_angle, false);
	Up_Arm_L->SetPosition(  l_Clav * cos(clav_angle * dDegreeToRad) / 2, sin(clav_angle* dDegreeToRad)*   l_Clav / 2 -   l_Up_Arm / 2, 0);
	Up_Arm_L->InitNewton(atCapsule,   r_bones,   r_bones,   l_Up_Arm, 10.0f  );
	NewtonBodySetTransformCallback(Up_Arm_L->GetBody(), NULL);
	Shoulder_LNode = new dModelNode(Up_Arm_L->GetBody(), dGetIdentityMatrix(), Clav_LNode);
	NewtonBodySetMassMatrix(Up_Arm_L->GetBody(), masses[2], Ixx[2], Iyy[2], Izz[2]);
	NewtonBodyGetCentreOfMass(Up_Arm_L->GetBody(), &com[0]);
	com.m_y = com.m_y + DeltaCM[2];
	NewtonBodySetCentreOfMass(Up_Arm_L->GetBody(), &com[0]); // WIP

	Low_Arm_L = new GeomNewton(m_winManager->aManager);
	Low_Arm_L->SetBodyType(adtDynamic);
	Low_Arm_L->SetParent(Up_Arm_L);
	Low_Arm_L->SetTexture0(&tex[0], "Tex0");
	Low_Arm_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Low_Arm_L->SetPosition(0, -  (l_Up_Arm + l_Low_Arm) / 2, 0);
	Low_Arm_L->InitNewton(atCapsule,   r_bones,   r_bones,   l_Low_Arm, 10.0f);
	NewtonBodySetTransformCallback(Low_Arm_L->GetBody(), NULL);
	Elbow_LNode = new dModelNode(Low_Arm_L->GetBody(), dGetIdentityMatrix(), Shoulder_LNode);
	NewtonBodySetMassMatrix(Low_Arm_L->GetBody(), masses[3], Ixx[3], Iyy[3], Izz[3]);
	NewtonBodyGetCentreOfMass(Low_Arm_L->GetBody(), &com[0]);
	com.m_y = com.m_y + DeltaCM[3];
	NewtonBodySetCentreOfMass(Low_Arm_L->GetBody(), &com[0]); // WIP

	Hand_L = new GeomNewton(m_winManager->aManager);
	Hand_L->SetBodyType(adtDynamic);
	Hand_L->SetParent(Low_Arm_L);
	Hand_L->SetTexture0(&tex[0], "Tex0");
	Hand_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Hand_L->SetPitchAngle(90.0f, false);
	Hand_L->SetPosition(0, -  (l_Low_Arm+ l_Hand) / 2,0);
	Hand_L->InitNewton(atCapsule,   r_bones,   r_bones,   l_Hand, 10.0f  );
	NewtonBodySetTransformCallback(Hand_L->GetBody(), NULL);
	Wrist_LNode = new dModelNode(Hand_L->GetBody(), dGetIdentityMatrix(), Elbow_LNode);
	NewtonBodySetMassMatrix(Hand_L->GetBody(), masses[4], Ixx[4], Iyy[4], Izz[4]);
	NewtonBodyGetCentreOfMass(Hand_L->GetBody(), &com[0]);
	com.m_y = com.m_y + DeltaCM[4];
	NewtonBodySetCentreOfMass(Hand_L->GetBody(), &com[0]); // WIP

	// Right upper limb

	Clav_R = new GeomNewton(m_winManager->aManager);
	Clav_R->SetBodyType(adtDynamic);
	Clav_R->SetParent(Spine);
	Clav_R->SetTexture0(&tex[0], "Tex0");
	Clav_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Clav_R->SetRollAngle(90.0f - clav_angle, false);
	Clav_R->SetPosition(-  l_Clav* cos(clav_angle* dDegreeToRad) / 2 - 2 *   r_bones,   l_Spine / 2 +   l_Clav * sin(clav_angle * dDegreeToRad) / 2, 0);
	Clav_R->InitNewton(atCapsule,   r_bones,   r_bones,   l_Clav, 10.0f  );
	NewtonBodySetTransformCallback(Clav_R->GetBody(), NULL);
	Clav_RNode = new dModelNode(Clav_R->GetBody(), dGetIdentityMatrix(), Lumbar_Node);

	Up_Arm_R = new GeomNewton(m_winManager->aManager);
	Up_Arm_R->SetBodyType(adtDynamic);
	Up_Arm_R->SetParent(Clav_R);
	Up_Arm_R->SetTexture0(&tex[0], "Tex0");
	Up_Arm_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Up_Arm_R->SetRollAngle(-90.0f + clav_angle, false);
	Up_Arm_R->SetPosition(-  l_Clav* cos(clav_angle* dDegreeToRad) / 2, sin(clav_angle* dDegreeToRad)*   l_Clav / 2 -   l_Up_Arm / 2, 0);
	Up_Arm_R->InitNewton(atCapsule,   r_bones,   r_bones,   l_Up_Arm, 10.0f  );
	NewtonBodySetTransformCallback(Up_Arm_R->GetBody(), NULL);
	Shoulder_RNode = new dModelNode(Up_Arm_R->GetBody(), dGetIdentityMatrix(), Clav_RNode);
	NewtonBodySetMassMatrix(Up_Arm_R->GetBody(), masses[2], Ixx[2], Iyy[2], Izz[2]);
	NewtonBodyGetCentreOfMass(Up_Arm_R->GetBody(), &com[0]);
	com.m_y = com.m_y + DeltaCM[2];
	NewtonBodySetCentreOfMass(Up_Arm_R->GetBody(), &com[0]); // WIP

	Low_Arm_R = new GeomNewton(m_winManager->aManager);
	Low_Arm_R->SetBodyType(adtDynamic);
	Low_Arm_R->SetParent(Up_Arm_R);
	Low_Arm_R->SetTexture0(&tex[0], "Tex0");
	Low_Arm_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Low_Arm_R->SetPosition(0, -  (l_Up_Arm + l_Low_Arm) / 2, 0);
	Low_Arm_R->InitNewton(atCapsule,   r_bones,   r_bones,   l_Low_Arm, 10.0f);
	NewtonBodySetTransformCallback(Low_Arm_R->GetBody(), NULL);
	Elbow_RNode = new dModelNode(Low_Arm_R->GetBody(), dGetIdentityMatrix(), Shoulder_RNode);
	NewtonBodySetMassMatrix(Low_Arm_R->GetBody(), masses[3], Ixx[3], Iyy[3], Izz[3]);
	NewtonBodyGetCentreOfMass(Low_Arm_R->GetBody(), &com[0]);
	com.m_y = com.m_y + DeltaCM[3];
	NewtonBodySetCentreOfMass(Low_Arm_R->GetBody(), &com[0]); // WIP

	Hand_R = new GeomNewton(m_winManager->aManager);
	Hand_R->SetBodyType(adtDynamic);
	Hand_R->SetParent(Low_Arm_R);
	Hand_R->SetTexture0(&tex[0], "Tex0");
	Hand_R->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Hand_R->SetPitchAngle(90.0f, false);
	Hand_R->SetPosition(0, -  (l_Low_Arm + l_Hand) / 2, 0);
	Hand_R->InitNewton(atCapsule,   r_bones,   r_bones,   l_Hand, 10.0f  );
	NewtonBodySetTransformCallback(Hand_R->GetBody(), NULL);
	Wrist_RNode = new dModelNode(Hand_R->GetBody(), dGetIdentityMatrix(), Elbow_RNode);
	NewtonBodySetMassMatrix(Hand_R->GetBody(), masses[4], Ixx[4], Iyy[4], Izz[4]);
	NewtonBodyGetCentreOfMass(Hand_R->GetBody(), &com[0]);
	com.m_y = com.m_y + DeltaCM[4];
	NewtonBodySetCentreOfMass(Hand_R->GetBody(), &com[0]); // WIP
    
	// Foot pads

	Heel_L = new GeomNewton(m_winManager->aManager);
	Heel_L->SetBodyType(adtDynamic);
	Heel_L->SetParent(Plantar_L);
	Heel_L->SetTexture0(&tex[0], "Tex0");
	Heel_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Heel_L->SetPitchAngle(90.0f, false);
	Heel_L->SetPosition(0, -  h_sphere,   (l_plantar / 2));
	Heel_L->InitNewton(atSphere, r_Pad, r_Pad, r_Pad, 10.0f  );
	NewtonBodySetTransformCallback(Heel_L->GetBody(), NULL);
	Heel_L_Node = new dModelNode(Heel_L->GetBody(), dGetIdentityMatrix(), Plantar_LNode);
	NewtonBodySetMassMatrix(Heel_L->GetBody(), masses[7], Ixx[7], Iyy[7], Izz[7]);
	NewtonBodyGetCentreOfMass(Heel_L->GetBody(), &com[0]);
	com.m_x = com.m_x - DeltaCM[7];
	NewtonBodySetCentreOfMass(Heel_L->GetBody(), &com[0]); // WIP

	Pad1_L = new GeomNewton(m_winManager->aManager);
	Pad1_L->SetBodyType(adtDynamic);
	Pad1_L->SetParent(Toe_L);
	Pad1_L->SetTexture0(&tex[0], "Tex0");
	Pad1_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Pad1_L->SetPitchAngle(90.0f, false);
	Pad1_L->SetPosition(   w_foot / 2, -  h_sphere, - (l_toe / 2));
	Pad1_L->InitNewton(atSphere, r_Pad, r_Pad, r_Pad, 10.0f  );
	NewtonBodySetTransformCallback(Pad1_L->GetBody(), NULL);
	Pad1_L_Node = new dModelNode(Pad1_L->GetBody(), dGetIdentityMatrix(), Toe_LNode);

	Pad2_L = new GeomNewton(m_winManager->aManager);
	Pad2_L->SetBodyType(adtDynamic);
	Pad2_L->SetParent(Toe_L);
	Pad2_L->SetTexture0(&tex[0], "Tex0");
	Pad2_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Pad2_L->SetPitchAngle(90.0f, false);
	Pad2_L->SetPosition( -  w_foot / 2, -  h_sphere, -  (l_toe/ 2));
	Pad2_L->InitNewton(atSphere, r_Pad, r_Pad, r_Pad, 10.0f  );
	NewtonBodySetTransformCallback(Pad2_L->GetBody(), NULL);
	Pad2_L_Node = new dModelNode(Pad2_L->GetBody(), dGetIdentityMatrix(), Toe_LNode);

	Heel_R = new GeomNewton(m_winManager->aManager);
	Heel_R->SetBodyType(adtDynamic);
	Heel_R->SetParent(Plantar_R);
	Heel_R->SetTexture0(&tex[0], "Tex0");
	Heel_R->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Heel_R->SetPitchAngle(90.0f, false);
	Heel_R->SetPosition(0, -  h_sphere,   (l_plantar / 2));
	Heel_R->InitNewton(atSphere, r_Pad, r_Pad, r_Pad, 10.0f  );
	NewtonBodySetTransformCallback(Heel_R->GetBody(), NULL);
	Heel_R_Node = new dModelNode(Heel_R->GetBody(), dGetIdentityMatrix(), Plantar_RNode);
	NewtonBodySetMassMatrix(Heel_R->GetBody(), masses[7], Ixx[7], Iyy[7], Izz[7]);
	NewtonBodyGetCentreOfMass(Heel_R->GetBody(), &com[0]);
	com.m_x = com.m_x - DeltaCM[7];
	NewtonBodySetCentreOfMass(Heel_R->GetBody(), &com[0]); // WIP

	Pad1_R = new GeomNewton(m_winManager->aManager);
	Pad1_R->SetBodyType(adtDynamic);
	Pad1_R->SetParent(Toe_R);
	Pad1_R->SetTexture0(&tex[0], "Tex0");
	Pad1_R->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Pad1_R->SetPitchAngle(90.0f, false);
	Pad1_R->SetPosition(  w_foot / 2, -  h_sphere, -(l_toe / 2));
	Pad1_R->InitNewton(atSphere, r_Pad, r_Pad, r_Pad, 10.0f  );
	NewtonBodySetTransformCallback(Pad1_R->GetBody(), NULL);
	Pad1_R_Node = new dModelNode(Pad1_R->GetBody(), dGetIdentityMatrix(), Toe_RNode);

	Pad2_R = new GeomNewton(m_winManager->aManager);
	Pad2_R->SetBodyType(adtDynamic);
	Pad2_R->SetParent(Toe_R);
	Pad2_R->SetTexture0(&tex[0], "Tex0");
	Pad2_R->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Pad2_R->SetPitchAngle(90.0f, false);
	Pad2_R->SetPosition(-  w_foot / 2, -  h_sphere, -(l_toe / 2));
	Pad2_R->InitNewton(atSphere, r_Pad, r_Pad, r_Pad, 10.0f  );
	NewtonBodySetTransformCallback(Pad2_R->GetBody(), NULL);
	Pad2_R_Node = new dModelNode(Pad2_R->GetBody(), dGetIdentityMatrix(), Toe_RNode);


	//Pad joints

	//Left foot
	dMatrix Heel_L_PinMatrix(dGetIdentityMatrix());
	Heel_L_PinMatrix.m_posit = dVector(0, -  h_sphere,   (l_plantar / 2));
	Hl_L = new dCustomDoubleHinge(Heel_L_PinMatrix, Heel_L->GetBody(), Plantar_L->GetBody());
	Hl_L->SetMassIndependentSpringDamper(true,0.01, 1.e20f, 1.e15f);
	Hl_L->SetMassIndependentSpringDamper1(true,0.01, 1.e20f, 1.e15f);
	m_winManager->aManager->vJointList.push_back(Hl_L);

	dMatrix Pad1_L_PinMatrix(dGetIdentityMatrix());
	Pad1_L_PinMatrix.m_posit = dVector(  w_foot / 2, -  h_sphere, -  (l_plantar / 4));
	Pd1_L = new dCustomDoubleHinge(Pad1_L_PinMatrix, Pad1_L->GetBody(), Toe_L->GetBody());
	Pd1_L->SetMassIndependentSpringDamper(true, 0.01, 1.e20f, 1.e15f);
	Pd1_L->SetMassIndependentSpringDamper1(true, 0.01, 1.e20f, 1.e15f);
	m_winManager->aManager->vJointList.push_back(Pd1_L);

	dMatrix Pad2_L_PinMatrix(dGetIdentityMatrix());
	Pad2_L_PinMatrix.m_posit = dVector(-  w_foot / 2, -  h_sphere, -  (l_plantar / 4));
	Pd2_L = new dCustomDoubleHinge(Pad2_L_PinMatrix, Pad2_L->GetBody(), Toe_L->GetBody());
	Pd2_L->SetMassIndependentSpringDamper(true, 0.01, 1.e20f, 1.e15f);
	Pd2_L->SetMassIndependentSpringDamper1(true, 0.01, 1.e20f, 1.e15f);
	m_winManager->aManager->vJointList.push_back(Pd2_L);

	//Right foot
	dMatrix Heel_R_PinMatrix(dGetIdentityMatrix());
	Heel_R_PinMatrix.m_posit = dVector(0, -  h_sphere,   (l_plantar / 2));
	Hl_R = new dCustomDoubleHinge(Heel_R_PinMatrix, Heel_R->GetBody(), Plantar_R->GetBody());
	Hl_R->SetMassIndependentSpringDamper(true, 0.01, 1.e20f, 1.e15f);
	Hl_R->SetMassIndependentSpringDamper1(true, 0.01, 1.e20f, 1.e15f);
	m_winManager->aManager->vJointList.push_back(Hl_R);

	dMatrix Pad1_R_PinMatrix(dGetIdentityMatrix());
	Pad1_R_PinMatrix.m_posit = dVector(  w_foot / 2, -  h_sphere, -  (l_plantar / 4));
	Pd1_R = new dCustomDoubleHinge(Pad1_R_PinMatrix, Pad1_R->GetBody(), Toe_R->GetBody());
	Pd1_R->SetMassIndependentSpringDamper(true, 0.01, 1.e20f, 1.e15f);
	Pd1_R->SetMassIndependentSpringDamper1(true, 0.01, 1.e20f, 1.e15f);
	m_winManager->aManager->vJointList.push_back(Pd1_R);

	dMatrix Pad2_R_PinMatrix(dGetIdentityMatrix());
	Pad2_R_PinMatrix.m_posit = dVector(-  w_foot / 2, -  h_sphere, -  (l_plantar / 4));
	Pd2_R = new dCustomDoubleHinge(Pad2_R_PinMatrix, Pad2_R->GetBody(), Toe_R->GetBody());
	Pd2_R->SetMassIndependentSpringDamper(true, 0.01, 1.e20f, 1.e15f);
	Pd2_R->SetMassIndependentSpringDamper1(true, 0.01, 1.e20f, 1.e15f);
	m_winManager->aManager->vJointList.push_back(Pd2_R); 

	//trunk joints

	// create Lumbar joint. 
	dMatrix Spine_PinMatrix(dYawMatrix(90.0f * dDegreeToRad));
	Spine_PinMatrix.m_posit = dVector(_Pos.x , _Pos.y +   l_Sacrum  / 2 +   r_bones, _Pos.z);
	Disk2 = new dCustomDoubleHinge(Spine_PinMatrix, Spine->GetBody(), Sacrum->GetBody());
	Disk2->SetMassIndependentSpringDamper(true,0.3,  1.e6f, 1.e4f);
	Disk2->SetMassIndependentSpringDamper1(true,0.3,  1.e6f, 1.e4f);
	m_winManager->aManager->vJointList.push_back(Disk2);

	// create Neck joint. 
	//dMatrix Neck_PinMatrix(dYawMatrix(90.0f * dDegreeToRad));
	dMatrix Neck_PinMatrix(dGetIdentityMatrix());
	Neck_PinMatrix.m_posit = dVector(Neck->GetPosition().m_x, Neck->GetPosition().m_y -   l_Neck / 2 -    r_bones, Neck->GetPosition().m_z);
	Nck = new dCustomDoubleHinge(Neck_PinMatrix, Neck->GetBody(), Spine->GetBody());
	Nck->SetMassIndependentSpringDamper(true, 0.01, 1000.f, 10.f);
	Nck->SetMassIndependentSpringDamper1(true, 0.01, 1000.f, 10.f);
	m_winManager->aManager->vJointList.push_back(Nck);

	// create Head joint. 
	dMatrix Head_PinMatrix(dYawMatrix(90.0f * dDegreeToRad));
	Head_PinMatrix.m_posit = dVector(Head->GetPosition().m_x, Head->GetPosition().m_y -   l_Head / 2 -    r_bones, Head->GetPosition().m_z);
	Hd = new dCustomDoubleHinge(Head_PinMatrix, Head->GetBody(), Neck->GetBody());
	Hd->SetMassIndependentSpringDamper(true, 0.01, 1000.f, 10.f);
	Hd->SetMassIndependentSpringDamper1(true, 0.01, 1000.f, 10.f);
	m_winManager->aManager->vJointList.push_back(Hd);

	// Left upper limb joints

	// create sternum joint.

	dMatrix Sternum_PinMatrix(dYawMatrix(90.0f * dDegreeToRad));

	Sternum_PinMatrix.m_posit = dVector(Spine->GetPosition().m_x +   r_bones, Spine->GetPosition().m_y+   l_Spine / 2, Spine->GetPosition().m_z);
	Strn_L = new dCustomDoubleHinge(Sternum_PinMatrix, Clav_L->GetBody(), Spine->GetBody());
	Strn_L->SetMassIndependentSpringDamper(true, 0.01, 1.e6f, 1.e4f);
	Strn_L->SetMassIndependentSpringDamper1(true, 0.01, 1.e6f, 1.e4f);
	m_winManager->aManager->vJointList.push_back(Strn_L);

	// create Shoulder joint. 
	dMatrix Shoulder_LPinMatrix(dGetIdentityMatrix());
	Shoulder_LPinMatrix = Shoulder_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Shoulder_LPinMatrix.m_posit = dVector(Up_Arm_L->GetPosition().m_x, Up_Arm_L->GetPosition().m_y +   l_Up_Arm / 2, Up_Arm_L->GetPosition().m_z);
	shld_L = new dCustomBallAndSocket(Shoulder_LPinMatrix,Up_Arm_L->GetBody(), Clav_L->GetBody() );
	m_winManager->aManager->vJointList.push_back(shld_L);

	// create elbow joint. 
	dMatrix Elbow_LPinMatrix(dGetIdentityMatrix());
	Elbow_LPinMatrix = Elbow_LPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Elbow_LPinMatrix.m_posit = dVector(Low_Arm_L->GetPosition().m_x, Low_Arm_L->GetPosition().m_y +   l_Low_Arm / 2, Low_Arm_L->GetPosition().m_z);
	Elb_L = new dCustomHinge(Elbow_LPinMatrix, Low_Arm_L->GetBody(), Up_Arm_L->GetBody());
	Elb_L->EnableLimits(true);
	Elb_L->SetLimits(0.f * dDegreeToRad, 180.f);	
	m_winManager->aManager->vJointList.push_back(Elb_L);

	// create wrist joint.
	dMatrix Wrist_LPinMatrix(dGetIdentityMatrix());
	Wrist_LPinMatrix = Wrist_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Wrist_LPinMatrix.m_posit = dVector(Hand_L->GetPosition().m_x, Hand_L->GetPosition().m_y +   l_Hand / 2, Hand_L->GetPosition().m_z);
	Wr_L = new dCustomBallAndSocket(Wrist_LPinMatrix, Hand_L->GetBody(), Low_Arm_L->GetBody());
	m_winManager->aManager->vJointList.push_back(Wr_L);

	// Right upper limb joints

	// create sternum joint. 

	Sternum_PinMatrix.m_posit = dVector(Spine->GetPosition().m_x -   r_bones, Spine->GetPosition().m_y +   l_Spine / 2, Spine->GetPosition().m_z);
	Strn_R = new dCustomDoubleHinge(Sternum_PinMatrix, Clav_R->GetBody(), Spine->GetBody());
	Strn_R->SetMassIndependentSpringDamper(true, 0.01, 1.e6f, 1.e4f);
	Strn_R->SetMassIndependentSpringDamper1(true, 0.01, 1.e6f, 1.e4f);
	m_winManager->aManager->vJointList.push_back(Strn_R);


	// create Shoulder joint. 
	dMatrix Shoulder_RPinMatrix(dGetIdentityMatrix());
	Shoulder_RPinMatrix = Shoulder_RPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Shoulder_RPinMatrix.m_posit = dVector(Up_Arm_R->GetPosition().m_x, Up_Arm_R->GetPosition().m_y +   l_Up_Arm / 2, Up_Arm_R->GetPosition().m_z);
	shld_R = new dCustomBallAndSocket(Shoulder_RPinMatrix, Up_Arm_R->GetBody(),Clav_R->GetBody());
	m_winManager->aManager->vJointList.push_back(shld_R);

	// create elbow joint. 
	dMatrix Elbow_RPinMatrix(dGetIdentityMatrix());
	Elbow_RPinMatrix = Elbow_RPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Elbow_RPinMatrix.m_posit = dVector(Low_Arm_R->GetPosition().m_x, Low_Arm_R->GetPosition().m_y +   l_Low_Arm / 2, Low_Arm_R->GetPosition().m_z);
	Elb_R = new dCustomHinge(Elbow_RPinMatrix, Low_Arm_R->GetBody(), Up_Arm_R->GetBody());
	Elb_R->EnableLimits(true);
	Elb_R->SetLimits(0.f * dDegreeToRad, 180.f);
	m_winManager->aManager->vJointList.push_back(Elb_R);

	// create wrist joint.
	dMatrix Wrist_RPinMatrix(dGetIdentityMatrix());
	Wrist_RPinMatrix = Wrist_RPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Wrist_RPinMatrix.m_posit = dVector(Hand_R->GetPosition().m_x, Hand_R->GetPosition().m_y +   l_Hand / 2, Hand_R->GetPosition().m_z);
	Wr_R = new dCustomBallAndSocket(Wrist_RPinMatrix, Hand_R->GetBody(), Low_Arm_R->GetBody());
	m_winManager->aManager->vJointList.push_back(Wr_R);



	//Left lower limb joints

	// create Sacrum joint. 
	dMatrix Sacrum_PinMatrix(dYawMatrix(90.0f * dDegreeToRad));
	Sacrum_PinMatrix.m_posit = dVector(_Pos.x +   r_bones, _Pos.y -   l_Sacrum / 2, _Pos.z);
	Disk1_L = new dCustomDoubleHinge(Sacrum_PinMatrix, Hip_L->GetBody(), Sacrum->GetBody());
	Disk1_L->SetMassIndependentSpringDamper(true,0.01,  1.e6f, 1.e4f);
	Disk1_L->SetMassIndependentSpringDamper1(true, 0.01, 1.e6f, 1.e4f);
	m_winManager->aManager->vJointList.push_back(Disk1_L);

	// create hip joint. 
	dMatrix Hip_LPinMatrix(dGetIdentityMatrix());
	Hip_LPinMatrix = Hip_LPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Hip_LPinMatrix.m_posit = dVector(Up_Leg_L->GetPosition().m_x , Up_Leg_L->GetPosition().m_y +   l_Up_Leg / 2, Up_Leg_L->GetPosition().m_z);
	Rotule_L = new dCustomDoubleHinge(Hip_LPinMatrix, Up_Leg_L->GetBody(), Hip_L->GetBody());
	Rotule_L->EnableLimits(true);
	Rotule_L->EnableLimits1(true);
	Rotule_L->SetLimits(-40.f * dDegreeToRad,120.f * dDegreeToRad);
	Rotule_L->SetLimits1(-5.f * dDegreeToRad, 30.f * dDegreeToRad);
	m_winManager->aManager->vJointList.push_back(Rotule_L);

	// create knee joint. 
	dMatrix Knee_LPinMatrix(dGetIdentityMatrix());
	Knee_LPinMatrix = Knee_LPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Knee_LPinMatrix.m_posit = dVector(Low_Leg_L->GetPosition().m_x, Low_Leg_L->GetPosition().m_y +   l_Low_Leg / 2 , Low_Leg_L->GetPosition().m_z);
	Knee_L = new dCustomHinge(Knee_LPinMatrix, Low_Leg_L->GetBody(), Up_Leg_L->GetBody());
	Knee_L->EnableLimits(true);
	Knee_L->SetLimits(-180.f * dDegreeToRad,0.f);
	m_winManager->aManager->vJointList.push_back(Knee_L);

	// create ankle joint.
	dMatrix Ankle_LPinMatrix(dGetIdentityMatrix()); //flexion twist
	//Ankle_LPinMatrix = Ankle_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);//supination twist
	Ankle_LPinMatrix = Ankle_LPinMatrix * dRollMatrix(90.0f * dDegreeToRad);//torsion twist
	Ankle_LPinMatrix.m_posit = dVector(Low_Leg_L->GetPosition().m_x, Low_Leg_L->GetPosition().m_y -   l_Low_Leg / 2-   r_bones, Low_Leg_L->GetPosition().m_z);
	Ankle_L = new dCustomBallAndSocket(Ankle_LPinMatrix,  Plantar_L->GetBody(),Low_Leg_L->GetBody());
	Ankle_L->EnableTwist(true);
	Ankle_L->SetTwistLimits(-15.f * dDegreeToRad,15.f * dDegreeToRad);
	Ankle_L->EnableCone(true);
	Ankle_L->SetConeLimits(40.f * dDegreeToRad );
	m_winManager->aManager->vJointList.push_back(Ankle_L);

	// create toe joint.
	dMatrix Toe_LPinMatrix(dGetIdentityMatrix());
	Toe_LPinMatrix = Toe_LPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Toe_LPinMatrix.m_posit = dVector(Plantar_L->GetPosition().m_x, Plantar_L->GetPosition().m_y, Plantar_L->GetPosition().m_z-   l_plantar/2);
	Flextoe_L = new dCustomHinge(Toe_LPinMatrix, Plantar_L->GetBody(), Toe_L->GetBody());
	Flextoe_L->SetAsSpringDamper(true,  1.e5f, 1.e4f);
	m_winManager->aManager->vJointList.push_back(Flextoe_L);

	//Right lower limb joints
	
	// create Sacrum joint. 
	Sacrum_PinMatrix.m_posit = dVector(_Pos.x -   r_bones, _Pos.y -   l_Sacrum / 2, _Pos.z);
	Disk1_R = new dCustomDoubleHinge(Sacrum_PinMatrix, Hip_R->GetBody(), Sacrum->GetBody());
	Disk1_R->SetMassIndependentSpringDamper(true, 0.01, 1.e6f, 1.e4f);
	Disk1_R->SetMassIndependentSpringDamper1(true, 0.01, 1.e6f, 1.e4f);
	m_winManager->aManager->vJointList.push_back(Disk1_L);

	// create hip joint. 
	dMatrix Hip_RPinMatrix(dGetIdentityMatrix());
	Hip_RPinMatrix = Hip_RPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Hip_RPinMatrix.m_posit = dVector(Up_Leg_R->GetPosition().m_x , Up_Leg_R->GetPosition().m_y +   l_Up_Leg / 2, Up_Leg_R->GetPosition().m_z);
	Rotule_R = new dCustomDoubleHinge(Hip_RPinMatrix, Up_Leg_R->GetBody(), Hip_R->GetBody());
	Rotule_R->EnableLimits(true);
	Rotule_R->EnableLimits1(true);
	Rotule_R->SetLimits(-40.f * dDegreeToRad, 120.f * dDegreeToRad);
	Rotule_R->SetLimits1(-30.f * dDegreeToRad, 5.0f * dDegreeToRad);
	m_winManager->aManager->vJointList.push_back(Rotule_R);

	// create knee joint. 
	dMatrix Knee_RPinMatrix(dGetIdentityMatrix());
	Knee_RPinMatrix = Knee_RPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Knee_RPinMatrix.m_posit = dVector(Low_Leg_R->GetPosition().m_x, Low_Leg_R->GetPosition().m_y +   l_Low_Leg / 2 , Low_Leg_R->GetPosition().m_z);
	Knee_R = new dCustomHinge(Knee_RPinMatrix, Low_Leg_R->GetBody(), Up_Leg_R->GetBody());
	Knee_R->EnableLimits(true);
	Knee_R->SetLimits(-180.f * dDegreeToRad, 0);
	m_winManager->aManager->vJointList.push_back(Knee_R);

	// create ankle joint.
	dMatrix Ankle_RPinMatrix(dGetIdentityMatrix());
	Ankle_RPinMatrix = Ankle_RPinMatrix * dRollMatrix(90.0f * dDegreeToRad);
	Ankle_RPinMatrix.m_posit = dVector(Low_Leg_R->GetPosition().m_x, Low_Leg_R->GetPosition().m_y -   l_Low_Leg / 2-   r_bones, Low_Leg_R->GetPosition().m_z);
	Ankle_R = new dCustomBallAndSocket(Ankle_RPinMatrix, Plantar_R->GetBody(), Low_Leg_R->GetBody());
	Ankle_R->EnableTwist(true);
	Ankle_R->SetTwistLimits(-15.f * dDegreeToRad, 15.f * dDegreeToRad);
	Ankle_R->EnableCone(true);
	Ankle_R->SetConeLimits(40.f * dDegreeToRad);
	m_winManager->aManager->vJointList.push_back(Ankle_R);

	// create toe joint.
	dMatrix Toe_RPinMatrix(dGetIdentityMatrix());
	Toe_RPinMatrix = Toe_RPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Toe_RPinMatrix.m_posit = dVector(Plantar_R->GetPosition().m_x, Plantar_R->GetPosition().m_y, Plantar_R->GetPosition().m_z -   l_plantar / 2);
	Flextoe_R = new dCustomHinge(Toe_RPinMatrix, Plantar_R->GetBody(), Toe_R->GetBody());
	Flextoe_R->SetAsSpringDamper(true,  1.e5f, 1.e4f);
	m_winManager->aManager->vJointList.push_back(Flextoe_R);
	
	// MUSCLE DEFINITION

	ins11 = dVector(0.f, 0.f, 0.f);
	ins12 = dVector(0.f, 0.f, 0.f);
/*	m1 = new Muscle(m_winManager->aLineManager, m_winManager->aManager, GetUp_Leg_L(), GetLow_Leg_L(), ins11, ins12);
	m1->GenerateMesh();*/


	// FOOT LINES
	FootLineIndex_L = CreateFootScanLine();
	FootLineIndex_R = CreateFootScanLine();

	// ADDITIONAL BODIES TO TEST MUSCLE
	// body 1 //
	float l1 = 0.5f;
	float l2 = 0.5f;
	b1 = new GeomNewton(m_winManager->aManager);
	b1->SetBodyType(adtDynamic);
	b1->SetParent(Toe_R);
	b1->SetTurnAngle(90.0f, false);
	b1->SetTexture0(&tex[0], "Tex0");
	b1->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	b1->SetPosition(5, 0, 0);
	b1->InitNewton(atBox, l1, 0.1, 0.1, 1);
	NewtonBodySetTransformCallback(b1->GetBody(), NULL);
	b1_Node = new dModelNode(b1->GetBody(), dGetIdentityMatrix(), Toe_RNode);

	b2 = new GeomNewton(m_winManager->aManager);
	b2->SetBodyType(adtDynamic);
	b2->SetParent(b1);
	b2->SetTexture0(&tex[0], "Tex0");
	b2->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	b2->SetPosition(l1 / 2 + l2 / 2, 0, 0);
	b2->InitNewton(atBox, l2, 0.1, 0.1, 1);
	NewtonBodySetTransformCallback(b2->GetBody(), NULL);
	b2_Node = new dModelNode(b2->GetBody(), dGetIdentityMatrix(), b1_Node);

	// create knee joint. 
	dMatrix joint1PinMatrix(dGetIdentityMatrix());
	joint1PinMatrix = joint1PinMatrix * dRollMatrix(90.0f * dDegreeToRad);
	joint1PinMatrix.m_posit = dVector(b1->GetPosition().m_x + l1/2, b1->GetPosition().m_y , b1->GetPosition().m_z);
	joint1 = new dCustomHingeActuator(joint1PinMatrix, b1->GetBody(), b2->GetBody());
	joint1->SetTargetAngle(-50.0f * dDegreeToRad);
	joint1->EnableMotor(true, 500.0f * dDegreeToRad);
	joint1->SetLimits(-3600.f, 3600.f * dDegreeToRad);
	m_winManager->aManager->vJointList.push_back(joint1);



	if (1) // select 1 if you want to use (type Hill's model)
	{
		/*
		ins11 = dVector(0.f, 0.f, 0.f);
		ins12 = dVector(0.f, 0.f, 0.f);
		m1 = new Muscle(m_winManager->aLineManager, m_winManager->aManager, Getb1(), Getb2(), ins11, ins12);
		m1->GenerateMesh();
		m_winManager->aManager->vMuscleList.push_back(m1);
		*/
		
		
		//The Origin point is the one that is closer to the trunk
		//SOL muscle Creation..Origin Point: Low_Leg, Insertion Point: Plantar
		m_sol_L = new Muscle(m_winManager->aLineManager, m_winManager->aManager, Low_Leg_L, Plantar_L, dVector(v_x1[0], v_y1[0], v_z1[0]), dVector(v_x2[0], v_y2[0], v_z2[0]));
		m_sol_L->GenerateMesh();
		m_winManager->aManager->vMuscleList.push_back(m_sol_L);

		//TA muscle Creation..Origin Point 1: Low_Leg, Insertion Point: Plantar
		m_ta_L = new Muscle(m_winManager->aLineManager, m_winManager->aManager, Low_Leg_L, Plantar_L, dVector(v_x1[1], v_y1[1], v_z1[1]), dVector(v_x2[1], v_y2[1], v_z2[1]));
		m_ta_L->GenerateMesh();
		m_winManager->aManager->vMuscleList.push_back(m_ta_L);

		//GAS muscle Creation..Origin Point: Up_Leg, Insertion Point: Plantar
		m_gas_L = new Muscle(m_winManager->aLineManager, m_winManager->aManager, Up_Leg_L, Plantar_L, dVector(v_x1[2], v_y1[2], v_z1[2]), dVector(v_x2[2], v_y2[2], v_z2[2]));
		m_gas_L->GenerateMesh();
		m_winManager->aManager->vMuscleList.push_back(m_gas_L);

		//VAS muscle Creation..Origin Point: Up_Leg, Insertion Point: Low_Leg
		m_vas_L = new Muscle(m_winManager->aLineManager, m_winManager->aManager, Up_Leg_L, Low_Leg_L, dVector(v_x1[3], v_y1[3], v_z1[3]), dVector(v_x2[3], v_y2[3], v_z2[3]));
		m_vas_L->GenerateMesh();
		m_winManager->aManager->vMuscleList.push_back(m_vas_L);

		//HAM muscle Creation..Origin Point: Hip, Insertion Point: Low_Leg
		m_ham_L = new Muscle(m_winManager->aLineManager, m_winManager->aManager, Hip_L, Low_Leg_L, dVector(v_x1[4], v_y1[4], v_z1[4]), dVector(v_x2[4], v_y2[4], v_z2[4]));
		m_ham_L->GenerateMesh();
		m_winManager->aManager->vMuscleList.push_back(m_ham_L);

		//RF muscle Creation..Origin Point: Hip, Insertion Point: Low_Leg
		m_rf_L = new Muscle(m_winManager->aLineManager, m_winManager->aManager, Hip_L, Low_Leg_L, dVector(v_x1[5], v_y1[5], v_z1[5]), dVector(v_x2[5], v_y2[5], v_z2[5]));
		m_rf_L->GenerateMesh();
		m_winManager->aManager->vMuscleList.push_back(m_rf_L);

		//GLU muscle Creation..Origin Point: Hip, Insertion Point: Up_Leg
		m_glu_L = new Muscle(m_winManager->aLineManager, m_winManager->aManager, Hip_L, Up_Leg_L, dVector(v_x1[6], v_y1[6], v_z1[6]), dVector(v_x2[6], v_y2[6], v_z2[6]));
		m_glu_L->GenerateMesh();
		m_winManager->aManager->vMuscleList.push_back(m_glu_L);

		//HFL muscle Creation..Origin Point: Hip, Insertion Point: Up_Leg
		m_hfl_L = new Muscle(m_winManager->aLineManager, m_winManager->aManager, Hip_L, Up_Leg_L, dVector(v_x1[7], v_y1[7], v_z1[7]), dVector(v_x2[7], v_y2[7], v_z2[7]));
		m_hfl_L->GenerateMesh();
		m_winManager->aManager->vMuscleList.push_back(m_hfl_L);

		//RIGHT SIDE MUSCLE GROUP

		//SOL muscle Creation..Origin Point: Low_Leg, Insertion Point: Plantar
		m_sol_R = new Muscle(m_winManager->aLineManager, m_winManager->aManager, Low_Leg_R, Plantar_R, dVector(v_x1[0], v_y1[0], v_z1[0]), dVector(v_x2[0], v_y2[0], v_z2[0]));
		m_sol_R->GenerateMesh();
		m_winManager->aManager->vMuscleList.push_back(m_sol_R);

		//TA muscle Creation..Origin Point 1: Low_Leg, Insertion Point: Plantar
		m_ta_R = new Muscle(m_winManager->aLineManager, m_winManager->aManager, Low_Leg_R, Plantar_R, dVector(v_x1[1], v_y1[1], v_z1[1]), dVector(v_x2[1], v_y2[1], v_z2[1]));
		m_ta_R->GenerateMesh();
		m_winManager->aManager->vMuscleList.push_back(m_ta_R);

		//GAS muscle Creation..Origin Point: Up_Leg, Insertion Point: Plantar
		m_gas_R = new Muscle(m_winManager->aLineManager, m_winManager->aManager, Up_Leg_R, Plantar_R, dVector(v_x1[2], v_y1[2], v_z1[2]), dVector(v_x2[2], v_y2[2], v_z2[2]));
		m_gas_R->GenerateMesh();
		m_winManager->aManager->vMuscleList.push_back(m_gas_R);

		//VAS muscle Creation..Origin Point: Up_Leg, Insertion Point: Low_Leg
		m_vas_R = new Muscle(m_winManager->aLineManager, m_winManager->aManager, Up_Leg_R, Low_Leg_R, dVector(v_x1[3], v_y1[3], v_z1[3]), dVector(v_x2[3], v_y2[3], v_z2[3]));
		m_vas_R->GenerateMesh();
		m_winManager->aManager->vMuscleList.push_back(m_vas_R);

		//HAM muscle Creation..Origin Point: Hip, Insertion Point: Low_Leg
		m_ham_R = new Muscle(m_winManager->aLineManager, m_winManager->aManager, Hip_R, Low_Leg_R, dVector(v_x1[4], v_y1[4], v_z1[4]), dVector(v_x2[4], v_y2[4], v_z2[4]));
		m_ham_R->GenerateMesh();
		m_winManager->aManager->vMuscleList.push_back(m_ham_R);

		//RF muscle Creation..Origin Point: Hip, Insertion Point: Low_Leg
		m_rf_R = new Muscle(m_winManager->aLineManager, m_winManager->aManager, Hip_R, Low_Leg_R, dVector(v_x1[5], v_y1[5], v_z1[5]), dVector(v_x2[5], v_y2[5], v_z2[5]));
		m_rf_R->GenerateMesh();
		m_winManager->aManager->vMuscleList.push_back(m_rf_R);

		//GLU muscle Creation..Origin Point: Hip, Insertion Point: Up_Leg
		m_glu_R = new Muscle(m_winManager->aLineManager, m_winManager->aManager, Hip_R, Up_Leg_R, dVector(v_x1[6], v_y1[6], v_z1[6]), dVector(v_x2[6], v_y2[6], v_z2[6]));
		m_glu_R->GenerateMesh();
		m_winManager->aManager->vMuscleList.push_back(m_glu_R);

		//HFL muscle Creation..Origin Point: Hip, Insertion Point: Up_Leg
		m_hfl_R = new Muscle(m_winManager->aLineManager, m_winManager->aManager, Hip_R, Up_Leg_R, dVector(v_x1[7], v_y1[7], v_z1[7]), dVector(v_x2[7], v_y2[7], v_z2[7]));
		m_hfl_R->GenerateMesh();
		m_winManager->aManager->vMuscleList.push_back(m_hfl_R);

		
	}
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

dCustomHingeActuator* dRaycastVHModel::Getjoint1() {
	return joint1;
}

GeomNewton* dRaycastVHModel::Getb1() {
	return b1;
}

GeomNewton* dRaycastVHModel::Getb2() {
	return b2;
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
	//cout << "DGVehicleRCManager OnPreUpdate \n";


	dVector Vtemp(0.0f);

	// scan all  Muscle Elements
	for (auto itr = m_winManager->aManager->vMuscleList.begin();
		itr != m_winManager->aManager->vMuscleList.end(); itr++)
	{
		Muscle* Mobj = (Muscle*)*itr;

		// Get the Body1 connected to the muscle and apply the muscle force
		GeomNewton* gNewton = (GeomNewton*)(Mobj->body1);
		NewtonBody* NBody = gNewton->GetBody();
		Vtemp = Mobj->GetForceElas(timestep);
		//cout << timestep << ' ' << Vtemp[0] << ' ' << Vtemp[1] << ' ' << Vtemp[2] << '\n';
		//dVector P = Mobj->GetInsert1_GlobalRef(); // Check
		dVector vOrigin(0.0f), vInsert(0.0f);
		Mobj->GetOriginAndInsertion(vOrigin, vInsert);
		AddForceAtRelPos(NBody, &Vtemp[0], &vOrigin[0]);

		// Get the Body2 connected and apply the opposite muscle force
		gNewton = (GeomNewton*)(Mobj->body2);
		NBody = (NewtonBody*)gNewton->GetBody();
		Vtemp = Mobj->GetForceElas(timestep);
		Vtemp = Vtemp.Scale(-1.0f);
		//P = Mobj->GetInsert2_GlobalRef(); // Check
		AddForceAtRelPos(NBody, &Vtemp[0], &vInsert[0]);

		monFlux << newTime << "  " << Mobj->fSE(Mobj->GetDelta_l()) << "  " << Mobj->fCE(Mobj->GetDelta_l(), timestep) << "  " << Mobj->fPE(Mobj->GetDelta_l()) << "  " << Mobj->GetNmax() << "  " << Mobj->GetLCE() << "  " << Mobj->GetDelta_l() << std::endl;

		Mobj->SetLCE(Mobj->GetLCE() + Mobj->GetDelta_l());
	}

	newTime += timestep;
	if (newTime > 0.5f) {
		controller->Getjoint1()->SetTargetAngle((-50.0f + 10.0f * sin((newTime - 0.5f) * 2.0f * dPi)) * dDegreeToRad);
	}

}

void DGVehicleRCManager::OnPostUpdate(dModelRootNode* const model, dFloat timestep, int threadID) const
//pour VS<VS2017 supprimer ,int threadID

{
	//printf("DGVehicleRCManager OnPostUpdate \n");
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



