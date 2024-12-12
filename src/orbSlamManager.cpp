#include <orbSlamManager.hpp>

orbSlamManager::orbSlamManager(QObject *parent, const string& strVocFile, const string &strSettingsFile, const bool &ifVisualize) {
    pORBSLAM = std::make_unique<ORB_SLAM3::System>(strVocFile, strSettingsFile, ORB_SLAM3::System::IMU_MONOCULAR, ifVisualize);
    imgScale = pORBSLAM->GetImageScale();
}

orbSlamManager::~orbSlamManager() {
    quit();
    wait();
}

void orbSlamManager::recCamData(cv::Mat frame) {
    auto now = std::chrono::system_clock::now();
    auto now_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();

    if (imgScale != 1.f) {
        int width = frame.cols * imgScale;
        int height = frame.rows * imgScale;
        cv::resize(frame, frame, cv::Size(width, height));
    }
    
    if (!pORBSLAM->isShutDown()) {
        pORBSLAM->TrackMonocular(frame, now_ns, imuData);
    }
    
    imuData.clear();

}

void orbSlamManager::recImuData(const QString &frame) {
    auto now = std::chrono::system_clock::now();
    auto now_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();

    QStringList components = frame.split(',');
	
    if (components.size() != 6) {
        qDebug() << "Invalid Imu Data Received";
	return;
    }

    bool conversionOk = true;
    double mx = components[0].toDouble(&conversionOk);
    double my = components[1].toDouble(&conversionOk);
    double mz = components[2].toDouble(&conversionOk);
    double ax = components[3].toDouble(&conversionOk);
    double ay = components[4].toDouble(&conversionOk);
    double az = components[5].toDouble(&conversionOk);

    if (!conversionOk) {
	qDebug() << "Error during conversion of IMU Data";
        return;
    }

    imuData.push_back(ORB_SLAM3::IMU::Point(ax, ay, az, mx, my, mz, now_ns));

    //std::cout << std::setprecision(std::numeric_limits<double>::max_digits10) << "Imu Data " << now_ns << " " << ax << " " << ay << " " << az << " " << mx << " " << my << " " << mz << " " << std::endl;

}
