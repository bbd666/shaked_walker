#include "pch.h"
#include "ControlAlgorithm.h"

// ----------------------------------------------------------------------
// CONTROL ALGORITHM CLASS DEFINITION
// ----------------------------------------------------------------------

ControlAlgorithm::ControlAlgorithm()
{
    /// Control parameter initialization
    Gf = { {GLU, 0},    {HAM, 0},   {VAS, 0},   {SOL, 0},     {GAS, 0} };// list gain force feedback
    Glg = { {HFL, 0},    {HAM, 0},   {TA, 0} };// list gain length 1 feedback
    Glh = { {HFL, 0},    {HAM, 0},   {TA, 0} };// list gain length 2 feedback
    GPDk = { {HFL, 0},    {GLU, 0},   {VAS, 0} };// list gain PD controller spring
    GPDd = { {HFL, 0},    {GLU, 0},   {VAS, 0} };// list gain PD controller damper
    GPDa = { {HFL, 0},    {GLU, 0},   {VAS, 0} };//list gain PD controller ang
    GP1 = { {HFL, 0}, {VAS, 0} };
    GP2 = { {HFL, 0}, {VAS, 0} };
    Glead1 = { {HAM, 0},    {GLU, 0},   {HFL, 0} };
    Glead2 = { {HAM, 0},    {GLU, 0},   {HFL, 0} };
    Glead3 = { {HAM, 0},    {GLU, 0},   {HFL, 0} };

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
    vector<dFloat>  Tstate, vector<dFloat>  Pstate, char lead, char lat, float l_til, float f_norm, float time)
{

    // get sol and ham excitation for TA (stance) and HFL (swing) excitation. SOL and HAM must be solved before TA and HFL
    float other_ext = 0;
    if (m_name == TA || m_name == HFL)
        other_ext = GetOtherMuscleExcitation(m_name, lat);

    //Get muscle control gains form model//
    float gf = 0, glg = 0, glh = 0, gPDk = 0, gPDd = 0, gPDa = 0, gLead1 = 0, gLead2 = 0, gLead3 = 0, gP1 = 0, gP2 = 0;
    gf      = GetGain_Force_Feedback(m_name);
    glg     = GetGain1_Length_Feedback(m_name);
    glh     = GetGain2_Length_Feedback(m_name);
    gPDk    = GetGain_PDk(m_name);
    gPDd    = GetGain_PDd(m_name);
    gPDa    = GetGain_PDa(m_name);
    gLead1  = GetGain_Lead1(m_name);
    gLead2  = GetGain_Lead2(m_name);
    gLead3  = GetGain_Lead3(m_name);
    gP1     = GetGain_P1(m_name);
    gP2     = GetGain_P2(m_name);

    float u = 0;
    float p = 0.05, s = 0.05, q = 0.05;// initial constant excitation

    // Gait state
    bool SP = false, SI = false, STANCE = false, SWING = false, DS = false;
    STANCE = gait_state[0];SWING = gait_state[1];SP = gait_state[2]; SI = gait_state[3], DS = gait_state[4];

    // neural trasmission delay for each MTU
    float delay = 0;// neural signal propagation according to geyer 2010 in [s]
    if (m_name == HFL || m_name == GLU || m_name == HAM || m_name == RF)
        delay = 5 / 1000;
    else if (m_name == SOL || m_name == TA || m_name == GAS)
        delay = 20 / 1000;
    else
        delay = 10 / 1000;

    // Positive force feedback///
    float uf = gf * f_norm * (time + delay);

    // Positive length feedback///
    float ul = glg * (l_til * (time + delay) - glh);
    if (ul <= 0)
        ul = 0;

    // Muscle-driven P control  for VAS (stance) e HFL (swing)/// 
    float up = 0;
    if (m_name == VAS)
        if (Pstate[1] < 0)// Check
            up = gP1 * (Pstate[0] * (time - delay) - gP2);
        else if (m_name == HFL)
            up = gP1 * (Tstate[0] * (time - delay) - gP2);//Trunk orientation control

        // HIP control stance, during Double stance only for lead leg //
    float u_hip = 0;
    if (m_name == HFL || m_name == GLU || m_name == HAM)// for trunk orientation
        u_hip = abs(gLead1 * (Tstate[0] * (time - delay) - gLead2 + gLead3 * Tstate[1] * (time - delay)));
    if (DS && (lat != lead))
        u_hip = 0;

    if (STANCE) {
        switch (m_name) {
        case SOL:
            u = p + uf;break;
        case TA:
            u = p + ul - other_ext;break;
        case GAS:
            u = p + uf;break;
        case VAS:
            u = p + uf + up;break;
        case HAM:
            u = p + u_hip;break;
        case GLU:
            u = p + u_hip;break;
        case HFL:
            u = p + u_hip;break;
        case RF:// RF
            u = p;break;
        }
    }
    if (SI) {
        switch (m_name) {
        case VAS:
            u = u - s;break;
        case RF:
            u = u + s;break;
        case GLU:
            u = u - s;break;
        case HFL:
            u = u + s;break;
        }// other muscles same as stance
    }
    if (SWING) {
        switch (m_name) {
        case TA:
            u = q + ul;break;
        case HAM:
            u = q + uf;break;
        case GLU:
            u = q + uf;break;
        case HFL:
            u = q + ul - other_ext + up;break;
        default:// RF, SOL, GAS, VAS
            u = q;break;
        }
    }
    if (SP) {
        //Muscle-driven PD control in Stance Preparation//
        if (m_name == VAS || m_name == GLU || m_name == HAM)// for trunk orientation
            u = q + abs(gPDk * (Pstate[0] * (time - delay) - gPDa + gPDd * Pstate[1] * (time - delay)));
        // other muscles same as swing
    }

    // set SOL and HAM excitation 
    if (m_name == SOL || m_name == HAM)
        SaveOtherMuscleExcitation(u, lat, m_name);

    return u;
}

vector<bool> ControlAlgorithm::Stance_Swing_Detection(dVector foot, dVector com, float leg, float clearance)
{

    bool SP = false, SI = false, STANCE = false, SWING = false;
    float stride = 1.8 * 0.415;///[m] better to compute as distance/number of steps
    float dsp(stride*0.9), dsi(stride*0.9);// define values
    float d = (com.m_z - foot.m_z) / leg;
    if (clearance < 0.005)// check value
    {
        STANCE = true;
        if (d > dsi)// Z+ direction opposite to walk direction
            SI = true;
    }
    else
    {
        SWING = true;
        if (d <-dsp)// Z+ direction opposite to walk direction
            SP = true;
    }
    vector<bool> foot_state{ STANCE, SWING, SP, SI };
    return foot_state;
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

void ControlAlgorithm::SaveOtherMuscleExcitation(float u, char lat, Mtuname mname)
{
    if (mname == SOL) {
        if (lat == 'R')
            uf_sol_r = u;
        else
            uf_sol_l = u;
    }
    else if (mname == HAM) {
        if (lat == 'R')
            ul_ham_r = u;
        else
            ul_ham_l = u;
    }
}

float ControlAlgorithm::GetOtherMuscleExcitation(Mtuname mname, char lat)
{
    float u=0.0;
    if (mname == TA) {
        if (lat == 'R')
            u = uf_sol_r;
        else
            u = uf_sol_l;
    }
    else if (mname == HFL) {
        if (lat == 'R')
            u = ul_ham_r;
        else
            u = ul_ham_l;
    }
    return u;
}
