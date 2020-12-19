#ifndef KL_CLIENT_RESULT_COMMON_H
#define KL_CLIENT_RESULT_COMMON_H

#include <string>
#include <list>

namespace client {
struct SearchItem
{
    std::string name;
};
class ClientResult
{
public:
    ClientResult();


    virtual void searchResult(std::list<SearchItem> const &lst);
};
}


#endif // KL_CLIENT_RESULT_COMMON_H
