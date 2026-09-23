#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QString>
#include <QLocalSocket>
#include <QTimer>
#include <QVariantList>
#include <QVariantMap>
#include <QtQml/qqmlregistration.h>
#include <iostream>

struct ProcessData {
    int pid;
    QString name;
    long ramKb;
};


class Client : public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int cpuUsage READ reportCpuUsage NOTIFY cpuUsageChanged)
    Q_PROPERTY(int ramUsage READ reportRamUsage NOTIFY ramUsageChanged)
    Q_PROPERTY(QString currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)
    Q_PROPERTY(QVariantList processList READ reportProcessList NOTIFY processListChanged)
public:
    explicit Client(QObject *parent = nullptr);

    int reportRamUsage() const;
    int reportCpuUsage() const;
    QString currentPage() const;
    QVariantList reportProcessList() const;
    Q_INVOKABLE int killProcess(int pid);
    Q_INVOKABLE int connectToServer();
    Q_INVOKABLE void print(const QString &message) {
        std::cout << "[CPP STDOUT] " << message.toStdString() << std::endl;
    }

signals:
    void cpuUsageChanged();
    void ramUsageChanged();
    void currentPageChanged();
    void processListChanged();

public slots:
    void setCurrentPage(const QString &data);

private slots:
    void sendRequest();
    void handleReadyRead();

private:
    int m_cpuUsage = 0;
    int m_ramUsage = 0;
    QString m_currentPage = "p";
    QList<ProcessData> m_processList;


    QLocalSocket m_socket;
    QTimer m_timer;
};

#endif