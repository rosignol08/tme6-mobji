#ifndef INSTANCE_H
#define INSTANCE_H

#pragma once
#include "Point.h"
#include "Term.h"
#include "Cell.h"

namespace Netlist {

    class Instance{
        private:
                Cell*               owner_;
                Cell*               masterCell_;
                std::string         name_;
                std::vector<Term*>  terms_;
                Point               position_;
        /*Constructeurs & Destructeur. Ils devront gérer l'ajout 
        et le retrait de l'Instance au niveau de la Cell.
        Le constructeur devra dupliquer la liste des terminaux 
        de la Cell model qu'il instancie. 
        A l'inverse, le destructeur détruit ses terminaux.

        Une Instance possède ses terminaux,
        elle doit donc les détruire dans son destructeur.

        */

        public:
                Instance                ( Cell* owner, Cell* model, const std::string& );
                ~Instance               ();

                //accesseurs
                const std::string&        getName       () const;
                Cell*                     getMasterCell () const;
                Cell*                     getCell       () const;
                const std::vector<Term*>& getTerms      () const;
                Term*                     getTerm       (const std::string&) const;
                Point                     getPosition   () const;
                //Modificateurs. connect() va associer le Net au terminal de nom name (s'il existe).

                bool                      connect       (const std::string& name, Net*);
                void                      add           (Term*);
                void                      remove        (Term*);
                void                      setPosition   (const Point&);
                void                      setPosition   (int x, int y);
                void                      toXml         (std::ostream&);
        /*
        class Term   masterCell_;
            std::string         name_;
            std::vector<Netlist::Term*>  terms_;
            Netlist::Point               position_;
        public:
                    Instance        ( Cell* owner, Cell* model, const std::string& );
                    ~Instance       (){};
            const std::string&
                    getName         () const ;
            Cell *  getMasterCell   () const ;
            Cell *  getCell         () const ;
            const std::vector<Netlist::Term*>&
                    getTerms        () const ;
            Term *  getTerm         (const std::string&) const ;
            Point   getPosition     () const;
            bool    connect         (const std::string & name, Net*);
            void    add             (Term *);
            void    remove          (Term *);
            void    setPosition     (const Point &);
            void    setPosition     (int x, int y);
            */
    };
}

#endif