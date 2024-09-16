#include "appMain.h"
#include <Poco/Net/HTTPServer.h>
#include <Poco/Util/ServerApplication.h>

#include "ws.h" 
#include "ws_req.h" 


using namespace Poco::Net;
using namespace Poco::Util;
using namespace std;


void MyServer::initialize(Application& self)
{
    loadConfiguration(); // Load default configuration
    ServerApplication::initialize(self);
}

void MyServer::uninitialize()
{
    ServerApplication::uninitialize();
}


int MyServer::main(const vector<string>& args)
{
    HTTPServerParams* pParams = new HTTPServerParams;
    pParams->setMaxQueued(100);
    pParams->setMaxThreads(16);
    int time = 520000;
    HTTPServer srv(new RequestHandlerFactoryTwo, ServerSocket(8080), pParams);
 


    srv.start();
   
    cout << "Server started on port 8080" << endl;
    waitForTerminationRequest();
    srv.stop();
    
    return Application::EXIT_OK;

}
int main(int argc, char** argv)
{
    MyServer app;
    return app.run(argc, argv);
}