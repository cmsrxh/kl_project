#ifndef KL_RES_SUGGESTION_WORD_H
#define KL_RES_SUGGESTION_WORD_H

#include "kl_save_object.h"

namespace kl
{
class SuggestionWord : public SaveObject<SuggestItem>
{
public:
    SuggestionWord(const ByteString &suggestWord, int limit = 10);
    ~SuggestionWord();

    NetUrl &genQueryUrl();

    void profile();

    int genResult(NetBuffer *data);

private:
    ByteString mLimit;
    ByteString mSuggestWord;
};
}

#endif // KL_RES_SUGGESTION_WORD_H
