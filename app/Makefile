build:
	protoc -I . --cpp_out=. gatewayContract.proto
	g++ -std=c++11 `pkg-config --cflags --libs protobuf grpc`  -c -o gatewayContract.pb.o gatewayContract.pb.cc
	protoc -I . --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` gatewayContract.proto
	g++ -std=c++11 `pkg-config --cflags --libs protobuf grpc`  -c -o gatewayContract.grpc.pb.o gatewayContract.grpc.pb.cc
	g++ -std=c++11 -c -o Model.o Model.cc
	g++ -std=c++11 `pkg-config --cflags opencv`  -c -o Blob.o Blob.cc
	g++ -std=c++11 `pkg-config --cflags opencv` -c -o IPM.o IPM.cc
	g++ -std=c++11 `pkg-config --cflags --libs protobuf grpc opencv`  -c -o Volume.o Volume.cc
	g++ -std=c++11 `pkg-config --cflags --libs protobuf grpc opencv`  -c -o Density.o Density.cc
	g++ -std=c++11 `pkg-config --cflags --libs protobuf grpc opencv`  -c -o Semantic.o Semantic.cc
	g++ -std=c++11 `pkg-config --cflags --libs protobuf grpc opencv`  -c -o main.o main.cc
	g++ gatewayContract.pb.o gatewayContract.grpc.pb.o main.o IPM.o Blob.o Model.o Volume.o Density.o Semantic.o -I/usr/inlude/cppconn -L/usr/lib -lmysqlcppconn -lcurl -L/usr/local/lib `pkg-config --cflags --libs protobuf grpc++ grpc opencv` -Wl,--no-as-needed -lgrpc++_reflection -ldl -o main

run:
	./main

clean:
	rm -f *.o *.pb.cc *.pb.h main

build_db_image:
	docker build -t asia.gcr.io/tugas-akhir-hana/mariadb-mono:latest ./mariadb/

push_gcp_image:
	docker push asia.gcr.io/tugas-akhir-hana/mariadb-mono:latest