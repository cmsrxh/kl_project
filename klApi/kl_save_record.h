#ifndef KL_SAVE_RECORD_H
#define KL_SAVE_RECORD_H

#include "kl_url/kl_common.h"
#include <util/byte_string.h>
#include "save_manage_object.h"

namespace kl
{
class SaveRecord : public SaveFileObject<RecordItem>
{
public:
    SaveRecord();

    void genResult(const char *data, unsigned long size);

    bool genSaveString(char *& data, unsigned long &len);

    void profile();
private:

};
}


#endif // KL_SAVE_RECORD_H
