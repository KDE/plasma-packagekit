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

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <PackageKit/Transaction>

// Override PackageKit transaction to give it a functional lastPackage property.
// The PK one doesn't actually return a non-empty string... ever.
class Transaction : public PackageKit::Transaction
{
    Q_OBJECT
    Q_PROPERTY(QString tidString READ tidString)
    Q_PROPERTY(QString lastPackage READ lastPackage NOTIFY lastPackageChanged)
public:
    Transaction(const QDBusObjectPath &path = QDBusObjectPath());

    QString tidString() const;
    QString lastPackage() const;

    Q_INVOKABLE QString enumString(Role role);

signals:
    void tidStringChanged();
    void lastPackageChanged();

private:
    QString m_lastPackage;
};

#endif // TRANSACTION_H
