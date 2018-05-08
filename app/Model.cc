// Model.cpp

#include "Model.h"

Model::Model(){

}

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

// get camera data by id [NEW]
map<string, boost::variant<int, string>> Model::getCameraDataByID(int camera_id){

	map<string, boost::variant<int, string>> data;

	while (true) {
		try {
			sql::Driver *driver;
			sql::Connection *con;
			sql::Statement *stmt;
			sql::ResultSet *res;

			/* Create a connection */
			driver = get_driver_instance();
			con = driver->connect("tcp://db-mono:3306", "root", "root");

	        // configuration
	        con->setSchema("traffic_detection");
	        stmt = con->createStatement();

	        ostringstream query;
	        query << "SELECT * FROM `camera` WHERE `camera_id` = " << camera_id;
			res = stmt->executeQuery(query.str());

	        while (res->next()) {
	        /* Access column data by alias or column name */
	            data["camera_id"] = res->getInt("camera_id");
	            data["url"] = res->getString("url");
	            data["street_name"] = res->getString("street_name");
	            data["latitude"] = res->getString("latitude");
	            data["longitude"] = res->getString("longitude");
	        }

			delete res;
			delete stmt;
			delete con;
		  	break;

		} catch (sql::SQLException &e) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
	}

	return data;
}

// get density config by id
vector<map<string, boost::variant<int, string, map<string, int> > > > Model::getAllDensityConfig(){
	vector<map<string, boost::variant<int, string, map<string, int> > > > camera;
    int mask_points_id;

    while(true){
		try {
			sql::Driver *driver;
			sql::Connection *con;
			sql::Statement *stmt;
			sql::ResultSet *res;
		  
			/* Create a connection */
			driver = get_driver_instance();
			con = driver->connect("tcp://db-mono:3306", "root", "root");
			
			con->setSchema("traffic_detection");
	        stmt = con->createStatement();

			res = stmt->executeQuery("SELECT * FROM `camera`");

	        while (res->next()) {
	        /* Access column data by alias or column name */
				map<string, boost::variant<int, string, map<string, int> > > data;

	            data["camera_id"] = res->getInt("camera_id");
	            data["url"] = res->getString("url");

	            camera.push_back(data);
	        }
			/* Connect to the MySQL  database */
	        // configuration
			res = stmt->executeQuery("SELECT * FROM `density_configuration`");
			int i = 0;

	        while (res->next()) {
	        /* Access column data by alias or column name */
	        	map<string, boost::variant<int, string, map<string, int> > > data;
	        	data = camera[i];

	            data["real_width"] = res->getInt("real_street_width");
	            data["real_height"] = res->getInt("real_street_height");
	            data["edge_thresh"] = res->getInt("edge_threshold");
	            data["low_thresh"] = res->getInt("low_threshold");
	            data["max_thresh"] = res->getInt("max_threshold");
	            data["morph_iteration"] = res->getInt("morph_closing_iteration");

	            camera.at(i) = data;
	            i++;
	        }

	        res = stmt->executeQuery("SELECT * FROM `street_mask_points`");
	        i = 0;
	        while (res->next()) {
	        /* Access column data by alias or column name */
	            map<string, boost::variant<int, string, map<string, int> > > data;
	        	data = camera[i];
	            map<string, int> data_mask_point;

	            data_mask_point["x_lb"] = res->getInt("x_lb");
	            data_mask_point["x_lt"] = res->getInt("x_lt");
	            data_mask_point["x_rb"] = res->getInt("x_rb");
	            data_mask_point["x_rt"] = res->getInt("x_rt");
	            data_mask_point["y_lb"] = res->getInt("y_lb");
	            data_mask_point["y_lt"] = res->getInt("y_lt");
	            data_mask_point["y_rb"] = res->getInt("y_rb");
	            data_mask_point["y_rt"] = res->getInt("y_rt");
	            data["mask_points"] = data_mask_point;

	            camera.at(i) = data;
	            i++;
	        }

			delete res;
			delete stmt;
			delete con;
		  	break;

		} catch (sql::SQLException &e) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
	}
	  
	return camera;
}

