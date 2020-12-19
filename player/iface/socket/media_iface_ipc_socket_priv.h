#ifndef MEDIA_IFACE_IPC_SOCKET_COMMON_H
#define MEDIA_IFACE_IPC_SOCKET_COMMON_H

#include "ipc/ipc_sock_common.h"

enum {
    CLIENT_CALL_Start = ipc::CLIENT_CALL_Method_Base,
    CLIENT_CALL_Play,
    CLIENT_CALL_Stop,
    CLIENT_CALL_Pause,
    CLIENT_CALL_Reset,
    CLIENT_CALL_PlayPause,
    CLIENT_CALL_SetFile,
    CLIENT_CALL_setSubtitle,
    CLIENT_CALL_getPlayState,
    CLIENT_CALL_SeekTo,
    CLIENT_CALL_Get_Current_Position,
    CLIENT_CALL_Get_Duration,
    CLIENT_CALL_setPlaybackSpeed,
    CLIENT_CALL_getPlaybackSpeed,
};

#define MEDIA_SERVICE_PORT 50080

#endif // MEDIA_IFACE_IPC_SOCKET_COMMON_H
