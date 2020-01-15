#ifndef KL_RES_VOICE_SEARCH_ALL_H
#define KL_RES_VOICE_SEARCH_ALL_H

#include "kl_save_object.h"

namespace kl
{
class VoiceSearchAll;
typedef void (*SearchNotify)(VoiceSearchAll *obj, bool res);
class VoiceSearchAll : public SaveObject<SearchItem>
{
public:
    VoiceSearchAll(const ByteString &query);
    ~VoiceSearchAll();

    NetUrl &genQueryUrl();

    void profile();

    void search(const ByteString &query);

    void genResult(NetBuffer *data);
    void loadErrorInfo(int type, const char *str);

    NetBuffer *pData() const
    {
        return m_pData;
    }

    void setNofity(const SearchNotify &value)
    {
        nofity = value;
    }

private:
    NetBuffer      *m_pData;
    ByteString      mQuery;
    SearchNotify    nofity;
};
}

#endif // KL_RES_VOICE_SEARCH_ALL_H
