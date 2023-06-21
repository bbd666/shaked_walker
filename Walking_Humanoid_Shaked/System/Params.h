#ifndef _PARAMS_H_
#define _PARAMS_H_

#pragma once
#include "tinyxml.h"
#include "pch.h"

#include <string>
#include <map>
using namespace std;
typedef std::map<std::string, std::string> MessageMap;
/**
 * @class OptParams
 * useful to load and write Optimization parameters from OptimizationParameters.xml file
 */
class OptParams
{
public:
	OptParams() {}
	~OptParams() 
	{ 
		// something to delete?
	}
	//! vector with the optimization parameters.
	std::vector<double> ParamList;
	std::vector<double> IC_min, IC_max, SL_min, SL_max, FF_min, FF_max, LF_min, LF_max, CL_min, CL_max, C_min, C_max, A_min, A_max, Additional_min, Additional_max,
		PreSI_max, PreSI_min, min_v, PreSTANCE_max, PreSTANCE_min, Stance_prep_min, Stance_prep_max, Simbicon_max, Simbicon_min, max_v;
	vector<double> parameters_scaled, params_not_scaled;
	//! pointer to the vector with the optimization parameters
	double* params_pointer() {
		return &ParamList[0];
	}
	/**
	 * Loading optimization parameters form xml
	 * @param pFilename (input) name of the xml file
	 * @param flag_error_xml (output) flag error.
	 */
	bool load(const char* pFilename) {
		TiXmlDocument doc(pFilename);
		if (!doc.LoadFile()) return(1);
		bool flag_error_xml = 0; // flag error on loading values from xml file

		TiXmlHandle hDoc(&doc);
		TiXmlElement* pElem;
		TiXmlHandle hRoot(0);

		{
			pElem = hDoc.FirstChildElement().Element();

			// save this for later
			hRoot = TiXmlHandle(pElem);
		}

		// parameters
		{
			ParamList.clear();

			char IC_arr[7][6] = { {"trunk"}, {"alfaR"}, {"alfaL"}, {"beta"}, {"gamma" }, { "head" }, { "vel" } }; // names of each attribute of Initial Condition in XML file.
			IC_min = { 0, 0, 0, 0, 0, 0, 0 }; // minima for initial condition params
			IC_max = { 20.0*dDegreeToRad, 20.0*dDegreeToRad, 30.0*dDegreeToRad, 20.0*dDegreeToRad, 20.0*dDegreeToRad, 20.0*dDegreeToRad, 3.5 }; // maxima for initial condition params
			char SL_arr[9][6] = { {"hamP"}, {"hamA"}, {"hamD"}, {"gluP"}, {"gluA" }, { "gluD" }, { "hflP" }, {"hflA"}, {"hflD"}};
			SL_min = { 1.78, 0.97*dDegreeToRad, 0.1, 1.0, 0.97 * dDegreeToRad, 0.1,1.78, 0.97 * dDegreeToRad, 0.1 };
			SL_max = { 22, 6.3 * dDegreeToRad, 0.5, 22, 6.3 * dDegreeToRad, 0.5, 22, 6.3 * dDegreeToRad, 0.5};
			char FF_arr[6][6] = { {"glu"}, {"ham"}, {"vas"}, {"sol"}, {"gas" }, { "tasol" }};
			FF_min = { 0, 0, 0.82, 0.97, 0, 0 };
			FF_max = { 1.0, 0.67, 13.5, 2.17, 10, 10};
			char LF_arr[6][6] = { {"hflG"}, {"hamG"}, {"taG"}, {"hflH"}, {"hamH" }, { "taH" }};
			LF_min = { 0.17, 0, 0.55, 0, 0.6, 0.59 };
			LF_max = { 5, 100, 3.2, 0.67, 10, 2.0};
			char CL_arr[4][7] = { {"trunkP"}, {"trunkV"}, {"footP"}, {"footV"}};
			CL_min = { 100, 10, 10, 1 };
			CL_max = { 3000, 300, 300, 30 };
			char C_arr[6][8] = { {"trunkP"}, {"trunkV"}, {"footP"}, {"footV"}, {"TtrunkP"}, {"TtrunkV"}};
			C_min = { 50, 0, 1, 0, 50, 0 };
			C_max = { 200, 30, 100, 10, 500, 50};
			char A_arr[2][6] = { {"alpha"}, {"elbow"}};
			A_min = { 0, 0 };
			A_max = { 1, 90.0*dDegreeToRad };
			char PreSI_arr[4][6] = { {"vas"}, {"rf"}, {"glu"}, {"hfl"}};
			PreSI_min = { 0.01, 0.01, 0.01, 0.01 };
			PreSI_max = { 1, 1, 1, 1 };
			char PreSTANCE_arr[2][6] = { {"vas"}, {"hip"}};
			PreSTANCE_min = { 0.01, 0.01 };
			PreSTANCE_max = { 1, 1 };
			char Stanceprep_arr[9][6] = { {"hflK"}, {"hflD"},{"hflA"},{"gluK"}, {"gluD"},{"gluA"} ,{"vasK"}, {"vasD"},{"vasA"} };
			Stance_prep_min = {0.1, 0.001, 100 * dDegreeToRad, 0.1, 0.01, 100 * dDegreeToRad,0.01, 0.01, 100 * dDegreeToRad };
			Stance_prep_max = { 2, 0.5, 180 * dDegreeToRad, 2, 0.5, 180 * dDegreeToRad,2, 0.5, 180 * dDegreeToRad };
			char Simbicon_arr[4][6] = { {"cd"}, {"cv"}, {"cd1"}, {"cv1"} };
			Simbicon_min = { 0.4,0.1,0.0,0.0};
			Simbicon_max = { 0.6, 0.3, 0.6, 0.3};
			char Additional_arr[3][6] = { {"hflK"}, {"vasK"},{"vasA"} };
			Additional_min = { 0.0, 0.0, 160 * dDegreeToRad };
			Additional_max = { 2.0, 3.0, 180 * dDegreeToRad };

			min_v.insert(min_v.end(), IC_min.begin(), IC_min.end());
			min_v.insert(min_v.end(), SL_min.begin(), SL_min.end());
			min_v.insert(min_v.end(), FF_min.begin(), FF_min.end());
			min_v.insert(min_v.end(), LF_min.begin(), LF_min.end());
			min_v.insert(min_v.end(), CL_min.begin(), CL_min.end());
			min_v.insert(min_v.end(), C_min.begin(), C_min.end());
			min_v.insert(min_v.end(), A_min.begin(), A_min.end());
			min_v.insert(min_v.end(), PreSI_min.begin(), PreSI_min.end());
			min_v.insert(min_v.end(), PreSTANCE_min.begin(), PreSTANCE_min.end());
			min_v.insert(min_v.end(), Stance_prep_min.begin(), Stance_prep_min.end());
			min_v.insert(min_v.end(), Simbicon_min.begin(), Simbicon_min.end());
			min_v.insert(min_v.end(), Additional_min.begin(), Additional_min.end());

			max_v.insert(max_v.end(), IC_max.begin(), IC_max.end());
			max_v.insert(max_v.end(), SL_max.begin(), SL_max.end());
			max_v.insert(max_v.end(), FF_max.begin(), FF_max.end());
			max_v.insert(max_v.end(), LF_max.begin(), LF_max.end());
			max_v.insert(max_v.end(), CL_max.begin(), CL_max.end());
			max_v.insert(max_v.end(), C_max.begin(), C_max.end());
			max_v.insert(max_v.end(), A_max.begin(), A_max.end());
			max_v.insert(max_v.end(), PreSI_max.begin(), PreSI_max.end());
			max_v.insert(max_v.end(), PreSTANCE_max.begin(), PreSTANCE_max.end());
			max_v.insert(max_v.end(), Stance_prep_max.begin(), Stance_prep_max.end());
			max_v.insert(max_v.end(), Simbicon_max.begin(), Simbicon_max.end());
			max_v.insert(max_v.end(), Additional_max.begin(), Additional_max.end());

			TiXmlElement* pWindowNode = hRoot.FirstChild().Element();
		

			// loop all sets of params
			for (pWindowNode; pWindowNode != NULL; pWindowNode = pWindowNode->NextSiblingElement())
			{
				if (strcmp(pWindowNode->Value(), "InitialCondition") == 0)
				{
					// loop on all params of each set. TO DO: use just one fot loop and a function
					for (int ii = 0; ii < 7; ++ii)
					{
						double value = -1;// initialization
						pWindowNode->QueryDoubleAttribute(&IC_arr[ii][0], &value);
						ParamList.push_back(value);
						if (value == -1)
							flag_error_xml = 1;
					}
				}
				else if (strcmp(pWindowNode->Value(), "StanceLead") == 0)
				{
					for (int ii = 0; ii < 9; ++ii)
					{
						double value = -1;
						pWindowNode->QueryDoubleAttribute(&SL_arr[ii][0], &value);
						ParamList.push_back(value);
						if (value == -1)
							flag_error_xml = 1;
					}
				}
				else if (strcmp(pWindowNode->Value(), "ForceFeedback") == 0)
				{
					for (int ii = 0; ii < 6; ++ii)
					{
						double value = -1;
						pWindowNode->QueryDoubleAttribute(&FF_arr[ii][0], &value);
						ParamList.push_back(value);
						if (value == -1)
							flag_error_xml = 1;
					}
				}
				else if (strcmp(pWindowNode->Value(), "LengthFeedback") == 0)
				{
					for (int ii = 0; ii < 6; ++ii)
					{
						double value = -1;
						pWindowNode->QueryDoubleAttribute(&LF_arr[ii][0], &value);
						ParamList.push_back(value);
						if (value == -1)
							flag_error_xml = 1;
					}
				}
				else if (strcmp(pWindowNode->Value(), "Coronal_lead") == 0)
				{
					for (int ii = 0; ii < 4; ++ii)
					{
						double value = -1;
						pWindowNode->QueryDoubleAttribute(&CL_arr[ii][0], &value);
						ParamList.push_back(value);
						if (value == -1)
							flag_error_xml = 1;
					}
				}
				else if (strcmp(pWindowNode->Value(), "Coronal") == 0)
				{
					for (int ii = 0; ii < 6; ++ii)
					{
						double value = -1;
						pWindowNode->QueryDoubleAttribute(&C_arr[ii][0], &value);
						ParamList.push_back(value);
						if (value == -1)
							flag_error_xml = 1;
					}
				}
				else if (strcmp(pWindowNode->Value(), "Arm") == 0)
				{
					for (int ii = 0; ii < 2; ++ii)
					{
						double value = -1;
						pWindowNode->QueryDoubleAttribute(&A_arr[ii][0], &value);
						ParamList.push_back(value);
						if (value == -1)
							flag_error_xml = 1;
					}
				}
				else if (strcmp(pWindowNode->Value(), "PreSI") == 0)
				{
					for (int ii = 0; ii < 4; ++ii)
					{
						double value = -1;
						pWindowNode->QueryDoubleAttribute(&PreSI_arr[ii][0], &value);
						ParamList.push_back(value);
						if (value == -1)
							flag_error_xml = 1;
					}
				}
				else if (strcmp(pWindowNode->Value(), "PreSTANCE") == 0)
				{
					for (int ii = 0; ii < 2; ++ii)
					{
						double value = -1;
						pWindowNode->QueryDoubleAttribute(&PreSTANCE_arr[ii][0], &value);
						ParamList.push_back(value);
						if (value == -1)
							flag_error_xml = 1;
					}
				}
				else if (strcmp(pWindowNode->Value(), "StancePrep") == 0)
				{
					for (int ii = 0; ii < 9; ++ii)
					{
						double value = -1;
						pWindowNode->QueryDoubleAttribute(&Stanceprep_arr[ii][0], &value);
						ParamList.push_back(value);
						if (value == -1)
							flag_error_xml = 1;
					}
				}
				else if (strcmp(pWindowNode->Value(), "Simbicon") == 0)
				{
				for (int ii = 0; ii < 4; ++ii)
				{
					double value = -1;
					pWindowNode->QueryDoubleAttribute(&Simbicon_arr[ii][0], &value);
					ParamList.push_back(value);
					if (value == -1)
						flag_error_xml = 1;
				}
				}
				else if (strcmp(pWindowNode->Value(), "Additional") == 0)
				{
				for (int ii = 0; ii < 3; ++ii)
				{
					double value = -1;
					pWindowNode->QueryDoubleAttribute(&Additional_arr[ii][0], &value);
					ParamList.push_back(value);
					if (value == -1)
						flag_error_xml = 1;
				}
				}
			}
		}
		return(flag_error_xml);
	};

