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
/*
int main_ancien(int argc, char *argv[])
{
    cout << "\nConstruction du modele <and2>." << endl;
    Cell *and2 = new Cell("and2");
    new Term(and2, "i0", Term::In);
    new Term(and2, "i1", Term::In);
    new Term(and2, "q", Term::Out);
    and2->toXml(cout);

    cout << "\nConstruction du modele <or2>." << endl;
    Cell *or2 = new Cell("or2");
    new Term(or2, "i0", Term::In);
    new Term(or2, "i1", Term::In);
    new Term(or2, "q", Term::Out);
    or2->toXml(cout);

    cout << "\nConstruction du modele <xor2>." << endl;
    Cell *xor2 = new Cell("xor2");
    new Term(xor2, "i0", Term::In);
    new Term(xor2, "i1", Term::In);
    new Term(xor2, "q", Term::Out);
    xor2->toXml(cout);

    cout << "\nConstruction du modele <halfadder>." << endl;
    Cell *halfadder = new Cell("halfadder");
    new Term(halfadder, "a", Term::In);
    new Term(halfadder, "b", Term::In);
    new Term(halfadder, "sout", Term::Out);
    new Term(halfadder, "cout", Term::Out);
    Net *ha_a = new Net(halfadder, "a", Term::External);
    Net *ha_b = new Net(halfadder, "b", Term::External);
    Net *ha_sout = new Net(halfadder, "sout", Term::External);
    Net *ha_cout = new Net(halfadder, "cout", Term::External);
    Instance *ha_xor2 = new Instance(halfadder, Cell::find("xor2"), "xor2_1");
    Instance *ha_and2 = new Instance(halfadder, Cell::find("and2"), "and2_1");
    halfadder->connect("a", ha_a);
    halfadder->connect("b", ha_b);
    halfadder->connect("sout", ha_sout);
    halfadder->connect("cout", ha_cout);
    ha_xor2->connect("i0", ha_a);
    ha_xor2->connect("i1", ha_b);
    ha_xor2->connect("q", ha_sout);
    ha_and2->connect("i0", ha_a);
    ha_and2->connect("i1", ha_b);
    ha_and2->connect("q", ha_cout);
    halfadder->toXml(cout);

    cout << "\nConstruction du modele <fulladder>." << endl;
    Cell *fulladder = new Cell("fulladder");
    new Term(fulladder, "a", Term::In);
    new Term(fulladder, "b", Term::In);
    new Term(fulladder, "sout", Term::Out);
    new Term(fulladder, "cout", Term::Out);
    Net *fa_a = new Net(fulladder, "a", Term::External);
    Net *fa_b = new Net(fulladder, "b", Term::External);
    Net *fa_sout = new Net(fulladder, "sout", Term::External);
    Net *fa_cout = new Net(fulladder, "cout", Term::External);
    Net *fa_cin = new Net(fulladder, "cin", Term::External);

    Net *fa_half1_sout = new Net(fulladder, "half1_sout", Term::Internal);
    Net *fa_half1_cout = new Net(fulladder, "half1_cout", Term::Internal);
    Net *fa_half1_a = new Net(fulladder, "half1_a", Term::Internal);
    Net *fa_half1_b = new Net(fulladder, "half1_b", Term::Internal);

    Net *fa_half2_sout = new Net(fulladder, "half2_sout", Term::Internal);
    Net *fa_half2_cout = new Net(fulladder, "half2_cout", Term::Internal);
    Net *fa_half2_a = new Net(fulladder, "half2_a", Term::Internal);
    Net *fa_half2_b = new Net(fulladder, "half2_b", Term::Internal);

    Net *fa_or2_cout = new Net(fulladder, "or2_q", Term::Internal);
    Net *fa_or2_i0 = new Net(fulladder, "or2_i0", Term::Internal);
    Net *fa_or2_i1 = new Net(fulladder, "or2_i1", Term::Internal);

    Instance *fa_half1 = new Instance(fulladder, Cell::find("halfadder"), "halfadder_1");
    Instance *fa_half2 = new Instance(fulladder, Cell::find("halfadder"), "halfadder_2");
    Instance *fa_or2 = new Instance(fulladder, Cell::find("or2"), "or2_1");

    fulladder->connect("a", fa_a);
    fulladder->connect("b", fa_b);
    fulladder->connect("sout", fa_sout);
    fulladder->connect("cout", fa_or2_cout);
    fulladder->connect("cin", fa_cin);

    fa_half1->connect("a", fa_a);
    fa_half1->connect("b", fa_b);
    fa_half1->connect("sout", fa_half2_b);
    fa_half1->connect("cout", fa_or2_i1);

    fa_half2->connect("a", fa_cin);
    // fa_half2->connect("b", fa_half2_b); pas besoin d'etre reciproque surement
    fa_half2->connect("sout", fa_sout);
    fa_half2->connect("cout", fa_or2_i0);
    fulladder->toXml(cout);

    return 0;
}
*/
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
    halfadder->toXml(cout);

    return 0;
}