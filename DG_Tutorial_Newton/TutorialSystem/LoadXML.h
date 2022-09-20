#pragma once
#include "tinyxml.h"
#include "pch.h"

class ModelParams
{
public:
	std::vector<float> InitialCondition, StanceLead, ForceFeedback, LengthFeedback, Coronal_lead, Coronal, Arm;
	std::vector<float> IC_min, IC_max, SL_min, SL_max, FF_min, FF_max, LF_min, LF_max, CL_min, CL_max, C_min, C_max, A_min, A_max;
	void save(const char* pFilename) {};

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
			InitialCondition.clear(); // trash existing list
			StanceLead.clear();
			ForceFeedback.clear();
			LengthFeedback.clear();
			Coronal_lead.clear();
			Coronal.clear();
			Arm.clear();

			char IC_arr[7][6] = { {"trunk"}, {"alfaR"}, {"alfaL"}, {"beta"}, {"gamma" }, { "head" }, { "vel" } }; // names of each attribute of Initial Condition in XML file.
			IC_min = { 0, 0, 0, 0, 0, 0, 0 }; // minima for initial condition params
			IC_max = { 20*dDegreeToRad, 20 * dDegreeToRad, 20 * dDegreeToRad, 20 * dDegreeToRad, 20 * dDegreeToRad, 20 * dDegreeToRad, 3 }; // maxima for initial condition params
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
			A_max = { 1, 90*dDegreeToRad };

			TiXmlElement* pWindowNode = hRoot.FirstChild("Set").FirstChild().Element();
			
