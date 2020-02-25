#include <events/common_log.h>
#include <mpv/client.h>
#include "mpv_player_proc.h"
#include "iface/media_notify.h"
#include "mpv_property_event.h"

static struct
{
    const char   *name;
    mpv_format    fmt;
    EventProperty eventProp;
} gRegister[] =
{
//    { "metadata",  MPV_FORMAT_NODE,      MpvPropertyEvent::metadataPriser },
    { "duration",   MPV_FORMAT_DOUBLE,     MpvPropertyEvent::durationPriser },
//    { "time-pos",  MPV_FORMAT_DOUBLE,    MpvPropertyEvent::timePosPriser },
    { "ao-mute",    MPV_FORMAT_INT64,      MpvPropertyEvent::mutePriser },
    { "ao-volume",  MPV_FORMAT_INT64,      MpvPropertyEvent::volumePriser },
//    { "track-list", MPV_FORMAT_OSD_STRING, MpvPropertyEvent::tracklistPriser},
//    { "demuxer-cache-duration", MPV_FORMAT_DOUBLE,    MpvPropertyEvent::cacheDurationPriser },
    { "demuxer-cache-time",     MPV_FORMAT_DOUBLE,    MpvPropertyEvent::cacheTimePosPriser },

};

MpvPropertyEvent::MpvPropertyEvent(mpv_event_property *prop)
{
    for (int i = 0; i < ARRAY_SIZE(gRegister); ++i)
    {
        if (0 == strcmp(gRegister[i].name, prop->name))
        {
            if (gRegister[i].fmt == prop->format)
            {
                gRegister[i].eventProp(prop->data);
            } else
            {
                GEN_Printf(LOG_WARN, "property[%s] format[%d] is invalid.", prop->name, prop->format);
            }
            break;
        }
    }
}

void MpvPropertyEvent::registerProperty(mpv_handle *mpv)
{
    for (int i = 0; i < ARRAY_SIZE(gRegister); ++i)
    {
        mpv_observe_property((mpv_handle *)mpv, 0, gRegister[i].name, gRegister[i].fmt);
    }
}

void MpvPropertyEvent::metadataPriser(void *data)
{
    char *album = (char *)"未知专辑";
    char *author= (char *)"未知作者";

    mpv_node *node = static_cast<mpv_node *>(data);
    GEN_Printf(LOG_DEBUG, "node format: %d", node->format);

    if(node->format == MPV_FORMAT_NODE_MAP)
    {
        mpv_node_list *l = node->u.list;
        for (int i = 0; i < l->num; ++i)
        {
            if ((0 == strcmp(l->keys[i], "album"))
                    && (l->values[i].format == MPV_FORMAT_STRING))
            {
                album = l->values[i].u.string;
            } else if ((0 == strcmp(l->keys[i], "artist"))
                    && (l->values[i].format == MPV_FORMAT_STRING))
            {
                author = l->values[i].u.string;
            }
#if 0
            printf("[%s] = ", l->keys[i]);
            mpv_node *n = &l->values[i];
            switch (n->format)
            {
            case MPV_FORMAT_STRING:
                printf("%s (string)\n", n->u.string);
                break;
            case MPV_FORMAT_FLAG:
                printf("%d (flag)\n", n->u.flag);
                break;
            case MPV_FORMAT_INT64:
                printf("%lld (int64)\n", n->u.int64);
                break;
            case MPV_FORMAT_DOUBLE:
                printf("%g (double)\n", n->u.double_);
                break;
            default:
                printf("format: %d\n", n->format);
                break;
            }
#endif
        }
    }

    MediaNotify::instance()->notifyMusicInfo(author, album);
}

void MpvPropertyEvent::durationPriser(void *data)
{
    double time = *(double *)data;
    MediaNotify::instance()->notifyDuration((int)time);
}

void MpvPropertyEvent::timePosPriser(void *data)
{
    double time = *(double *)data;
    MediaNotify::instance()->notifySlider((int)time, (int)time);
}

void MpvPropertyEvent::cacheDurationPriser(void *data)
{
    double time = *(double *)data;
    MediaNotify::instance()->notifyCacheDuration((int)time);
}

void MpvPropertyEvent::cacheTimePosPriser(void *data)
{
    double time = *(double *)data;
    MediaNotify::instance()->notifyCacheTime((int)time);
}

void MpvPropertyEvent::mutePriser(void *data)
{
    MediaNotify::instance()->notifyMute((bool)(*((int64_t*)data)));
}

void MpvPropertyEvent::volumePriser(void *data)
{
    MediaNotify::instance()->notifyMute((int)(*((int64_t*)data)));
}

void MpvPropertyEvent::tracklistPriser(void *data)
{
    GEN_Printf(LOG_DEBUG, "track list: %s", *((char **)data));
    MPVPlayerProc::instance()->selectSubtileTrack(1);
}
