#include <QTimer>
#include <QFile>
#include <QDebug>
#include <global/manage/pages_manager.h>
#include "global_controller.h"


GlobalControls::GlobalControls(QObject *parent)
    : QObject(parent)
{
    connect(PagesManager::instance(), SIGNAL(pageChanged(int, int)), this, SLOT(onPageChanaged(int,int)));
}

GlobalControls::~GlobalControls()
{

}

void GlobalControls::onPageChanaged(int prevPage, int newPage)
{
    qDebug() << "window page change: " << prevPage << newPage;
}

