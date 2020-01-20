#ifndef LEFTVIEWDELEGATE_H
#define LEFTVIEWDELEGATE_H

#include "common/vnoteforlder.h"

#include <QPainter>

#include <DApplicationHelper>
#include <DStyledItemDelegate>

DWIDGET_USE_NAMESPACE
class LeftViewDelegate : public DStyledItemDelegate
{
    Q_OBJECT
public:
    LeftViewDelegate(QAbstractItemView *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,  const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

signals:
    void sigFolderRename(VNoteFolder *data) const;

public:
    void handleChangeTheme();
    void updateSearchKeyword(const QRegExp &keyword);
    void resetKeyword();
    void setItemEnable(bool enable);

private:
    QAbstractItemView *m_parentView {nullptr};
    DPalette m_parentPb;

    //Search keyword
    QRegExp m_searchKeyword;
    bool m_enaleItemFlag {true};
};

#endif // LEFTVIEWDELEGATE_H
