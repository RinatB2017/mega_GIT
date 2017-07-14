#ifndef PROGRESSVIEW_H
#define PROGRESSVIEW_H

#include <QtWidgets>
#include "panel.h"
#include "progressindicator.h"

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

class ProgressView : public QPushButton
{
    Q_OBJECT
    public:
        explicit ProgressView(QWidget *parent = 0);

    signals:
        void closeRequested();

    public slots:
        void setValue(int value);
        void setColor(QString color);

    private:
        QProgressBar *progress;
        QLabel *lblValue;

};

#endif // PROGRESSVIEW_H
