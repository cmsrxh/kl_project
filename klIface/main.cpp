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
    client::ClientResult res;
    KLClientCall::instance()->initClientResult(&res);

    char cmd[256];

    while (1)
    {
        scanf("%s", cmd);

        if (0 == strcmp("search", cmd))
        {
            printf("Please Input key word: \n");
            scanf("%s", cmd);
            KLClientCall::instance()->searchKeyWord(cmd);
        } else if (0 == strcmp("index", cmd))
        {
            printf("Please Input index: \n");
            scanf("%s", cmd);
            KLClientCall::instance()->playSearchIndex(atoi(cmd));
        } else if (0 == strcmp("pause", cmd))
        {
            KLClientCall::instance()->playPause();
        } else if (0 == strcmp("play", cmd))
        {
            KLClientCall::instance()->playPlaying();
        } else if (0 == strcmp("next", cmd))
        {
            KLClientCall::instance()->playNext();
        } else if (0 == strcmp("prev", cmd))
        {
            KLClientCall::instance()->playPrev();
        } else
        {
            printf("unkown cmd: %s\n", cmd);
        }
    }


    return 0;
}

