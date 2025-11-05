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
}