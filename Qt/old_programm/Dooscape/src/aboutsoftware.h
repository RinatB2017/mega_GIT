#ifndef ABOUTSOFTWARE_H
#define ABOUTSOFTWARE_H

#include "panel.h"

/*
    This file is part of Dooscape.

    Dooscape is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Dooscape is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Dooscape.  If not, see <http://www.gnu.org/licenses/>.
*/

class AboutSoftware : public Panel
{
    Q_OBJECT
    public:
        explicit AboutSoftware(QWidget *parent = 0);

    signals:

    public slots:

};

#endif // ABOUTSOFTWARE_H
