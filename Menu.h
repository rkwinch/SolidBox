#pragma once

#include <string>
#include <iomanip>
#include <vector>
#include <fstream>

class SolidBox;
class Sphere;
class Shape;

class Menu {

	//friend class Utility;

public:
	
	bool GetIsSpeechFlag();
	static Menu* GetInstance(const bool &speechFlag = false);
	static void Run();
	void CreateShape();
	void DeleteExistingSolid();
	void ShowSolidsInMemory();
	void CopyExistingSolid();
	void MoveASolid();
	void DebugSolidBox();
	int SaveAllObjects();
	void LoadAllObjects();
	void SaveShapes(std::ofstream &outFile);

private:

	bool isSpeech = false;

	Menu(int isSpeech = 0); // private because want to control when it is instantiated for Singleton purposes 
	~Menu();
	void WelcomeAndOptions();
	void LoadFile();
	void RetrieveInitialParams(int &solidBoxNameIDCntr, int &sphereNameIDCntr, int &rectPrismNameIDCntr, int &connChannelNmIDCntr, 
		                       int &rectPlnNmIDCntr, int &curvedSurfNmIDCntr, int &cubeVecSize, int &sphereVecSize, 
							   std::vector<std::vector<std::string>>::iterator &itr);
	std::string PrintShapeDebugNames();
};