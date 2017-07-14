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

#include "gameinstance.h"
#include "gnuchessengine.h"

#include <QChar>
#include <QDebug>
#include <QtGlobal>

#include <QLocale>

GameInstance::GameInstance()
{
    m_board = new Board();
    m_engine = new GnuChessEngine();
    m_gameConsole = new GameConsole(m_engine->getReservedWords());
    m_movetable = new MoveTableModel();

    m_moveList = new MoveList();

    m_startFenPosition = QString();

    m_whitePlayer = true;
    m_blackPlayer = false;
    m_instanceBoard = new ChessUtilities::Pieces*[8];
    for(int i = 0; i < 8; i++) {
        m_instanceBoard[i] = new ChessUtilities::Pieces[8];
        for (int j = 0; j < 8; j++){
            m_instanceBoard[i][j] = ChessUtilities::NOPIECE;
        }
    }

    m_difficulty = ChessUtilities::HARD;
    initValues();

    //	connect(this, SIGNAL(setMove(QPoint,QPoint,QString)), m_board, SLOT(setMove(QPoint,QPoint,QString)));
    connect(m_board, SIGNAL(newMove(PieceInstance)), this, SLOT(byNewMove(PieceInstance)));
    connect(this, SIGNAL(sendEngineOutput(QString)), m_gameConsole, SLOT( engineOutput(QString)));
    connect(this, SIGNAL(sendErrorMessage(QString)), m_board, SLOT(showErrorMessage(QString)));
    connect(this, SIGNAL(setColor(bool)), m_board, SLOT(changeColor(bool)));
    //	connect(this, SIGNAL(removePiece(QPoint)), m_board, SLOT(removePiece(QPoint)));
    connect(this, SIGNAL(sendIllegalMove(QString,PieceInstance)), m_board, SLOT(illegalMove(QString,PieceInstance)));
    connect(this, SIGNAL(sendResult(QString,ChessUtilities::Result,QPoint)), m_board, SLOT(showResult(QString,ChessUtilities::Result,QPoint)));
    connect(m_gameConsole, SIGNAL(sendCommand(QString)), this, SLOT(bySendCommand(QString)));

    emit setColor(m_whiteBlackTurn);
}

GameInstance::~GameInstance()
{

}

void GameInstance::start()
{
    if(m_started || m_finished){
        clean();
        if(m_started)
            m_engine->exit();
        m_engine = new GnuChessEngine();
        m_engine->start();
        initValues();
    }else{
        m_engine->start();
    }
    m_started = true;
    m_engine->difficulty(m_difficulty);
    if(!m_whitePlayer || !m_blackPlayer){
        if(m_whitePlayer){
            m_engine->playOtherColor(false);
        }else{
            m_engine->playActualColor(true);
        }
    }else{
        m_engine->manual();
    }
    emit setColor(m_whiteBlackTurn);
}

bool GameInstance::pause()
{
    if(m_started){
        qDebug() << "game instance enable";
        m_board->enabled(false);
        m_engine->pause();
        return true;
    }
    return false;
}

void GameInstance::resume()
{
    if(m_started){
        m_board->enabled(true);
        m_engine->resume(m_whiteBlackTurn, !((m_whitePlayer && m_whiteBlackTurn) || (m_blackPlayer && !m_whiteBlackTurn)));
    }
}

void GameInstance::setPlayers(ChessUtilities::Players type)
{
    if(m_started){
        switch(type){
        case ChessUtilities::BLACKHUMAN:
            if(m_whiteBlackTurn){
                m_engine->playActualColor(true);
            }else{
                m_engine->playOtherColor(true);
            }
            break;
        case ChessUtilities::WHITEHUMAN:
            if(m_whiteBlackTurn){
                m_engine->playOtherColor(false);
            }else{
                m_engine->playActualColor(false);
            }
            break;
        case ChessUtilities::BOTHHUMAN:
            m_engine->manual();
            break;
        default:
            break;
        }
    }
    m_whitePlayer = (type == ChessUtilities::BOTHHUMAN || type == ChessUtilities::WHITEHUMAN);
    m_blackPlayer = (type == ChessUtilities::BOTHHUMAN || type == ChessUtilities::BLACKHUMAN);
}

