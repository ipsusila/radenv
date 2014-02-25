#include "kelement.h"
#include <QSharedData>

KElement KElement::Empty = KElement(QString(), QString());
KElement KElement::Ac	= KElement("Ac","Actinium");
KElement KElement::Ag	= KElement("Ag","Silver");
KElement KElement::Al	= KElement("Al","Aluminium");
KElement KElement::Am	= KElement("Am","Americium");
KElement KElement::Ar	= KElement("Ar","Argon");
KElement KElement::As	= KElement("As","Arsenic");
KElement KElement::At	= KElement("At","Astatine");
KElement KElement::Au	= KElement("Au","Gold");
KElement KElement::B	= KElement("B","Boron");
KElement KElement::Ba	= KElement("Ba","Barium");
KElement KElement::Be	= KElement("Be","Beryllium");
KElement KElement::Bh	= KElement("Bh","Bohrium");
KElement KElement::Bi	= KElement("Bi","Bismuth");
KElement KElement::Bk	= KElement("Bk","Berkelium");
KElement KElement::Br	= KElement("Br","Bromine");
KElement KElement::C	= KElement("C","Carbon");
KElement KElement::Ca	= KElement("Ca","Calcium");
KElement KElement::Cd	= KElement("Cd","Cadmium");
KElement KElement::Ce	= KElement("Ce","Cerium");
KElement KElement::Cf	= KElement("Cf","Californium");
KElement KElement::Cl	= KElement("Cl","Chlorine");
KElement KElement::Cm	= KElement("Cm","Curium");
KElement KElement::Cn	= KElement("Cn","Copernicium");
KElement KElement::Co	= KElement("Co","Cobalt");
KElement KElement::Cr	= KElement("Cr","Chromium");
KElement KElement::Cs	= KElement("Cs","Caesium");
KElement KElement::Cu	= KElement("Cu","Copper");
KElement KElement::Db	= KElement("Db","Dubnium");
KElement KElement::Ds	= KElement("Ds","Darmstadtium");
KElement KElement::Dy	= KElement("Dy","Dysprosium");
KElement KElement::Er	= KElement("Er","Erbium");
KElement KElement::Es	= KElement("Es","Einsteinium");
KElement KElement::Eu	= KElement("Eu","Europium");
KElement KElement::F	= KElement("F","Fluorine");
KElement KElement::Fe	= KElement("Fe","Iron");
KElement KElement::Fl	= KElement("Fl","Flerovium");
KElement KElement::Fm	= KElement("Fm","Fermium");
KElement KElement::Fr	= KElement("Fr","Francium");
KElement KElement::Ga	= KElement("Ga","Gallium");
KElement KElement::Gd	= KElement("Gd","Gadolinium");
KElement KElement::Ge	= KElement("Ge","Germanium");
KElement KElement::H	= KElement("H","Hydrogen");
KElement KElement::He	= KElement("He","Helium");
KElement KElement::Hf	= KElement("Hf","Hafnium");
KElement KElement::Hg	= KElement("Hg","Mercury");
KElement KElement::Ho	= KElement("Ho","Holmium");
KElement KElement::Hs	= KElement("Hs","Hassium");
KElement KElement::I	= KElement("I","Iodine");
KElement KElement::In	= KElement("In","Indium");
KElement KElement::Ir	= KElement("Ir","Iridium");
KElement KElement::K	= KElement("K","Potassium");
KElement KElement::Kr	= KElement("Kr","Krypton");
KElement KElement::La	= KElement("La","Lanthanum");
KElement KElement::Li	= KElement("Li","Lithium");
KElement KElement::Lr	= KElement("Lr","Lawrencium");
KElement KElement::Lu	= KElement("Lu","Lutetium");
KElement KElement::Lv	= KElement("Lv","Livermorium");
KElement KElement::Md	= KElement("Md","Mendelevium");
KElement KElement::Mg	= KElement("Mg","Magnesium");
KElement KElement::Mn	= KElement("Mn","Manganese");
KElement KElement::Mo	= KElement("Mo","Molybdenum");
KElement KElement::Mt	= KElement("Mt","Meitnerium");
KElement KElement::N	= KElement("N","Nitrogen");
KElement KElement::Na	= KElement("Na","Sodium");
KElement KElement::Nb	= KElement("Nb","Niobium");
KElement KElement::Nd	= KElement("Nd","Neodymium");
KElement KElement::Ne	= KElement("Ne","Neon");
KElement KElement::Ni	= KElement("Ni","Nickel");
KElement KElement::No	= KElement("No","Nobelium");
KElement KElement::Np	= KElement("Np","Neptunium");
KElement KElement::O	= KElement("O","Oxygen");
KElement KElement::Os	= KElement("Os","Osmium");
KElement KElement::P	= KElement("P","Phosphorus");
KElement KElement::Pa	= KElement("Pa","Protactinium");
KElement KElement::Pb	= KElement("Pb","Lead");
KElement KElement::Pd	= KElement("Pd","Palladium");
KElement KElement::Pm	= KElement("Pm","Promethium");
KElement KElement::Po	= KElement("Po","Polonium");
KElement KElement::Pr	= KElement("Pr","Praseodymium");
KElement KElement::Pt	= KElement("Pt","Platinum");
KElement KElement::Pu	= KElement("Pu","Plutonium");
KElement KElement::Ra	= KElement("Ra","Radium");
KElement KElement::Rb	= KElement("Rb","Rubidium");
KElement KElement::Re	= KElement("Re","Rhenium");
KElement KElement::Rf	= KElement("Rf","Rutherfordium");
KElement KElement::Rg	= KElement("Rg","Roentgenium");
KElement KElement::Rh	= KElement("Rh","Rhodium");
KElement KElement::Rn	= KElement("Rn","Radon");
KElement KElement::Ru	= KElement("Ru","Ruthenium");
KElement KElement::S	= KElement("S","Sulfur");
KElement KElement::Sb	= KElement("Sb","Antimony");
KElement KElement::Sc	= KElement("Sc","Scandium");
KElement KElement::Se	= KElement("Se","Selenium");
KElement KElement::Sg	= KElement("Sg","Seaborgium");
KElement KElement::Si	= KElement("Si","Silicon");
KElement KElement::Sm	= KElement("Sm","Samarium");
KElement KElement::Sn	= KElement("Sn","Tin");
KElement KElement::Sr	= KElement("Sr","Strontium");
KElement KElement::Ta	= KElement("Ta","Tantalum");
KElement KElement::Tb	= KElement("Tb","Terbium");
KElement KElement::Tc	= KElement("Tc","Technetium");
KElement KElement::Te	= KElement("Te","Tellurium");
KElement KElement::Th	= KElement("Th","Thorium");
KElement KElement::Ti	= KElement("Ti","Titanium");
KElement KElement::Tl	= KElement("Tl","Thallium");
KElement KElement::Tm	= KElement("Tm","Thulium");
KElement KElement::U	= KElement("U","Uranium");
KElement KElement::Uuo	= KElement("Uuo","Ununoctium");
KElement KElement::Uup	= KElement("Uup","Ununpentium");
KElement KElement::Uus	= KElement("Uus","Ununseptium");
KElement KElement::Uut	= KElement("Uut","Ununtrium");
KElement KElement::V	= KElement("V","Vanadium");
KElement KElement::W	= KElement("W","Tungsten");
KElement KElement::Xe	= KElement("Xe","Xenon");
KElement KElement::Y	= KElement("Y","Yttrium");
KElement KElement::Yb	= KElement("Yb","Ytterbium");
KElement KElement::Zn	= KElement("Zn","Zinc");
KElement KElement::Zr	= KElement("Zr","Zirconium");

