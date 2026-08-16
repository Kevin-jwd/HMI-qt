#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QDateTime>
#include <QSqlDatabase>

class QSqlQueryModel;
class QObject;

class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();
    bool openDatabase(QString *errorMessage = nullptr);
    bool initializeSchema(QString *errorMessage = nullptr);
    bool insertSystemEvent(const QString &eventType, QString *errorMessage = nullptr);
    bool insertVehicleLog(const QString &direction, int speed, QString *errorMessage = nullptr);
    bool insertSensorLog(double temperature, double humidity, const QString &fanState,
                         QString *errorMessage = nullptr);
    QSqlQueryModel *queryLogs(int logType, const QDateTime &start, const QDateTime &end,
                              QObject *parent, QString *errorMessage = nullptr);
    QString databasePath() const;

private:
    QString resolveDatabasePath() const;
    QSqlDatabase m_database;
};

#endif // DATABASEMANAGER_H
