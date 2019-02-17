/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include <QDialogButtonBox>
#include <QStringList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QFileInfo>
#include <QDialog>
#include <QLabel>
#include <QFile>
//--------------------------------------------------------------------------------
#include "selectbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
SelectBox::SelectBox(QWidget *parent,
                     const QString &caption,
                     const QString &path,
                     const QString &filename,
                     const QStringList &list_pict) :
    QDialog(parent)
{
    setWindowTitle(caption);
    this->path = path;
    current_picture = QString();

    current_filename = new QLabel(QString(tr("<b>Файл<br>%1<br>соответствует картинке:</b>")).arg(filename));

    QVBoxLayout *vbox = new QVBoxLayout();
    QHBoxLayout *hbox = new QHBoxLayout();

    picture_label = new QLabel();

    buttonBox = new QDialogButtonBox();

    list_pictures = new QListWidget();
    foreach (QString picture, list_pict)
    {
        list_pictures->addItem(picture);
    }

    hbox->addWidget(list_pictures);
    hbox->addWidget(picture_label);

    list_pictures->setFixedSize(MAX_WIDTH, MAX_HEIGHT);
    picture_label->setFixedSize(MAX_WIDTH, MAX_HEIGHT);

    buttonBox->setCenterButtons(true);
    buttonBox->addButton(QDialogButtonBox::Ok);
    buttonBox->addButton(QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    connect(list_pictures, SIGNAL(currentTextChanged(QString)), this, SLOT(show_picture(QString)));
    connect(list_pictures, SIGNAL(doubleClicked(QModelIndex)),  this, SLOT(select_picture(QModelIndex)));

    vbox->addWidget(current_filename);
    vbox->addLayout(hbox);
    vbox->addWidget(buttonBox);
    setLayout(vbox);

    list_pictures->setCurrentRow(0);
}
//--------------------------------------------------------------------------------
void SelectBox::select_picture(QModelIndex index)
{
    Q_UNUSED(index);
    this->accept();
}
//--------------------------------------------------------------------------------
void SelectBox::show_picture(const QString &picture_name)
{
    float multiplier = 1.0f;
    int width  = MAX_WIDTH;
    int height = MAX_HEIGHT;
    float ideal_proportion = static_cast<float>(MAX_WIDTH)/static_cast<float>(MAX_HEIGHT);
    float proportion;
    QPixmap picture;
    QPixmap new_picture;
    QString filename;
    QString temp;

    temp = QString("%1/%2")
            .arg(path)
            .arg(picture_name);
    filename = temp;
    if(!QFile(filename).exists())
        filename = QString(SPACE_FILE);
    picture.load(filename);
    proportion = static_cast<float>(picture.width())/static_cast<float>(picture.height());
    if(proportion < ideal_proportion)
        multiplier = static_cast<float>(MAX_HEIGHT) / picture.height();
    else
        multiplier = static_cast<float>(MAX_WIDTH) / picture.width();

    width  = static_cast<int>(picture.width() * multiplier);
    height = static_cast<int>(picture.height() * multiplier);
    // qDebug() << width << height;

    new_picture = picture.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    picture_label->setPixmap(new_picture);
    current_picture = filename;
}
//--------------------------------------------------------------------------------
QString SelectBox::selected_pictures(void)
{
    return current_picture;
}
//--------------------------------------------------------------------------------
