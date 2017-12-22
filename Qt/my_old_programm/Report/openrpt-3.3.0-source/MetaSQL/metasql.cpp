/*
 * OpenRPT report writer and rendering engine
 * Copyright (C) 2001-2011 by OpenMFG, LLC
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * Please contact info@openmfg.com with any questions on this license.
 */

#include <QString>
#include <QRegExp>

#include <QStringList>
#include <QTextStream>
#include <QVariant>
#include <QList>

#include <parameter.h>

#include <QSqlDatabase>

#include "metasql.h"


static const QVariant _trueVariant = QVariant(true);
static const QVariant _falseVariant = QVariant(false);

class MetaSQLBlock;

class MetaSQLQueryPrivate { 
    public:
        MetaSQLQueryPrivate() {
            _valid = false;
            _top = 0;
            _log = QString::null;
            _logger = new QTextStream(&_log, QIODevice::WriteOnly);
        }
        virtual ~MetaSQLQueryPrivate();

        bool isValid() { return _valid; }

        bool parse_query(const QString & query);

        bool populate(XSqlQuery &, const ParameterList &);

        bool _valid;
        MetaSQLBlock * _top;

        QString _log;
        QTextStream * _logger;
};

class MetaSQLInfo {
    public:
        MetaSQLInfo() {
            _paramCount = 0;
        }
        virtual ~MetaSQLInfo() {}

        int _paramCount;
        ParameterList _pList;
};

class MetaSQLOutput {
    public:
        MetaSQLOutput(MetaSQLQueryPrivate * parent) { _parent = parent; }
        virtual ~MetaSQLOutput() { _parent = 0; }

        virtual QString toString(MetaSQLInfo *, const ParameterList &, int * = 0, bool * = 0) = 0;

        MetaSQLQueryPrivate * getParent() { return _parent; }

    protected:
        MetaSQLQueryPrivate * _parent;
};

class MetaSQLString : public MetaSQLOutput {
    public:
        MetaSQLString(MetaSQLQueryPrivate * parent, const QString & str) : MetaSQLOutput(parent), _string(str) {}
        ~MetaSQLString() {}

        virtual QString toString(MetaSQLInfo *, const ParameterList &, int * = 0, bool * = 0) { return _string; }

    protected:
        QString _string;
};

class MetaSQLFunction : public MetaSQLOutput {
    public:
        MetaSQLFunction(MetaSQLQueryPrivate * parent, const QString & func, const QStringList & params)
          : MetaSQLOutput(parent) {
            _valid = 0;
            _nBreaks = 0;
            _noOutput = 0;

            _params = params;

            _func = identifyFunction(func);
            if(_func != FunctionUnknown) {
                switch(_func) {
                    case FunctionValue:
                    case FunctionLiteral:
                    case FunctionExists:
                    case FunctionReExists:
                    case FunctionIsFirst:
                    case FunctionIsLast:
                        _valid = (_params.size() >= 1);
                        break;
                    case FunctionContinue:
                    case FunctionBreak:
                        _valid = 1;
                        _noOutput = 1;
                        if(params.size() >= 1)
                            _nBreaks = params[0].toInt();
                        if(_nBreaks < 1) _nBreaks = 1;
                        break;
                    default:
                        *(_parent->_logger) << "MetaSQLFunction::MetaSQLFunction() encountered unknown Function Type " << (int)_func << "!" << endl;
                };
            }
        }
        ~MetaSQLFunction() {}

        enum Function {
            FunctionUnknown = 0,
            FunctionValue,
            FunctionLiteral,
            FunctionExists,
            FunctionReExists,
            FunctionIsFirst,
            FunctionIsLast,
            FunctionContinue,
            FunctionBreak
        };

        bool isValid() { return _valid; }
        Function type() { return _func; }

