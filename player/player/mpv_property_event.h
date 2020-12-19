#ifndef MPV_PROPERTY_EVENT_H
#define MPV_PROPERTY_EVENT_H

#include <mpv/client.h>

typedef void (*EventProperty)(void *);

class MpvPropertyEvent
{
public:
    MpvPropertyEvent(mpv_event_property *prop);

    static void registerProperty(mpv_handle *);

    static void metadataPriser(void *data);
    static void durationPriser(void *data);
    static void timePosPriser(void *data);
    static void cacheDurationPriser(void *data);
    static void cacheTimePosPriser(void *data);
    static void mutePriser(void *data);
    static void volumePriser(void *data);
    static void tracklistPriser(void *data);

};

#endif // MPV_PROPERTY_EVENT_H
