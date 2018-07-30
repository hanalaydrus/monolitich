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

int conccurrentSemantic = 0;

string printTimeS(){
    milliseconds ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    return to_string(ms.count());
}

void counter(double duration){
	duration = 0;
	sleep(600000);
	duration = 600;
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

	double duration;
	thread tCounter (counter, duration);

	// for logging
    vector< vector<boost::variant<int, string>> > log;
    string Str = to_string(camera_id);
    camera_id = camera_id % 10;

    conccurrentSemantic++;
    int i = 0;
	for ( ; ; ) {
        vector<boost::variant<int, string>> logs;
        string t1 = printTimeS();
		cameraData = model.getCameraDataByID(camera_id);
		densityData = model.getDensityDataByID(camera_id);
		volumeData = model.getVolumeDataByID(camera_id);
		percentageData = model.getPercentage(camera_id, boost::get<string>(volumeData[0]), boost::get<int>(volumeData[1]));
		string t2 = printTimeS();
		percentageDataConv = boost::lexical_cast<std::string>(round(percentageData));
		if (duration == 600){
			weatherData = model.getWeather(boost::get<string>(cameraData["latitude"]), boost::get<string>(cameraData["longitude"]));
		}
		transform(weatherData.begin(), weatherData.end(), weatherData.begin(), ::tolower);
		size_t found = weatherData.find("hujan");
		
		if ((densityData.empty()) && (percentageData != percentageData)){
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
		} else {
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

		HelloReply r;
		r.set_response(sentence);
		writer->Write(r);
		// logging
        logs.push_back(Str);
        logs.push_back(t1);
        logs.push_back(t2);
        logs.push_back(printTimeS());
        logs.push_back(conccurrentSemantic);
        
        log.push_back(logs);
        i++;
        if (i == 1000) {
        	model.logging(log);
		    cout << "Finish check log cc: " << conccurrentSemantic << endl;
		    conccurrentSemantic--;
        }
        //
		if (context->IsCancelled()){
			tCounter.join();
			break;
		}
	}
	

}