/**
 * Known element list.
 */
static KElement * eElements[] = {
    &KElement::Ac,
    &KElement::Ag,
    &KElement::Al,
    &KElement::Am,
    &KElement::Ar,
    &KElement::As,
    &KElement::At,
    &KElement::Au,
    &KElement::B,
    &KElement::Ba,
    &KElement::Be,
    &KElement::Bh,
    &KElement::Bi,
    &KElement::Bk,
    &KElement::Br,
    &KElement::C,
    &KElement::Ca,
    &KElement::Cd,
    &KElement::Ce,
    &KElement::Cf,
    &KElement::Cl,
    &KElement::Cm,
    &KElement::Cn,
    &KElement::Co,
    &KElement::Cr,
    &KElement::Cs,
    &KElement::Cu,
    &KElement::Db,
    &KElement::Ds,
    &KElement::Dy,
    &KElement::Er,
    &KElement::Es,
    &KElement::Eu,
    &KElement::F,
    &KElement::Fe,
    &KElement::Fl,
    &KElement::Fm,
    &KElement::Fr,
    &KElement::Ga,
    &KElement::Gd,
    &KElement::Ge,
    &KElement::H,
    &KElement::He,
    &KElement::Hf,
    &KElement::Hg,
    &KElement::Ho,
    &KElement::Hs,
    &KElement::I,
    &KElement::In,
    &KElement::Ir,
    &KElement::K,
    &KElement::Kr,
    &KElement::La,
    &KElement::Li,
    &KElement::Lr,
    &KElement::Lu,
    &KElement::Lv,
    &KElement::Md,
    &KElement::Mg,
    &KElement::Mn,
    &KElement::Mo,
    &KElement::Mt,
    &KElement::N,
    &KElement::Na,
    &KElement::Nb,
    &KElement::Nd,
    &KElement::Ne,
    &KElement::Ni,
    &KElement::No,
    &KElement::Np,
    &KElement::O,
    &KElement::Os,
    &KElement::P,
    &KElement::Pa,
    &KElement::Pb,
    &KElement::Pd,
    &KElement::Pm,
    &KElement::Po,
    &KElement::Pr,
    &KElement::Pt,
    &KElement::Pu,
    &KElement::Ra,
    &KElement::Rb,
    &KElement::Re,
    &KElement::Rf,
    &KElement::Rg,
    &KElement::Rh,
    &KElement::Rn,
    &KElement::Ru,
    &KElement::S,
    &KElement::Sb,
    &KElement::Sc,
    &KElement::Se,
    &KElement::Sg,
    &KElement::Si,
    &KElement::Sm,
    &KElement::Sn,
    &KElement::Sr,
    &KElement::Ta,
    &KElement::Tb,
    &KElement::Tc,
    &KElement::Te,
    &KElement::Th,
    &KElement::Ti,
    &KElement::Tl,
    &KElement::Tm,
    &KElement::U,
    &KElement::Uuo,
    &KElement::Uup,
    &KElement::Uus,
    &KElement::Uut,
    &KElement::V,
    &KElement::W,
    &KElement::Xe,
    &KElement::Y,
    &KElement::Yb,
    &KElement::Zn,
    &KElement::Zr
};

