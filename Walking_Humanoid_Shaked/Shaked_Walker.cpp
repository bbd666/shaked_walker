/*
* Copyright(c) < 1999 - 2019 > <Dave Gravel, Free Tutorial - Newton Dynamics and more.>
*
* This software is provided 'as-is', without any express or implied
* warranty.In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
*including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions :
*
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software.If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*/
/*
  Tutorial for show how to use newton dynamics api with pure gl write by Dave Gravel - 2019.
  This tutorial if the first version, Some parts is quickly coded and need a better implementation.
  I have try to add the most options possible without have very complex code.
  I can surely update this tutorial later with better features and implementation.
  I think for the first version it already touch pretty much all subject about newton and GL.
  Have Fun...
  Dave Gravel.
*/

/**
 * @file Shaked_Walker.cpp
 * @author Stefano Marelli
 *
 * \mainpage
 * This application simulates a walking humanoid in a 3D real-physics environment. The floor can oscillate in roll or pitch direction with a sinusoidal wave.
 * The software includes:
	the Physics Engine (Newton Dynamics) provided by Dave Gravel;
	the humanoid model composed of: rigid bodies, musculo-tendon units, PD regulators, and gait control logic;
	optimization routine based on Covariance Matrix Adaptation Evolutionary Strategy (CMAES) by Nikolaus Hansen, ported to C++ by Alexander Fabisch.
 *
 * Copyright of the Walking Humanoid implementation by Stefano Marelli (e-mail:
 * <a href="mailto:ste.marelli@alice.it"> </a>.
 *
 * \section lgpl License
 * Copyright 2019-2023 Marelli Stefano
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty.In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 *including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions :
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software.If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 * 
 *
 * \section purpose General Purpose
 * The software should be used to simulate working environments where whole-body vibration is a risk factor for human health.
 * Thhanks to the siumulation, it is possible to measure gait parameters change due to WBV.   
 *
 * \section application Application Remark
 * In this version, WBV can be selected as sinusoidal waveform along roll or pitch direction. Further release should be include WVB along all the 6 axes.
 *
 * The window can crash on exit. In this case it is necessary to force stop the app.
 * \section links Links
 *
 * \section tut Tutorial
 * Run "Walking_Humanoid_Shaked.exe" the to see the simulation. The .exe file returns the "cost" of the simulation, which is a numeric value that quantifies the quality of the simulation (low value -> the model walks, high value-> the model falls). 
	Details on how the "cost" is computed in Info section.

	The .exe takes data from three files:

	1. Configuration.xml
		the settings are:
		. "SimulationTime" is the simulation length in seconds,
 		. "Optimization" activates the optimization routine based on CMAES genetic algorithm. This setting can be "no" or "yes",
 		. "IterationsNumber" is the number of iterations of the CMAES algorithm,
 		. "Output" writes simulation data to a .txt file (Gait_timeseries.txt). This setting can be "no" or "yes",
		. "Active" is related to Whole-Body Vibration: if "yes" the floor oscillates, if "no" the floor does not move,
		. "Direction" is the direction of sinusoidal WBV (roll or pitch),
		. "Amplitude" is the amplitude in degrees of the WBV wave,
		. "Frequency" is the frequency of the WBV wave in Hz.

	2. OptimizationParameters.xml

		This file contains the parameter of the model that could change in presence of WBV. At the end of an optimization routine, the file will contain two sets of parameters, the initial and optimized.

	3. DummyGeometricProperties.xml
	
		This file contains the humanoid parameters that are not optimized.
 *
 * \section references References
 * Marelli Stefano (2023): "Modelling and experiments on the effects of whole-body vibration while walking", PhD thesis in Mechanical Engineering, Politecnico di Milano, Italy.
 */

#include "pch.h"
#include "WindowGL.h"
#include "GeomGL.h"
#include "Muscle.h"
#include "Humanoid.h"
#include "Params.h"
#include "cmaes.h"
#include "tinyxml.h"

float CallNewton(double* params, int operation, int write_txt, WBV wbv);