void GameInstance::setWhite()
{
    qDebug() << "setWhite";
    setPlayers(ChessUtilities::WHITEHUMAN);
}

void GameInstance::setBlack()
{
    qDebug() << "setBlack";
    setPlayers(ChessUtilities::BLACKHUMAN);
}

void GameInstance::setBothHumans()
{
    qDebug() << "setBoth";
    setPlayers(ChessUtilities::BOTHHUMAN);
}


Board* GameInstance::getBoard()
{
    return m_board;
}


GameConsole* GameInstance::getGameConsole()
{
    return m_gameConsole;
}

QAbstractItemModel* GameInstance::getTableModel()
{
    return m_movetable;
}

void GameInstance::byNewMove(const PieceInstance& movement)
{
    if(!m_started)
        start();
    if((m_whiteBlackTurn && !m_whitePlayer) || (!m_whiteBlackTurn && !m_blackPlayer)){
        emit sendErrorMessage(i18n("It's not your turn"));
        return;
    }

    ChessUtilities::Pieces fromPiece = getInstancePiece(movement.from);
    bool white = ChessUtilities::isWhitePiece(fromPiece);
    if(m_whiteBlackTurn && !white){
        emit sendErrorMessage(i18n("It's White's turn"));
        return;
    }

    if(!m_whiteBlackTurn && white){
        emit sendErrorMessage(i18n("It's Black's turn"));
        return;
    }

    //Pawn promotion, ask dialog
    if(movement.promotion.isEmpty() && ((fromPiece == ChessUtilities::WPAWN && movement.to.y() == 1)
                                        || (fromPiece == ChessUtilities::BPAWN && movement.to.y() == 8))){

        m_board->promotion(movement);
        return;
    }

    m_engine->sendMove(matrixToEngine(movement));
}

