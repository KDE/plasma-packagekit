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

#include "AbstractModel.h"

#include <QMetaProperty>

#include "Debug.h"

AbstractModel::AbstractModel(QMap<QString, QObject *> *map, QObject *parent)
    : QAbstractListModel(parent)
    , m_map(map)
{
}

QHash<int, QByteArray> AbstractModel::roleNames() const
{
    if (!m_roles.empty()) {
        qCDebug(PLASMAPK) << "returning roles" << m_roles;
        return m_roles;
    }
    Q_UNREACHABLE();
    return QHash<int, QByteArray>();
}

int AbstractModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_map->count();
}

QVariant AbstractModel::data(const QModelIndex &index, int role) const
{
    auto data = m_map->values().at(index.row());
    Q_ASSERT(data);
    if (role == ObjectRole) {
        return QVariant::fromValue(data);
    }
    int property = m_objectProperties.value(role, -1);
    if (property == -1) {
        qCDebug(PLASMAPK) << "failed to resolve property " << role;
        return QVariant();
    }
    return data->metaObject()->property(property).read(data);
}

int AbstractModel::role(const QByteArray &roleName) const
{
    qCDebug(PLASMAPK) << roleName << m_roles.key(roleName, -1);
    return m_roles.key(roleName, -1);
}

void AbstractModel::initRoleNames(const QMetaObject &qobjectMetaObject)
{
    m_roles[ObjectRole] = QByteArrayLiteral("ModelObject");

    QMetaEnum enumerator;
    for (int i = 0; i < metaObject()->enumeratorCount(); ++i) {
        if (metaObject()->enumerator(i).name() == QLatin1String("ItemRole")) {
            enumerator = metaObject()->enumerator(i);
            break;
        }
    }

    for (int i = 0; i < enumerator.keyCount(); ++i) {
        // Clip the Role suffix and glue it in the hash.
        const int roleLength = 4;
        QByteArray key(enumerator.key(i));
        // Enum values must end in Role or the enum is crap
        Q_ASSERT(key.right(roleLength) == QByteArrayLiteral("Role"));
        key.chop(roleLength);
        m_roles[enumerator.value(i)] = key;
    }

    int maxEnumValue = -1;
    for (auto it = m_roles.constBegin(); it != m_roles.constEnd(); ++it) {
        if (it.key() > maxEnumValue) {
            maxEnumValue = it.key();
        }
    }
    Q_ASSERT(maxEnumValue != -1);
    auto mo = qobjectMetaObject;
    for (int i = 0; i < mo.propertyCount(); ++i) {
        QMetaProperty property = mo.property(i);
        QString name(property.name());
        // Coerece first char to be lower case to ensure we are consistent there.
        name.replace(0, 1, name.at(0).toLower());
        m_roles[++maxEnumValue] = name.toLatin1();
        m_objectProperties.insert(maxEnumValue, i);
        if (!property.hasNotifySignal()) {
            continue;
        }
        m_signalIndexToProperties.insert(property.notifySignalIndex(), i);
    }
    qCDebug(PLASMAPK) << m_roles;

    // Connect to property changes also with objects already in model
    for (int i = 0; i < m_map->keys().count(); ++i) {
        onDataAdded(i);
    }
}

void AbstractModel::propertyChanged()
{
    if (!sender() || senderSignalIndex() == -1) {
        return;
    }
    int propertyIndex = m_signalIndexToProperties.value(senderSignalIndex(), -1);
    if (propertyIndex == -1) {
        return;
    }
    int role = m_objectProperties.key(propertyIndex, -1);
    if (role == -1) {
        return;
    }
    int index = m_map->values().indexOf(sender());
    qCDebug(PLASMAPK) << "PROPERTY CHANGED (" << index << ") :: " << role << roleNames().value(role);
    emit dataChanged(createIndex(index, 0), createIndex(index, 0), {role});
}

void AbstractModel::onDataAdded(int index)
{
    beginInsertRows(QModelIndex(), index, index);
    QObject *data = m_map->values().at(index);
    const QMetaObject *mo = data->metaObject();
    // We have all the data changed notify signals already stored
    auto keys = m_signalIndexToProperties.keys();
    foreach (int index, keys) {
        QMetaMethod meth = mo->method(index);
        connect(data, meth, this, propertyChangedMetaMethod());
    }
    endInsertRows();
}

void AbstractModel::onDataRemoved(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    endRemoveRows();
}

QMetaMethod AbstractModel::propertyChangedMetaMethod() const
{
    auto mo = metaObject();
    int methodIndex = mo->indexOfMethod("propertyChanged()");
    if (methodIndex == -1) {
        return QMetaMethod();
    }
    return mo->method(methodIndex);
}
