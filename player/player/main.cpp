#include "events/common_log.h"
#include "application.h"


int main(int /*argc*/, char**/*argv*/)
{
    Application::init_all(INIT_NEED_NOSIGNAL, "player", "/etc/zlog.conf");

    Application::instance()->initialize();

    Application::instance()->runLoop();

    Application::uninit_all();
	return 0;
}