void GameInstance::bySetMove(const QString& movement)
{
    PieceInstance pinstance = engineToMatrix(movement);
    //FIXME: structure
    ChessUtilities::Pieces toPiece = getInstancePiece(pinstance.to);
    ChessUtilities::Pieces fromPiece = getInstancePiece(pinstance.from);
    Q_ASSERT(fromPiece != ChessUtilities::NOPIECE);
    bool actualEnPassant = false;
    QPoint diff = pinstance.to - pinstance.from;
    Movement move = Movement(pinstance);
    if(fromPiece == ChessUtilities::WPAWN){
        if(diff.y() == -2){
            actualEnPassant = true;
            m_enPassant = QPoint(pinstance.from.x(), 3);
        }else if(!m_enPassant.isNull() && pinstance.to == m_enPassant){
            QPoint pawn = QPoint(m_enPassant.x(), 5);
            PieceInstance ppinstance;
            ppinstance.from = pawn;
            setInstancePiece(pawn, ChessUtilities::NOPIECE);
            m_board->removePiece(pawn);
            move.setAuxMove(ppinstance);
            move.setCapturedPiece(ChessUtilities::BPAWN);
        }
    }else if(fromPiece == ChessUtilities::BPAWN){
        if(diff.y() == 2){
            actualEnPassant = true;
            m_enPassant = QPoint(pinstance.from.x(), 6);
        }else if(!m_enPassant.isNull() && pinstance.to == m_enPassant){
            QPoint pawn = QPoint(m_enPassant.x(), 4);
            PieceInstance ppinstance;
            ppinstance.from = pawn;
            setInstancePiece(pawn, ChessUtilities::NOPIECE);
            m_board->removePiece(pawn);
            move.setAuxMove(ppinstance);
            move.setCapturedPiece(ChessUtilities::WPAWN);
        }
    }else if(toPiece == ChessUtilities::NOPIECE){
        //TODO:Mark castling enpassant promotion
        bool white = ChessUtilities::isWhitePiece(fromPiece);
        //FIXME: CLEAR THAT MESS
        if(white && fromPiece == ChessUtilities::WKING){
            qDebug() << "castling: " << diff;
            if(diff.x() == 2){        //Kingside O-O
                PieceInstance castling;
                castling.from = QPoint(8, 8);
                castling.to = QPoint(6, 8);
                setInstancePiece(castling.to, getInstancePiece(castling.from));
                setInstancePiece(castling.from, ChessUtilities::NOPIECE);
                m_board->setMove(castling);
                move.setAuxMove(castling);
            }else if(diff.x() == -2){ //Queenside O-O-O
                PieceInstance castling;
                castling.from = QPoint(1, 8);
                castling.to = QPoint(4, 8);
                setInstancePiece(castling.to, getInstancePiece(castling.from));
                setInstancePiece(castling.from, ChessUtilities::NOPIECE);
                m_board->setMove(castling);
                move.setAuxMove(castling);
            }
        }else if(fromPiece == ChessUtilities::BKING){
            if(diff.x() == 2){        //Kingside O-O
                PieceInstance castling;
                castling.from = QPoint(8, 1);
                castling.to = QPoint(6, 1);
                setInstancePiece(castling.to, getInstancePiece(castling.from));
                setInstancePiece(castling.from, ChessUtilities::NOPIECE);
                m_board->setMove(castling);
                move.setAuxMove(castling);
            }else if(diff.x() == -2){ //Queenside O-O-O
                PieceInstance castling;
                castling.from = QPoint(1, 1);
                castling.to = QPoint(4, 1);
                setInstancePiece(castling.to, getInstancePiece(castling.from));
                setInstancePiece(castling.from, ChessUtilities::NOPIECE);
                m_board->setMove(castling);
                move.setAuxMove(castling);
            }
        }
    }else{
        PieceInstance capture;
        capture.from = pinstance.to;
        move.setAuxMove(capture);
        move.setCapturedPiece(toPiece);
    }
    m_board->setMove(pinstance);

    if(!pinstance.promotion.isEmpty()){
        PieceInstance promot;
        promot.from = pinstance.to;
        move.setAuxMove(promot);
        move.setCapturedPiece(fromPiece);
    }

    setInstancePiece(pinstance.to, getInstancePiece(pinstance.from));
    setInstancePiece(pinstance.from, ChessUtilities::NOPIECE);

    if(!m_enPassant.isNull())
        m_enPassant = QPoint(0, 0);

    if(m_whiteBlackTurn)
        m_movementsCount++;

    m_whiteBlackTurn = !m_whiteBlackTurn;
    emit setColor(m_whiteBlackTurn);
    //FIXME: Is not correct
    m_movetable->addMove(movement, fromPiece);
    m_moveList->addMove(move);
    //m_movetable->appendRow(&QStandardItem(ChessUtilities::MatrixToCoordenate(from, to, true)));
    printBoard();
}


void GameInstance::engineOutput(const QString& output)
{
    emit sendEngineOutput(output);
}


