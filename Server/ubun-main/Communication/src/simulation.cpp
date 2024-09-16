#include "simulation.h"
#include "Frame.h"
#include "TotalFrame.h"
#include<stdlib.h>

Frame Simulation::communication(int D0, int D1, int D2, int D3, int D4,int D5, int D6, int D7)
{
	// Initialize structs 
	Frame fr;
	transmitter_handle = PCANTP_HANDLE_USBBUS1; // TODO: modify the value according to your available PCAN devices.
	receiver_handle = PCANTP_HANDLE_USBBUS1; // TODO: modify the value according to your available PCAN devices.
	memset(&tx_msg, 0, sizeof(cantp_msg));
	memset(&rx_msg, 0, sizeof(cantp_msg));
	//memset(&mapping, 0, sizeof(cantp_mapping));

	// Print version informations
	CANTP_GetValue_2016(PCANTP_HANDLE_NONEBUS, PCANTP_PARAMETER_API_VERSION, buffer, 500);
	printf("PCAN-ISO-TP API Version: %s\n", buffer);

	printf("\n\n");
	printf("WARNING: this sample is not suited for segmented communication.\n");
	printf("         Data must be contained in a Single ISO-TP frame only.\n");
	printf("         Please check sample \"06_isotp_segmented_read_write\" for standard segmented communications.\n");
	printf("\n\n");


	// Initialize channels: CAN2.0 - 500Kbit/s
	res = CANTP_Initialize_2016(transmitter_handle, PCANTP_BAUDRATE_250K, (cantp_hwtype)0, 0, 0);
	printf("Initialize transmitter: %s\n", STATUS_OK_KO(res));
	res = CANTP_Initialize_2016(receiver_handle, PCANTP_BAUDRATE_250K, (cantp_hwtype)0, 0, 0);
	printf("Initialize receiver: %s\n", STATUS_OK_KO(res));

	// Create a receive event on receiver
	receive_event = CreateEvent(NULL, FALSE, FALSE, NULL);
	res = CANTP_SetValue_2016(receiver_handle, PCANTP_PARAMETER_RECEIVE_EVENT, &receive_event, sizeof(receive_event));
	printf("Create a receive event receiver: %s\n", STATUS_OK_KO(res));

	// Change STmin value to 600us
	STmin = STMIN_1500US;
	res = CANTP_SetValue_2016(transmitter_handle, PCANTP_PARAMETER_SEPARATION_TIME, &STmin, sizeof(STmin));
	printf("Set STMIN = 1500us on transmitter: %s\n", STATUS_OK_KO(res));
	res = CANTP_SetValue_2016(receiver_handle, PCANTP_PARAMETER_SEPARATION_TIME, &STmin, sizeof(STmin));
	printf("Set STMIN = 1500us on receiver: %s\n", STATUS_OK_KO(res));

	// Allocate tx message
	res = CANTP_MsgDataAlloc_2016(&tx_msg, PCANTP_MSGTYPE_CAN);
	printf("Allocate tx CAN message: %s\n", STATUS_OK_KO(res));

	// Allocate rx message
	res = CANTP_MsgDataAlloc_2016(&rx_msg, PCANTP_MSGTYPE_NONE);
	printf("Allocate rx message: %s\n", STATUS_OK_KO(res));


	// Create a simple physical mapping
	can_id = 0x18DA60F9;
	mapping.can_id = can_id;
	mapping.can_id_flow_ctrl = 0x18DAF960;
	mapping.can_msgtype = PCANTP_CAN_MSGTYPE_EXTENDED;
	mapping.netaddrinfo.extension_addr = 0x00;
	mapping.netaddrinfo.format = PCANTP_ISOTP_FORMAT_NORMAL;
	mapping.netaddrinfo.msgtype = PCANTP_ISOTP_MSGTYPE_DIAGNOSTIC;
	mapping.netaddrinfo.source_addr = 0xF9;
	mapping.netaddrinfo.target_addr = 0x60;
	mapping.netaddrinfo.target_type = PCANTP_ISOTP_ADDRESSING_PHYSICAL;

	// Add mapping on channels
	res = CANTP_AddMapping_2016(transmitter_handle, &mapping);
	printf("Add a simple mapping on transmitter: %s\n", STATUS_OK_KO(res));
	res = CANTP_AddMapping_2016(receiver_handle, &mapping);
	printf("Add a simple mapping on receiver: %s\n", STATUS_OK_KO(res));


	res = CANTP_MsgDataInit_2016(&tx_msg, mapping.can_id, mapping.can_msgtype, 8, NULL, &mapping.netaddrinfo);
	//tx_msg.msgdata.can->data[0] = D0;
	tx_msg.msgdata.can->data[0] = D0;
	tx_msg.msgdata.can->data[1] = D1;
	tx_msg.msgdata.can->data[2] = D2;
	tx_msg.msgdata.can->data[3] = D3;
	tx_msg.msgdata.can->data[4] = D4;
	tx_msg.msgdata.can->data[5] = D5;
	tx_msg.msgdata.can->data[6] = D6;
	tx_msg.msgdata.can->data[7] = D7;
	printf("Initialize tx message: %s\n", STATUS_OK_KO(res));

	// Write "42" message
	res = CANTP_Write_2016(transmitter_handle, &tx_msg);
	printf("Write SF message on transmitter: %s\n", STATUS_OK_KO(res));

	// Read CF frames
	int received_frames = 0;
	while (received_frames < 4) {
		// Wait for the next frame (CF)
		wait_result = WaitForSingleObject(receive_event, 2000); // Increased wait time to 10 seconds
		if (wait_result == WAIT_OBJECT_0) {
			res = CANTP_Read_2016(receiver_handle, &rx_msg, NULL, PCANTP_MSGTYPE_NONE);
			if (CANTP_StatusIsOk_2016(res, PCANTP_STATUS_OK, false)) {
				printf("Read CF message on receiver: %s\n", STATUS_OK_KO(res));
				//Process CF message here (rx_msg contains the CF data)

				printf("Received CF data:\n");
				printf("%02X ", rx_msg.msgdata.can->length);
				if (received_frames == 1 && rx_msg.msgdata.can->length < 9) {
					fr.Da = rx_msg.msgdata.can->length;
					fr.D00 = rx_msg.msgdata.can->data[0];
					fr.D11 = rx_msg.msgdata.can->data[1];
					fr.D22 = rx_msg.msgdata.can->data[2];
					fr.D33 = rx_msg.msgdata.can->data[3];
					fr.D44 = rx_msg.msgdata.can->data[4];
					fr.D55 = rx_msg.msgdata.can->data[5];
					fr.D66 = rx_msg.msgdata.can->data[6];
				}
				if (received_frames == 2) {

					fr.Da = 0x10;
					fr.D00 = rx_msg.msgdata.can->length;
					fr.D11 = rx_msg.msgdata.can->data[0];
					fr.D22 = rx_msg.msgdata.can->data[1];
					fr.D33 = rx_msg.msgdata.can->data[2];
					fr.D44 = rx_msg.msgdata.can->data[3];
					fr.D55 = rx_msg.msgdata.can->data[4];
					fr.D66 = rx_msg.msgdata.can->data[5];
					cout << fr.D66 << endl;
					fr.Db = 0x21;
					fr.D77 = rx_msg.msgdata.can->data[6];
					fr.D88 = rx_msg.msgdata.can->data[7];
					fr.D99 = rx_msg.msgdata.can->data[8];
					fr.D100 = rx_msg.msgdata.can->data[9];
					fr.D110 = rx_msg.msgdata.can->data[10];
					fr.D120 = rx_msg.msgdata.can->data[11];
					fr.D130 = rx_msg.msgdata.can->data[12];
					//cout << fr.D88;
					fr.Dc = 0x22;
					fr.D140 = rx_msg.msgdata.can->data[13];
					fr.D150 = rx_msg.msgdata.can->data[14];
					fr.D160 = rx_msg.msgdata.can->data[15];
					fr.D170 = rx_msg.msgdata.can->data[16];
					fr.D180 = rx_msg.msgdata.can->data[17];
					fr.D190 = rx_msg.msgdata.can->data[18];
					fr.D200 = rx_msg.msgdata.can->data[19];
					for (int i = 0; i < rx_msg.msgdata.can->length; ++i) {
						printf("%02X ", rx_msg.msgdata.can->data[i]);
						fr.DataStore.push_back(rx_msg.msgdata.can->data[i]);
					}
				}
				printf("\n");
				++received_frames;
			}
			else {
				printf("Error reading CF message on receiver: %s\n", STATUS_OK_KO(res));
			}
		}
		else {
			printf("WaitForSingleObject timeout or error: %d\n", wait_result);
			break; // Exit loop if timeout or error occurs
		}
	}

	// Free messages space
	res = CANTP_MsgDataFree_2016(&tx_msg);
	printf("Free tx message: %s\n", STATUS_OK_KO(res));
	res = CANTP_MsgDataFree_2016(&rx_msg);
	printf("Free rx message: %s\n", STATUS_OK_KO(res));

	// Close receive event
	res = CANTP_SetValue_2016(receiver_handle, PCANTP_PARAMETER_RECEIVE_EVENT, &null_handle, sizeof(HANDLE));
	printf("Stop receive event on receiver: %s\n", STATUS_OK_KO(res));
	res_b = CloseHandle(receive_event);
	printf("Close receive event: %s\n", OK_KO(res_b));

	// Uninitialize transmitter and receiver
	res = CANTP_Uninitialize_2016(transmitter_handle);
	printf("Uninitialize transmitter: %s\n", STATUS_OK_KO(res));
	res = CANTP_Uninitialize_2016(receiver_handle);
	printf("Uninitialize receiver: %s\n", STATUS_OK_KO(res));
	return fr;

}

