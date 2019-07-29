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

	static Menu* GetInstance();
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

	static Menu* m_instance; // used to help enforce only one instance of Menu

	Menu(); // private because want to control when it is instantiated for Singleton purposes 
	~Menu();
	void WelcomeAndOptions();
	void PrintShapeDebugInfo(Shape* shape);
	void PrintShapeInfo(Shape* shape);
	void PrintChannelInfo(Shape* shape);
	void PrintPlanesInfo(Shape* shape);
	void LoadFile();
	void RetrieveInitialParams(int &solidBoxNameIDCntr, int &sphereNameIDCntr, int &connChannelNmIDCntr, 
		                       int &rectPlnNmIDCntr, int &curvedSurfNmIDCntr, int &cubeVecSize, int &sphereVecSize, 
							   std::vector<std::vector<std::string>>::iterator &itr);
	void PrintShapeDebugNames();
};
