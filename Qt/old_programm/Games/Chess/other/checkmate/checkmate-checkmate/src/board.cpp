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

#include <QPixmapCache>

#include <QPixmap>
#include <QGraphicsSvgItem>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QColor>
#include <QPen>
#include <QBrush>
#include <QRectF>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QGridLayout>
#include <QLabel>

#include <QDialog>
#include <QLocale>

#include "board.h"
#include "themeprovider.h"
#include "promotiondialog.h"

const int Board::PIECEKEY = 0;

Board::Board()
{
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(QGraphicsView::NoDrag);
    setCacheMode(QGraphicsView::CacheNone);
    //setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);
    m_scene = new QGraphicsScene();
    this->setScene(m_scene);
    m_showCoord = false; //TODO:
    m_pieceIsSelected = false;
    ThemeProvider::self()->loadCostumTheme("fantasy");
    //Board

    setColors();

    QGraphicsSvgItem* btheme = ThemeProvider::self()->board();
    m_scene->addItem(btheme);
    initPosition();
    m_whiteMovedFromRect = 0;
    m_whiteMovedToRect = 0;
    m_blackMovedFromRect = 0;
    m_blackMovedToRect = 0;
    m_illegalFromRect = 0;
    m_illegalToRect = 0;
    m_selectedRect = 0;
    m_loserRect = 0;
    drawCoord();
    setVisibleCoord(false);//TODO:
    centerView();
    m_active = true;
}

void Board::clean()
{
    removeMarkRect(m_selectedRect);
    m_selectedRect = 0;
    removeMarkRect(m_whiteMovedFromRect);
    m_whiteMovedFromRect = 0;
    removeMarkRect(m_whiteMovedToRect);
    m_whiteMovedToRect = 0;
    removeMarkRect(m_blackMovedFromRect);
    m_blackMovedFromRect = 0;
    removeMarkRect(m_blackMovedToRect);
    m_blackMovedToRect = 0;
    removeMarkRect(m_illegalFromRect);
    m_illegalFromRect = 0;
    removeMarkRect(m_illegalToRect);
    m_illegalToRect = 0;
    removeMarkRect(m_loserRect);
    m_loserRect = 0;

    QList<QGraphicsItem*> all =  items();
    foreach(QGraphicsItem* piece, all){
        if(piece->data(PIECEKEY) == "piece")
            m_scene->removeItem(piece);
    }
    m_active = true;
}

Piece* Board::newPiece (ChessUtilities::Pieces pieceType)
{
    Piece* piece = ThemeProvider::self()->pieceToSvg(pieceType);
    piece->setSize(100);
    piece->setZValue(1);
    piece->setData(PIECEKEY, "piece");
    m_scene->addItem(piece);
    return piece;
}

void Board::setColors()
{
    m_qpenSelected = QPen(QBrush(Qt::green), 4);
    m_qpenSelected.setJoinStyle(Qt::RoundJoin);
    m_qbrushtransparent = QBrush(Qt::transparent);
    m_qpenlastMove = QPen(QBrush(Qt::cyan), 3);
    m_qpenlastMove.setJoinStyle(Qt::RoundJoin);
    m_qpenIllegalMove = QPen(QBrush(Qt::red), 4);
    m_qpenIllegalMove.setJoinStyle(Qt::RoundJoin);
    m_qpenwinner = QPen(QBrush(Qt::black), 3);
    m_qpenwinner.setJoinStyle(Qt::RoundJoin);
    m_qbrushwinner = QBrush(Qt::darkGreen);
    m_qbrushloser = QBrush(Qt::darkRed);
}

void Board::mousePressEvent (QMouseEvent* event)
{
    if(!m_active)
        return;
    QRectF board(0, 0, 800, 800);
    QPointF pos = mapToScene(event->pos());
    if(m_illegalFromRect != 0 && m_illegalToRect != 0){
        m_scene->removeItem(m_illegalFromRect);
        m_illegalFromRect = 0;
        m_scene->removeItem(m_illegalToRect);
        m_illegalToRect = 0;
    }
    //qDebug() << "pos" << pos.x() << pos.y();
    if(board.contains(pos)){
        if(m_pieceIsSelected){
            //Move piece
            QPoint to = realPosToMatrix(pos);
            if(to == m_selectedPiece){
                m_scene->removeItem(m_selectedRect);
            }else{
                qDebug() << "move Piece -> newMove emit";
                PieceInstance pinstance = PieceInstance();
                pinstance.from = m_selectedPiece;
                pinstance.to = to;
                emit newMove(pinstance);
            }
            m_pieceIsSelected = false;
        }else{
            if(getPiece(pos) != 0){
                m_pieceIsSelected = true;
                m_selectedPiece = realPosToMatrix(pos);
                m_selectedRect = m_scene->addRect(QRectF((m_selectedPiece.x() - 1) * 100, (m_selectedPiece.y() -1) * 100, 98, 98),m_qpenSelected, m_qbrushtransparent);
            }
        }
    }
}

