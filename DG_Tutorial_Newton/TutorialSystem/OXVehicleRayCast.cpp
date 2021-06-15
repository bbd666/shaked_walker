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
			num = dump_attribs_to_stdout(pParent->ToElement(), v_total_weight, indent + 1);
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
		else if (sParameter == "MaxMuscleForce")
		{
			num = dump_attribs_to_stdout(pParent->ToElement(), v_maxForce, indent + 1);
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
	, LPT(NULL)
	, MPT(NULL)
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
	, Foot_LNode(NULL)
	, Toe_LNode(NULL)
	, Hip_R(NULL)
	, Up_Leg_R(NULL)
	, Low_Leg_R(NULL)
	, Plantar_R(NULL)
	, Toe_R(NULL)
	, Disk1_R(NULL)
	, UPT(NULL)
	, Head(NULL)
	, Rotule_R(NULL)
	, Knee_R(NULL)
	, Ankle_R(NULL)
	, Flextoe_R(NULL)
	, Hip_RNode(NULL)
	, Up_Leg_RNode(NULL)
	, Low_Leg_RNode(NULL)
	, Foot_RNode(NULL)
	, Toe_RNode(NULL)
	, Lumbar_Node(NULL)
	, Clav_LNode(NULL)
	, Shoulder_LNode(NULL)
	, Elbow_LNode(NULL)
	, Wrist_LNode(NULL)
	, Clav_RNode(NULL)
	, Shoulder_RNode(NULL)
	, Elbow_RNode(NULL)
	, UPT_Node(NULL)
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
	// LOADING DATA FROM XML//
	TiXmlDocument doc("DummyGeometricProperties.xml");
	bool loadOkay = doc.LoadFile();
	(dump_to_stdout(&doc)); //parse the xml and load parameters
	if (v_scale.size() != 2 || v_lengths.size() != 17 || v_total_weight.size() != 1 || v_masses.size() != 12 || v_ixx.size() != 12 || v_iyy.size() != 12 || v_izz.size() != 12 || v_com.size() != 12 || v_angles.size() != 2)
		std::cout << "ERROR IN XML" << std::endl;
	// Saving data in the program//
	string tex("Textures//wood6.png");

	// Scale factor for length and mass
	scale_length = v_scale[0]; 
	scale_mass = v_scale[1];

	// LENGTH DATA
	l_Head		= scale_length * v_lengths[0]; 
	l_Up_Arm	= scale_length * v_lengths[1]; 
	l_Low_Arm	= scale_length * v_lengths[2];  // forearm
	l_Hand		= scale_length * v_lengths[3];
	l_trunk		= scale_length * v_lengths[4];
	l_Up_Leg	= scale_length * v_lengths[5];  // thigh
	l_Low_Leg	= scale_length * v_lengths[6];  // shank
	l_foot		= scale_length * v_lengths[7];	
	l_toe		= scale_length * v_lengths[8];	
	l_UPT		= scale_length * v_lengths[9];  // upper part trunk
	l_MPT		= scale_length * v_lengths[10]; // medium part trunk
	l_LPT		= scale_length * v_lengths[11]; // lower part trunk
	l_Hip		= scale_length * v_lengths[12];
	l_Clav		= scale_length * v_lengths[13];	// shoulder 
	w_foot		= scale_length * v_lengths[14]; // foot breadth
	h_foot		= scale_length * v_lengths[15]; // ankle height
	l_neck		= scale_length * v_lengths[16];
	l_delta		= scale_length * (0.0345- 0.0213); // distance from acromion to shoulder joint (arcomion position computed relative to proximal point of UPT 553.2-531.9 mm)
	r_bones		= h_foot / 2.0f;
	r_Pad		= w_foot / 4.0f;
	h_sphere	= h_foot / 2.0f;
	l_foot		= l_foot - l_toe;
	ankle_j		= scale_length * 0.046f * sin(49 * dDegreeToRad); // [m] distance from heel to ankle joint in z direction ((Isman and Inman, 1969; Lee et al., 2011; Zatsiorsky, 2002)

	//						0 head, 1 uparm, 2 forearm, 3 hand, 4 trunk, 5 thigh, 6 shank, 7 foot, 8 toes, 9 UPT, 10 MPT, 11 LPT
	std::vector<float> l = {l_Head, l_Up_Arm, l_Low_Arm, l_Hand, l_trunk, l_Up_Leg, l_Low_Leg, l_foot, l_toe, l_UPT, l_MPT, l_LPT};
	
	// MASS DATA
	float tot_w = scale_mass * v_total_weight[0]; 
	//							{0 head, 1 uparm, 2 forearm, 3 hand, 4 trunk, 5 thigh, 6 shank, 7 foot, 8 toes, 9 UPT, 10 MPT, 11 LPT }
	std::vector<float> masses  = { v_masses[0] * tot_w, v_masses[1] * tot_w, v_masses[2] * tot_w, v_masses[3] * tot_w, v_masses[4] * tot_w, v_masses[5] * tot_w, v_masses[6] * tot_w, v_masses[7] * tot_w, v_masses[8] * tot_w , v_masses[9] * tot_w , v_masses[10] * tot_w , v_masses[11] * tot_w };
	
	//INERTIA DATA
	//	     				 {0 head, 1 uparm, 2 forearm, 3 hand, 4 trunk, 5 thigh, 6 shank, 7 foot, 8 toes, 9 UPT, 10 MPT, 11 LPT } 
	std::vector<float> Ixx = { masses[0] * v_ixx[0], masses[1] * v_ixx[1], masses[2] * v_ixx[2], masses[3] * v_ixx[3], masses[4] * v_ixx[4], masses[5] * v_ixx[5], masses[6] * v_ixx[6], masses[7] * v_ixx[7] , masses[8] * v_ixx[8] , masses[9] * v_ixx[9] , masses[10] * v_ixx[10] , masses[11] * v_ixx[11] }; //radii of gyration Sagittal (X) %
	std::vector<float> Iyy = { masses[0] * v_iyy[0], masses[1] * v_iyy[1], masses[2] * v_iyy[2], masses[3] * v_iyy[3], masses[4] * v_iyy[4], masses[5] * v_iyy[5], masses[6] * v_iyy[6], masses[7] * v_iyy[7] , masses[8] * v_iyy[8] , masses[9] * v_iyy[9] , masses[10] * v_iyy[10] , masses[11] * v_iyy[11] }; //radii of gyration Transverse (Y) %
	std::vector<float> Izz = { masses[0] * v_izz[0], masses[1] * v_izz[1], masses[2] * v_izz[2], masses[3] * v_izz[3], masses[4] * v_izz[4], masses[5] * v_izz[5], masses[6] * v_izz[6], masses[7] * v_izz[7] , masses[8] * v_izz[8] , masses[9] * v_izz[9] , masses[10] * v_izz[10] , masses[11] * v_izz[11] }; //radii of gyration Longitudinal (Z) %
	
	// COM distance from the origin of each body (DIRECTION X: IN LOCAL COORDINATE OF THE BODY)
	vector<float> DeltaCM(v_com.size(), 0);

	for (int ii = 0; ii < DeltaCM.size(); ii++)
		DeltaCM[ii] = (0.5f - v_com[ii]) * l[ii];

	glm::vec3 _Pos(glm::vec3(0.0f, l_LPT/2+ l_Up_Leg+ l_Low_Leg+ h_foot+0.25f, 0.f)); // INITIAL POSITION  OF THE DUMMY IN THE SCENE X (lateral) Y(vertical) Z(front)

	float hip_angle = v_angles[0];
	float clav_angle = v_angles[1];

	float phi_hl = 180; // starting angle of the left hip [°]
	float phi_kl = 180; // starting angle of the left knee [°]
	float phi_al = 90; // starting angle of the  left ankle [°]
	float phi_hr = 180; // starting angle of the right hip [°]
	float phi_kr = 180; // starting angle of the right knee [°]
	float phi_ar = 90; // starting angle of the  right ankle [°]
	dVector com;

	// BODIES OF THE DUMMY DEFINITION
	LPT = new GeomNewton(m_winManager->aManager);
	LPT->SetBodyType(adtDynamic);
	LPT->SetTexture0(&tex[0], "Tex0");
	LPT->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	LPT->SetRollAngle(90.0f ,false);
	LPT->SetPosition(_Pos.x, _Pos.y, _Pos.z);
	LPT->InitNewton(atCapsule, r_bones, r_bones, l_LPT, 10.0f);
	m_body = LPT->GetBody();
	NewtonBodySetTransformCallback(m_body, NULL);
	NewtonBodySetMassMatrix(LPT->GetBody(), masses[11], Ixx[11], Iyy[11], Izz[11]);
	NewtonBodyGetCentreOfMass(LPT->GetBody(), &com[0]);
	com.m_y += DeltaCM[11];
	NewtonBodySetCentreOfMass(LPT->GetBody(), &com[0]); 

	MPT = new GeomNewton(m_winManager->aManager);
	MPT->SetBodyType(adtDynamic);
	MPT->SetParent(LPT);
	MPT->SetTexture0(&tex[0], "Tex0");
	MPT->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	MPT->SetPosition(0, ( l_LPT  + l_MPT) /2, 0);
	MPT->InitNewton(atCapsule, r_bones, r_bones, l_MPT, 10.0f);
	NewtonBodySetTransformCallback(MPT->GetBody(), NULL);
	Lumbar_Node = new dModelNode(MPT->GetBody(), dGetIdentityMatrix(), this);
	NewtonBodySetMassMatrix(MPT->GetBody(), masses[10], Ixx[10], Iyy[10], Izz[10]);
	NewtonBodyGetCentreOfMass(MPT->GetBody(), &com[0]);
	com.m_y += DeltaCM[10];
	NewtonBodySetCentreOfMass(MPT->GetBody(), &com[0]);

	UPT = new GeomNewton(m_winManager->aManager);
	UPT->SetBodyType(adtDynamic);
	UPT->SetParent(MPT);
	UPT->SetTexture0(&tex[0], "Tex0");
	UPT->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	UPT->SetPosition(0,   (l_UPT + l_MPT) / 2, 0);
	UPT->InitNewton(atCapsule, r_bones,  r_bones, l_UPT, 10.0f );
	NewtonBodySetTransformCallback(UPT->GetBody(), NULL);
	UPT_Node = new dModelNode(UPT->GetBody(), dGetIdentityMatrix(), Lumbar_Node);
	NewtonBodySetMassMatrix(UPT->GetBody(), masses[9], Ixx[9], Iyy[9], Izz[9]);
	NewtonBodyGetCentreOfMass(UPT->GetBody(), &com[0]);
	com.m_y += DeltaCM[9];
	NewtonBodySetCentreOfMass(UPT->GetBody(), &com[0]);

	Head = new GeomNewton(m_winManager->aManager);
	Head->SetBodyType(adtDynamic);
	Head->SetParent(UPT);
	Head->SetTexture0(&tex[0], "Tex0");
	Head->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Head->SetPosition(0, (l_Head + l_UPT) / 2, 0);
	Head->InitNewton(atCapsule,  r_bones,  r_bones,  l_Head, 10.0f);
	NewtonBodySetTransformCallback(Head->GetBody(), NULL);
	Head_Node = new dModelNode(Head->GetBody(), dGetIdentityMatrix(), UPT_Node);
	//NewtonBodySetMassMatrix(Head->GetBody(), 0.0f, 0.0f, 0.0f, 0.0f); // fixed head
	NewtonBodySetMassMatrix(Head->GetBody(), masses[0], Ixx[0], Iyy[0], Izz[0]);
	NewtonBodyGetCentreOfMass(Head->GetBody(), &com[0]);
	com.m_y = com.m_y + DeltaCM[0];
	NewtonBodySetCentreOfMass(Head->GetBody(), &com[0]);

	//Left lower limb

	Hip_L = new GeomNewton(m_winManager->aManager);
	Hip_L->SetBodyType(adtDynamic);
	Hip_L->SetParent(LPT);
	Hip_L->SetTexture0(&tex[0], "Tex0");
	Hip_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Hip_L->SetRollAngle(90.0f+hip_angle, false);
	Hip_L->SetPosition(l_Hip*cos(hip_angle * dDegreeToRad)/2, -l_LPT/ 2 + l_Hip*sin(hip_angle * dDegreeToRad)/2, 0.0);
	Hip_L->InitNewton(atCapsule, r_bones, r_bones, l_Hip, 1.0f);
	NewtonBodySetTransformCallback(Hip_L->GetBody(), NULL);
	Hip_LNode = new dModelNode(Hip_L->GetBody(), dGetIdentityMatrix(), this);

	Up_Leg_L = new GeomNewton(m_winManager->aManager);
	Up_Leg_L->SetBodyType(adtDynamic);
	Up_Leg_L->SetParent(Hip_L);
	Up_Leg_L->SetTexture0(&tex[0], "Tex0");
	Up_Leg_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Up_Leg_L->SetRollAngle(-90.0f - hip_angle, false);
	Up_Leg_L->SetTurnAngle(-(180.f - phi_hl), false);
	Up_Leg_L->SetPosition( l_Hip / 2*cos(hip_angle* dDegreeToRad) , sin(hip_angle * dDegreeToRad)*  l_Hip / 2 - (l_Up_Leg / 2 * cos(180.f * dDegreeToRad - phi_hl * dDegreeToRad)), -(l_Up_Leg / 2)* sin(180.f*dDegreeToRad -phi_hl* dDegreeToRad));
	Up_Leg_L->InitNewton(atCapsule, r_bones,r_bones, l_Up_Leg, 10.0f);
	NewtonBodySetTransformCallback(Up_Leg_L->GetBody(), NULL);
	Up_Leg_LNode = new dModelNode(Up_Leg_L->GetBody(), dGetIdentityMatrix(), Hip_LNode);
	NewtonBodySetMassMatrix(Up_Leg_L->GetBody(), masses[5], Ixx[5], Iyy[5], Izz[5]);
	NewtonBodyGetCentreOfMass(Up_Leg_L->GetBody(), &com[0]);
	com.m_y = com.m_y + DeltaCM[5];
	NewtonBodySetCentreOfMass(Up_Leg_L->GetBody(), &com[0]); 

	Low_Leg_L = new GeomNewton(m_winManager->aManager);
	Low_Leg_L->SetBodyType(adtDynamic);
	Low_Leg_L->SetParent(Up_Leg_L);
	Low_Leg_L->SetTurnAngle((180.f - phi_hl)+(180.f - phi_kl), false);
	Low_Leg_L->SetTexture0(&tex[0], "Tex0");
	Low_Leg_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Low_Leg_L->SetPosition(0,-l_Up_Leg/2 -((l_Low_Leg / 2) * cos(180.f * dDegreeToRad - phi_kl * dDegreeToRad)),  +(l_Low_Leg / 2) * sin(180.f * dDegreeToRad - phi_kl * dDegreeToRad));
	Low_Leg_L->InitNewton(atCapsule,  r_bones,   r_bones,  l_Low_Leg, 10.0f);
	NewtonBodySetTransformCallback(Low_Leg_L->GetBody(), NULL);
	Low_Leg_LNode = new dModelNode(Low_Leg_L->GetBody(), dGetIdentityMatrix(), Up_Leg_LNode);
	NewtonBodySetMassMatrix(Low_Leg_L->GetBody(), masses[6], Ixx[6], Iyy[6], Izz[6]);
	NewtonBodyGetCentreOfMass(Low_Leg_L->GetBody(), &com[0]);
	com.m_y = com.m_y + DeltaCM[6];
	NewtonBodySetCentreOfMass(Low_Leg_L->GetBody(), &com[0]); 

	Plantar_L = new GeomNewton(m_winManager->aManager);
	Plantar_L->SetBodyType(adtDynamic);
	Plantar_L->SetParent(Low_Leg_L);
	Plantar_L->SetTexture0(&tex[0], "Tex0");
	Plantar_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Plantar_L->SetPitchAngle(90.0f, false);
	Plantar_L->SetRollAngle((90.f - phi_al), false);
	Plantar_L->SetPosition (0,-( l_Low_Leg) / 2 -(h_foot/2), - (l_foot / 2 - ankle_j));
	Plantar_L->InitNewton(atBox, h_foot,    l_foot,   w_foot, 10.f  ); // CHECK
	NewtonBodySetTransformCallback(Plantar_L->GetBody(), NULL);
	Foot_LNode = new dModelNode(Plantar_L->GetBody(), dGetIdentityMatrix(), Low_Leg_LNode);
	NewtonBodySetMassMatrix(Plantar_L->GetBody(), masses[7], Ixx[7], Iyy[7], Izz[7]);
	NewtonBodyGetCentreOfMass(Plantar_L->GetBody(), &com[0]);
	com.m_y += DeltaCM[7];
	NewtonBodySetCentreOfMass(Plantar_L->GetBody(), &com[0]);

	Toe_L = new GeomNewton(m_winManager->aManager);
	Toe_L->SetBodyType(adtDynamic);
	Toe_L->SetParent(Plantar_L);
	Toe_L->SetTexture0(&tex[0], "Tex0");
	Toe_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Toe_L->SetPosition( 0,  0,-  ( l_foot / 2 + l_toe / 2));
	Toe_L->InitNewton(atBox, h_foot, l_toe, w_foot, 10.0f);
	NewtonBodySetTransformCallback(Toe_L->GetBody(), NULL);
	Toe_LNode = new dModelNode(Toe_L->GetBody(), dGetIdentityMatrix(), Foot_LNode);
	NewtonBodySetMassMatrix(Toe_L->GetBody(), masses[8], Ixx[8], Iyy[8], Izz[8]);
	NewtonBodyGetCentreOfMass(Toe_L->GetBody(), &com[0]);
	com.m_y += DeltaCM[8];
	NewtonBodySetCentreOfMass(Toe_L->GetBody(), &com[0]);

	//Right lower limb

	Hip_R = new GeomNewton(m_winManager->aManager);
	Hip_R->SetBodyType(adtDynamic);
	Hip_R->SetParent(LPT);
	Hip_R->SetTexture0(&tex[0], "Tex0");
	Hip_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Hip_R->SetRollAngle(-90.0f - hip_angle, false);
	Hip_R->SetPosition(-l_Hip*cos(hip_angle* dDegreeToRad) / 2, - l_LPT / 2 + l_Hip * sin(hip_angle * dDegreeToRad) / 2, 0);
	Hip_R->InitNewton(atCapsule,   r_bones,   r_bones,   l_Hip, 1.0f  );
	NewtonBodySetTransformCallback(Hip_R->GetBody(), NULL);
	Hip_RNode = new dModelNode(Hip_R->GetBody(), dGetIdentityMatrix(), this);

	Up_Leg_R = new GeomNewton(m_winManager->aManager);
	Up_Leg_R->SetBodyType(adtDynamic);
	Up_Leg_R->SetParent(Hip_R);
	Up_Leg_R->SetTexture0(&tex[0], "Tex0");
	Up_Leg_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Up_Leg_R->SetRollAngle(90.0f + hip_angle, false);
	Up_Leg_R->SetPosition(-  l_Hip / 2 * cos(hip_angle * dDegreeToRad) , sin(hip_angle* dDegreeToRad)*l_Hip / 2 - l_Up_Leg / 2, 0);
	Up_Leg_R->InitNewton(atCapsule,   r_bones,   r_bones,   l_Up_Leg, 10.0f  );
	NewtonBodySetTransformCallback(Up_Leg_R->GetBody(), NULL);
	Up_Leg_RNode = new dModelNode(Up_Leg_R->GetBody(), dGetIdentityMatrix(), Hip_RNode);
	NewtonBodySetMassMatrix(Up_Leg_R->GetBody(), masses[5], Ixx[5], Iyy[5], Izz[5]);
	NewtonBodyGetCentreOfMass(Up_Leg_R->GetBody(), &com[0]);
	com.m_y = com.m_y + DeltaCM[5];
	NewtonBodySetCentreOfMass(Up_Leg_R->GetBody(), &com[0]); 

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
	NewtonBodyGetCentreOfMass(Low_Leg_R->GetBody(), &com[0]);
	com.m_y = com.m_y + DeltaCM[6];
	NewtonBodySetCentreOfMass(Low_Leg_R->GetBody(), &com[0]); 

	Plantar_R = new GeomNewton(m_winManager->aManager);
	Plantar_R->SetBodyType(adtDynamic);
	Plantar_R->SetParent(Low_Leg_R);
	Plantar_R->SetTexture0(&tex[0], "Tex0");
	Plantar_R->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Plantar_R->SetPitchAngle(90.0f, false);
	Plantar_R->SetPosition(0, -(l_Low_Leg) / 2 - (h_foot / 2 ), -(l_foot / 2 - ankle_j));
	Plantar_R->InitNewton(atBox, h_foot,   l_foot,   w_foot, 10.0f  ); // CHECK
	NewtonBodySetTransformCallback(Plantar_R->GetBody(), NULL);
	Foot_RNode = new dModelNode(Plantar_R->GetBody(), dGetIdentityMatrix(), Low_Leg_RNode);
	NewtonBodySetMassMatrix(Plantar_R->GetBody(), masses[7], Ixx[7], Iyy[7], Izz[7]);
	NewtonBodyGetCentreOfMass(Plantar_R->GetBody(), &com[0]);
	com.m_y += DeltaCM[7];
	NewtonBodySetCentreOfMass(Plantar_R->GetBody(), &com[0]);

	Toe_R = new GeomNewton(m_winManager->aManager);
	Toe_R->SetBodyType(adtDynamic);
	Toe_R->SetParent(Plantar_R);
	Toe_R->SetTexture0(&tex[0], "Tex0");
	Toe_R->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Toe_R->SetPosition(0, 0, -  (l_foot / 2 + l_toe / 2));
	Toe_R->InitNewton(atBox, h_foot, l_toe,    w_foot, 1.f  );// CHECK
	NewtonBodySetTransformCallback(Toe_R->GetBody(), NULL);
	Toe_RNode = new dModelNode(Toe_R->GetBody(), dGetIdentityMatrix(), Foot_RNode); 
	NewtonBodySetMassMatrix(Toe_R->GetBody(), masses[8], Ixx[8], Iyy[8], Izz[8]);
	NewtonBodyGetCentreOfMass(Toe_R->GetBody(), &com[0]);
	com.m_y += DeltaCM[8];
	NewtonBodySetCentreOfMass(Toe_R->GetBody(), &com[0]);

	// Left upper limb

	Clav_L = new GeomNewton(m_winManager->aManager);
	Clav_L->SetBodyType(adtDynamic);
	Clav_L->SetParent(UPT);
	Clav_L->SetTexture0(&tex[0], "Tex0");
	Clav_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Clav_L->SetRollAngle(90.0f + clav_angle, false);
	Clav_L->SetPosition(  l_Clav* cos(clav_angle* dDegreeToRad) / 2 + 2 *   r_bones,   l_UPT / 2 +   l_Clav * sin(clav_angle * dDegreeToRad) / 2 - l_delta, 0);
	Clav_L->InitNewton(atCapsule,   r_bones,   r_bones,   l_Clav, 1.0f  );
	NewtonBodySetTransformCallback(Clav_L->GetBody(), NULL);
	Clav_LNode = new dModelNode(Clav_L->GetBody(), dGetIdentityMatrix(), UPT_Node);

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
	NewtonBodySetMassMatrix(Up_Arm_L->GetBody(), masses[1], Ixx[1], Iyy[1], Izz[1]);
	NewtonBodyGetCentreOfMass(Up_Arm_L->GetBody(), &com[0]);
	com.m_y = com.m_y + DeltaCM[1];
	NewtonBodySetCentreOfMass(Up_Arm_L->GetBody(), &com[0]); 

	Low_Arm_L = new GeomNewton(m_winManager->aManager);
	Low_Arm_L->SetBodyType(adtDynamic);
	Low_Arm_L->SetParent(Up_Arm_L);
	Low_Arm_L->SetTexture0(&tex[0], "Tex0");
	Low_Arm_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Low_Arm_L->SetPosition(0, -  (l_Up_Arm + l_Low_Arm) / 2, 0);
	Low_Arm_L->InitNewton(atCapsule,   r_bones,   r_bones,   l_Low_Arm, 10.0f);
	NewtonBodySetTransformCallback(Low_Arm_L->GetBody(), NULL);
	Elbow_LNode = new dModelNode(Low_Arm_L->GetBody(), dGetIdentityMatrix(), Shoulder_LNode);
	NewtonBodySetMassMatrix(Low_Arm_L->GetBody(), masses[2], Ixx[2], Iyy[2], Izz[2]);
	NewtonBodyGetCentreOfMass(Low_Arm_L->GetBody(), &com[0]);
	com.m_y = com.m_y + DeltaCM[2];
	NewtonBodySetCentreOfMass(Low_Arm_L->GetBody(), &com[0]); 

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
	NewtonBodySetMassMatrix(Hand_L->GetBody(), masses[3], Ixx[3], Iyy[3], Izz[3]);
	NewtonBodyGetCentreOfMass(Hand_L->GetBody(), &com[0]);
	com.m_y = com.m_y + DeltaCM[3];
	NewtonBodySetCentreOfMass(Hand_L->GetBody(), &com[0]);

	// Right upper limb

	Clav_R = new GeomNewton(m_winManager->aManager);
	Clav_R->SetBodyType(adtDynamic);
	Clav_R->SetParent(UPT);
	Clav_R->SetTexture0(&tex[0], "Tex0");
	Clav_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Clav_R->SetRollAngle(90.0f - clav_angle, false);
	Clav_R->SetPosition(-  l_Clav* cos(clav_angle* dDegreeToRad) / 2 - 2 *   r_bones,   l_UPT / 2 +   l_Clav * sin(clav_angle * dDegreeToRad) / 2 - l_delta, 0);
	Clav_R->InitNewton(atCapsule,   r_bones,   r_bones,   l_Clav, 1.0f  );
	NewtonBodySetTransformCallback(Clav_R->GetBody(), NULL);
	Clav_RNode = new dModelNode(Clav_R->GetBody(), dGetIdentityMatrix(), UPT_Node);

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
	NewtonBodySetMassMatrix(Up_Arm_R->GetBody(), masses[1], Ixx[1], Iyy[1], Izz[1]);
	NewtonBodyGetCentreOfMass(Up_Arm_R->GetBody(), &com[0]);
	com.m_y = com.m_y + DeltaCM[1];
	NewtonBodySetCentreOfMass(Up_Arm_R->GetBody(), &com[0]);

	Low_Arm_R = new GeomNewton(m_winManager->aManager);
	Low_Arm_R->SetBodyType(adtDynamic);
	Low_Arm_R->SetParent(Up_Arm_R);
	Low_Arm_R->SetTexture0(&tex[0], "Tex0");
	Low_Arm_R->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Low_Arm_R->SetPosition(0, -  (l_Up_Arm + l_Low_Arm) / 2, 0);
	Low_Arm_R->InitNewton(atCapsule,   r_bones,   r_bones,   l_Low_Arm, 10.0f);
	NewtonBodySetTransformCallback(Low_Arm_R->GetBody(), NULL);
	Elbow_RNode = new dModelNode(Low_Arm_R->GetBody(), dGetIdentityMatrix(), Shoulder_RNode);
	NewtonBodySetMassMatrix(Low_Arm_R->GetBody(), masses[2], Ixx[2], Iyy[2], Izz[2]);
	NewtonBodyGetCentreOfMass(Low_Arm_R->GetBody(), &com[0]);
	com.m_y = com.m_y + DeltaCM[2];
	NewtonBodySetCentreOfMass(Low_Arm_R->GetBody(), &com[0]);

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
	NewtonBodySetMassMatrix(Hand_R->GetBody(), masses[3], Ixx[3], Iyy[3], Izz[3]);
	NewtonBodyGetCentreOfMass(Hand_R->GetBody(), &com[0]);
	com.m_y = com.m_y + DeltaCM[3];
	NewtonBodySetCentreOfMass(Hand_R->GetBody(), &com[0]);
    
	// Foot pads

	Heel_L = new GeomNewton(m_winManager->aManager);
	Heel_L->SetBodyType(adtDynamic);
	Heel_L->SetParent(Plantar_L);
	Heel_L->SetTexture0(&tex[0], "Tex0");
	Heel_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Heel_L->SetPitchAngle(90.0f, false);
	Heel_L->SetPosition(0, -  h_sphere,   (l_foot / 2));
	Heel_L->InitNewton(atSphere, r_Pad, r_Pad, r_Pad, 1.0f  );
	NewtonBodySetTransformCallback(Heel_L->GetBody(), NULL);
	Heel_L_Node = new dModelNode(Heel_L->GetBody(), dGetIdentityMatrix(), Foot_LNode);

	Pad1_L = new GeomNewton(m_winManager->aManager);
	Pad1_L->SetBodyType(adtDynamic);
	Pad1_L->SetParent(Plantar_L);
	Pad1_L->SetTexture0(&tex[0], "Tex0");
	Pad1_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Pad1_L->SetPitchAngle(90.0f, false);
	Pad1_L->SetPosition(   w_foot / 2, -  h_sphere, - (l_foot / 2 - r_Pad));
	Pad1_L->InitNewton(atSphere, r_Pad, r_Pad, r_Pad, 1.0f  ); //mass
	NewtonBodySetTransformCallback(Pad1_L->GetBody(), NULL);
	Pad1_L_Node = new dModelNode(Pad1_L->GetBody(), dGetIdentityMatrix(), Foot_LNode);

	Pad2_L = new GeomNewton(m_winManager->aManager);
	Pad2_L->SetBodyType(adtDynamic);
	Pad2_L->SetParent(Plantar_L);
	Pad2_L->SetTexture0(&tex[0], "Tex0");
	Pad2_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Pad2_L->SetPitchAngle(90.0f, false);
	Pad2_L->SetPosition( -  w_foot / 2, -  h_sphere, -  (l_foot / 2 - r_Pad));
	Pad2_L->InitNewton(atSphere, r_Pad, r_Pad, r_Pad, 1.0f  );//mass
	NewtonBodySetTransformCallback(Pad2_L->GetBody(), NULL);
	Pad2_L_Node = new dModelNode(Pad2_L->GetBody(), dGetIdentityMatrix(), Foot_LNode);

	Heel_R = new GeomNewton(m_winManager->aManager);
	Heel_R->SetBodyType(adtDynamic);
	Heel_R->SetParent(Plantar_R);
	Heel_R->SetTexture0(&tex[0], "Tex0");
	Heel_R->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Heel_R->SetPitchAngle(90.0f, false);
	Heel_R->SetPosition(0, -  h_sphere,   (l_foot / 2));
	Heel_R->InitNewton(atSphere, r_Pad, r_Pad, r_Pad, 1.0f  );//mass
	NewtonBodySetTransformCallback(Heel_R->GetBody(), NULL);
	Heel_R_Node = new dModelNode(Heel_R->GetBody(), dGetIdentityMatrix(), Foot_RNode);

	Pad1_R = new GeomNewton(m_winManager->aManager);
	Pad1_R->SetBodyType(adtDynamic);
	Pad1_R->SetParent(Plantar_R);
	Pad1_R->SetTexture0(&tex[0], "Tex0");
	Pad1_R->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Pad1_R->SetPitchAngle(90.0f, false);
	Pad1_R->SetPosition(  w_foot / 2, -  h_sphere, -(l_foot / 2 - r_Pad));
	Pad1_R->InitNewton(atSphere, r_Pad, r_Pad, r_Pad, 1.0f  );//mass
	NewtonBodySetTransformCallback(Pad1_R->GetBody(), NULL);
	Pad1_R_Node = new dModelNode(Pad1_R->GetBody(), dGetIdentityMatrix(), Foot_RNode);

	Pad2_R = new GeomNewton(m_winManager->aManager);
	Pad2_R->SetBodyType(adtDynamic);
	Pad2_R->SetParent(Plantar_R);
	Pad2_R->SetTexture0(&tex[0], "Tex0");
	Pad2_R->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Pad2_R->SetPitchAngle(90.0f, false);
	Pad2_R->SetPosition(-  w_foot / 2, -  h_sphere, -(l_foot / 2 - r_Pad));
	Pad2_R->InitNewton(atSphere, r_Pad, r_Pad, r_Pad, 1.0f  );//mass
	NewtonBodySetTransformCallback(Pad2_R->GetBody(), NULL);
	Pad2_R_Node = new dModelNode(Pad2_R->GetBody(), dGetIdentityMatrix(), Foot_RNode);


	//Pad joints

	//Left foot
	dMatrix Heel_L_PinMatrix(dGetIdentityMatrix());
	Heel_L_PinMatrix.m_posit = dVector(0, -  h_sphere,   (l_foot / 2), 1.0f);
	Hl_L = new dCustomDoubleHinge(Heel_L_PinMatrix, Heel_L->GetBody(), Plantar_L->GetBody());
	Hl_L->SetMassIndependentSpringDamper(true,0.01, 1.e20f, 1.e15f);
	Hl_L->SetMassIndependentSpringDamper1(true,0.01, 1.e20f, 1.e15f);
	m_winManager->aManager->vJointList.push_back(Hl_L);


	dMatrix Pad1_L_PinMatrix(dGetIdentityMatrix());
	Pad1_L_PinMatrix.m_posit = dVector(  w_foot / 2, -  h_sphere, -  (l_foot / 4), 1.0f);
	Pd1_L = new dCustomDoubleHinge(Pad1_L_PinMatrix, Pad1_L->GetBody(), Plantar_L->GetBody());
	Pd1_L->SetMassIndependentSpringDamper(true, 0.01, 1.e20f, 1.e15f);
	Pd1_L->SetMassIndependentSpringDamper1(true, 0.01, 1.e20f, 1.e15f);
	m_winManager->aManager->vJointList.push_back(Pd1_L);

	dMatrix Pad2_L_PinMatrix(dGetIdentityMatrix());
	Pad2_L_PinMatrix.m_posit = dVector(-  w_foot / 2, -  h_sphere, -  (l_foot / 4), 1.0f);
	Pd2_L = new dCustomDoubleHinge(Pad2_L_PinMatrix, Pad2_L->GetBody(), Plantar_L->GetBody());
	Pd2_L->SetMassIndependentSpringDamper(true, 0.01, 1.e20f, 1.e15f);
	Pd2_L->SetMassIndependentSpringDamper1(true, 0.01, 1.e20f, 1.e15f);
	m_winManager->aManager->vJointList.push_back(Pd2_L);

	//Right foot
	dMatrix Heel_R_PinMatrix(dGetIdentityMatrix());
	Heel_R_PinMatrix.m_posit = dVector(0, -  h_sphere,   (l_foot / 2), 1.0f);
	Hl_R = new dCustomDoubleHinge(Heel_R_PinMatrix, Heel_R->GetBody(), Plantar_R->GetBody());
	Hl_R->SetMassIndependentSpringDamper(true, 0.01, 1.e20f, 1.e15f);
	Hl_R->SetMassIndependentSpringDamper1(true, 0.01, 1.e20f, 1.e15f);
	m_winManager->aManager->vJointList.push_back(Hl_R);

	dMatrix Pad1_R_PinMatrix(dGetIdentityMatrix());
	Pad1_R_PinMatrix.m_posit = dVector(  w_foot / 2, -  h_sphere, -  (l_foot / 4), 1.0f);
	Pd1_R = new dCustomDoubleHinge(Pad1_R_PinMatrix, Pad1_R->GetBody(), Plantar_R->GetBody());
	Pd1_R->SetMassIndependentSpringDamper(true, 0.01, 1.e20f, 1.e15f);
	Pd1_R->SetMassIndependentSpringDamper1(true, 0.01, 1.e20f, 1.e15f);
	m_winManager->aManager->vJointList.push_back(Pd1_R);

	dMatrix Pad2_R_PinMatrix(dGetIdentityMatrix());
	Pad2_R_PinMatrix.m_posit = dVector(-  w_foot / 2, -  h_sphere, -  (l_foot / 4), 1.0f);
	Pd2_R = new dCustomDoubleHinge(Pad2_R_PinMatrix, Pad2_R->GetBody(), Plantar_R->GetBody());
	Pd2_R->SetMassIndependentSpringDamper(true, 0.01, 1.e20f, 1.e15f);
	Pd2_R->SetMassIndependentSpringDamper1(true, 0.01, 1.e20f, 1.e15f);
	m_winManager->aManager->vJointList.push_back(Pd2_R); 

	//trunk joints

	// create Lumbar joint. 
	dMatrix Spine_PinMatrix(dGetIdentityMatrix()); // define the pin WIP
	Spine_PinMatrix.m_posit = dVector(_Pos.x , _Pos.y +   l_LPT  / 2 +   r_bones, _Pos.z);
	Disk2 = new dCustomDoubleHinge(Spine_PinMatrix, MPT->GetBody(), LPT->GetBody());
	Disk2->SetMassIndependentSpringDamper(true,0.3,  1.e6f, 1.e4f); // VALUES ?
	Disk2->SetMassIndependentSpringDamper1(true,0.3,  1.e6f, 1.e4f); // VALUES ?
	m_winManager->aManager->vJointList.push_back(Disk2);

	// create Neck joint. 
	dMatrix Neck_PinMatrix(dGetIdentityMatrix()); // define the pin WIP
	Neck_PinMatrix.m_posit = dVector(UPT->GetPosition().m_x, UPT->GetPosition().m_y -   l_UPT / 2 -    r_bones, UPT->GetPosition().m_z, 1.0f);
	Nck = new dCustomDoubleHinge(Neck_PinMatrix, UPT->GetBody(), MPT->GetBody());
	Nck->SetMassIndependentSpringDamper(true, 0.01, 1000.f, 10.f); // VALUES ?
	Nck->SetMassIndependentSpringDamper1(true, 0.01, 1000.f, 10.f); // VALUES ?
	m_winManager->aManager->vJointList.push_back(Nck);

	// create Head joint. 
	dMatrix Head_PinMatrix(dYawMatrix(90.0f * dDegreeToRad));
	Head_PinMatrix.m_posit = dVector(Head->GetPosition().m_x, Head->GetPosition().m_y -   l_Head / 2 -    r_bones, Head->GetPosition().m_z, 1.0f);
	Hd = new dCustomDoubleHinge(Head_PinMatrix, Head->GetBody(), UPT->GetBody());
	Hd->SetMassIndependentSpringDamper(true, 0.01, 1000.f, 10.f); // VALUES ?
	Hd->SetMassIndependentSpringDamper1(true, 0.01, 1000.f, 10.f); // VALUES ?
	m_winManager->aManager->vJointList.push_back(Hd);

	// Left upper limb joints

	// create sternum joint.

	dMatrix Sternum_PinMatrix(dYawMatrix(90.0f * dDegreeToRad));

	Sternum_PinMatrix.m_posit = dVector(MPT->GetPosition().m_x +   r_bones, MPT->GetPosition().m_y+   l_MPT / 2, MPT->GetPosition().m_z, 1.0f);
	Strn_L = new dCustomDoubleHinge(Sternum_PinMatrix, Clav_L->GetBody(), MPT->GetBody());
	Strn_L->SetMassIndependentSpringDamper(true, 0.01, 1.e6f, 1.e4f);
	Strn_L->SetMassIndependentSpringDamper1(true, 0.01, 1.e6f, 1.e4f);
	m_winManager->aManager->vJointList.push_back(Strn_L);

	// create Shoulder joint. 
	dMatrix Shoulder_LPinMatrix(dGetIdentityMatrix());
	Shoulder_LPinMatrix = Shoulder_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Shoulder_LPinMatrix.m_posit = dVector(Up_Arm_L->GetPosition().m_x, Up_Arm_L->GetPosition().m_y +   l_Up_Arm / 2, Up_Arm_L->GetPosition().m_z, 1.0f);
	shld_L = new dCustomBallAndSocket(Shoulder_LPinMatrix,Up_Arm_L->GetBody(), Clav_L->GetBody() );
	m_winManager->aManager->vJointList.push_back(shld_L);

	// create elbow joint. 
	dMatrix Elbow_LPinMatrix(dGetIdentityMatrix());
	Elbow_LPinMatrix = Elbow_LPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Elbow_LPinMatrix.m_posit = dVector(Low_Arm_L->GetPosition().m_x, Low_Arm_L->GetPosition().m_y +   l_Low_Arm / 2, Low_Arm_L->GetPosition().m_z, 1.0f);
	Elb_L = new dCustomHinge(Elbow_LPinMatrix, Low_Arm_L->GetBody(), Up_Arm_L->GetBody());
	Elb_L->EnableLimits(true);
	Elb_L->SetLimits(0.f * dDegreeToRad, 180.f);	
	m_winManager->aManager->vJointList.push_back(Elb_L);

	// create wrist joint.
	dMatrix Wrist_LPinMatrix(dGetIdentityMatrix());
	Wrist_LPinMatrix = Wrist_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Wrist_LPinMatrix.m_posit = dVector(Hand_L->GetPosition().m_x, Hand_L->GetPosition().m_y +   l_Hand / 2, Hand_L->GetPosition().m_z, 1.0f);
	Wr_L = new dCustomBallAndSocket(Wrist_LPinMatrix, Hand_L->GetBody(), Low_Arm_L->GetBody());
	m_winManager->aManager->vJointList.push_back(Wr_L);

	// Right upper limb joints

	// create sternum joint. 

	Sternum_PinMatrix.m_posit = dVector(MPT->GetPosition().m_x -   r_bones, MPT->GetPosition().m_y +   l_MPT / 2, MPT->GetPosition().m_z, 1.0f);
	Strn_R = new dCustomDoubleHinge(Sternum_PinMatrix, Clav_R->GetBody(), MPT->GetBody());
	Strn_R->SetMassIndependentSpringDamper(true, 0.01, 1.e6f, 1.e4f);
	Strn_R->SetMassIndependentSpringDamper1(true, 0.01, 1.e6f, 1.e4f);
	m_winManager->aManager->vJointList.push_back(Strn_R);


	// create Shoulder joint. 
	dMatrix Shoulder_RPinMatrix(dGetIdentityMatrix());
	Shoulder_RPinMatrix = Shoulder_RPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Shoulder_RPinMatrix.m_posit = dVector(Up_Arm_R->GetPosition().m_x, Up_Arm_R->GetPosition().m_y +   l_Up_Arm / 2, Up_Arm_R->GetPosition().m_z, 1.0f);
	shld_R = new dCustomBallAndSocket(Shoulder_RPinMatrix, Up_Arm_R->GetBody(),Clav_R->GetBody());
	m_winManager->aManager->vJointList.push_back(shld_R);

	// create elbow joint. 
	dMatrix Elbow_RPinMatrix(dGetIdentityMatrix());
	Elbow_RPinMatrix = Elbow_RPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Elbow_RPinMatrix.m_posit = dVector(Low_Arm_R->GetPosition().m_x, Low_Arm_R->GetPosition().m_y +   l_Low_Arm / 2, Low_Arm_R->GetPosition().m_z, 1.0f);
	Elb_R = new dCustomHinge(Elbow_RPinMatrix, Low_Arm_R->GetBody(), Up_Arm_R->GetBody());
	Elb_R->EnableLimits(true);
	Elb_R->SetLimits(0.f * dDegreeToRad, 180.f);
	m_winManager->aManager->vJointList.push_back(Elb_R);

	// create wrist joint.
	dMatrix Wrist_RPinMatrix(dGetIdentityMatrix());
	Wrist_RPinMatrix = Wrist_RPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Wrist_RPinMatrix.m_posit = dVector(Hand_R->GetPosition().m_x, Hand_R->GetPosition().m_y +   l_Hand / 2, Hand_R->GetPosition().m_z, 1.0f);
	Wr_R = new dCustomBallAndSocket(Wrist_RPinMatrix, Hand_R->GetBody(), Low_Arm_R->GetBody());
	m_winManager->aManager->vJointList.push_back(Wr_R);



	//Left lower limb joints

	// create Sacrum joint. 
	dMatrix Sacrum_PinMatrix(dYawMatrix(90.0f * dDegreeToRad));
	Sacrum_PinMatrix.m_posit = dVector(_Pos.x +   r_bones, _Pos.y -   l_LPT / 2, _Pos.z, 1.0f);
	Disk1_L = new dCustomDoubleHinge(Sacrum_PinMatrix, Hip_L->GetBody(), LPT->GetBody());
	Disk1_L->SetMassIndependentSpringDamper(true,0.01,  1.e6f, 1.e4f);
	Disk1_L->SetMassIndependentSpringDamper1(true, 0.01, 1.e6f, 1.e4f);
	m_winManager->aManager->vJointList.push_back(Disk1_L);

	// create hip joint. 
	dMatrix Hip_LPinMatrix(dGetIdentityMatrix());
	Hip_LPinMatrix = Hip_LPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Hip_LPinMatrix.m_posit = dVector(Up_Leg_L->GetPosition().m_x , Up_Leg_L->GetPosition().m_y +   l_Up_Leg / 2, Up_Leg_L->GetPosition().m_z, 1.0f);
	Rotule_L = new dCustomHinge(Hip_LPinMatrix, Up_Leg_L->GetBody(), Hip_L->GetBody());
	Rotule_L->EnableLimits(true);
	Rotule_L->SetLimits(-40.f * dDegreeToRad,120.f * dDegreeToRad);
	m_winManager->aManager->vJointList.push_back(Rotule_L);

	// create knee joint. 
	dMatrix Knee_LPinMatrix(dGetIdentityMatrix());
	Knee_LPinMatrix = Knee_LPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Knee_LPinMatrix.m_posit = dVector(Low_Leg_L->GetPosition().m_x, Low_Leg_L->GetPosition().m_y +   l_Low_Leg / 2 , Low_Leg_L->GetPosition().m_z, 1.0f);
	Knee_L = new dCustomHinge(Knee_LPinMatrix, Low_Leg_L->GetBody(), Up_Leg_L->GetBody());
	Knee_L->EnableLimits(true);
	Knee_L->SetLimits(-180.f * dDegreeToRad,0.f);
	m_winManager->aManager->vJointList.push_back(Knee_L);

	// create ankle joint.
	dMatrix Ankle_LPinMatrix(dGetIdentityMatrix()); //flexion twist
	//Ankle_LPinMatrix = Ankle_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);//supination twist
	Ankle_LPinMatrix = Ankle_LPinMatrix * dRollMatrix(90.0f * dDegreeToRad);//torsion twist
	Ankle_LPinMatrix.m_posit = dVector(Low_Leg_L->GetPosition().m_x, Low_Leg_L->GetPosition().m_y -   l_Low_Leg / 2-   r_bones, Low_Leg_L->GetPosition().m_z,1.0f);
	Ankle_L = new dCustomBallAndSocket(Ankle_LPinMatrix,  Plantar_L->GetBody(),Low_Leg_L->GetBody());
	Ankle_L->EnableTwist(true);
	Ankle_L->SetTwistLimits(-15.f * dDegreeToRad,15.f * dDegreeToRad);
	Ankle_L->EnableCone(true);
	Ankle_L->SetConeLimits(40.f * dDegreeToRad );
	m_winManager->aManager->vJointList.push_back(Ankle_L);

	// create toe joint.
	dMatrix Toe_LPinMatrix(dGetIdentityMatrix());
	Toe_LPinMatrix = Toe_LPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Toe_LPinMatrix.m_posit = dVector(Plantar_L->GetPosition().m_x, Plantar_L->GetPosition().m_y, Plantar_L->GetPosition().m_z-   l_foot/2, 1.0f);
	Flextoe_L = new dCustomHinge(Toe_LPinMatrix, Plantar_L->GetBody(), Toe_L->GetBody());
	Flextoe_L->SetAsSpringDamper(true,  1.e3f, 1.e2f);
	m_winManager->aManager->vJointList.push_back(Flextoe_L);

	//Right lower limb joints
	
	// create Sacrum joint. 
	Sacrum_PinMatrix.m_posit = dVector(_Pos.x -   r_bones, _Pos.y -   l_LPT / 2, _Pos.z);
	Disk1_R = new dCustomDoubleHinge(Sacrum_PinMatrix, Hip_R->GetBody(), LPT->GetBody());
	Disk1_R->SetMassIndependentSpringDamper(true, 0.01, 1.e6f, 1.e4f);
	Disk1_R->SetMassIndependentSpringDamper1(true, 0.01, 1.e6f, 1.e4f);
	m_winManager->aManager->vJointList.push_back(Disk1_L);

	// create hip joint. 
	dMatrix Hip_RPinMatrix(dGetIdentityMatrix());
	Hip_RPinMatrix = Hip_RPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Hip_RPinMatrix.m_posit = dVector(Up_Leg_R->GetPosition().m_x , Up_Leg_R->GetPosition().m_y +   l_Up_Leg / 2, Up_Leg_R->GetPosition().m_z, 1.0f);
	Rotule_R = new dCustomDoubleHinge(Hip_RPinMatrix, Up_Leg_R->GetBody(), Hip_R->GetBody());
	Rotule_R->EnableLimits(true);
	Rotule_R->EnableLimits1(true);
	Rotule_R->SetLimits(-40.f * dDegreeToRad, 120.f * dDegreeToRad);
	Rotule_R->SetLimits1(-30.f * dDegreeToRad, 5.0f * dDegreeToRad);
	m_winManager->aManager->vJointList.push_back(Rotule_R);

	// create knee joint. 
	dMatrix Knee_RPinMatrix(dGetIdentityMatrix());
	Knee_RPinMatrix = Knee_RPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Knee_RPinMatrix.m_posit = dVector(Low_Leg_R->GetPosition().m_x, Low_Leg_R->GetPosition().m_y +   l_Low_Leg / 2 , Low_Leg_R->GetPosition().m_z, 1.0f);
	Knee_R = new dCustomHinge(Knee_RPinMatrix, Low_Leg_R->GetBody(), Up_Leg_R->GetBody());
	Knee_R->EnableLimits(true);
	Knee_R->SetLimits(-180.f * dDegreeToRad, 0);
	m_winManager->aManager->vJointList.push_back(Knee_R);

	// create ankle joint.
	dMatrix Ankle_RPinMatrix(dGetIdentityMatrix());
	Ankle_RPinMatrix = Ankle_RPinMatrix * dRollMatrix(90.0f * dDegreeToRad);
	Ankle_RPinMatrix.m_posit = dVector(Low_Leg_R->GetPosition().m_x, Low_Leg_R->GetPosition().m_y -   l_Low_Leg / 2-   r_bones, Low_Leg_R->GetPosition().m_z, 1.0f);
	Ankle_R = new dCustomBallAndSocket(Ankle_RPinMatrix, Plantar_R->GetBody(), Low_Leg_R->GetBody());
	Ankle_R->EnableTwist(true);
	Ankle_R->SetTwistLimits(-15.f * dDegreeToRad, 15.f * dDegreeToRad);
	Ankle_R->EnableCone(true);
	Ankle_R->SetConeLimits(40.f * dDegreeToRad);
	m_winManager->aManager->vJointList.push_back(Ankle_R);

	// create toe joint.
	dMatrix Toe_RPinMatrix(dGetIdentityMatrix());
	Toe_RPinMatrix = Toe_RPinMatrix * dPitchMatrix(90.0f * dDegreeToRad);
	Toe_RPinMatrix.m_posit = dVector(Plantar_R->GetPosition().m_x, Plantar_R->GetPosition().m_y, Plantar_R->GetPosition().m_z -   l_foot/2 ,1.0f);
	Flextoe_R = new dCustomHinge(Toe_RPinMatrix, Plantar_R->GetBody(), Toe_R->GetBody());
	Flextoe_R->SetAsSpringDamper(true,  1.e3f, 1.e2f);
	m_winManager->aManager->vJointList.push_back(Flextoe_R);
	
	// MUSCLE DEFINITION

	// FOOT LINES
	FootLineIndex_L = CreateFootScanLine();
	FootLineIndex_R = CreateFootScanLine();
		
		
	////The Origin point is the one that is closer to the trunk
	////EX: SOL muscle Creation..Origin Point: Low_Leg, Insertion Point: Plantar
	//m_sol_L = new Muscle(m_winManager->aLineManager, m_winManager->aManager, Low_Leg_L, Plantar_L, dVector(v_x1[0], v_y1[0], v_z1[0]), dVector(v_x2[0], v_y2[0], v_z2[0]));
	//m_sol_L->GenerateMesh();
	//m_winManager->aManager->vMuscleList.push_back(m_sol_L);

	// MUSCLE wang

	////EX: GLU muscle Creation: input insertion body and actuated joint
	//m_hfl2_L = new MuscleV2(m_winManager->aLineManager, m_winManager->aManager, Up_Leg_L, Rotule_L);
	//m_winManager->aManager->vMuscleV2List.push_back(m_glu_L);
	//m_hfl2_L->SetThetazero(phi_hl * dDegreeToRad);
}
MuscleV2* dRaycastVHModel::Gethfl2_L()
{
	return m_hfl2_L;
}
dCustomDoubleHinge* dRaycastVHModel::GetRotule_R()
{
	return Rotule_R;
}
dCustomHinge* dRaycastVHModel::GetRotule_L()
{
	return Rotule_L;
}
dCustomHinge* dRaycastVHModel::GetKnee_L()
{
	return Knee_L;
}

