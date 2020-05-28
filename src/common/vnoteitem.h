#ifndef VNOTEITEM_H
#define VNOTEITEM_H

#include "common/datatypedef.h"

#include <QDateTime>
#include <QTextCursor>
#include <QTextDocumentFragment>
#include <QTextDocument>

#include <DWidget>

DWIDGET_USE_NAMESPACE

struct VNoteBlock;
struct VNoteFolder;

struct VNoteItem {
public:
    VNoteItem();

    bool isValid();
    void delNoteData();
    bool search(const QString &keyword);
    void setMetadata(const QVariant &meta);
    void setFolder(VNoteFolder* folder);
    VNoteFolder* folder() const;

    enum {
        INVALID_ID = -1
    };

    enum State {
        Normal,
        Deleted,
    };

    enum VNOTE_TYPE {
        VNT_Text = 0,
        VNT_Voice,
        VNT_Mixed,
    };

    qint32 noteId {INVALID_ID};
    qint64 folderId {INVALID_ID};
    qint32 noteType {VNOTE_TYPE::VNT_Text};
    qint32 noteState {State::Normal};

    QString noteTitle;

    QDateTime createTime;
    QDateTime modifyTime;
    QDateTime deleteTime;

    QVariant& metaDataRef();
    const QVariant& metaDataConstRef() const;

    qint32 &maxVoiceIdRef();
    qint32 voiceMaxId() const;

    VNOTE_DATAS datas;

    VNoteBlock *newBlock(int type);
    void addBlock(VNoteBlock *block);
    void addBlock(VNoteBlock *before, VNoteBlock *block);
    void delBlock(VNoteBlock *block);

    bool haveVoice() const;
    bool haveText() const;

    qint32 voiceCount() const;
protected:
    QVariant metaData;

    //Use to make default voice name
    //auto increment.
    qint32 maxVoiceId {0};

    //TODO:
    //    Don't used now ,Used for quick lookup.
    VNoteFolder* ownFolder {nullptr};

    friend QDebug &operator << (QDebug &out, VNoteItem &noteItem);
};

struct VNTextBlock;
struct VNVoiceBlock;

struct VNoteBlock {
    enum {
        InValid,
        Text,
        Voice
    };

    virtual ~VNoteBlock();

    virtual void releaseSpecificData() = 0;

    qint32 getType();

    qint32  blockType {InValid};
    /*
     * Comment:
     *      For text block, store the content of the text,
     * for voice block, store the result of audio to text.
     * */
    QString blockText;

    union {
        VNoteBlock   *ptrBlock;
        VNTextBlock  *ptrText;
        VNVoiceBlock *ptrVoice;
    };
protected:
    VNoteBlock(qint32 type = InValid);
    VNoteBlock(const VNoteBlock &);
    const VNoteBlock &operator=(const VNoteBlock &);
};

struct VNTextBlock  : public VNoteBlock {
    VNTextBlock();
    virtual ~VNTextBlock() override;
    virtual void releaseSpecificData() override;
};

struct VNVoiceBlock : public VNoteBlock {
    VNVoiceBlock();
    virtual ~VNVoiceBlock() override;
    virtual void releaseSpecificData() override;

    QString voicePath;
    qint64  voiceSize;
    QString voiceTitle;
    bool    state {false};
    QDateTime createTime;

};

class DetailItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DetailItemWidget(QWidget *parent = nullptr);
    virtual void        updateSearchKey(QString searchKey);
    virtual void        pasteText();
    virtual bool        isTextContainsPos(const QPoint &globalPos);

    virtual VNoteBlock *getNoteBlock() = 0;
    virtual QTextCursor getTextCursor() = 0;
    virtual void        setTextCursor(const QTextCursor &cursor) = 0;
    virtual bool        textIsEmpty() = 0;
    virtual QRect       getCursorRect() = 0;
    virtual void        setFocus() = 0;
    virtual bool        hasFocus() = 0;
    //选中相关
    virtual void selectText(const QPoint &globalPos, QTextCursor::MoveOperation op) = 0;
    virtual void selectText(QTextCursor::MoveOperation op) = 0;
    virtual void removeSelectText() = 0;
    virtual void selectAllText() = 0;
    virtual void clearSelection() = 0;
    virtual bool hasSelection() = 0;
    virtual bool isSelectAll() = 0;
    virtual QTextDocumentFragment getSelectFragment() = 0;
    virtual QTextDocument* getTextDocument() = 0;
};

#endif // VNOTEITEM_H