	/**
	 * remove scaling of params, X_SCALED * (max - min) + min
	 * @param scaled_params (input) pointer to scaled parameters
	 * @param params_not_scaled (output) pointer to un-scaled parameters.
	 */
	double* RemoveScaling(double * scaled_params)
	{
		params_not_scaled.clear();
		for(int ii = 0; ii < 62; ii++)
			params_not_scaled.push_back(scaled_params[ii] * (max_v[ii] - min_v[ii]) + min_v[ii]);
		return &params_not_scaled[0];
	}

	/**
	 * scaling of params, (X - min) / (max - min) 
	 * @param params_not_scaled (output) pointer to scaled parameters.
	 */
	double* ScaleParameters()
	{
		parameters_scaled.clear();

		for (int ii = 0; ii < ParamList.size(); ii++)
			parameters_scaled.push_back((ParamList[ii] - min_v[ii]) / (max_v[ii] - min_v[ii]));// NORMALIZATION BEFORE OPTIMIZATION
		return &parameters_scaled[0];
	}

	/**
	 * writes a XML file with  params
	 * @param opt_params (input) pointer to parameters.
	 * @param Name (input) string to check if the params are the optimized or the initial.
	 */
	void write_params_doc(double * opt_params, const string &Name)
	{
		TiXmlDocument doc;
		TiXmlElement* msg;
		string check = "Params";
		if (Name.compare(check) == 0)
			doc.LoadFile("OptimizationParameters.xml");

		TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
		doc.LinkEndChild(decl);

		TiXmlElement* root = new TiXmlElement(Name.c_str());
		doc.LinkEndChild(root);

		TiXmlComment* comment = new TiXmlComment();
		//comment->SetValue(" Bestever parameters of CMAES ");
		//root->LinkEndChild(comment);

		TiXmlElement* IC;
		IC = new TiXmlElement("InitialCondition");
		root->LinkEndChild(IC);
		IC->SetDoubleAttribute("trunk", opt_params[0]);
		IC->SetDoubleAttribute("alfaR", opt_params[1]);
		IC->SetDoubleAttribute("alfaL", opt_params[2]);
		IC->SetDoubleAttribute("beta",  opt_params[3]);
		IC->SetDoubleAttribute("gamma", opt_params[4]);
		IC->SetDoubleAttribute("head",  opt_params[5]);
		IC->SetDoubleAttribute("vel",   opt_params[6]);
		TiXmlElement* SL;
		SL = new TiXmlElement("StanceLead");
		root->LinkEndChild(SL);
		SL->SetDoubleAttribute("hamP", opt_params[7]);
		SL->SetDoubleAttribute("hamA", opt_params[8]);
		SL->SetDoubleAttribute("hamD", opt_params[9]);
		SL->SetDoubleAttribute("gluP", opt_params[10]);
		SL->SetDoubleAttribute("gluA", opt_params[11]);
		SL->SetDoubleAttribute("gluD", opt_params[12]);
		SL->SetDoubleAttribute("hflP", opt_params[13]);
		SL->SetDoubleAttribute("hflA", opt_params[14]);
		SL->SetDoubleAttribute("hflD", opt_params[15]);
		TiXmlElement* FF;
		FF = new TiXmlElement("ForceFeedback");
		root->LinkEndChild(FF);
		FF->SetDoubleAttribute("glu", opt_params[16]);
		FF->SetDoubleAttribute("ham", opt_params[17]);
		FF->SetDoubleAttribute("vas", opt_params[18]);
		FF->SetDoubleAttribute("sol", opt_params[19]);
		FF->SetDoubleAttribute("gas", opt_params[20]);
		FF->SetDoubleAttribute("tasol", opt_params[21]);
		TiXmlElement* LF;
		LF = new TiXmlElement("LengthFeedback");
		root->LinkEndChild(LF);
		LF->SetDoubleAttribute("hflG", opt_params[22]);
		LF->SetDoubleAttribute("hamG", opt_params[23]);
		LF->SetDoubleAttribute("taG", opt_params[24]);
		LF->SetDoubleAttribute("hflH", opt_params[25]);
		LF->SetDoubleAttribute("hamH", opt_params[26]);
		LF->SetDoubleAttribute("taH", opt_params[27]);
		TiXmlElement* CL;
		CL = new TiXmlElement("Coronal_lead");
		root->LinkEndChild(CL);
		CL->SetDoubleAttribute("trunkP", opt_params[28]);
		CL->SetDoubleAttribute("trunkV", opt_params[29]);
		CL->SetDoubleAttribute("footP", opt_params[30]);
		CL->SetDoubleAttribute("footV", opt_params[31]);
		TiXmlElement* C;
		C = new TiXmlElement("Coronal");
		root->LinkEndChild(C);
		C->SetDoubleAttribute("trunkP", opt_params[32]);//rename
		C->SetDoubleAttribute("trunkV", opt_params[33]);//rename
		C->SetDoubleAttribute("footP", opt_params[34]);//rename
		C->SetDoubleAttribute("footV", opt_params[35]);//rename
		C->SetDoubleAttribute("TtrunkP", opt_params[36]);
		C->SetDoubleAttribute("TtrunkV", opt_params[37]);
		TiXmlElement* A;
		A = new TiXmlElement("Arm");
		root->LinkEndChild(A);
		A->SetDoubleAttribute("alpha", opt_params[38]);
		A->SetDoubleAttribute("elbow", opt_params[39]);
		TiXmlElement* pre_si;
		pre_si = new TiXmlElement("PreSI");
		root->LinkEndChild(pre_si);
		pre_si->SetDoubleAttribute("vas", opt_params[40]);
		pre_si->SetDoubleAttribute("rf", opt_params[41]);
		pre_si->SetDoubleAttribute("glu", opt_params[42]);
		pre_si->SetDoubleAttribute("hfl", opt_params[43]);
		TiXmlElement* pre_st;
		pre_st = new TiXmlElement("PreSTANCE");
		root->LinkEndChild(pre_st);
		pre_st->SetDoubleAttribute("vas", opt_params[44]);
		pre_st->SetDoubleAttribute("hip", opt_params[45]);
		TiXmlElement* st_prep;
		st_prep = new TiXmlElement("StancePrep");
		root->LinkEndChild(st_prep);
		st_prep->SetDoubleAttribute("hflK", opt_params[46]);
		st_prep->SetDoubleAttribute("hflD", opt_params[47]);
		st_prep->SetDoubleAttribute("hflA", opt_params[48]);
		st_prep->SetDoubleAttribute("gluK", opt_params[49]);
		st_prep->SetDoubleAttribute("gluD", opt_params[50]);
		st_prep->SetDoubleAttribute("gluA", opt_params[51]);
		st_prep->SetDoubleAttribute("vasK", opt_params[52]);
		st_prep->SetDoubleAttribute("vasD", opt_params[53]);
		st_prep->SetDoubleAttribute("vasA", opt_params[54]);
		TiXmlElement* simbicon;
		simbicon = new TiXmlElement("Simbicon");
		root->LinkEndChild(simbicon);
		simbicon->SetDoubleAttribute("cd", opt_params[55]);
		simbicon->SetDoubleAttribute("cv", opt_params[56]);
		simbicon->SetDoubleAttribute("cd1", opt_params[57]);
		simbicon->SetDoubleAttribute("cv1", opt_params[58]);
		TiXmlElement* additional;
		additional = new TiXmlElement("Additional");
		root->LinkEndChild(additional);
		additional->SetDoubleAttribute("hflK", opt_params[59]);
		additional->SetDoubleAttribute("vasK", opt_params[60]);
		additional->SetDoubleAttribute("vasA", opt_params[61]);
		//dump_to_stdout(&doc);
		doc.SaveFile("OptimizationParameters.xml");
	}
};

