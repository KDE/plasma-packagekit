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

import org.kde.plasma.private.packagekit 0.1

PlasmaCore.SortFilterModel {
    sourceModel: TransactionModel {}

    property var filters: [] // Whitelist

    filterCallback: function(source_row, value) {
        var idx = sourceModel.index(source_row, 0);
        var data = sourceModel.data(idx, sourceModel.role("role"));
        for (var i = 0; i < filters.length; ++i) {
            if (data == filters[i]) {
                return true;
            }
        }
        return false;
    }
}
