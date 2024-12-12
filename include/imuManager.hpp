#ifndef IMU_MANAGER_HPP
#define IMU_MANAGER_HPP

#include <QThread>
#include <QObject>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

class imuManager : public QThread {
    Q_OBJECT
public:
    explicit imuManager(QObject *parent = nullptr);
    ~imuManager();
    void startImu(QString port);
    void stopImu();
    void run() override;

signals:
    void sendCurrentImuFrame(const QString &frame);

private:
    QSerialPort serialImu;
    bool isRunning = false;
};

#endif // IMU_MANAGER_HPP
