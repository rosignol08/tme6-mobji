#ifndef NETLIST_XMLUTIL_H
#define NETLIST_XMLUTIL_H

#include <libxml2/libxml/xmlreader.h>
#include <string>

namespace Netlist {


  std::string  xmlCharToString    ( xmlChar* truc);
  bool         xmlGetIntAttribute ( xmlTextReaderPtr autre_truc, const std::string& attribute, int& value );


}  // Netlist namespace.

#endif  // NETLIST_XMLUTIL_H