/**
 * @brief The XElementData class
 */
class KElementPrivate : public QSharedData {
public:
    QString symbol;
    QString name;

    KElementPrivate(const QString& e, const QString n) : symbol(e), name(n) {}
};

KElement::KElement(const QString& e, const QString& n)
    : data(new KElementPrivate(e,n))
{
}

KElement::KElement(const KElement &rhs) : data(rhs.data)
{
}

KElement &KElement::operator=(const KElement &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

KElement::~KElement()
{
}

bool KElement::isValid() const
{
    return !data->name.isEmpty();
}

QString KElement::symbol() const
{
    return data->symbol;
}
QString KElement::name() const
{
    return data->name;
}

bool KElement::operator==(const KElement &other) const
{
    return (data->symbol == other.data->symbol);
}
bool KElement::operator!=(const KElement &other) const
{
    return (data->symbol != other.data->symbol);
}

KElement KElement::fromSymbol(const QString &symbol)
{
    int n = sizeof(eElements) / sizeof(KElement *);
    for (int k = 0; k < n; k++) {
        if (symbol == eElements[k]->symbol())
            return *eElements[k];
    }

    return KElement::Empty;
}
KElement KElement::fromNuclide(const QString &nuclide)
{
    int np = nuclide.indexOf('-');
    if (np >= 0) {
        QString symbol = nuclide.left(np);
        int n = sizeof(eElements) / sizeof(KElement *);
        for (int k = 0; k < n; k++) {
            if (symbol == eElements[k]->symbol())
                return *eElements[k];
        }
    }

    return KElement::Empty;
}
