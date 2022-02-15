#include "pch.h"
#include "ControlAlgorithm.h"

// ----------------------------------------------------------------------
// CONTROL ALGORITHM CLASS DEFINITION
// ----------------------------------------------------------------------

ControlAlgorithm::ControlAlgorithm()
{
    /// Control parameter initialization WANG's values
    Gf = { {GLU, 0.5},    {HAM, 0.65},   {VAS, 1.2},   {SOL, 1.2},     {GAS, 1.1} };// list gain force feedback
    Gf_TA_SOL = { {SOL, 0.4} };// gain force feedback for TA depending ofìn SOL
    Glg = { {HFL, 0.5},    {HAM, 4.0},   {TA, 1.1} };// list gain length 1 feedback
    Glh = { {HFL, 0.65},    {HAM, 0.85},   {TA, 0.72} };// list gain length 2 feedback
    // stance preparation
    GPDk = { {HFL, 1.0},    {GLU, 1.0},   {VAS, 1.0} };// list gain  PD controller spring
    GPDd = { {HFL, 0.2},    {GLU, 0.2},   {VAS, 0.2} };// list gain PD controller damper
    GPDa = { {HFL,160 * dDegreeToRad},    {GLU, 160 * dDegreeToRad},   {VAS, 170 * dDegreeToRad} };//list gain PD controller ang in rad. CHECK
    cd = 0.5;// position/ coeff for  target angle hip SIMBICON
    cv = 0.2; // velocity coeff for  target angle hip SIMBICON
    // PD for vas e hfl
    GP1 = { {HFL, 1.15}, {VAS, 2.0} };
    GP2 = { {HFL, 0.0 * dDegreeToRad}, {VAS, 15 * dDegreeToRad} };// CHECK ANGLE
    // stance hip control
    Glead1 = { {HAM, 1.91},    {GLU, 1.91},   {HFL, 1.91} }; 
    Glead2 = { {HAM, -5 * dDegreeToRad},    {GLU, -5 * dDegreeToRad},   {HFL, -5 * dDegreeToRad} };
    Glead3 = { {HAM, 0.2},    {GLU, 0.2},   {HFL, 0.2} };

        /// Control parameter initialization
    
    //Gf = { {GLU, 0.0},    {HAM, 0.0},   {VAS, 0.0},   {SOL, 0.0},     {GAS, 0.0} };// list gain force feedback
    //Gf_TA_SOL = { {SOL, 0.0} };// gain force feedback for TA depending on SOL
    //Glg = { {HFL, 0.0},    {HAM, 0.0},   {TA, 0.0} };// list gain length 1 feedback
    //Glh = { {HFL, 0.6},    {HAM, 1.0},   {TA, 0.8} };// list gain length 2 feedback
    //// stance preparation
    //GPDk = { {HFL, 0.0},    {GLU, 0.0},   {VAS, 0.0} };// list gain  PD controller spring
    //GPDd = { {HFL, 0.0},    {GLU, 0.0},   {VAS, 0.0} };// list gain PD controller damper
    //GPDa = { {HFL, 160*dDegreeToRad},    {GLU, 160 * dDegreeToRad},   {VAS, 170 * dDegreeToRad} };//list gain PD controller ang in rad. CHECK
    //cd = 0.0;// position/ coeff for  target angle hip SIMBICON
    //cv = 0.0; // velocity coeff for  target angle hip SIMBICON
    //// P for vas e hfl
    //GP1 = { {HFL, 0.0}, {VAS, 0.0} };
    //GP2 = { {HFL, 0.0*dDegreeToRad}, {VAS, 15*dDegreeToRad} };// inclinazione tronco, 15° flessione ginocchio
    //// stance hip control initialized
    //// inclinazione tronco, negativo in avanti
    //Glead1 = { {HAM, 2.0},    {GLU, 2.0},   {HFL, 2.0} };
    //Glead2 = { {HAM, -5 * dDegreeToRad},    {GLU, -5 * dDegreeToRad},   {HFL, -5 * dDegreeToRad} };
    //Glead3 = { {HAM, 0.2},    {GLU, 0.2},   {HFL, 0.2 } };
    
    // excitation list initialization for each muscle
    list<float> l15(15, 0.0);// 5 ms delay
    list<float> l30(30, 0.0);// 10 ms delay
    list<float> l60(60, 0.0);// 20 ms delay 

    uf_l = { {"sol_R", l60 },{"sol_L" , l60},
    {"gas_R", l60 },{"gas_L" , l60},
    {"glu_R", l15 },{"glu_L" , l15},
    {"ham_R", l15 },{"ham_L" , l15},
    {"vas_R", l30 },{"vas_L" , l30} };

    uf_ta_sol_l = { {"sol_R", l60 },{"sol_L" , l60} };

    ul_l = { {"ta_R", l60 },{"ta_L" , l60},
    {"hfl_R", l15 },{"hfl_L" , l15},
    {"ham_R", l15 },{"ham_L" , l15} };

    up_l = { {"vas_R", l30 },{"vas_L" , l30},
    {"hfl_R", l15 },{"hfl_L" , l15} };

    u_sp_l = { {"vas_R", l30 },{"vas_L" , l30},
    {"glu_R", l15 },{"glu_L" , l15},
    {"hfl_R", l15 },{"hfl_L" , l15}};

    uHIP_s_l = { {"hfl_R", l15 },{"hfl_L" , l15},
    {"glu_R", l15 },{"glu_L" , l15},
    {"ham_R", l15 },{"ham_L" , l15} };

    f_ta_sol_l = { {"sol_R", l60 },{"sol_L" , l60} };

    State_velocity = { {"Sknee_r", 0},{"Sknee_l", 0},
    {"Ship_r", 0},{"Ship_l", 0},
    {"Sankle_r", 0},{"Sankle_l", 0} ,
    {"Strunk", 0}, {"Ctrunk", 0} };
    State_position = { {"Sknee_r", 0},{"Sknee_l", 0},
    {"Ship_r", 0},{"Ship_l", 0},
    {"Sankle_r", 0},{"Sankle_l", 0} ,
    {"Strunk", 0}, {"Ctrunk", 0} };

    State0_velocity = { {"Sknee_r", 0},{"Sknee_l", 0},
    {"Ship_r", 0},{"Ship_l", 0},
    {"Sankle_r", 0},{"Sankle_l", 0} ,
    {"Strunk", 0}, {"Ctrunk", 0} };
    State0_position = { {"Sknee_r", 0},{"Sknee_l", 0},
    {"Ship_r", 0},{"Ship_l", 0},
    {"Sankle_r", 0},{"Sankle_l", 0} ,
    {"Strunk", 0}, {"Ctrunk", 0} };

}

