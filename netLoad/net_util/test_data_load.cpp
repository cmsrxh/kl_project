#include <events/common_log.h>
#include "test_data_load.h"

TestDataLoad::TestDataLoad()
    : LoadItem(true)
{
    GEN_Printf(LOG_DEBUG, "----------------------------");
}

TestDataLoad::~TestDataLoad()
{
    GEN_Printf(LOG_DEBUG, "----------------------------");
}

void TestDataLoad::start()
{
    obtain(NetUrl("www.baidu.com"));
}

void TestDataLoad::allDataObtain(NetBuffer *data)
{
    GEN_Printf(LOG_DEBUG, "Load Legnth: %ld\n %s", data->size(), data->buffer());
}

void TestDataLoad::errorInfo(int type, const char *str)
{
    GEN_Printf(LOG_DEBUG, "type: %d, error info: %s", type, str);
}
