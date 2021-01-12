#include <QDebug>
#include "load_model_base.h"

LoadModelBase::LoadModelBase()
{

}

LoadModelBase::~LoadModelBase()
{

}

void LoadModelBase::dataPrepare()
{
    Q_EMIT sigLoadOver();
}

void LoadModelBase::errorInfo(int type, const ByteString &str)
{
    Q_EMIT sigLoadFailed(type, QString::fromUtf8(str.string(), str.size()));
}

void LoadModelBase::reset()
{
    beginResetModel();
    endResetModel();
}