        virtual QString toString(MetaSQLInfo * mif, const ParameterList & params, int * nBreaks = 0, bool * isContinue = 0) {
            //qDebug("MetaSQLFunction::toString()");
            QVariant v = toVariant(params, nBreaks, isContinue);
            if(_noOutput) return QString::null;
            if(_func==FunctionLiteral)
              return v.toString();
            QString n = QString(":_%1_").arg(++(mif->_paramCount));
            mif->_pList.append(n, v);
            return n + " ";
        }
        virtual QVariant toVariant(const ParameterList & params, int * nBreaks = 0, bool * isContinue = 0) {
            //qDebug("MetaSQLFunction::toVariant()");
            QVariant val;
            if(_valid) {
                bool found;
                QString str;
                QRegExp re;
                QVariant t;
                int i = 0;
                switch(_func) {
                    case FunctionValue:
                    case FunctionLiteral:
                        //qDebug("  FunctionValue");
                        str = _params[0];
                        val = params.value(str);
                        if(val.type() == QVariant::List || val.type() == QVariant::StringList) {
                            str += "__FOREACH_POS__";
                            t = params.value(str, &found);
                            if(found) {
                                val = (val.toList())[t.toInt()];
                            } else {
                                // we are not in a loop or the loop we are in is not for
                                // this list so just return the first value in the list
                                val = val.toList().first();
                            }
                        }
                        break;
                    case FunctionExists:
                        //qDebug("  FunctionExists");
                        params.value(_params[0], &found);
                        val = ( found ? _trueVariant : _falseVariant );
                        break;
                    case FunctionReExists:
                        //qDebug("  FunctionReExists");
                        //qDebug("    Pattern: %s", (const char*)_params[0]);
                        re.setPattern(_params[0]);
                        for(i = 0; i < params.count(); i++) {
                            if(re.indexIn(params.name(i)) != -1) {
                                val = _trueVariant;
                                break;
                            }
                        }
                        break;
                    case FunctionIsFirst:
                    case FunctionIsLast:
                        //qDebug("  FunctionIsFirst/FunctionIsLast");
                        val = _falseVariant;
                        str = _params[0];
                        t = params.value(str, &found);
                        if(found) {
                            if(t.type() == QVariant::List || t.type() == QVariant::StringList) {
                                str += "__FOREACH_POS__";
                                QVariant t2 = params.value(str, &found);
                                int pos = 0;
                                if(found)
                                    pos = t2.toInt();

                                QList<QVariant> l = t.toList();
                                if(l.size() > 0) {
                                    if((_func == FunctionIsFirst) && (pos == 0)) val = _trueVariant;
                                    else if((_func == FunctionIsLast) && ((pos + 1) == l.size())) val = _trueVariant;
                                }
                            } else {
                                val = _trueVariant;
                            }
                        }
                        break;
                    case FunctionContinue:
                    case FunctionBreak:
                        //qDebug("  FunctionContinue/FunctionBreak");
                        if(nBreaks && isContinue) {
                            *nBreaks = _nBreaks;
                            *isContinue = (_func == FunctionContinue);
                        }
                        break;
                    default:
                        (*_parent->_logger) << "MetaSQLFunction::toVariant() encountered unknown Function Type " << (int)_func << "!" << endl; 
                        // how did we get here?
                };
            }
            return val;
        }

    protected:
        Function identifyFunction(const QString & func) {
            QString f = func.trimmed().toLower();
            if(f == "value")
                return FunctionValue;
            else if(f == "literal")
                return FunctionLiteral;
            else if(f == "exists")
                return FunctionExists;
            else if(f == "reexists")
                return FunctionReExists;
            else if(f == "isfirst")
                return FunctionIsFirst;
            else if(f == "islast")
                return FunctionIsLast;
            else if(f == "continue")
                return FunctionContinue;
            else if(f == "break")
                return FunctionBreak;

            (*_parent->_logger) << "Unable to identify function '" << f << "'!" << endl;

            return FunctionUnknown;
        }

