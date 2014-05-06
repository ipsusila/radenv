#ifndef TESTCLASS_H
#define TESTCLASS_H

#include "radglobal.h"

class TestClassData;
class RADENV_API TestClass
{
public:
    TestClass();
    TestClass(const TestClass &);
    TestClass &operator=(const TestClass &);
    ~TestClass();

    void increment(int dx);
    int currentValue() const;

    static TestClass * instance();
    
private:
    QSharedDataPointer<TestClassData> data;
};

#endif // TESTCLASS_H
