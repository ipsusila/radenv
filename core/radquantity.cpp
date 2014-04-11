#include "radglobal.h"

/** empty variable */
namespace Rad {

const char LatinEndLine = '\n';

const Quantity EmptyQuantity = {0, Rad::Real, 0, 0, 0, 0, "","","","","",""};
const Quantity ScalarInput =
    {std::numeric_limits<unsigned int>::max(), Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "X","X",QObject::tr("Input"),"","","Arbitrary numeric value"};
const Quantity ScalarInput1 =
    {std::numeric_limits<unsigned int>::max() - 1, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "X1","X1",QObject::tr("Input 1"),"","","Arbitrary numeric value"};
const Quantity ScalarInput2 =
    {std::numeric_limits<unsigned int>::max() - 2, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "X2","X2",QObject::tr("Input 2"),"","","Arbitrary numeric value"};
const Quantity ScalarInput3 =
    {std::numeric_limits<unsigned int>::max() - 3, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "X3","X3",QObject::tr("Input 3"),"","","Arbitrary numeric value"};
const Quantity ScalarInput4 =
    {std::numeric_limits<unsigned int>::max() - 4, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "X4","X4",QObject::tr("Input 4"),"","","Arbitrary numeric value"};
const Quantity ScalarInput5 =
    {std::numeric_limits<unsigned int>::max() - 5, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "X5","X5",QObject::tr("Input 5"),"","","Arbitrary numeric value"};
const Quantity ScalarInput6 =
    {std::numeric_limits<unsigned int>::max() - 6, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "X6","X6",QObject::tr("Input 6"),"","","Arbitrary numeric value"};
const Quantity ScalarInput7 =
    {std::numeric_limits<unsigned int>::max() - 7, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "X7","X7",QObject::tr("Input 7"),"","","Arbitrary numeric value"};
const Quantity ScalarInput8 =
    {std::numeric_limits<unsigned int>::max() - 8, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "X8","X8",QObject::tr("Input 8"),"","","Arbitrary numeric value"};
const Quantity ScalarInput9 =
    {std::numeric_limits<unsigned int>::max() - 9, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "X9","X9",QObject::tr("Input 9"),"","","Arbitrary numeric value"};

const Quantity ScalarOutput =
    {std::numeric_limits<unsigned int>::max() - 10, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "Y","Y",QObject::tr("Output"),"","","Arbitrary numeric value"};
const Quantity ScalarOutput1 =
    {std::numeric_limits<unsigned int>::max() - 11, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "Y1","Y1",QObject::tr("Output 1"),"","","Arbitrary numeric value"};
const Quantity ScalarOutput2 =
    {std::numeric_limits<unsigned int>::max() - 12, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "Y2","Y2",QObject::tr("Output 2"),"","","Arbitrary numeric value"};
const Quantity ScalarOutput3 =
    {std::numeric_limits<unsigned int>::max() - 13, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "Y3","Y3",QObject::tr("Output 3"),"","","Arbitrary numeric value"};
const Quantity ScalarOutput4 =
    {std::numeric_limits<unsigned int>::max() - 14, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "Y4","Y4",QObject::tr("Output 4"),"","","Arbitrary numeric value"};
const Quantity ScalarOutput5 =
    {std::numeric_limits<unsigned int>::max() - 15, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "Y5","Y5",QObject::tr("Output 5"),"","","Arbitrary numeric value"};

const Quantity CommentQuantity =
    {std::numeric_limits<unsigned int>::max() - 16, Rad::Comment, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "C","C",QObject::tr("Comment"),"","","Arbitrary comment"};

const Quantity NameQuantity =
    {std::numeric_limits<unsigned int>::max() - 17, Rad::Text, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "","",QObject::tr("Name"),"","","Arbritrary name"};

const Quantity UseDefaultValue =
    {std::numeric_limits<unsigned int>::max() - 18, Rad::Boolean, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "ud","ud",QObject::tr("Use predefined default value(s)"),"","","Use default value defined in software."};

const Quantity LongCommentQuantity =
    {std::numeric_limits<unsigned int>::max() - 19, Rad::LongComment, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "C","C",QObject::tr("Comment"),"","","Full row arbitrary comment"};

const Quantity UseDefaultValue2 =
    {std::numeric_limits<unsigned int>::max() - 20, Rad::Boolean, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "ud","ud",QObject::tr("Use predefined default value(s)"),"","","Use default value defined in software."};


}