bool GameInstance::getFENPosition(const QString& fen)
{
    //TODO:Only support standart FEN. TODO XFEN
    bool validFen;
    QStringList fenSplit = fen.split(" ");
    if(fenSplit.size() == 6){
        QString fenPos = fenSplit.at(0);
        int x = 0;
        int y = 0;
        validFen = true;
        for(int i = 0; i < fenPos.size() && validFen; i++){
            QChar c = fenPos.at(i);
            if(c == '/'){
                y++;
                x = 0;
            }else if(c.isNumber()){
                //TODO: Support 1x boards
                x+= c.digitValue();
            }else if(c.isLower() || c.isUpper()){
                ChessUtilities::Pieces piece = ChessUtilities::FenToPieces(c);
                m_instanceBoard[x][y] = piece;
                m_board->setInitialPiece(piece, QPoint(x+1,y+1));
                x++;
            }
        }

        QString fenTurn = fenSplit.at(1);
        if(fenTurn == "w"){
            m_whiteBlackTurn = true;
        }else if(fenTurn == "b"){
            m_whiteBlackTurn = false;
        }else{
            validFen = false;
        }

        QString castle = fenSplit.at(2);
        for(int i = 0; i < castle.size() && validFen; i++){
            QChar c = castle.at(i);
            if(c == 'K'){
                m_whiteKCastle = true;
            }else if(c == 'Q'){
                m_whiteQCastle = true;
            }else if(c == 'k'){
                m_blackKCastle = true;
            }else if(c == 'q'){
                m_blackQCastle = true;
            }else if(c != '-'){
                validFen = false;
            }
        }

        //TODO: check if value is correcte
        m_enPassant = (fenSplit.at(3) == "-") ? QPoint() : ChessUtilities::AlgebraicUnitToMatrix(fenSplit.at(3));
        //TODO: check if value is correcte
        m_fiftyMoveRuleCount = fenSplit.at(4).toInt();
        //TODO: check if value is correcte
        m_movementsCount = fenSplit.at(5).toInt();

    }else{
        validFen = false;
    }
    return validFen;
}

bool GameInstance::setFENPosition(const QString& fen)
{
    bool ret =  getFENPosition(fen);
    if(ret)
        m_engine->setFENPosition(fen);
    return ret;
}

ChessUtilities::Pieces GameInstance::getInstancePiece(const QPoint& pos)
{
    return m_instanceBoard[pos.x()-1][pos.y()-1];
}


void GameInstance::setInstancePiece(const QPoint& pos, ChessUtilities::Pieces piece)
{
    m_instanceBoard[pos.x()-1][pos.y()-1] = piece;
}

void GameInstance::printBoard()
{
    for (int y = 0 ; y < 8; y++){
        QString line = "";
        for (int x = 0; x < 8; x++){
            line += QString(ChessUtilities::PiecesToFen(m_instanceBoard[x][y])) + " ";
        }
        qDebug() << line;
    }
}