ControlAlgorithm::~ControlAlgorithm()
{
    //    if (!m_Manager->IsTerminated){
    //// SOMETHING TO DELETE?
    //    }
}
//MTU control law based on Force feedback, Length feedback and PD controller
//The output is the excitation signal for muscle m_name. 
float ControlAlgorithm::MTU_excitation(Mtuname m_name,vector<bool> gait_state,char lead, char lat, float l_til, float f_norm, dVector com_Playervel)
{
    //Get muscle control gains form model//
    float gf = 0, glg = 0, glh = 0, gPDk = 0, gPDd = 0, gPDa = 0, gLead1 = 0, gLead2 = 0, gLead3 = 0, gP1 = 0, gP2 = 0;
    gf = GetGain_Force_Feedback(m_name);
    glg = GetGain1_Length_Feedback(m_name);
    glh = GetGain2_Length_Feedback(m_name);
    gPDk = GetGain_PDk(m_name);
    gPDd = GetGain_PDd(m_name);
    gPDa = GetGain_PDa(m_name);
    gLead1 = GetGain_Lead1(m_name);
    gLead2 = GetGain_Lead2(m_name);
    gLead3 = GetGain_Lead3(m_name);
    gP1 = GetGain_P1(m_name);
    gP2 = GetGain_P2(m_name);

    float u = 0;
    float p = 0.0, p1 = 0.0, p2 =0.0, s = 0.0, q = 0.0;// initial constant excitation

    string muscle = MuscleName(m_name, lat);

    // Gait state
    bool SP = false, SI = false, STANCE = false, SWING = false, DS = false;
    STANCE = gait_state[0];SWING = gait_state[1];SP = gait_state[2]; SI = gait_state[3], DS = gait_state[4];

    // Positive force feedback and length feedback///
    float uf=0,ul=0;

    // Muscle-driven P control  for VAS (stance) e HFL (swing)/// 
    float up = 0;
    if (m_name == VAS) {
        if (lat == 'R') {
            if (State_velocity.find("Sknee_r")->second < 0)// compute excitation just if the knee just if ankle velocity is negative
                if (State_position.find("Sknee_r")->second - gP2 < 0)//negative excitation just if knee angle is below a threshold
                    up = gP1 * (State_position.find("Sknee_r")->second - gP2);
        }
        else {// the same for left knee
            if (State_velocity.find("Sknee_l")->second < 0)
                if (State_position.find("Sknee_l")->second - gP2 < 0)
                    up = gP1 * (State_position.find("Sknee_l")->second - gP2);
        }
    }
    else if (m_name == HFL)
        up = gP1 * (abs(State_position.find("Strunk")->second) - gP2);//Trunk orientation control
    
    // control during stance preparation VAS HFL GLU
    float u_sp = 0;
    float angle;
    if (lat == 'R') {
        if (m_name == VAS) {
            angle = State0_position.find("Sknee_r")->second - State_position.find("Sknee_r")->second;
            u_sp = gPDk * (angle - gPDa) + gPDd * State_velocity.find("Sknee_r")->second;
        }
        else if (m_name == HFL || m_name == GLU) {
            angle = State0_position.find("Ship_r")->second + State_position.find("Ship_r")->second;
            u_sp = gPDk * (angle - (gPDa + cd * d + cv * (-com_Playervel.m_z))) + gPDd * State_velocity.find("Ship_r")->second;
        }
    }
    else
    {
        if (m_name == VAS) {
            angle = State0_position.find("Sknee_l")->second - State_position.find("Sknee_l")->second;
            u_sp = gPDk * (angle - gPDa) + gPDd * State_velocity.find("Sknee_l")->second;
        }
        else if (m_name == HFL || m_name == GLU) {
            angle = State0_position.find("Ship_l")->second + State_position.find("Ship_l")->second;
            u_sp = gPDk * (angle - (gPDa + cd * d + cv * (-com_Playervel.m_z))) + gPDd * State_velocity.find("Ship_l")->second;
        }
    }
    // HIP control stance, HFL GLU HAM
    float u_hip = 0;
    if (m_name == HFL || m_name == GLU || m_name == HAM)// for trunk orientation
        u_hip = gLead1 * (State_position.find("Strunk")->second - gLead2) + gLead3 * (State_velocity.find("Strunk")->second);
    // 
    // Update of excitation queue for each muscle according to delay
    if (m_name == SOL) {
        uf = gf * f_norm;
        UpdateQueue(muscle, uf_l, uf);
        float uf_ta_sol = Gf_TA_SOL[SOL] * f_norm;
        UpdateQueue(muscle, uf_ta_sol_l, uf_ta_sol);
    }
    else if (m_name == TA) {
        ul = glg * (l_til - glh);
        UpdateQueue(muscle, ul_l, ul);
    }
    else if (m_name == VAS) {
        uf = gf * f_norm;
        UpdateQueue(muscle, uf_l, uf);
        UpdateQueue(muscle, up_l, up);
        UpdateQueue(muscle, u_sp_l, u_sp);
    }
    else if (m_name == GAS){
        uf = gf * f_norm;
        UpdateQueue(muscle, uf_l, uf);
    }
    else if (m_name == HFL) {
        ul = glg * (l_til - glh);
        UpdateQueue(muscle, ul_l, ul);
        UpdateQueue(muscle, u_sp_l, u_sp);
        UpdateQueue(muscle, up_l, up);
        if (!DS || (DS && lat == lead))// if double stance, onlt leading leg is responsible of trunk orientation
            UpdateQueue(muscle, uHIP_s_l, u_hip);
        else
            UpdateQueue(muscle, uHIP_s_l, 0);
    }
    else if (m_name == HAM) {
        uf = gf * f_norm;
        UpdateQueue(muscle, uf_l, uf);
        ul = glg * (l_til - glh);
        UpdateQueue(muscle, ul_l, ul);
        if (!DS || (DS && lat == lead))// if double stance, onlt leading leg is responsible of trunk orientation
            UpdateQueue(muscle, uHIP_s_l, u_hip);
        else
            UpdateQueue(muscle, uHIP_s_l, 0);
    }
    else if (m_name == GLU) {
        uf = gf * f_norm;
        UpdateQueue(muscle, uf_l, uf);
        UpdateQueue(muscle, u_sp_l, u_sp);
        if (!DS || (DS && lat == lead))// if double stance, onlt leading leg is responsible of trunk orientation
            UpdateQueue(muscle, uHIP_s_l, u_hip);
        else
            UpdateQueue(muscle, uHIP_s_l, 0);
    }

    if (STANCE) {
        switch (m_name) {// p values form wang supplemental material section 5
        case SOL:
            u = p + uf_l[muscle].front();break;
        case TA: {
            string name = "sol_";
            name += lat;
            u = p + ul_l[muscle].front() - uf_ta_sol_l[name].front();break;
        }
        case GAS:
            u = p + uf_l[muscle].front();break;
        case VAS: {
            u = uf_l[muscle].front() + up_l[muscle].front();break;}
        case HAM: {
            if (uHIP_s_l[muscle].front() > 0)// torque on trunk rotates the body forward
                u = p1 + abs(uHIP_s_l[muscle].front());
            else
                u = p1;
            break;}
        case GLU:{
            if (uHIP_s_l[muscle].front() > 0)// torque on trunk rotates the body forward
                u = p1 + abs(uHIP_s_l[muscle].front());
            else
                u = p1;
            break;}
        case HFL: {
            if (uHIP_s_l[muscle].front() < 0)// torque on trunk rotates the body backward
                u = p1 + abs(uHIP_s_l[muscle].front());
            else
                u = p1;
            break;}
        case RF:// RF
            u = p;break;
        }
    }
    if (SI) { // || DS for walking? // s values from wang supplemental material section 5
        switch (m_name) {
        case VAS:
            u = u-1;break;
        case RF:
            u = u+0.01;break;
        case GLU:
            u = u-0.25;break;
        case HFL:
            u = u+0.25;break;
        }// other muscles same as stance
    }
    if (SWING) {
        switch (m_name) {
        case TA:
            u = q + ul_l[muscle].front();break;
        case HAM:
            u = q + uf_l[muscle].front();break;
        case GLU:
            u = q + uf_l[muscle].front();break;
        case HFL: {
            string name = "ham_";
            name += lat;
            u = q + ul_l[muscle].front() - ul_l[name].front() + up_l[muscle].front();break;}// !!!!the minus ul leads to negative excittaion!!!!
        case VAS:
            u = q;break;
        case RF:
            u = q;break;
        case GAS:
            u = q;break;
        case SOL:
            u = q;break;
        default:// RF, SOL, GAS, VAS
            u = q;break;
        }
    }
    if (SP) {
        //Muscle-driven PD control in Stance Preparation//
        if (m_name == HFL) {
            if (u_sp_l[muscle].front() > 0) // torque on thigh lifts the knee up
                u = q + abs(u_sp_l[muscle].front());
            else
                u = q;
        }
        else if (m_name == VAS || m_name == GLU) { 
            if (u_sp_l[muscle].front() < 0)// GLU: torque on thigh pushes down the knee, VAS: extension on knee
                u = q + abs(u_sp_l[muscle].front());
            else
                u = q;
        }
    }
    if (u < p)// security check: no negative values
        u = p;
    return u;
}

