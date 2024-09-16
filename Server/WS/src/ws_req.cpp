#include "WS.h"
#include "WS_req.h"
HTTPRequestHandler* RequestHandlerFactoryTwo::createRequestHandler(const HTTPServerRequest& request) {

	return new WebSocketRequestHandlerTwo;
}