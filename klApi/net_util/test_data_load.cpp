#include <events/common_log.h>
#include "test_data_load.h"

TestDataLoad::TestDataLoad()
    : LoadItem(true)
{

}

void TestDataLoad::start()
{
    obtain(NetUrl("www.baidu.com"));
}

void TestDataLoad::allDataObtain(uint8_t *data, size_t len)
{
    GEN_Printf(LOG_DEBUG, "Load Legnth: %ld\n %s", len, (char *)data);
}

void TestDataLoad::errorInfo(int type, const char *str)
{
    GEN_Printf(LOG_DEBUG, "type: %d, error info: %s", type, str);
}
