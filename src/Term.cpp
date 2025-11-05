#include "Term.h"
#include "Net.h"
#include "Node.h"

using namespace std;

namespace Netlist{

    Term::Term ( Cell* cell, const std :: string & name , Direction direction)
        :   owner_      (), 
            name_       (name),
            direction_  (direction),
            type_       (),
            net_        (),
            node_       (Node(this, Netlist::Net::noid))
    {
        cell->add(this);
    }

    Term::Term ( Instance* inst, const Term* modelTerm)
        :   owner_      (),
            name_       (modelTerm->getName()),
            direction_  (modelTerm->getDirection()),
            type_       (modelTerm->getType()),
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
        return (owner_ && !isExternal()) ? static_cast<Cell*>(owner_) : nullptr;
    }
    Cell*                  Term::getOwnerCell    ()const{
        //getOwnerCell() renvoie la Cell dans laquelle l'objet se trouve, ce qui, dans le cas d'un Term d'instance est la Cell possédant celle-ci.
        if(isExternal()){
            return static_cast<Cell*>(owner_); //obligé de cast
        }
        //sinon il a pas d'owner cell donc on renvoie nullptr
        return nullptr;
    }
    Instance*              Term::getInstance     ()const{
        return (owner_ && isExternal()) ? static_cast<Instance*>(owner_) : nullptr;//checker ça

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
        stream << "\"/>\n";
    }
    Term* Term::fromXml (Cell* cell, xmlTextReaderPtr reader ) {
    enum  State { Init           = 0
                  , BeginTerms
                  , EndTerms
                  };

    //const xmlChar* cellTag      = xmlTextReaderConstString( reader, (const xmlChar*)"cell" );
    //const xmlChar* netsTag      = xmlTextReaderConstString( reader, (const xmlChar*)"nets" );
    const xmlChar* termsTag     = xmlTextReaderConstString( reader, (const xmlChar*)"terms" );
    //const xmlChar* instancesTag = xmlTextReaderConstString( reader, (const xmlChar*)"instances" );

    //Cell* cell   = NULL;
    Term* term = nullptr;
    State state  = Init;

    while ( true ) {
        int status = xmlTextReaderRead(reader);
        if (status != 1) {
          if (status != 0) {
            cerr << "[ERROR] Term::fromXml(): Unexpected termination of the XML parser." << endl;
          }
          break;
        }

        switch ( xmlTextReaderNodeType(reader) ) {
          case XML_READER_TYPE_COMMENT:
          case XML_READER_TYPE_WHITESPACE:
          case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
            continue;
        }

        const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );

        switch ( state ) {
          case Init:
          if(termsTag == nodeName){
            state = BeginTerms;
            string termName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) ); //référence indéfinie vers « Netlist::xmlCharToString[abi:cxx11](unsigned char*) »
            
            if (not termName.empty()){
                string dirString = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"direction" ) );
                Direction dir;
                if(dirString == "In"){
                    dir = In;
                }else if(dirString == "Out"){
                    dir = Out;
                }else if(dirString == "Inout"){
                    dir = Inout;
                }else if(dirString == "Tristate"){
                    dir = Tristate;
                }else if(dirString == "Transcv"){
                    dir = Transcv;
                }else{
                    dir = Unknown;
                }
                //Term temporaire
                term = new Term ( cell, termName, dir );
                state = EndTerms;
                continue;
            }
            break;
          }
            
            //if (cellTag == nodeName) {
            //  state = BeginCell;
            //  string cellName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
            //  if (not cellName.empty()) {
            //    cell = new Cell ( cellName );
            //    state = BeginTerms;
            //    continue;
            //  }
            //}
            //break;{
          case BeginTerms:
            if ( (nodeName == termsTag) and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT) ) {
                state = EndTerms;
                continue;
            }
            break;
          case EndTerms:
            if ( (nodeName == termsTag) and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT) ) {
              //state = $;
              continue;
            }
            break;
          //case BeginInstances:
          //  if ( (nodeName == instancesTag) and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT) ) {
          //    state = EndInstances;
          //    continue;
          //  }
          //  break;
          //case EndInstances:
          //  if ( (nodeName == instancesTag) and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT) ) {
          //    state = BeginNets;
          //    continue;
          //  } else {
          //    if (Instance::fromXml(cell,reader)) continue;
          //  }
          //  break;
          //case BeginNets:
          //  if ( (nodeName == netsTag) and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT) ) {
          //    state = EndNets;
          //    continue;
          //  }
          //  break;
          //case EndNets:
          //  if ( (nodeName == netsTag) and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT) ) {
          //    state = EndCell;
          //    continue;
          //  } else {
          //    if (Net::fromXml(cell,reader)) continue;
          //  }
          //  break;
          //case EndCell:
          //  if ( (nodeName == cellTag) and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT) ) {
          //    continue;
          //  }
          //break;
          default:
            break;
        }

        cerr << "[ERROR] Term::fromXml(): Unknown or misplaced tag <" << nodeName << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
        //renvoie un ptr NULL en cas d'erreur
        //delete term;
        term = nullptr; //TODO checker si on tombe sur ces lignes tout le temps
        break;
  }

  return term;
}

}