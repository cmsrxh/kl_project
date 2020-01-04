#ifndef KL_RES_VOICE_SEARCH_ALL_H
#define KL_RES_VOICE_SEARCH_ALL_H

#include "kl_save_object.h"

namespace kl
{
class VoiceSearchAll : public SaveObject<SearchItem>
{
public:
    VoiceSearchAll(const ByteString &query);
    ~VoiceSearchAll();

    NetUrl &genQueryUrl();

    void profile();

    void genResult(const char *data, unsigned long size);

private:
    ByteString mQuery;
};
}

#endif // KL_RES_VOICE_SEARCH_ALL_H