float CallNewton(double* params, int operation, int iteration_max, int current_iteration, int popsize, WBV wbv);// overload for optimization

int call_cmaes(OptParams params,int simulationtime, int iteration_max, int operation, WBV wbv);

// boundary condition for parameters. Parameters must be positive and less than 1
bool is_feasible(double* p) {
	bool flag = true;
	for (int i = 0; i < 62; ++i) {
		double x = p[i];
		if (x >= 1.3)
			flag = false;
	}
	return flag;
}

#if defined(_DEBUG)
int main(int argc, char *argv[])
// if you like to use windows mode. 
// Comment the main section here, And uncomment the winmain section.
// Remember you to replace the console mode to windows mode in the preprossesor menu and in the system menu options.
#else
int WINAPI wWinMain(
	HINSTANCE   hInstance,
	HINSTANCE   hPrevInstance,
	PWSTR       lpCmdLine,
	int         nCmdShow)
#endif
{
	//int operation = 1; //"To run simulation enter 1, else nothing.
	//// take inputs
	//#if defined(NDEBUG)
	//	// Convert the wide character wchar_t string to a string
	//	wchar_t** origin = __wargv;
	//	vector<string> argv(__argc);
	//	for (int i = 0; i < __argc; ++i) {
	//		wstring ws(origin[i]);
	//		string str(ws.begin(), ws.end());
	//		argv[i] = str;
	//	}

	//	if (__argc > 1) {
	//		operation = stof(argv[1]);
	//	}
	//#else
	//	if (argc > 1) {
	//		operation = stof(argv[1]);
	//	}
	//#endif

	// load configuration options from xml
	AppSettings settings;
	bool flag_opts = settings.load("Configuration.xml");
	// load optimization parameters from xml
	OptParams params;
	bool flag_xml = params.load("OptimizationParameters.xml");

	if (flag_xml == 1){
		cout << "Error in loading model parameters from ModelParameters.xml file" << endl;
		return 0;
	}
	else if (flag_opts) {
		cout << "Error in loading configuration options from Configuration.xml file" << endl;
		return 0;
	}

	Options opts = settings.m_options.front();
	WBV wbv = settings.m_wbv.front();

	int operation = 0;//default no optimization
	if (opts.optimization.compare("yes") == 0)
		operation = 2;
	else if (opts.optimization.compare("no") == 0)
		operation = 1;

	int write_out = 0;// default no output
	if (opts.output.compare("yes") == 0)
		write_out = 1;

	// run simulation
	int cost = 1;
	if (operation == 1)// run simulation
	{
		double* parameters = params.RemoveScaling(params.params_pointer());//not scaled params

		cost = CallNewton(parameters, opts.simulationtime, write_out, wbv);
	}
	else if (operation == 2) // run optimization 
	{
		params.write_params_doc(params.params_pointer(), "InitialParams");
		cost = call_cmaes(params, opts.simulationtime, opts.iterationnumber, operation, wbv);
	}
	else
		return(0);

	return cost;
}

