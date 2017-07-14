/************************************************************************
 * Copyright  2010  Miquel Canes Gonzalez  miquelcanes@gmail.com        *
 *                                                                      *
 * This program is free software; you can redistribute it and/or        *
 * modify it under the terms of the GNU General Public License as       *
 * published by the Free Software Foundation; either version 2 of       *
 * the License, or (at your option) any later version.                  *
 *                                                                      *
 * This program is distributed in the hope that it will be useful,      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.*
 ************************************************************************/

#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
#include <QLineEdit>

#include <QLocale>

#include "advancegame.h"         
#include "themeprovider.h"

AdvanceGame::AdvanceGame(QWidget* parent): QDialog(parent, flags)
{
    setCaption(i18n("New Game"));
    setModal(true);
    setup();
    setButtons(QDialog::Ok | QDialog::Cancel);
}

QWidget* AdvanceGame::setup()
{  
    bwhite = new QButtonGroup();
    bwhite->setTitle(i18n("White Player"));

    QVBoxLayout* wlayout = new QVBoxLayout(bwhite);

    wengine = new QRadioButton(i18n("Engine"));
    wlayout->addWidget(wengine);

    whuman = new QRadioButton(i18n("Player"));
    whuman->setChecked(true);
    wlayout->addWidget(whuman);

    QLabel* wimage = new QLabel();
    wimage->setPixmap(ThemeProvider::self()->pieceToPixmap(ChessUtilities::WKNIGHT, 80));

    bwhite->setLayout(wlayout);

    bblack = new KButtonGroup();
    bblack->setTitle(i18n("Black Player"));

    QVBoxLayout* blayout = new QVBoxLayout(bblack);

    bengine = new QRadioButton(i18n("Engine"));
    bengine->setChecked(true);
    blayout->addWidget(bengine);

    bhuman = new QRadioButton(i18n("Player"));
    blayout->addWidget(bhuman);

    QLabel* bimage = new QLabel();
    bimage->setPixmap(ThemeProvider::self()->pieceToPixmap(ChessUtilities::BKNIGHT, 80));

    bblack->setLayout(blayout);

    QWidget* main = new QWidget();
    QGridLayout* layout = new QGridLayout(main);

    layout->addWidget(wimage, 0, 0);
    layout->addWidget(bwhite, 0, 1);
    layout->addWidget(bimage, 1, 0);
    layout->addWidget(bblack, 1, 1);

    m_fen = new QLineEdit();
    layout->addWidget(m_fen, 2, 0, 1, -1);

    main->setLayout(layout);
    setMainWidget(main);
}

ChessUtilities::Players AdvanceGame::player()
{
    bool bwe = wengine->isChecked();
    bool bbe = bengine->isChecked();
    if(bwe && bbe){
        return ChessUtilities::BOTHENGINES;
    }else if(!bwe && !bbe){
        return ChessUtilities::BOTHHUMAN;
    }else if(!bwe){
        return ChessUtilities::WHITEHUMAN;
    }else{
        return ChessUtilities::BLACKHUMAN;
    }
}

QString AdvanceGame::initialFenPosition() const
{
    return m_fen->text();
}
