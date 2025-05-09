/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include <qtpropertymanager.h>
#include <qtpropertybrowser.h>
#include <qtvariantproperty.h>
#include <qttreepropertybrowser.h>
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
typedef struct StructProperty
{
    QString name;
    QString object_name;
    int propertyType;
    QtVariantProperty *item;
} StructProperty_t;
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class QToolBar;
class QComboBox;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    virtual ~MainBox();

private slots:
    void choice_test(void);
    void test_0(void);
    void test_1(void);
    void test_2(void);
    void test_3(void);
    void test_4(void);
    void test_5(void);

private:
    struct CMD
    {
        int cmd;
        QString cmd_text;
        void (MainBox::*func)(void);
    };
    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    QPointer<QComboBox> cb_test;
    QList<CMD> commands;

    QList<StructProperty_t> l_property_0;
    QList<StructProperty_t> l_property_1;
    QList<StructProperty_t> l_property_2;

    void add_property(QtVariantPropertyManager *variantManager,
                      QList<StructProperty_t> *l_property,
                      const QString &name,
                      const QString &object_name,
                      int propertyType);

    void init(void);

    void createTestBar(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

protected:
    void closeEvent(QCloseEvent *event);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