    private:
        bool _valid;
        bool _noOutput;
        Function _func;
        QStringList _params;
        int _nBreaks;
};

class MetaSQLBlock : public MetaSQLOutput {
    public:
        MetaSQLBlock(MetaSQLQueryPrivate * parent, const QString & cmd, const QString & _options)
          : MetaSQLOutput(parent) {
            _valid = false;

            _alt = 0;
            _if_not = false;
            _if_func = 0;

            _block = identifyBlock(cmd);
            if(_block != BlockUnknown) {
                QRegExp nw("[^\\w]"); // will find the first non word character
                int i, in_list;
                QString cmd, options, wip, tmp;
                QStringList plist;
                QChar qc, string_starter;
                bool enclosed, working, in_string;
                int p = 0;
                switch(_block) {
                    case BlockGeneric:
                        _valid = true;
                        break;
                    case BlockIf:
                    case BlockElseIf:
                        // hmmm the hard part ;)
                        // short solution to just get it to work.
                        // there is only one option and that is a single
                        // function call that returns true or false.
                        // with an optional NOT clause.
                        wip = _options.trimmed();
                        if(wip.left(4).toLower() == "not ") {
                            _if_not = true;
                            wip = wip.right(wip.length() - 4);
                        }

                        i = nw.indexIn(wip);
                        if(i == -1) {
                            cmd = wip;
                            options = QString::null;
                        } else {
                            cmd = wip.left(i);
                            options = wip.mid(i);
                        }
                        cmd = cmd.toLower();
                        options = options.trimmed();

                        if(!options.isEmpty()) {
                            // first if we have a '(' then we will only parse out the information between it
                            // and the following ')'
                            qc = options[0];
                            enclosed = false;
                            working = !enclosed;
                            in_string = false;
                            string_starter = '"';
                            wip = QString::null;
                            if(qc == '(') enclosed = true;
                            working = !enclosed;
                            for(int p = 0; p < options.length(); p++) {
                                qc = options.at(p);
                                if(!working && enclosed && qc == '(') working = true;
                                else {
                                    if(in_string) {
                                        if(qc == '\\') {
                                            wip += options.at(++p);
                                        } else if(qc == string_starter) {
                                            in_string = false;
                                        } else {
                                            wip += qc;
                                        }
                                    } else {
                                        if(qc == ',') {
                                            plist.append(wip);
                                            wip = QString::null;
                                        } else if(qc.isSpace()) {
                                            // eat white space
                                        } else if(qc == '\'' || qc == '"') {
                                            in_string = true;
                                            string_starter = qc;
                                        } else if(enclosed && qc == ')') {
                                            working = false;
                                            break;
                                        } else {
                                            wip += qc;
                                        }
                                    }
                                }
                            }
                            if(wip != QString::null) plist.append(wip);
                        }

                        _if_func = new MetaSQLFunction(_parent, cmd, plist);
                        if(!_if_func->isValid()) {
                            (*_parent->_logger) << "Failed to create new " << cmd << " function in if/elseif." << endl;
                            //qDebug("Options: '%s'\n", (const char*)_options);
                            delete _if_func;
                            _if_func = 0;
                        } else {
                            _valid = true;
                        }

                        break;
                    case BlockElse:
                        _valid = true;
                        break;
                    case BlockForEach:
                        tmp = _options.trimmed();
                        wip = QString::null;
                        in_string = false;
                        in_list = 0;
                        string_starter = '"';
                        for(p = 0; p < tmp.length(); p++) {
                            QChar qc = tmp.at(p);
                            if(in_string) {
                                if(qc == '\\') wip += _options.at(++p);
                                else if(qc == string_starter) in_string = false;
                                else wip += qc;
                            } else {
                                if(qc == '(') in_list++;
                                else if(qc == ')') {
                                    in_list--;
                                    if(in_list < 1) break;
                                } else if(qc == '\'' || qc == '"') {
                                    in_string = true;
                                    string_starter = qc;
                                } else if(qc == ',') break;
                                // everything else just... disapears?
                            }
                        }
                        if(!wip.isEmpty()) {
                            _loopVar = wip;
                            _valid = true;
                        }
                        break;
                    default:
                        (*_parent->_logger) << "MetaSQLBlock::MetaSQLBlock() encountered unknown Block Type " << (int)_block << "!" << endl;
                };
            }
        }
        virtual ~MetaSQLBlock() {
            while (!_items.isEmpty())
                delete _items.takeFirst();
            if(_alt) {
                delete _alt;
                _alt = 0;
            }
			if (_if_func) {
				delete _if_func;
				_if_func = 0;
			}
        }