// Creates the window and shows the simulation. The output is the simulation cost.
float CallNewton(double* params, int simulationtime, int write_txt, WBV wbv)
{
	WindowMain* ContextGL = new WindowMain();
	//
	ContextGL->SetUseMouseViewRotation(true);
	//	 
	WalkerManager* aWalkerManager = new WalkerManager(ContextGL);
	dMatrix matrix(dGetIdentityMatrix());
	dWalkerModel* const Model = aWalkerManager->CreateWalkerPlayer("WALKER", matrix, params, wbv);
	if (write_txt)
		Model->write_txt = 1;
	else
		Model->write_txt = 0;
	/*Model->controller.SetOptimizationParam(params);*/

	float cost = 0, mass = 0;
	vector<float> reward;
	if (ContextGL != NULL) {
		ContextGL->SetSimulationTime(0.0);// [s]
		ContextGL->SetMaxSimulationTime(simulationtime); //set max time in s of simulation
		if (wbv.active.compare("yes") == 0)
			ContextGL->SetSimulationInfo(simulationtime, 0, 0, 0, wbv.direction, wbv.amplitude, wbv.frequency); // SET WBV info
		else
			ContextGL->SetSimulationInfo(simulationtime, 0, 0, 0, "None", 0.0, 0.0); // SET WBV info
		ContextGL->MainLoop();

		reward = Model->controller.GetRewardValues();
		mass = Model->GetModelMass();
		float we = 0.004 / mass; 
		float wm = 100;// weight for Muscles torques
		float wpd = 1; // weight for PD torques
		float T = 3000 * ContextGL->GetMaxSimulationTime(); // 3000 Hz * simulation time (5s) 
		cost = reward[0] + we / T * (wm * reward[1] + wpd * reward[2]);

		if (write_txt)
		{
			// open file for writing
			ofstream fw("Gait_timeseries.txt", std::ofstream::out);
			int ok = 1;
			//get array size
			int arraySize = Model->time_v.size();
			//exception handling
			try {
				//open file for writing
				ofstream fw("Gait_timeseries.txt", std::ofstream::out);
				//check if file was successfully opened for writing
				if (fw.is_open())
				{
					fw << "Time [s]" << "\t"
						<< "HAMr Torque1 [Nm]" << "\t" << "HAMr Torque2 [Nm]" << "\t"
						<< "GLUr Torque1 [Nm]" << "\t" 
						<< "HFLr Torque1 [Nm]" << "\t"
						<< "RFr Torque1 [Nm]" << "\t" << "RFr Torque2 [Nm]" << "\t"
						<< "VASr Torque1 [Nm]" << "\t"
						<< "GASr Torque1 [Nm]" << "\t" << "GASr Torque2 [Nm]" << "\t"
						<< "SOLr Torque1 [Nm]" << "\t"
						<< "TAr Torque1 [Nm]" << "\t"
						<< "HAMl Torque1 [Nm]" << "\t" << "HAMl Torque2 [Nm]" << "\t"
						<< "GLUl Torque1 [Nm]" << "\t"
						<< "HFLl Torque1 [Nm]" << "\t"
						<< "RFl Torque1 [Nm]" << "\t" << "RFl Torque2 [Nm]" << "\t"
						<< "VASl Torque1 [Nm]" << "\t"
						<< "GASl Torque1 [Nm]" << "\t" << "GASl Torque2 [Nm]" << "\t"
						<< "SOLl Torque1 [Nm]" << "\t"
						<< "TAl Torque1 [Nm]" << "\t"
						<< "Hipr Angle [rad]" << "\t" << "Kneer Angle [rad]" << "\t" << "Ankler Angle [rad]" << "\t"
						<< "Hipl Angle [rad]" << "\t" << "Kneel Angle [rad]" << "\t" << "Anklel Angle [rad]" << "\t"
						<< "COM Position X [m]" << "\t" << "COM Position Y [m]" << "\t" << "COM Position Z [m]" << "\t"
						<< "COM Velocity X [m/s]" << "\t" << "COM Velocity Y [m/s]" << "\t" << "COM Velocity Z [m/s]" << "\t"
						<< "Footr Position X [m/s]" << "\t" << "Footr Position Y [m/s]" << "\t" << "Footr Position Z [m/s]" << "\t"
						<< "Footl Position X [m/s]" << "\t" << "Footl Position Y [m/s]" << "\t" << "Footl Position Z [m/s]" << "\t"
						<< "Stancer" << "\t" << "Swingr" << "\t" << "SPr" << "\t" << "SIr" << "\t"
						<< "Stancel" << "\t" << "Swingl" << "\t" << "SPl" << "\t" << "SIl" << "\t"
						"\n";
					//store array contents to text file
					for (int i = 0; i < arraySize; i++) {
						fw << Model->time_v[i] << "\t" 
							<< Model->HAMr_T1[i] << "\t" << Model->HAMr_T2[i] << "\t"
							<< Model->GLUr_T1[i] << "\t"
							<< Model->HFLr_T1[i] << "\t"
							<< Model->RFr_T1[i] << "\t" << Model->RFr_T2[i] << "\t"
							<< Model->VASr_T1[i] << "\t" 
							<< Model->GASr_T1[i] << "\t" << Model->GASr_T2[i] << "\t"
							<< Model->SOLr_T1[i] << "\t"
							<< Model->TAr_T1[i] << "\t"
							<< Model->HAMl_T1[i] << "\t" << Model->HAMl_T2[i] << "\t"
							<< Model->GLUl_T1[i] << "\t" 
							<< Model->HFLl_T1[i] << "\t"
							<< Model->RFl_T1[i] << "\t" << Model->RFl_T2[i] << "\t"
							<< Model->VASl_T1[i] << "\t" 
							<< Model->GASl_T1[i] << "\t" << Model->GASl_T2[i] << "\t"
							<< Model->SOLl_T1[i] << "\t" 
							<< Model->TAl_T1[i] << "\t"
							<< Model->HipRAng[i] << "\t" << Model->KneeRAng[i] << "\t" << Model->AnkleRAng[i] << "\t"
							<< Model->HipLAng[i] << "\t" << Model->KneeLAng[i] << "\t" << Model->AnkleLAng[i] << "\t"
							<< Model->COMPos_X[i] << "\t" << Model->COMPos_Y[i] << "\t" << Model->COMPos_Z[i] << "\t"
							<< Model->COMVel_X[i] << "\t" << Model->COMVel_Y[i] << "\t" << Model->COMVel_Z[i] << "\t"
							<< Model->FootRPos_X[i] << "\t" << Model->FootRPos_Y[i] << "\t" << Model->FootRPos_Z[i] << "\t"
							<< Model->FootLPos_X[i] << "\t" << Model->FootLPos_Y[i] << "\t" << Model->FootLPos_Z[i] << "\t"
							<< Model->StanceR[i] << "\t" << Model->SwingR[i] << "\t" << Model->SPR[i] << "\t" << Model->SIR[i] << "\t"
							<< Model->StanceL[i] << "\t" << Model->SwingL[i] << "\t" << Model->SPL[i] << "\t" << Model->SIL[i] <<
							"\n";
					}
					fw.close();
				}
				else cout << "Problem with opening file";
			}
			catch (const char* msg) {
				cerr << msg << endl;
				ok = 0;
			}

		}

		// delete context and newton manager and close tutorial.
		if (Model) { delete(Model); }
		delete ContextGL;
		//
	}

	
	return cost;
}

