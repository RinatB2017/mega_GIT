/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2010 riuson
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

#ifndef EDITORTABIMAGE_H
#define EDITORTABIMAGE_H

#include <QWidget>

#include "ieditor.h"

namespace Ui
{
class EditorTabImage;
}

class QImage;
class DataContainer;
class StatusData;
class ImageDocument;
class IDocument;

namespace ImageEditor
{
class Editor;
}

class EditorTabImage : public QWidget, public IEditor
{
  Q_OBJECT
  Q_INTERFACES(IEditor)

public:
  explicit EditorTabImage(QWidget *parent = 0);
  ~EditorTabImage();

  IDocument *document() const;
  QStringList selectedKeys() const;
  StatusData *statusData() const;
  EditorType type() const;

protected:
  void changeEvent(QEvent *e);

private:
  Ui::EditorTabImage *ui;
  QWidget *mEditorWidget;
  ImageEditor::Editor *mEditorObject;
  ImageDocument *mDocument;
  StatusData *mStatusData;

  void initStatusData();
  void updateStatus();
  void updateSelectedImage();

private slots:
  void mon_documentChanged();
  void mon_editor_imageChanged(const QImage *value);
  void mon_editor_mouseMove(const QPoint *point);
  void mon_editor_scaleChanged(int scale);
signals:
  void documentChanged();
  void statusChanged();
};

#endif // EDITORTABIMAGE_H
