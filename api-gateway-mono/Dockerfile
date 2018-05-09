FROM golang:1.8

WORKDIR /go/src/api.gateway
COPY . .

RUN go get -d -v ./...
RUN go install -v ./...

ENV GRPC_GO_LOG_SEVERITY_LEVEL="INFO"
ENV GRPC_GO_LOG_VERBOSITY_LEVEL=2

CMD ["make", "run"]