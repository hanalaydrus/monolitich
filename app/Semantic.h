// Semantic.h
#include <grpc++/grpc++.h>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <ctime>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include "boost/variant.hpp"
#include "boost/lexical_cast.hpp"
#include "gatewayContract.grpc.pb.h"
#include "Model.h"

using namespace std;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;
using gatewayContract::HelloRequest;
using gatewayContract::HelloReply;
using gatewayContract::Greeter;

class Semantic {
public:
	Semantic();
	
	static void runSemanticService(ServerContext* context, ServerWriter<HelloReply>* writer, int camera_id);
};