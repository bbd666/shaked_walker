# Walking Humanoid Shaked

11/05/2023

## Introduction

This application simulates a walking humanoid in a 3D real-physics environment. The floor can oscillate in roll or pitch direction with a sinusoidal wave.

## Usage

Run "Walking_Humanoid_Shaked.exe" the to see the simulation. The .exe file returns the "cost" of the simulation, which is a numeric value that quantifies the quality of the simulation (low value -> the model walks, high value-> the model falls). 
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


## System requirements

* GPU
* OpenGL
* glfw3

## Info

CMAES link: https://cma-es.github.io/
Model document: "Modelling and experiments on the effects of whole-body vibration while walking", Stefano Marelli, 2023 PhD thesis, Mechanical Department Politecnico di Milano.


## Reporting bugs
The window can crash on exit. In this case it is necessary to force stop the app.

## Author
Stefano Marelli