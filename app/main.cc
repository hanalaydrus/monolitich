// main.cc

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <thread>
#include <grpc++/grpc++.h>
#include "boost/variant.hpp"
#include "gatewayContract.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;
using gatewayContract::HelloRequest;
using gatewayContract::HelloReply;
using gatewayContract::Greeter;

#include "Model.h"
#include "Volume.h"
#include "Density.h"
#include "Semantic.h"

using namespace std;

// Logic and data behind the server's behavior.
class GreeterServiceImpl final : public Greeter::Service {
    Status SayHello(ServerContext* context,
                    const HelloRequest* request,
                    ServerWriter<HelloReply>* writer) override {
        HelloReply r;
        Model model;
        vector<boost::variant<int, string>> volumeResponse;
        string densityResponse;
        
        cout << "request come, type: " << request->typeofservice() << ", id: " << request->id() << endl;

        if (request->typeofservice() == "volume"){
        	while (true) {
	            volumeResponse = model.getVolumeDataByID(request->id());
	            r.set_response(to_string(boost::get<int>(volumeResponse[1])));
	            writer->Write(r);
	            
	            if (context->IsCancelled()){
                    cout << "done volume" << endl;
	                break;
	            }
	        }
        } else if (request->typeofservice() == "density") {
	        while (true) {
				densityResponse = model.getDensityDataByID(request->id());
				r.set_response(densityResponse);
				writer->Write(r);
				
				if (context->IsCancelled()){
					break;
				}
			}
        } else {
        	thread tRunSemantic(Semantic::runSemanticService, context, writer, request->id());
        	tRunSemantic.join();
        }
	        
        return Status::OK;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////// 
void RunServer() {
    string server_address("0.0.0.0:50000");
    GreeterServiceImpl service;
  
    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    unique_ptr<Server> server(builder.BuildAndStart());
    cout << "Server listening on " << server_address << endl;
  
    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(void) {
	Model model;
    vector< map<string, boost::variant<int, string>> > volumeConfig = model.getAllVolumeConfig();
	vector< map<string, boost::variant<int, string, map<string, int>>>> densityConfig = model.getAllDensityConfig();
    vector<int> index;

    for (int i = 0; i < volumeConfig.size(); ++i){
        if (!volumeConfig[i].empty()) {
            index.push_back(i);
        }
    }

    thread tRunVolumeService[index.size()];
    
    for (int i = 0; i < index.size(); ++i){
        tRunVolumeService[i] = thread (
            Volume::runVolumeService, 
            index[i], 
            boost::get<string>(volumeConfig[index[i]]["url"]),
            boost::get<int>(volumeConfig[index[i]]["x0"]),
            boost::get<int>(volumeConfig[index[i]]["y0"]),
            boost::get<int>(volumeConfig[index[i]]["x1"]),
            boost::get<int>(volumeConfig[index[i]]["y1"])
        );
    }

    thread tRunDensityService[densityConfig.size()];

	for (int i = 0; i < densityConfig.size(); ++i){
		tRunDensityService[i] = thread ( 
			Density::runDensityService,
			boost::get<int>(densityConfig[i]["camera_id"]),
			boost::get<string>(densityConfig[i]["url"]),
			boost::get<int>(densityConfig[i]["real_width"]),
			boost::get<int>(densityConfig[i]["real_height"]),
			boost::get< map<string, int> >(densityConfig[i]["mask_points"]),
			boost::get<int>(densityConfig[i]["edge_thresh"]),
			boost::get<int>(densityConfig[i]["low_thresh"]),
			boost::get<int>(densityConfig[i]["max_thresh"]),
			boost::get<int>(densityConfig[i]["morph_iteration"])
		);
	}

    thread tRunServer (RunServer);

    for (int i = 0; i < index.size(); ++i){
		tRunVolumeService[i].join();
    }

    for (int i = 0; i < densityConfig.size(); ++i){
		tRunDensityService[i].join();
    }

    tRunServer.join();

	return(0);
}

