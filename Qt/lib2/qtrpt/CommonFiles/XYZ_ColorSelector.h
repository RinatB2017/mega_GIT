/*
Name: XYZLibrary
Version: 4.0.0
Web-site: http://qtrpt.sourceforge.io
Programmer: Oleksii Osypov
E-mail: aliks-os@ukr.net

Copyright 2012-2025 Oleksii Osypov

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef XYZ_COLORSELECTOR_H
#define XYZ_COLORSELECTOR_H

#include <QWidget>
#include <QToolButton>
#include <QHBoxLayout>

class XYZ_ColorSelector : public QWidget
{
    Q_OBJECT
public:
    explicit XYZ_ColorSelector(QWidget *parent = nullptr, QString rgbColor = "rgba(255,255,255,255)");
    QToolButton *button;
    QString getBackGroundColor();
    void setBackGroundColor(QString rgbColor);
    void setMargins(QMargins margins);

private:
    QHBoxLayout *vLayout;

signals:

public slots:

};

#endif // XYZ_COLORSELECTOR_H
