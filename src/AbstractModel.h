/*
    Copyright © 2014-2017 Harald Sitter <sitter@kde.org>
    Copyright © 2016 David Rosca <nowrep@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H

#include <QAbstractListModel>

class AbstractModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ItemRole {
        ObjectRole = Qt::UserRole + 1
    };

    AbstractModel(QMap<QString, QObject*> *map, QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const Q_DECL_FINAL;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_FINAL;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    // setData removed, PK is not writable. setData reference in plasma-pa repo.

    Q_INVOKABLE int role(const QByteArray &roleName) const;

protected:
    void initRoleNames(const QMetaObject &qobjectMetaObject);
    void onDataAdded(int index);
    void onDataRemoved(int index);

private slots:
    void propertyChanged();

private:
    QMetaMethod propertyChangedMetaMethod() const;

    QHash<int, QByteArray> m_roles;
    QHash<int, int> m_objectProperties;
    QHash<int, int> m_signalIndexToProperties;

    QMap<QString, QObject*> *m_map;

private:
    // Prevent leaf-classes from default constructing as we want to enforce
    // them passing us a context or explicit nullptrs.
    AbstractModel() {}
};

#endif // ABSTRACTMODEL_H
