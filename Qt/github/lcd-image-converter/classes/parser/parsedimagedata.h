/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2013 riuson
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

#ifndef PARSEDIMAGEDATA_H
#define PARSEDIMAGEDATA_H

#include <QObject>

class QImage;
class Tags;
class Preset;

class ParsedImageData : public QObject
{
  Q_OBJECT
public:
  explicit ParsedImageData(Preset *preset, const QImage *image, const Tags &tags, QObject *parent = 0);
  ~ParsedImageData();

  Tags *tags() const;
  uint hash() const;

  const QString outputImageDataWithEOL(const Tags &tags) const;

private:
  Tags *mTags;
  uint mHash;
  QString mPreparedOutputImageData;
};

#endif // PARSEDIMAGEDATA_H
