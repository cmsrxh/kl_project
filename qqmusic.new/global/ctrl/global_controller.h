#ifndef GLOBAL_CONTROLLER_H
#define GLOBAL_CONTROLLER_H

#include <QObject>

class QTimer;
class GlobalControls : public QObject
{
    Q_OBJECT

private:
    explicit GlobalControls(QObject *parent = 0);

public:
    ~GlobalControls();

    static GlobalControls *instance()
    {
        static GlobalControls *i = [] { return new GlobalControls; } ();
        return i;
    }

Q_SIGNALS:

public Q_SLOTS:
    void onPageChanaged(int prevPage, int newPage);

private:

};

#endif // GLOBAL_CONTROLLER_H