TotalFrame Simulation::communicationDL(vector<BYTE> DataAll) {

	TotalFrame TFr;
	
	transmitter_handle = PCANTP_HANDLE_USBBUS1;
	receiver_handle = PCANTP_HANDLE_USBBUS1;
	memset(&rx_msg, 0, sizeof(cantp_msg));
	memset(&tx_msg, 0, sizeof(cantp_msg));

	// Print version informations
	CANTP_GetValue_2016(PCANTP_HANDLE_NONEBUS, PCANTP_PARAMETER_API_VERSION, buffer, 500);
	printf("PCAN-ISO-TP API Version: %s\n", buffer);
	Sleep(2000);

	// Initialize channels: CAN1.0 - 250Kbit/s
	res = CANTP_Initialize_2016(transmitter_handle, PCANTP_BAUDRATE_250K, (cantp_hwtype)0, 0, 0);
	printf("Initialize transmitter: %s\n", STATUS_OK_KO(res));
	res = CANTP_Initialize_2016(receiver_handle, PCANTP_BAUDRATE_250K, (cantp_hwtype)0, 0, 0);
	printf("Initialize receiver: %s\n", STATUS_OK_KO(res));

	// Create a receive event on receiver
	receive_event = CreateEvent(NULL, FALSE, FALSE, NULL);
	res = CANTP_SetValue_2016(receiver_handle, PCANTP_PARAMETER_RECEIVE_EVENT, &receive_event, sizeof(receive_event));
	printf("Create a receive event on receiver: %s\n", STATUS_OK_KO(res));

	// Change STmin value to 1500us
	STmin = STMIN_1500US;
	res = CANTP_SetValue_2016(transmitter_handle, PCANTP_PARAMETER_SEPARATION_TIME, &STmin, sizeof(STmin));
	printf("Set STMIN = 1500us on transmitter: %s\n", STATUS_OK_KO(res));
	res = CANTP_SetValue_2016(receiver_handle, PCANTP_PARAMETER_SEPARATION_TIME, &STmin, sizeof(STmin));
	printf("Set STMIN = 1500us on receiver: %s\n", STATUS_OK_KO(res));

	// Allocate tx message
	res = CANTP_MsgDataAlloc_2016(&tx_msg, PCANTP_MSGTYPE_CAN);
	printf("Allocate tx CAN message: %s\n", STATUS_OK_KO(res));

	// Allocate rx message
	res = CANTP_MsgDataAlloc_2016(&rx_msg, PCANTP_MSGTYPE_NONE);
	printf("Allocate rx message: %s\n", STATUS_OK_KO(res));

	// Create a simple physical mapping:
	can_id = 0x18DA60F9;
	mapping.can_id = can_id;
	mapping.can_id_flow_ctrl = 0x18DAF960;
	mapping.can_msgtype = PCANTP_CAN_MSGTYPE_EXTENDED;
	mapping.netaddrinfo.extension_addr = 0x00;
	mapping.netaddrinfo.format = PCANTP_ISOTP_FORMAT_NORMAL;
	mapping.netaddrinfo.msgtype = PCANTP_ISOTP_MSGTYPE_DIAGNOSTIC;
	mapping.netaddrinfo.source_addr = 0xF9;
	mapping.netaddrinfo.target_addr = 0x60;
	mapping.netaddrinfo.target_type = PCANTP_ISOTP_ADDRESSING_PHYSICAL;

	// Add mapping on channels
	res = CANTP_AddMapping_2016(transmitter_handle, &mapping);
	printf("Add a simple mapping on transmitter: %s\n", STATUS_OK_KO(res));
	res = CANTP_AddMapping_2016(receiver_handle, &mapping);
	printf("Add a simple mapping on receiver: %s\n", STATUS_OK_KO(res));

	// Initialize Tx message
	res = CANTP_MsgDataInit_2016(&tx_msg, mapping.can_id, PCANTP_CAN_MSGTYPE_EXTENDED, 8, NULL, NULL);
	printf("Initialize tx message: %s\n", STATUS_OK_KO(res));

	for (size_t j = 0; j < 8; j++) {
		tx_msg.msgdata.can->data[j] = DataAll[j];
	}

	// Write FF message
	res = CANTP_Write_2016(transmitter_handle, &tx_msg);
	printf("Write FF message on transmitter: %s\n", STATUS_OK_KO(res));

	// Wait for Flow Control (FC) frame
	wait_result = WaitForSingleObject(receive_event, 20);
	printf("Wait for FC frame on receiver: %s\n", OK_KO(wait_result == WAIT_OBJECT_0));

	if (wait_result == WAIT_OBJECT_0) {
		// Read FC frame
		res = CANTP_Read_2016(receiver_handle, &rx_msg, NULL, PCANTP_MSGTYPE_CAN);
		printf("Read FC frame on receiver: %s\n", STATUS_OK_KO(res));
		printf("FC frame data: ");

		for (int i = 0; i < 8; i++) {
			printf("%02x ", rx_msg.msgdata.can->data[i]);
		}
		printf("\n");

		// Send Consecutive Frames (CF)
		for (int i = 0; i < ((DataAll.size() - 8) / 8) + 1; i++) {
			res = CANTP_MsgDataInit_2016(&tx_msg, mapping.can_id, PCANTP_CAN_MSGTYPE_EXTENDED, 8, NULL, NULL);
			tx_msg.msgdata.can->data[0] = 0x21 + i; // CF identifier
			for (int j = 1; j < 8; j++) {
				if (i == 0) {
					tx_msg.msgdata.can->data[j] = DataAll[7 + j];
					if ((7 + j) == DataAll.size()) {
						DataAll.push_back(0xcc);
					}
				}
				if (i == 1) {
					tx_msg.msgdata.can->data[j] = DataAll[14 + j];
					if ((14 + j) == DataAll.size()) {
						DataAll.push_back(0xcc);
					}
				}
				if (i == 2) {
					tx_msg.msgdata.can->data[j] = DataAll[21 + j];
					if ((21 + j) == DataAll.size()) {
						DataAll.push_back(0xcc);
					}
				}
				if (i == 3) {
					tx_msg.msgdata.can->data[j] = DataAll[28 + j];
					if ((28 + j) == DataAll.size()) {
						DataAll.push_back(0xcc);
					}
				}
				if (i == 4) {
					tx_msg.msgdata.can->data[j] = DataAll[35 + j];
					if ((35 + j) == DataAll.size()) {
						DataAll.push_back(0xcc);
					}
				}
			}
			// Write CF message
			res = CANTP_Write_2016(transmitter_handle, &tx_msg);
			printf("Write CF message %d on transmitter: %s\n", i + 1, STATUS_OK_KO(res));
		}
	}
	else {
		printf("Timeout or error while waiting for FC frame: %d\n", wait_result);
	}

	while (true) {
		// Wait for a message to be received
		wait_result = WaitForSingleObject(receive_event, 1000); // 1000ms timeout
		if (wait_result == WAIT_OBJECT_0) {
			// Read the received message
			res = CANTP_Read_2016(receiver_handle, &rx_msg, NULL, PCANTP_MSGTYPE_ISOTP);
			if (res == PCANTP_STATUS_CAUTION_RX_QUEUE_FULL) {
				printf("Warning: Receive queue is full.\n");
			}
			//else if (CANTP_StatusIsOk_2016(res, PCANTP_STATUS_OK, false)) {
				// Check if the received message has identifier 0x7F
			if (rx_msg.msgdata.can->data[0] == 0x7F) {
				printf("Received message with identifier 0x7F: ");
				for (int i = 0; i < rx_msg.msgdata.can->length; i++) {
					printf("%02x ", rx_msg.msgdata.can->data[i]);
					TFr.DataTotal.push_back(rx_msg.msgdata.can->data[i]);

				}
				printf("\n");
				break;
			}
			//}
			else {
				printf("Error reading message: %s\n", STATUS_OK_KO(res));
			}
		}
		else {
			printf("Timeout or error while waiting for a message: %d\n", wait_result);
		}
	}

	// Free messages space
	res = CANTP_MsgDataFree_2016(&tx_msg);
	printf("Free tx message: %s\n", STATUS_OK_KO(res));
	res = CANTP_MsgDataFree_2016(&rx_msg);
	printf("Free rx message: %s\n", STATUS_OK_KO(res));

	// Close receive event
	res = CANTP_SetValue_2016(receiver_handle, PCANTP_PARAMETER_RECEIVE_EVENT, &null_handle, sizeof(HANDLE));
	printf("Stop receive event on receiver: %s\n", STATUS_OK_KO(res));
	res_b = CloseHandle(receive_event);
	printf("Close receive event: %s\n", OK_KO(res_b));

	// Uninitialize channels
	res = CANTP_Uninitialize_2016(transmitter_handle);
	return TFr;
}