#ifndef TEST_DATA_LOAD_H
#define TEST_DATA_LOAD_H

#include "load_item.h"

class TestDataLoad : public LoadItem
{
public:
    TestDataLoad();
    ~TestDataLoad();

    void start();

    void allDataObtain(NetBuffer */*data*/);

    void errorInfo(int type, const char *str);
};

#endif // TEST_DATA_LOAD_H
