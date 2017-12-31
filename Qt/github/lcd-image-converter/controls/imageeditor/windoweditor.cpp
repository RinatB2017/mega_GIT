/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2015 riuson
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

#include "windoweditor.h"
#include "ui_windoweditor.h"

#include <QMouseEvent>
#include <QColorDialog>
#include <QPainter>
#include <QIcon>

#include "bitmaphelper.h"
#include "toolsmanager.h"
#include "iimageeditortool.h"
#include "imageeditoroptions.h"

namespace ImageEditor
{

WindowEditor::WindowEditor(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::WindowEditor)
{
  ui->setupUi(this);

  this->ui->label->installEventFilter(this);
  this->ui->toolBarOptions->hide();

  this->restoreState(ImageEditorOptions::toolbarsState(), 0);

  this->mSelectedTool = NULL;
  this->createTools();

  this->updateImageScaled(this->mTools->scale());

  QImage templateImage(":/images/template");
  this->setImage(&templateImage);
}

WindowEditor::~WindowEditor()
{
  if (this->ui->toolBarOptions->actions().length() == 0) {
    this->ui->toolBarOptions->hide();
  }

  ImageEditorOptions::setToolbarsState(this->saveState(0));

  delete this->mTools;
  delete ui;
}

void WindowEditor::changeEvent(QEvent *e)
{
  QWidget::changeEvent(e);

  switch (e->type()) {
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
      this->updateImageScaled(this->mTools->scale());
      break;

    default:
      break;
  }
}

bool WindowEditor::eventFilter(QObject *obj, QEvent *event)
{
  bool result = false;

  if (this->mSelectedTool != NULL) {
    if (event->type() == QEvent::MouseButtonDblClick ||
        event->type() == QEvent::MouseButtonPress ||
        event->type() == QEvent::MouseButtonRelease ||
        event->type() == QEvent::MouseMove) {
      QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

      int xscaled = mouseEvent->pos().x();
      int yscaled = mouseEvent->pos().y();
      int xreal = xscaled / this->mTools->scale();
      int yreal = yscaled / this->mTools->scale();
      QPoint mousePosition(xreal, yreal);

      bool inRect = this->mImageOriginal.rect().contains(mousePosition, false);

      QMouseEvent mouseEventScaled = QMouseEvent(
                                       mouseEvent->type(),
                                       mousePosition,
                                       mouseEvent->button(),
                                       mouseEvent->buttons(),
                                       mouseEvent->modifiers());

      result = this->mSelectedTool->processMouse(&mouseEventScaled, &this->mImageOriginal, inRect);

      if (mouseEventScaled.isAccepted()) {
        mouseEvent->accept();
      }

      if (!inRect) {
        mousePosition.setX(-1);
        mousePosition.setY(-1);
      }

      emit this->mouseMove(&mousePosition);

      if (result) {
        return result;
      }
    }
  }

  result = QObject::eventFilter(obj, event);

  return result;
}

void WindowEditor::wheelEvent(QWheelEvent *event)
{
  if ((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier) {
    QPoint point = event->globalPos();
    point = this->mapFromGlobal(point);

    QRect labelRect = this->ui->label->rect();
    QPoint labelPoint = this->ui->label->pos();
    labelRect.moveTo(labelPoint);

    if (labelRect.contains(point.x(), point.y())) {
      if (event->orientation() == Qt::Vertical) {
        int scale = this->mTools->scale();

        if (event->delta() > 0) {
          scale++;
        } else {
          scale--;
        }

        this->mTools->setScale(scale);
      }

      event->accept();
    }
  }
}

const QImage *WindowEditor::image() const
{
  const QImage *result = &this->mImageOriginal;
  return result;
}

void WindowEditor::setImage(const QImage *value)
{
  this->mImageOriginal = QImage(*value);
  this->updateImageScaled(this->mTools->scale());
}

int WindowEditor::scale() const
{
  return this->mTools->scale();
}

void WindowEditor::updateImageScaled(int value)
{
  if (!this->mImageOriginal.isNull()) {
    QImage image = this->mImageOriginal;

    image = BitmapHelper::drawSelection(&image, this->mTools->selectedPath());
    image = BitmapHelper::scale(&image, value);
    image = BitmapHelper::drawGrid(&image, value);
    this->mImageScaled = image;
    this->mPixmapScaled = QPixmap::fromImage(image);

    this->ui->label->setPixmap(this->mPixmapScaled);
  }
}

void WindowEditor::updateImageScaled(const QImage &image, int scale)
{
  this->mImageScaled = BitmapHelper::scale(&image, scale);
  this->mImageScaled = BitmapHelper::drawGrid(&this->mImageScaled, scale);
  this->mPixmapScaled = QPixmap::fromImage(this->mImageScaled);

  this->ui->label->setPixmap(this->mPixmapScaled);
}

void WindowEditor::drawPixel(int x, int y, const QColor &color)
{
  QImage image = this->mImageOriginal;
  this->mImageOriginal = BitmapHelper::drawPixel(&image, x, y, color);
  this->updateImageScaled(this->mTools->scale());
}

void WindowEditor::createTools()
{
  this->mTools = new ToolsManager(this);
  QList<QAction *> actions = QList<QAction *> (*this->mTools->toolsActions());
  this->ui->toolBarTools->addActions(actions);
  this->connect(this->mTools, SIGNAL(toolChanged(int)), SLOT(toolChanged(int)));
  this->connect(this->mTools, SIGNAL(scaleChanged(int)), SLOT(tool_scaleChanged(int)));
  this->connect(this->mTools, SIGNAL(selectionChanged(QPainterPath)), SLOT(tool_selectionChanged(QPainterPath)));
  this->ui->toolBarOptions->hide();

  if (this->ui->toolBarTools->actions().length() > 0) {
    this->ui->toolBarTools->actions().at(0)->activate(QAction::Trigger);
  }
}

void WindowEditor::tool_started(const QImage *value)
{
  QImage image = *value;
  this->updateImageScaled(image, this->mTools->scale());
}

void WindowEditor::tool_processing(const QImage *value)
{
  QImage image = *value;
  this->updateImageScaled(image, this->mTools->scale());
}

void WindowEditor::tool_completed(const QImage *value, bool changed)
{
  if (changed) {
    this->mImageOriginal = *value;
    this->updateImageScaled(this->mTools->scale());
    emit this->imageChanged();
  } else {
    this->updateImageScaled(this->mTools->scale());
  }
}

void WindowEditor::tool_scaleChanged(int value)
{
  this->updateImageScaled(value);
  emit this->scaleChanged(value);
}

void WindowEditor::tool_selectionChanged(const QPainterPath &value)
{
  this->updateImageScaled(this->mTools->scale());
}

void WindowEditor::toolChanged(int toolIndex)
{
  this->ui->toolBarOptions->clear();

  IImageEditorTool *tool = this->mSelectedTool;

  if (this->mSelectedTool != NULL) {
    QObject *obj = dynamic_cast<QObject *>(tool);

    if (tool != NULL) {
      obj->disconnect(SIGNAL(started(const QImage *)), this, SLOT(tool_started(const QImage *)));
      obj->disconnect(SIGNAL(processing(const QImage *)), this, SLOT(tool_processing(const QImage *)));
      obj->disconnect(SIGNAL(completed(const QImage *, bool)), this, SLOT(tool_completed(const QImage *, bool)));
    }
  }

  tool = this->mTools->tools()->at(toolIndex);
  this->ui->toolBarOptions->addActions(*tool->actions());
  this->mSelectedTool = tool;

  this->connect(dynamic_cast<QObject *>(tool), SIGNAL(started(const QImage *)), SLOT(tool_started(const QImage *)));
  this->connect(dynamic_cast<QObject *>(tool), SIGNAL(processing(const QImage *)), SLOT(tool_processing(const QImage *)));
  this->connect(dynamic_cast<QObject *>(tool), SIGNAL(completed(const QImage *, bool)), SLOT(tool_completed(const QImage *, bool)));

  for (int i = 0; i < tool->widgets()->length(); i++) {
    QWidget *widget = tool->widgets()->at(i);

    // toolbar takes ownership on widget
    // so widget will be deleted on toolbar deleting
    QAction *widgetAction = this->ui->toolBarOptions->addWidget(widget);
    widgetAction->setToolTip(widget->toolTip());
    widgetAction->setVisible(true);
  }

  this->ui->toolBarOptions->setVisible(this->ui->toolBarOptions->actions().length() > 0);
}

}

