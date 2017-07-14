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

#include "promotiondialog.h"
#include "themeprovider.h"

#include <KLocale>

#include <QPushButton>
#include <QIcon>

//FIXME: Theme
PromotionDialog::PromotionDialog(const QPoint& pos, bool white, QWidget* parent, Qt::WFlags flags): KDialog(parent, flags)
{
	m_pos = pos;
	setWindowTitle(i18n("Pawn promotion") + " " + ChessUtilities::MatrixUnitToAlgebraic(pos));
	setModal(true);

	m_layout = new QGridLayout(this);
	setPieceButton((white)?ChessUtilities::WQUEEN : ChessUtilities::BQUEEN, 0, 0, "q");
	setPieceButton((white)?ChessUtilities::WROCK : ChessUtilities::BROCK, 0, 1, "r");
	setPieceButton((white)?ChessUtilities::WKNIGHT : ChessUtilities::BKNIGHT, 1, 0, "n");
	setPieceButton((white)?ChessUtilities::WBISHOP : ChessUtilities::BBISHOP, 1, 1, "b");
	setLayout(m_layout);
}


void PromotionDialog::setPieceButton(ChessUtilities::Pieces type, int row, int col, const QString& id)
{
	QPushButton* button = new QPushButton(QIcon(ThemeProvider::self()->pieceToPixmap(type, 100)), "");
	m_layout->addWidget(button, row, col);
	button->setAccessibleName(id);
	connect(button, SIGNAL(clicked(bool)), this, SLOT(setPromotion()));
}


void PromotionDialog::setPromotion()
{
	QPushButton* button = (QPushButton*) sender();
	promotionC = button->accessibleName();
	this->done(0);
}


QString PromotionDialog::promotion()
{
	return promotionC;
}