        enum Block {
            BlockGeneric = -1,
            BlockUnknown = 0,
            BlockIf,
            BlockElseIf,
            BlockElse,
            BlockForEach
        };

        bool isValid() { return _valid; }
        Block type() { return _block; }

        void append(MetaSQLOutput * mso) {
            if(mso) {
                _items.append(mso);
            }
        }

        void setAlternate(MetaSQLBlock * alt) {
            _alt = alt;
        }

        virtual QString toString(MetaSQLInfo * mif, const ParameterList & params, int * nBreaks = 0, bool * isContinue = 0) {
            //qDebug("MetaSQLBlock::toString()");
            QString results = QString::null;

            MetaSQLOutput * output = 0;
            bool b = false, myContinue = false, found;
            int myBreaks = 0;
            int i = 0, n = 0, ii = 0;
            QList<QVariant> list;
            QVariant v, t;
            ParameterList pList;
            QString str;
            switch(_block) {
                case BlockIf:
                case BlockElseIf:
                    //qDebug("  BlockIf/BlockEleseIf");
                    b = _if_func->toVariant(params, nBreaks, isContinue).toBool();
                    if(_if_not) b = !b;
                    if(b) {
                        //qDebug("    Expression evaluated TRUE");
                        for(i = 0; i < _items.size(); i++)
                        {
                            output = _items.at(i);
                            results += output->toString(mif, params, nBreaks, isContinue);
                            if(nBreaks && *nBreaks) break;
                        }
                    } else if(_alt) {
                        //qDebug("    Expression evaluated FALSE");
                        results = _alt->toString(mif, params, nBreaks, isContinue);
                    }
                    break;

                case BlockForEach:
                    //qDebug("  BlockForEach");

                    // HERE
                    v = params.value(_loopVar, &found);
                    if(found) {
                        list = v.toList();
                        for(i = 0; i < list.count(); i++) {
                            str = _loopVar + "__FOREACH_POS__";

                            // create a new params list with our special var added in 
                            pList.clear();
                            pList.append(str, i);
                            for(n = 0; n < params.count(); n++) {
                                if(params.name(n) != str) {
                                    pList.append(params.name(n), params.value(n));
                                }
                            }

                            myBreaks = 0;
                            myContinue = false;

                            // execute the block
                            for(ii = 0; ii < _items.size(); ii++)
                            {
                                output = _items.at(ii);
                                results += output->toString(mif, pList, &myBreaks, &myContinue);
                                if(myBreaks) break;
                            }

                            if(myBreaks > 0) {
                                myBreaks--;
                                if(myBreaks > 0 || !myContinue) {
                                    if(nBreaks) *nBreaks = myBreaks;
                                    if(isContinue) *isContinue = myContinue;
                                    break;
                                }
                            }
                        }
                    }



                    break;

                case BlockElse:
                case BlockGeneric:
                    //qDebug("  BlockElse/BlockGeneric");
                    for(i = 0; i < _items.size(); i++)
                    {
                        output = _items.at(i);
                        results += output->toString(mif, params, nBreaks, isContinue);
                        if(nBreaks && *nBreaks) break;
                    }
                    break;

                default:
                    qDebug("Encountered unknown Block type %d.", _block);
            };

            return results;
        }

