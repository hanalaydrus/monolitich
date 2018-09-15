// include model

// model.getCameraDataByID
// model.getDensityDataByID
// model.getVolumeDataByID
// model.getPercentage
// model.getWeather in 600second
// create kalimat


#include "Semantic.h"

Semantic::Semantic(){

}
void counter(double duration){
	duration = 0;
	sleep(600000);
	duration = 600;
}

void getDensityData(Model model, int camera_id, string data, int exitFlag){
	for(;;){
		data = model.getDensityDataByID(camera_id);
		if (exitFlag == 1) {
			break;
		}
	}
}

void getVolumeData(Model model, int camera_id, vector<boost::variant<int, string>> data, int exitFlag){
	for(;;){
		data = model.getVolumeDataByID(camera_id);
		if (exitFlag == 1) {
			break;
		}
	}
}

void getPercentageData(Model model, int camera_id, vector<boost::variant<int, string>>volumeData, float data, int exitFlag){
	for(;;){
		if (!volumeData.empty()){
			data = model.getPercentage(camera_id, boost::get<string>(volumeData[0]), boost::get<int>(volumeData[1]));
		}
		if (exitFlag == 1) {
			break;
		}
	}
}

void getWeatherData(Model model, map<string, boost::variant<int, string>> cameraData, string data, double duration, int exitFlag){
	for (;;){
		if (duration == 600 && !cameraData.empty()){
			data = model.getWeather(boost::get<string>(cameraData["latitude"]), boost::get<string>(cameraData["longitude"]));
		}
		if (exitFlag == 1) {
			break;
		}
	}
}

void Semantic::runSemanticService(ServerContext* context, ServerWriter<HelloReply>* writer, int camera_id){
	Model model;
	map<string, boost::variant<int, string>> cameraData;
	string densityData;
	vector<boost::variant<int, string>> volumeData;
	float percentageData;
	string percentageDataConv;
	string weatherData;
	string sentence;
	int exitFlag = 0;

	double duration;
	thread tCounter (counter, duration);

	cameraData = model.getCameraDataByID(camera_id);

	thread densityThread(getDensityData, model, camera_id, densityData, exitFlag);
	thread volumeThread(getVolumeData, model, camera_id, volumeData, exitFlag);
	thread percentageThread(getPercentageData, model, camera_id, volumeData, percentageData, exitFlag);
	thread weatherThread(getWeatherData, model, cameraData, weatherData, duration, exitFlag);

	for(;;){
		if ((weatherData.empty()) && (densityData.empty()) && (percentageData != percentageData)) {
			sentence = boost::get<string>(cameraData["street_name"]) + ".";
		} else {
			percentageDataConv = boost::lexical_cast<std::string>(round(percentageData));

			transform(weatherData.begin(), weatherData.end(), weatherData.begin(), ::tolower);
			size_t found = weatherData.find("hujan");

			if ((densityData.empty()) && !(percentageData != percentageData)){

				if (found!=std::string::npos){
					if (percentageData > 0){
						sentence = "Hujan mengguyur " + boost::get<string>(cameraData["street_name"]) + ". Terjadi kenaikan volume kendaraan sebesar " + percentageDataConv + " persen dibandingkan lalu lintas normal.";
					} else if (percentageData == 0) {
						sentence = "Hujan mengguyur " + boost::get<string>(cameraData["street_name"]) + ". Volume lalu lintas terpantau normal.";
					} else {
						sentence = "Hujan mengguyur " + boost::get<string>(cameraData["street_name"]) + ". Terjadi penurunan volume kendaraan sebesar " + percentageDataConv + " persen dibandingkan lalu lintas normal.";
					}
				} else {
					if (percentageData > 0){
						sentence = "Pada, " + boost::get<string>(cameraData["street_name"]) + ", terjadi kenaikan volume kendaraan sebesar " + percentageDataConv + " persen dibandingkan lalu lintas normal.";
					} else if (percentageData == 0) {
						sentence = "Pada, " + boost::get<string>(cameraData["street_name"]) + ", volume lalu lintas terpantau normal.";
					} else {
						sentence = "Pada, " + boost::get<string>(cameraData["street_name"]) + ", terjadi penurunan volume kendaraan sebesar " + percentageDataConv + " persen dibandingkan lalu lintas normal.";
					}
				}
			} else if ((!densityData.empty()) && (percentageData != percentageData)) {
				if (found!=std::string::npos) {
					sentence = "Hujan mengguyur " + boost::get<string>(cameraData["street_name"]) + ". Arus lalu lintas terpantau " + densityData + ".";
				} else {
					sentence = boost::get<string>(cameraData["street_name"]) + " terpantau " + densityData + ".";
				}
			} else if ((!densityData.empty()) && !(percentageData != percentageData)){
				if (found!=std::string::npos) {
					sentence = "Hujan mengguyur " + boost::get<string>(cameraData["street_name"]) + ". Arus lalu lintas terpantau " + densityData + ".";
				} else {
					sentence = boost::get<string>(cameraData["street_name"]) + " terpantau " + densityData + ".";
				}

				if (percentageData > 0){
					sentence = sentence + " Terjadi kenaikan volume kendaraan sebesar " + percentageDataConv + " persen dibandingkan lalu lintas normal.";
				} else if (percentageData == 0){
					sentence = sentence + " Volume lalu lintas terpantau normal.";
				} else {
					sentence = sentence + " Terjadi penurunan volume kendaraan sebesar " + percentageDataConv + " persen dibandingkan lalu lintas normal.";
				}
			}
		}

		HelloReply r;
		r.set_response(sentence);
		writer->Write(r);

		if (context->IsCancelled()){
			exitFlag = 1;
			tCounter.join();
			densityThread.join();
			volumeThread.join();
			percentageThread.join();
			weatherThread.join();
			break;
		}
	}
}