// Overload for cmaes: Creates the window and shows the simulation. The output is the simulation cost.
float CallNewton(double* params, int simulationtime, int iteration_max, int current_iteration, int popsize, WBV wbv)
{
	WindowMain* ContextGL = new WindowMain();
	//
	ContextGL->SetUseMouseViewRotation(true);
	//	 
	WalkerManager* aWalkerManager = new WalkerManager(ContextGL);
	dMatrix matrix(dGetIdentityMatrix());
	dWalkerModel* const Model = aWalkerManager->CreateWalkerPlayer("WALKER", matrix, params, wbv);

	Model->controller.SetOptimizationParam(params);

	float cost = 0, mass = 0;
	vector<float> reward;
	if (ContextGL != NULL) {
		ContextGL->SetSimulationTime(0.0);// [s]
		ContextGL->SetMaxSimulationTime(simulationtime); //set max time in s of simulation
		int remaining_time = (iteration_max - current_iteration) * ContextGL->GetMaxSimulationTime() * popsize; 
		if (wbv.active.compare("yes")==0)
			ContextGL->SetSimulationInfo(simulationtime, current_iteration, iteration_max, remaining_time,wbv.direction,wbv.amplitude,wbv.frequency); // SET WBV info
		else
			ContextGL->SetSimulationInfo(simulationtime, current_iteration, iteration_max, remaining_time, "None", 0.0, 0.0); // SET WBV info
		ContextGL->MainLoop();

		reward = Model->controller.GetRewardValues();
		mass = Model->GetModelMass();
		float we = 0.004 / mass; // weight for torques
		float wm = 100;// weight for Muscles torques
		float wpd = 1; // weight for PD torques
		float T = 3000 * ContextGL->GetMaxSimulationTime(); // 3000 Hz * simulation time (5s) 
		cost = reward[0] + we / T * (wm * reward[1] + wpd * reward[2]);
		// delete context and newton manager and close tutorial.
		if (Model) { delete(Model); }
		delete ContextGL;
		//
	}

	return cost;
}

