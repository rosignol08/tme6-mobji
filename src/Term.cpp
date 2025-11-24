#include "Term.h"
#include "Net.h"
#include "Node.h"

using namespace std;

namespace Netlist{

    Term::Term ( Cell* cell, const std :: string & name , Direction direction)
        :   owner_      (cell), 
            name_       (name),
            direction_  (direction),
            type_       (Type::External),
            net_        (),
            node_       (Node(this, Netlist::Net::noid))
    {
        cell->add(this);
    }

    Term::Term ( Instance* inst, const Term* modelTerm)
        :   owner_      (inst),
            name_       (modelTerm->getName()),
            direction_  (modelTerm->getDirection()),
            type_       (Type::Internal),
            net_        (modelTerm->getNet()),
            node_       (Node(this, (size_t) Net::noid))
    {
        inst->add(this);
    }

    Term::~Term() {}

    //constructeur pour un terminal interne
    bool                    Term::isInternal      ()const{
        if(this->type_ == 1){
            return true;
        }else if(this->type_ == 2)
        {
            return false;
        }
        return false;//par defaut
    }
    bool                    Term::isExternal      ()const{
     if(this->type_ == 2){
            return true;
        }else if(this->type_ == 1)
        {
            return false;
        }
        return false;//par defaut

    }
    const std::string &   Term::getName         () const{
        return this->name_;
    }
    Node*                  Term::getNode         (){
        return &this->node_;
    }
    Net*                   Term::getNet          ()const{
        return this->net_;
    }
    Cell*                  Term::getCell         ()const{
        return (owner_ && isExternal()) ? static_cast<Cell*>(owner_) : nullptr;
    }
    Cell*                  Term::getOwnerCell    ()const{
        //getOwnerCell() renvoie la Cell dans laquelle l'objet se trouve, ce qui, dans le cas d'un Term d'instance est la Cell possédant celle-ci.
        if(isExternal()){
            return static_cast<Cell*>(owner_); //obligé de cast
        }
        //sinon c'est un Term d'Instance, on renvoie la Cell qui possède l'Instance
        Instance* inst = getInstance();
        return inst ? inst->getCell() : nullptr;
    }
    Instance* Term::getInstance() const {
    //check si owner_ existe
    if (!owner_) {
        return nullptr;
    }
    
    //si un Term de Cell, owner_ est une Cell, donc pas d'instance
    if (getCell() != nullptr) {
        return nullptr;
    }
    
    //sinon Term d'Instance
    return static_cast<Instance*>(owner_);
}
    Term::Direction              Term::getDirection    ()const{
        return this->direction_;
    }
    Point                   Term::getPosition     ()const{
        return node_.getPosition();
    }
    Term::Type              Term::getType               () const{
        return this->type_;
    }
    void                    Term::setNet          ( Net * net ){
        net->add(&node_);
        this->net_ = net;
    }
    void                    Term::setNet          ( const std::string& netnom){
        net_ = getCell()->getNet(netnom);
        net_->add(&node_);
    }
    void                    Term::setPosition     ( const Point& point){
        node_.setPosition(point);
    }
    void                    Term::setPosition     ( int x , int y ){
        node_.setPosition(x,y);
    }
    void                    Term::setDirection    ( Direction direction_ ){
        this->direction_ = direction_;
    }

    void Term::toXml( std::ostream& stream){
        stream << indent << "<term name=\"" << name_;
        stream << "\" direction=\"";
        switch(direction_){
            case In:
                stream << "In";
                break;
            case Out:
                stream << "Out";
                break;
            case Inout:
                stream << "Inout";
            case Tristate:
                stream << "Tristate";
                break;
            case Transcv:
                stream << "Transcv";
                break;
            case Unknown:
                stream << "Unknown";
                break;
        }
        stream << "\" x=\"" << node_.getPosition().getX() << "\" y=\"" << node_.getPosition().getY() << "\"/>\n";
    }
    Term* Term::fromXml (Cell* cell, xmlTextReaderPtr reader ) {
        Term* term = nullptr;
        
        //nettoyage
        while (xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT) {
            int nodeType = xmlTextReaderNodeType(reader);
            if (nodeType == XML_READER_TYPE_COMMENT ||
                nodeType == XML_READER_TYPE_WHITESPACE ||
                nodeType == XML_READER_TYPE_SIGNIFICANT_WHITESPACE ||
                nodeType == XML_READER_TYPE_TEXT) {
                //skip les vilain <text> etc
                int status = xmlTextReaderRead(reader);
                if (status != 1) {
                    return nullptr;
                }
            } else {
                break;
            }
        }
        
        const xmlChar* nodeName = xmlTextReaderConstLocalName(reader);
        if (xmlStrcmp(nodeName, (const xmlChar*)"term") != 0) {
            cerr << "[WARNING] Term::fromXml(): Expected <term> tag but found <" << nodeName << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << "), skipping." << endl;
            return nullptr;
        }

        string termName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"name"));
        
        if (!termName.empty()) {
            string dirString = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"direction"));
            Direction dir;
            if (dirString == "In") {
                dir = In;
            } else if (dirString == "Out") {
                dir = Out;
            } else if (dirString == "Inout") {
                dir = Inout;
            } else if (dirString == "Tristate") {
                dir = Tristate;
            } else if (dirString == "Transcv") {
                dir = Transcv;
            } else {
                dir = Unknown;
            }
            string xpos = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"x"));
            string ypos = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"y"));
            term = new Term(cell, termName, dir);
            if (!xpos.empty() && !ypos.empty()) {
            term->setPosition(stoi(xpos), stoi(ypos));
            }
        }

        return term;
    }

}