    protected:
        Block identifyBlock(const QString & block) {
            QString b = block.trimmed().toLower();
            if(b == "generic")
                return BlockGeneric;
            else if(b == "if")
                return BlockIf;
            else if(b == "elseif")
                return BlockElseIf;
            else if(b == "else")
                return BlockElse;
            else if(b == "foreach")
                return BlockForEach;

            qDebug("Unable to identify block '%s'!", b.toLatin1().data());

            return BlockUnknown;
        }

    private:
        bool _valid;
        Block _block;

        MetaSQLBlock * _alt;
        QList<MetaSQLOutput*> _items;

        QString _loopVar;

        bool _if_not;
        MetaSQLFunction * _if_func;
};

MetaSQLQueryPrivate::~MetaSQLQueryPrivate() {
    if(_top) {
        delete _top;
        _top = 0;
    }
    if(_logger) {
        delete _logger;
        _logger = 0;
    }
}

bool MetaSQLQueryPrivate::populate(XSqlQuery & qry, const ParameterList & params) {
    bool res = false;
    //qDebug("MetaSQLQueryPrivate::toString()");
    if(_top) {
        MetaSQLInfo mif;
        QString sql = _top->toString(&mif, params);
        sql = sql.trimmed();
        res = qry.prepare(sql);
        for(int i = 0; i < mif._pList.count(); i++) {
            qry.bindValue(mif._pList.name(i), mif._pList.value(i));
        }
    }
    return res;
}

