#ifndef KL_CLIENT_CALL_H
#define KL_CLIENT_CALL_H

class ClientMsgPriserProc;
class ClientIfaceBase;
class KLClientCall
{
public:
    static KLClientCall *instance()
    {
        static KLClientCall i;
        return &i;
    }

    bool initClientIface(ClientMsgPriserProc *);

    void searchKeyWord(const char *str);
private:
    KLClientCall();
    KLClientCall(KLClientCall &);
    KLClientCall &operator=(KLClientCall &);

    ClientIfaceBase *m_pHandler;
};

#endif // KL_CLIENT_CALL_H
