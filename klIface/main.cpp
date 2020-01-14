/**
 * @file:    main.cpp
 * @author:  陈猛
 * @em:      chen-meng@hongjing-wh.com
 * @date:    2020年01月14日 星期二 17时16分47秒
 * @details: 
 ************************************************************************/

#include <events/common_log.h>
#include "kl_client_call.h"

int main(int , char **)
{
    KLClientCall::instance()->initClientIface(nullptr);

    char cmd[256];

    while (1)
    {
        scanf("%s", cmd);

        if (0 == strcmp("search", cmd))
        {
            printf("Please Input key word\n");

            scanf("%s", cmd);

            KLClientCall::instance()->searchKeyWord(cmd);
        }
    }


    return 0;
}

