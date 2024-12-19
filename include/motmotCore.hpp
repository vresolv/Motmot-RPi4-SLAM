#ifndef MOTMOT_CORE_HPP
#define MOTMOT_CORE_HPP

#include <cameraManager.hpp>
#include <imuManager.hpp>
#include <orbSlamManager.hpp>

#include <chrono>
#include <signal.h>

#include <iostream>
#include <string>

#include <unistd.h>

#include <QThread>
#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QCoreApplication>

class motmotCore : public QObject {
    Q_OBJECT
public:
    explicit motmotCore(QObject *parent = nullptr, int argc = 0, char *argv[] = nullptr);
    ~motmotCore();
    
    static void callSignalHandler(int num) {
        appCoreInstance->handleSignal(num);
    }
    
public slots:
    void closeApp();
    
signals:
    void startCam(int width, int height, int framerate);
    void stopCam();
    void startImu(QString port);
    void stopImu();
    
private:
    static void setSignalHandlerObject(motmotCore* coreObj) {
        motmotCore::appCoreInstance = coreObj;
    }
    
    void handleSignal(int param);
    static motmotCore* appCoreInstance;
    
    std::unique_ptr<cameraManager> pCamManager;
    std::unique_ptr<imuManager> pImuManager;
    std::unique_ptr<orbSlamManager> pOrbSlamManager;
    
    std::unique_ptr<QThread> pCamManagerThread;
    std::unique_ptr<QThread> pImuManagerThread;
    std::unique_ptr<QThread> pOrbSlamManagerThread;
};

#endif //MOTMOT_CORE_HPP
