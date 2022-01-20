#include "pch.h"
#include "ControlAlgorithm.h"

// ----------------------------------------------------------------------
// CONTROL ALGORITHM CLASS DEFINITION
// ----------------------------------------------------------------------

ControlAlgorithm::ControlAlgorithm()
{
    /// Control parameter initialization WANG's values
    Gf = { {GLU, 0.5},    {HAM, 0.65},   {VAS, 1.2},   {SOL, 1.2},     {GAS, 1.1} };// list gain force feedback
    Gf_TA_SOL = { {SOL, 0.4} };// gain force feedback for TA depending of�n SOL
    Glg = { {HFL, 0.5},    {HAM, 4.0},   {TA, 1.1} };// list gain length 1 feedback
    Glh = { {HFL, 0.65},    {HAM, 0.85},   {TA, 0.72} };// list gain length 2 feedback
    // stance preparation
    GPDk = { {HFL, 1.0},    {GLU, 1.0},   {VAS, 1.0} };// list gain  PD controller spring
    GPDd = { {HFL, 0.2},    {GLU, 0.2},   {VAS, 0.2} };// list gain PD controller damper
    GPDa = { {HFL, M_PI - 2.8},    {GLU, M_PI- 2.8},   {VAS, M_PI - 3.0} };//list gain PD controller ang in rad. CHECK
    cd = 0.5;// position/ coeff for  target angle hip SIMBICON
    cv = 0.2; // velocity coeff for  target angle hip SIMBICON
    // PD for vas e hfl
    GP1 = { {HFL, 1.15}, {VAS, 2.0} };
    GP2 = { {HFL, M_PI - 2.8}, {VAS, M_PI - 2.97} };// CHECK ANGLE
    // stance hip control
    Glead1 = { {HAM, 1.91},    {GLU, 1.91},   {HFL, 1.91} }; 
    Glead2 = { {HAM, 0.105},    {GLU, 0.105},   {HFL, 0.105} };
    Glead3 = { {HAM, 0.2},    {GLU, 0.2},   {HFL, 0.2} };

    //    /// Control parameter initialization
    //Gf = { {GLU, 0.0},    {HAM, 0.0},   {VAS, 0.0},   {SOL, 0.0},     {GAS, 0.0} };// list gain force feedback
    //Gf_TA_SOL = { {SOL, 0.0} };// gain force feedback for TA depending of�n SOL
    //Glg = { {HFL, 0.0},    {HAM, 0.0},   {TA, 0.0} };// list gain length 1 feedback
    //Glh = { {HFL, 0.0},    {HAM, 0.0},   {TA, 0.0} };// list gain length 2 feedback
    //// stance preparation
    //GPDk = { {HFL, 0.0},    {GLU, 0.0},   {VAS, 0.0} };// list gain  PD controller spring
    //GPDd = { {HFL, 0.0},    {GLU, 0.0},   {VAS, 0.0} };// list gain PD controller damper
    //GPDa = { {HFL, M_PI - 2.8},    {GLU, M_PI - 2.8},   {VAS, M_PI - 3.0} };//list gain PD controller ang in rad. CHECK
    //cd = 0.5;// position/ coeff for  target angle hip SIMBICON
    //cv = 0.2; // velocity coeff for  target angle hip SIMBICON
    //// PD for vas e hfl
    //GP1 = { {HFL, 0.0}, {VAS, 0.0} };
    //GP2 = { {HFL, M_PI - 2.8}, {VAS, M_PI - 2.97} };// CHECK ANGLE
    //// stance hip control
    //Glead1 = { {HAM, 0.0},    {GLU, 0.0},   {HFL, 0.0} };
    //Glead2 = { {HAM, 0.0},    {GLU, 0.0},   {HFL, 0.0} };
    //Glead3 = { {HAM, 0.0},    {GLU, 0.0},   {HFL, 0.0} };

    // excitation list initialization for each muscle
    list<float> l15(15, 0);// 5 ms delay
    list<float> l30(30, 0);// 10 ms delay
    list<float> l60(60, 0);// 20 ms delay 

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

    uHIP_sp_l = { {"vas_R", l30 },{"vas_L" , l30},
    {"glu_R", l15 },{"glu_L" , l15},
    {"hfl_R", l15 },{"hfl_L" , l15}};

    uHIP_s_l = { {"hfl_R", l15 },{"hfl_L" , l15},
    {"glu_R", l15 },{"glu_L" , l15},
    {"ham_R", l15 },{"ham_L" , l15} };
}

