// -*- explicit-buffer-name: "Node.cpp<M1-MOBJ/4-5>" -*-

#include  <limits>
#include  "Node.h"
#include  "Term.h"
#include  "Net.h"
#include  "Instance.h"
#include  "Cell.h"

namespace Netlist {

  using namespace std;


  const size_t  Node::noid = numeric_limits<size_t>::max();


  Node::Node ( Term* term, size_t id  )
    : id_      (id)
    , term_    (term)
    , position_()
  { }


  Node::~Node ()
  {
    if (getNet()) getNet()->remove( this );
  }


  inline Net* Node::getNet () const { return term_->getNet(); }


  void Node::toXml(ostream& stream){
    stream << indent << "<node term=\"" << term_->getName() << "\"";
    Instance* inst = term_->getInstance();
    if (inst){
      stream << " instance=\"" << inst->getName() << "\"";
    }
    stream << "\" id=\"" << id_ << "\" x=\"" << position_.getX() << "\" y=\"" << position_.getY() << "\">\n";
  }


}  // Netlist namespace.