void Model::storeDensityData(int camera_id, string density_state) {
    // Input : camera_id, density_state
	// Output : -
    bool isExist = false;
    int density_history_id;
    string current_density_state;
    while (true){
		try {
			sql::Driver *driver;
			sql::Connection *con;
			sql::Statement *stmt;
			sql::ResultSet *res;
		  
			/* Create a connection */
			driver = get_driver_instance();
			con = driver->connect("tcp://db-mono:3306", "root", "root");
			/* Connect to the MySQL  database */
			con->setSchema("traffic_detection");
		  
	        stmt = con->createStatement();
	        ostringstream query;
	        query << "SELECT * FROM `density_history` WHERE DATE(`date_time`) = CURDATE() AND `camera_id` = " << camera_id;
			res = stmt->executeQuery(query.str());
	        
	        while (res->next()) {
	          /* Access column data by alias or column name */
	            isExist = true;
	            density_history_id = res->getInt(1);
	            current_density_state = res->getString("density_state");
	        }

	        if (isExist && (current_density_state != density_state)) {
	            stmt = con->createStatement();
	            ostringstream query;
	            query << "UPDATE `density_history` SET `date_time` = CURRENT_TIMESTAMP, `density_state` = '" << density_state << "' WHERE `density_history_id` = " << density_history_id;
	            stmt->execute(query.str());
	        } else if (!isExist) {
	            stmt = con->createStatement();
	            ostringstream query;
	            query << "INSERT INTO `density_history` (`camera_id`, `density_state`) VALUES (" << camera_id << ", '" << density_state << "')";
	            stmt->execute(query.str());
	        }

			delete res;
			delete stmt;
			delete con;
		  	break;

		} catch (sql::SQLException &e) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
	}
}

