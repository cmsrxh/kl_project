#include <events/common_log.h>
#include "application.h"


int main(int /*argc*/, char**/*argv*/)
{
    Application::init_all(0, "default", "/etc/zlog.conf");
    
    Application::instance()->initialize();

    while (1)
    {
        sleep(10);
    }

    Application::uninit_all();
	return 0;
}
