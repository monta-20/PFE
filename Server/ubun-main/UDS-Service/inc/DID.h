#include <iostream>
#include <sstream>
#include "string"
#include <vector>
#include "Poco/FileStream.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/NodeList.h"
#include "Poco/DOM/NodeIterator.h"
#include "Poco/DOM/Element.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/Node.h"
#include "Poco/DOM/NodeList.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/AutoPtr.h"
#include "Poco/XML/XMLException.h"
#include "Poco/StreamCopier.h"

using namespace Poco::XML;

using namespace std;

#ifndef DID_H
#define DID_H

class DID {
public:
	string category;
	string size;

};

#endif