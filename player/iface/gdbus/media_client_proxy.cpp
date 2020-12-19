#include "media_client_proxy.h"



MediaClientProxy::MediaClientProxy()
    : callback(nullptr)
{

}

void MediaClientProxy::start()
{}

void MediaClientProxy::play()
{}

void MediaClientProxy::stop()
{}

void MediaClientProxy::pause()
{}

void MediaClientProxy::reset()
{}

void MediaClientProxy::playPause()
{}

void MediaClientProxy::setFile(const char *file)
{}

void MediaClientProxy::setSubtitle(bool ishave)
{}

void MediaClientProxy::seekTo(long sec, int mode)
{}

int MediaClientProxy::getPlayState()
{
    return -1;
}

int MediaClientProxy::getCurrentPosition()
{
    return 0;
}

int MediaClientProxy::getDuration()
{
    return 0;
}