vector<bool> ControlAlgorithm::Stance_Swing_Detection(dVector ankle, dVector com, float leg, float contact)
{

    bool SP = false, SI = false, STANCE = false, SWING = false;
    float dsp(0.15), dsi(0.4);// wang supplemental material section 5
    float d = sqrt(pow(com.m_z - ankle.m_z,2)+pow(com.m_x - ankle.m_x,2)) / leg;// horizontal distance between body com and ankle 
    if (contact >= 1)// check value
    {
        STANCE = true;
        if (d > dsi && ankle.m_z > com.m_z)
            SI = true;
    }
    else
    {
        SWING = true;
        if (d > dsp && ankle.m_z < com.m_z)
            SP = true;
    }
    vector<bool> foot_state{ STANCE, SWING, SP, SI };
    return foot_state;
}

void ControlAlgorithm::ControlSetHorizontalDistance(float d1)
{
     d = d1;
}

float ControlAlgorithm::GetGain_Force_Feedback(Mtuname NAME) {
    float g = 0;
    if (NAME == SOL || NAME == GAS || NAME == VAS || NAME == HAM || NAME == GLU)
        g = Gf.find(NAME)->second;
    return g;
}

float ControlAlgorithm::GetGain1_Length_Feedback(Mtuname NAME)
{
    float g = 0;
    if (NAME == TA || NAME == HFL || NAME == HAM)
        g = Glg.find(NAME)->second;
    return g;
}

