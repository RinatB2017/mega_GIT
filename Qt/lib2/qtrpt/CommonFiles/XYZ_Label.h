/*
Name: XYZ
Version: 3.1.1
Programmer: Oleksii Osypov
E-mail: aliks-os@ukr.net
Web-site: http://www.aliks-os.tk

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

#ifndef XYZ_LABEL_H
#define XYZ_LABEL_H

#include <QLabel>
#include <QBasicTimer>

class XYZ_Label : public QLabel
{
    Q_OBJECT
public:
    XYZ_Label(QWidget *parent = 0);
    XYZ_Label(const QString &Text, QWidget *parent = 0);
    ~XYZ_Label();
    void setHoverText(bool bHover);

private:
	QBasicTimer timer;
    bool m_bHover;
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
	void timerEvent(QTimerEvent *);

signals:
    void clicked();
	void doubleClicked();

};
#endif

