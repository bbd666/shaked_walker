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
    , UP_leg(NULL)
    , LPT(NULL)
    , Plantar_L(NULL)
    , Plantar_R(NULL)
    , N1(NULL)

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
    for (std::vector<std::string>::iterator it = muscle_keys.begin(); it != muscle_keys.end(); it++)
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
        Ixx.size() != 12 || Iyy.size() != 12 || Izz.size() != 12 || delta_cm.size() != 12 || v_angles.size() != 2 || 
        sol.size() != 11 || ta.size() != 11 || vas.size() != 11 || gas.size() != 11 || hfl.size() != 11 ||ham.size() != 11 ||
        rf.size() != 11 || glu.size() != 11)
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

    glm::vec3 _Pos(glm::vec3(0.0f, 0.5+l_LPT/2 + 2 * l_Up_Leg + h_foot, 0.f)); // INITIAL POSITION  OF THE DUMMY IN THE SCENE X (lateral, + left) Y(vertical, + sky) Z(front, + back)

    float alfa = 0 * dDegreeToRad;
    float gamma = 10 * dDegreeToRad;
    float beta = 90 * dDegreeToRad;
    // BODIES OF THE DUMMY DEFINITION

    LPT = new GeomNewton(m_winManager->aManager);
    LPT->SetBodyType(adtDynamic);
    LPT->SetTexture0(&tex[0], "Tex0");
    LPT->SetDiffuseColor(0.7f, 0.7f, 0.7f);
    LPT->SetRollAngle(90.0f, false);
    LPT->SetPosition(_Pos.x, _Pos.y, _Pos.z);
    LPT->InitNewton(atCapsule, r_bones, r_bones, l_LPT, 0.0f);
    m_body = LPT->GetBody();
    NewtonBodySetTransformCallback(m_body, NULL);

    UP_leg = new GeomNewton(m_winManager->aManager);
    UP_leg->SetBodyType(adtDynamic);
    UP_leg->SetParent(LPT);
    UP_leg->SetTexture0(&tex[0], "Tex0");
    UP_leg->SetDiffuseColor(1.0f, 1.0f, 1.0f);
    UP_leg->SetTurnAngle(-alfa*dRadToDegree, false);
    UP_leg->SetPosition(0, -l_LPT / 2 - (l_Up_Leg / 2 )*cos(alfa)-r_bones, -(l_Up_Leg / 2 )* sin(alfa));
    UP_leg->InitNewton(atCapsule, r_bones, r_bones, l_Up_Leg, 1.0f);
    NewtonBodySetMassMatrix(UP_leg->GetBody(), mass_distrib["Thigh"], Ixx["Thigh"], Iyy["Thigh"], Izz["Thigh"]);
    NewtonBodySetTransformCallback(UP_leg->GetBody(), NULL);
    N1 = new dModelNode(UP_leg->GetBody(), dGetIdentityMatrix(), this);

    LP_leg = new GeomNewton(m_winManager->aManager);
    LP_leg->SetBodyType(adtDynamic);
    LP_leg->SetParent(UP_leg);
    LP_leg->SetTexture0(&tex[0], "Tex0");
    LP_leg->SetDiffuseColor(1.0f, 1.0f, 1.0f);
    LP_leg->SetTurnAngle((alfa+gamma) * dRadToDegree, false);
    LP_leg->SetPosition(0, -(l_Up_Leg / 2) * cos(alfa) - r_bones - (l_Up_Leg / 2) * cos(gamma), -(l_Up_Leg / 2) * sin(alfa) + (l_Up_Leg / 2) * sin(gamma));
    LP_leg->InitNewton(atCapsule, r_bones, r_bones, l_Up_Leg, 15.0f);
    NewtonBodySetMassMatrix(LP_leg->GetBody(), mass_distrib["Shank"], Ixx["Shank"], Iyy["Shank"], Izz["Shank"]);
    NewtonBodySetTransformCallback(LP_leg->GetBody(), NULL);
    N2 = new dModelNode(LP_leg->GetBody(), dGetIdentityMatrix(), N1);

    Plantar_L = new GeomNewton(m_winManager->aManager);
    Plantar_L->SetBodyType(adtDynamic);
    Plantar_L->SetParent(LP_leg);
    Plantar_L->SetRollAngle(beta * dRadToDegree, false);
    Plantar_L->SetTexture0(&tex[0], "Tex0");
    Plantar_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
    Plantar_L->SetPosition(0, -(l_Up_Leg / 2) * cos(gamma) - r_bones, (l_Up_Leg / 2) * sin(gamma) -l_foot/3);
    Plantar_L->InitNewton(atBox, h_foot, h_foot, l_foot, 4.0f);
    NewtonBodySetMassMatrix(Plantar_L->GetBody(), mass_distrib["Foot"], Ixx["Foot"], Iyy["Foot"], Izz["Foot"]);
    NewtonBodySetTransformCallback(Plantar_L->GetBody(), NULL);
    N3 = new dModelNode(Plantar_L->GetBody(), dGetIdentityMatrix(), N2);

    Ball = new GeomNewton(m_winManager->aManager);
    Ball->SetBodyType(adtDynamic);
    Ball->SetParent(LPT);
    Ball->SetRollAngle(beta * dRadToDegree, false);
    Ball->SetTurnAngle(180, false);
    Ball->SetTexture0("Textures//wood6.png", "Tex0");
    Ball->SetDiffuseColor(1.0f, 1.0f, 1.0f);
    Ball->SetPosition(0, -(l_LPT / 2 + 2 * l_Up_Leg + h_foot), -l_foot);
    Ball->InitNewton(atSphere, 0.1, 0, 0, 1.0f);
    NewtonBodySetTransformCallback(Ball->GetBody(), NULL);
    N4 = new dModelNode(Ball->GetBody(), dGetIdentityMatrix(), this);

    //// EX. BallAndSocket joint. WIP
    //dMatrix Ankle_PinMatrix(dGetIdentityMatrix()); // define the pin
    //Ankle_PinMatrix = Ankle_PinMatrix * dPitchMatrix(90.0f * dDegreeToRad); // DEFINE THE CORRECT DOF OF A BALLANDSOCKET
    //Ankle_PinMatrix.m_posit = dVector(LP_leg->GetPosition().m_x, LP_leg->GetPosition().m_y - (l_Up_Leg / 2) * cos(gamma), LP_leg->GetPosition().m_z + (l_Up_Leg / 2) * sin(gamma), 1.0f);
    //Ankle = new dCustomBallAndSocket(Ankle_PinMatrix, LP_leg->GetBody(), Plantar_L->GetBody());
    //m_winManager->aManager->vJointList.push_back(Ankle);
    //m_winManager->aManager->vJointNameList.push_back(ANKLE);

    dMatrix Ankle_PinMatrix(dGetIdentityMatrix()); // define the pin
    //Ankle_PinMatrix = Ankle_PinMatrix * dYawMatrix(90.0f * dDegreeToRad);
    Ankle_PinMatrix.m_posit = dVector(LP_leg->GetPosition().m_x, LP_leg->GetPosition().m_y - (l_Up_Leg / 2) * cos(gamma), LP_leg->GetPosition().m_z+ (l_Up_Leg / 2) * sin(gamma), 1.0f);
    Ankle = new dCustomDoubleHinge(Ankle_PinMatrix, LP_leg->GetBody(), Plantar_L->GetBody());
    m_winManager->aManager->vJointList.push_back(Ankle);
    m_winManager->aManager->vJointNameList.push_back(ANKLE);

    // EX. HINGE joint. 
    dMatrix Knee_PinMatrix(dGetIdentityMatrix()); // define the pin
    Knee_PinMatrix.m_posit = dVector(UP_leg->GetPosition().m_x, UP_leg->GetPosition().m_y -(l_Up_Leg / 2) * cos(alfa), UP_leg->GetPosition().m_z - (l_Up_Leg / 2) * sin(alfa), 1.0f);
    Knee = new dCustomHinge(Knee_PinMatrix, UP_leg->GetBody(), LP_leg->GetBody());
    m_winManager->aManager->vJointList.push_back(Knee);
    m_winManager->aManager->vJointNameList.push_back(KNEE);
    
    //Knee->EnableLimits(true);
    //Knee->SetLimits(-45.0f * dDegreeToRad, 45.0f * dDegreeToRad);
    
    // EX. DOUBLE HINGE joint. 
    dMatrix Hip_PinMatrix(dGetIdentityMatrix()); // define the pin
    Hip_PinMatrix = Hip_PinMatrix * dPitchMatrix(90.0f * dDegreeToRad); // DEFINE THE CORRECT DOF OF A DOUBLEHINGE (the second pin must be the one actuated by the muscle)
    Hip_PinMatrix.m_posit = dVector(LPT->GetPosition().m_x, LPT->GetPosition().m_y - l_LPT / 2 - r_bones, LPT->GetPosition().m_z, 1.0f);
    Hip = new dCustomDoubleHinge(Hip_PinMatrix, LPT->GetBody(), UP_leg->GetBody());
    m_winManager->aManager->vJointList.push_back(Hip);
    m_winManager->aManager->vJointNameList.push_back(HIP);


    ////// MUSCLE DEFINITION

    //////EX: muscle:
    //////1. body 1 (proximal ref)
    //////2. body 2 (proximal ref)
    //////3. body 3 (proximal ref) (NULL if no body)
    //////4. Mesh coordinates: vector 1 on body 1, vector 2 on body 2 (or 3 for biarticular) (change the values in 'DummyGeometricProperties.xml' file)
    //////5. String containing the name of the first joint. The name MUST be UNIQUE. The first joint is the proximal (closer to the head)
    //////6. Enum containing the type of the first joint (Hinge, Ball, DoubleHinge)
    //////7. String containing the name of the second joint. The name MUST be UNIQUE. Use "None" if the muscle is monoarticular
    //////8. Enum containing the type of the second joint (Hinge, Ball, DoubleHinge, None). select None if the muscle is monoarticular
    
    
    // ex of monoarticular muscle
    m_hfl_R = new Muscle(m_winManager->aLineManager, m_winManager->aManager, LPT, UP_leg, NULL, dVector(0, 0, -2*r_bones), dVector(0, 0, -2 * r_bones), HIP, DoubleHinge, NOjoint, NOtype, HFL);
    m_hfl_R->GenerateMesh();
    m_winManager->aManager->vMuscleList.push_back(m_hfl_R);
    m_hfl_R->SetThetazero((180)* dDegreeToRad-alfa); // initial joint angle
    m_hfl_R->SetMuscleParams(hfl["Fmax"], hfl["v_max"], hfl["lopt"], hfl["lslk"], hfl["rho"], hfl["r"], hfl["r1"], 
        hfl["phiM"] * dDegreeToRad, hfl["phiR"]*dDegreeToRad, hfl["phi1M"] * dDegreeToRad, hfl["phi1R"] * dDegreeToRad); // params from Geyer  
    
    // ex of monoarticular muscle
    m_glu_R = new Muscle(m_winManager->aLineManager, m_winManager->aManager, LPT, UP_leg, NULL, dVector(0, 0, +2 * r_bones), dVector(0, 0, +2 * r_bones), HIP, DoubleHinge, NOjoint, NOtype, GLU);
    m_glu_R->GenerateMesh();
    m_winManager->aManager->vMuscleList.push_back(m_glu_R);
    m_glu_R->SetThetazero((180) * dDegreeToRad +alfa); // initial joint angle
    m_glu_R->SetMuscleParams(glu["Fmax"], glu["v_max"], glu["lopt"], glu["lslk"], glu["rho"], glu["r"], glu["r1"],
        glu["phiM"] * dDegreeToRad, glu["phiR"] * dDegreeToRad, glu["phi1M"] * dDegreeToRad, glu["phi1R"] * dDegreeToRad); // params from Geyer  
    m_glu_R->SetLimits(140 * dDegreeToRad, 0);

    //// Ex of biarticular muscle
    m_ham_R = new Muscle(m_winManager->aLineManager, m_winManager->aManager, LPT, UP_leg, LP_leg, dVector(-l_LPT, 0, 2 * r_bones), dVector(-l_Up_Leg/2, 0, 2 * r_bones), HIP, DoubleHinge, KNEE, Hinge, HAM);
    m_ham_R->GenerateMesh();
    m_winManager->aManager->vMuscleList.push_back(m_ham_R);
    m_ham_R->SetThetazero(180* dDegreeToRad + alfa); // initial joint angle
    m_ham_R->SetTheta1zero(180* dDegreeToRad - gamma); // initial joint 1 angle
    m_ham_R->SetMuscleParams(ham["Fmax"], ham["v_max"], ham["lopt"], ham["lslk"], ham["rho"], ham["r"], ham["r1"],
        ham["phiM"] * dDegreeToRad, ham["phiR"] * dDegreeToRad, ham["phi1M"] * dDegreeToRad, ham["phi1R"] * dDegreeToRad); // params from Geyer  
    m_ham_R->SetLimits(140 * dDegreeToRad, 0);

    // Ex of biarticular muscle
    m_rf_R = new Muscle(m_winManager->aLineManager, m_winManager->aManager, LPT, UP_leg, LP_leg, dVector(-l_LPT, 0, -2 * r_bones), dVector(-l_Up_Leg/2, 0, -2 * r_bones), HIP, DoubleHinge, KNEE, Hinge, RF);
    m_rf_R->GenerateMesh();
    m_winManager->aManager->vMuscleList.push_back(m_rf_R);
    m_rf_R->SetThetazero(180* dDegreeToRad - alfa); // initial joint angle
    m_rf_R->SetTheta1zero(180* dDegreeToRad + gamma); // initial joint 1 angle
    m_rf_R->SetMuscleParams(rf["Fmax"], rf["v_max"], rf["lopt"], rf["lslk"], rf["rho"], rf["r"], rf["r1"],
        rf["phiM"] * dDegreeToRad, rf["phiR"] * dDegreeToRad, M_PI - rf["phi1M"] * dDegreeToRad, M_PI - rf["phi1R"] * dDegreeToRad); // params from Geyer 
    m_rf_R->SetLimits(0, 185 * dDegreeToRad);

    // ex of monoarticular muscle
    m_vas_R = new Muscle(m_winManager->aLineManager, m_winManager->aManager, UP_leg, LP_leg, NULL, dVector(0, 0, -2 * r_bones), dVector(0, 0, -2 * r_bones), KNEE, Hinge, NOjoint, NOtype, VAS);
    m_vas_R->GenerateMesh();
    m_winManager->aManager->vMuscleList.push_back(m_vas_R);
    m_vas_R->SetThetazero(180 * dDegreeToRad + gamma); // initial joint angle
    m_vas_R->SetMuscleParams(vas["Fmax"], vas["v_max"], vas["lopt"], vas["lslk"], vas["rho"], vas["r"], vas["r1"],
        M_PI - vas["phiM"] * dDegreeToRad, M_PI - vas["phiR"] * dDegreeToRad, 0, 0); // params from Geyer  
    m_vas_R->SetLimits(185 * dDegreeToRad, 0);

    // ex of monoarticular muscle
    m_sol_R = new Muscle(m_winManager->aLineManager, m_winManager->aManager, LP_leg, Plantar_L, NULL, dVector(0, 0, 2*r_bones), dVector(0, 0, 2 * r_bones), ANKLE, DoubleHinge, NOjoint, NOtype, SOL);
    m_sol_R->GenerateMesh();
    m_winManager->aManager->vMuscleList.push_back(m_sol_R);
    m_sol_R->SetThetazero(2*M_PI-beta); // initial joint angle
    m_sol_R->SetMuscleParams(sol["Fmax"]/10, sol["v_max"], sol["lopt"], sol["lslk"], sol["rho"], sol["r"], sol["r1"],// RIDOTTA FORZA MAXXXXXX
        2*M_PI- sol["phiM"] * dDegreeToRad, 2*M_PI - sol["phiR"] * dDegreeToRad, 0, 0); // params from Geyer  
    m_sol_R->SetLimits(230 * dDegreeToRad, 0);

     //ex of monoarticular muscle
    m_ta_R = new Muscle(m_winManager->aLineManager, m_winManager->aManager, LP_leg, Plantar_L, NULL, dVector(0, 0, -2 * r_bones), dVector(0, 0, -2 * r_bones), ANKLE, DoubleHinge, NOjoint, NOtype, TA);
    m_ta_R->GenerateMesh();
    m_winManager->aManager->vMuscleList.push_back(m_ta_R);
    m_ta_R->SetThetazero(beta); // initial joint angle
    m_ta_R->SetMuscleParams(ta["Fmax"], ta["v_max"], ta["lopt"], ta["lslk"], ta["rho"], ta["r"], ta["r1"],
        ta["phiM"] * dDegreeToRad, ta["phiR"] * dDegreeToRad, 0, 0); // params from Geyer  
    m_ta_R->SetLimits(70 * dDegreeToRad, 0);

    //Ex of biarticular muscle
    m_gas_R = new Muscle(m_winManager->aLineManager, m_winManager->aManager, UP_leg, LP_leg, Plantar_L, dVector(-l_Up_Leg, 0, 2 * r_bones), dVector(-l_Up_Leg/2, 0, 2 * r_bones), KNEE, Hinge, ANKLE, DoubleHinge, GAS);
    m_gas_R->GenerateMesh();
    m_winManager->aManager->vMuscleList.push_back(m_gas_R);
    m_gas_R->SetThetazero(180 * dDegreeToRad - gamma); // initial joint angle
    m_gas_R->SetTheta1zero(2*M_PI - beta); // initial joint 1 angle
    m_gas_R->SetMuscleParams(gas["Fmax"], gas["v_max"], gas["lopt"], gas["lslk"], gas["rho"], gas["r"], gas["r1"], 
        gas["phiM"] * dDegreeToRad, gas["phiR"] * dDegreeToRad, 2*M_PI - gas["phi1M"] * dDegreeToRad, 2*M_PI - gas["phi1R"] * dDegreeToRad); // params from Geyer 
    m_gas_R->SetLimits(0, 230 * dDegreeToRad);



    ////// SOL e TA con BALL and Socket
    //    // ex of monoarticular muscle
    //m_sol_R = new Muscle(m_winManager->aLineManager, m_winManager->aManager, LP_leg, Plantar_L, NULL, dVector(0, 0, 2 * r_bones), dVector(0, 0, 2 * r_bones), ANKLE, BallAndSocket, NOjoint, NOtype, SOL);
    //m_sol_R->GenerateMesh();
    //m_winManager->aManager->vMuscleList.push_back(m_sol_R);
    //m_sol_R->SetThetazero(2 * M_PI - beta); // initial joint angle
    //m_sol_R->SetMuscleParams(sol["Fmax"] / 500, sol["v_max"], sol["lopt"], sol["lslk"], sol["rho"], sol["r"], sol["r1"],// RIDOTTA FORZA MAXXXXXX
    //    2 * M_PI - sol["phiM"] * dDegreeToRad, 2 * M_PI - sol["phiR"] * dDegreeToRad, 0, 0); // params from Geyer  
    //m_sol_R->SetLimits(230 * dDegreeToRad, 0);

    ////ex of monoarticular muscle
    //m_ta_R = new Muscle(m_winManager->aLineManager, m_winManager->aManager, LP_leg, Plantar_L, NULL, dVector(0, 0, -2 * r_bones), dVector(0, 0, -2 * r_bones), ANKLE, BallAndSocket, NOjoint, NOtype, TA);
    //m_ta_R->GenerateMesh();
    //m_winManager->aManager->vMuscleList.push_back(m_ta_R);
    //m_ta_R->SetThetazero(beta); // initial joint angle
    //m_ta_R->SetMuscleParams(ta["Fmax"]/100, ta["v_max"], ta["lopt"], ta["lslk"], ta["rho"], ta["r"], ta["r1"],
    //    ta["phiM"] * dDegreeToRad, ta["phiR"] * dDegreeToRad, 0, 0); // params from Geyer  
    //m_ta_R->SetLimits(70 * dDegreeToRad, 0);
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
    //cout << "DGVehicleRCManager OnP reUpdate \n";

    float hip_angle = 0;
    float knee_angle = 0;
    float ankle_angle = 0;

     //scan all  Muscle Elements
    for (auto itr = m_winManager->aManager->vMuscleList.begin();
        itr != m_winManager->aManager->vMuscleList.end(); itr++)
    {
        dVector pin; // needed to apply the torque along the joint pin
        dVector pin1; // needed to apply the torque along the joint 1 pin
        Muscle* Mobj = (Muscle*)*itr;
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
        //Impose muscle as actuator sinusoidal activation to body 1 and 2
        Mobj->SetNeuralDelay(1.f / 3000.f); // 1.f/2400.f s

        float delay = 0;
        switch (Mobj->GetMuscleName()) {
        case HFL:
            delay = 3.14/2;break;
        case RF:
            delay = 3.14/2;break;
        case SOL:
            delay = 3.14 / 2;break;
        case GAS:
            delay = 3.14 / 2;break;
        default:
            delay = 0;break;
        }
        Mobj->SetActivation(0.05 + 0.05 * sin(2 * 3.14f * newTime + delay));

        //Mobj->SetActivation(0.5);

        dVector Ttemp = Mobj->Compute_muscle_Torque(timestep);

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
        ////// stamp data
        //float theta(0.0f), theta1(0.0f), lce(0.0f), Fmtu(0.0f), vel(0.0f), lmtu(0.0f);
        //Mobj->GetMuscleParams(theta, theta1, lce, Fmtu, vel, lmtu);
        //monFlux << newTime << "  " << theta <<  "  " << theta1 << "  " << Mobj->fSE(Mobj->GetDelta_l()) 
        //    << "  " << Mobj->fCE(Mobj->GetDelta_l(), timestep) << "  " << Mobj->fPE(Mobj->GetDelta_l()) 
        //    << "  " << Mobj->fDE(Mobj->GetDelta_l(), timestep) << "  " << Mobj->GetNmax() << "  " << Mobj->GetLCE() 
        //    << "  " << Mobj->GetDelta_l() << "  " << Mobj->GetActivation()
        //    << "  " << vel << "  " << lmtu << "  " << Ttemp.m_x << "  " << Ttemp.m_y << std::endl;
 
        // update for next force update
        Mobj->SetLCE(Mobj->GetLCE() + Mobj->GetDelta_l()); // update lCE

        // stamp torque of all muscles
        monFlux << Ttemp.m_x << "  " << Ttemp.m_y << "  ";
        
        if (Mobj->GetMuscleName() == RF)
        {
            hip_angle = Mobj->GetAngle();
            knee_angle = Mobj->GetAngle1();
        }
        if (Mobj->GetMuscleName() == TA)
            ankle_angle = Mobj->GetAngle();
    }	
    // verify total torque on joints
    monFlux << newTime << "  " << 180*dDegreeToRad-hip_angle << "  " << 190 * dDegreeToRad -knee_angle << "  " << 90 * dDegreeToRad - ankle_angle << endl;// check initial angle according to initial geometry
    newTime = newTime + timestep; // update time
}

void DGVehicleRCManager::OnPostUpdate(dModelRootNode* const model, dFloat timestep, int threadID) const
//pour VS<VS2017 supprimer ,int threadID

{
    //printf("DGVehicleRCManager OnPostUpdate \n");
    dRaycastVHModel* controller = (dRaycastVHModel*)model;
    //controller->CastFoot("L");
    //controller->CastFoot("R");
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



