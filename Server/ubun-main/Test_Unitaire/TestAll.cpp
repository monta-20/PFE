#include <gtest/gtest.h>
#include "../UDS-Service/inc/Service.h"
#include "../DDM-Server/inc/websocket_server.h"
Service service;

/*TEST(ServiceCLASS, SendDescripionFUNCTION) {
    std::string BR = "250K";
    std::string CH = "USBBUS1";
    DWORD ID = 0x18DA60F9;
    std::vector<BYTE> DA = { 0x02,0x10,0x02,0x00,0x00,0x00,0x00,0x00 };
    int dlc = 8;
    vector<Service> services = service.SendDescription(BR, CH, ID, DA, dlc);
    EXPECT_EQ(services[0].name, "DSC");
    EXPECT_EQ(services[0].value, "10");
    EXPECT_EQ(services[0].desc, "Diagnostic Session Control");

    EXPECT_EQ(services[0].sbfs[0].name, "Programming Session");
    EXPECT_EQ(services[0].sbfs[0].value, "02");
    EXPECT_EQ(services[0].sbfs[0].description, "Used to upload software");
}

TEST(ServiceCLASS, RecieveDescriptionFUNCTION) {
    std::string BR = "250K";
    std::string CH = "USBBUS1";
    DWORD ID = 0X18DAF960;
    std::vector<BYTE> DA = { 0x01, 0x02, 0x03 };
    int dlc = 8;

    vector<Service> services = service.RecieveDescription(BR, CH, ID, DA, dlc);
    EXPECT_EQ(services[0].name, "DSC");
    EXPECT_EQ(services[0].value, "10");
    EXPECT_EQ(services[0].sbfs[0].name, "Positive response");
    EXPECT_EQ(services[0].sbfs[0].value, "50");
    EXPECT_EQ(services[0].sbfs[0].description, "Service supported in active session.");
}*/

TEST(ServiceCLASS, ByteToHexFUNCTION) {
    // Test cases
   // EXPECT_EQ(service.byteToHex(0x0), "0");
    EXPECT_EQ(service.byteToHex(0x1), "1");
    EXPECT_EQ(service.byteToHex(0xA), "A");
    EXPECT_EQ(service.byteToHex(0xF), "F");
    EXPECT_EQ(service.byteToHex(0x10), "10");
    EXPECT_EQ(service.byteToHex(0x2F), "2F");
    EXPECT_EQ(service.byteToHex(0x3C), "3C");
    EXPECT_EQ(service.byteToHex(0x4D), "4D");
    EXPECT_EQ(service.byteToHex(0xFF), "FF");
    // Edge cases
   //EXPECT_EQ(service.byteToHex(0x00), "0");
    EXPECT_EQ(service.byteToHex(0xFF), "FF");
}
/*
// Test for hexStringToUint function
TEST(WebSocketRequestHandlerTest, HexStringToUintTest) {
    EXPECT_EQ(WebSocketRequestHandler::hexStringToUint("0x1A"), 26);
    EXPECT_EQ(WebSocketRequestHandler::hexStringToUint("0xFF"), 255);
    EXPECT_EQ(WebSocketRequestHandler::hexStringToUint("0x100"), 256);
    EXPECT_EQ(WebSocketRequestHandler::hexStringToUint("0x1234"), 4660);
}*/

// Test for hexStringToUSint function
/*TEST(WebSocketRequestHandlerTest, HexStringToUSintTest) {
    WebSocketRequestHandler ws;
    EXPECT_EQ(ws.hexStringToUSint("1A"), 26);
    EXPECT_EQ(ws.hexStringToUSint("FF"), 255);
    EXPECT_EQ(ws.hexStringToUSint("100"), 256);
    EXPECT_EQ(ws.hexStringToUSint("1234"), 4660);
}*/

// Test for hexStringToByte function
/*TEST(WebSocketRequestHandlerTest, HexStringToByteTest) {
    vector<BYTE> expected = { 0x02,0x10,0x02,0x00,0x00,0x00,0x00,0x00 };
    EXPECT_EQ(WebSocketRequestHandler::hexStringToByte("0x0210020000000000"), expected);*/

    //expected = { 0xFF, 0xEE, 0xDD, 0xCC };
   // EXPECT_EQ(WebSocketRequestHandler::hexStringToByte("0xFFEEDDCC"), expected);
//}
/*
// Test for hexStringToInt function
TEST(Testall, HexStringToIntTest) {
    EXPECT_EQ(WebSocketRequestHandler::hexStringToInt("1A"), 26);
    EXPECT_EQ(WebSocketRequestHandler::hexStringToInt("FF"), 255);
    EXPECT_EQ(WebSocketRequestHandler::hexStringToInt("100"), 256);
    EXPECT_EQ(WebSocketRequestHandler::hexStringToInt("1234"), 4660);
}

*/