void Board::initPosition()
{   

}

void Board::drawCoord()
{
    QFont font (); //TODO:
    coord = new QGraphicsSimpleTextItem*[16];
    for(int i = 0 ; i < 8; i++){
        QString lettersList = "ABCDEFGH";
        QGraphicsSimpleTextItem* number = m_scene->addSimpleText(QString().setNum(i));
        QRectF boundRect = number->boundingRect();
        number->setPos(-25 - (boundRect.width()/2), 100 * i + 50 - (boundRect.height()/2));
        coord[i] = number;
        QGraphicsSimpleTextItem* letter = m_scene->addSimpleText(lettersList.at(i));
        boundRect = letter->boundingRect();
        letter->setPos(100 * i + 50 - (boundRect.width()/2), -25 - (boundRect.height()/2));
        coord[i+8] = letter;
    }
}

void Board::setVisibleCoord (const bool& visible )
{
    for(int i = 0; i < 16; i++){
        coord[i]->setVisible(visible);
    }
}

void Board::centerView()
{
    if(m_showCoord){
        fitInView(QRect(-50, -50, 850, 850), Qt::KeepAspectRatio);
    }else{
        fitInView(QRect(0 ,0 , 800, 800), Qt::KeepAspectRatio);
    }
}


void Board::resizeEvent ( QResizeEvent* event )
{
    centerView();
}

//Must be a valid movement. Matrix coord
void Board::setMove(const PieceInstance& movement)
{
    qDebug() << "Board::setMove : " << movement.from << movement.to << movement.promotion;
    QGraphicsItem* piece = m_scene->itemAt((movement.from.x() - 1) * 100 + 50, ((movement.from.y() - 1) * 100 + 50));
    //TODO: check errors
    QList<QGraphicsItem*> pieces = m_scene->items(QPointF((movement.from.x() - 1) * 100 + 50, ((movement.from.y() -1 ) * 100 + 50)));
    if(pieces.size() != 2)
        qDebug() << "Board error setMove -> less than 2 items selected";
    QList<QGraphicsItem*> killPieces = m_scene->items(QPointF((movement.to.x() - 1) * 100 + 50, ((movement.to.y() -1 ) * 100 + 50)));
    if(killPieces.size() >= 2){
        //QGraphicsItem* killPiece;
        for(int i = 0; i < killPieces.size(); i++){
            QGraphicsItem* item = killPieces.at(i);
            if(item->data(PIECEKEY) == "piece")
                m_scene->removeItem(item);
        }
        //TODO: NOT SET VISIBLE MOVE TO KILLED PIECES
        //killPiece->setVisible(false);
        //m_scene->removeItem(killPiece);
    }

    if(movement.promotion.isEmpty()){
        qDebug() << "Board::setMove: No promotion";
        piece->setPos((movement.to.x() -1) * 100, (movement.to.y()-1) * 100);
    }else{
        qDebug() << "Board::setMove: promotion";
        m_scene->removeItem(piece);
        ChessUtilities::Pieces promotionPiece = ChessUtilities::FenToPieces((m_whitePlayer)?movement.promotion.at(0):movement.promotion.toUpper().at(0));
        setInitialPiece(promotionPiece, movement.to);
    }
    //TODO: Not remove, only move
    if(m_selectedRect != 0){
        m_scene->removeItem(m_selectedRect);
        m_selectedRect = 0;
    }

    if(m_whitePlayer){
        if(m_whiteMovedToRect == 0){
            m_whiteMovedToRect = m_scene->addRect(QRectF(0,0, 98, 98),m_qpenlastMove, m_qbrushtransparent);
        }//else{
        m_whiteMovedToRect->setPos((movement.to.x() - 1) * 100, (movement.to.y() -1) * 100);
        //}
        if(m_whiteMovedFromRect == 0){
            m_whiteMovedFromRect = m_scene->addRect(0,0, 98, 98,m_qpenlastMove, m_qbrushtransparent);
        }//else{
        m_whiteMovedFromRect->setPos((movement.from.x() - 1) * 100, (movement.from.y() -1) * 100);
        //}
    }else{
        if(m_blackMovedToRect == 0){
            m_blackMovedToRect = m_scene->addRect(QRectF(0, 0, 98, 98),m_qpenlastMove, m_qbrushtransparent);
        }//else{
        m_blackMovedToRect->setPos((movement.to.x() - 1) * 100, (movement.to.y() -1) * 100);
        //}
        if(m_blackMovedFromRect == 0){
            m_blackMovedFromRect = m_scene->addRect(QRectF(0, 0, 98, 98),m_qpenlastMove, m_qbrushtransparent);
        }//else{
        m_blackMovedFromRect->setPos((movement.from.x() - 1) * 100, (movement.from.y() -1) * 100);
        //}
    }
}

