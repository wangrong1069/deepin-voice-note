#include "loadiconsworker.h"
#include "common/vnotedatamanager.h"

#include <QPixmap>
#include <QPainter>

QVector<QPixmap> VNoteDataManager::m_defaultIcons[IconsType::MaxIconsType];
QReadWriteLock VNoteDataManager::m_iconLock;

LoadIconsWorker::LoadIconsWorker(QObject *parent)
    : QObject(parent)
    , QRunnable ()
{
    //Hold the lock at constructor, this can void
    //other thread acess the icons before that being
    //loaded.
    VNoteDataManager::m_iconLock.lockForWrite();
}

QPixmap LoadIconsWorker::greyPix(QPixmap pix)
{
    QPixmap temp(pix.size());
    temp.fill(Qt::transparent);

    QPainter iconPainer(&temp);
    iconPainer.setCompositionMode(QPainter::CompositionMode_Source);
    iconPainer.drawPixmap(0, 0, pix);
    iconPainer.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    iconPainer.fillRect(temp.rect(), QColor(0, 0, 0, 64));
    iconPainer.end();

    return temp;
}

void LoadIconsWorker::run()
{
    QString defaultIconPathFmt(":/icons/deepin/builtin/default_folder_icons/%1.svg");

    for (int i=0; i<DEFAULTICONS_COUNT; i++) {
        QString iconPath = defaultIconPathFmt.arg(i+1);
        QPixmap bitmap(iconPath);
        VNoteDataManager::m_defaultIcons[IconsType::DefaultIcon].push_back(bitmap);
        VNoteDataManager::m_defaultIcons[IconsType::DefaultGrayIcon].push_back(greyPix(bitmap));
    }

    VNoteDataManager::m_iconLock.unlock();
}
