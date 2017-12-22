/********************************************************************
 KWin - the KDE window manager
 This file is part of the KDE project.

Copyright (C) 2012 Mathias Gottschlag <mgottschlag@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/

import QtQuick 2.0;
/* import org.kde.qtextracomponents 2.0 as QtExtra; */
import org.kde.plasma.core 2.0 as PlasmaCore;
import org.kde.plasma.components 2.0 as Plasma;
import org.kde.kwin 2.0;
import "../code/tilingmanager.js" as Tiling

Item {
    property variant tiling
    Component.onCompleted: {
        console.log("Starting tiling");
        // Initialize tiling
        tiling = new Tiling.TilingManager();
        // Attach resized here so we can give it a timer (since QTimer isn't exported to QML)
        /* tiling.resized.connect(function() { */
        /*     timer.start(); */
        /* }); */
    }

    /* Timer { */
    /*     id: timer */
    /*     interval: 1000; running: false; repeat: false */
    /*     onTriggered: tiling.resize(); */
    /* } */

}
