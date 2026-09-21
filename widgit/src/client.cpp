#include "client.h"
#include <iostream>
#include <QDebug>

Client::Client(QObject *parent) : QObject(parent) {
    connect(&m_socket, &QLocalSocket::readyRead, this, &Client::handleReadyRead);
    connect(&m_timer, &QTimer::timeout, this, &Client::sendRequest);
}

QString Client::currentPage() const {
    return m_currentPage;
}

void Client::setCurrentPage(const QString &data)
{
    if (m_currentPage == data)
        return;

    m_currentPage = data;
    emit currentPageChanged();

    if (m_currentPage == "p")
        m_timer.setInterval(3000);
    else
        m_timer.setInterval(1000);

    if (m_socket.state() == QLocalSocket::ConnectedState) {
        m_socket.write(m_currentPage.toUtf8() + "\n");
    }
}


int Client::reportCpuUsage() const {
    return m_cpuUsage;
}

int Client::reportRamUsage() const {
    return m_ramUsage;
}

QVariantList Client::reportProcessList() const
{
    QVariantList result;

    for (const auto& process : m_processList) {
        QVariantMap item;

        item["pid"] = process.pid;
        item["name"] = process.name;
        item["ramKb"] = static_cast<qint64>(process.ramKb);

        result.append(item);
    }

    return result;
}

int Client::killProcess(int pid)
{
    std::string msg = std::to_string(pid) + "k\n";

    if (m_socket.write(msg.c_str(), msg.size())) {
        return 1;
    }

    return 0;
}


int Client::connectToServer() {

    m_socket.connectToServer("/tmp/sysmon.sock");

    if (!m_socket.waitForConnected(1000)) {
        qWarning() << "Failed to connect to daemon socket!";
        return -1;
    }

    m_timer.start(m_currentPage == "p" ? 3000 : 1000);
    return 1;
}

void Client::sendRequest() {
    if (m_socket.state() != QLocalSocket::ConnectedState)
        return;
    m_socket.write(m_currentPage.toUtf8() + "\n");
}

// void Client::handleReadyRead() {
//     QByteArray data = m_socket.readAll().trimmed();

//     QList<QByteArray> parts = data.split(' ');
//     if (parts.size() >= 2) {
//         m_cpuUsage = parts[0].toInt();
//         m_ramUsage = parts[1].toInt();

//         emit cpuUsageChanged();
//         emit ramUsageChanged();
//     }
// }

void Client::handleReadyRead()
{
    QByteArray data = m_socket.readAll().trimmed();

    if (m_currentPage == "p") {
        m_processList.clear();

        QList<QByteArray> lines = data.split('\n');

        for (const QByteArray& line : lines) {
            QList<QByteArray> parts = line.split('|');

            if (parts.size() != 3)
                continue;

            ProcessData process;
            process.pid = parts[0].toInt();
            process.name = QString::fromUtf8(parts[1]);
            process.ramKb = parts[2].toLong();

            m_processList.append(process);
        }

        std::sort(m_processList.begin(), m_processList.end(),
                  [](const ProcessData &a, const ProcessData &b) {
                      return a.ramKb > b.ramKb;
                  });

        emit processListChanged();
    }
    else if (m_currentPage == "s") {
        QList<QByteArray> parts = data.split(' ');

        if (parts.size() >= 2) {
            m_cpuUsage = parts[0].toInt();
            m_ramUsage = parts[1].toInt();

            emit cpuUsageChanged();
            emit ramUsageChanged();
        }
    }
}

