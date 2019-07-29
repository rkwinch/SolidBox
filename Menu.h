#pragma once

#include <string>
#include <iomanip>
#include <vector>

class SolidBox;
class Sphere;
template<class T, class M>
class Shape;
template<class T>
class RectPlane;
template<class T>
class CurvedSurface;

class Menu {

	friend class Utility;

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
	void PrintCubeDebugInfo(std::vector<std::shared_ptr<Shape<SolidBox, RectPlane<SolidBox>>>>::iterator cubeVecItr);
	void PrintCubeInfo(std::vector<std::shared_ptr<Shape<SolidBox, RectPlane<SolidBox>>>>::iterator cubeVecItr);
	void PrintChannelInfo(std::vector<std::shared_ptr<Shape<SolidBox, RectPlane<SolidBox>>>>::iterator cubeVecItr);
	void PrintPlanesInfo(std::vector<std::shared_ptr<Shape<SolidBox, RectPlane<SolidBox>>>>::iterator cubeVecItr);
	void LoadASolidBox();
	void RetrieveInitialParams(int &solidBoxNameIDCntr, int &connChannelNmIDCntr, int &sqPlnNmIDCntr, int &vecSize, std::vector<std::string>::iterator &itr);
};

