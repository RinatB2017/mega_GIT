/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2014 riuson
 * mailto: riuson@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/
 */

#include <qt-version-check.h>

#if QT_VERSION_COMBINED >= VERSION_COMBINE(5, 2, 0)

#ifndef MODEHEX2BIN_H
#define MODEHEX2BIN_H

#include "modeparserbase.h"
#include <QObject>

namespace CommandLine
{

class ModeHex2Bin : public ModeParserBase
{
  Q_OBJECT
public:
  explicit ModeHex2Bin(QCommandLineParser *parser, QObject *parent = 0);

  static QString modeName();

  void fillParser() const;
  bool collectArguments();
  int process();

private:
  QString mInputFilename;
  QString mOuputFilename;

  QByteArray hex2bin(QString &hexString);
  void appendDataLE(QByteArray *array, int size, quint32 value);
  void appendDataBE(QByteArray *array, int size, quint32 value);
};

}

#endif // MODEHEX2BIN_H

#endif // QT_VERSION
