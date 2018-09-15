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
void counter(atomic<bool>& startRequest, atomic<bool>& exitFlag){
	while(true){
		if(exitFlag) {
			break;
		}
		clock_t goal = (600000 * CLOCKS_PER_SEC/1000) + clock();
		while (goal > clock()){
			if(exitFlag) {
				break;
			}

		}
		startRequest = true;
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
	atomic<bool> startRequest{true};
	atomic<bool> exitFlag{false};
	thread tCounter(counter, ref(startRequest), ref(exitFlag));
	
	cameraData = model.getCameraDataByID(camera_id);

	for(;;){
		densityData = model.getDensityDataByID(camera_id);
		volumeData = model.getVolumeDataByID(camera_id);
		percentageData = model.getPercentage(camera_id, boost::get<string>(volumeData[0]), boost::get<int>(volumeData[1]));
		percentageDataConv = boost::lexical_cast<std::string>(round(percentageData));
		if (startRequest){
			weatherData = model.getWeather(boost::get<string>(cameraData["latitude"]), boost::get<string>(cameraData["longitude"]));
			startRequest = false;
		}
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
		} else {
			sentence = boost::get<string>(cameraData["street_name"]) + ".";
		}

		HelloReply r;
		r.set_response(sentence);
		writer->Write(r);

		if (context->IsCancelled()){
			exitFlag = true;
			tCounter.join();
			cout << "done semantic " << camera_id << endl;
			break;
		}
	}
}