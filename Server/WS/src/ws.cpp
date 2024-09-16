#include "WS.h"

int WebSocketRequestHandlerTwo::StringToByte(const string& hexString) {
	int value = strtol(hexString.c_str(), nullptr, 16);
	cout.setf(ios::hex, ios::basefield);
	return value;
}

std::vector<BYTE> WebSocketRequestHandlerTwo:: hexStringToByte(const std::string& hexString) {
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

string WebSocketRequestHandlerTwo::VectorToStringHex(vector<BYTE> vec) {
	string res; 
	for (BYTE b : vec) {
		stringstream ss;
		ss << hex << setfill('0') << setw(2) << (int)b;
		res += ss.str();
	}
	return res; 
}
void WebSocketRequestHandlerTwo::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
	char buffer[1024];
	int flags;
	int n;

	try
	{

		WebSocket ws(request, response);
		ws.setReceiveTimeout(Poco::Timespan(60*60, 0)); // 3600 seconds receive timeout
		ws.setSendTimeout(Poco::Timespan(60*60, 0)); // 3600 seconds send timeout
		do
		{
			n = ws.receiveFrame(buffer, sizeof(buffer), flags);
			string receivedData(buffer, n);
			processJSONDATA(receivedData, ws);
		} while (n > 0 && (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);
	}
	catch (WebSocketException& exc)
	{
		switch (exc.code())
		{
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

Object::Ptr parseJSON(const string& data) {
	Parser parser;
	Poco::Dynamic::Var result = parser.parse(data);
	Object::Ptr jsonObject = result.extract<Object::Ptr>();

	return jsonObject;
}
void WebSocketRequestHandlerTwo::processJSONDATA(const string& Data, WebSocket& ws) {

	Parser parser;
	Poco::Dynamic::Var result = parser.parse(Data);
	Object::Ptr JSONObject = result.extract<Object::Ptr>();

	if (JSONObject->has("D0") && JSONObject->has("D1") && JSONObject->has("D2") && JSONObject->has("D3") && JSONObject->has("D4") &&
		JSONObject->has("D5") && JSONObject->has("D6") && JSONObject->has("D7")  && JSONObject->has("DataL")) {
		D0 = StringToByte(JSONObject->get("D0").toString());
		D1 = StringToByte(JSONObject->get("D1").toString());
		D2 = StringToByte(JSONObject->get("D2").toString());
		D3 = StringToByte(JSONObject->get("D3").toString());
		D4 = StringToByte(JSONObject->get("D4").toString());
		D5 = StringToByte(JSONObject->get("D5").toString());
		D6 = StringToByte(JSONObject->get("D6").toString());
		D7 = StringToByte(JSONObject->get("D7").toString());
		
		data = hexStringToByte(JSONObject->get("DataL").toString());
		string a = JSONObject->get("A").toString();

		
		
		try {
		cout << "D0: " << D0 << endl;
		cout << "D1: " << D1 << endl;
		cout << "D2: " << D2 << endl;
		cout << "D3: " << D3 << endl;
		cout << "D4: " << D4 << endl;
		cout << "D5: " << D5 << endl;
		cout << "D6: " << D6 << endl;
		cout << "D7: " << D7 << endl;
		}
		catch (const std::exception& e) 
		{
			std::cerr << "Error: " << e.what() << std::endl;
		}
		//One 1
		
		//int a; 
		//std::cin >> a; 
		
		Simulation sim;
		Object::Ptr receivedObject = parseJSON(Data);
		Object::Ptr responseObject = new Object;

		
		if (a == "1") {
			Frame fr = sim.communication(D0, D1, D2, D3, D4, D5, D6, D7);
			cout << "Final Value Of D00 : " << fr.D00 << endl;
			
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
		
		if(a=="2"){

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