QGraphicsItem* Board::getPiece(const QPointF& from)
{
    QList<QGraphicsItem*> sceneObjects = m_scene->items(from);
    for(int i = 0; i < sceneObjects.size(); i++){
        QGraphicsItem* item = sceneObjects.at(i);
        if(item->data(PIECEKEY) == "piece")
            return item;
    }
    return 0;
}

QPoint Board::realPosToMatrix(const QPointF& realPos)
{
    float rx = realPos.x();
    float ry = realPos.y();
    int x = rx/100 + 1;
    int y = ry/100 + 1;
    return QPoint(x, y);
}

void Board::setInitialPiece(ChessUtilities::Pieces pieceType, const QPoint& pos)
{
    Piece* piece = newPiece(pieceType);
    piece->setPos((pos.x() -1) * 100, (pos.y()-1) * 100);
}

void Board::showErrorMessage(const QString& message)
{
    if(m_selectedRect != 0){
        m_scene->removeItem(m_selectedRect);
        m_selectedRect = 0;
    }
    qDebug() << message;
}

void Board::showInfoMessage(const QString& message)
{
    qDebug() << message;
}

void Board::showIllegalMessage(const QString& message)
{
    qDebug() << message;
}

void Board::illegalMove(const QString& message, const PieceInstance& movement)
{
    if(!movement.from.isNull() && !movement.to.isNull()){
        m_scene->removeItem(m_selectedRect);
        m_selectedRect = 0;
        m_illegalFromRect = m_scene->addRect(QRectF((movement.from.x() - 1) * 100, (movement.from.y() -1) * 100, 98, 98),m_qpenIllegalMove, m_qbrushtransparent);
        m_illegalToRect = m_scene->addRect(QRectF((movement.to.x() - 1) * 100, (movement.to.y() -1) * 100, 98, 98),m_qpenIllegalMove, m_qbrushtransparent);
        m_pieceIsSelected = false;
    }
}

//Change color player if True = white
void Board::changeColor(bool white)
{
    m_whitePlayer = white;
}

void Board::promotion(const PieceInstance& movement)
{
    PromotionDialog pd(movement.to, m_whitePlayer, this);
    pd.exec();
    PieceInstance p = PieceInstance(movement);
    p.promotion = QString(pd.promotion());
    emit newMove(p);
}

void Board::removePiece(const QPoint& pos)
{
    QGraphicsItem* piece = getPiece(QPointF((pos.x() - 1) * 100 + 50, ((pos.y() - 1) * 100 + 50)));
    m_scene->removeItem(piece);
}

void Board::removeMarkRect(QGraphicsRectItem* rect)
{
    if(rect != 0){
        m_scene->removeItem(rect);
        // rect = 0;
    }
}

void Board::showResult(const QString& message, ChessUtilities::Result res, const QPoint& kpos)
{
    /* KDialog *dialog = new KDialog();
    QLabel *label = new QLabel();

    switch(res){
    case ChessUtilities::WHITESWIN:
        label->setPixmap(ThemeProvider::self()->pieceToPixmap(ChessUtilities::WKING, 100));
        break;
    case ChessUtilities::BLACKSWIN:
        label->setPixmap(ThemeProvider::self()->pieceToPixmap(ChessUtilities::BKING, 100));
        break;
    case ChessUtilities::DRAW:
        break;
    }

    dialog->setCaption(i18n("Result"));
    dialog->setButtons(KDialog::);

    QGridLayout *layout = new QGridLayout(this);
    if(label != 0)
    layout->addWidget(label);
    layout->addWidget(new QLabel(message));      //FIXME LOCALIZATION
    dialog->setLayout(layout);
    dialog->exec();*/
    showInfoMessage(message);
    if(m_whitePlayer){
        m_blackMovedToRect->setBrush(m_qbrushwinner);
        m_blackMovedToRect->setPen(m_qpenwinner);
    }else{
        m_whiteMovedToRect->setBrush(m_qbrushwinner);
        m_whiteMovedToRect->setPen(m_qpenwinner);
    }
    m_active = false;

    m_loserRect = m_scene->addRect(QRectF((kpos.x() - 1) * 100, (kpos.y() -1) * 100, 100, 100),m_qpenwinner, m_qbrushloser);
}

void Board::enabled(bool enabled)
{
    m_active = enabled;
}
