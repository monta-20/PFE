#pragma once 
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Timespan.h>
#include <Poco/JSON/Object.h>
#include <Poco/Net/HTTPServer.h>
#include "Poco/Dynamic/Var.h"
#include <Poco/Net/WebSocket.h>
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include <iostream>
#include "Poco/Net/NetException.h"
#include <iostream>
#include "string"
#include "sstream"
#include "iomanip"
#include "Poco/JSON/Stringifier.h"
#include "Poco/Net/Socket.h"
#include "simulation.h"

//Websocket Implementation
using namespace Poco::Net;
//Parsing Data Recieved from IHM
using namespace Poco::JSON;
//Threading
using namespace Poco;
//Printing and Debogage code 
using namespace std;

class WebSocketRequestHandlerTwo : public HTTPRequestHandler {
    int D0, D1, D2, D3, D4, D5, D6, D7;
    vector<BYTE> data;
public:
    //handleRequest
    void processJSONDATA(const string&, WebSocket&);
    void processJSON(const string&);
    void handleRequest(HTTPServerRequest&, HTTPServerResponse&);
    static int StringToByte(const string&);
    static vector<BYTE> hexStringToByte(const string&);
    static string VectorToStringHex(vector<BYTE> vec);
};