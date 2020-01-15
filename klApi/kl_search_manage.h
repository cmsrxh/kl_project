#ifndef KL_SEARCH_MANAGE_H
#define KL_SEARCH_MANAGE_H

#include <util/byte_string.h>
#include <util/map_table.h>

namespace kl
{
class VoiceSearchAll;
class SearchManage
{
public:
    class SearchStatus
    {
    public:
        SearchStatus()
            : search(nullptr), playState(0)
            , currentIndex(-1)
        {}

        VoiceSearchAll *search;
        int             playState; //空闲=0, 播放=1, 暂停=2, 停止=3
        int             currentIndex;
        ByteString      id;
    };
    static SearchManage *instance()
    {
        static SearchManage i;
        return &i;
    }
    ~SearchManage();

    void searchKeyword(const char *id, const char *keyWord);

    void onSearchResult(VoiceSearchAll *search, bool result);

    void playSearchName(const char *id, const char *name);

    void playSearchIndex(const char *id, int index);

    void playPause(const char *id);

    void playPlaying(const char *id);

    void playNext(const char *id);

    void playPrev(const char *id);

private:
    SearchManage(SearchManage &);
    SearchManage &operator=(SearchManage &);
    SearchManage();

    SearchStatus *getSearchNode(ByteString const &id);

    void playSearchItem(SearchStatus *item, int index);

    ListTable<SearchStatus> mList;
};
}

#endif // KL_SEARCH_MANAGE_H