void GameInstance::clean(){
    m_engine->exit();
    m_board->clean();
    m_movetable->clean();
    m_gameConsole->clean();

    disconnect(m_engine, SIGNAL(illegalMove(QString,QString)), this, SLOT(reciveIllegalMove(QString,QString)));
    disconnect(m_engine, SIGNAL(sendOutput(QString)), this, SLOT(engineOutput(QString)));
    disconnect(m_engine, SIGNAL(confirmedMove(QString)), this, SLOT(bySetMove(QString)));
    //	disconnect(this, SIGNAL(newMove(QPoint,QPoint,QString)), m_engine, SLOT(sendMove(QPoint,QPoint,QString)));
    connect(m_engine, SIGNAL(result(QString,ChessUtilities::Result)), this, SLOT(result(QString,ChessUtilities::Result)));
    //FIXME: Support variable size boards
    for(int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            m_instanceBoard[i][j] = ChessUtilities::NOPIECE;
}


bool GameInstance::initValues()
{
    m_whiteBlackTurn = true;
    m_whiteKCastle = false;
    m_whiteQCastle = false;
    m_blackKCastle = false;
    m_blackQCastle = false;
    m_started = false;
    m_active = true;
    m_finished = false;
    m_movementsCount = 0;
    m_enPassant = QPoint(0, 0);
    connect(m_engine, SIGNAL(illegalMove(QString,QString)), this, SLOT(reciveIllegalMove(QString,QString)));
    connect(m_engine, SIGNAL(sendOutput(QString)), this, SLOT(engineOutput(QString)));
    connect(m_engine, SIGNAL(confirmedMove(QString)), this, SLOT(bySetMove(QString)));
    //	connect(this, SIGNAL(newMove(QPoint,QPoint,QString)), m_engine, SLOT(sendMove(QPoint,QPoint,QString)));
    connect(m_engine, SIGNAL(result(QString,ChessUtilities::Result)), this, SLOT(result(QString,ChessUtilities::Result)));
    //FIXME!!!
    if(m_startFenPosition.isEmpty())
        getFENPosition(m_engine->getStartPosition());
    else {
        setFENPosition(m_startFenPosition);
    }
}

void GameInstance::undo()
{
    if((!m_whiteBlackTurn && !m_blackPlayer) || (m_whiteBlackTurn && !m_whitePlayer)){
        emit sendErrorMessage(i18n("It's Computers's turn"));
        return;
    }
    m_engine->undo(m_whiteBlackTurn, (m_whiteBlackTurn && !m_blackPlayer) || (!m_whiteBlackTurn && !m_whitePlayer));
    m_movetable->removeLastMove();
    undoMovement();
    if(!m_whitePlayer || !m_blackPlayer){
        m_movetable->removeLastMove();
        undoMovement();
    }else{
        m_whiteBlackTurn = !m_whiteBlackTurn;
        emit setColor(m_whiteBlackTurn);
    }
    printBoard();
}

void GameInstance::exit()
{
    m_engine->exit();
}

void GameInstance::undoMovement()
{
    Movement move = m_moveList->getMove();
    PieceInstance pointsCapture = move.auxMove();
    ChessUtilities::Pieces piece = move.capturedPiece();
    bool capture = pointsCapture.to.isNull();
    if(!capture){
        m_board->setMove(pointsCapture.swap());
        setInstancePiece(pointsCapture.from, getInstancePiece(pointsCapture.to));
        setInstancePiece(pointsCapture.to, ChessUtilities::NOPIECE);
    }

    PieceInstance points = move.move();
    m_board->setMove(points.swap());
    setInstancePiece(points.from, getInstancePiece(points.to));
    setInstancePiece(points.to, ChessUtilities::NOPIECE);

    if(capture && piece != ChessUtilities::NOPIECE){
        m_board->setInitialPiece(piece, pointsCapture.from);
        setInstancePiece(pointsCapture.from, piece);
    }
}

void GameInstance::result(const QString& message, ChessUtilities::Result res)
{
    m_finished = true;
    QList<QPoint>* list = findPieces(m_whiteBlackTurn?ChessUtilities::WKING:ChessUtilities::BKING);
    Q_ASSERT(list->size() != 1);
    emit sendResult(message, res, list->takeFirst());
    delete list;
}

void GameInstance::changeDifficulty(ChessUtilities::Difficulty difficulty)
{
    m_difficulty = difficulty;
    if(m_started){
        m_engine->difficulty(difficulty);
    }
}

void GameInstance::setStartFenPosition(const QString& fen)
{
    m_startFenPosition = fen;
}

void GameInstance::finished()
{
    m_finished = true;
}

void GameInstance::reciveIllegalMove(const QString& message, const QString& movement)
{
    PieceInstance pinstance = engineToMatrix(movement);
    emit sendIllegalMove(message, pinstance);
}

QList< QPoint >* GameInstance::findPieces(ChessUtilities::Pieces type)
{
    QList<QPoint>* list = new QList<QPoint>();
    for (int y = 0; y < 8; y++){
        for (int x = 0; x < 8; x++){
            if(m_instanceBoard[x][y] == type)
                list->append(QPoint(x+1, y+1));
        }
    }
    return list;
}

void GameInstance::bySendCommand ( const QString& command )
{
    m_engine->sendCommand(command);
}

PieceInstance GameInstance::engineToMatrix ( const QString& movement ) const
{
    if(m_engine->san())
        return PieceInstance();
    else
        return ChessUtilities::CoordenateToMatrix(movement);
}

QString GameInstance::matrixToEngine ( PieceInstance movement ) const
{
    if(m_engine->san())
        return QString();
    else
        return ChessUtilities::MatrixToCoordenate(movement);
}
