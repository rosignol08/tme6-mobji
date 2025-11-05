#ifndef NET_H
#define NET_H

#pragma once
#include "Term.h"
#include "Instance.h"
#include <vector>
#include <string>

namespace Netlist {
    class Cell;
    class Node;

    class Net {
        private:
                    Cell*                     owner_;
                    std::string               name_;
                    Term::Type                type_;
                    unsigned int              id_;
                    std::vector<Node*>        nodes_;

        public:
                    Net                                       ( Cell*, const std::string&, Term::Type dir );
                    Net                                       ( Instance* , const std::string&, Term::Type dir );
                    ~Net                                      () {};

        private:
            Net ( const Net & );

        public:
            static  const size_t              noid;
                    Cell*                     getCell         () const;
                    const std::string&        getName         () const;
                    unsigned int              getId           () const;
                    Term::Type                getType         () const;
                    const std::vector<Node*>& getNodes        () const;
                    size_t                    getFreeNodeId   () const;
                    void                      add             (Node*);
                    bool                      remove          (Node*);
                    void                      toXml           (std::ostream&);
    };
}
#endif