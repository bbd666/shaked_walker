#include "pch.h"
#include "OXVehicleRayCast.h"

#if defined(_DEBUG)
    ofstream monFlux("history.txt");
#endif
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
    //printf("\n");
    while (pAttrib)
    {
        if (pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS) 
            //printf("%s%s: value=[%1.4f] \n", pIndent, pAttrib->Name(), dval);
        
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
    //printf("\n");
    while (pAttrib)
    {
        if (pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS) 
            //printf("%s%s: value=[%1.4f] \n", pIndent, pAttrib->Name(), dval);

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
    //printf("%s", getIndent(indent));
    int num=0;

    switch (t)
    {
    case TiXmlNode::DOCUMENT:
        //printf("Document");
        break;

    case TiXmlNode::ELEMENT:
    {
        //printf("Element [%s]", pParent->Value());
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
            //printf("No attributes \n");
        }

        switch (num)
        {
        case 0:  break;
        case 1:
        {//printf("%s1 attribute \n", getIndentAlt(indent)); 
            break;
        }
        default:
        {//printf("%s%d attributes \n", getIndentAlt(indent), num); 
            break;
        }
        }
        break;
    }
    case TiXmlNode::COMMENT: {
        //printf("Comment: [%s]", pParent->Value());
        break;
    }
    case TiXmlNode::UNKNOWN: {
        //printf("Unknown");
        break;
    }
    case TiXmlNode::TEXT: {
        pText = pParent->ToText();
        //printf("Text: [%s]", pText->Value());
        break;
    }
    case TiXmlNode::DECLARATION: {
        //printf("Declaration");
        break;}
    default:
        break;
    }
    //printf("\n");
    for (pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
    {
        dump_to_stdout(pChild, indent + 1);
    }
}

// load the named file and dump its structure to STDOUT, recursive function
void dRaycastVHModel::dump_to_stdout(const char* pFilename)
{
    TiXmlDocument doc(pFilename);
    bool loadOkay = doc.LoadFile();
    if (loadOkay)
    {
        printf("\n%s:\n", pFilename);
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
    delete[]  Geomfloor;
}

dRaycastVHModel::dRaycastVHModel(WindowMain* winctx, const char* const modelName, const dMatrix& location, int linkMaterilID)
    : dModelRootNode(NULL, dGetIdentityMatrix())
    , m_winManager(winctx)
    , SimulationCost(0.0f)

{
    // create floor object zero mass
    Geomfloor = new GeomNewton(m_winManager->aManager);
    Geomfloor->SetBodyType(adtDynamic);
    Geomfloor->SetTexTileU(3.0f);
    Geomfloor->SetTexTileV(3.0f);
    Geomfloor->SetTexture0("Textures//MRAMOR6X6.jpg", "Tex0");
    Geomfloor->SetDiffuseColor(0.45f, 0.45f, 0.45f);
    Geomfloor->InitNewton(atBox, 25.0f, 0.25f, 25.0f, 1000.0f); // CHANGE
    // create hinge actuator to impose ROLL WBV
    dMatrix matrix;
    NewtonBodyGetMatrix(Geomfloor->GetBody(), &matrix[0][0]);
    matrix = matrix * dYawMatrix(90.0f * dDegreeToRad);
    FloorAct = new dCustomHingeActuator(matrix, Geomfloor->GetBody());
    //FloorAct->SetAngularRate(0.5f); // ?? meaning ??
    // 
    ////// MECHANICAL DATA /////////////////
    //key initialization of the maps 'lengths', 'mass_distrib', 'Ixx', 'Iyy' and 'Izz'
    for (std::vector<std::string>::iterator it = lengths_keys.begin(); it != lengths_keys.end(); it++)
    {
        lengths[*it] = 0.f;
    };
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
        cout << "ERROR IN XML" << endl;
    else
        cout << "Data Model Loaded!" << endl;
    // Saving data in the program//

    // Scale factor for length and mass
    scale_length = v_scale[0];
    scale_mass = v_scale[1];

    // LENGTH DATA
    l_Head = scale_length * lengths["Head"];
    l_Up_Arm = scale_length * lengths["UpArm"];
    l_Low_Arm = scale_length * lengths["ForeArm"];  // forearm
    l_Hand = scale_length * lengths["Hand"];
    l_trunk = scale_length * lengths["Trunk"];
    l_Up_Leg = scale_length * lengths["Thigh"];  // thigh
    l_Low_Leg = scale_length * lengths["Shank"];  // shank
    l_foot = scale_length * lengths["Foot"];
    l_toe = scale_length * lengths["Toe"];
    l_UPT = scale_length * lengths["UPT"];  // upper part trunk
    l_MPT = scale_length * lengths["MPT"]; // medium part trunk
    l_LPT = scale_length * lengths["LPT"]; // lower part trunk
    l_Hip = scale_length * lengths["Hip"];
    l_Clav = scale_length * lengths["Shoulder"];	// shoulder 
    w_foot = scale_length * lengths["FootBreadth"]; // foot breadth
    h_foot = w_foot/2; // foot heigth
    l_neck = scale_length * lengths["Neck"];
    l_delta = scale_length * (0.0345 - 0.0213); // distance from acromion to shoulder joint (arcomion position computed relative to proximal point of UPT 553.2-531.9 mm)
    r_bones = scale_length * lengths["RadBones"];
    r_Pad = w_foot / 4.0f;
    h_sphere = scale_length * lengths["RadSpheres"];
    l_foot = l_foot - l_toe;
    ankle_j = scale_length * lengths["AnkleHeight"]; // de leva table 2
    knee_j = scale_length * 0.035; // de leva table 2
    // MASS DATA in kg
    Model_Mass = scale_mass * v_total_weight[0];
    for (map<std::string, float>::iterator it = mass_distrib.begin(); it != mass_distrib.end(); it++)
    {
        it->second = it->second * Model_Mass;
    };
    //INERTIA DATA in kg*m^2
    for (map<std::string, float>::iterator it = Ixx.begin(); it != Ixx.end(); it++)
    {
        it->second = mass_distrib.find(it->first)->second * pow(it->second * scale_length * lengths.find(it->first)->second, 2);
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
        it->second = lengths.find(it->first)->second * (0.5f - it->second);
    };
    // Control class initialization. check reference or pointer
    controller = ControlAlgorithm();
    vector<float> IC_v = controller.GetInitialCondition();
    LPT_A = -IC_v[0];// theta see model notes figure 1
    ThighR_A = IC_v[1];// alpha right see model notes figure 1
    ThighL_A = IC_v[2];// alpha left leg see model notes figure 1
    Shank_A = IC_v[3];// beta see model notes figure 1
    Foot_A = IC_v[4];// gamma see model notes figure 1
    Head_A = -IC_v[5];// 
    Elbow_A = IC_v[7];// 

    controller.SetState0(180 * dDegreeToRad - Shank_A, 0, "Sknee_r");
    controller.SetState0(180 * dDegreeToRad - Shank_A, 0, "Sknee_l");
    controller.SetState0(180 * dDegreeToRad - LPT_A - ThighR_A, 0, "Ship_r");
    controller.SetState0(180 * dDegreeToRad - LPT_A - ThighL_A, 0, "Ship_l");
    controller.SetState0(90 * dDegreeToRad + Foot_A, 0, "Sankle_r");
    controller.SetState0(90 * dDegreeToRad + Foot_A, 0, "Sankle_l");

    vector<float> shoulder_angles = controller.GetShoulderTargetAngles(true,'l');
    Shoulder_sagittal_L = shoulder_angles[0];
    Shoulder_transverse = shoulder_angles[1];
    shoulder_angles = controller.GetShoulderTargetAngles(true, 'r');
    Shoulder_sagittal_R = shoulder_angles[0];
    // INITIAL POSITION  OF THE DUMMY IN THE SCENE X (lateral, + left) Y(vertical, + sky) Z(front, + back)
    dVector _Pos0(dVector(3* l_Hip,0.115f+(l_LPT / 2) * cos(LPT_A) + l_Up_Leg * cos(ThighR_A) + (l_Low_Leg + h_foot) * cos(ThighR_A - Shank_A), -0.2f - (l_LPT / 2) * sin(LPT_A))); // LPT

    dVector _Pos1(dVector(l_Hip, -(l_LPT / 2) * cos(LPT_A) - (l_Up_Leg / 2) * cos(ThighR_A), (l_LPT / 2) * sin(LPT_A) - (l_Up_Leg / 2) * sin(ThighR_A)));// Thigh_r
    dVector _Pos2(dVector(0.0f, -(l_Up_Leg / 2) * cos(ThighR_A) - (l_Low_Leg / 2) * cos(ThighR_A - Shank_A), -(l_Up_Leg / 2) * sin(ThighR_A) - (l_Low_Leg / 2) * sin(ThighR_A - Shank_A)));// Shank_r 
    dVector _Pos3(dVector(0, -((l_Low_Leg + h_foot) / 2) * cos(ThighR_A - Shank_A) + (l_foot / 3) * sin(-Foot_A), -(l_foot / 3) * cos(-Foot_A) - (l_Low_Leg / 2) * sin(ThighR_A - Shank_A)));// Foot_r 

    dVector _Pos4(dVector(-l_Hip, -(l_LPT / 2) * cos(LPT_A) - (l_Up_Leg / 2) * cos(ThighL_A), (l_LPT / 2)* sin(LPT_A) - (l_Up_Leg / 2) * sin(ThighL_A)));// Thigh_l
    dVector _Pos5(dVector(0.0f, -(l_Up_Leg / 2) * cos(ThighL_A) - (l_Low_Leg / 2) * cos(ThighL_A - Shank_A), -(l_Up_Leg / 2) * sin(ThighL_A) - (l_Low_Leg / 2) * sin(ThighL_A - Shank_A)));// Shank_l
    dVector _Pos9(dVector(0, -((l_Low_Leg + h_foot) / 2) * cos(ThighL_A - Shank_A) + (l_foot / 3) * sin(-Foot_A), -(l_foot / 3) * cos(-Foot_A) - (l_Low_Leg / 2) * sin(ThighL_A - Shank_A)));// Foot_l

    dVector _Pos6(dVector(0.0f, ((l_LPT + l_MPT) / 2) * cos(LPT_A), -((l_LPT + l_MPT) / 2) * sin(LPT_A)));// MPT
    dVector _Pos7(dVector(0.0f, ((l_MPT + l_UPT) / 2) * cos(LPT_A), -((l_MPT + l_UPT) / 2) * sin(LPT_A)));// UPT

    dVector _Pos8(dVector(0.0f, (l_foot + l_toe) / 2 * sin(- Foot_A + ThighR_A - Shank_A), -(l_foot + l_toe) / 2 * cos(-Foot_A + ThighR_A - Shank_A)));// toe r
    dVector _Pos10(dVector(0.0f, (l_foot + l_toe) / 2 * sin(-Foot_A + ThighL_A - Shank_A), -(l_foot + l_toe) / 2 * cos(-Foot_A + ThighL_A - Shank_A)));// toe l 

    dVector _Pos11(dVector(l_Clav, (l_UPT / 2) * cos(LPT_A) - (l_Up_Arm / 2) * cos(Shoulder_sagittal_R+LPT_A), -(l_UPT / 2) * sin(LPT_A) + (l_Up_Arm / 2) * sin(Shoulder_sagittal_R + LPT_A)));// UpArm r
    dVector _Pos12(dVector((l_Low_Arm / 2) * sin(Shoulder_transverse), - (l_Up_Arm/2) * cos(Shoulder_sagittal_R + LPT_A) - (l_Low_Arm/ 2)*cos(Elbow_A - (Shoulder_sagittal_R + LPT_A)), (l_Up_Arm / 2) * sin(Shoulder_sagittal_R + LPT_A) - (l_Low_Arm / 2) * sin(Elbow_A - (Shoulder_sagittal_R + LPT_A))* cos(-Shoulder_transverse)));// ForeArm r
    dVector _Pos13(dVector((l_Low_Arm + l_Hand) / 2 * sin(Shoulder_transverse), -((l_Low_Arm + l_Hand) / 2) * cos(Elbow_A - (Shoulder_sagittal_R + LPT_A)), -((l_Low_Arm + l_Hand) / 2) * sin(Elbow_A - (Shoulder_sagittal_R + LPT_A)) * cos(-Shoulder_transverse)));// hand r

    dVector _Pos14(dVector(-l_Clav, (l_UPT / 2) * cos(LPT_A) - (l_Up_Arm / 2) * cos(Shoulder_sagittal_L + LPT_A), -(l_UPT / 2) * sin(LPT_A) + (l_Up_Arm / 2) * sin(Shoulder_sagittal_L + LPT_A)));// UpArm l
    dVector _Pos15(dVector((l_Low_Arm / 2) * sin(-Shoulder_transverse), -(l_Up_Arm / 2) * cos(Shoulder_sagittal_L + LPT_A) - (l_Low_Arm / 2) * cos(Elbow_A - (Shoulder_sagittal_L + LPT_A)), (l_Up_Arm / 2) * sin(Shoulder_sagittal_L + LPT_A) - (l_Low_Arm / 2) * sin(Elbow_A - (Shoulder_sagittal_L + LPT_A)) * cos(Shoulder_transverse)));// ForeArm l
    dVector _Pos16(dVector((l_Low_Arm + l_Hand) / 2 * sin(-Shoulder_transverse), -((l_Low_Arm + l_Hand) / 2) * cos(Elbow_A - (Shoulder_sagittal_L + LPT_A)), -((l_Low_Arm + l_Hand) / 2) * sin(Elbow_A - (Shoulder_sagittal_L + LPT_A)) * cos(Shoulder_transverse)));// hand l

    dVector _Pos17(dVector(0.0f, (l_UPT / 2) * cos(LPT_A) + (l_Head / 2) * cos(Head_A), -(l_UPT / 2) * sin(LPT_A) - (l_Head / 2) * sin(Head_A)));// head

    std::vector< dVector* > body_pos_vec = { &_Pos0, &_Pos1, &_Pos2, &_Pos3,&_Pos4, &_Pos5, &_Pos9, &_Pos6, &_Pos7, &_Pos8, &_Pos10, &_Pos11, &_Pos12, &_Pos13 , &_Pos14, &_Pos15, &_Pos16, &_Pos17 };// vector with coordinates of bodies' geometric center

    // COM RELATIVE POSITION in local coords for each body 
    dVector _com0(dVector(delta_cm.find("LPT")->second * cos(LPT_A), 0.0f, delta_cm.find("LPT")->second * sin(LPT_A))); // LPT

    dVector _com1(dVector(delta_cm.find("Thigh")->second * cos(ThighR_A), 0.0f, delta_cm.find("Thigh")->second * sin(ThighR_A)));// Thigh_r l
    dVector _com2(dVector(delta_cm.find("Shank")->second * cos(ThighR_A - Shank_A), 0.0f, delta_cm.find("Shank")->second * sin(ThighR_A - Shank_A)));// Shank_r  l
    dVector _com3(dVector(delta_cm.find("Foot")->second * sin(-Foot_A), 0.0f, delta_cm.find("Foot")->second * cos(-Foot_A) ));// Foot_r l

    dVector _com4(dVector(delta_cm.find("MPT")->second * cos(LPT_A), 0.0f, delta_cm.find("MPT")->second * sin(LPT_A)));// MPT
    dVector _com5(dVector(delta_cm.find("UPT")->second * cos(LPT_A), 0.0f, delta_cm.find("UPT")->second * sin(LPT_A)));// UPT

    dVector _com6(dVector(0.0f, 0.0f, 0.0f));// toe r l

    dVector _com7(dVector(delta_cm.find("UpArm")->second * cos(Shoulder_sagittal_R + LPT_A), 0.0f, delta_cm.find("UpArm")->second * sin(Shoulder_sagittal_R + LPT_A)));// UpArm r 
    dVector _com8(dVector(delta_cm.find("ForeArm")->second * cos(Elbow_A - (Shoulder_sagittal_R + LPT_A)), delta_cm.find("ForeArm")->second * sin(Shoulder_transverse), delta_cm.find("ForeArm")->second * sin(Elbow_A - (Shoulder_sagittal_R + LPT_A)) * cos(-Shoulder_transverse)));// ForeArm r
    dVector _com9(dVector(delta_cm.find("Hand")->second * cos(Elbow_A - (Shoulder_sagittal_R + LPT_A)), delta_cm.find("Hand")->second * sin(Shoulder_transverse), delta_cm.find("Hand")->second * sin(Elbow_A - (Shoulder_sagittal_R + LPT_A)) * cos(-Shoulder_transverse)));// hand r

    dVector _com10(dVector(delta_cm.find("Head")->second * cos(Head_A), 0.0f, delta_cm.find("Head")->second * sin(Head_A)));// head

    dVector _com11(dVector(delta_cm.find("Thigh")->second * cos(ThighL_A), 0.0f, delta_cm.find("Thigh")->second * sin(ThighL_A)));// Thigh_l
    dVector _com12(dVector(delta_cm.find("Shank")->second * cos(ThighL_A - Shank_A), 0.0f, delta_cm.find("Shank")->second * sin(ThighL_A - Shank_A)));// Shank_l

    dVector _com13(dVector(delta_cm.find("UpArm")->second * cos(Shoulder_sagittal_L + LPT_A), 0.0f, delta_cm.find("UpArm")->second * sin(Shoulder_sagittal_L + LPT_A)));// UpArm l
    dVector _com14(dVector(delta_cm.find("ForeArm")->second * cos(Elbow_A - (Shoulder_sagittal_L + LPT_A)), delta_cm.find("ForeArm")->second * sin(-Shoulder_transverse), delta_cm.find("ForeArm")->second * sin(Elbow_A - (Shoulder_sagittal_L + LPT_A)) * cos(Shoulder_transverse)));// ForeArm l
    dVector _com15(dVector(delta_cm.find("Hand")->second * cos(Elbow_A - (Shoulder_sagittal_L + LPT_A)), delta_cm.find("Hand")->second * sin(-Shoulder_transverse), delta_cm.find("Hand")->second * sin(Elbow_A - (Shoulder_sagittal_L + LPT_A)) * cos(Shoulder_transverse)));// hand l

    std::vector< dVector* > body_com_vec = { &_com0, &_com1, &_com2, &_com3,&_com11, &_com12, &_com3, &_com4, &_com5, &_com6, &_com6, &_com7, &_com8, &_com9 , &_com13, &_com14, &_com15, &_com10 };// vector with relative coordinates of bodies' com

    dVector dim0(dVector(r_bones, r_bones, l_LPT));
    dVector dim1(dVector(r_bones, r_bones, l_Up_Leg));// Thigh_r and l
    dVector dim2(dVector(r_bones, r_bones, l_Low_Leg));// Shank_r and l
    dVector dim3(dVector(h_foot, w_foot, l_foot));// Foot_r and l
    dVector dim4(dVector(r_bones, r_bones, l_MPT));// MPT
    dVector dim5(dVector(r_bones, r_bones, l_UPT));// UPT
    dVector dim6(dVector(h_foot, w_foot, l_toe));// toe l and r
    dVector dim7(dVector(r_bones, r_bones, l_Up_Arm));// UpArm l UpArm r
    dVector dim8(dVector(r_bones, r_bones, l_Low_Arm));// LArm lLArm r
    dVector dim9(dVector(r_bones/2, r_bones/2, l_Hand));// LArm lLArm r
    dVector dim10(dVector(r_bones, r_bones, l_Head));// head

    std::vector< dVector* >body_dim_vec = { &dim0, &dim1, &dim2, &dim3, &dim1, &dim2, &dim3, &dim4, &dim5, &dim6, &dim6, &dim7, &dim8, &dim9 , &dim7, &dim8, &dim9, &dim10};// vector with bodies' dimensions

    // BODIES OF THE DUMMY DEFINITION
    std:vector<float> body_rot_ang_vect = { LPT_A * dRadToDegree ,
                                        -ThighR_A * dRadToDegree - LPT_A * dRadToDegree, (Shank_A)*dRadToDegree,Foot_A * dRadToDegree,// angles of each body LPT Thigh_r Shank_r Foot_r
                                        -ThighL_A * dRadToDegree - LPT_A * dRadToDegree, (Shank_A)*dRadToDegree,Foot_A * dRadToDegree,//Thigh_l Shank_l Foot_l
                                        0.0f, 0.0f,// MPT, UPT
                                        0.0f, 0.0f,//toe l and r
                                        Shoulder_sagittal_R* dRadToDegree, -Elbow_A* dRadToDegree, 0.0f,// UpArm r LoeArm r Hand r
                                        Shoulder_sagittal_L* dRadToDegree, -Elbow_A* dRadToDegree, 0.0f,// UpArm l LoeArm l Hand l
                                        Head_A* dRadToDegree-LPT_A* dRadToDegree };//head
    int aa = 0;
    for (std::vector<std::string>::iterator it = body_keys.begin(); it != body_keys.end(); it++)
    {
        body_rot_ang[*it] = body_rot_ang_vect[aa];
        body_pos[*it] = body_pos_vec[aa];
        body_dim[*it] = body_dim_vec[aa];
        body_com[*it] = body_com_vec[aa];
        rigid_element[*it] = NULL;
        nodes[*it] = this;
        aa++;
    };
    ////// END of MECHANIDAL DATA /////////////////
    
    //////// MODEL DEFINITION //////////////////
    AssemblyCreation();
    // each plantar is made of three spheres
    CreateDescreteContactFootCollision(rigid_element["Foot_r"]->GetBody());
    CreateDescreteContactFootCollision(rigid_element["Foot_l"]->GetBody());
    //// each toe is one sphere
    //NewtonWorld* const world = NewtonBodyGetWorld(rigid_element["Toe_r"]->GetBody());
    //NewtonCollision* const sphereCollision = NewtonCreateSphere(world, 0.05f, 0, NULL);
    //NewtonBodySetCollision(rigid_element["Toe_r"]->GetBody(), sphereCollision);
    //NewtonBodySetCollision(rigid_element["Toe_l"]->GetBody(), sphereCollision);
    //NewtonDestroyCollision(sphereCollision);
    //////// END of MODEL DEFINITION //////////////////

    //////// MUSCLE DEFINITION //////////////////
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
    m_point1 = { {"hfl_r", dVector(-l_LPT / 2, -l_Hip, -4 * r_bones)},    {"hfl_l", dVector(-l_LPT / 2, l_Hip, -4 * r_bones)},
        {"glu_r", dVector(-l_LPT / 2, -l_Hip, 4 * r_bones)},            {"glu_l", dVector(-l_LPT / 2, l_Hip, 4 * r_bones)},
        {"ham_r",dVector(-l_LPT / 2, -l_Hip, 4 * r_bones)},             {"ham_l",dVector(-l_LPT / 2,  l_Hip, 4 * r_bones)},
        {"rf_r",dVector(-l_LPT / 2, -l_Hip, -4 * r_bones)},             {"rf_l",dVector(-l_LPT / 2,   l_Hip, -4 * r_bones)},
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
    m_theta0 = { {"hfl_r", (180) * dDegreeToRad - ThighR_A -LPT_A},    {"hfl_l", (180) * dDegreeToRad - ThighL_A - LPT_A},
        {"glu_r",(180) * dDegreeToRad + ThighR_A + LPT_A},              {"glu_l",(180) * dDegreeToRad + ThighL_A + LPT_A},
        {"ham_r",180 * dDegreeToRad + ThighR_A + LPT_A},                {"ham_l",180 * dDegreeToRad + ThighL_A + LPT_A},
        {"rf_r",180 * dDegreeToRad - ThighR_A - LPT_A},                 {"rf_l",180 * dDegreeToRad - ThighL_A - LPT_A},
        {"vas_r",180 * dDegreeToRad + Shank_A},               {"vas_l",180 * dDegreeToRad + Shank_A},
        {"sol_r",2 * M_PI - (M_PI/2 + Foot_A)},                 {"sol_l",2 * M_PI - (M_PI/2 + Foot_A)},
        {"ta_r", M_PI/2 + Foot_A},                               {"ta_l",M_PI/2 + Foot_A},
        {"gas_r",180 * dDegreeToRad - Shank_A},               {"gas_l",180 * dDegreeToRad - Shank_A} };/////// list muscle initial angle 1
    m_theta10 = { {"ham_r",180 * dDegreeToRad - Shank_A}, {"ham_l",180 * dDegreeToRad - Shank_A},
        {"rf_r",180 * dDegreeToRad + Shank_A},            {"rf_l",180 * dDegreeToRad + Shank_A},
        {"gas_r",2 * M_PI - (M_PI/2 +Foot_A)},              {"gas_l",2 * M_PI - (M_PI/2 + Foot_A)} };/////list muscle initial angle 2
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

        index++;
    }
    //////// END of MUSCLE DEFINITION //////////////////

    // Check for body com position!
    COMXline_id = CreateLine();
    COMYline_id = CreateLine(); 
    COMZline_id = CreateLine(); 
    
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
// create 3 point collision for the foot. Same method of BalancingBibep.cpp in DemoSandBox 
void dRaycastVHModel::CreateDescreteContactFootCollision(NewtonBody* const footBody) const
{
    NewtonWorld* const world = NewtonBodyGetWorld(footBody);
    NewtonCollision* const sphereCollision = NewtonCreateSphere(world, h_foot/2, 0, NULL);

    dMatrix subShapeLocation(dGetIdentityMatrix());
    NewtonCollision* const threePointCollision = NewtonCreateCompoundCollision(world, 0);
    NewtonCompoundCollisionBeginAddRemove(threePointCollision);

    dFloat height = h_foot/2;
    dFloat widthSeparation = w_foot/2- height;
    dFloat frontSeparation = l_foot/2- height;

#if 1
    // tree points contacts
    subShapeLocation.m_posit.m_z = frontSeparation;
    subShapeLocation.m_posit.m_x = 0.0f;
    subShapeLocation.m_posit.m_y = 0.0f;
    NewtonCollisionSetMatrix(sphereCollision, &subShapeLocation[0][0]);
    NewtonCompoundCollisionAddSubCollision(threePointCollision, sphereCollision);

#else
    // four points contacts
    subShapeLocation.m_posit.m_z = frontSeparation;
    subShapeLocation.m_posit.m_x = 0.0f;
    subShapeLocation.m_posit.m_y = widthSeparation;
    NewtonCollisionSetMatrix(sphereCollision, &subShapeLocation[0][0]);
    NewtonCompoundCollisionAddSubCollision(threePointCollision, sphereCollision);

    subShapeLocation.m_posit.m_z = frontSeparation;
    subShapeLocation.m_posit.m_x = 0.0f;
    subShapeLocation.m_posit.m_y = -widthSeparation;
    NewtonCollisionSetMatrix(sphereCollision, &subShapeLocation[0][0]);
    NewtonCompoundCollisionAddSubCollision(threePointCollision, sphereCollision);
#endif

    subShapeLocation.m_posit.m_z = -frontSeparation;
    subShapeLocation.m_posit.m_x = 0.0f;
    subShapeLocation.m_posit.m_y = widthSeparation;
    NewtonCollisionSetMatrix(sphereCollision, &subShapeLocation[0][0]);
    NewtonCompoundCollisionAddSubCollision(threePointCollision, sphereCollision);

    subShapeLocation.m_posit.m_z = -frontSeparation;
    subShapeLocation.m_posit.m_x = 0.0f;
    subShapeLocation.m_posit.m_y = -widthSeparation;
    NewtonCollisionSetMatrix(sphereCollision, &subShapeLocation[0][0]);
    NewtonCompoundCollisionAddSubCollision(threePointCollision, sphereCollision);

    NewtonCompoundCollisionEndAddRemove(threePointCollision);

    NewtonBodySetCollision(footBody, threePointCollision);

    NewtonDestroyCollision(sphereCollision);
    NewtonDestroyCollision(threePointCollision);
}
// Compute trunk or head orientation and rotation velocity in sagittal plane 
vector<dFloat> dRaycastVHModel::GetSagittalState(string body)
{
    dMatrix mat = rigid_element.find(body)->second->GetMatrix();
    dVector dir = dVector(0, mat.m_front.m_y, mat.m_front.m_z, 1);
    dFloat ang = ComputeAngleBWVectors(dir, dVector(0, 1, 0, 1), dVector(1, 0, 0, 1));

    // save the current joint Omega
    dVector omega0(0.0f);
    NewtonBodyGetOmega(rigid_element.find(body)->second->GetBody(), &omega0[0]);
    return { ang, omega0.m_x };
}

// Compute trunk orientation and rotation velocity in  coronal plane
vector<dFloat> dRaycastVHModel::GetTrunkCoronalState()
{   
    dMatrix mat = rigid_element.find("LPT")->second->GetMatrix();
    dVector dir = dVector(mat.m_front.m_x, mat.m_front.m_y,0, 1);
    dFloat ang = ComputeAngleBWVectors(dir, dVector(0, 1, 0, 1), dVector(0, 0, 1, 1));

    // save the current joint Omega
    dVector omega0(0.0f);
    NewtonBodyGetOmega(rigid_element.find("LPT")->second->GetBody(), &omega0[0]);
    return { ang, omega0.m_z };
}
// Compute trunk orientation and rotation velocity in  frontal plane
vector<dFloat> dRaycastVHModel::GetTrunkTransverseState()
{
    dMatrix mat = rigid_element.find("LPT")->second->GetMatrix();
    dVector dir = dVector(mat.m_right.m_x, 0, mat.m_right.m_z, 1);
    dFloat ang = ComputeAngleBWVectors(dir, dVector(0, 0, 1, 1), dVector(0, -1, 0, 1));

    // save the current joint Omega
    dVector omega0(0.0f);
    NewtonBodyGetOmega(rigid_element.find("LPT")->second->GetBody(), &omega0[0]);
    return { ang, -omega0.m_y };
}
vector<dFloat> dRaycastVHModel::GetFootCoronalState(string body)
{
    dMatrix mat = rigid_element.find(body)->second->GetMatrix();
    dVector dir = { mat.m_up.m_x, mat.m_up.m_y, 0.0, 1.0 };
    dFloat ang = ComputeAngleBWVectors(dir, dVector(-1, 0, 0, 1), dVector(0, 0, -1, 1));

    // save the current joint Omega
    dVector omega0(0.0f);
    NewtonBodyGetOmega(rigid_element.find(body)->second->GetBody(), &omega0[0]);
    return { ang, -omega0.m_z };
}

vector<dFloat> dRaycastVHModel::GetRelativeSagittalState(string body1, string body2)
{
    dMatrix mat = rigid_element.find(body2)->second->GetMatrix();
    dVector dir = { 0.0, mat.m_front.m_y, mat.m_front.m_z, 1.0 };
    mat = rigid_element.find(body1)->second->GetMatrix();
    dVector ref = {0.0, mat.m_front.m_y, mat.m_front.m_z, 1.0 };
    dFloat ang = ComputeAngleBWVectors(dir, ref, mat.m_up);

    // save the current joint Omega
    dVector omega0(0.0f);
    NewtonBodyGetOmega(rigid_element.find(body2)->second->GetBody(), &omega0[0]);
    return { -ang, omega0.m_x };
}

vector<dFloat> dRaycastVHModel::GetRelativeTransverseState(string body1, string body2)
{
    dMatrix mat = rigid_element.find(body2)->second->GetMatrix();
    dVector dir = { mat.m_right.m_x, 0.0, mat.m_right.m_z, 1.0 };
    mat = rigid_element.find(body1)->second->GetMatrix();
    dVector ref = { mat.m_right.m_x, 0.0, mat.m_right.m_z, 1.0 };
    dFloat ang = ComputeAngleBWVectors(dir, ref, mat.m_front);

    // save the current joint Omega
    dVector omega0(0.0f);
    NewtonBodyGetOmega(rigid_element.find(body2)->second->GetBody(), &omega0[0]);
    return { -ang, omega0.m_y };
}

float dRaycastVHModel::ComputeAngleBWVectors(dVector const& dir, dVector const& ref, dVector const& crossref)
{
    dVector cross = dir.CrossProduct(ref);
    float norm = sqrt(pow(cross.m_x, 2) + pow(cross.m_y, 2) + pow(cross.m_z, 2));// norm of vector
    dFloat ang = dAtan2(norm, dir.DotProduct3(ref));

    if (cross.DotProduct3(crossref) > 0) {// sign og angle
        ang = -ang;
    }
    return ang;
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


void dRaycastVHModel::AssemblyCreation()
{
    for (std::vector<std::string>::iterator it = body_keys.begin(); it != body_keys.end(); it++)// rigid body list initialization
    {
        rigid_element[*it] = NULL;
    }
    for (std::vector<std::string>::iterator it = DHjoint_keys.begin(); it != DHjoint_keys.end(); it++)// double hinge  list initialization
    {
        JDoubleHinge[*it] = NULL;
    }
    for (std::vector<std::string>::iterator it = Hjoint_keys.begin(); it != Hjoint_keys.end(); it++)// hinge list initialization
    {
        JHinge[*it] = NULL;
    }
    
    TrunkCreation();
    LegCreation("LPT","Thigh_r","Shank_r", "Foot_r","Toe_r", "Hip_r", "Knee_r", "Ankle_r", "Toe_hinge_r");
    LegCreation("LPT", "Thigh_l", "Shank_l", "Foot_l", "Toe_l", "Hip_l", "Knee_l", "Ankle_l", "Toe_hinge_l");

    ArmCreation("UPT", "UpArm_r", "ForeArm_r", "Hand_r", "Shoulder_r", "Elbow_r", "Wrist_r");
    ArmCreation("UPT", "UpArm_l", "ForeArm_l", "Hand_l", "Shoulder_l", "Elbow_l", "Wrist_l");

    RigidBodyCreation("Head");
    dVector pos;
    pos = { 0.0f, (l_UPT / 2) * cos(LPT_A), -(l_UPT / 2) * sin(LPT_A) };// head
    HingeJoint("Neck", "UPT", "Head", pos, -50 * dDegreeToRad, 80 * dDegreeToRad);
}

void dRaycastVHModel::ArmCreation(string trunk, string uparm, string farm, string hand, string shoulder, string elbow, string wrist)
{
    RigidBodyCreation(uparm);
    RigidBodyCreation(farm);
    RigidBodyCreation(hand);

    float s=1, ang, Amin, Amax, ShoulderA;
    if (uparm.back() == 'r') {
       s = 1;
    ////    ang = ThighR_A;
        Amin = -20 * dDegreeToRad;//-90
        Amax = 20 * dDegreeToRad;// 0
        ShoulderA = Shoulder_sagittal_R;
    }
    else {
       s = -1;
    ////    ang = ThighL_A;
        Amin = 20 * dDegreeToRad;//0
        ShoulderA = Shoulder_sagittal_L;
        Amax = -20 * dDegreeToRad;//-90
   }

    dVector pos;
    pos = { s * l_Clav,  (l_UPT / 2) * cos(LPT_A), -(l_UPT / 2) * sin(LPT_A) };
    DoubleHingeJoint(shoulder, trunk, uparm, pos, Amin, Amax, -90 * dDegreeToRad - ShoulderA, 45 * dDegreeToRad - ShoulderA);// shoulder

    pos = { 0.0f,-(l_Up_Arm / 2) * cos(ShoulderA + LPT_A), (l_Up_Arm / 2) * sin(ShoulderA + LPT_A) };// elbow
    HingeJoint(elbow, uparm, farm, pos, -170 * dDegreeToRad + Elbow_A, 0.0f +Elbow_A);

    pos = { (l_Low_Arm/2) * sin(s*Shoulder_transverse), -((l_Low_Arm) / 2) * cos(Elbow_A - (ShoulderA + LPT_A)), -((l_Low_Arm) / 2) * sin(Elbow_A - (ShoulderA + LPT_A)) * cos(s*Shoulder_transverse) };
    HingeJoint(wrist, farm, hand, pos, -0.01f, 0.01f);// wrist
}

void dRaycastVHModel::LegCreation(string trunk, string thigh, string shank, string foot, string toes, string hip, string knee, string ankle, string toe_hinge)
{
    RigidBodyCreation(thigh);
    RigidBodyCreation(shank);
    RigidBodyCreation(foot);
    RigidBodyCreation(toes);
    float s, ang, Amin, Amax, Aminh, Amaxh;
    if (thigh.back() == 'r') {
        s = 1;
        ang = ThighR_A;
        Amin = 2 * dDegreeToRad;
        Amax = -35 * dDegreeToRad;
        Aminh = -20 * dDegreeToRad;
        Amaxh = 10 * dDegreeToRad;//5
    }
    else {
        s = -1;
        ang = ThighL_A;
        Amin = 35 * dDegreeToRad; 
        Amax = -2 * dDegreeToRad;
        Aminh = -10 * dDegreeToRad;//-5
        Amaxh = 20 * dDegreeToRad;
    }

    dVector pos;
    pos = { s*l_Hip, -l_LPT / 2 * cos(LPT_A), l_LPT / 2 * sin(LPT_A) };
    DoubleHingeJoint(hip, trunk, thigh, pos, -90*dDegreeToRad + ang, 45 * dDegreeToRad + ang, Aminh, Amaxh);// Hip

    pos = { 0.0f, -(l_Up_Leg / 2 - knee_j) * cos(ang), -(l_Up_Leg / 2 - knee_j) * sin(ang) };// Knee
    HingeJoint(knee, thigh, shank, pos, -Shank_A, 160*dDegreeToRad - Shank_A);

    pos = { 0.0f,  -(l_Low_Leg / 2+ ankle_j) * cos(-Shank_A + ang), -(l_Low_Leg / 2+ ankle_j) * sin(-Shank_A + ang) };
    DoubleHingeJoint(ankle, shank, foot, pos, -90* dDegreeToRad + Foot_A, 90 * dDegreeToRad+ Foot_A, Amin, Amax);// Ankle

    pos = { 0.0f,  l_foot / 2 * sin(-Foot_A + ang - Shank_A),  -l_foot / 2 * cos(-Foot_A + ang - Shank_A) };
    HingeJoint(toe_hinge, foot, toes, pos, -30*dDegreeToRad, 0 * dDegreeToRad);// Toes

}

void dRaycastVHModel::TrunkCreation()
{
    // Trunk creation
    RigidBodyCreation("LPT");
    RigidBodyCreation("MPT");
    RigidBodyCreation("UPT");

    dVector pos;
    pos = {0.0f, l_LPT/2*cos(LPT_A), -l_LPT / 2 * sin(LPT_A) };
    HingeJoint("Spine1", "LPT", "MPT", pos, -0.01f, 0.01f);
    pos = { 0.0f, l_MPT / 2 * cos(LPT_A), -l_MPT / 2 * sin(LPT_A) };
    HingeJoint("Spine2", "MPT", "UPT", pos, -0.01f, 0.01f);

}
void dRaycastVHModel::HingeJoint(string jname, string body1, string body2, dVector pos, float minAng, float maxAng)
{
    GeomNewton* b1 = rigid_element.find(body1)->second;
    GeomNewton* b2 = rigid_element.find(body2)->second;
    dMatrix matrix(dGetIdentityMatrix()); // define the pin
    matrix.m_posit = dVector(b1->GetPosition().m_x + pos.m_x, b1->GetPosition().m_y + pos.m_y, b1->GetPosition().m_z + pos.m_z, 1.0f);
    JHinge[jname] = new dCustomHinge(matrix, b1->GetBody(), b2->GetBody());
    JHinge[jname]->EnableLimits(true);
    JHinge[jname]->SetLimits(minAng, maxAng);
    if (jname == "Knee_l") {
        m_winManager->aManager->vJointList.push_back(JHinge[jname]);
        m_winManager->aManager->vJointNameList.push_back(KNEE_L);
        //JHinge[jname]->SetLimits(-0.01, 0.01);
    }
    if (jname == "Knee_r") {
        m_winManager->aManager->vJointList.push_back(JHinge[jname]);
        m_winManager->aManager->vJointNameList.push_back(KNEE_R);
        //JHinge[jname]->SetLimits(-0.01, 0.01);
    }
    if (jname == "Toe_hinge_r" || jname == "Toe_hinge_l" || jname == "Wrist_r" || jname == "Wrist_l"){
        JHinge[jname]->SetAsSpringDamper(true, 0.99, 0);} // max stiffness zero damping
}
// apply titling motion to the floor. Input is the desired angle in rad at each timestep
void dRaycastVHModel::ApplyTilting(float angle)
{
    FloorAct->SetTargetAngle(angle);
}

void dRaycastVHModel::DoubleHingeJoint(string jname, string body1, string body2, dVector pos, float minAng1, float maxAng1, float minAng2, float maxAng2)
{
    GeomNewton* b1 = rigid_element.find(body1)->second;
    GeomNewton* b2 = rigid_element.find(body2)->second;
    dMatrix matrix(dGetIdentityMatrix()); // define the pin

    if(jname != "Shoulder_r" || jname != "Shoulder_l")
        matrix = matrix * dPitchMatrix(90.0f * dDegreeToRad); // DEFINE THE CORRECT DOF OF A DOUBLEHINGE (the second pin must be the one actuated by the muscle)

    if (jname == "Shoulder_r" || jname == "Shoulder_l"){
        matrix = matrix * dRollMatrix(90.0f * dDegreeToRad); // DEFINE THE CORRECT DOF OF A DOUBLEHINGE (the second pin must be the one actuated by the muscle)
        matrix = matrix * dYawMatrix(90.0f * dDegreeToRad); // DEFINE THE CORRECT DOF OF A DOUBLEHINGE (the second pin must be the one actuated by the muscle)
    }

    matrix.m_posit = dVector(b1->GetPosition().m_x + pos.m_x, b1->GetPosition().m_y + pos.m_y, b1->GetPosition().m_z + pos.m_z, 1.0f);
    JDoubleHinge[jname] = new dCustomDoubleHinge(matrix, b1->GetBody(), b2->GetBody());

    JDoubleHinge[jname]->EnableLimits(true);
    JDoubleHinge[jname]->SetLimits(minAng1, maxAng1);
    JDoubleHinge[jname]->EnableLimits1(true);
    JDoubleHinge[jname]->SetLimits1(minAng2, maxAng2);
   
    if (jname == "Ankle_r") {
        m_winManager->aManager->vJointList.push_back(JDoubleHinge[jname]);
        m_winManager->aManager->vJointNameList.push_back(ANKLE_R);
        //JDoubleHinge[jname]->SetLimits(-0.01, 0.01);
    }
    if (jname == "Ankle_l") {
        m_winManager->aManager->vJointList.push_back(JDoubleHinge[jname]);
        m_winManager->aManager->vJointNameList.push_back(ANKLE_L);
        //JDoubleHinge[jname]->SetLimits(-0.01, 0.01);
    }
    if (jname == "Hip_r") {
        m_winManager->aManager->vJointList.push_back(JDoubleHinge[jname]);
        m_winManager->aManager->vJointNameList.push_back(HIP_R);
        //JDoubleHinge[jname]->SetLimits(-0.01, 0.01);
    }
    if (jname == "Hip_l") {
        m_winManager->aManager->vJointList.push_back(JDoubleHinge[jname]);
        m_winManager->aManager->vJointNameList.push_back(HIP_L);
        //JDoubleHinge[jname]->SetLimits(-0.01, 0.01);
    }
}

float dRaycastVHModel::GetHipLength()
{
    return l_Hip;
}

float dRaycastVHModel::GetModelMass()
{
    return Model_Mass;
}

void dRaycastVHModel::RigidBodyCreation(string body)
{
    string tex("Textures//wood6.png");
    std::string mass_prop_key;
    if (body == "LPT" || body == "MPT" || body == "UPT" || body == "Head")
        mass_prop_key = body;
    else
    {
        string name = body;
        name.pop_back();
        name.pop_back();
        mass_prop_key = name;
    }
    
    rigid_element[body] = new GeomNewton(m_winManager->aManager);
    rigid_element[body]->SetBodyType(adtDynamic);
    if (body != "LPT")
        rigid_element[body]->SetParent(rigid_element[child_father.find(body)->second]);

    rigid_element[body]->SetTexture0(&tex[0], "Tex0");
    rigid_element[body]->SetDiffuseColor(1.0f, 1.0f, 1.0f);

    if (body == "LPT")
        rigid_element[body]->SetRollAngle(90, false);
    rigid_element[body]->SetTurnAngle(body_rot_ang[body], false);
    if (body == "UpArm_r")
        rigid_element[body]->SetPitchAngle(-Shoulder_transverse*dRadToDegree, false);
    else if (body == "UpArm_l")
        rigid_element[body]->SetPitchAngle(Shoulder_transverse * dRadToDegree, false);
    rigid_element[body]->SetPosition(body_pos[body]->m_x, body_pos[body]->m_y, body_pos[body]->m_z);

    if (body == "Foot_r" || body == "Foot_l" || body == "Toe_l" || body == "Toe_r")
        rigid_element[body]->InitNewton(atBox, body_dim[body]->m_x, body_dim[body]->m_y, body_dim[body]->m_z, 1.0f);
    else
        rigid_element[body]->InitNewton(atCapsule, body_dim[body]->m_x, body_dim[body]->m_y, body_dim[body]->m_z*0.5f, 1.0f);// smaller length of capsule to remove fake force due to compenetration of collision

    //if (body == "Thigh_l")// body == "LPT"|| body == "Foot_r" || body == "Foot_l"
    //    NewtonBodySetMassMatrix(rigid_element[body]->GetBody(), 0, Ixx[mass_prop_key], Iyy[mass_prop_key], Izz[mass_prop_key]);
    //else
        NewtonBodySetMassMatrix(rigid_element[body]->GetBody(), mass_distrib[mass_prop_key], Ixx[mass_prop_key], Iyy[mass_prop_key], Izz[mass_prop_key]);
    //to move center of mass
    dVector com = body_com[body][0];
    NewtonBodySetCentreOfMass(rigid_element[body]->GetBody(), &com[0]);


    if (body == "LPT"){
        m_body = rigid_element["LPT"]->GetBody();
        NewtonBodySetTransformCallback(m_body, NULL);}
    else {
        NewtonBodySetTransformCallback(rigid_element[body]->GetBody(), NULL);
        nodes[body] = new dModelNode(rigid_element[body]->GetBody(), dGetIdentityMatrix(), nodes[child_father.find(body)->second]);}
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

void dRaycastVHModel::AddActionReactionTorque(float Torque, dVector pin, GeomNewton* b1, GeomNewton* b2)
{
    if (b1 != NULL) {
        // Get the Body1 connected to the muscle and apply the muscle force
        pin = pin.Scale(-Torque); // apply torque on the correct axis
        NewtonBodyAddTorque(b1->GetBody(), &pin.m_x);
    }
    if (b2 != NULL) {
        // Get the Body2 connected and apply the opposite muscle force
        pin = pin.Scale(-1); // opposite force on second body
        NewtonBodyAddTorque(b2->GetBody(), &pin.m_x);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DGVehicleRCManager::DGVehicleRCManager(WindowMain* winctx)
: dModelManager(winctx->aManager->GetWorld()),
  m_winManager(winctx)
{
    
}

dRaycastVHModel* DGVehicleRCManager::CreateWalkerPlayer(const char* const modelName, const dMatrix& location)
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
    dFloat newTime = m_winManager->GetSimulationTime();
    dRaycastVHModel* Model = (dRaycastVHModel*)model;

    ////cout << "DGVehicleRCManager OnP reUpdate \n";
    map<std::string, GeomNewton*> RE_list = Model->Get_RigidElemetList();

    //INITIAL CONDITION //
    if (newTime == 0) {
        vector<float> ICv = Model->controller.GetInitialCondition();
        dVector LPTvel_init = { 0.0f,0.0f, ICv[6] };

        GeomNewton* BODY = RE_list.find("LPT")->second;
        NewtonBodySetVelocity(BODY->GetBody(), &LPTvel_init[0]);
        BODY = RE_list.find("UPT")->second;
        NewtonBodySetVelocity(BODY->GetBody(), &LPTvel_init[0]);

    }

    /// MUSCLE STUFF /////////////////////////////////////////////////////////////////////////////////////////////
    // find ankle position
    dVector com_Player = Model->ComputePlayerCOM();
    dVector com_Player_vel = Model->ComputePlayerCOMvelocity();
    Model->controller.SetPlayerCOMvel(com_Player_vel);

    // Control the dummy just if is upright
    if (com_Player.m_y > 0.6) {// guess value 
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
        //BODY0 = RE_list.find("Toe_r")->second;
        //count += GetContactPoints(BODY0->GetBody(), &supportPolygon[count]);
        vector<bool> state_foot_r = Model->controller.Stance_Swing_Detection(com_ankle_r, com_Player, Model->GetLegLength(), count);

        count = 0;
        count += GetContactPoints(BODY1->GetBody(), &supportPolygon[count]);
        //BODY1 = RE_list.find("Toe_l")->second;
        //count += GetContactPoints(BODY1->GetBody(), &supportPolygon[count]);
        vector<bool> state_foot_l = Model->controller.Stance_Swing_Detection(com_ankle_l, com_Player, Model->GetLegLength(), count);

        // Model com visualization
        Model->DrawFrame(com_Player, 1.0); // comment this line to check single body com position and use following lines
        //// Check for body com position! change manually the name of the body 
        //NewtonBody* body = RE_list.find("Hand_l")->second->GetBody();
        //dVector com;
        //NewtonBodyGetCentreOfMass(body, &com[0]);
        //com = Rel2AbsPoint(body, &com[0]);
        //Model->DrawFrame(com, 1.0);

        // leading leg: the one that is in heel strike (z coordinate of ankle is smaller than z coordinate of player com)
        char lead = 'L';
        bool DS = false; // double stance
        if (state_foot_r[0] == true && state_foot_l[0] == true)// double stance
        {
            DS = true;
            if (com_ankle_r.m_z < com_Player.m_z)// Check direzione maggiore/minore
                lead = 'R';
            else if (com_ankle_l.m_z < com_Player.m_z)
                lead = 'L';
        }
        else if (state_foot_r[0] == true && state_foot_l[0] == false)
            lead = 'R';
        else if (state_foot_r[0] == false && state_foot_l[0] == true)
            lead = 'L';
        else
            lead = 'N';// feet are both in air

        if (newTime < 0.1f) // initial condition
            lead = 'L';

        state_foot_l.push_back(DS); // add double stance state
        state_foot_r.push_back(DS); // add double stance state

        float d = 0;// compute horizontal distance between stance foot and com for stance preparation hip control
        if (!DS)
        {
            if (state_foot_r[0] == true)
                d = sqrt(pow(com_Player.m_z - com_ankle_r.m_z, 2) + pow(com_Player.m_x - com_ankle_r.m_x, 2));
            else if (state_foot_l[0] == true)
                d = sqrt(pow(com_Player.m_z - com_ankle_l.m_z, 2) + pow(com_Player.m_x - com_ankle_l.m_x, 2));
            Model->controller.SetSagittalDistance(d);
        }

        // update sagittal trunk angle and velocity
        vector<float> Strunk = Model->GetSagittalState("LPT");// trunk vertical vector projection on sagittal plane);
        Model->controller.SetState(Strunk[0], Strunk[1], "Strunk");

        vector<float> state0hipr = Model->controller.GetState0("Ship_r");
        vector<float> state0hipl = Model->controller.GetState0("Ship_l");
        vector<float> state0kneer = Model->controller.GetState0("Sknee_r");
        vector<float> state0kneel = Model->controller.GetState0("Sknee_l");
        vector<float> state0ankler = Model->controller.GetState0("Sankle_r");
        vector<float> state0anklel = Model->controller.GetState0("Sankle_l");

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
                            Model->controller.SetState(state0kneer[0] - joint->GetJointAngle(), joint->GetJointOmega(), "Sknee_r");
                        }
                        if (Mobj->m_name == GAS && lat == 'L') {
                            Model->controller.SetState(state0kneel[0] - joint->GetJointAngle(), joint->GetJointOmega(), "Sknee_l");
                        }
                    }
                    else
                    {
                        Mobj->SetAngle1(joint->GetJointAngle()); // joint angle
                        pin1 = joint->GetPinAxis(); // joint pin
                    }
                    break;
                }
                //case BallAndSocket: // NOT USED
                //{
                //    dCustomBallAndSocket* joint = (dCustomBallAndSocket*)m_winManager->aManager->vJointList[j_index]; // joint from jointlist
                //    //GetJointAngle() and GetPinAxis() methods and m_curJointAngle() must be added inside dCustomAndSocket.cpp and the respective header file 
                //    //Copy the dCustomHinge.cpp respective methods and build newton again. 
                //    //Copy new dlls in project folder
                //    if (itr1 == 0)
                //    {
                //        Mobj->SetAngle(joint->GetJointAngle()); // joint angle
                //        pin = joint->GetPinAxis(); // joint pin
                //    }
                //    else
                //    {
                //        Mobj->SetAngle1(joint->GetJointAngle()); // joint angle
                //        pin1 = joint->GetPinAxis(); // joint pin
                //    }
                //    break;
                //}
                case DoubleHinge:
                {
                    dCustomDoubleHinge* joint = (dCustomDoubleHinge*)m_winManager->aManager->vJointList[j_index];
                    if (itr1 == 0)
                    {
                        Mobj->SetAngle(joint->GetJointAngle());
                        pin = joint->GetPinAxis();
                        if (Mobj->m_name == HFL && lat == 'R') {
                            Model->controller.SetState(state0hipr[0] + joint->GetJointAngle(), joint->GetJointOmega(), "Ship_r");
                        }
                        if (Mobj->m_name == HFL && lat == 'L') {
                            Model->controller.SetState(state0hipl[0] + joint->GetJointAngle(), joint->GetJointOmega(), "Ship_l");
                        }
                        if (Mobj->m_name == TA && lat == 'R') {
                            Model->controller.SetState(state0ankler[0] + joint->GetJointAngle(), joint->GetJointOmega(), "Sankle_r");
                        }
                        if (Mobj->m_name == TA && lat == 'L') {
                            Model->controller.SetState(state0anklel[0] + joint->GetJointAngle(), joint->GetJointOmega(), "Sankle_l");
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
            float u = Model->controller.MTU_excitation(Mobj->m_name, Gait, lead, lat, lmtu / lopt, Fmuscle / Fmax, com_Player_vel);

            //Impose muscle as actuator
            Mobj->SetNeuralDelay(1.f / 3000.f); // 1.f/2400.f s

            // set activation
            Mobj->SetActivation(u);

            dVector Ttemp = Mobj->Compute_muscle_Torque(timestep);

            Model->AddActionReactionTorque(Ttemp.m_x, pin, Mobj->body1, Mobj->body2);
            if (Mobj->body3 != NULL)
                Model->AddActionReactionTorque(Ttemp.m_y, pin1, Mobj->body2, Mobj->body3);

            // update for next force update
            Mobj->SetLCE(Mobj->GetLCE() + Mobj->GetDelta_l()); // update lCE
            Model->controller.UpdateMuscleReward(abs(Ttemp.m_x) + abs(Ttemp.m_y));// update torque reward

            #if defined(_DEBUG)
                //// stamp u of all muscles
                //monFlux << u << "  ";// Use Test_excitations.m to see data

                // stamp torque of all muscles 
                monFlux << Ttemp.m_x << "  " << Ttemp.m_y << "  ";
            #endif
        }
        // Stuff for plots
        vector<float> statehipr = Model->controller.GetState("Ship_r");
        vector<float> statehipl = Model->controller.GetState("Ship_l");
        vector<float> statekneer = Model->controller.GetState("Sknee_r");
        vector<float> statekneel = Model->controller.GetState("Sknee_l");
        vector<float> stateankler = Model->controller.GetState("Sankle_r");
        vector<float> stateanklel = Model->controller.GetState("Sankle_l");
        vector<float> stateLPT = Model->controller.GetState("Ctrunk");

        #if defined(_DEBUG)
            // verify total torque on joints
            monFlux << newTime << "  " << statehipr[0] << "  " << statekneer[0] << "  " << stateankler[0]
                << "  " << statehipl[0] << "  " << statekneel[0] << "  " << stateanklel[0] << "  " << stateLPT[0] << "  " << stateLPT[1] <<
                "  " << state_foot_r[0] << "  " << state_foot_r[1] << "  " << state_foot_r[2] << "  " << state_foot_r[3] <<
                "  " << state_foot_l[0] << "  " << state_foot_l[1] << "  " << state_foot_l[2] << "  " << state_foot_l[3] << endl;

            //// verify excitations
            //monFlux << newTime << "  " <<statehipr[0] << "  " << statekneer[0] << "  " << stateankler[0] <<
            //    "  " << statehipl[0] << "  " << statekneel[0] << "  " << stateanklel[0] <<
            //    "  " << state_foot_r[0] << "  " << state_foot_r[1] << "  " << state_foot_r[2] << "  " << state_foot_r[3] <<
            //    "  " << state_foot_l[0] << "  " << state_foot_l[1] << "  " << state_foot_l[2] << "  " << state_foot_l[3] << "  " << stateLPT[0] <<endl;// check initial angle according to initial geometry
        #endif
        /// END OF MUSCLE STUFF /////////////////////////////////////////////////////////////////////////////

         // PD CONTROLLERS STUFF /////////////////////////////////////////////
        if (1) {
            float dx = 0;// compute horizontal distance between foot and com along x direction
            float torque;
            dVector pin(0.0, 0.0, 1.0);// pin for torque around Z axis
            // trunk coronal control
            vector<float> state = Model->GetTrunkCoronalState();
            Model->controller.SetState(state[0], state[1], "Ctrunk");

            dVector T;
            dx = (com_ankle_r.m_x - com_Player.m_x);
            //if (dx > 0.0f) {
                dx = -(dx - Model->GetHipLength());
                Model->controller.SetCoronalDistance(dx);
                torque = Model->controller.PD_controller("Ctrunk", lead == 'R');
                Model->AddActionReactionTorque(torque, pin, RE_list.find("LPT")->second, RE_list.find("Thigh_r")->second);
            //}

            dx = (com_Player.m_x - com_ankle_l.m_x);
            //if (dx > 0.0f) {
                dx = dx - Model->GetHipLength();
                Model->controller.SetCoronalDistance(dx);
                torque = Model->controller.PD_controller("Ctrunk", lead == 'L');
                Model->AddActionReactionTorque(torque, pin, RE_list.find("LPT")->second, RE_list.find("Thigh_l")->second);
            //}
            //// end trunk coronal control

            //cout << lead << endl;
            // RIGHT FOOT ORIENTATION CONTROL 
            state = Model->GetFootCoronalState("Foot_r");
            Model->controller.SetState(state[0], state[1], "Cfoot_r");

            torque = Model->controller.PD_controller("Cfoot_r",lead == 'R');
            Model->AddActionReactionTorque(torque, pin, RE_list.find("Shank_r")->second, RE_list.find("Foot_r")->second);

            //// LEFT FOOT ORIENTATION CONTROL 
            state = Model->GetFootCoronalState("Foot_l");
            Model->controller.SetState(state[0], state[1], "Cfoot_l");

            NewtonBodyGetTorque(RE_list.find("Shank_l")->second->GetBody(), &T.m_x);

            torque = Model->controller.PD_controller("Cfoot_l", lead == 'L');
            Model->AddActionReactionTorque(torque, pin, RE_list.find("Shank_l")->second, RE_list.find("Foot_l")->second);

            // trunk transverse control
            pin = dVector(0.0, 1.0, 0.0);// pin for torque around Y axis
            state = Model->GetTrunkTransverseState();
            Model->controller.SetState(state[0], state[1], "Ttrunk");

            torque = Model->controller.PD_controller("Ttrunk", false);
            Model->AddActionReactionTorque(torque, pin, RE_list.find("LPT")->second, NULL);
            // end trunk transverse control
            // 
            // head sagittal control
            vector<float> Shead = Model->GetSagittalState("Head");// head vertical vector projection on sagittal plane);
            Model->controller.SetState(Shead[0], Shead[1], "Shead");

            pin = dVector(1.0, 0.0, 0.0);// pin for torque around X axis
            torque = Model->controller.PD_controller("Shead", false);
            Model->AddActionReactionTorque(torque, pin, RE_list.find("Head")->second, RE_list.find("UPT")->second);
            // end head sagittal control
            
            // WIP: verificare che angolo relativo sia corretto in dinamica
            vector<float> shoulder_angles = Model->controller.GetShoulderTargetAngles(false,'r');

            // shoulder R sagittal control
            vector<float> SshoulderR = Model->GetRelativeSagittalState("UPT", "UpArm_r");// 
            Model->controller.SetState(-SshoulderR[0], -SshoulderR[1], "Sshoulder_r");
            Model->controller.SetShoulderAngleGain(shoulder_angles[0], "Sshoulder");

            pin = dVector(1.0, 0.0, 0.0);// pin for torque around X axis
            torque = Model->controller.PD_controller("Sshoulder_r", false);
            Model->AddActionReactionTorque(torque, pin, RE_list.find("UPT")->second, RE_list.find("UpArm_r")->second);
            // end shoulder R sagittal control

            // shoulder R transverse control
            vector<float> TshoulderR = Model->GetRelativeTransverseState("UPT", "UpArm_r");// 
            Model->controller.SetState(-TshoulderR[0], TshoulderR[1], "Tshoulder_r");
            Model->controller.SetShoulderAngleGain(shoulder_angles[1], "Tshoulder");

            pin = dVector(0.0, 1.0, 0.0);// pin for torque around Y axis
            torque = Model->controller.PD_controller("Tshoulder_r", false);
            Model->AddActionReactionTorque(-torque, pin, RE_list.find("UPT")->second, RE_list.find("UpArm_r")->second);
            // end shoulder R transverse control
            
            // elbow sagittal control
            vector<float> Selbow = Model->GetRelativeSagittalState("UpArm_r", "ForeArm_r");// 
            Model->controller.SetState(Selbow[0], Selbow[1], "Selbow_r");

            pin = dVector(1.0, 0.0, 0.0);// pin for torque around Y axis
            torque = Model->controller.PD_controller("Selbow_r", false);
            Model->AddActionReactionTorque(-torque, pin, RE_list.find("UpArm_r")->second, RE_list.find("ForeArm_r")->second);
            // end elbow sagittal control
            // 
            shoulder_angles = Model->controller.GetShoulderTargetAngles(false, 'l');

            // shoulder L sagittal control
            vector<float> SshoulderL = Model->GetRelativeSagittalState("UPT", "UpArm_l");// 
            Model->controller.SetState(-SshoulderL[0], -SshoulderL[1], "Sshoulder_l");
            Model->controller.SetShoulderAngleGain(shoulder_angles[0], "Sshoulder");

            pin = dVector(1.0, 0.0, 0.0);// pin for torque around X axis
            torque = Model->controller.PD_controller("Sshoulder_l", false);
            Model->AddActionReactionTorque(torque, pin, RE_list.find("UPT")->second, RE_list.find("UpArm_l")->second);
            // end shoulder R sagittal control

            // shoulder L transverse control
            vector<float> TshoulderL = Model->GetRelativeTransverseState("UPT", "UpArm_l");// 
            Model->controller.SetState(-TshoulderL[0], TshoulderL[1], "Tshoulder_l");
            Model->controller.SetShoulderAngleGain(shoulder_angles[1], "Tshoulder");

            pin = dVector(0.0, 1.0, 0.0);// pin for torque around Y axis
            torque = Model->controller.PD_controller("Tshoulder_l", false);
            Model->AddActionReactionTorque(-torque, pin, RE_list.find("UPT")->second, RE_list.find("UpArm_l")->second);
            // end shoulder R transverse control

            // elbow sagittal control
            Selbow = Model->GetRelativeSagittalState("UpArm_l", "ForeArm_l");// 
            Model->controller.SetState(Selbow[0], Selbow[1], "Selbow_l");

            pin = dVector(1.0, 0.0, 0.0);// pin for torque around Y axis
            torque = Model->controller.PD_controller("Selbow_l", false);
            Model->AddActionReactionTorque(-torque, pin, RE_list.find("UpArm_l")->second, RE_list.find("ForeArm_l")->second);
            // end elbow sagittal control
            // 
            // END OF PD CONTROLLERS STUFF /////////////////////////////////////////
        }
    }
    // COST FUNCTION COMPUTATION
    vector<dFloat> Strunk = Model->GetSagittalState("LPT");
    vector<dFloat> Ctrunk = Model->GetTrunkCoronalState();
    dVector COM_vel = Model->ComputePlayerCOMvelocity();
    Model->controller.UpdateTaskReward(com_Player.m_y, COM_vel, Strunk[0], Ctrunk[0]);
    
    // ADD WBV
    Model->ApplyTilting(8 * dDegreeToRad * sin(0.5 * 2 * M_PI * newTime)); // roll 8° 0.5 Hz

    newTime = newTime + timestep; // update time
    m_winManager->SetSimulationTime(newTime);

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