float ControlAlgorithm::GetGain2_Length_Feedback(Mtuname NAME)
{
    float g = 0;
    if (NAME == TA || NAME == HFL || NAME == HAM)
        g = Glh.find(NAME)->second;
    return g;
}

float ControlAlgorithm::GetGain_PDk(Mtuname NAME)
{
    float g = 0;
    if (NAME == VAS || NAME == GLU || NAME == HFL)
        g = GPDk.find(NAME)->second;
    return g;
}

float ControlAlgorithm::GetGain_PDd(Mtuname NAME)
{
    float g = 0;
    if (NAME == VAS || NAME == GLU || NAME == HFL)
        g = GPDd.find(NAME)->second;
    return g;
}

float ControlAlgorithm::GetGain_PDa(Mtuname NAME)
{
    float g = 0;
    if (NAME == VAS || NAME == GLU || NAME == HFL)
        g = GPDa.find(NAME)->second;
    return g;
}

float ControlAlgorithm::GetGain_Lead1(Mtuname NAME)
{
    float g = 0;
    if (NAME == HAM || NAME == GLU || NAME == HFL)
        g = Glead1.find(NAME)->second;
    return g;
}

float ControlAlgorithm::GetGain_Lead2(Mtuname NAME)
{
    float g = 0;
    if (NAME == HAM || NAME == GLU || NAME == HFL)
        g = Glead2.find(NAME)->second;
    return g;
}

