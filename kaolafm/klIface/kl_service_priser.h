#ifndef KL_SERVICE_PRISER_H
#define KL_SERVICE_PRISER_H

#include "kl_iface_common.h"

class KLServicePriser : public ServiceCmdPriserProc
{
public:
    KLServicePriser();

    void cmdPriser(IPCDataPackage *in, IPCDataPackage *out);

};

#endif // KL_SERVICE_PRISER_H
