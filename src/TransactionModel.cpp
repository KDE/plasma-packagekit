/*
    Copyright © 2017 Harald Sitter <sitter@kde.org>

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

#include "TransactionModel.h"

#include <QMetaEnum>

#include <PackageKit/Daemon>

#include "Debug.h"
#include "Transaction.h"

TransactionModel::TransactionModel(QObject *parent)
    : AbstractModel(&m_objMap, parent)
{
    initRoleNames(Transaction::staticMetaObject);

    connect(PackageKit::Daemon::global(), &PackageKit::Daemon::transactionListChanged,
            this, &TransactionModel::onTransactionsChanged);

    // Fetch what is available right now.
    auto reply = PackageKit::Daemon::getTransactionList();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    QObject::connect(watcher, &QDBusPendingCallWatcher::finished,
                     [&](QDBusPendingCallWatcher *call) {
        QDBusPendingReply<QList<QDBusObjectPath>> reply = *call;
        onTransactionObjectPathsChanged(reply.argumentAt<0>());
        call->deleteLater();
    });
}

void TransactionModel::onTransactionObjectPathsChanged(const QList<QDBusObjectPath> &newTids)
{
    // Convert to StringList. Slight waste of time, but probably not slower as this is only used
    // For the initial list, whereas the StringList based code is used for everything else.
    QStringList strList;
    for (auto objectPath : newTids) {
        strList << objectPath.path();
    }

    onTransactionsChanged(strList);

    if (m_objMap.isEmpty()) {
        // On start suggest that the daemon quits again if it has no.
        // It ought to be noted that later we should no longer suggest this as it can cause quits
        // clients do not expect.
        PackageKit::Daemon::suggestDaemonQuit();
    }
}

void TransactionModel::onTransactionsChanged(const QStringList &newTids)
{
    qDebug() << "transactions changed";
    qDebug() << newTids;
    for (auto it = m_objMap.begin(); it != m_objMap.end();) {
        if (newTids.contains(it.key())) {
            // Keep
            ++it;
        } else {
            qDebug() << "dropping transaction" << it.key();
            // Delete
            auto index = m_objMap.keys().indexOf(it.key());
            auto transaction = it.value();
            it = m_objMap.erase(it);
            onDataRemoved(index);
            transaction->deleteLater();
        }
    }
    auto knownTids = m_objMap.keys();
    for (const auto &tid : newTids) {
        if (knownTids.contains(tid)) {
            continue;
        }

        qDebug() << "adding transaction" << tid;
        auto it = m_objMap.insert(tid, new Transaction(QDBusObjectPath(tid)));
        onDataAdded(m_objMap.keys().indexOf(it.key()));

    }
}
