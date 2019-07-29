#include <iostream>
#include "Solidbox.h"
#include "SquarePlane.h"
#include "Utility.h"
#include <iomanip>

int main()
{
	Utility::Run();
	//int a, b, c, d = 1;
	//bool e = false;

	//CFile solidBoxFile;
	//solidBoxFile.Open(_T("test.txt"), CFile::modeCreate | CFile::modeWrite /*| CFile::modeNoTruncate*/);
	//CArchive archive(&solidBoxFile, CArchive::store);
	//int a = 1; int b = 1; int c = 1; int d = 1;
	//bool e = false;
	//archive << a << e << b << c << d;
	//archive.Close();
	//solidBoxFile.Close();

	//int aa = 0; int bb = 0; int cc = 0; int dd = 0;
	//bool ee = true;

	//CFile file;
	//file.Open(_T("test.txt"), CFile::modeRead | CFile::typeBinary);
	//CArchive ar(&file, CArchive::load);
	//try
	//{
	//	ar >> aa >> ee >> bb >> cc >> dd;
	//}
	//catch(std::exception e)
	//{
	//	std::cout << e.what() << std::endl;
	//}
	//ar.Close();
	//file.Close();
	//std::cout << "a,e,b,c,d" << aa << ee << bb << cc << dd << std::endl;
	system("pause");
	return 0;
}
