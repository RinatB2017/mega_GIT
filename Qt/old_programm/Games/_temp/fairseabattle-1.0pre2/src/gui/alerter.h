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
#ifndef BS_GUI_ALERTER_H
#define BS_GUI_ALERTER_H

#include <QObject>

class Alerter : public QObject {

    Q_OBJECT

private:

public:
    Alerter(QObject * parent = 0);

public slots:
    void invalid_field_setup() const;
    void postpone_rules() const;
    void field_already_fired() const;

    void help() const;
    void about() const;
    void about_qt() const;
};

#endif
