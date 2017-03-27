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
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras

import org.kde.plasma.private.packagekit 0.1

import "../code/role.js" as Role

PlasmaComponents.ListItem {
    ColumnLayout {
        width: parent.width

        RowLayout {
            PlasmaExtras.Heading {
                function roleText() {
                    var str = Role.localizedString(role)
                    if (str) {
                        console.debug(str)
                        return str;
                    }
                    console.debug("FAILED TO MAP %1 defaulting to %2".arg(role.valueOf()).arg(role.toLocaleString()))
                    console.debug(Transaction.Role)
                    return ModelObject.enumString(role) + " " + role.valueOf()
                }

                Layout.fillWidth: true
                level: 2
                wrapMode: Text.NoWrap
                elide: Text.ElideRight
                text: roleText() + " " // We want a white space spacing to the tid.
            }

            PlasmaExtras.Heading {
                level: 6
                wrapMode: Text.NoWrap
                elide: Text.ElideRight
                text: "[%2]".arg(tidString)
            }
        }

        PlasmaExtras.Heading {
            Layout.fillWidth: true
            level: 5
            wrapMode: Text.NoWrap
            elide: Text.ElideRight
            text: lastPackage || ""
//            onTextChanged: {
//                console.debug("!!")
//                console.debug("!!")
//                console.debug("!!")
//                console.debug("!!")
//                console.debug("!!")
//                console.debug("!!")
//                console.debug("!!")
//                console.debug("!!")
//                console.debug("!!")
//                console.debug("!!")
//                console.debug("!!")
//                console.debug("!!")
//                console.debug("!!")
//                console.debug(text)
//                console.debug(text != "")
//                console.debug(visible)
//            }
//            onVisibleChanged:{
//                console.debug("~~")
//                console.debug("~~")
//                console.debug("~~")
//                console.debug("~~")
//                console.debug("~~")
//                console.debug("~~")
//                console.debug("~~")
//                console.debug("~~")
//                console.debug("~~")
//                console.debug("~~")
//                console.debug(text)
//                console.debug(text != "")
//                console.debug(visible)
//            }
            visible: text != ""
        }
        PlasmaExtras.Heading {
            id: debugHeading
            visible: false
            Layout.fillWidth: true
            level: 5
            wrapMode: Text.NoWrap
            elide: Text.ElideRight
            text: "%1 -- %2 (%3:%4)".arg(lastPackage).arg(percentage).arg(status).arg(role)
        }
    }
}