float ControlAlgorithm::GetGain_Lead3(Mtuname NAME)
{
    float g = 0;
    if (NAME == HAM || NAME == GLU || NAME == HFL)
        g = Glead3.find(NAME)->second;
    return g;
}

float ControlAlgorithm::GetGain_P1(Mtuname NAME)
{
    float g = 0;
    if (NAME == VAS || NAME == HFL)
        g = GP1.find(NAME)->second;
    return g;
}
float ControlAlgorithm::GetGain_P2(Mtuname NAME)
{
    float g = 0;
    if (NAME == VAS || NAME == HFL)
        g = GP2.find(NAME)->second;
    return g;
}
vector<float> ControlAlgorithm::GetGain_HFLswing()
{
    float g1 = GP1.find(HFL)->second;
    float g2 = GP2.find(HFL)->second;
    return { g1, g2 };
}

void ControlAlgorithm::SetState(float position, float velocity, string dof)
{
    State_velocity.find(dof)->second = velocity;
    State_position.find(dof)->second = position;
}

void ControlAlgorithm::SetState0(float position, float velocity, string dof)
{
    State0_velocity.find(dof)->second = velocity;
    State0_position.find(dof)->second = position;
}

vector<float> ControlAlgorithm::GetState(string dof)
{
    vector<float> state{ State_position.find(dof)->second, State_velocity.find(dof)->second };
    return state;
}

vector<float> ControlAlgorithm::GetState0(string dof)
{
    vector<float> state{ State0_position.find(dof)->second, State0_velocity.find(dof)->second };
    return state;
}
// remove excitation of previous iteration and add the new one to excitation list. This list is needed to apply neural delay.
void ControlAlgorithm::UpdateQueue(string muscle, map<string, list<float>>& queue, float value)
{
   queue[muscle].pop_front();// remove old data
   queue[muscle].push_back(value);// add new data at the end of list
}

string ControlAlgorithm::MuscleName(Mtuname m_name, char lat)
{
    string muscle;
    if (m_name == SOL) {
        muscle = "sol_";muscle += lat;
    }
    else if (m_name == TA) {
        muscle = "ta_";muscle += lat;
    }
    else if (m_name == VAS) {
        muscle = "vas_";muscle += lat;
    }
    else if (m_name == GAS) {
        muscle = "gas_";muscle += lat;
    }
    else if (m_name == HFL) {
        muscle = "hfl_";muscle += lat;
    }
    else if (m_name == HAM) {
        muscle = "ham_";muscle += lat;
    }
    else if (m_name == GLU) {
        muscle = "glu_";muscle += lat;
    }
    else if (m_name == RF) {
        muscle = "rf_";muscle += lat;
    }
    return muscle;
}