// is the fitness function for CMAES algorithm
double fitfun(double const* x, int N, OptParams P, int simulationtime, int iteration_max, int current_it, int popsize, WBV wbv)// cambia in call newton. 
{
	double y[62];
	for (int i = 0; i < N;i++)
		y[i] = abs(x[i]);// just positive values.
	double* parameters = P.RemoveScaling(y);//not scaled params
	double sum = CallNewton(parameters, simulationtime, iteration_max, current_it, popsize, wbv);
	return sum;
}

// calls CMAES class. At the end writes a XML file with optimized parapeters
int call_cmaes(OptParams params, int simulationtime, int iteration_max, int operation, WBV wbv)
{

	CMAES<double> evo;
	double* arFunvals, *const* pop, * xfinal;

	double* params_scaled = params.params_pointer();

	// Initialize everything
	//int dim = params.ParamList.size(); // change according to optimization parameters in XML file
	const int dim = 62;
	double xstart[dim];
	for (int i = 0; i < dim; i++) xstart[i] = params_scaled[i];
	double stddev[dim]; // assign sigma to each parameter
	for (int i = 0; i < dim; i++) stddev[i] = 0.2; 
	Parameters<double> parameters;
	// TODO Adjust parameters here
	parameters.init(dim, xstart, stddev);
	parameters.stopTolX = 1e-11;
	parameters.stopMaxIter = iteration_max; 
	parameters.lambda = 4 + (int)(3.0 * log((double)dim));
	arFunvals = evo.init(parameters);

	std::cout << evo.sayHello() << std::endl;

	// Iterate until stop criterion holds
	while (!evo.testForTermination())
	{
		// Generate lambda new search points, sample population
		pop = evo.samplePopulation(); // Do not change content of pop

		/* Here you may resample each solution point pop[i] until it
		   becomes feasible, e.g. for box constraints (variable
		   boundaries). function is_feasible(...) needs to be
		   user-defined.
		   Assumptions: the feasible domain is convex, the optimum is
		   not on (or very close to) the domain boundary, initialX is
		   feasible and initialStandardDeviations are sufficiently small
		   to prevent quasi-infinite looping.
		*/
		for (int i = 0; i < evo.get(CMAES<double>::PopSize); ++i)
			while (!is_feasible(pop[i]))// just positive values
				evo.reSampleSingle(i);
 
		// evaluate the new search points using fitfun from above
		for (int i = 0; i < evo.get(CMAES<double>::Lambda); ++i)
			arFunvals[i] = fitfun(pop[i], (int)evo.get(CMAES<double>::Dimension), params, simulationtime, iteration_max, (int)evo.get(CMAES<double>::Generation), parameters.lambda, wbv);

		// update the search distribution used for sampleDistribution()
		evo.updateDistribution(arFunvals);
	}
	std::cout << "Stop:" << std::endl << evo.getStopMessage();
	evo.writeToFile(CMAES<double>::WKAll, "resumeevo.dat"); // write resumable state of CMA-ES

	// get best estimator for the optimum, xmean
	//xfinal = evo.getNew(CMAES<double>::XMean); // "XBestEver" might be used as well
	xfinal = evo.getNew(CMAES<double>::XBestEver); 

	//xfinal = params.RemoveScaling(xfinal);
	params.write_params_doc(xfinal, "Params");
	// do something with final solution and finally release memory
	//delete[] xfinal; error, why?

	return 0;
}