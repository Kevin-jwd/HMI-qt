#include "databasemanager.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVariant>

namespace { const char kConnectionName[] = "vehicle_hmi_connection"; const char kFormat[] = "yyyy-MM-dd HH:mm:ss"; }

DatabaseManager::DatabaseManager() : m_database(QSqlDatabase::addDatabase("QSQLITE", kConnectionName)) {}

DatabaseManager::~DatabaseManager()
{
    if (m_database.isOpen()) m_database.close();
    m_database = QSqlDatabase();
    QSqlDatabase::removeDatabase(kConnectionName);
}

QString DatabaseManager::resolveDatabasePath() const
{
    const QString appDir = QCoreApplication::applicationDirPath();
    const QStringList candidates = { QDir(appDir).filePath("vehicle_hmi_dummy.db"),
        QDir(appDir).filePath("../QLabel/vehicle_hmi_dummy.db"),
        QDir::current().filePath("vehicle_hmi_dummy.db"), QDir::current().filePath("QLabel/vehicle_hmi_dummy.db") };
    for (const QString &path : candidates)
        if (QFileInfo::exists(path)) return QFileInfo(path).absoluteFilePath();
    return QDir(appDir).filePath("vehicle_hmi_dummy.db");
}

bool DatabaseManager::openDatabase(QString *errorMessage)
{
    m_database.setDatabaseName(resolveDatabasePath());
    if (m_database.open()) return true;
    if (errorMessage) *errorMessage = m_database.lastError().text();
    return false;
}

bool DatabaseManager::initializeSchema(QString *errorMessage)
{
    const QStringList statements = {
        "CREATE TABLE IF NOT EXISTS system_event (id INTEGER PRIMARY KEY AUTOINCREMENT, timestamp TEXT NOT NULL DEFAULT (datetime('now','localtime')), event_type TEXT NOT NULL)",
        "CREATE TABLE IF NOT EXISTS vehicle_log (id INTEGER PRIMARY KEY AUTOINCREMENT, timestamp TEXT NOT NULL DEFAULT (datetime('now','localtime')), direction TEXT NOT NULL CHECK(direction IN ('FWD','BACK','LEFT','RIGHT','STOP')), speed INTEGER NOT NULL CHECK(speed BETWEEN 0 AND 100))",
        "CREATE TABLE IF NOT EXISTS sensor_log (id INTEGER PRIMARY KEY AUTOINCREMENT, timestamp TEXT NOT NULL DEFAULT (datetime('now','localtime')), temperature REAL, humidity REAL, fan_state TEXT NOT NULL CHECK(fan_state IN ('ON','OFF')))",
        "CREATE INDEX IF NOT EXISTS idx_system_event_time ON system_event(timestamp)", "CREATE INDEX IF NOT EXISTS idx_vehicle_log_time ON vehicle_log(timestamp)",
        "CREATE INDEX IF NOT EXISTS idx_sensor_log_time ON sensor_log(timestamp)" };
    for (const QString &statement : statements) {
        QSqlQuery query(m_database);
        if (!query.exec(statement)) {
            if (errorMessage)
                *errorMessage = query.lastError().text();
            return false;
        }
    }
    return true;
}

namespace {
bool executeInsert(QSqlDatabase &database, const QString &sql, const QVariantList &values,
                   QString *errorMessage)
{
    QSqlQuery query(database);
    query.prepare(sql);
    for (const QVariant &value : values)
        query.addBindValue(value);
    if (query.exec())
        return true;
    if (errorMessage)
        *errorMessage = query.lastError().text();
    return false;
}
}

bool DatabaseManager::insertSystemEvent(const QString &eventType, QString *errorMessage)
{
    return executeInsert(m_database,
        QStringLiteral("INSERT INTO system_event (event_type) VALUES (?)"),
        {eventType}, errorMessage);
}

bool DatabaseManager::insertVehicleLog(const QString &direction, int speed, QString *errorMessage)
{
    QSqlQuery latest(m_database);
    if (!latest.exec(QStringLiteral(
            "SELECT direction, speed FROM vehicle_log ORDER BY id DESC LIMIT 1"))) {
        if (errorMessage)
            *errorMessage = latest.lastError().text();
        return false;
    }
    if (latest.next() && latest.value(0).toString() == direction
            && latest.value(1).toInt() == speed) {
        return true;
    }

    return executeInsert(m_database,
        QStringLiteral("INSERT INTO vehicle_log (direction, speed) VALUES (?, ?)"),
        {direction, speed}, errorMessage);
}

bool DatabaseManager::insertSensorLog(double temperature, double humidity,
                                      const QString &fanState, QString *errorMessage)
{
    return executeInsert(m_database,
        QStringLiteral("INSERT INTO sensor_log (temperature, humidity, fan_state) VALUES (?, ?, ?)"),
        {temperature, humidity, fanState}, errorMessage);
}

QSqlQueryModel *DatabaseManager::queryLogs(int logType, const QDateTime &start, const QDateTime &end,
                                           QObject *parent, QString *errorMessage)
{
    const QStringList sql = {
        "SELECT id AS 'ID', timestamp AS '시간', event_type AS '이벤트' FROM system_event WHERE timestamp BETWEEN ? AND ? ORDER BY timestamp DESC",
        "SELECT id AS 'ID', timestamp AS '시간', direction AS '방향', speed AS '속도' FROM vehicle_log WHERE timestamp BETWEEN ? AND ? ORDER BY timestamp DESC",
        "SELECT id AS 'ID', timestamp AS '시간', temperature AS '온도 (°C)', humidity AS '습도 (%)', fan_state AS 'FAN' FROM sensor_log WHERE timestamp BETWEEN ? AND ? ORDER BY timestamp DESC" };
    if (logType < 0 || logType >= sql.size()) { if (errorMessage) *errorMessage = QStringLiteral("지원하지 않는 데이터 종류입니다."); return nullptr; }
    QSqlQuery query(m_database); query.prepare(sql.at(logType)); query.addBindValue(start.toString(kFormat)); query.addBindValue(end.toString(kFormat));
    if (!query.exec()) { if (errorMessage) *errorMessage = query.lastError().text(); return nullptr; }
    auto *model = new QSqlQueryModel(parent); model->setQuery(query);
    if (model->lastError().isValid()) { if (errorMessage) *errorMessage = model->lastError().text(); delete model; return nullptr; }
    return model;
}

QString DatabaseManager::databasePath() const { return m_database.databaseName(); }
