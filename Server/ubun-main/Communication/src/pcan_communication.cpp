#include<Windows.h>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include<map>
#include "pcan_communication.h"
#include "canMessage.h"

canMessage Communication::notify(string Br, string Ch, DWORD Id, vector<BYTE> Da, int Dl) {
	cantp_handle channelValue;
	cantp_baudrate baudrateValue;

	map<string, cantp_baudrate> baudrateMap;
	baudrateMap = {
	  {"1M", PCANTP_BAUDRATE_1M},
	  {"800K", PCANTP_BAUDRATE_800K},
	  {"500K", PCANTP_BAUDRATE_500K},
	  {"250K", PCANTP_BAUDRATE_250K},
	  {"125K", PCANTP_BAUDRATE_125K},
	  {"100K", PCANTP_BAUDRATE_100K},
	  {"95K", PCANTP_BAUDRATE_95K},
	  {"83K", PCANTP_BAUDRATE_83K},
	  {"50K", PCANTP_BAUDRATE_50K},
	  {"47K", PCANTP_BAUDRATE_47K},
	  {"33K", PCANTP_BAUDRATE_33K},
	  {"22K", PCANTP_BAUDRATE_20K},
	  {"10K", PCANTP_BAUDRATE_10K},
	  {"5K", PCANTP_BAUDRATE_5K}
	};


	if (baudrateMap.find(Br) != baudrateMap.end()) {
		cout << "Yes\n";
		baudrateValue = baudrateMap[Br];
		cout << baudrateValue << ": Final" << endl;
	}
	else {
		std::cout << "La valeur saisie pour le Baud rate n'est pas valide." << std::endl;
	}

	std::map<string, cantp_handle> channelMap;
	channelMap = {
		   {"USBBUS1", PCANTP_HANDLE_USBBUS1},
		   {"USBBUS2", PCANTP_HANDLE_USBBUS2},
		   {"USBBUS3", PCANTP_HANDLE_USBBUS3},
		   {"USBBUS4", PCANTP_HANDLE_USBBUS4},
		   {"USBBUS5", PCANTP_HANDLE_USBBUS5},
		   {"USBBUS6", PCANTP_HANDLE_USBBUS6},
		   {"USBBUS7", PCANTP_HANDLE_USBBUS7},
		   {"USBBUS8", PCANTP_HANDLE_USBBUS8},
		   {"USBBUS9", PCANTP_HANDLE_USBBUS9},
		   {"USBBUS10", PCANTP_HANDLE_USBBUS10},
		   {"USBBUS11", PCANTP_HANDLE_USBBUS11},
		   {"USBBUS12", PCANTP_HANDLE_USBBUS12},
		   {"USBBUS13", PCANTP_HANDLE_USBBUS13},
		   {"USBBUS14", PCANTP_HANDLE_USBBUS14},
		   {"USBBUS15", PCANTP_HANDLE_USBBUS15},
		   {"USBBUS16", PCANTP_HANDLE_USBBUS16}
	};

	if (channelMap.find(Ch) != channelMap.end()) {
		// R?cup?rer la valeur associ?e ? la cl? dans le tableau associatif
		channelValue = channelMap[Ch];
	}
	else {
		std::cout << "La valeur saisie pour le channal n'est pas valide." << std::endl;
	}
	// Initialize structs 
	transmitter_handle = PCANTP_HANDLE_USBBUS1; // TODO: modify the value according to your available PCAN devices.
	receiver_handle = PCANTP_HANDLE_USBBUS1; // TODO: modify the value according to your available PCAN devices.
	memset(&rx_msg, 0, sizeof(cantp_msg));
	memset(&tx_msg, 0, sizeof(cantp_msg));

	// Print version informations
	CANTP_GetValue_2016(PCANTP_HANDLE_NONEBUS, PCANTP_PARAMETER_API_VERSION, buffer, 500);
	printf("PCAN-ISO-TP API Version: %s\n", buffer);
	printf("\n\n");
	printf("WARNING: this sample is not suited for segmented communication.\n");
	printf("         Data must be contained in a Single ISO-TP frame only.\n");
	printf("         Please check sample \"06_isotp_segmented_read_write\" for standard segmented communications.\n");
	printf("\n\n");
	Sleep(2000);

	// Initialize channels: CAN1.0 - 250Kbit/s
	res = CANTP_Initialize_2016(channelValue, baudrateValue, (cantp_hwtype)0, 0, 0);
	printf("Initialize transmitter: %s\n", STATUS_OK_KO(res));
	res = CANTP_Initialize_2016(receiver_handle, baudrateValue, (cantp_hwtype)0, 0, 0);
	printf("Initialize receiver: %s\n", STATUS_OK_KO(res));

	// Create a receive event on receiver
	receive_event = CreateEvent(NULL, FALSE, FALSE, NULL);
	res = CANTP_SetValue_2016(channelValue, PCANTP_PARAMETER_RECEIVE_EVENT, &receive_event, sizeof(receive_event));
	printf("Create a receive event on receiver: %s\n", STATUS_OK_KO(res));

	// Change STmin value to 600us
	STmin = STMIN_600US;
	res = CANTP_SetValue_2016(transmitter_handle, PCANTP_PARAMETER_SEPARATION_TIME, &STmin, sizeof(STmin));
	printf("Set STMIN = 600us on transmitter: %s\n", STATUS_OK_KO(res));
	res = CANTP_SetValue_2016(receiver_handle, PCANTP_PARAMETER_SEPARATION_TIME, &STmin, sizeof(STmin));
	printf("Set STMIN = 600us on receiver: %s\n", STATUS_OK_KO(res));

	// Allocate tx message
	res = CANTP_MsgDataAlloc_2016(&tx_msg, PCANTP_MSGTYPE_CAN);
	printf("Allocate tx CAN message: %s\n", STATUS_OK_KO(res));



	// Allocate rx message
	res = CANTP_MsgDataAlloc_2016(&rx_msg, PCANTP_MSGTYPE_NONE);
	printf("Allocate rx message: %s\n", STATUS_OK_KO(res));

	// Create a simple physical mapping: 
	can_id = Id;
	mapping.can_id = can_id;
	mapping.can_id_flow_ctrl = 0x18DAF960;
	mapping.can_msgtype = PCANTP_CAN_MSGTYPE_EXTENDED;
	mapping.netaddrinfo.extension_addr = 0x00;
	mapping.netaddrinfo.format = PCANTP_ISOTP_FORMAT_NORMAL;
	mapping.netaddrinfo.msgtype = PCANTP_ISOTP_MSGTYPE_DIAGNOSTIC;
	mapping.netaddrinfo.source_addr = 0xF9;
	mapping.netaddrinfo.target_addr = 0x60;
	mapping.netaddrinfo.target_type = PCANTP_ISOTP_ADDRESSING_PHYSICAL;
	// Initialize Tx message 

	// Add mapping on channels
	res = CANTP_AddMapping_2016(transmitter_handle, &mapping);
	printf("Add a simple mapping on transmitter: %s\n", STATUS_OK_KO(res));
	res = CANTP_AddMapping_2016(receiver_handle, &mapping);
	printf("Add a simple mapping on receiver: %s\n", STATUS_OK_KO(res));

	res = CANTP_MsgDataInit_2016(&tx_msg, mapping.can_id, PCANTP_CAN_MSGTYPE_EXTENDED, 8, NULL, NULL);
	for (int i = 0; i < 8; i++) {
		tx_msg.msgdata.can->data[i] = Da[i];
		//Da.push_back(tx_msg.msgdata.can->data[i]);
		cout << Da[0];
	}

	printf("Initialize tx message: %s\n", STATUS_OK_KO(res));

	// Write  message
	res = CANTP_Write_2016(channelValue, &tx_msg);
	printf("Write message on transmitter: %s\n", STATUS_OK_KO(res));

	// Wait a receive event on receiver
	wait_result = WaitForSingleObject(receive_event, 100);
	printf("Wait a message on receiver: %s\n", OK_KO(wait_result == WAIT_OBJECT_0));

	// If we receive something on receiver, read the message
	if (wait_result == WAIT_OBJECT_0) {
		res = CANTP_Read_2016(channelValue, &rx_msg, NULL, PCANTP_MSGTYPE_NONE);
		printf("Read message on receiver: %s\n", STATUS_OK_KO(res));
		//char* data = (char*)rx_msg.msgdata.any->data;
		printf("%x\n", rx_msg.msgdata.can->data[0]);
	}
	if (wait_result == WAIT_OBJECT_0) {
		res = CANTP_Read_2016(channelValue, &rx_msg, NULL, PCANTP_MSGTYPE_NONE);
		printf("Read message on receiver: %s\n", STATUS_OK_KO(res));
		//char* data = (char*)rx_msg.msgdata.any->data;
		printf("%x\n", rx_msg.msgdata.can->length);
		printf("%x\n", rx_msg.msgdata.can->data[0]);
		printf("%x\n", rx_msg.msgdata.can->data[1]);
		message.data.push_back(rx_msg.msgdata.can->length);
		for (int i = 0; i < rx_msg.msgdata.can->length; i++) {
			message.data.push_back(rx_msg.msgdata.can->data[i]);
			cout << rx_msg.msgdata.can->data[i] << endl;
		}
		message.id = 0x18DAF960;
		message.dlc = /*8*/rx_msg.msgdata.can->length;
	}

	// Free messages space
	res = CANTP_MsgDataFree_2016(&tx_msg);
	printf("Free tx message: %s\n", STATUS_OK_KO(res));
	res = CANTP_MsgDataFree_2016(&rx_msg);
	printf("Free rx message: %s\n", STATUS_OK_KO(res));

	// Close receive event
	res = CANTP_SetValue_2016(channelValue, PCANTP_PARAMETER_RECEIVE_EVENT, &null_handle, sizeof(HANDLE));
	printf("Stop receive event on receiver: %s\n", STATUS_OK_KO(res));
	res_b = CloseHandle(receive_event);
	printf("Close receive event: %s\n", OK_KO(res_b));

	// Uninitialize channels
	res = CANTP_Uninitialize_2016(channelValue);
	printf("Uninitialize transmitter: %s\n", STATUS_OK_KO(res));
	res = CANTP_Uninitialize_2016(channelValue);
	printf("Uninitialize receiver: %s\n", STATUS_OK_KO(res));
	return message;
}