/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "classwizard.hpp"

#include "outputfilepage.hpp"
#include "conclusionpage.hpp"
#include "classinfopage.hpp"
#include "codestylepage.hpp"
#include "infopage.hpp"

#include "defines.hpp"

ClassWizard::ClassWizard(QWidget *parent)
    : QWizard(parent)
{
    addPage(new IntroPage);
    addPage(new ClassInfoPage);
    addPage(new CodeStylePage);
    addPage(new OutputFilesPage);
    addPage(new ConclusionPage);

    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));

    setWindowTitle(tr("Class Wizard"));
}

void ClassWizard::accept()
{
    QByteArray className = field("className").toByteArray();
    QByteArray baseClass = field("baseClass").toByteArray();
    QByteArray macroName = field("macroName").toByteArray();
    QByteArray baseInclude = field("baseInclude").toByteArray();

    QString outputDir = field("outputDir").toString();
    QString header = field("header").toString();
    QString implementation = field("implementation").toString();

    QByteArray block;

    if (field("comment").toBool())
    {
        block += license;
        block += '\n';
    }
    if (field("protect").toBool())
    {
        block += "#ifndef " + macroName + '\n';
        block += "#define " + macroName + '\n';
        block += '\n';
    }
    if (field("includeBase").toBool())
    {
        block += "#include " + baseInclude + '\n';
        block += '\n';
    }

    block += "class " + className;
    if (!baseClass.isEmpty())
        block += " : public " + baseClass;
    block += '\n';
    block += "{\n";

    /* qmake ignore Q_OBJECT */

    if (field("qobjectMacro").toBool())
    {
        block += "    Q_OBJECT\n";
        block += '\n';
    }
    block += "public:\n";

    if (field("qobjectCtor").toBool())
    {
        block += "    " + className + "(QObject *parent = nullptr);\n";
    } else if (field("qwidgetCtor").toBool())
    {
        block += "    " + className + "(QWidget *parent = nullptr);\n";
    } else if (field("defaultCtor").toBool())
    {
        block += "    " + className + "();\n";
        if (field("copyCtor").toBool())
        {
            block += "    " + className + "(const " + className + " &other);\n";
            block += '\n';
            block += "    " + className + " &operator=" + "(const " + className
                    + " &other);\n";
        }
    }
    block += "};\n";

    if (field("protect").toBool())
    {
        block += '\n';
        block += "#endif\n";
    }

    QFile headerFile(outputDir + '/' + header);
    if (!headerFile.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(nullptr, QObject::tr("Simple Wizard"),
                             QObject::tr("Cannot write file %1:\n%2")
                             .arg(headerFile.fileName())
                             .arg(headerFile.errorString()));
        return;
    }
    headerFile.write(block);

    block.clear();

    if (field("comment").toBool())
    {
        block += license;
        block += '\n';
    }
    block += "#include \"" + header.toLatin1() + "\"\n";
    block += '\n';

    if (field("qobjectCtor").toBool())
    {
        block += className + "::" + className + "(QObject *parent)\n";
        block += "    : " + baseClass + "(parent)\n";
        block += "{\n";
        block += "}\n";
    } else if (field("qwidgetCtor").toBool())
    {
        block += className + "::" + className + "(QWidget *parent)\n";
        block += "    : " + baseClass + "(parent)\n";
        block += "{\n";
        block += "}\n";
    } else if (field("defaultCtor").toBool())
    {
        block += className + "::" + className + "()\n";
        block += "{\n";
        block += "    // missing code\n";
        block += "}\n";

        if (field("copyCtor").toBool())
        {
            block += "\n";
            block += className + "::" + className + "(const " + className
                    + " &other)\n";
            block += "{\n";
            block += "    *this = other;\n";
            block += "}\n";
            block += '\n';
            block += className + " &" + className + "::operator=(const "
                    + className + " &other)\n";
            block += "{\n";
            if (!baseClass.isEmpty())
                block += "    " + baseClass + "::operator=(other);\n";
            block += "    // missing code\n";
            block += "    return *this;\n";
            block += "}\n";
        }
    }

    QFile implementationFile(outputDir + '/' + implementation);
    if (!implementationFile.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(nullptr, QObject::tr("Simple Wizard"),
                             QObject::tr("Cannot write file %1:\n%2")
                             .arg(implementationFile.fileName())
                             .arg(implementationFile.errorString()));
        return;
    }
    implementationFile.write(block);

    QDialog::accept();
}

