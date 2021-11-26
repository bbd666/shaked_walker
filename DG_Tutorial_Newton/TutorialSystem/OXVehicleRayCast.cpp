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
        else if (sParameter == "MaxMuscleForce")
        {
            num = dump_attribs_to_stdout(pParent->ToElement(), v_maxForce, indent + 1);
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

    // LOADING DATA FROM XML//
    TiXmlDocument doc("DummyGeometricProperties.xml");
    bool loadOkay = doc.LoadFile();
    (dump_to_stdout(&doc)); //parse the xml and load parameters
    if (v_scale.size() != 2 || lengths.size() != 19 || v_total_weight.size() != 1 || mass_distrib.size() != 12 || Ixx.size() != 12 || Iyy.size() != 12 || Izz.size() != 12 || delta_cm.size() != 12 || v_angles.size() != 2)
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

    
    // MASS DATA
    float tot_w = scale_mass * v_total_weight[0]; 
    for (map<std::string, float>::iterator it = mass_distrib.begin(); it != mass_distrib.end(); it++)
    {
        it->second = it->second * tot_w;
    };
    //INERTIA DATA
    for (map<std::string, float>::iterator it = Ixx.begin(); it != Ixx.end(); it++)
    {
        it->second = mass_distrib.find(it->first)->second * it->second;
    };
    for (map<std::string, float>::iterator it = Iyy.begin(); it != Iyy.end(); it++)
    {
        it->second = mass_distrib.find(it->first)->second * it->second;
    };
    for (map<std::string, float>::iterator it = Izz.begin(); it != Izz.end(); it++)
    {
        it->second = mass_distrib.find(it->first)->second * it->second;
    };


    // COM distance from the origin of each body (DIRECTION X: IN LOCAL COORDINATE OF THE BODY)
    for (map<std::string, float>::iterator it = delta_cm.begin(); it != delta_cm.end(); it++)
    {
        it->second = lengths.find(it->first)->second * (0.5f-it->second);
    };


    glm::vec3 _Pos(glm::vec3(0.0f, 1.5f, 0.f)); // INITIAL POSITION  OF THE DUMMY IN THE SCENE X (lateral) Y(vertical) Z(front)

    float min_hip_angle = 180 * dDegreeToRad;
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

    float mass, ix, iy, iz;
    UP_leg = new GeomNewton(m_winManager->aManager);
    UP_leg->SetBodyType(adtDynamic);
    UP_leg->SetParent(LPT);
    UP_leg->SetTexture0(&tex[0], "Tex0");
    UP_leg->SetDiffuseColor(1.0f, 1.0f, 1.0f);
    UP_leg->SetPosition(0, -l_LPT / 2 - l_Up_Leg / 2  - r_bones, 0);
    UP_leg->InitNewton(atCapsule, r_bones, r_bones, l_Up_Leg, 15.0f);
    NewtonBodyGetMass(UP_leg->GetBody(), &mass, &ix, &iy, &iz); // add values in local coordinates of body
    NewtonBodySetTransformCallback(UP_leg->GetBody(), NULL);
    N1 = new dModelNode(UP_leg->GetBody(), dGetIdentityMatrix(), this);

    LP_leg = new GeomNewton(m_winManager->aManager);
    LP_leg->SetBodyType(adtDynamic);
    LP_leg->SetParent(UP_leg);
    LP_leg->SetTexture0(&tex[0], "Tex0");
    LP_leg->SetDiffuseColor(1.0f, 1.0f, 1.0f);
    LP_leg->SetPosition(0,- l_Up_Leg - r_bones, 0);
    LP_leg->InitNewton(atCapsule, r_bones, r_bones, l_Up_Leg, 15.0f);
    NewtonBodyGetMass(LP_leg->GetBody(), &mass, &ix, &iy, &iz); // add values in local coordinates of body
    NewtonBodySetTransformCallback(LP_leg->GetBody(), NULL);
    N2 = new dModelNode(LP_leg->GetBody(), dGetIdentityMatrix(), N1);

    //// EX. BallAndSocket joint. 
    //dMatrix Ankle_PinMatrix(dGetIdentityMatrix()); // define the pin
    //Ankle_PinMatrix.m_posit = dVector(LPT->GetPosition().m_x, LPT->GetPosition().m_y - l_LPT / 2 - r_bones, LPT->GetPosition().m_z, 1.0f);
    //Ankle = new dCustomBallAndSocket(Ankle_PinMatrix, LPT->GetBody(), UP_leg->GetBody());
    //Ankle->EnableCone(false);
    //Ankle->SetConeLimits(40.0f * dPi);
    //Ankle->EnableTwist(false);
    //Ankle->SetTwistLimits(-10.0f * dPi, +10.0f * dPi);
    //m_winManager->aManager->vJointList.push_back(Ankle);
    //m_winManager->aManager->vJointNameList.push_back("Ankle");
    
    // EX. DOUBLE HINGE joint. 
    dMatrix Hip_PinMatrix(dGetIdentityMatrix()); // define the pin
    Hip_PinMatrix = Hip_PinMatrix * dPitchMatrix(90.0f * dDegreeToRad); // DEFINE THE CORRECT DOF OF A DOUBLEHINGE (the first pin must be the one actuated by the muscle)
    Hip_PinMatrix.m_posit = dVector(UP_leg->GetPosition().m_x, UP_leg->GetPosition().m_y - l_Up_Leg/2, UP_leg->GetPosition().m_z, 1.0f);
    Hip = new dCustomDoubleHinge(Hip_PinMatrix, UP_leg->GetBody(), LP_leg->GetBody());
    //Hip->EnableLimits(false);
    //Hip->SetLimits(-10.0f * dPi, 10.0f * dPi);
    //Hip->EnableLimits1(false);
    //Hip->SetLimits1(-10.0f * dPi, 10.0f * dPi);
    m_winManager->aManager->vJointList.push_back(Hip);
    m_winManager->aManager->vJointNameList.push_back("Hip");

    // EX. HINGE joint. 
    dMatrix Knee_PinMatrix(dGetIdentityMatrix()); // define the pin
    Knee_PinMatrix.m_posit = dVector(LPT->GetPosition().m_x, LPT->GetPosition().m_y - l_LPT / 2 - r_bones, LPT->GetPosition().m_z, 1.0f);
    Knee = new dCustomHinge(Knee_PinMatrix, LPT->GetBody(), UP_leg->GetBody());
    Knee->EnableLimits(false);
    Knee->SetLimits(-10.0f * dPi, 10.0f * dPi);
    m_winManager->aManager->vJointList.push_back(Knee);
    m_winManager->aManager->vJointNameList.push_back("Knee");

    ////// MUSCLE DEFINITION

    //////EX: hfl muscle Creation:
    //////1. Origin body: Up_Leg, 
    //////2. Insertion body: Hip, 
    //////3. Mesh coordinates: vector 1 on body 1, vector 2 on body 2 (change the values in 'DummyGeometricProperties.xml' file)
    //////4. String containing the name of the first joint. The name MUST be UNIQUE. The first joint is the proximal (closer to the head)
    //////5. Enum containing the type of the first joint (Hinge, Ball, DoubleHinge)
    //////6. String containing the name of the second joint. The name MUST be UNIQUE. Use "None" if the muscle is monoarticular
    //////7. Enum containing the type of the second joint (Hinge, Ball, DoubleHinge, None). select None if the muscle is monoarticular

    //// ex of monoarticular muscle
    //m_hfl_R = new Muscle(m_winManager->aLineManager, m_winManager->aManager, LPT, UP_leg, dVector(0, 0, -r_bones), dVector(0, 0, -r_bones), "Hip", DoubleHinge, "None", None);
    //m_hfl_R->GenerateMesh();
    //m_winManager->aManager->vMuscleList.push_back(m_hfl_R);
    //m_hfl_R->SetThetazero((180)* dDegreeToRad); // initial joint angle
    //m_hfl_R->SetMuscleParams(2000, 12, 11, 10, 10, 0.5, 0, 180.0f * dDegreeToRad, 0, 0); // params from Geyer 
    //m_hfl_R->SetLCE(10.6f); // balanced initial condition
    //m_hfl_R->SetMaxJointAngle(min_hip_angle);

    // Ex of biarticular muscle
    m_ham_L = new Muscle(m_winManager->aLineManager, m_winManager->aManager, LPT, UP_leg, dVector(0, 0, -r_bones), dVector(0, 0, -r_bones), "Hip", DoubleHinge, "Knee", Hinge);
    m_ham_L->GenerateMesh();
    m_winManager->aManager->vMuscleList.push_back(m_ham_L);
    m_ham_L->SetThetazero((180)* dDegreeToRad); // initial joint angle
    m_ham_L->SetTheta1zero((180)* dDegreeToRad); // initial joint 1 angle
    m_ham_L->SetMuscleParams(3000, 12, 10, 31, 8, 0.7, 0, 155.0f * dDegreeToRad, 0, 180.0f * dDegreeToRad); // params from Geyer 
    m_ham_L->SetLCE(10.0f); // optimal initial condition
    m_ham_L->SetMaxJointAngle(min_hip_angle);
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

    dVector Vtemp(0.0f);
    
     //scan all  Muscle Elements
    for (auto itr = m_winManager->aManager->vMuscleList.begin();
        itr != m_winManager->aManager->vMuscleList.end(); itr++)
    {
        dVector pin; // needed to apply the torque along the joint pin
        Muscle* Mobj = (Muscle*)*itr;
        // loop on two joints
        for (auto itr1 = 0; itr1 <= 1; itr1++) 
        {
            // Read the joint name list and find the joint name of the Mobj
            std::vector<std::string> list = m_winManager->aManager->vJointNameList;
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
            //dAssert(j_index = 100); // if the muscle is not found ERROR
            // switch the type joint of the Mobj 
            switch (type) {
            case Hinge:
            {
                dCustomHinge* joint = (dCustomHinge*)m_winManager->aManager->vJointList[j_index]; // joint from jointlist
                if (itr1 == 0)
                    Mobj->SetAngle(joint->GetJointAngle()); // joint angle
                else
                    Mobj->SetAngle1(joint->GetJointAngle()); // joint angle

                pin = joint->GetPinAxis(); // joint pin
                break;
            }
            case BallAndSocket:
            {
                dCustomBallAndSocket* joint = (dCustomBallAndSocket*)m_winManager->aManager->vJointList[j_index]; // joint from jointlist
                //GetJointAngle() and GetPinAxis() methods must be added inside dCustomAndSocket.cpp. Copy the dCustomHinge.cpp respective methods and build newton again
                //Mobj->SetAngle(joint->GetJointAngle()); // joint angle
                //pin = joint->GetPinAxis(); // joint pin
                break;
            }

            case DoubleHinge:
            {
                dCustomDoubleHinge* joint = (dCustomDoubleHinge*)m_winManager->aManager->vJointList[j_index];
                if (itr1 == 0)
                    Mobj->SetAngle(joint->GetJointAngle());
                else
                    Mobj->SetAngle1(joint->GetJointAngle());

                pin = joint->GetPinAxis();
                break;
            }
            case None:
            {
                break;
            }
            }
            //// Impose sinusoidal rotation of joint
            //dCustomHinge* joint = (dCustomHinge*)m_winManager->aManager->vJointList[j_index - 1];
            //float mv = 3.14f/3.f; // displacement [rad] 60°
            //joint->EnableMotor(1, 2*3.14f*mv*cos(2*3.14f*newTime));
            //joint->SetFriction(1.0e4f);
            //Mobj->SetNeuralDelay(1.f / 2400.f); // 1.f/2400.f s
            //Mobj->SetActivation(0); // modify
            //double Ttemp = Mobj->Compute_muscle_Torque(timestep);
        }
        //Impose muscle as actuator sinusoidal activation
        Mobj->SetNeuralDelay(1.f / 2400.f); // 1.f/2400.f s
        Mobj->SetActivation(0.5 + 0.5 * sin(2 * 3.14f * newTime));
        double Ttemp = Mobj->Compute_muscle_Torque(timestep);

        // Get the Body1 connected to the muscle and apply the muscle force
        GeomNewton* gNewton = (GeomNewton*)(Mobj->body1);
        NewtonBody* NBody = gNewton->GetBody();
        pin = pin * (-Ttemp);
        NewtonBodyAddTorque(NBody, &pin.m_x);

        // Get the Body2 connected and apply the opposite muscle force
        gNewton = (GeomNewton*)(Mobj->body2);
        NBody = (NewtonBody*)gNewton->GetBody();
        pin.Scale(-1); // opposite force on second body
        NewtonBodyAddTorque(NBody, &pin.m_x);

        // stamp data
        float theta(0.0f), theta1(0.0f), lce(0.0f), Fmtu(0.0f), vel(0.0f), lmtu(0.0f);
        Mobj->GetMuscleParams(theta, theta1, lce, Fmtu, vel, lmtu);
        monFlux << newTime << "  " << theta <<  "  " << theta1 << "  " << Mobj->fSE(Mobj->GetDelta_l()) << "  " << Mobj->fCE(Mobj->GetDelta_l(), timestep) << "  " << Mobj->fPE(Mobj->GetDelta_l()) << "  " << Mobj->fDE(Mobj->GetDelta_l(), timestep) << "  " << Mobj->GetNmax() << "  " << Mobj->GetLCE() << "  " << Mobj->GetDelta_l() << "  " << Mobj->GetActivation() << "  " << vel << "  " << lmtu << std::endl;
        //cout << Mobj->GetNmax() << '\t' << theta << '\t' << lce << '\t' << vel << '\t' << T.m_x << "\n";
           
        // update for next force update
        newTime = newTime + timestep; // update time
        Mobj->SetLCE(Mobj->GetLCE() + Mobj->GetDelta_l()); // update lCE
    }	
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



