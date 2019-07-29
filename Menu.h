#pragma once

#include <string>
#include <iomanip>
#include <vector>

class SolidBox;
class Sphere;
class Shape;

class Menu {

	//friend class Utility;

public:
	static const int PRINTING_WIDTH;
	
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

private:

	bool isSpeech = false;

	Menu(bool isSpeech = false); // private because want to control when it is instantiated for Singleton purposes 
	~Menu();
	void WelcomeAndOptions();
	void PrintShapeDebugInfo(Shape* shape);
	void PrintShapeInfo(Shape* shape);
	void PrintChannelInfo(Shape* shape);
	void PrintPlanesInfo(Shape* shape);
	void LoadFile();
	void RetrieveInitialParams(int &solidBoxNameIDCntr, int &sphereNameIDCntr, int &rectPrismNameIDCntr, int &connChannelNmIDCntr, 
		                       int &rectPlnNmIDCntr, int &curvedSurfNmIDCntr, int &cubeVecSize, int &sphereVecSize, 
							   std::vector<std::vector<std::string>>::iterator &itr);
	void PrintShapeDebugNames();
};