string Model::getDensityDataByID(int camera_id) {
    // Input : camera_id
	// Output : density_state
	string response;

	while(true){
		try {
			sql::Driver *driver;
			sql::Connection *con;
			sql::Statement *stmt;
			sql::ResultSet *res;
		  
			/* Create a connection */
			driver = get_driver_instance();
			con = driver->connect("tcp://db-mono:3306", "root", "root");
			/* Connect to the MySQL test database */
			con->setSchema("traffic_detection");
		  
			stmt = con->createStatement();
			ostringstream query;
			query << "SELECT * FROM `density_history` WHERE `camera_id` = " << camera_id;
			res = stmt->executeQuery(query.str());

			while (res->next()) {
				response = res->getString("density_state");
			}

			delete res;
			delete stmt;
			delete con;
		  	break;

		} catch (sql::SQLException &e) {
	        response = "";
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
	}

	 return response;
}

// get all volume config
vector< map<string, boost::variant<int, string>> > Model::getAllVolumeConfig(){
	vector< map<string, boost::variant<int, string>> > cameras;

	while (true) {
		try {
			sql::Driver *driver;
			sql::Connection *con;
			sql::Statement *stmt;
			sql::ResultSet *res;

			/* Create a connection */
			driver = get_driver_instance();
			con = driver->connect("tcp://db-mono:3306", "root", "root");

	        // configuration
	        con->setSchema("traffic_detection");
	        stmt = con->createStatement();

			res = stmt->executeQuery("SELECT * FROM `camera`");
	        while (res->next()) {
	        /* Access column data by alias or column name */
	        	map<string, boost::variant<int, string>> data;
	        	data["url"] = res->getString("url");

            	cameras.resize(res->getInt("camera_id") + 1);
            	cameras.at(res->getInt("camera_id")) = data;
	        }

	        res = stmt->executeQuery("SELECT * FROM `volume_configuration`");
	        while (res->next()) {
	        /* Access column data by alias or column name */
	            map<string, boost::variant<int, string>> data;
	            data = cameras[res->getInt("camera_id")];

	            data["x0"] = res->getInt("crossing_line_x0");
	            data["y0"] = res->getInt("crossing_line_y0");
	            data["x1"] = res->getInt("crossing_line_x1");
	            data["y1"] = res->getInt("crossing_line_y1");

	            cameras.at(res->getInt("camera_id")) = data;
	        }

			delete res;
			delete stmt;
			delete con;
		  	break;

		} catch (sql::SQLException &e) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
	}

	return cameras;
}

// store volume data
void Model::storeVolumeData(int camera_id, int volume_size) {
	bool isExist = false;
    int volume_history_id, current_volume_size;
    while (true){
		try {
			sql::Driver *driver;
			sql::Connection *con;
			sql::Statement *stmt;
			sql::ResultSet *res;

			/* Create a connection */
			driver = get_driver_instance();
			con = driver->connect("tcp://db-mono:3306", "root", "root");
			/* Connect to the MySQL  database */
			con->setSchema("traffic_detection");
		  
	        stmt = con->createStatement();
	        ostringstream query;
	        query << "SELECT * FROM `volume_history` WHERE DATE(`date_time`) = CURDATE() AND `camera_id` = " << camera_id;
			res = stmt->executeQuery(query.str());
	        
	        while (res->next()) {
	          /* Access column data by alias or column name */
	            isExist = true;
	            volume_history_id = res->getInt(1);
	            current_volume_size = res->getInt("volume_size");
	        }

	        if (isExist && (current_volume_size != volume_size)) {
	            stmt = con->createStatement();
	            ostringstream query;
	            query << "UPDATE `volume_history` SET `date_time` = CURRENT_TIMESTAMP, `volume_size` = " << volume_size << " WHERE `volume_history_id` = " << volume_history_id;
	            stmt->execute(query.str());
	        } else if (!isExist) {
	            stmt = con->createStatement();
	            ostringstream query;
	            query << "INSERT INTO `volume_history` (`camera_id`, `volume_size`) VALUES (" << camera_id << ", " << volume_size << ")";
	            stmt->execute(query.str());
	        }

			delete res;
			delete stmt;
			delete con;
			break;
		  
		} catch (sql::SQLException &e) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
	}
}

// get volume data by id
vector<boost::variant<int, string>> Model::getVolumeDataByID(int camera_id){
	vector<boost::variant<int, string>> response;
	while (true){
		try {
			sql::Driver *driver;
			sql::Connection *con;
			sql::Statement *stmt;
			sql::ResultSet *res;

			/* Create a connection */
			driver = get_driver_instance();
			con = driver->connect("tcp://db-mono:3306", "root", "root");
		  
			/* Connect to the MySQL test database */
			con->setSchema("traffic_detection");
		  
			stmt = con->createStatement();
			ostringstream query;
			query << "SELECT * FROM `volume_history` WHERE DATE(`date_time`) = CURDATE() AND `camera_id` = " << camera_id;
			res = stmt->executeQuery(query.str());
			while (res->next()) {
				response.push_back(res->getString("date_time"));
				// cout << "count mysql : " << res->getInt("volume_size") << ", camera_id: " << camera_id << endl;
				response.push_back(res->getInt("volume_size"));
			}
			delete res;
			delete stmt;
			delete con;
		  	break;
		} catch (sql::SQLException &e) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
	}

	return response;
}

// get volume percentage by id
float Model::getPercentage(int camera_id, string date_time, int volume_size){
	float percentage;
	int volume_normal_size;
	while(true){
		try {
			sql::Driver *driver;
			sql::Connection *con;
			sql::Statement *stmt;
			sql::ResultSet *res;

			/* Create a connection */
			driver = get_driver_instance();
			con = driver->connect("tcp://db-mono:3306", "root", "root");
		  
			/* Connect to the MySQL test database */
			con->setSchema("traffic_detection");
		  
			stmt = con->createStatement();
			ostringstream query;
			query << "SELECT * FROM `volume_normal` WHERE `camera_id` = " << camera_id << " ORDER BY ABS(`time` - TIME('" << date_time << "')) LIMIT 1";
			res = stmt->executeQuery(query.str());
			while (res->next()) {
				volume_normal_size = res->getInt("volume_normal_size");
			}
			delete res;
			delete stmt;
			delete con;
		  	break;
		} catch (sql::SQLException &e) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
	}
	// cout << "volume_size: " << volume_size << ", volume_normal_size" << volume_normal_size << endl; 
	percentage = ((( (float)volume_size - (float)volume_normal_size ) / (float)volume_normal_size) * 100);
	
	return percentage;
}

// get weather [NEW]
string Model::getWeather(string latitude, string longitude){
	string response_string;
	string weather;

    CURL *curl;
	CURLcode res;

	while(true){
		curl = curl_easy_init();
		string url = "http://dataservice.accuweather.com/locations/v1/cities/geoposition/search?apikey=zUAxVR88QcZr5j4hpl4IxMUnuxixTnfd&q=";
		url = url + latitude + "," + longitude;

		if(curl) {
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			/* example.com is redirected, so we tell libcurl to follow redirection */ 
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

			/* Perform the request, res will get the return code */ 
			res = curl_easy_perform(curl);
			/* Check for errors */ 
			if(res != CURLE_OK) {
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			 	continue;
			}

			/* always cleanup */ 
			curl_easy_cleanup(curl);
		}

		auto j = json::parse(response_string);
		string locationKey = j["Key"];
		url = "http://dataservice.accuweather.com/currentconditions/v1/"+ locationKey +"?apikey=zUAxVR88QcZr5j4hpl4IxMUnuxixTnfd&language=id-ID";

		if(curl) {
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			/* example.com is redirected, so we tell libcurl to follow redirection */ 
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

			/* Perform the request, res will get the return code */ 
			res = curl_easy_perform(curl);
			/* Check for errors */ 
			if(res != CURLE_OK){
			  	fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			  	continue;
			}

			/* always cleanup */ 
			curl_easy_cleanup(curl);
		}

		j = json::parse(response_string);
		weather = j[0]["WeatherText"];
		break;
	}

	return weather;

}