// -*- explicit-buffer-name: "Main.cpp<M1-MOBJ/4-5>" -*-

#include <string>
#include <sstream>
#include <iostream>
#include <memory>
using namespace std;

#include "Term.h"
#include "Net.h"
#include "Instance.h"
#include "Cell.h"
#include "XmlUtil.h"
using namespace Netlist;

int main(int argc, char *argv[])
{
    cout << "Chargement des modeles:" << endl;
    cout << "- <and2> ..." << endl;
    Cell::load("and2");

    cout << "- <or2> ..." << endl;
    Cell::load("or2");

    cout << "- <xor2> ..." << endl;
    Cell::load("xor2");

    cout << "- <halfadder> ..." << endl;
    Cell *halfadder = Cell::load("halfadder");

    cout << "\nContenu du <halfadder>:" << endl;
    if (halfadder == NULL) {
        cerr << "[ERROR] main: failed to load <halfadder>.\n";
        return 1;
    }
    halfadder->toXml(cout);

    return 0;
}

/*

 Term* Term::fromXml (Cell* cell, xmlTextReaderPtr reader ) {
    // Vérifier si c'est un élément <term>
    if ( xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT ) {
        return nullptr;
    }
    
    const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );
    if ( xmlStrcmp(nodeName, (const xmlChar*)"term") != 0 ) {
        return nullptr;
    }
    
    // Lire les attributs
    xmlChar* nameAttr = xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" );
    xmlChar* dirAttr = xmlTextReaderGetAttribute( reader, (const xmlChar*)"direction" );
    
    if ( nameAttr == nullptr ) {
        if (dirAttr) xmlFree(dirAttr);
        return nullptr;
    }
    
    string termName = xmlCharToString( nameAttr );
    string dirString = dirAttr ? xmlCharToString( dirAttr ) : "";
    
    xmlFree(nameAttr);
    if (dirAttr) xmlFree(dirAttr);
    
    Direction dir = Unknown;
    if(dirString == "In") dir = In;
    else if(dirString == "Out") dir = Out;
    else if(dirString == "Inout") dir = Inout;
    else if(dirString == "Tristate") dir = Tristate;
    else if(dirString == "Transcv") dir = Transcv;
    
    return new Term( cell, termName, dir );
}
    */