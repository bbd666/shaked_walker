#include "pch.h"
#include "OXVehicleRayCast.h"
#include <tinyxml.h>


dFloat newTime = 0;
ofstream monFlux("history.txt");
// ----------------------------------------------------------------------
// STDOUT dump and indenting utility functions
// ----------------------------------------------------------------------

const char* dRaycastVHModel::getIndent(unsigned int numIndents)
{
    static const char* pINDENT = "                                      + ";
    static const unsigned int LENGTH = strlen(pINDENT);
    unsigned int n = numIndents * 2;
    if (n > LENGTH) n = LENGTH;

    return &pINDENT[LENGTH - n];
}

// same as getIndent but no "+" at the end
const char* dRaycastVHModel::getIndentAlt(unsigned int numIndents)
{
    static const char* pINDENT = "                                        ";
    static const unsigned int LENGTH = strlen(pINDENT);
    unsigned int n = numIndents * 2;
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
        if (pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS) printf("%s%s: value=[%1.4f] \n", pIndent, pAttrib->Name(), dval);
        
        vector.push_back(dval);
        i++;
        pAttrib = pAttrib->Next();
    }
    return i;
}

int dRaycastVHModel::dump_attribs_to_stdout2(TiXmlElement* pElement, std::map<std::string, float>& l, unsigned int indent)
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
        if (pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS) printf("%s%s: value=[%1.4f] \n", pIndent, pAttrib->Name(), dval);

        l.find(pAttrib->Name())->second = dval;
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
            num = dump_attribs_to_stdout2(pParent->ToElement(), lengths, indent + 1);
        }
        else if (sParameter == "Weight")
        {
            num = dump_attribs_to_stdout(pParent->ToElement(), v_total_weight, indent + 1);
        }
        else if (sParameter == "Masses")
        {
            num = dump_attribs_to_stdout2(pParent->ToElement(), mass_distrib, indent + 1);
        }
        else if (sParameter == "Ixx")
        {
            num = dump_attribs_to_stdout2(pParent->ToElement(), Ixx, indent + 1);
        }
        else if (sParameter == "Iyy")
        {
            num = dump_attribs_to_stdout2(pParent->ToElement(), Iyy, indent + 1);
        }
        else if (sParameter == "Izz")
        {
            num = dump_attribs_to_stdout2(pParent->ToElement(), Izz, indent + 1);
        }
        else if (sParameter == "COM")
        {
            num = dump_attribs_to_stdout2(pParent->ToElement(), delta_cm, indent + 1);
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
        else if (sParameter == "Sol_p")
        {
            num = dump_attribs_to_stdout2(pParent->ToElement(), sol, indent + 1);
        }
        else if (sParameter == "Ta_p")
        {
            num = dump_attribs_to_stdout2(pParent->ToElement(), ta, indent + 1);
        }
        else if (sParameter == "Vas_p")
        {
            num = dump_attribs_to_stdout2(pParent->ToElement(), vas, indent + 1);
        }
        else if (sParameter == "Gas_p")
        {
            num = dump_attribs_to_stdout2(pParent->ToElement(), gas, indent + 1);
        }
        else if (sParameter == "Hfl_p")
        {
            num = dump_attribs_to_stdout2(pParent->ToElement(), hfl, indent + 1);
        }
        else if (sParameter == "Ham_p")
        {
            num = dump_attribs_to_stdout2(pParent->ToElement(), ham, indent + 1);
        }
        else if (sParameter == "Rf_p")
        {
            num = dump_attribs_to_stdout2(pParent->ToElement(), rf, indent + 1);
        }
        else if (sParameter == "Glu_p")
        {
            num = dump_attribs_to_stdout2(pParent->ToElement(), glu, indent + 1);
        }
        else
        {
            printf("No attributes \n");
        }

        switch (num)
        {
        case 0:  break;
        case 1:  printf("%s1 attribute \n", getIndentAlt(indent)); break;
        default: printf("%s%d attributes \n", getIndentAlt(indent), num); break;
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
// ----------------------------------------------------------------------
// MODEL CLASS DEFINITION
// ----------------------------------------------------------------------
dRaycastVHModel::~dRaycastVHModel()
{
     
}

dRaycastVHModel::dRaycastVHModel(WindowMain* winctx, const char* const modelName, const dMatrix& location, int linkMaterilID)
    : dModelRootNode(NULL, dGetIdentityMatrix())
    , m_winManager(winctx)

{
    //key initialization of the maps 'lengths', 'mass_distrib', 'Ixx', 'Iyy' and 'Izz'
    for (std::vector<std::string>::iterator it = lengths_keys.begin(); it != lengths_keys.end(); it++)
    {		lengths[*it] = 0.f;	    };
    for (std::vector<std::string>::iterator it = mass_keys.begin(); it != mass_keys.end(); it++)
    {		
        mass_distrib[*it] = 0.f;
        Ixx[*it] = 0.f;
        Iyy[*it] = 0.f;
        Izz[*it] = 0.f;
        delta_cm[*it] = 0.f;
    };
    for (std::vector<std::string>::iterator it = muscle_p_keys.begin(); it != muscle_p_keys.end(); it++)
    {
        sol[*it] = 0.0f;
        ta[*it] = 0.0f;
        vas[*it] = 0.0f;
        gas[*it] = 0.0f;
        hfl[*it] = 0.0f;
        ham[*it] = 0.0f;
        rf[*it] = 0.0f;
        glu[*it] = 0.0f;
    };

    // LOADING DATA FROM XML//
    TiXmlDocument doc("DummyGeometricProperties.xml");
    bool loadOkay = doc.LoadFile();
    (dump_to_stdout(&doc)); //parse the xml and load parameters
    if (v_scale.size() != 2 || lengths.size() != 19 || v_total_weight.size() != 1 || mass_distrib.size() != 12 || 
        Ixx.size() != 12 || Iyy.size() != 12 || Izz.size() != 12 || delta_cm.size() != 12 || v_angles.size() != 2)
        std::cout << "ERROR IN XML" << std::endl;
    // Saving data in the program//
    string tex("Textures//wood6.png");

    // Scale factor for length and mass
    scale_length = v_scale[0]; 
    scale_mass = v_scale[1];

    // LENGTH DATA
    l_Head		= scale_length * lengths["Head"];
    l_Up_Arm	= scale_length * lengths["UpArm"];
    l_Low_Arm	= scale_length * lengths["ForeArm"];  // forearm
    l_Hand		= scale_length * lengths["Hand"];
    l_trunk		= scale_length * lengths["Trunk"];
    l_Up_Leg	= scale_length * lengths["Thigh"];  // thigh
    l_Low_Leg	= scale_length * lengths["Shank"];  // shank
    l_foot		= scale_length * lengths["Foot"];
    l_toe		= scale_length * lengths["Toes"];
    l_UPT		= scale_length * lengths["UPT"];  // upper part trunk
    l_MPT		= scale_length * lengths["MPT"]; // medium part trunk
    l_LPT		= scale_length * lengths["LPT"]; // lower part trunk
    l_Hip		= scale_length * lengths["Hip"];
    l_Clav		= scale_length * lengths["Shoulder"];	// shoulder 
    w_foot		= scale_length * lengths["FootBreadth"]; // foot breadth
    h_foot		= scale_length * lengths["AnkleHeight"]; // ankle height
    l_neck		= scale_length * lengths["Neck"];
    l_delta		= scale_length * (0.0345- 0.0213); // distance from acromion to shoulder joint (arcomion position computed relative to proximal point of UPT 553.2-531.9 mm)
    r_bones		= scale_length * lengths["RadBones"];
    r_Pad		= w_foot / 4.0f;
    h_sphere	= scale_length * lengths["RadSpheres"];
    l_foot		= l_foot - l_toe;
    ankle_j		= scale_length * 0.046f * sin(49 * dDegreeToRad); // [m] distance from heel to ankle joint in z direction ((Isman and Inman, 1969; Lee et al., 2011; Zatsiorsky, 2002)

    // MASS DATA in kg
    float tot_w = scale_mass * v_total_weight[0]; 
    for (map<std::string, float>::iterator it = mass_distrib.begin(); it != mass_distrib.end(); it++)
    {
        it->second = it->second * tot_w;
    };
    //INERTIA DATA in kg*m^2
    for (map<std::string, float>::iterator it = Ixx.begin(); it != Ixx.end(); it++)
    {
        it->second = mass_distrib.find(it->first)->second * pow(it->second*scale_length * lengths.find(it->first)->second,2);
    };
    for (map<std::string, float>::iterator it = Iyy.begin(); it != Iyy.end(); it++)
    {
        it->second = mass_distrib.find(it->first)->second * pow(it->second * scale_length * lengths.find(it->first)->second, 2);
    };
    for (map<std::string, float>::iterator it = Izz.begin(); it != Izz.end(); it++)
    {
        it->second = mass_distrib.find(it->first)->second * pow(it->second * scale_length * lengths.find(it->first)->second, 2);
    };

    // COM distance from the origin of each body (DIRECTION X: IN LOCAL COORDINATE OF THE BODY)
    for (map<std::string, float>::iterator it = delta_cm.begin(); it != delta_cm.end(); it++)
    {
        it->second = lengths.find(it->first)->second * (0.5f-it->second);
    };

    float alfa = 30 * dDegreeToRad;
    float alfa1 = 0 * dDegreeToRad;
    float gamma = 10 * dDegreeToRad;
    float beta = -10 * dDegreeToRad;

    // INITIAL POSITION  OF THE DUMMY IN THE SCENE X (lateral, + left) Y(vertical, + sky) Z(front, + back)
    dVector _Pos0(dVector(0.0f, 0.115 +l_LPT / 2 + l_Up_Leg + l_Low_Leg + h_foot, -0.1f)); // LPT
    dVector _Pos1(dVector(l_Hip/2, -l_LPT / 2 - (l_Up_Leg / 2) * cos(alfa), -(l_Up_Leg / 2) * sin(alfa)));// Thigh_r
    dVector _Pos2(dVector(0.0f, -(l_Up_Leg / 2) * cos(alfa) - (l_Low_Leg / 2) * cos(gamma), -(l_Up_Leg / 2) * sin(alfa) + (l_Low_Leg / 2) * sin(gamma)));// Shank_r 
    dVector _Pos3(dVector(0, -(l_Low_Leg / 2) * cos(gamma)-h_foot/2, (l_Low_Leg / 2) * sin(gamma) - l_foot / 3));// Foot_r and l

    dVector _Pos4(dVector(-l_Hip / 2, -l_LPT / 2 - (l_Up_Leg / 2) * cos(alfa1), -(l_Up_Leg / 2) * sin(alfa1)));// Thigh_l
    dVector _Pos5(dVector(0.0f, -(l_Up_Leg / 2) * cos(alfa1) - (l_Low_Leg / 2) * cos(gamma), -(l_Up_Leg / 2) * sin(alfa1) + (l_Low_Leg / 2) * sin(gamma)));// Shank_l

    dVector _Pos6(dVector(0.0f, l_LPT / 2 + l_MPT / 2, 0.0f));// MPT
    dVector _Pos7(dVector(0.0f, l_MPT / 2 + l_UPT / 2, 0.0f));// UPT


    std::vector< dVector* > body_pos_vec = { &_Pos0, &_Pos1, &_Pos2, &_Pos3,&_Pos4, &_Pos5, &_Pos3, &_Pos6, &_Pos7 };// vector with coordinates of bodies' com
    
    dVector dim0(dVector(r_bones, r_bones, l_LPT));
    dVector dim1(dVector(r_bones, r_bones, l_Up_Leg));// Thigh_r and l
    dVector dim2(dVector(r_bones, r_bones, l_Low_Leg));// Shank_r and l
    dVector dim3(dVector(h_foot, h_foot, l_foot));// Foot_r and l
    dVector dim4(dVector(r_bones, r_bones, l_MPT));// MPT
    dVector dim5(dVector(r_bones, r_bones, l_UPT));// UPT

    std::vector< dVector* >body_dim_vec = { &dim0, &dim1, &dim2, &dim3, &dim1, &dim2, &dim3, &dim4, &dim5 };// vector with bodies' dimensions

    child_father = { {"Thigh_r", "LPT"}, {"Shank_r", "Thigh_r"}, {"Foot_r", "Shank_r"},
        {"Thigh_l", "LPT"}, {"Shank_l", "Thigh_l"}, {"Foot_l", "Shank_l"},
        {"MPT", "LPT"}, {"UPT", "MPT"} };// list with body child and father relationship

    // BODIES OF THE DUMMY DEFINITION
    std:vector<float> body_rot_ang_vect = {90.0f ,-alfa * dRadToDegree, alfa * dRadToDegree+(gamma) * dRadToDegree,beta * dRadToDegree,// angles of each body LPT Thigh_r Shank_r Foot_r
                                        -alfa1 * dRadToDegree, alfa1* dRadToDegree + (gamma)* dRadToDegree,beta* dRadToDegree,//Thigh_l Shank_l Foot_l
                                           0.0f, 0.0f};// MPT, UPT
    int aa = 0;
    for (std::vector<std::string>::iterator it = body_keys.begin(); it != body_keys.end(); it++)
    {
        body_rot_ang[*it] = body_rot_ang_vect[aa];
        body_pos[*it] = body_pos_vec[aa];
        body_dim[*it] = body_dim_vec[aa];
        rigid_element[*it] = NULL;
        nodes[*it] = this;
        aa++;
    };
    
    int ind = 0;
    for (std::vector<std::string>::iterator it = body_keys.begin(); it != body_keys.end(); it++)
    {
        std::string mass_prop_key;
        if (*it == "Foot_r" || *it == "Foot_l")
            mass_prop_key = "Foot";
        else if (*it == "Shank_r" || *it == "Shank_l")
            mass_prop_key = "Shank";
        else if (*it == "Thigh_r" || *it == "Thigh_l")
            mass_prop_key = "Thigh";
        else if (*it == "LPT")
            mass_prop_key = "LPT";
        else if (*it == "MPT")
            mass_prop_key = "MPT";
        else if (*it == "UPT")
            mass_prop_key = "UPT";
        //else if (*it == altri corpi rigidi")
        //    mass_prop_key == "MPT";

        rigid_element[*it] = new GeomNewton(m_winManager->aManager);
        rigid_element[*it]->SetBodyType(adtDynamic);
        if (body_keys[ind] != "LPT")
            rigid_element[*it]->SetParent(rigid_element[child_father.find(body_keys[ind])->second]);

        rigid_element[*it]->SetTexture0(&tex[0], "Tex0");
        rigid_element[*it]->SetDiffuseColor(1.0f, 1.0f, 1.0f);

        if (*it =="LPT")
            rigid_element[*it]->SetRollAngle(body_rot_ang[*it], false);
        else
            rigid_element[*it]->SetTurnAngle(body_rot_ang[*it], false);

        rigid_element[*it]->SetPosition(body_pos[*it]->m_x, body_pos[*it]->m_y, body_pos[*it]->m_z);
        if (*it == "Foot_r" || *it == "Foot_l")
            rigid_element[*it]->InitNewton(atBox, body_dim[*it]->m_x, body_dim[*it]->m_y, body_dim[*it]->m_z, 1.0f);
        else
            rigid_element[*it]->InitNewton(atCapsule, body_dim[*it]->m_x, body_dim[*it]->m_y, body_dim[*it]->m_z, 1.0f);

        //if (body_keys[ind] == "Thigh_l")
        //    NewtonBodySetMassMatrix(rigid_element[*it]->GetBody(), 0, Ixx[mass_prop_key], Iyy[mass_prop_key], Izz[mass_prop_key]);
        //else
            NewtonBodySetMassMatrix(rigid_element[*it]->GetBody(), mass_distrib[mass_prop_key], Ixx[mass_prop_key], Iyy[mass_prop_key], Izz[mass_prop_key]);
        
        if (body_keys[ind] == "LPT")
        {
            m_body = rigid_element["LPT"]->GetBody();
            NewtonBodySetTransformCallback(m_body, NULL);
        }
        else
            NewtonBodySetTransformCallback(rigid_element[*it]->GetBody(), NULL);

        if (body_keys[ind] != "LPT")
            nodes[*it] = new dModelNode(rigid_element[*it]->GetBody(), dGetIdentityMatrix(), nodes[child_father.find(body_keys[ind])->second]);

        ind++;
    }
    // joint lists initialization
    for (std::vector<std::string>::iterator it = DHjoint_keys.begin(); it != DHjoint_keys.end(); it++)
        JDoubleHinge[*it] = NULL;
    for (std::vector<std::string>::iterator it = Hjoint_keys.begin(); it != Hjoint_keys.end(); it++)
        JHinge[*it] = NULL;

    /// Right leg joints
    GeomNewton* b1 = rigid_element.find("Shank_r")->second;
    GeomNewton* b2 = rigid_element.find("Foot_r")->second;
    dMatrix Ankle_PinMatrix(dGetIdentityMatrix()); // define the pin
    //Ankle_PinMatrix = Ankle_PinMatrix * dYawMatrix(90.0f * dDegreeToRad);
    Ankle_PinMatrix.m_posit = dVector(b1->GetPosition().m_x, b1->GetPosition().m_y - (l_Low_Leg / 2) * cos(gamma), b1->GetPosition().m_z+ (l_Low_Leg / 2) * sin(gamma), 1.0f);
    Ankle_r = new dCustomDoubleHinge(Ankle_PinMatrix, b1->GetBody(), b2->GetBody());
    ////Ankle_r->EnableLimits(true);
    Ankle_r->EnableLimits1(true);
    ////Ankle_r->SetLimits(-0.1f, 0.1f);
    Ankle_r->SetLimits1(-0.1f, 0.1f);
    m_winManager->aManager->vJointList.push_back(Ankle_r);
    m_winManager->aManager->vJointNameList.push_back(ANKLE_R);
    JDoubleHinge.find("Ankle_r")->second = Ankle_r;

    // EX. HINGE joint. 
    b1 = rigid_element.find("Thigh_r")->second;
    b2 = rigid_element.find("Shank_r")->second;
    dMatrix Knee_PinMatrix(dGetIdentityMatrix()); // define the pin
    Knee_PinMatrix.m_posit = dVector(b1->GetPosition().m_x, b1->GetPosition().m_y - (l_Up_Leg / 2) * cos(alfa), b1->GetPosition().m_z - (l_Up_Leg / 2) * sin(alfa), 1.0f);
    Knee_r = new dCustomHinge(Knee_PinMatrix, b1->GetBody(), b2->GetBody());
    m_winManager->aManager->vJointList.push_back(Knee_r);
    m_winManager->aManager->vJointNameList.push_back(KNEE_R);
    JHinge.find("Knee_r")->second = Knee_r;

    // EX. DOUBLE HINGE joint. 
    b1 = rigid_element.find("LPT")->second;
    b2 = rigid_element.find("Thigh_r")->second;
    dMatrix Hip_PinMatrix(dGetIdentityMatrix()); // define the pin
    Hip_PinMatrix = Hip_PinMatrix * dPitchMatrix(90.0f * dDegreeToRad); // DEFINE THE CORRECT DOF OF A DOUBLEHINGE (the second pin must be the one actuated by the muscle)
    Hip_PinMatrix.m_posit = dVector(b2->GetPosition().m_x, b1->GetPosition().m_y - l_LPT / 2, b1->GetPosition().m_z, 1.0f);
    Hip_r = new dCustomDoubleHinge(Hip_PinMatrix, b1->GetBody(), b2->GetBody());
    Hip_r->EnableLimits1(true);
    Hip_r->SetLimits1(-0.1f, 0.1f);
    m_winManager->aManager->vJointList.push_back(Hip_r);
    m_winManager->aManager->vJointNameList.push_back(HIP_R);
    JDoubleHinge.find("Hip_r")->second = Hip_r;

    // Left leg joints
    b1 = rigid_element.find("Shank_l")->second;
    b2 = rigid_element.find("Foot_l")->second;
    dMatrix Anklel_PinMatrix(dGetIdentityMatrix()); // define the pin
    //Ankle_PinMatrix = Ankle_PinMatrix * dYawMatrix(90.0f * dDegreeToRad);
    Anklel_PinMatrix.m_posit = dVector(b1->GetPosition().m_x, b1->GetPosition().m_y - (l_Low_Leg / 2) * cos(gamma), b1->GetPosition().m_z + (l_Low_Leg / 2) * sin(gamma), 1.0f);
    Ankle_l = new dCustomDoubleHinge(Anklel_PinMatrix, b1->GetBody(), b2->GetBody());
    ////Ankle_l->EnableLimits(true);
    Ankle_l->EnableLimits1(true);
    ////Ankle_l->SetLimits(-0.1f, 0.1f);
    Ankle_l->SetLimits1(-0.01f, 0.01f);
    m_winManager->aManager->vJointList.push_back(Ankle_l);
    m_winManager->aManager->vJointNameList.push_back(ANKLE_L);
    JDoubleHinge.find("Ankle_l")->second = Ankle_l;

    // EX. HINGE joint. 
    b1 = rigid_element.find("Thigh_l")->second;
    b2 = rigid_element.find("Shank_l")->second;
    dMatrix Kneel_PinMatrix(dGetIdentityMatrix()); // define the pin
    Kneel_PinMatrix.m_posit = dVector(b1->GetPosition().m_x, b1->GetPosition().m_y - (l_Up_Leg / 2) * cos(alfa1), b1->GetPosition().m_z - (l_Up_Leg / 2) * sin(alfa1), 1.0f);
    Knee_l = new dCustomHinge(Kneel_PinMatrix, b1->GetBody(), b2->GetBody());
    m_winManager->aManager->vJointList.push_back(Knee_l);
    m_winManager->aManager->vJointNameList.push_back(KNEE_L);
    JHinge.find("Knee_l")->second = Knee_l;

    // EX. DOUBLE HINGE joint. 
    b1 = rigid_element.find("LPT")->second;
    b2 = rigid_element.find("Thigh_l")->second;
    dMatrix Hipl_PinMatrix(dGetIdentityMatrix()); // define the pin
    Hipl_PinMatrix = Hipl_PinMatrix * dPitchMatrix(90.0f * dDegreeToRad); // DEFINE THE CORRECT DOF OF A DOUBLEHINGE (the second pin must be the one actuated by the muscle)
    Hipl_PinMatrix.m_posit = dVector(b2->GetPosition().m_x, b1->GetPosition().m_y - l_LPT / 2, b1->GetPosition().m_z, 1.0f);
    Hip_l = new dCustomDoubleHinge(Hipl_PinMatrix, b1->GetBody(), b2->GetBody());
    Hip_l->EnableLimits1(true);
    Hip_l->SetLimits1(-0.1f, 0.1f);
    m_winManager->aManager->vJointList.push_back(Hip_l);
    m_winManager->aManager->vJointNameList.push_back(HIP_L);
    JDoubleHinge.find("Hip_l")->second = Hip_l;

    // EX. spine joint 1. 
    b1 = rigid_element.find("LPT")->second;
    b2 = rigid_element.find("MPT")->second;
    dMatrix spine1_PinMatrix(dGetIdentityMatrix()); // define the pin
    spine1_PinMatrix.m_posit = dVector(b1->GetPosition().m_x, b1->GetPosition().m_y + l_LPT/2, b1->GetPosition().m_z, 1.0f);
    spine1 = new dCustomHinge(spine1_PinMatrix, b1->GetBody(), b2->GetBody());
    spine1->EnableLimits(true);
    spine1->SetLimits(-0.01f, 0.01f);
    m_winManager->aManager->vJointList.push_back(spine1);
    //m_winManager->aManager->vJointNameList.push_back(KNEE_L);
    JHinge.find("Spine1")->second = spine1;

    // EX. spine joint 2. 
    b1 = rigid_element.find("MPT")->second;
    b2 = rigid_element.find("UPT")->second;
    dMatrix spine2_PinMatrix(dGetIdentityMatrix()); // define the pin
    spine2_PinMatrix.m_posit = dVector(b1->GetPosition().m_x, b1->GetPosition().m_y + l_MPT / 2, b1->GetPosition().m_z, 1.0f);
    spine2 = new dCustomHinge(spine1_PinMatrix, b1->GetBody(), b2->GetBody());
    spine2->EnableLimits(true);
    spine2->SetLimits(-0.01f, 0.01f);
    m_winManager->aManager->vJointList.push_back(spine2);
    //m_winManager->aManager->vJointNameList.push_back(KNEE_L);
    JHinge.find("Spine2")->second = spine2;
    
    //////// MUSCLE DEFINITION
    m_body1 = { {"hfl_r", "LPT"},   {"hfl_l", "LPT"},
        {"glu_r", "LPT"},           {"glu_l", "LPT"},
        {"ham_r", "LPT"},           {"ham_l", "LPT"},
        {"rf_r", "LPT"},            {"rf_l", "LPT"},
        {"vas_r", "Thigh_r"},       {"vas_l", "Thigh_l"},
        {"sol_r", "Shank_r"},       {"sol_l", "Shank_l"},
        {"ta_r", "Shank_r"},        {"ta_l", "Shank_l"},
        {"gas_r", "Thigh_r"},       {"gas_l", "Thigh_l"} };// list muscle bodies 1
    m_body2 = { {"hfl_r", "Thigh_r"},   {"hfl_l", "Thigh_l"},
        {"glu_r", "Thigh_r"},           {"glu_l", "Thigh_l"},
        {"ham_r", "Thigh_r"},           {"ham_l", "Thigh_l"},
        {"rf_r", "Thigh_r"},            {"rf_l", "Thigh_l"},
        {"vas_r", "Shank_r"},           {"vas_l", "Shank_l"},
        {"sol_r", "Foot_r"},            {"sol_l", "Foot_l"},
        {"ta_r", "Foot_r"},             {"ta_l", "Foot_l"},
        {"gas_r", "Shank_r"},           {"gas_l", "Shank_l"} };// list muscle bodies 2
    m_body3 = { {"hfl_r", ""},  {"hfl_l", ""},
        {"glu_r", ""} ,         {"glu_l", ""} ,
        {"ham_r", "Shank_r"},   {"ham_l", "Shank_l"},
        {"rf_r", "Shank_r"},    {"rf_l", "Shank_l"},
        {"vas_r", ""},          {"vas_l", ""},
        {"sol_r", ""},          {"sol_l", ""},
        {"ta_r", ""},           {"ta_l", ""},
        {"gas_r", "Foot_r"},    {"gas_l", "Foot_l"} };// list muscle bodies 3
    m_point1 = { {"hfl_r", dVector(-l_LPT / 2, -l_Hip/2, -4 * r_bones)},    {"hfl_l", dVector(-l_LPT / 2, l_Hip / 2, -4 * r_bones)},
        {"glu_r", dVector(-l_LPT / 2, -l_Hip / 2, 4 * r_bones)},            {"glu_l", dVector(-l_LPT / 2, l_Hip / 2, 4 * r_bones)},
        {"ham_r",dVector(-l_LPT / 2, -l_Hip / 2, 4 * r_bones)},             {"ham_l",dVector(-l_LPT / 2,  l_Hip / 2, 4 * r_bones)},
        {"rf_r",dVector(-l_LPT / 2, -l_Hip / 2, -4 * r_bones)},             {"rf_l",dVector(-l_LPT / 2,   l_Hip / 2, -4 * r_bones)},
        {"vas_r",dVector(-l_Up_Leg / 2, 0, -4 * r_bones)},                  {"vas_l",dVector(-l_Up_Leg / 2, 0, -4 * r_bones)},
        {"sol_r",dVector(0, 0, 0)},                                         {"sol_l",dVector(0, 0, 0)},
        {"ta_r",dVector(0, 0, 0)},                                          {"ta_l",dVector(0, 0, 0)},                
        {"gas_r",dVector(-l_Up_Leg * 0.4, 0, 0)},                           {"gas_l",dVector(-l_Up_Leg * 0.4, 0, 0)} };// list muscle line point 1   
    m_point2 = { {"hfl_r", dVector(0, 0, 0)},           {"hfl_l", dVector(0, 0, 0)},
        {"glu_r", dVector(0, 0, 0)},                    {"glu_l", dVector(0, 0, 0)},
        {"ham_r",dVector(-l_Up_Leg / 2, 0, r_bones)},   {"ham_l",dVector(-l_Up_Leg / 2, 0, r_bones)},
        {"rf_r",dVector(-l_Up_Leg / 2, 0, -4 * r_bones)},   {"rf_l",dVector(-l_Up_Leg / 2, 0, -4 * r_bones)},
        {"vas_r",dVector(0, 0, 0)},                         {"vas_l",dVector(0, 0, 0)},
        {"sol_r",dVector(0, 0, 2 * l_foot / 3)},            {"sol_l",dVector(0, 0, 2 * l_foot / 3)},
        {"ta_r",dVector(0, 0, 0)},                          {"ta_l",dVector(0, 0, 0)},
        {"gas_r", dVector(-l_Up_Leg * 0.5, 0, l_foot / 3)}, {"gas_l", dVector(-l_Up_Leg * 0.5, 0, l_foot / 3)} };// list muscle line point 2
    m_joint1 = { {"hfl_r", HIP_R},      {"hfl_l", HIP_L},
        {"glu_r", HIP_R},               {"glu_l", HIP_L},
        {"ham_r", HIP_R},               {"ham_l", HIP_L},
        {"rf_r", HIP_R},                {"rf_l", HIP_L},
        {"vas_r", KNEE_R},              {"vas_l", KNEE_L},
        {"sol_r",ANKLE_R},              {"sol_l",ANKLE_L},
        {"ta_r",ANKLE_R},               {"ta_l",ANKLE_L},
        {"gas_r", KNEE_R},              {"gas_l", KNEE_L} };// list muscle joint 1
    m_joint2 = { {"hfl_r", NOjoint},    {"hfl_l", NOjoint},
        {"glu_r", NOjoint},             {"glu_l", NOjoint},
        {"ham_r", KNEE_R},              {"ham_l", KNEE_L},
        {"rf_r", KNEE_R},               {"rf_l", KNEE_L},
        {"vas_r", NOjoint},             {"vas_l", NOjoint},
        {"sol_r", NOjoint},             {"sol_l", NOjoint},
        {"ta_r", NOjoint},              {"ta_l", NOjoint},
        {"gas_r", ANKLE_R},             {"gas_l", ANKLE_L} };// list muscle joint 2
    m_joint_type1 = { {"hfl_r", DoubleHinge},       {"hfl_l", DoubleHinge},
        {"glu_r", DoubleHinge},                     {"glu_l", DoubleHinge},
        {"ham_r", DoubleHinge},                     {"ham_l", DoubleHinge},
        {"rf_r", DoubleHinge},                      {"rf_l", DoubleHinge},
        {"vas_r", Hinge},                           {"vas_l", Hinge},
        {"sol_r", DoubleHinge},                     {"sol_l", DoubleHinge},
        {"ta_r", DoubleHinge},                      {"ta_l", DoubleHinge},
        {"gas_r", Hinge},                           {"gas_l", Hinge} };// list muscle joint type 1
    m_joint_type2 = { {"hfl_r", NOtype},    {"hfl_l", NOtype},
        {"glu_r", NOtype},                  {"glu_l", NOtype},
        {"ham_r", Hinge},                   {"ham_l", Hinge},
        {"rf_r", Hinge},                    {"rf_l", Hinge},
        {"vas_r", NOtype},                  {"vas_l", NOtype},
        {"sol_r", NOtype},                  {"sol_l", NOtype},
        {"ta_r", NOtype},                   {"ta_l", NOtype},
        {"gas_r", DoubleHinge},             {"gas_l", DoubleHinge} };// list muscle joint type 2
    m_muscle_name = { {"hfl_r", HFL},   {"hfl_l", HFL},
        {"glu_r", GLU},                 {"glu_l", GLU},
        {"ham_r", HAM},                 {"ham_l", HAM},
        {"rf_r", RF},                   {"rf_l", RF},
        {"vas_r", VAS},                 {"vas_l", VAS},
        {"sol_r", SOL},                 {"sol_l", SOL},
        {"ta_r", TA},                   {"ta_l", TA},
        {"gas_r", GAS},                 {"gas_l", GAS} };// list muscle name
    m_theta0 = { {"hfl_r", (180) * dDegreeToRad - alfa},    {"hfl_l", (180) * dDegreeToRad - alfa1},
        {"glu_r",(180) * dDegreeToRad + alfa},              {"glu_l",(180) * dDegreeToRad + alfa1},
        {"ham_r",180 * dDegreeToRad + alfa},                {"ham_l",180 * dDegreeToRad + alfa1},
        {"rf_r",180 * dDegreeToRad - alfa},                 {"rf_l",180 * dDegreeToRad - alfa1},
        {"vas_r",180 * dDegreeToRad + gamma},               {"vas_l",180 * dDegreeToRad + gamma},
        {"sol_r",2 * M_PI - (M_PI/2 + beta)},                 {"sol_l",2 * M_PI - (M_PI/2 + beta)},
        {"ta_r", M_PI/2 + beta},                               {"ta_l",M_PI/2 + beta},
        {"gas_r",180 * dDegreeToRad - gamma},               {"gas_l",180 * dDegreeToRad - gamma} };/////// list muscle initial angle 1
    m_theta10 = { {"ham_r",180 * dDegreeToRad - gamma}, {"ham_l",180 * dDegreeToRad - gamma},
        {"rf_r",180 * dDegreeToRad + gamma},            {"rf_l",180 * dDegreeToRad + gamma},
        {"gas_r",2 * M_PI - (M_PI/2 +beta)},              {"gas_l",2 * M_PI - (M_PI/2 + beta)} };/////list muscle initial angle 2
    m_list_prop = { {"hfl_r",hfl},  {"hfl_l",hfl},
        {"glu_r",glu},              {"glu_l",glu},
        {"ham_r",ham},              {"ham_l",ham},
        {"rf_r",rf},                {"rf_l",rf},
        {"vas_r",vas},              {"vas_l",vas},
        {"sol_r", sol},             {"sol_l", sol},
        {"ta_r", ta},               {"ta_l", ta},
        {"gas_r", gas},             {"gas_l", gas} };// lost of list muscle properties
    m_lim1 = { {"hfl_r", 0},            {"hfl_l", 0},
        {"glu_r",140 * dDegreeToRad},   {"glu_l",140 * dDegreeToRad},
        {"ham_r", 140 * dDegreeToRad},  {"ham_l", 140 * dDegreeToRad},
        {"rf_r",0},                     {"rf_l",0},
        {"vas_r",185 * dDegreeToRad},   {"vas_l",185 * dDegreeToRad},
        {"sol_r",0},                    {"sol_l",0},
        {"ta_r",70 * dDegreeToRad},     {"ta_l",70 * dDegreeToRad},
        {"gas_r",0},                    {"gas_l",0} }; // list of muscle angle 1 limit
    m_lim2 = { {"hfl_r", 0},            {"hfl_l", 0},
        {"glu_r",0},                    {"glu_l",0},
        {"ham_r", 0},                   {"ham_l", 0},
        {"rf_r",185 * dDegreeToRad},    {"rf_l",185 * dDegreeToRad},
        {"vas_r",0},                    {"vas_l",0},
        {"sol_r", 230 * dDegreeToRad},  {"sol_l", 230 * dDegreeToRad},
        {"ta_r",0},                     {"ta_l",0},
        {"gas_r",230 * dDegreeToRad},   {"gas_l",230 * dDegreeToRad} };// list of muscle angle 2 limit
   
    for (std::vector<std::string>::iterator it = muscle_keys.begin(); it != muscle_keys.end(); it++)// muscle list initialization
    {
        muscles[*it] = NULL;
    }
    int index = 0;
    for (std::vector<std::string>::iterator it = muscle_keys.begin(); it != muscle_keys.end(); it++)
    {
        GeomNewton* b1 = rigid_element.find(m_body1.find(muscle_keys[index])->second)->second;
        GeomNewton* b2 = rigid_element.find(m_body2.find(muscle_keys[index])->second)->second;
        GeomNewton* b3;
        if (muscle_keys[index] == "ham_r" || muscle_keys[index] == "ham_l" || muscle_keys[index] == "rf_r" || muscle_keys[index] == "rf_l" ||
            muscle_keys[index] == "gas_r" || muscle_keys[index] == "gas_l")
            b3 = rigid_element.find(m_body3.find(muscle_keys[index])->second)->second;
        else
            b3 = NULL;

        muscles[*it] = new Muscle(m_winManager->aLineManager, m_winManager->aManager, b1, b2, b3,
            m_point1.find(muscle_keys[index])->second, m_point2.find(muscle_keys[index])->second,
            m_joint1.find(muscle_keys[index])->second, m_joint_type1.find(muscle_keys[index])->second,
            m_joint2.find(muscle_keys[index])->second, m_joint_type2.find(muscle_keys[index])->second,
            m_muscle_name.find(muscle_keys[index])->second);
        muscles[*it]->GenerateMesh();
        m_winManager->aManager->vMuscleList.push_back(muscles[*it]);
        muscles[*it]->SetThetazero(m_theta0.find(muscle_keys[index])->second); // initial joint angle

        if (muscle_keys[index] == "ham_r" || muscle_keys[index] == "ham_l" || muscle_keys[index] == "rf_r" || muscle_keys[index] == "rf_l" ||
            muscle_keys[index] == "gas_r" || muscle_keys[index] == "gas_l")
            muscles[*it]->SetTheta1zero(m_theta10.find(muscle_keys[index])->second); // initial joint angle

        map<std::string, float> p = m_list_prop.find(muscle_keys[index])->second;
        if (muscle_keys[index] == "rf_r" || muscle_keys[index] == "rf_l") {// reference and maximum angle in local angle reference system
            muscles[*it]->SetMuscleParams(p["Fmax"], p["v_max"], p["lopt"], p["lslk"], p["rho"], p["r"], p["r1"],
                p["phiM"] * dDegreeToRad, p["phiR"] * dDegreeToRad, 2 * M_PI - p["phi1M"] * dDegreeToRad, 2 * M_PI - p["phi1R"] * dDegreeToRad);
        }// params from Geyer 
        else if (muscle_keys[index] == "vas_r"|| muscle_keys[index] == "vas_l")
            muscles[*it]->SetMuscleParams(p["Fmax"], p["v_max"], p["lopt"], p["lslk"], p["rho"], p["r"], p["r1"],
                2 * M_PI - p["phiM"] * dDegreeToRad, 2 * M_PI - p["phiR"] * dDegreeToRad, 0, 0); // params from Geyer  
        else if (muscle_keys[index] == "sol_r"|| muscle_keys[index] == "sol_l")
            muscles[*it]->SetMuscleParams(p["Fmax"], p["v_max"], p["lopt"], p["lslk"], p["rho"], p["r"], p["r1"],
                2*M_PI-p["phiM"] * dDegreeToRad, 2 * M_PI-p["phiR"] * dDegreeToRad, 0, 0); // params from Geyer  
        else if (muscle_keys[index] == "gas_r"|| muscle_keys[index] == "gas_l")
            muscles[*it]->SetMuscleParams(p["Fmax"], p["v_max"], p["lopt"], p["lslk"], p["rho"], p["r"], p["r1"],
                p["phiM"] * dDegreeToRad, p["phiR"] * dDegreeToRad, 2 * M_PI - p["phi1M"] * dDegreeToRad, 2 * M_PI - p["phi1R"] * dDegreeToRad); // params from Geyer 
        else if (muscle_keys[index] == "ham_r" || muscle_keys[index] == "ham_l" || muscle_keys[index] == "glu_r" || muscle_keys[index] == "glu_l")
            muscles[*it]->SetMuscleParams(p["Fmax"], p["v_max"], p["lopt"], p["lslk"], p["rho"], p["r"], p["r1"],
                p["phiM"] * dDegreeToRad, 2 * M_PI - p["phiR"] * dDegreeToRad, p["phi1M"] * dDegreeToRad, p["phi1R"] * dDegreeToRad); // params from Geyer 
        else {
            muscles[*it]->SetMuscleParams(p["Fmax"], p["v_max"], p["lopt"], p["lslk"], p["rho"], p["r"], p["r1"],
                p["phiM"] * dDegreeToRad, p["phiR"] * dDegreeToRad, p["phi1M"] * dDegreeToRad, p["phi1R"] * dDegreeToRad);
        } // params from Geyer  
        muscles[*it]->SetLimits(m_lim1.find(muscle_keys[index])->second, m_lim2.find(muscle_keys[index])->second);
        if (muscle_keys[index].back() == 'r')
            muscles[*it]->SetLaterality('R');
        else
            muscles[*it]->SetLaterality('L');

        //// Initial condition of muscles
        //if (muscle_keys[index] == "ham_l" || muscle_keys[index] == "glu_l")// CHANGE
        //    muscles[*it]->SetLCE(14);
        //if (muscle_keys[index] == "ham_r" || muscle_keys[index] == "glu_r")// CHANGE
        //    muscles[*it]->SetLCE(18);

        index++;
    }
 
    //// create floor object zero mass
    //box = new GeomNewton(m_winManager->aManager);
    //box->SetBodyType(adtDynamic);
    //box->SetTexTileU(3.0f);
    //box->SetTexTileV(3.0f);
    //box->SetTexture0("Textures//MRAMOR6X6.jpg", "Tex0");
    //box->SetDiffuseColor(0.45f, 0.45f, 0.45f);
    //box->SetPitchAngle(-30);
    //box->SetPosition(0, 1.125, 0);
    //box->InitNewton(atBox, 1.0f, 2.0f, 1.0f,10.f);
    //m_body = box->GetBody();
    //NewtonBodySetTransformCallback(m_body, NULL);

    // create floor object zero mass
    Geomfloor = new GeomNewton(m_winManager->aManager);
    Geomfloor->SetBodyType(adtDynamic);
    Geomfloor->SetTexTileU(3.0f);
    Geomfloor->SetTexTileV(3.0f);
    Geomfloor->SetTexture0("Textures//MRAMOR6X6.jpg", "Tex0");
    Geomfloor->SetDiffuseColor(0.45f, 0.45f, 0.45f);
    //Geomfloor->SetPitchAngle(-30);
    Geomfloor->InitNewton(atBox, 25.0f, 0.25f, 25.0f);

    //NewtonBodySetMaterialGroupID(Geomfloor->GetBody(), 0);

    //NewtonCollision* const collision2 = NewtonBodyGetCollision(Geomfloor->GetBody());

    //// use the collision user data to save the coefficient of friction 
    //dFloat coefficientOfFriction = 1.0f;

    //// save the restitution coefficient with the collision user data
    //NewtonCollisionMaterial material;
    //NewtonCollisionGetMaterial(collision2, &material);
    //material.m_userId = 1;

    //// save kinetic friction in param[0]
    //material.m_userParam[0].m_float = coefficientOfFriction;

    //// save static friction in param[1]
    //material.m_userParam[1].m_float = coefficientOfFriction;
    //NewtonCollisionSetMaterial(collision2, &material);

    //NewtonCollision* const collision1 = NewtonBodyGetCollision(rigid_element["Foot_l"]->GetBody());

    //// use the collision user data to save the coefficient of friction 
    //coefficientOfFriction = 1.0f;

    //// save the restitution coefficient with the collision user data
    //NewtonCollisionGetMaterial(collision1, &material);
    //material.m_userId = 1;

    //// save kinetic friction in param[0]
    //material.m_userParam[0].m_float = coefficientOfFriction;

    //// save static friction in param[1]
    //material.m_userParam[1].m_float = coefficientOfFriction;
    //NewtonCollisionSetMaterial(collision1, &material);

    // Check for body com position!
    COMXline_id = CreateLine();
    COMYline_id = CreateLine(); 
    COMZline_id = CreateLine(); 

    // Control class initialization. check reference or pointer
    controller = ControlAlgorithm();
    controller.SetState0(180 * dDegreeToRad -gamma, 0, "Sknee_r");
    controller.SetState0(180 * dDegreeToRad -gamma, 0, "Sknee_l");
    controller.SetState0(180 * dDegreeToRad - alfa, 0, "Ship_r");
    controller.SetState0(180 * dDegreeToRad - alfa1, 0, "Ship_l");
    controller.SetState0(90 * dDegreeToRad + beta, 0, "Sankle_r");
    controller.SetState0(90 * dDegreeToRad + beta, 0, "Sankle_l");
    controller.SetState0(0, 0, "Strunk");
    
}

int dRaycastVHModel::CreateLine() {
glm::vec3 linepos1;
glm::vec3 linepos2;
glm::vec3 linecolor;

linepos1.x = 0; linepos1.y = 0; linepos1.z = 0;
linepos2.x = 2.0f; linepos2.y = 2.0f; linepos2.z = 2.0f;
linecolor.x = 0.0f; linecolor.y = 1.0f; linecolor.z = 0.f;

return m_winManager->aLineManager->AddLine(linepos1, linepos2, linecolor);
}
// Same method of dCustomJoint
dFloat dRaycastVHModel::CalculateAngle(const dVector& dir, const dVector& cosDir, const dVector& sinDir) const 
{
    //dAssert(dAbs(sinDir.DotProduct3(cosDir)) < dFloat (1.0e-4f));
    dVector projectDir(dir - sinDir.Scale(dir.DotProduct3(sinDir)));
    dFloat cosAngle = projectDir.DotProduct3(cosDir);
    dFloat sinAngle = sinDir.DotProduct3(projectDir.CrossProduct(cosDir));
    return dAtan2(sinAngle, cosAngle);
}

// Compute trunk orientation and rotation velocity in sagittal plane
vector<dFloat> dRaycastVHModel::GetTrunkSagittalState()
{   
    //dVector cos, sin;
    //dMatrix mat = rigid_element.find("LPT")->second->GetMatrix();// in globa coordinate??
    //dVector vertical = dVector(0, 1, 0, 1);
    //cos = mat.m_front.DotProduct3(dVector(1, 0, 1, 1));
    //sin = mat.m_right.DotProduct3(dVector(1, 0, 1, 1));
    //dFloat ang = CalculateAngle(vertical, cos, sin);
    // 
    dMatrix mat = rigid_element.find("LPT")->second->GetMatrix();
    dVector body(0, mat.m_front.m_y, mat.m_front.m_z, 1);
    dVector vertical = dVector(0, 1, 0, 1);
    dVector aa = body.CrossProduct(vertical);
    float norm = sqrt(pow(aa.m_x, 2) + pow(aa.m_y, 2) + pow(aa.m_z, 2));
    dFloat ang = dAtan2(norm, body.DotProduct3(vertical));
    if (aa.DotProduct3(dVector(1, 0, 0, 1)) < 0) {
        ang = -ang;
    }
    // save the current joint Omega
    dVector omega0(0.0f);
    NewtonBodyGetOmega(rigid_element.find("LPT")->second->GetBody(), &omega0[0]);
    return { -ang, omega0.m_x };
}
// Compute COM position of the humanoid in global coordinates
dVector dRaycastVHModel::ComputePlayerCOM()
{
    dVector PlayerCOM(0,0,0);
    float mass_tot=0;
    for (std::vector<std::string>::iterator it = body_keys.begin(); it != body_keys.end(); it++)
    {
        dVector com;
        float mass, Ix, Iy, Iz;
        NewtonBodyGetMass(rigid_element[*it]->GetBody(), &mass, &Ix, &Iy, &Iz);
        NewtonBodyGetCentreOfMass(rigid_element[*it]->GetBody(), &com[0]);
        com = Rel2AbsPoint(rigid_element[*it]->GetBody(), &com[0]);
        PlayerCOM.m_x = PlayerCOM.m_x + com.m_x * mass;
        PlayerCOM.m_y = PlayerCOM.m_y + com.m_y * mass;
        PlayerCOM.m_z = PlayerCOM.m_z + com.m_z * mass;
        mass_tot = mass_tot + mass;
    }
    PlayerCOM = PlayerCOM.Scale(1/mass_tot);
    return PlayerCOM;
}

// Compute COM velocity of the humanoid in global coordinates
dVector dRaycastVHModel::ComputePlayerCOMvelocity()
{
    dVector PlayerCOMvel(0, 0, 0);
    float mass_tot = 0;
    for (std::vector<std::string>::iterator it = body_keys.begin(); it != body_keys.end(); it++)
    {
        dVector v;
        float mass, Ix, Iy, Iz;
        NewtonBodyGetMass(rigid_element[*it]->GetBody(), &mass, &Ix, &Iy, &Iz);
        NewtonBodyGetVelocity(rigid_element[*it]->GetBody(), &v[0]);
        PlayerCOMvel.m_x = PlayerCOMvel.m_x + v.m_x * mass;
        PlayerCOMvel.m_y = PlayerCOMvel.m_y + v.m_y * mass;
        PlayerCOMvel.m_z = PlayerCOMvel.m_z + v.m_z * mass;
        mass_tot = mass_tot + mass;
    }
    PlayerCOMvel = PlayerCOMvel.Scale(1 / mass_tot);
    return PlayerCOMvel;
}

GeomNewton* dRaycastVHModel::Get_Floor()
{
    return Geomfloor;
}

GeomNewton* dRaycastVHModel::Get_box()
{
    return box;
}

map<std::string, GeomNewton*> dRaycastVHModel::Get_RigidElemetList()
{
    return rigid_element;
}

map<std::string, dCustomHinge*> dRaycastVHModel::Get_HingeJointList()
{
    return JHinge;
}
map<std::string, dCustomDoubleHinge*> dRaycastVHModel::Get_DoubleHingeJointList()
{
    return JDoubleHinge;
}

float dRaycastVHModel::GetLegLength()
{
    return l_Up_Leg + l_Low_Leg;
}

//Draw three lines parallel to global x y and z respectively. The origin of all lines is the input position vector. Use a the second input to set line length
void dRaycastVHModel::DrawFrame(const dVector& posit, dFloat scale)
{
    int Xindex = COMXline_id;
    int Yindex = COMYline_id;
    int Zindex = COMZline_id;
    vector <int> ind = { Xindex,Yindex,Zindex };
    float val1 = 0, val2 = 0, val3 = 0;
    for (int n = 0; n<3; ++n)
    {
        if (n == 0) { val1 = 0.2*scale; val2 = 0; val3 = 0; }
        else if (n == 1) { val1 = 0; val2 = 0.2 * scale; val3 = 0; }
        else if (n == 2) { val1 = 0; val2 = 0; val3 = 0.2 * scale; }
        m_winManager->aLineManager->aLineBuffer[ind[n] - 1].posit.x = posit[0];
        m_winManager->aLineManager->aLineBuffer[ind[n] - 1].posit.y = posit[1];
        m_winManager->aLineManager->aLineBuffer[ind[n] - 1].posit.z = posit[2];
        //aLineBuffer[LineIndex].color = newlinecolor;
        m_winManager->aLineManager->aLineBuffer[ind[n] - 2].posit.x = posit[0] + val1;
        m_winManager->aLineManager->aLineBuffer[ind[n] - 2].posit.y = posit[1] + val2;
        m_winManager->aLineManager->aLineBuffer[ind[n] - 2].posit.z = posit[2] + val3;
    }

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DGVehicleRCManager::DGVehicleRCManager(WindowMain* winctx)
: dModelManager(winctx->aManager->GetWorld()),
  m_winManager(winctx)
{
    
}

dModelRootNode* DGVehicleRCManager::CreateWalkerPlayer(const char* const modelName, const dMatrix& location)
{
    dRaycastVHModel* const controller = new dRaycastVHModel(m_winManager, modelName, location, 0);

    // set the model to calculate the local transformation
    controller->SetTransformMode(true);

    // add the model to the manager
    AddRoot(controller);

    return controller;
}

int DGVehicleRCManager::GetContactPoints(NewtonBody* const body, dVector* const points) const
{
    dVector point(0.0f);
    dVector normal(0.0f);
    int count = 0;
    for (NewtonJoint* contactJoint = NewtonBodyGetFirstContactJoint(body); contactJoint; contactJoint = NewtonBodyGetNextContactJoint(body, contactJoint)) {
        if (NewtonJointIsActive(contactJoint)) {
            for (void* contact = NewtonContactJointGetFirstContact(contactJoint); contact; contact = NewtonContactJointGetNextContact(contactJoint, contact)) {
                NewtonMaterial* const material = NewtonContactGetMaterial(contact);
                NewtonMaterialGetContactPositionAndNormal(material, body, &point.m_x, &normal.m_x);
                points[count] = point;
                count++;
            }
        }
    }
    return count;
}

void DGVehicleRCManager::OnPreUpdate(dModelRootNode* const model, dFloat timestep, int threadID) const
//pour VS<VS2017 supprimer ,int threadID
{
    dRaycastVHModel* Model = (dRaycastVHModel*)model;
    ////cout << "DGVehicleRCManager OnP reUpdate \n";
    map<std::string, GeomNewton*> RE_list = Model->Get_RigidElemetList();

     //INITIAL CONDITION //
    if (newTime < 0.1) {
        GeomNewton* BODY = RE_list.find("LPT")->second;
        float vel = -350;
        dVector LPTvel_init = { 0,0,vel };
        NewtonBodyAddForce(BODY->GetBody(), &LPTvel_init[0]);
        BODY = RE_list.find("UPT")->second;
        vel = -250;
        LPTvel_init = { 0,0,vel };
        NewtonBodyAddForce(BODY->GetBody(), &LPTvel_init[0]);
        //GeomNewton* BODY = RE_list.find("LPT")->second;
        //float t = -10;
        //dVector LPTvel_init = { t,0,0 };
        //NewtonBodyAddTorque(BODY->GetBody(), &LPTvel_init[0]);

    }

    // find ankle position
    dVector com_Player = Model->ComputePlayerCOM();
    dVector com_Playervel = Model->ComputePlayerCOMvelocity();
    dVector com_ankle_r, com_ankle_l;
    map<std::string, dCustomDoubleHinge*> JDoubleHinge = Model->Get_DoubleHingeJointList();
    dCustomDoubleHinge* ankle = JDoubleHinge.find("Ankle_r")->second;
    dMatrix mat = ankle->GetMatrix0();// error fix
    com_ankle_r = mat.m_posit;
    GeomNewton* BODY0 = RE_list.find("Foot_r")->second;
    GeomNewton* BODY1 = RE_list.find("Foot_l")->second;
    com_ankle_r = Rel2AbsPoint(BODY0->GetBody(), &com_ankle_r.m_x);
    ankle = JDoubleHinge.find("Ankle_l")->second;
    mat = ankle->GetMatrix0();// error fix
    com_ankle_l = mat.m_posit;
    com_ankle_l = Rel2AbsPoint(BODY1->GetBody(), &com_ankle_l.m_x);

    // find contact between foot and ground.
    float count_contact = 0;
    dVector supportPolygon[32];
    int count = 0;
    count += GetContactPoints(BODY0->GetBody(), &supportPolygon[count]);
    vector<bool> state_foot_r = Model->controller.Stance_Swing_Detection(com_ankle_r, com_Player, Model->GetLegLength(), count);

    count = 0;
    count += GetContactPoints(BODY1->GetBody(), &supportPolygon[count]);
    vector<bool> state_foot_l = Model->controller.Stance_Swing_Detection(com_ankle_l, com_Player, Model->GetLegLength(), count);
  
    // Model com visualization
    Model->DrawFrame(com_Player, 1.0); // comment this line to check single body com position and use following lines
    //// Check for body com position! change manually the name of the body 
    //dMatrix mat = list_RE.find("Thigh_r")->second->GetMatrix();;
    //Model->DrawFrame(mat.m_posit, 1.0);

    // leading leg: the one that is in heel strike (z coordinate of ankle is smaller than z coordinate of player com)
    char lead = 'R';
    bool DS = false; // double stance
    if (state_foot_r[0] == true && state_foot_l[0] == true)// double stance
    {
        DS = true;
        if (com_ankle_r.m_z < com_Player.m_z)// Check direzione maggiore/minore
            lead = 'R';
        else if (com_ankle_l.m_z < com_Player.m_z)
            lead = 'L';
    }
    state_foot_l.push_back(DS); // add double stance state
    state_foot_r.push_back(DS); // add double stance state

    float d = 0;// compute horizontal distance between stance foot and com for stance preparation hip control
    if (!DS)
    {
        if (state_foot_r[0] == true)
            d = sqrt(pow(com_Player.m_z - com_ankle_r.m_z, 2) + pow(com_Player.m_x - com_ankle_r.m_x, 2));
        else if (state_foot_l[0] == true)
            d = sqrt(pow(com_Player.m_z - com_ankle_l.m_z, 2) + pow(com_Player.m_x - com_ankle_l.m_x, 2));
        Model->controller.ControlSetHorizontalDistance(d);
    }

    if (newTime > 0.1633)
        int a = 0;
    // update sagittal trunk angle and velocity
    vector<float> state = Model->GetTrunkSagittalState(); 
    Model->controller.SetState(state[0], state[1], "Strunk");

    //scan all  Muscle Elements
    for (auto itr = m_winManager->aManager->vMuscleList.begin();
        itr != m_winManager->aManager->vMuscleList.end(); itr++)
    {
        dVector pin; // needed to apply the torque along the joint pin
        dVector pin1; // needed to apply the torque along the joint 1 pin
        Muscle* Mobj = (Muscle*)*itr;
        // Get joint angle and joint velocity for proportional controller of VAS, HFL and GLU (stance preparation)
        char lat = Mobj->GetLaterality();
        // loop on two joints
        for (auto itr1 = 0; itr1 <= 1; itr1++)
        {
            // Read the joint name list and find the joint name of the Mobj
            std::vector<JointName> list = m_winManager->aManager->vJointNameList;
            int j_index;
            JointType type;
            if (itr1 == 0)
            {
                j_index = std::distance(list.begin(), std::find(list.begin(), list.end(), Mobj->Jname));// find index of joint in jointnamelist
                type = Mobj->Jtype;
            }
            else
            {
                j_index = std::distance(list.begin(), std::find(list.begin(), list.end(), Mobj->Jname1));// find index of joint in jointnamelist
                type = Mobj->Jtype1;
            }
            // switch the type joint of the Mobj 
            switch (type) {
            case Hinge:
            {
                dCustomHinge* joint = (dCustomHinge*)m_winManager->aManager->vJointList[j_index]; // joint from jointlist
                if (itr1 == 0)
                {
                    Mobj->SetAngle(joint->GetJointAngle()); // joint angle
                    pin = joint->GetPinAxis(); // joint pin
                    if (Mobj->m_name == GAS && lat == 'R') {
                        vector<float> state = Model->controller.GetState("Sknee_r");
                        Model->controller.SetState(joint->GetJointAngle(),joint->GetJointOmega(), "Sknee_r");
                    }
                    if (Mobj->m_name == GAS && lat == 'L') {
                        vector<float> state = Model->controller.GetState("Sknee_l");
                        Model->controller.SetState(joint->GetJointAngle(),joint->GetJointOmega(), "Sknee_l");
                    }
                }
                else
                {
                    Mobj->SetAngle1(joint->GetJointAngle()); // joint angle
                    pin1 = joint->GetPinAxis(); // joint pin
                }
                break;
            }
            case BallAndSocket:
            {
                dCustomBallAndSocket* joint = (dCustomBallAndSocket*)m_winManager->aManager->vJointList[j_index]; // joint from jointlist
                //GetJointAngle() and GetPinAxis() methods and m_curJointAngle() must be added inside dCustomAndSocket.cpp and the respective header file 
                //Copy the dCustomHinge.cpp respective methods and build newton again. 
                //Copy new dlls in project folder
                if (itr1 == 0)
                {
                    Mobj->SetAngle(joint->GetJointAngle()); // joint angle
                    pin = joint->GetPinAxis(); // joint pin
                }
                else
                {
                    Mobj->SetAngle1(joint->GetJointAngle()); // joint angle
                    pin1 = joint->GetPinAxis(); // joint pin
                }
                break;
            }
            case DoubleHinge:
            {
                dCustomDoubleHinge* joint = (dCustomDoubleHinge*)m_winManager->aManager->vJointList[j_index];
                if (itr1 == 0)
                {
                    Mobj->SetAngle(joint->GetJointAngle());
                    pin = joint->GetPinAxis();
                    if (Mobj->m_name == HFL && lat == 'R') {
                        vector<float> state = Model->controller.GetState("Ship_r");
                        Model->controller.SetState(joint->GetJointAngle(),joint->GetJointOmega(), "Ship_r");
                    }
                    if (Mobj->m_name == HFL && lat == 'L') {
                        vector<float> state = Model->controller.GetState("Ship_l");
                        Model->controller.SetState(joint->GetJointAngle(),joint->GetJointOmega(), "Ship_l");
                    }
                    if (Mobj->m_name == TA && lat == 'R') {
                        vector<float> state = Model->controller.GetState("Sankle_r");
                        Model->controller.SetState(joint->GetJointAngle(),joint->GetJointOmega(), "Sankle_r");
                    }
                    if (Mobj->m_name == TA && lat == 'L') {
                        vector<float> state = Model->controller.GetState("Sankle_l");
                        Model->controller.SetState(joint->GetJointAngle(),joint->GetJointOmega(), "Sankle_l");
                    }
                }
                else
                {
                    Mobj->SetAngle1(joint->GetJointAngle());
                    pin1 = joint->GetPinAxis(); // joint pin
                }
                break;
            }
            case NOtype:
            {
                break;
            }
            }
        }
        
        // Get state of current foot
        vector<bool> Gait;
        if (lat == 'R')
            Gait = state_foot_r;
        else
            Gait = state_foot_l;

        // Get excitation
        float angle, angle1, lce, Fmuscle, mus_vel, lmtu, Fmax, lopt;
        Mobj->GetMuscleParams(angle, angle1, lce, lopt, lmtu, Fmuscle, Fmax, mus_vel);
        float u = Model->controller.MTU_excitation(Mobj->m_name, Gait, lead, lat, lce / lopt, Fmuscle / Fmax, com_Playervel);
        
        //Impose muscle as actuator
        Mobj->SetNeuralDelay(1.f / 3000.f); // 1.f/2400.f s

        // set activation
        Mobj->SetActivation(u);

        dVector Ttemp = Mobj->Compute_muscle_Torque(timestep);
        //cout << newTime << ' ' << Ttemp.m_x << endl;

        // Get the Body1 connected to the muscle and apply the muscle force
        GeomNewton* gNewton = (GeomNewton*)(Mobj->body1);
        NewtonBody* NBody = gNewton->GetBody();

        pin = pin.Scale(-Ttemp.m_x); // apply torque on the correct axis
        NewtonBodyAddTorque(NBody, &pin.m_x);

        // Get the Body2 connected and apply the opposite muscle force
        gNewton = (GeomNewton*)(Mobj->body2);
        NBody = (NewtonBody*)gNewton->GetBody();
        pin.Scale(-1); // opposite force on second body
        NewtonBodyAddTorque(NBody, &pin.m_x);

        if (Mobj->body3 != NULL)
        {
            // Get the Body2 connected to the muscle and apply the muscle force
            pin1 = pin1.Scale(-Ttemp.m_y);
            NewtonBodyAddTorque(NBody, &pin1.m_x);

            // Get the Body3 connected and apply the opposite muscle force
            gNewton = (GeomNewton*)(Mobj->body3);
            NBody = (NewtonBody*)gNewton->GetBody();
            pin1.Scale(-1); // opposite force on third body
            NewtonBodyAddTorque(NBody, &pin1.m_x);
        }
        // update for next force update
        Mobj->SetLCE(Mobj->GetLCE() + Mobj->GetDelta_l()); // update lCE

        //// stamp u of all muscles
        //monFlux << u << "  ";// Use Test_excitations.m to see data
   
        // stamp torque of all muscles 
        monFlux << Ttemp.m_x << "  " << Ttemp.m_y << "  ";
    }

     
    vector<float> statehipr = Model->controller.GetState("Ship_r");
    vector<float> statehipl = Model->controller.GetState("Ship_l");
    vector<float> statekneer = Model->controller.GetState("Sknee_r");
    vector<float> statekneel = Model->controller.GetState("Sknee_l");
    vector<float> stateankler = Model->controller.GetState("Sankle_r");
    vector<float> stateanklel = Model->controller.GetState("Sankle_l");
    vector<float> stateLPT = Model->controller.GetState("Strunk");

    vector<float> state0hipr = Model->controller.GetState0("Ship_r");
    vector<float> state0hipl = Model->controller.GetState0("Ship_l");
    vector<float> state0kneer = Model->controller.GetState0("Sknee_r");
    vector<float> state0kneel = Model->controller.GetState0("Sknee_l");
    vector<float> state0ankler = Model->controller.GetState0("Sankle_r");
    vector<float> state0anklel = Model->controller.GetState0("Sankle_l");

    // verify total torque on joints
    monFlux << newTime << "  " << statehipr[0] << "  " << statekneer[0] << "  " << stateankler[0] 
        << "  " << statehipl[0] << "  " << statekneel[0] << "  " << stateanklel[0] << "  " << stateLPT[0] << "  " << stateLPT[1] <<
            "  " << state_foot_r[0] << "  " << state_foot_r[1] << "  " << state_foot_r[2] << "  " << state_foot_r[3] <<
       "  " << state_foot_l[0] << "  " << state_foot_l[1] << "  " << state_foot_l[2] << "  " << state_foot_l[3] <<endl;

    //// verify excitations
    ////monFlux << newTime << "  " << state0hipr[0]+statehipr[0] << "  " << state0kneer[0]+statekneer[0] << "  " << state0kneer[0]+stateankler[0] <<
    ////    "  " << state0hipl[0]+statehipl[0] << "  " << state0kneel[0]+statekneel[0] << "  " << state0anklel[0]+stateanklel[0] <<
    ////    "  " << state_foot_r[0] << "  " << state_foot_r[1] << "  " << state_foot_r[2] << "  " << state_foot_r[3] <<
    ////    "  " << state_foot_l[0] << "  " << state_foot_l[1] << "  " << state_foot_l[2] << "  " << state_foot_l[3] << endl;// check initial angle according to initial geometry
    //monFlux << newTime << "  " <<statehipr[0] << "  " << statekneer[0] << "  " << stateankler[0] <<
    //    "  " << statehipl[0] << "  " << statekneel[0] << "  " << stateanklel[0] <<
    //    "  " << state_foot_r[0] << "  " << state_foot_r[1] << "  " << state_foot_r[2] << "  " << state_foot_r[3] <<
    //    "  " << state_foot_l[0] << "  " << state_foot_l[1] << "  " << state_foot_l[2] << "  " << state_foot_l[3] << "  " << stateLPT[0] <<endl;// check initial angle according to initial geometry

    newTime = newTime + timestep; // update time
}

void DGVehicleRCManager::OnPostUpdate(dModelRootNode* const model, dFloat timestep, int threadID) const
//pour VS<VS2017 supprimer ,int threadID

{
    //printf("DGVehicleRCManager OnPostUpdate \n");
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