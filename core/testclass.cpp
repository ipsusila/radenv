#include "testclass.h"
#include <QSharedData>

static TestClass __globalInstance;

class TestClassData : public QSharedData {
public:
    int value;
    TestClassData() : value(0) {}

    inline void inc(int dx)
    {
        value += dx;
    }
};

TestClass::TestClass() : data(new TestClassData)
{
}

TestClass::TestClass(const TestClass &rhs) : data(rhs.data)
{
}

TestClass &TestClass::operator=(const TestClass &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

TestClass::~TestClass()
{
}

void TestClass::increment(int dx)
{
    data->inc(dx);
}

int TestClass::currentValue() const
{
    return data->value;
}

TestClass * TestClass::instance()
{
    return &__globalInstance;
}
