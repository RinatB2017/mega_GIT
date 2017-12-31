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

#include "qt-version-check.h"

#if QT_VERSION_COMBINED >= VERSION_COMBINE(5, 0, 0)
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

#include "mainwindow.h"
#include "revisioninfo.h"

#if QT_VERSION_COMBINED >= VERSION_COMBINE(5, 2, 0)
#include "cmdline.h"
#endif

void setupApplication(QApplication *app)
{
  QString version = QString("rev.%1 from %2").arg(RevisionInfo::hash(), RevisionInfo::date());
  QCoreApplication::setApplicationVersion(version);

  app->addLibraryPath(QApplication::applicationDirPath());
  app->addLibraryPath(QApplication::applicationDirPath() + "/plugins");
}

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  setupApplication(&a);

#if QT_VERSION_COMBINED >= VERSION_COMBINE(5, 2, 0)
  CommandLine::CmdLine cmd(a.arguments());

  if (cmd.needProcess()) { // if console mode
    switch (cmd.process()) {
      case CommandLine::CmdLine::ProcessResult::Failed: {
        return 1;
      }

      case CommandLine::CmdLine::ProcessResult::Success: {
        return 0;
      }

      case CommandLine::CmdLine::ProcessResult::None:
      default: {
        break;
      }
    }
  }

#endif

  // gui mode
  MainWindow w;
  w.show();
  return a.exec();
}

