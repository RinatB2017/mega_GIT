/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2012 riuson
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

#ifndef TEMPLATEOPTIONS_H
#define TEMPLATEOPTIONS_H

#include <QObject>
#include <QString>

class QSettings;
class QDomElement;

class TemplateOptions : public QObject
{
  Q_OBJECT
public:
  explicit TemplateOptions(QObject *parent = 0);

  const QString &image() const;
  const QString &font() const;

  void setImage(const QString &filename);
  void setFont(const QString &filename);

  bool load(QSettings *settings);
  bool loadXmlElement(QDomElement element);
  void save(QSettings *settings);
  void saveXmlElement(QDomElement element);

private:
  static const QString GroupName;
  static const QString FieldImages;
  static const QString FieldFonts;

  QString mImageTemplate;
  QString mFontTemplate;

signals:
  void changed();

public slots:

};

#endif // TEMPLATEOPTIONS_H
