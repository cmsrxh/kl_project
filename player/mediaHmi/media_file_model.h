#ifndef MEDIA_FILE_MODEL_H_
#define MEDIA_FILE_MODEL_H_

#include <QObject>
#include "media_file_internal.h"
#include <QAbstractListModel>
#include <QtQml>

class MediaFileModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int playMode READ playMode WRITE setPlayMode NOTIFY playModeChanged)

    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)

public:
    explicit MediaFileModel(int mediaType, QObject *parent = nullptr);

    enum PeopleRoles
    {
        FileName    = Qt::UserRole,
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
        return mList.size();
        Q_UNUSED(parent);
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void appendList(Media::vector_s &lst, bool haveNext, int startPos);

    bool isEmply()  const
    {
        return mList.empty();
    }
    int  lstSize()  const
    {
        return mList.size();
    }
    bool haveNext() const
    {
        return mHaveNext;
    }
    void setList(Media::vector_s &lst)
    {
        mList.swap(lst);
    }

    int  currentIndex() const;
    void setCurrentIndex(int currentIndex);

    int  playMode() const;
    void setPlayMode(int playMode);

public Q_SLOTS:
    void resetAll();

    void mediaItemClicked(int index);

Q_SIGNALS:
    void currentIndexChanged();
    void updateMediaFile();
    void playModeChanged();

protected:
    QHash<int, QByteArray> roleNames() const
    {
        return roles;
    }

private:
    const int mMediaType;
    int  mCurrentIndex;
    int  mPlayMode;
    int  head, tail;
    bool mHaveNext;

    QHash<int, QByteArray> roles;
    Media::vector_s mList;
};

#endif // MEDIA_FILE_MODEL_H_
