#ifndef APP_MAIN_H
#define APP_MAIN_H

#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <iostream>
#include <Poco/Net/HTTPServer.h>
#include "Poco/Dynamic/Var.h"

using namespace Poco::Net;
using namespace Poco::Util;
using namespace Poco::JSON;
using namespace std;


class MyServer : public ServerApplication {

protected:
    void initialize(Application& self);

    void uninitialize();
public:
    int main(const std::vector<std::string>& args);
    //void run();
};

#endif 

