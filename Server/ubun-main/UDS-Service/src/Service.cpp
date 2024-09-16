#include "Service.h"
string Service::byteToHex(unsigned char byte) {

    string hexString;

    while (byte > 0) {

        int remainder = byte % 16;

        char hexChar;
        if (remainder < 10) {
            hexChar = '0';
            hexChar = remainder + 48;
        }
        else {

            hexChar = remainder + 55;
        }

        hexString = hexChar + hexString;

        byte /= 16;
    }

    return hexString;
}
vector<Service> Service::SendDescription(std::string BR, std::string CH, DWORD ID, std::vector<BYTE> DA, int dlc) {
    vector<Service> services;
    try {
        DOMParser parser;
        //std::string str = "C:/Users/mobenbra/source/repos/DDM_test/UDS-Service/inc/UDS_Req.xml";
        std::string str = "C:/Users/monta/OneDrive/Desktop/UDS_Req.xml";
        Poco::FileInputStream fis(str);
        std::stringstream ss;
        Poco::StreamCopier::copyStream(fis, ss);
        const std::string& xmlString = ss.str();
        AutoPtr<Document> pDoc = parser.parseString(xmlString);

        Element* pRoot = pDoc->documentElement();
        NodeList* pServiceList = pRoot->getElementsByTagName("SID");

        for (unsigned int i = 0; i < pServiceList->length(); ++i) {
            Service service;
            Element* pServiceElement = dynamic_cast<Element*>(pServiceList->item(i));
            service.name = pServiceElement->getAttribute("Name");
            service.value = pServiceElement->getAttribute("value");
            service.desc = pServiceElement->getAttribute("DES");

            // Parsing sub-functions
            NodeList* pSubFuncList = pServiceElement->getElementsByTagName("SUB-FUNCS");
            for (unsigned int j = 0; j < pSubFuncList->length(); ++j) {
                DID did;
                Element* pSubFuncElement = dynamic_cast<Element*>(pSubFuncList->item(j));
                did.category = pSubFuncElement->getAttribute("CATEGORY");
                did.size = pSubFuncElement->getAttribute("SIZE");
                service.dids.push_back(did);
            }

            // Parsing response sub-functions
            NodeList* pRespSubFuncList = pServiceElement->getElementsByTagName("SUB-FUNC");
            for (unsigned int j = 0; j < pRespSubFuncList->length(); ++j) {
                Element* pRespSubFuncElement = dynamic_cast<Element*>(pRespSubFuncList->item(j));
                SBF respSubFunction;
                respSubFunction.name = pRespSubFuncElement->getAttribute("Name");
                respSubFunction.value = pRespSubFuncElement->getAttribute("VALUE");
                respSubFunction.description = pRespSubFuncElement->innerText();
                service.sbfs.push_back(respSubFunction);
            }

            services.push_back(service);
        }
    }
    catch (Poco::XML::XMLException& ex) {
        std::cerr << "Error: " << ex.displayText() << std::endl;
    }
    catch (const Poco::FileException& e) {
        std::cout << e.displayText();
    }
    return services;
}

vector<Service> Service::RecieveDescription(std::string BR, std::string CH, DWORD ID, std::vector<BYTE> DA, int dlc) {
    vector<Service> services;
    try {
        DOMParser parser;
        //std::string str = "C:/Users/mobenbra/source/repos/DDM_test/UDS-Service/inc/UDS_Res.xml";
        std::string str = "C:/Users/monta/OneDrive/Desktop/UDS_Res.xml";
        Poco::FileInputStream fis(str);
        std::stringstream ss;
        Poco::StreamCopier::copyStream(fis, ss);
        const std::string& xmlString = ss.str();
        AutoPtr<Document> pDoc = parser.parseString(xmlString);

        Element* pRoot = pDoc->documentElement();
        NodeList* pServiceList = pRoot->getElementsByTagName("SID");

        for (unsigned int i = 0; i < pServiceList->length(); ++i) {
            Service service;
            Element* pServiceElement = dynamic_cast<Element*>(pServiceList->item(i));

            service.name = pServiceElement->getAttribute("Name");
            service.value = pServiceElement->getAttribute("value");


            // Parsing sub-functions
            NodeList* pSubFuncList = pServiceElement->getElementsByTagName("SUB-FUNCS");
            for (unsigned int j = 0; j < pSubFuncList->length(); ++j) {
                DID did;
                Element* pSubFuncElement = dynamic_cast<Element*>(pSubFuncList->item(j));
                did.category = pSubFuncElement->getAttribute("CATEGORY");
                did.size = pSubFuncElement->getAttribute("SIZE");
                service.dids.push_back(did);
            }

            // Parsing response sub-functions
            NodeList* pRespSubFuncList = pServiceElement->getElementsByTagName("SUB-FUNC");
            for (unsigned int j = 0; j < pRespSubFuncList->length(); ++j) {
                Element* pRespSubFuncElement = dynamic_cast<Element*>(pRespSubFuncList->item(j));
                SBF respSubFunction;
                respSubFunction.name = pRespSubFuncElement->getAttribute("Name");
                respSubFunction.value = pRespSubFuncElement->getAttribute("VALUE");
                respSubFunction.description = pRespSubFuncElement->innerText();
                service.sbfs.push_back(respSubFunction);
            }

            services.push_back(service);
        }
    }
    catch (Poco::XML::XMLException& ex) {
        std::cerr << "Error: " << ex.displayText() << std::endl;
    }
    catch (const Poco::FileException& e) {
        std::cout << e.displayText();
    }
    return services;
}