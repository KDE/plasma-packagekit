/*
    Copyright © 2017 Harald Sitter <sitter@kde.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License or (at your option) version 3 or any later version
    accepted by the membership of KDE e.V. (or its successor approved
    by the membership of KDE e.V.), which shall act as a proxy
    defined in Section 14 of version 3 of the license.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 2.0

import org.kde.plasma.core 2.1 as PlasmaCore
import org.kde.plasma.plasmoid 2.0

// As the plasmoid state is dependent on the running transaction count we can easily get into
// situations where the state switches quickly when a client doesn't cause a steady stream of
// transactions (e.g. numerous refreshes). To mitigate this we'll only switch to passivity after
// a long while and to activity after a short while. Also, scheduling a change to either
// aborts the change to the other.
Item {
    Timer {
        // Wait 1 second before switching to active state.
        id: activeTimer
        interval: 1000
        repeat: false
        running: transactionModel.count >= 1
        onTriggered: {
            console.debug(plasmoid.status)
            plasmoid.status = PlasmaCore.Types.ActiveStatus
            console.debug(plasmoid.status)
        }
        onRunningChanged: running && passiveTimer.stop
    }
    Timer {
        // Wait 2.5 seconds before switching to passive state.
        id: passiveTimer
        interval: 2500
        repeat: false
        running: transactionModel.count < 1
        onTriggered: plasmoid.status = PlasmaCore.Types.PassiveStatus
        onRunningChanged: running && activeTimer.stop
    }
}
