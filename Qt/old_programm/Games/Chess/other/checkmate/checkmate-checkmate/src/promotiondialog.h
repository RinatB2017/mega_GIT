/************************************************************************
 * Copyright  2009  Miquel Canes Gonzalez  miquelcanes@gmail.com        *
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

#ifndef PROMOTIONDIALOG_H
#define PROMOTIONDIALOG_H

#include "chessutilities.h"

#include <QDialog>

#include <QPoint>
#include <QChar>
#include <QGridLayout>

class PromotionDialog : public KDialog 
{
public:
    PromotionDialog(const QPoint& pos, bool white, QWidget* parent = 0, Qt::WFlags flags = 0);
    QString promotion();

private slots:
    void setPromotion();

private:
    void setPieceButton(ChessUtilities::Pieces type, int row, int col, const QString& id);

    QPoint m_pos;
    QGridLayout* m_layout;
    QString promotionC;
};

#endif // PROMOTIONDIALOG_H
