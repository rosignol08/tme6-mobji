#include "Instance.h"
#include "Term.h"
#include <algorithm> //voir si c'est obligé ça TODO

using namespace std;
using namespace Netlist;

namespace Netlist
{
    // constructeur : doit gerer l'ajout de l'instance au niveau de la cell
    Instance::Instance(Cell *owner, Cell *model, const std::string &name)
        : owner_(owner), masterCell_(model), name_(name), position_(0, 0)
    {
        // ajout de l'instance à la cell owner
        if (owner_ != nullptr)
        {
            owner_->add(this);
        }
        // duplication des terminaux de la cell model
        if (masterCell_ != nullptr)
        {
            vector<Term *> modelTerms = masterCell_->getTerms();
            for (const Term *term : modelTerms)
            {
                auto newterm = new Term(this, term);
                this->terms_.push_back(newterm);
            }
        }
    }
    // destructeur : doit gerer le retrait de l'instance au niveau de la cell
    Instance::~Instance()
    {
        // retrait de l'instance de la cell owner
        if (owner_ != nullptr)
        {
            owner_->remove(this);
        }
        // destruction des terminaux
        for (auto &term : terms_)
        {
            delete term; // psk on utilise new dans le constructeur
        }
        terms_.clear();
    }

    const std::string &Instance::getName() const
    {
        return name_;
    }
    // renvoie la cellule maitre
    Cell *Instance::getMasterCell() const
    {
        return this->masterCell_;
    }
    // renvoie la cellule owner_ ? TODO je suis pas sûr
    Cell *Instance::getCell() const
    {
        return this->owner_;
    }
    // renvoie le vecteur des terminaux
    const std::vector<Term *> &Instance::getTerms() const
    {
        return this->terms_;
    }
    // renvoie le terminal de nom name s'il existe
    Term *Instance::getTerm(const std::string &name) const
    {
        for (const auto &term : terms_)
        { // parcourt les term du vecteur et on check si celui avec ce nom donné existe
            if (term->getName() == name)
            {
                return term;
            }
        }
        return nullptr;
    }
    // renvoie la position
    Point Instance::getPosition() const
    {
        return this->position_;
    }
    // associe le net au terminal de nom name s'il existe
    bool Instance::connect(const std::string &name, Net *net)
    {
        Term *terme = this->getTerm(name);
        if (terme != nullptr)
        { // il existe on l'associe
            terme->setNet(net);
            return true;
        }
        else
        {
            return false;
        }
    };
    // ajoute un terminal au vecteur
    void Instance::add(Term *terminal)
    {
        this->terms_.push_back(terminal);
        return;
    };
    void Instance::remove(Term *terminal)
    {
        auto it = std::find(this->terms_.begin(), this->terms_.end(), terminal);
        if (it != this->terms_.end())
        {
            this->terms_.erase(it);
        }
        return;
    };
    void Instance::setPosition(const Point &point)
    {
        this->position_.setX(point.getX());
        this->position_.setY(point.getY());
        return;
    };
    void Instance::setPosition(int x, int y)
    {
        this->position_.setX(x);
        this->position_.setY(y);
        return;
    };

    void Instance::toXml(ostream &stream)
    {
        stream << indent << "<instance name=\"" << name_ << "\" mastercell=\"";
        if (masterCell_)
            stream << masterCell_->getName();
        else
            stream << "None";
        stream << "\" x=\"" << position_.getX() << "\" y=\"" << position_.getY() << "\"/>\n";
    }
    // TODO voir de quoi est composée une instance
    Instance *Instance::fromXml(Cell *cell, xmlTextReaderPtr reader)
    {
        Instance* instance = nullptr;
        
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
        if (xmlStrcmp(nodeName, (const xmlChar*)"instance") != 0) {
            cerr << "[WARNING] Instance::fromXml(): Expected <instance> tag but found <" << nodeName << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << "), skipping." << endl;
            return nullptr;
        }

        string instanceName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"name"));
        string masterCellName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"mastercell"));
        
        if (!instanceName.empty()) {
            Cell* masterCell = Cell::find(masterCellName);
            instance = new Instance(cell, masterCell, instanceName);
            
            //get les position
            string xStr = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"x"));
            string yStr = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"y"));
            
            if (!xStr.empty() && !yStr.empty()) {
                int x = std::stoi(xStr);
                int y = std::stoi(yStr);
                instance->setPosition(x, y);
            }
        }

        return instance;
    }
}