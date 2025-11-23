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

    void Net::toXml(std::ostream& stream){
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
    
/*
check si id node plus petit que le tableau ajouter des deletes dans le main
-composée de :
Cell*                     owner_;
std::string               name_;
Term::Type                type_;
unsigned int              id_;
std::vector<Node*>        nodes_;
le net qu'on doit crée est long
*/

    //doit renvoyer NULL en cas d'erreur
    Net* Net::fromXml(Cell* cell, xmlTextReaderPtr reader ){
       Net* net = nullptr;
       Node* node = nullptr;
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
        if (xmlStrcmp(nodeName, (const xmlChar*)"net") != 0) {
            cerr << "[WARNING] Net::fromXml(): Expected <net> tag but found <" << nodeName << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << "), skipping." << endl;
            return nullptr;
        }

        string netName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"name"));
        if(!netName.empty()){
            string typeStr = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"type"));
            Term::Type type;
            if(typeStr == "Internal"){
                type = Term::Type::Internal;
            } else if (typeStr == "External"){
                type = Term::Type::External;
            } else {
                cerr << "[ERROR] Net::fromXml(): Invalid type attribute \"" << typeStr << "\" in <net> tag (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
                return nullptr;
            }
            net = new Net(cell, netName, type);
        }
            //on accede à ses nodes maintenant
            while(true){
                int status = xmlTextReaderRead(reader);
                if (status != 1) {
                    break;//sortie de boucle
                }
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
                nodeName = xmlTextReaderConstLocalName(reader);
                //verif si c'est la fin du net
                if (xmlStrcmp(nodeName, (const xmlChar*)"net") == 0 && 
                    xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT) {
                    break;
                }
                if(xmlStrcmp(nodeName, (const xmlChar*)"node") == 0) {
                    string termName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"term"));
                    string instanceName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"instance"));
                    
                    //cerr << "[DEBUG] Looking for term='" << termName << "', instance='" << instanceName << "'" << endl;
                    
                    Term* term = nullptr;
                    Instance* inst = nullptr;
                    if (instanceName.empty()) {
                        // Cas 1: terme de la cellule (pas d'instance)
                        term = cell->getTerm(termName);
                        //cerr << "[DEBUG] Cell term '" << termName << "' " << (term ? "found" : "NOT FOUND") << endl;
                    } else {
                        //cas terme d'une instance
                        inst = cell->getInstance(instanceName);
                        cerr << "[DEBUG] Instance '" << instanceName << "' " << (inst ? "found" : "NOT FOUND") << endl;
                        if (inst) {
                            term = inst->getTerm(termName);
                            //cerr << "[DEBUG] Instance term '" << termName << "' " << (term ? "found" : "NOT FOUND") << endl;
                        }
                    }
                    
                    if (term == nullptr) {
                        cerr << "[ERROR] Net::fromXml(): Term '" << termName << "' not found" << endl;
                        return nullptr;
                    }
                    string nodeIdStr = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"id"));
                    if(!nodeIdStr.empty()){
                        size_t nodeId = stoi(nodeIdStr);
                        node = new Node(term, nodeId);

                        // Lire les coordonnées si présentes
                        string xStr = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"x"));
                        string yStr = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"y"));

                        if (!xStr.empty() && !yStr.empty()) {
                            node->setPosition(stoi(xStr), stoi(yStr));
                        }
                        //ajout du node au net
                        net->nodes_.push_back(node);
                    } else {
                        cerr << "[ERROR] Net::fromXml(): Missing id attribute in <node> tag (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
                        return nullptr;
                    }
                }
            
    }
    return net;
}

}