bool MetaSQLQueryPrivate::parse_query(const QString & query) {
    _top = new MetaSQLBlock(this, "generic", QString::null);
    QList<MetaSQLBlock*> _blocks;
    _blocks.append(_top);
    MetaSQLBlock * _current = _top;

    QRegExp re("<\\?(.*)\\?>");
    QRegExp nw("[^\\w]"); // will find the first non word character 
    re.setMinimal(TRUE);

    QString s;

    int lastPos = 0;
    int currPos = 0;
    while(currPos >= 0) {
        currPos = re.indexIn(query, currPos);
        if(lastPos != currPos) {
            _current->append(new MetaSQLString(this, query.mid(lastPos, (currPos==-1?query.length():currPos)-lastPos)));
        }
        if(currPos >= 0) {
            s = re.cap(1);
            s = s.trimmed();
            int i = nw.indexIn(s);
            QString cmd, options;
            if(i == -1) {
                cmd = s;
                options = QString::null;
            } else {
                cmd = s.left(i);
                options = s.mid(i);
            }
            cmd = cmd.toLower();

            if(cmd == "endif" || cmd == "endforeach") {
                MetaSQLBlock::Block _block = _current->type();
                if(  (cmd == "endif" && (  _block == MetaSQLBlock::BlockIf
                                        || _block == MetaSQLBlock::BlockElseIf
                                        || _block == MetaSQLBlock::BlockElse) )
                  || (cmd == "endforeach" && ( _block == MetaSQLBlock::BlockForEach ) ) ) {
                    _blocks.removeLast();
                    _current = _blocks.last();
                } else {
                    // uh oh! We encountered an end block tag when we were either not in a
                    // block or were in a block of a different type.
                    *_logger << "Encountered an unexpected " << cmd << "." << endl;
                    _valid = false;
                    return false;
                }
            } else if(cmd == "if" || cmd == "foreach") {
                // we have a control statement here and need to create a new block
                MetaSQLBlock * b = new MetaSQLBlock(this, cmd, options);
                if(b->isValid()) {
                    _current->append(b);
                    _blocks.append(b);
                    _current = b;
                } else {
                    *_logger << "Failed to create new " << cmd << " block." << endl;
                    delete b;
                    _valid = false;
                    return false;
                }
            } else if(cmd == "elseif" || cmd == "else") {
                // we need to switch up are if block to include this new alternate
                if(_current->type() == MetaSQLBlock::BlockElse) {
                    *_logger << "Encountered unexpected " << cmd << " statement within else block." << endl;
                    _valid = false;
                    return false;
                } else if(_current->type() != MetaSQLBlock::BlockIf && _current->type() != MetaSQLBlock::BlockElseIf) {
                    *_logger << "Encountered unexpected " << cmd << " statement outside of if/elseif block." << endl;
                    _valid = false;
                    return false;
                } else {
                    MetaSQLBlock * b = new MetaSQLBlock(this, cmd, options);
                    if(b->isValid()) {
                        _current->setAlternate(b);
                        _blocks.removeLast();
                        _blocks.append(b);
                        _current = b;
                    } else {
                        *_logger << "Failed to create new " << cmd << " block." << endl;
                        delete b;
                        _valid = false;
                        return false;
                    }
                }
            } else {
                // we must have a function... if not then i don't know what it could be.
                // first we must parse the options into a list of parameters for the function
                options = options.trimmed();
                QStringList plist;
                if(!options.isEmpty()) {
                    // first if we have a '(' then we will only parse out the information between it
                    // and the following ')'
                    QChar qc = options[0];
                    bool enclosed = false;
                    bool working = !enclosed;
                    bool in_string = false;
                    QChar string_starter = '"';
                    QString wip = QString::null;
                    if(qc == '(') enclosed = true;
                    working = !enclosed;
                    for(int p = 0; p < options.length(); p++) {
                        qc = options.at(p);
                        if(!working && enclosed && qc == '(') working = true;
                        else {
                            if(in_string) {
                                if(qc == '\\') {
                                    wip += options.at(++p);
                                } else if(qc == string_starter) {
                                    in_string = false;
                                } else {
                                    wip += qc;
                                }
                            } else {
                                if(qc == ',') {
                                    plist.append(wip);
                                    wip = QString::null;
                                } else if(qc.isSpace()) {
                                    // eat white space
                                } else if(qc == '\'' || qc == '"') {
                                    in_string = true;
                                    string_starter = qc;
                                } else if(enclosed && qc == ')') {
                                    working = false;
                                    break;
                                } else {
                                    wip += qc;
                                }
                            }
                        }
                    }
                    if(wip != QString::null) plist.append(wip);
                }

                MetaSQLFunction * f = new MetaSQLFunction(this, cmd, plist);
                if(f->isValid()) {
                    _current->append(f);
                } else {
                    *_logger << "Failed to create new " << cmd << " function." << endl;
                    delete f;
                    _valid = false;
                    return false;
                }
            }

            currPos += re.matchedLength();
        }
        lastPos = currPos;
    }

    _valid = true;
    return true;
}

MetaSQLQuery::MetaSQLQuery(const QString & query) {
    _data = new MetaSQLQueryPrivate();
    _source = QString::null;

    if(!query.isEmpty()) {
        setQuery(query);
    }
}

MetaSQLQuery::~MetaSQLQuery() {
    if(_data) {
        delete _data;
        _data = 0;
    }
}

bool MetaSQLQuery::setQuery(const QString & query) {
    bool valid = false;
    if(_data) {
        _source = query;
        if(_data->_top) {
            delete _data->_top;
            _data->_top = 0;
            _data->_valid = false;
        }
        valid = _data->parse_query(query);
    }
    return valid;
}

QString MetaSQLQuery::getSource() { return _source; }
bool MetaSQLQuery::isValid() { return (_data && _data->isValid()); }

XSqlQuery MetaSQLQuery::toQuery(const ParameterList & params, QSqlDatabase pDb, bool pExec) {
    XSqlQuery qry(pDb);
    if(isValid()) {
        if(_data->populate(qry, params) && pExec) {
            qry.exec();
        }
    }
    return qry;
}

QString MetaSQLQuery::parseLog() { return (_data ? _data->_log : QString::null); }
