#include "websocket_server.h"
#include "websocket_server_req.h"
HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(const HTTPServerRequest& request) {

	return new UnifiedWebSocketRequestHandler;
}