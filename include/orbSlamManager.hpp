#ifndef ORBSLAM_MANAGER_HPP
#define ORBSLAM_MANAGER_HPP

#include <System.h>
#include <ImuTypes.h>

#include <opencv2/core/core.hpp>

#include <chrono>

#include <QThread>
#include <QObject>
#include <QDebug>
#include <QString>
#include <QStringList>

#include <iostream>

//Hardcoded for now, for IMU sensor at 100 HZ, untested
#define IMU_VECTOR_MAX_SIZE 100
#define IMU_VECTOR_MIN_SIZE 45

class orbSlamManager : public QObject {
    Q_OBJECT
public:
    explicit orbSlamManager(QObject *parent);
    ~orbSlamManager();
    
    void initORBSLAM3(const string& strVocFile, const string &strSettingsFile, const bool &ifVisualize);
    void stopORBSLAM3();

public slots:
    void recCamData(cv::Mat frame);
    void recImuData(const QString &frame);
    
private:
    std::unique_ptr<ORB_SLAM3::System> pORBSLAM;
    float imgScale; 
    cv::Mat camFrame;
    double camTimestamp;
    std::vector<ORB_SLAM3::IMU::Point> imuData;
};

#endif //ORBSLAM_MANAGER_HPP
