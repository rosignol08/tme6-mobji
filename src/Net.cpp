#include "Net.h"
#include  <limits>

using namespace std;

namespace Netlist{
    
    const size_t Net::noid = numeric_limits<size_t>::max();

    // Constructeur
    Net::Net ( Cell* cell, const std::string & name , Term::Type dir )
        :   owner_  (),
            name_   (name),
            type_   (dir),
            id_     (cell->newNetId()),
            nodes_  ()
    {
        cell->add(this);
    }

    Net::Net ( Instance* inst, const std::string & name , Term::Type dir )
        :   owner_  (),
            name_   (name),
            type_   (dir),
            id_     (inst->getMasterCell()->newNetId()),
            nodes_  ()
    {
        inst->getMasterCell()->add(this);
    }

    //retourne le pointeur vers la cellule propriétaire du net
    Cell* Net::getCell()const{
        return this->owner_;
    }
    //retourne le nom du net
    const std::string &Net::getName()const{
        return this->name_;

    }
    //retourne l'id du net
    unsigned int Net::getId()const{
        return this->id_;
        
    }
    //retourne le type du net
    Term::Type Net::getType()const{
        return this->type_;

    }
    //retourne le vecteur de noeuds
    const std::vector<Node*>& Net::getNodes() const{
        return this->nodes_;
    }

    //on doit trouver l'index de la première case libre dans le tableau 
    //si aucune case n'est libre, elle renverra la taille du tableau,
    //c'est à dire l'index situé immédiatement après le dernier élément
    size_t Net::getFreeNodeId() const {
        for (size_t i = 0; i < nodes_.size(); ++i) {
            if (nodes_[i] == nullptr) {
                return i;
            }
        }//pas de case libre return size du tab
        return nodes_.size();
    }
    //ajoute un noeud au net
    void Net::add(Node * node){
        size_t id = this->getFreeNodeId();
        if (id <= this->nodes_.size()){
            node->setId(id); // associe l'id du noeud
            this->nodes_.push_back(node);
        }
    }
    //enlève un noeud au net
    bool Net::remove(Node * node){
        for(size_t i = 0; i < this->nodes_.size(); i++){
            if(this->nodes_[i] == node){
                this->nodes_.erase(this->nodes_.begin()+i);
                return true;
            }
        }
        return false;
    }

    void Net::toXml (std::ostream& stream){
        stream << indent << "<net name=\"" << name_ << "\" type=\"";
        switch(type_){
            case Term::Type::Internal:
                stream << "Internal";
                break;
            case Term::Type::External:
                stream << "External";
                break;
        }
        stream << "\">\n";
        indent++;
        for (Node* n: nodes_){
            n->toXml(stream);
        }
        stream << --indent << "</net>\n";
    }
}
