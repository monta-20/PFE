#include <iostream>

using namespace std;

#ifndef SERVICE_H
#define SERVICE_H

#include "DID.h"
#include "SBF.h"

class Service {

public:
	string name;
	string value;
	string desc;
	vector<DID> dids;
	vector<SBF> sbfs;

	string byteToHex(unsigned char byte);
	//Methods
	vector<Service> SendDescription(std::string, std::string, DWORD, std::vector<BYTE>, int);
	vector<Service> RecieveDescription(std::string, std::string, DWORD, std::vector<BYTE>, int);



};

#endif