/**
 * @class Options
 * useful to load Options from configuration.xml file
 */
class Options
{
public:
	//! option: simulation time in [s] and iteration number for the CMAES routine
	int simulationtime, iterationnumber;
	//! option: optimization routine and output flags
	string optimization, output;

	Options()
		: simulationtime(8), iterationnumber(50), output("no"), optimization("no")
	{
	}

	Options(int simulationtime, const string& optimization, int iterationnumber, const string& output)
	{
		this->simulationtime = simulationtime;
		this->optimization = optimization;
		this->iterationnumber = iterationnumber;
		this->output = output;
	}
};
/**
 * @class WBV
 * useful to load Whole-Body vibration from configuration.xml file
 */
class WBV
{
public:
	//! WBV option: amplitude in [°] and frequency in [Hz]
	float amplitude, frequency;
	//! WBV option: active flag and direction.
	string  active, direction;

	WBV()
		: amplitude(4), frequency(0.25), active("no"), direction("roll")
	{
	}

	WBV(const string& active, const string& direction, float amplitude, float frequency)
	{
		this->active = active;
		this->direction = direction;
		this->amplitude = amplitude;
		this->frequency = frequency;
	}
};

class AppSettings
{
public:
	//! Setting name
	string m_name;
	//! Options list
	list<Options> m_options;
	//! WBV options list
	list<WBV> m_wbv;

