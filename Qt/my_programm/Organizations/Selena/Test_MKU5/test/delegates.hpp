//--------------------------------------------------------------------------------
#ifndef _DELEGATES_H
#define _DELEGATES_H
//--------------------------------------------------------------------------------
#include <QItemDelegate>
#include <QWidget>
#include <QLineEdit>
#include <QIntValidator>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QChar>
#include <QTimeEdit>
//--------------------------------------------------------------------------------
class FrqEditDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    FrqEditDelegate(QObject* parent = 0) : QItemDelegate(parent)
    {
    }
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex &) const
    {
        QLineEdit* le = new QLineEdit(parent);
        le->setInputMask("99 999 99;_");
        return le;
    }
    void setEditorData(QWidget* editor, const QModelIndex& index) const
    {
        QString str = index.model()->data(index, Qt::DisplayRole).toString();
        QLineEdit* le = static_cast<QLineEdit*>(editor);
        le->setText(str);
    }
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
    {
        QLineEdit* le = static_cast<QLineEdit*>(editor);
        QString str = le->text();
        model->setData(index, str, Qt::EditRole);
    }
};
//--------------------------------------------------------------------------------
class ComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ComboBoxDelegate(const QStringList& list, QObject* parent = 0) : QItemDelegate(parent)
    {
        items = QStringList(list);
    }
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex &) const
    {
        QComboBox* cb = new QComboBox(parent);
        cb->addItems(items);
        return cb;
    }
    void setEditorData(QWidget* editor, const QModelIndex& index) const
    {
        QString str = index.model()->data(index, Qt::DisplayRole).toString();
        QComboBox* cb = static_cast<QComboBox*>(editor);
        cb->setEditText(str);
    }
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
    {
        QComboBox* cb = static_cast<QComboBox*>(editor);
        QString str = cb->currentText();
        model->setData(index, str, Qt::EditRole);
    }

private:
    QStringList items;
};
//--------------------------------------------------------------------------------
class IntEditDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    IntEditDelegate(int min_val, int max_val, QObject* parent = 0) : QItemDelegate(parent)
    {
        min = min_val;
        max = max_val;
    }

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex &) const
    {
        QSpinBox* spin = new QSpinBox(parent);
        spin->setRange(min, max);
        return spin;
    }
    
    void setEditorData(QWidget* editor, const QModelIndex& index) const
    {
        int val = index.model()->data(index, Qt::DisplayRole).toInt();
        QSpinBox* spin = static_cast<QSpinBox*>(editor);
        spin->setValue(val);
    }

    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
    {
        QSpinBox* spin = static_cast<QSpinBox*>(editor);
        QString str = spin->cleanText();
        model->setData(index, str, Qt::EditRole);
    }

private:
    int min, max;
};
//--------------------------------------------------------------------------------
class DoubleEditDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    DoubleEditDelegate(double min_val, double max_val, int dec = 2, QObject* parent = 0) : QItemDelegate(parent)
    {
        min = min_val;
        max = max_val;
        decimals = dec;
    }
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex &) const
    {
        QDoubleSpinBox* spin = new QDoubleSpinBox(parent);
        spin->setRange(min, max);
        spin->setDecimals(decimals);
        return spin;
    }
    
    void setEditorData(QWidget* editor, const QModelIndex& index) const
    {
        double val = index.model()->data(index, Qt::DisplayRole).toDouble();
        QDoubleSpinBox* spin = static_cast<QDoubleSpinBox*>(editor);
        spin->setValue(val);
    }

    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
    {
        QDoubleSpinBox* spin = static_cast<QDoubleSpinBox*>(editor);
        QString str = spin->cleanText();
        model->setData(index, str, Qt::EditRole);
    }

private:
    double min, max;
    int decimals;
};
//--------------------------------------------------------------------------------
class AngleEditDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    AngleEditDelegate(int degree = 0, int minute = 0, int second = 0, QObject* parent = 0) : QItemDelegate(parent)
    {
        degr = degree;
        min = minute;
        sec = second;
        mask = QString("999%1 99' 99\"").arg(QChar(0x00b0));
    }

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex &) const
    {
        QLineEdit *le = new QLineEdit(parent);
        le->setInputMask(mask);
        return le;
    }
    
    void setEditorData(QWidget* editor, const QModelIndex& index) const
    {
        QString val = index.model()->data(index, Qt::DisplayRole).toString();
        QLineEdit* le = static_cast<QLineEdit*>(editor);
        le->setText(val);
        le->setInputMask(mask);
    }

    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
    {
        QLineEdit* le = static_cast<QLineEdit*>(editor);
        QString str = le->text();
        model->setData(index, str, Qt::EditRole);
    }

private:
    int degr, min, sec;
    QString mask;
};
//--------------------------------------------------------------------------------
class TimeEditDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    TimeEditDelegate(QObject* parent = 0) : QItemDelegate(parent)
    {
        format = "hh:mm:ss:z";
    }

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex &) const
    {
        QTimeEdit* tie = new QTimeEdit(parent);
        tie->setDisplayFormat(format);
        return tie;
    }
    
    void setEditorData(QWidget* editor, const QModelIndex& index) const
    {
        QString val = index.model()->data(index, Qt::DisplayRole).toString();
        QTimeEdit* tie = static_cast<QTimeEdit*>(editor);
        tie->setTime(QTime::fromString(val, format));
    }

    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
    {
        QTimeEdit* tie = static_cast<QTimeEdit*>(editor);
        QString str = tie->time().toString(format);
        model->setData(index, str, Qt::EditRole);
    }

private:
    QString format;
    
};
//--------------------------------------------------------------------------------
#endif
