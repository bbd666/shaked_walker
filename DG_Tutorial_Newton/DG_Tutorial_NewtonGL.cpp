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

#include "pch.h"
#include "WindowGL.h"
#include "GeomGL.h"
#include "biped.h"
#include "OXVehicleRayCast.h"
#include "LoadXML.h"
#include <iostream>
#include "cmaes.h"

float CallNewton(double* params); // overload for optimization
int call_cmaes(ModelParams params);
// boundary condition for parameters. Parameters must be positive or 0
bool is_feasible(double* p) {
	bool flag = true;
	for (int ii = 0; ii<40; ii++)
		if (p[ii] < 0)
			flag = false;
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
	int operation = 1; //"To run simulation enter 1, to run optimization enter 2:", else nothing.
	// take inputs
	#if defined(NDEBUG)
		// Convert the wide character wchar_t string to a string
		wchar_t** origin = __wargv;
		vector<string> argv(__argc);
		for (int i = 0; i < __argc; ++i) {
			wstring ws(origin[i]);
			string str(ws.begin(), ws.end());
			argv[i] = str;
		}

		if (__argc > 1) {
			operation = stof(argv[1]);
		}
	#else
		if (argc > 1) {
			operation = stof(argv[1]);
		}
	#endif

	// load optimization parameters form xml
	ModelParams params;
	bool flag_xml = params.load("OptimizationParameters.xml");
	if (flag_xml == 1)
	{
		cout << "Error in loading model parameters from ModelParameters.xml file" << endl;
		return 0;
	}

	// run simulation/optimization
	int cost = 1;
	if (operation == 1)// run simulation
		cost = CallNewton(params.params_pointer());
	else if (operation == 2) // run optimization 
		cost = call_cmaes(params);
	else
		return(0);

	return cost;
}

// Creates the window and shows the simulation. The output is the simulation cost.
float CallNewton(double* params)
{
	WindowMain* ContextGL = new WindowMain();
	//
	ContextGL->SetUseMouseViewRotation(true);
	//	 
	DGVehicleRCManager* aWalkerManager = new DGVehicleRCManager(ContextGL);
	dMatrix matrix(dGetIdentityMatrix());
	dRaycastVHModel* const Model = aWalkerManager->CreateWalkerPlayer("WALKER", matrix);

	Model->controller.SetOptimizationParam(params);

	float cost = 0, mass = 0;
	vector<float> reward;
	if (ContextGL != NULL) {
		ContextGL->SetSimulationTime(0.0);// [s]
		ContextGL->SetMaxSimulationTime(5.0); //set max 5 s of simulation
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
double fitfun(double const* x, int N, ModelParams P)// cambia in call newton. 
{
	double* parameters = P.RemoveScaling(x);//not scaled params
	double sum = CallNewton(parameters);
	return sum;
}

// calls CMAES class. At the end writes a XML file with optimized parapeters
int call_cmaes(ModelParams params)
{

	CMAES<double> evo;
	double* arFunvals, * const* pop, * xfinal;

	double* params_scaled = params.ScaleParameters(); //parameter scaled for optimization

	// Initialize everything
	const int dim = 40; // change according to optimization parameters in XML file
	double xstart[dim];
	for (int i = 0; i < dim; i++) xstart[i] = params_scaled[i];
	double stddev[dim]; // assign sigma to each parameter
	for (int i = 0; i < dim; i++) stddev[i] = 0.005; /// between 0.1 and 0.001
	Parameters<double> parameters;
	// TODO Adjust parameters here
	parameters.init(dim, xstart, stddev);
	parameters.stopTolX = 1e-11;
	parameters.stopMaxIter = 100; // max 3000
	parameters.lambda = 50; // 50  Wang or 4 + (int)(3.0 * log((double)dim))
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
			arFunvals[i] = fitfun(pop[i], (int)evo.get(CMAES<double>::Dimension), params);

		// update the search distribution used for sampleDistribution()
		evo.updateDistribution(arFunvals);
	}
	std::cout << "Stop:" << std::endl << evo.getStopMessage();
	evo.writeToFile(CMAES<double>::WKAll, "resumeevo1.dat"); // write resumable state of CMA-ES

	// get best estimator for the optimum, xmean
	//xfinal = evo.getNew(CMAES<double>::XMean); // "XBestEver" might be used as well
	xfinal = evo.getNew(CMAES<double>::XBestEver); 

	xfinal = params.RemoveScaling(xfinal);
	params.write_optimized_params_doc(xfinal);
	// do something with final solution and finally release memory
	//delete[] xfinal; error, why?

	return 0;
}