dCustomBallAndSocket* dRaycastVHModel::GetAnkle_L()
{
	return Ankle_L;
}

dCustomHinge* dRaycastVHModel::GetKnee_R()
{
	return Knee_R;
}

dCustomBallAndSocket* dRaycastVHModel::GetAnkle_R()
{
	return Ankle_R;
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

GeomNewton* dRaycastVHModel::GetHip_L() {
	return Hip_L;
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
	//cout << "DGVehicleRCManager OnPreUpdate \n";

	dVector Vtemp(0.0f);

	// scan all  Muscle Elements
	//for (auto itr = m_winManager->aManager->vMuscleList.begin();
	//	itr != m_winManager->aManager->vMuscleList.end(); itr++)
	//{
	//	Muscle* Mobj = (Muscle*)*itr;

	//	// Get the Body1 connected to the muscle and apply the muscle force
	//	GeomNewton* gNewton = (GeomNewton*)(Mobj->body1);
	//	NewtonBody* NBody = gNewton->GetBody();
	//	//Mobj->SetStepSize(1/1000.0f); //1000 Hz or 1/1000 s
	//	Mobj->SetExcitation(0.0);
	//	Vtemp = Mobj->GetForceElas(timestep);
	//	//cout << timestep << ' ' << Vtemp[0] << ' ' << Vtemp[1] << ' ' << Vtemp[2] << '\n';
	//	//dVector P = Mobj->GetInsert1_GlobalRef(); // Check
	//	dVector vOrigin(0.0f), vInsert(0.0f);
	//	Mobj->GetOriginAndInsertion(vOrigin, vInsert);
	//	AddForceAtRelPos(NBody, &Vtemp[0], &vOrigin[0]);

	//	// Get the Body2 connected and apply the opposite muscle force
	//	gNewton = (GeomNewton*)(Mobj->body2);
	//	NBody = (NewtonBody*)gNewton->GetBody();
	//	Vtemp = Vtemp.Scale(-1.0f);
	//	//P = Mobj->GetInsert2_GlobalRef(); // Check
	//	AddForceAtRelPos(NBody, &Vtemp[0], &vInsert[0]);

	//	//monFlux << newTime << "  " << Mobj->fSE(Mobj->GetDelta_l()) << "  " << Mobj->fCE(Mobj->GetDelta_l(), timestep) << "  " << Mobj->fPE(Mobj->GetDelta_l()) << "  " << Mobj->GetNmax() << "  " << Mobj->GetLCE() << "  " << Mobj->GetDelta_l() << std::endl;

	//	Mobj->SetLCE(Mobj->GetLCE() + Mobj->GetDelta_l());
	//}
	// EX Geyer's muscle WIP
	//MuscleV2* mobj = controller->Gethfl2_L();
	//double Ttemp = mobj->Compute_muscle_Torque(timestep);
	//GeomNewton* mbody1 = controller->GetUp_Leg_L();
	//GeomNewton* mbody2 = controller->GetHip_L();

	//dCustomHinge* joint = controller->GetRotule_L();
	//dFloat ang = joint->GetJointAngle();

	//dVector T(0.0f, 0.0f, 0.0f);
	//T.m_x = Ttemp - double(200.f);
	//NewtonBodyAddTorque(mbody1->GetBody(), &T.m_x);
	//T.Scale(-1); // opposite force on second body
	//NewtonBodyAddTorque(mbody2->GetBody(), &T.m_x);

	//double theta(0.0f), lce(0.0f), Fmtu(0.0f);
	//mobj->GetMuscleParams(theta, lce, Fmtu);
	//newTime = newTime + timestep;
	//monFlux << newTime << '\t' << theta << '\t' << lce << '\t' << T.m_x << std::endl;
	
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