			// loop all sets of params
			for (pWindowNode; pWindowNode != NULL; pWindowNode = pWindowNode->NextSiblingElement())
			{
				if (strcmp(pWindowNode->Value(), "InitialCondition") == 0)
				{
					// loop on all params of each set. TO DO: use just one fot loop and a function
					for (int ii = 0; ii < 7; ++ii)
					{
						float value = -1;// initialization
						pWindowNode->QueryFloatAttribute(&IC_arr[ii][0], &value);
						InitialCondition.push_back(value);// NORMALIZATION BEFORE OPTIMIZATION
						if (value == -1)
							flag_error_xml = 1;
					}
				}
				else if (strcmp(pWindowNode->Value(), "StanceLead") == 0)
				{
					for (int ii = 0; ii < 9; ++ii)
					{
						float value = -1;
						pWindowNode->QueryFloatAttribute(&SL_arr[ii][0], &value);
						StanceLead.push_back(value);
						if (value == -1)
							flag_error_xml = 1;
					}
				}
				else if (strcmp(pWindowNode->Value(), "ForceFeedback") == 0)
				{
					for (int ii = 0; ii < 6; ++ii)
					{
						float value = -1;
						pWindowNode->QueryFloatAttribute(&FF_arr[ii][0], &value);
						ForceFeedback.push_back(value);
						if (value == -1)
							flag_error_xml = 1;
					}
				}
				else if (strcmp(pWindowNode->Value(), "LengthFeedback") == 0)
				{
					for (int ii = 0; ii < 6; ++ii)
					{
						float value = -1;
						pWindowNode->QueryFloatAttribute(&LF_arr[ii][0], &value);
						LengthFeedback.push_back(value);
						if (value == -1)
							flag_error_xml = 1;
					}
				}
				else if (strcmp(pWindowNode->Value(), "Coronal_lead") == 0)
				{
					for (int ii = 0; ii < 4; ++ii)
					{
						float value = -1;
						pWindowNode->QueryFloatAttribute(&CL_arr[ii][0], &value);
						Coronal_lead.push_back(value);
						if (value == -1)
							flag_error_xml = 1;
					}
				}
				else if (strcmp(pWindowNode->Value(), "Coronal") == 0)
				{
					for (int ii = 0; ii < 6; ++ii)
					{
						float value = -1;
						pWindowNode->QueryFloatAttribute(&C_arr[ii][0], &value);
						Coronal.push_back(value);
						if (value == -1)
							flag_error_xml = 1;
					}
				}
				else if (strcmp(pWindowNode->Value(), "Arm") == 0)
				{
					for (int ii = 0; ii < 2; ++ii)
					{
						float value = -1;
						pWindowNode->QueryFloatAttribute(&A_arr[ii][0], &value);
						Arm.push_back(value);
						if (value == -1)
							flag_error_xml = 1;
					}
				}
			}
		}
		return(flag_error_xml);
	};

	ModelParams()
		: InitialCondition(0), StanceLead(0), ForceFeedback(0), LengthFeedback(0), Coronal_lead(0), Coronal(0), Arm(0)
	{
	}

	//remove scaling of params, X_SCALED * (max - min) + min
	void RemoveScaling()
	{
		for (int ii = 0; ii < 7; ++ii)
			InitialCondition[ii] = InitialCondition[ii]*(IC_max[ii]-IC_min[ii]) + IC_min[ii];

		for (int ii = 0; ii < 9; ++ii)
			StanceLead[ii] = StanceLead[ii] * (SL_max[ii] - SL_min[ii]) + SL_min[ii];

		for (int ii = 0; ii < 6; ++ii)
			ForceFeedback[ii] = ForceFeedback[ii] * (FF_max[ii] - FF_min[ii]) + FF_min[ii];

		for (int ii = 0; ii < 6; ++ii)
			LengthFeedback[ii] = LengthFeedback[ii] * (LF_max[ii] - LF_min[ii]) + LF_min[ii];

		for (int ii = 0; ii < 4; ++ii)
			Coronal_lead[ii] = Coronal_lead[ii] * (CL_max[ii] - CL_min[ii]) + CL_min[ii];

		for (int ii = 0; ii < 6; ++ii)
			Coronal[ii] = Coronal[ii] * (C_max[ii] - C_min[ii]) + C_min[ii];

		for (int ii = 0; ii < 2; ++ii)
			Arm[ii] = Arm[ii] * (A_max[ii] - A_min[ii]) + A_min[ii];
	}

	//scaling of params, (X - min) / (max - min) 
	void ScaleParameters()
	{
		for (int ii = 0; ii < 7; ++ii)
			InitialCondition[ii] = (InitialCondition[ii] - IC_min[ii]) / (IC_max[ii] - IC_min[ii]);// NORMALIZATION BEFORE OPTIMIZATION

		for (int ii = 0; ii < 9; ++ii)
			StanceLead[ii] = (StanceLead[ii] - SL_min[ii]) / (SL_max[ii] - SL_min[ii]);// NORMALIZATION BEFORE OPTIMIZATION

		for (int ii = 0; ii < 6; ++ii)
			ForceFeedback[ii] = (ForceFeedback[ii] - FF_min[ii]) / (FF_max[ii] - FF_min[ii]);// NORMALIZATION BEFORE OPTIMIZATION

		for (int ii = 0; ii < 6; ++ii)
			LengthFeedback[ii] = (LengthFeedback[ii] - LF_min[ii]) / (LF_max[ii] - LF_min[ii]);// NORMALIZATION BEFORE OPTIMIZATION

		for (int ii = 0; ii < 4; ++ii)
			Coronal_lead[ii] = (Coronal_lead[ii] - CL_min[ii]) / (CL_max[ii] - CL_min[ii]);// NORMALIZATION BEFORE OPTIMIZATION

		for (int ii = 0; ii < 6; ++ii)
			Coronal[ii] = (Coronal[ii] - C_min[ii]) / (C_max[ii] - C_min[ii]);// NORMALIZATION BEFORE OPTIMIZATION

		for (int ii = 0; ii < 2; ++ii)
			Arm[ii] = (Arm[ii] - A_min[ii]) / (A_max[ii] - A_min[ii]);// NORMALIZATION BEFORE OPTIMIZATION
	}

	//ModelParams(std::vector<float> IC, std::vector<float> SL, std::vector<float> FF, std::vector<float> LF, std::vector<float> CL, std::vector<float> C, std::vector<float> A)
	//{
	//	this->InitialCondition = IC;
	//	this->StanceLead = SL;
	//	this->ForceFeedback = FF;
	//	this->LengthFeedback = LF;
	//	this->Coronal_lead = CL;
	//	this->Coronal = C;
	//	this->Arm = A;
	//}
};