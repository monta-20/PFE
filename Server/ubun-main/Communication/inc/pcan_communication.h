#pragma once
#include <string>
#include <vector>
#include <typeinfo>
#include <sstream>
#include "canMessage.h"
#include "PCAN-ISO-TP_2016.h"

#define STMIN_600US 0xF6
#define OK_KO(test) (test)?"OK":"KO"
#define STATUS_OK_KO(test) OK_KO(CANTP_StatusIsOk_2016(test, PCANTP_STATUS_OK, false))

class Communication {
	cantp_status res;
	char buffer[500];
	cantp_msg tx_msg;
	cantp_msg rx_msg;
	int can_id;
	uint32_t STmin;
	cantp_mapping mapping;
	HANDLE receive_event;
	int wait_result;
	BOOL res_b;
	uint64_t null_handle = 0;
	cantp_handle transmitter_handle;
	cantp_handle receiver_handle;
	canMessage message;

public:

	canMessage notify(std::string, std::string, DWORD, std::vector<BYTE>, int);
};