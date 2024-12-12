#ifndef MOTMOT_CORE_HPP
#define MOTMOT_CORE_HPP

#include <cameraManager.hpp>
#include <imuManager.hpp>
#include <orbSlamManager.hpp>

#include <chrono>

#include <QThread>
#include <QObject>
#include <QDebug>

class motmotCore : public QObject {
    Q_OBJECT
public:
    explicit motmotCore(QObject *parent = nullptr, int argc = 0, char *argv[] = nullptr);
    ~motmotCore();

private:
    std::unique_ptr<cameraManager> pCamManager;
    std::unique_ptr<imuManager> pImuManager;
    std::unique_ptr<orbSlamManager> pOrbSlamManager;
};

#endif //MOTMOT_CORE_HPP
