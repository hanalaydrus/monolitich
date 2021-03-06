// Model.h

#ifndef MY_MODEL
#define MY_MODEL

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <boost/variant.hpp>
#include <curl/curl.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;
///////////////////////////////////////////////////////////////////////////////////////////////////
class Model {
public:
    // function prototypes ////////////////////////////////////////////////////////////////////////
    Model();
    
    // get camera data by id [NEW]
    map<string, boost::variant<int, string>> getCameraDataByID(int camera_id);

    // get all density config
    vector< map<string, boost::variant<int, string, map<string, int>>> > getAllDensityConfig();

    // store density data
    void storeDensityData(int camera_id, string density_state);

    // get density daya by id
    string getDensityDataByID(int camera_id);

    // get all volume config
    vector< map<string, boost::variant<int, string>> > getAllVolumeConfig();

    // store volume data
    void storeVolumeData(int camera_id, int volume_size);

    // get volume data by id
	vector<boost::variant<int, string>> getVolumeDataByID(int camera_id);

    // get volume percentage by id
    float getPercentage(int camera_id, string date_time, int volume_size);

    // get weather [NEW]
    string getWeather(string latitude, string longitude);
    
};

#endif    // MY_MODEL
