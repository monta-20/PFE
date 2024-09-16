#pragma once
#include <WinSock2.h>
#include <windows.h>
#include <string>
#include <vector>
#include <typeinfo>
#include <sstream>
#include "PCAN-ISO-TP_2016.h"
#include "Frame.h"
#include "TotalFrame.h"
using namespace std;

#define STMIN_1500US 0xFF
#define OK_KO(test) (test)?"OK":"KO"
#define STATUS_OK_KO(test) OK_KO(CANTP_StatusIsOk_2016(test, PCANTP_STATUS_OK, false))
class Simulation {
	cantp_status res;
	char buffer[500];
	cantp_msg tx_msg;
	cantp_msg rx_msg;
	int can_id;
	uint32_t STmin;
	HANDLE receive_event;
	int wait_result;
	BOOL res_b;
	cantp_mapping mapping;
	uint64_t null_handle = 0;
	cantp_handle transmitter_handle;
	cantp_handle receiver_handle;
    char strMsg[256];
public:
    Frame communication(int, int, int, int, int, int, int, int);
	TotalFrame communicationDL(vector<BYTE> DataAll);
};
