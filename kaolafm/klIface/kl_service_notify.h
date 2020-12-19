#ifndef KL_SERVICE_NOTIFY_H
#define KL_SERVICE_NOTIFY_H

class ServiceIfaceBase;
class ServiceCmdPriserProc;
class KLServiceNotify
{
public:
    static KLServiceNotify *instance()
    {
        static KLServiceNotify i;
        return &i;
    }

    void initPriser(ServiceCmdPriserProc *priser);

    void notifySearchResult(const char *id, const char *data, int size);
private:
    KLServiceNotify();
    KLServiceNotify(KLServiceNotify &);
    KLServiceNotify &operator=(KLServiceNotify &);

    ServiceIfaceBase *m_pHandler;
};

#endif // KL_SERVICE_NOTIFY_H
