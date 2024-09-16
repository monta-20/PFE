#include "websocket_server.h"
#include "websocket_server.h"
#include "canMessage.h"
#include "Frame.h"
#include "TotalFrame.h"
#include "pcan_communication.h"
#include "simulation.h"
#include "Service.h"
//Conversion Function
DWORD UnifiedWebSocketRequestHandler::hexStringToUint(const string& hexString) {
	string strippedString = hexString.substr(2); // Start from index 2 (after "0x")
	DWORD result = strtol(strippedString.c_str(), nullptr, 16);
	return result;
}
WORD UnifiedWebSocketRequestHandler::hexStringToUSint(const string& hexString) {
	return stoi(hexString, nullptr, 16);
}
vector<BYTE> UnifiedWebSocketRequestHandler::hexStringToByte(const string& hexString) {
	std::vector<BYTE> bytes;

	if (hexString.empty()) {

		return bytes;
	}

	string strippedString = hexString.substr(2);
	for (size_t i = 0; i < strippedString.size(); i += 2) {
		std::string byteString = strippedString.substr(i, 2);
		unsigned int byteValue = std::stoi(byteString, nullptr, 16);
		std::cout << "Data[" << i / 2 << "]=" << std::hex << byteValue << std::endl;
		bytes.push_back(static_cast<BYTE>(byteValue));
	}

	return bytes;
}
int UnifiedWebSocketRequestHandler::hexStringToInt(const string& hexString) {
	return stoi(hexString);
}
int UnifiedWebSocketRequestHandler::StringToByte(const string& hexString) {
	int value = strtol(hexString.c_str(), nullptr, 16);
	cout.setf(ios::hex, ios::basefield);
	return value;
}
string UnifiedWebSocketRequestHandler::VectorToStringHex(vector<BYTE> vec) {
	string res;
	for (BYTE b : vec) {
		stringstream ss;
		ss << hex << setfill('0') << setw(2) << (int)b;
		res += ss.str();
	}
	return res;
}
void UnifiedWebSocketRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
    char buffer[1024];
    int flags;
    int n;

    try {
        WebSocket ws(request, response);
        ws.setReceiveTimeout(Poco::Timespan(60 * 60, 0)); // 3600 seconds receive timeout
        ws.setSendTimeout(Poco::Timespan(60 * 60, 0)); // 3600 seconds send timeout
        do {
            n = ws.receiveFrame(buffer, sizeof(buffer), flags);
            string receivedData(buffer, n);

            // Call both processJsonData and processJSONDATA
            processData(receivedData, ws);
        } while (n > 0 && (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);
    }
    catch (WebSocketException& exc) {
        switch (exc.code()) {
        case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
            response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
            // fallthrough
        case WebSocket::WS_ERR_NO_HANDSHAKE:
        case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
        case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
        case WebSocket::WS_ERR_HANDSHAKE_ACCEPT:
        case WebSocket::WS_ERR_UNAUTHORIZED:
        case WebSocket::WS_ERR_PAYLOAD_TOO_BIG:
        case WebSocket::WS_ERR_INCOMPLETE_FRAME:
            response.setStatusAndReason(HTTPServerResponse::HTTP_BAD_REQUEST);
            response.setContentLength(0);
            response.send();
            break;
        }
    }
    catch (Poco::TimeoutException& e) {
        cout << "Timeout Exception: " << e.displayText() << endl;
        response.setStatusAndReason(HTTPServerResponse::HTTP_REQUEST_TIMEOUT);
        response.setContentLength(0);
        response.send();
    }
    catch (JSONException& e) {
        cout << "JSON Exception: " << e.message() << endl;
    }
    catch (Poco::Exception& e) {
        cout << "Poco Exception: " << e.displayText() << endl;
    }
    catch (std::exception& e) {
        cout << "Standard Exception: " << e.what() << endl;
    }
}
Object::Ptr parseJson(const string& data) {
    Parser parser;
    Poco::Dynamic::Var result = parser.parse(data);
    Object::Ptr jsonObject = result.extract<Object::Ptr>();

    return jsonObject;
}
void UnifiedWebSocketRequestHandler::processData(const string& Data, WebSocket& ws) {
    Parser parser;
    Poco::Dynamic::Var result = parser.parse(Data);
    Object::Ptr jsonObject = result.extract<Object::Ptr>();

    bool isProcessJsonData = jsonObject->has("Baud Rate") && jsonObject->has("Channel") && jsonObject->has("ID") && jsonObject->has("Data") && jsonObject->has("DLC");
    bool isProcessJSONDATA = jsonObject->has("D0") && jsonObject->has("D1") && jsonObject->has("D2") && jsonObject->has("D3") && jsonObject->has("D4") &&
        jsonObject->has("D5") && jsonObject->has("D6") && jsonObject->has("D7") && jsonObject->has("DataL");

    if (isProcessJsonData) {
        // Traitement spécifique à processJsonData
        baudRate = jsonObject->get("Baud Rate").toString();
        channel = jsonObject->get("Channel").toString();
        id = hexStringToUint(jsonObject->get("ID").toString());
        data = hexStringToByte(jsonObject->get("Data").toString());
        dlc = hexStringToInt(jsonObject->get("DLC").toString());

        Communication comm;
        canMessage mg = comm.notify(baudRate, channel, id, data, dlc);
        Service s;
        s.SendDescription(baudRate, channel, id, data, dlc);

        string result;
        for (BYTE b : mg.data) {
            stringstream ss;
            ss << hex << setfill('0') << setw(2) << (int)b;
            result += ss.str();
        }

        Service ser;
        string sst = "0x" + ser.byteToHex(data[1]);
        string sstr = "0x0" + ser.byteToHex(data[2]);
        string sstrr = "0x" + ser.byteToHex(data[2]) + ser.byteToHex(data[3]);

        vector<Service> serv = ser.SendDescription(baudRate, channel, id, data, dlc);
        vector<Service> servR = ser.RecieveDescription(baudRate, channel, id, data, dlc);

        string ServiceDescription, ServiceValue, SBFName, SBFValue, SBFDescription, DIDName, DIDValue, DIDDescription;
        for (const auto& service : serv) {
            if (sst == service.value) {
                ServiceDescription = service.desc;
                ServiceValue = service.value;
                for (const auto& did : service.dids) {}
            }
            if (sst != "0x22" && sst != "0x2E" && sst == service.value) {
                for (const auto& subFunc : service.sbfs) {
                    if (sstr == subFunc.value) {
                        SBFName = subFunc.name;
                        SBFValue = subFunc.value;
                        SBFDescription = subFunc.description;
                    }
                }
            }
            else {
                for (const auto& subFunc : service.sbfs) {
                    if (sstrr == subFunc.value) {
                        DIDName = subFunc.name;
                        DIDValue = subFunc.value;
                        DIDDescription = subFunc.description;
                    }
                }
            }
        }

        string SBFNameRes, SBFValueRes, SBFDescriptionRes;
        for (const auto& service : servR) {
            if (("0x" + result.substr(2, 2)) != "0x7f") {
                for (const auto& subFunc : service.sbfs) {
                    if (("0x" + result.substr(2, 2)) == subFunc.value) {
                        SBFNameRes = subFunc.name;
                        SBFValueRes = subFunc.value;
                        SBFDescriptionRes = subFunc.description;
                    }
                }
            }
            else {
                for (const auto& subFunc : service.sbfs) {
                    if (("0x" + result.substr(6, 2)) == subFunc.value) {
                        SBFNameRes = subFunc.name;
                        SBFValueRes = subFunc.value;
                        SBFDescriptionRes = subFunc.description;
                    }
                }
            }
        }

        Object::Ptr receivedObject = parseJson(Data);
        receivedObject->set("SRVName", ServiceDescription);
        receivedObject->set("SRVValue", ServiceValue);
        receivedObject->set("SBFName", SBFName);
        receivedObject->set("SBFValue", SBFValue);
        receivedObject->set("SBFDescription", SBFDescription);
        receivedObject->set("DIDName", DIDName);
        receivedObject->set("DIDValue", DIDValue);
        receivedObject->set("DIDDescription", DIDDescription);
        receivedObject->set("Id", mg.id);
        receivedObject->set("Dlc", mg.dlc);
        receivedObject->set("Data", result);
        receivedObject->set("SBFNameRes", SBFNameRes);
        receivedObject->set("SBFValueRes", SBFValueRes);
        receivedObject->set("SBFDescriptionRes", SBFDescriptionRes);

        Object::Ptr responseObject = new Object;
        responseObject->set("response", "Data received successfully!");
        responseObject->set("SRVName", ServiceDescription);
        responseObject->set("SRVValue", ServiceValue);
        responseObject->set("SBFName", SBFName);
        responseObject->set("SBFValue", SBFValue);
        responseObject->set("SBFDescription", SBFDescription);
        responseObject->set("DIDName", DIDName);
        responseObject->set("DIDValue", DIDValue);
        responseObject->set("DIDDescription", DIDDescription);
        responseObject->set("Id", mg.id);
        responseObject->set("Dlc", mg.dlc);
        responseObject->set("Data", result);
        responseObject->set("SBFNameRes", SBFNameRes);
        responseObject->set("SBFValueRes", SBFValueRes);
        responseObject->set("SBFDescriptionRes", SBFDescriptionRes);

        std::ostringstream oss;
        Stringifier::stringify(*responseObject, oss);
        string responseStr = oss.str();
        ws.sendFrame(responseStr.c_str(), responseStr.size(), WebSocket::FRAME_TEXT);
    }
    else if (isProcessJSONDATA) {
        // Traitement spécifique à processJSONDATA
        D0 = StringToByte(jsonObject->get("D0").toString());
        D1 = StringToByte(jsonObject->get("D1").toString());
        D2 = StringToByte(jsonObject->get("D2").toString());
        D3 = StringToByte(jsonObject->get("D3").toString());
        D4 = StringToByte(jsonObject->get("D4").toString());
        D5 = StringToByte(jsonObject->get("D5").toString());
        D6 = StringToByte(jsonObject->get("D6").toString());
        D7 = StringToByte(jsonObject->get("D7").toString());
        data = hexStringToByte(jsonObject->get("DataL").toString());
        string a = jsonObject->get("A").toString();

        Simulation sim;
        Object::Ptr receivedObject = parseJson(Data);
        Object::Ptr responseObject = new Object;

        if (a == "1") {
            Frame fr = sim.communication(D0, D1, D2, D3, D4, D5, D6, D7);
            string RESULT = VectorToStringHex(fr.DataStore);

            receivedObject->set("Da", fr.Da);
            receivedObject->set("D00", fr.D00);
            receivedObject->set("D11", fr.D11);
            receivedObject->set("D22", fr.D22);
            receivedObject->set("D33", fr.D33);
            receivedObject->set("D44", fr.D44);
            receivedObject->set("D55", fr.D55);
            receivedObject->set("D66", fr.D66);
            receivedObject->set("Db", fr.Db);
            receivedObject->set("D77", fr.D77);
            receivedObject->set("D88", fr.D88);
            receivedObject->set("D99", fr.D99);
            receivedObject->set("D100", fr.D100);
            receivedObject->set("D110", fr.D110);
            receivedObject->set("D120", fr.D120);
            receivedObject->set("D130", fr.D130);
            receivedObject->set("Dc", fr.Dc);
            receivedObject->set("D140", fr.D140);
            receivedObject->set("D150", fr.D150);
            receivedObject->set("D160", fr.D160);
            receivedObject->set("D170", fr.D170);
            receivedObject->set("D180", fr.D180);
            receivedObject->set("D190", fr.D190);
            receivedObject->set("D200", fr.D200);
            receivedObject->set("RESULT", RESULT);

            responseObject->set("Da", fr.Da);
            responseObject->set("D00", fr.D00);
            responseObject->set("D11", fr.D11);
            responseObject->set("D22", fr.D22);
            responseObject->set("D33", fr.D33);
            responseObject->set("D44", fr.D44);
            responseObject->set("D55", fr.D55);
            responseObject->set("D66", fr.D66);
            responseObject->set("Db", fr.Db);
            responseObject->set("D77", fr.D77);
            responseObject->set("D88", fr.D88);
            responseObject->set("D99", fr.D99);
            responseObject->set("D100", fr.D100);
            responseObject->set("D110", fr.D110);
            responseObject->set("D120", fr.D120);
            responseObject->set("D130", fr.D130);
            responseObject->set("Dc", fr.Dc);
            responseObject->set("D140", fr.D140);
            responseObject->set("D150", fr.D150);
            responseObject->set("D160", fr.D160);
            responseObject->set("D170", fr.D170);
            responseObject->set("D180", fr.D180);
            responseObject->set("D190", fr.D190);
            responseObject->set("D200", fr.D200);
            responseObject->set("RESULT", RESULT);
        }

        if (a == "2") {
            TotalFrame TFr = sim.communicationDL(data);
            string RESULTT = VectorToStringHex(TFr.DataTotal);

            receivedObject->set("RESULTT", RESULTT);
            responseObject->set("RESULTT", RESULTT);
        }

        std::ostringstream oss;
        Stringifier::stringify(*responseObject, oss);
        string responseStr = oss.str();
        ws.sendFrame(responseStr.c_str(), responseStr.size(), WebSocket::FRAME_TEXT);
    }
}

//Getter Fuunctions

string UnifiedWebSocketRequestHandler::getbaudRate() const { return baudRate; }
string UnifiedWebSocketRequestHandler::getchannel() const { return channel; }
DWORD UnifiedWebSocketRequestHandler::getid() const { return id; }
vector<BYTE> UnifiedWebSocketRequestHandler::getdata() const { return data; }
int UnifiedWebSocketRequestHandler::getdlc() const { return dlc; }