	AppSettings() {}

	// just to show how to do it
	void setDemoValues()
	{
		m_name = "Shaked Walker";
		m_options.clear();
		m_options.push_back(Options(8, "no", 50, "no"));
		m_wbv.clear();
		m_wbv.push_back(WBV("no", "no", 4, 0.25));
	}
	/**
	* Loading optimization parameters form xml
	* @param pFilename (input) name of the xml file
	* @param flag_error_xml (output) error flag.
	*/
	bool load(const char* pFilename)
	{
		TiXmlDocument doc(pFilename);
		if (!doc.LoadFile()) return(1);
		bool flag_error_xml = 0; // flag error on loading values from xml file

		TiXmlHandle hDoc(&doc);
		TiXmlElement* pElem;
		TiXmlHandle hRoot(0);

		// block: name
		{
			pElem = hDoc.FirstChildElement().Element();
			// should always have a valid root but handle gracefully if it does
			if (!pElem) return(1);
			m_name = pElem->Value();

			// save this for later
			hRoot = TiXmlHandle(pElem);
		}

		// block: options
		{
			m_options.clear(); // trash existing list

			TiXmlElement* pWindowNode = hRoot.FirstChild("Options").Element();
			for (pWindowNode; pWindowNode; pWindowNode = pWindowNode->NextSiblingElement())
			{
				Options w;
				pWindowNode->QueryIntAttribute("SimulationTime", &w.simulationtime); // If this fails, original value is left as-is

				const char* pName = pWindowNode->Attribute("Optimization");
				if (pName) w.optimization = pName;

				pWindowNode->QueryIntAttribute("IterationsNumber", &w.iterationnumber); // If this fails, original value is left as-is

				pName = pWindowNode->Attribute("Output");
				if (pName) w.output = pName;

				m_options.push_back(w);
			}
		}

		// block: wbv
		{
			m_wbv.clear(); // trash existing list

			TiXmlElement* pWindowNode = hRoot.FirstChild("WholeBodyVibration").Element();
			for (pWindowNode; pWindowNode; pWindowNode = pWindowNode->NextSiblingElement())
			{
				WBV w;
				const char* pName = pWindowNode->Attribute("Active");
				if (pName) w.active = pName;
				pName = pWindowNode->Attribute("Direction");
				if (pName) w.direction = pName;

				pWindowNode->QueryFloatAttribute("Amplitude", &w.amplitude); // If this fails, original value is left as-is
				pWindowNode->QueryFloatAttribute("Frequency", &w.frequency); // If this fails, original value is left as-is


				m_wbv.push_back(w);
			}
		}
		return flag_error_xml;
	}
	
};
#endif // _PARAMS_H_