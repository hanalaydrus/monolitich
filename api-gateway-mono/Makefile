build:
	protoc -I gatewayContract/ gatewayContract/gatewayContract.proto --go_out=plugins=grpc:gatewayContract

run:
	go run ./client_server.go