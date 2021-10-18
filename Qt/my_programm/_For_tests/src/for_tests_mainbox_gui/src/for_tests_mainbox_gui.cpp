/*********************************************************************************
**                                                                              **
**     Copyright (C) 2021                                                       **
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
#include "for_tests_mainbox_gui.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
MainBox_GUI::MainBox_GUI(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox_GUI)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox_GUI::~MainBox_GUI()
{
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox_GUI::init(void)
{
    ui->setupUi(this);

    connect_log_signals(ui->controls_widget, this);

    connect(ui->btn_show, &QPushButton::clicked, [this]() {
        emit info(ui->cw_calendar->selectedDate().toString("yyyy.MM.dd"));
    });

#ifdef USE_DOCK_WIDGETS
    QTimer::singleShot(0, [this]{
        MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
        if(mw)
        {
#ifndef NO_MENU
            mw->add_mdi_sorting();
#endif
#ifdef TEST_DOCK
            mw->add_dock_widget("test_dock",
                                "test_dock",
                                Qt::BottomDockWidgetArea,
                                ui->test_frame);
#endif
        }
        load_widgets();
    });
#endif
}
//--------------------------------------------------------------------------------
void MainBox_GUI::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
