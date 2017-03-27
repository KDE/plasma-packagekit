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
import QtQuick.Layouts 1.0

import org.kde.plasma.core 2.1 as PlasmaCore
import org.kde.plasma.extras 2.0 as PlasmaExtras
import org.kde.plasma.plasmoid 2.0

import org.kde.plasma.private.packagekit 0.1

import "../code/role.js" as Role

Item {
    id: main

    function stateText() {
        return Daemon.isRunning ? i18n("PackageKit is Running") : i18n("PackageKit is not Running")
    }

    function flavorText() {
        var options = ['']

        if (Daemon.isRunning && Daemon.locked) {
            // Running and doing something important.
            options = [
                        i18n("Aligning the Flux Capacitor"),
                        i18n("Preparing for brain surgery"),
                        i18n("Initiating launch sequence"),
                        i18n("Checking matrix for glitches"),
                        i18n("Using the Force")
                    ]
        } else if (Daemon.isRunning && !Daemon.locked) {
            // Running but not doing anything worthwhile.
            options = [
                        i18n("Watering the flowers"),
                        i18n("Having a chat with Systemd"),
                        i18n("Ordering some pizza"),
                        i18n("Dusting the bookshelf"),
                        i18n("Learning the flute")
                    ]
        } else if (!Daemon.isRunning) {
            // Not even running
            options = [
                        i18n("zzzzZZZZZZzz")
                    ]
        }

        return options[Math.floor(Math.random() * options.length)];
    }

    Layout.minimumHeight: units.gridUnit * 12
    Layout.minimumWidth: units.gridUnit * 12
    Layout.preferredHeight: units.gridUnit * 20
    Layout.preferredWidth: units.gridUnit * 20
    //    Plasmoid.switchWidth: units.gridUnit * 12
    //    Plasmoid.switchHeight: units.gridUnit * 12

     // State is set through timer so we avoid state-switch spam (see below).
    Plasmoid.status: PlasmaCore.Types.PassiveStatus
    // FIXME: need to talk to AndreasK about iconing
    Plasmoid.icon: "apper" // plasmoid.status === PlasmaCore.Types.ActiveStatus ? "package-install" : "apper"
    Plasmoid.toolTipMainText: stateText()
    Plasmoid.toolTipSubText: flavorText()
    Plasmoid.compactRepresentation: PlasmaCore.IconItem {
        source: plasmoid.icon
        colorGroup: PlasmaCore.ColorScope.colorGroup
    }

    Connections {
        target: Daemon
        onIsRunningChanged: {
            plasmoid.toolTipMainText = stateText()
            plasmoid.toolTipSubText = flavorText()
        }
        onLockedChanged: {
            plasmoid.toolTipSubText = flavorText()
        }
    }

    StateSwitcher {}

    ListView {
        anchors.fill: parent
        model: FilteredTransactionModel {
            id: transactionModel
            filters: Role.mappedRoles() // Whitelist
            onCountChanged: plasmoid.toolTipSubText = flavorText()
        }
        delegate: TransactionListItem {}
    }

    PlasmaExtras.Heading {
        anchors.centerIn: parent
        level: 3
        opacity: 0.6
        text: plasmoid.toolTipSubText
        visible: transactionModel.count === 0
    }
}
