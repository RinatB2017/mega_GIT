/************************************************************************   
 * Copyright  2009  Miquel Canes Gonzalez  miquelcanes@gmail.com        *
 *                                                                      *
 * This program is free software; you can redistribute it and/or        *
 * modify it under the terms of the GNU General Public License as       *
 * published by the Free Software Foundation; either version 2 of       *
 * the License, or (at your option) any later version.                  *
 *                                                                      *
 * This program is distributed in the hope that it will be useful,      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.*
 ************************************************************************/

#include <QApplication>
#include <QAboutData>
#include <QCmdLineArgs>
#include <QMessageBox>

#include "checkmate.h"

int main(int argc, char** argv)
{
	KAboutData aboutData("CheckMate", 0, ki18n("Checkmate"), "0.0.1",
			ki18n("Chess Game"),
			KAboutData::License_GPL, ki18n("(c) 2010"),
			ki18n("Chess game"),
			"",
			"miquelcanes@gmail.com");
	aboutData.addAuthor( ki18n("Miquel Canes Gonzalez"), KLocalizedString(), "miquelcanes@gmail.com" );

	KCmdLineArgs::init(argc, argv, &aboutData);
	KApplication app;
	Checkmate* t = new Checkmate();
	t->show();
	return app.exec();
}
