#pragma once 
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Timespan.h>
#include <Poco/JSON/Object.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Net/WebSocket.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/NetException.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/Net/Socket.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace Poco::Net;
using namespace Poco::JSON;
using namespace Poco;
using namespace std;

class UnifiedWebSocketRequestHandler : public HTTPRequestHandler {
    // Members from both classes
    string baudRate;
    string channel;
    DWORD id;
    int D0, D1, D2, D3, D4, D5, D6, D7;
    vector<BYTE> data;
    int dlc;

public:
    // Handle request
    void handleRequest(HTTPServerRequest&, HTTPServerResponse&);

    // Processing JSON data
    void processData(const string&, WebSocket&);
    void processJson(const string& data);
    void processJSON(const string&);

    // Conversion functions
    static DWORD hexStringToUint(const string&);
    WORD hexStringToUSint(const string&);
    static vector<BYTE> hexStringToByte(const string&);
    static int hexStringToInt(const string&);
    static int StringToByte(const string&);
    static string VectorToStringHex(vector<BYTE> vec);

    // Getters
    string getbaudRate() const;
    string getchannel() const;
    DWORD getid() const;
    vector<BYTE> getdata() const;
    int getdlc() const;
};