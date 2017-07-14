/*
 * Copyright 2010 Alexey Michurin
 *
 * This file is part of FairSeaBattle.
 *
 * FairSeaBattle is free software: you can redistribute
 * it and/or modify it under the terms of the
 * GNU General Public License as published by
 * the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * FairSeaBattle is distributed in the hope that
 * it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the
 * GNU General Public License along with FairSeaBattle.
 * If not, see <http://www.gnu.org/licenses/>.
 */
#include "alerter.h"
#include <QMessageBox>

Alerter::Alerter(QObject * parent) :
    QObject(parent)
{
}

void
Alerter::invalid_field_setup() const {
    QMessageBox::information(0,
        tr("Error"),
        tr("Field incorrect! Correct it and try again.<br>"
           "Do not forget, you can use <b>autosetup</b>."));
}

void
Alerter::postpone_rules() const {
    QMessageBox::information(0,
        tr("Note"),
        tr("New rules would be appayed at next round.<br>"
           "Start new game to affect new rules."));
}

void
Alerter::field_already_fired() const {
    QMessageBox::information(0,
        tr("Why?"),
        tr("This field is already fired."));
}

void
Alerter::help() const {
    QMessageBox::information(0,
        tr("Help"),
        tr("Sink the computer's fleet."));
}

#define _STR_EXPAND(t) #t
#define STR(t) _STR_EXPAND(t)
#ifndef FAIRSEABATTLEVERSION
#define FAIRSEABATTLEVERSION [unknown version]
#endif

void
Alerter::about() const {
    QMessageBox::information(0,
        tr("About"),
        tr("<h3>FairSeaBattle "
              STR(FAIRSEABATTLEVERSION)
              STR(FAIRSEABATTLEPRERELEASE)
           "</h3>"
           "<p>Copyright (c) 2010 Alexey Michurin</p>"
           "<p>Released under the terms of the"
           " <a href=\"http://www.gnu.org/licenses/\">GNU GPL v3</a></p>"
           "<p>FairSeaBattle is Qt-based battleship type game"
           " you might have played before."
           " It also known as"
           " &laquo;Affonda la flotta&raquo;,"
           " &laquo;Battleboats&raquo;,"
           " &laquo;Broadside&raquo;,"
           " &laquo;Combate Naval&raquo;,"
           " &laquo;Flotta contro flotta&raquo;,"
           " &laquo;Sea Battle&raquo;,"
           " &laquo;Sunk&raquo;,"
           " &laquo;Travel Battleship&raquo;."
           "</p>"
#ifdef FAIRSEABATTLEPRERELEASE
           "<blockquote>"
           "<b>Remember that this versions <u>is not</u> RELEASE yet.</b><br/>"
           "It is PRE-RELEASE versions."
           " In an effort to provide quality software at release time I"
           " will PRE-RELEASE one for bug checking.<br/>"
           "Please submit bugs by sending an e-mail to me:"
           " <a href=\"a.michurin@gmail.com\">a.michurin@gmail.com</a>.<br/>"
           "Thanks a lot!"
           "</blockquote>"
#endif
           "<p>The newest version of FairSeaBattle can be obtained"
           " at the project home page: "
           "<a href=\"http://fairseabattle.googlecode.com/\">"
           "http://fairseabattle.googlecode.com/</a>.</p>"));
}

void
Alerter::about_qt() const {
    QMessageBox::aboutQt(0);
}
