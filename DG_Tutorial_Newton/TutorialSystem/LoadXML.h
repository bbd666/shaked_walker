#pragma once
#include "tinyxml.h"
#include "pch.h"

class ModelParams
{
public:
	ModelParams() {}
	~ModelParams() 
	{ 
		// somethinbg to delete?
	}

	std::vector<double> ParamList;
	std::vector<double> IC_min, IC_max, SL_min, SL_max, FF_min, FF_max, LF_min, LF_max, CL_min, CL_max, C_min, C_max, A_min, A_max, min_v, max_v;
	vector<double> parameters_scaled, params_not_scaled;

	double* params_pointer() {
		return &ParamList[0];
	}

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
			IC_max = { 20.0*dDegreeToRad, 20.0*dDegreeToRad, 20.0*dDegreeToRad, 20.0*dDegreeToRad, 20.0*dDegreeToRad, 20.0*dDegreeToRad, 3.0 }; // maxima for initial condition params
			char SL_arr[9][6] = { {"hamP"}, {"hamA"}, {"hamD"}, {"gluP"}, {"gluA" }, { "gluD" }, { "hflP" }, {"hflA"}, {"hflD"}};
			SL_min = { 1.78, 0.97*dDegreeToRad, 0.1, 1.78, 0.97 * dDegreeToRad, 0.1,1.78, 0.97 * dDegreeToRad, 0.1 };
			SL_max = { 22, 6.3 * dDegreeToRad, 0.5, 22, 6.3 * dDegreeToRad, 0.5, 22, 6.3 * dDegreeToRad, 0.5};
			char FF_arr[6][6] = { {"glu"}, {"ham"}, {"vas"}, {"sol"}, {"gas" }, { "tasol" }};
			FF_min = { 0, 0, 0.82, 0.97, 0, 0 };
			FF_max = { 0.52, 0.67, 13.5, 2.17, 10, 10};
			char LF_arr[6][6] = { {"hflG"}, {"hamG"}, {"taG"}, {"hflH"}, {"hamH" }, { "taH" }};
			LF_min = { 0.17, 0, 0.55, 0, 0.6, 0.59 };
			LF_max = { 5, 100, 3.2, 0.67, 10, 0.8 };
			char CL_arr[4][7] = { {"trunkP"}, {"trunkV"}, {"footP"}, {"footV"}};
			CL_min = { 100, 10, 10, 1 };
			CL_max = { 3000, 300, 300, 30 };
			char C_arr[6][8] = { {"trunkP"}, {"trunkV"}, {"footP"}, {"footV"}, {"TtrunkP"}, {"TtrunkV"}};
			C_min = { 100, 0, 1, 0, 50, 0 };
			C_max = { 200, 30, 100, 10, 500, 50};
			char A_arr[2][6] = { {"alpha"}, {"elbow"}};
			A_min = { 0, 0 };
			A_max = { 1, 90.0*dDegreeToRad };

			min_v.insert(min_v.end(), IC_min.begin(), IC_min.end());
			min_v.insert(min_v.end(), SL_min.begin(), SL_min.end());
			min_v.insert(min_v.end(), FF_min.begin(), FF_min.end());
			min_v.insert(min_v.end(), LF_min.begin(), LF_min.end());
			min_v.insert(min_v.end(), CL_min.begin(), CL_min.end());
			min_v.insert(min_v.end(), C_min.begin(), C_min.end());
			min_v.insert(min_v.end(), A_min.begin(), A_min.end());


			max_v.insert(max_v.end(), IC_max.begin(), IC_max.end());
			max_v.insert(max_v.end(), SL_max.begin(), SL_max.end());
			max_v.insert(max_v.end(), FF_max.begin(), FF_max.end());
			max_v.insert(max_v.end(), LF_max.begin(), LF_max.end());
			max_v.insert(max_v.end(), CL_max.begin(), CL_max.end());
			max_v.insert(max_v.end(), C_max.begin(), C_max.end());
			max_v.insert(max_v.end(), A_max.begin(), A_max.end());

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
			}
		}
		return(flag_error_xml);
	};

	//remove scaling of params, X_SCALED * (max - min) + min
	double* RemoveScaling(double const* scaled_params)
	{
		params_not_scaled.clear();
		for(int ii = 0; ii < 40; ii++)
			params_not_scaled.push_back(scaled_params[ii] * (max_v[ii] - min_v[ii]) + min_v[ii]);
		return &params_not_scaled[0];
	}

	//scaling of params, (X - min) / (max - min) 
	double* ScaleParameters()
	{
		parameters_scaled.clear();

		for (int ii = 0; ii < ParamList.size(); ii++)
			parameters_scaled.push_back((ParamList[ii] - min_v[ii]) / (max_v[ii] - min_v[ii]));// NORMALIZATION BEFORE OPTIMIZATION
		return &parameters_scaled[0];
	}

	// writes a XML file with optimized params
	void write_optimized_params_doc(double * opt_params)
	{
		TiXmlDocument doc;
		TiXmlElement* msg;
		TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
		doc.LinkEndChild(decl);

		TiXmlElement* root = new TiXmlElement("Optimized_parameters");
		doc.LinkEndChild(root);

		TiXmlComment* comment = new TiXmlComment();
		comment->SetValue(" Bestever parameters of CMAES ");
		root->LinkEndChild(comment);

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
		CL = new TiXmlElement("CoronalLead");
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

		//dump_to_stdout(&doc);
		doc.SaveFile("Results_Optimization.xml");
	}
};