ControlAlgorithm::~ControlAlgorithm()
{
    //    if (!m_Manager->IsTerminated){
    //// SOMETHING TO DELETE?
    //    }
}
//MTU control law based on Force feedback, Length feedback and PD controller
//The output is the excitation signal for muscle m_name. 
float ControlAlgorithm::MTU_excitation(Mtuname m_name, vector<bool> gait_state,
    vector<dFloat>  Tstate, vector<dFloat>  Pstate, char lead, char lat, float l_til, float f_norm, float timestep, dVector com_Playervel)
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
    float p = 0.1, s = 0.1, q = 0.1;// initial constant excitation

    // Gait state
    bool SP = false, SI = false, STANCE = false, SWING = false, DS = false;
    STANCE = gait_state[0];SWING = gait_state[1];SP = gait_state[2]; SI = gait_state[3], DS = gait_state[4];

    // neural trasmission delay for each MTU
    int u_ind = 0;// neural signal propagation according to geyer 2010 
    if (m_name == HFL || m_name == GLU || m_name == HAM || m_name == RF)
        u_ind = 5 / 1000 / timestep * timestep;
    else if (m_name == SOL || m_name == TA || m_name == GAS)
        u_ind = 20 / 1000 / timestep * timestep;
    else
        u_ind = 10 / 1000 / timestep * timestep;

    // Positive force feedback///
    float uf = gf * f_norm;
    float uf_ta_sol;
    if (m_name == SOL)
        uf_ta_sol = Gf_TA_SOL[SOL] * f_norm;

    // Positive length feedback///
    float ul = glg * (l_til - glh);
    if (ul <= 0)
        ul = 0;

    // Muscle-driven P control  for VAS (stance) e HFL (swing)/// 
    float up = 0;
    if (m_name == VAS) {
        if (Pstate[1] < 0) {// Check
            up = -gP1 * (Pstate[0] - gP2);// Check sign (should be - or 0)
        }
    }
    else if (m_name == HFL)
        up = gP1 * (Tstate[0] - gP2);//Trunk orientation control
    

    // trunk control during stance preparation
    float uHIP_sp = abs(gPDk * (Pstate[0] - (gPDa + cd * d + cv * (-com_Playervel.m_z)) + gPDd * Pstate[1]));

    // HIP control stance, during Double stance only for lead leg //
    float u_hip = 0;
    if (m_name == HFL || m_name == GLU || m_name == HAM)// for trunk orientation
        u_hip = abs(gLead1 * (Tstate[0] - gLead2 + gLead3 * Tstate[1]));
    if (DS && (lat != lead))
        u_hip = 0;

    // remove excitation of previous iteration and add the new one to excitation list. This list is needed to apply neural delay.
    string muscle;
    if (m_name == SOL) {
        muscle = "sol_";muscle += lat;
        uf_l[muscle].pop_front();// remove old data
        uf_ta_sol_l[muscle].pop_front();// remove old data
        uf_l[muscle].push_back(uf);//populate vectors of excitation
        uf_ta_sol_l[muscle].push_back(uf_ta_sol);//populate vectors of excitation
    }
    else if (m_name == TA) {
        muscle = "ta_";muscle += lat;
        ul_l[muscle].pop_front();
        ul_l[muscle].push_back(ul);
    }
    else if (m_name == VAS) {
        muscle = "vas_";muscle += lat;
        uf_l[muscle].pop_front();
        up_l[muscle].pop_front();
        uHIP_sp_l[muscle].pop_front();
        uf_l[muscle].push_back(uf);
        up_l[muscle].push_back(up);
        uHIP_sp_l[muscle].push_back(uHIP_sp);
    }
    else if (m_name == GAS){
        muscle = "gas_";muscle += lat;
        uf_l[muscle].pop_front();
        uf_l[muscle].push_back(uf);
    }
    else if (m_name == HFL) {
        muscle = "hfl_";muscle += lat;
        ul_l[muscle].pop_front();
        uHIP_sp_l[muscle].pop_front();
        up_l[muscle].pop_front();
        uHIP_s_l[muscle].pop_front();
        ul_l[muscle].push_back(ul);
        up_l[muscle].push_back(up);
        uHIP_sp_l[muscle].push_back(uHIP_sp);
        uHIP_s_l[muscle].push_back(u_hip);
    }
    else if (m_name == HAM) {
        muscle = "ham_";muscle += lat;
        uf_l[muscle].pop_front();
        ul_l[muscle].pop_front();
        uHIP_s_l[muscle].pop_front();
        uf_l[muscle].push_back(uf);
        ul_l[muscle].push_back(ul);
        uHIP_s_l[muscle].push_back(u_hip);
    }
    else if (m_name == GLU) {
        muscle = "glu_";muscle += lat;
        uf_l[muscle].pop_front();
        uHIP_sp_l[muscle].pop_front();
        uHIP_s_l[muscle].pop_front();
        uf_l[muscle].push_back(uf);
        uHIP_sp_l[muscle].push_back(uHIP_sp);
        uHIP_s_l[muscle].push_back(u_hip);
    }
    else if (m_name == RF){
        muscle = "rf_";muscle += lat;
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
        case VAS:
            u = p*8 + uf_l[muscle].front() + up_l[muscle].front();break;
        case HAM:
            u = p*5 + uHIP_s_l[muscle].front();break;
        case GLU:
            u = p*5 + uHIP_s_l[muscle].front();break;
        case HFL:
            u = p*5 + uHIP_s_l[muscle].front();break;
        case RF:// RF
            u = p;break;
        }
    }
    if (SI || DS) {// s values form wang supplemental material section 5
        switch (m_name) {
        case VAS:
            u = u - 1;break;
        case RF:
            u = u + 0.01;break;
        case GLU:
            u = u - 0.25;break;
        case HFL:
            u = u + 0.25;break;
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
        default:// RF, SOL, GAS, VAS
            u = q;break;
        }
    }
    if (SP) {
        //Muscle-driven PD control in Stance Preparation//
        if (m_name == VAS || m_name == GLU || m_name == HAM)// for trunk orientation
            u = q + uHIP_sp_l[muscle].front();
            //other muscles same as swing
    }

    return u;
}

vector<bool> ControlAlgorithm::Stance_Swing_Detection(dVector ankle, dVector com, float leg, float contact)
{

    bool SP = false, SI = false, STANCE = false, SWING = false;
    float dsp(-0.15), dsi(0.4);// wang supplemental material section 5
    float d = sqrt(pow(com.m_z - ankle.m_z,2)+pow(com.m_x - ankle.m_x,2)) / leg;// horizontal distance between foot and ankle 
    if (contact >= 1)// check value
    {
        STANCE = true;
        if (d > dsi)// Z+ direction opposite to walk direction
            SI = true;
    }
    else
    {
        SWING = true;
        if (d < dsp)// Z+ direction opposite to walk direction
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