run_local:
	cd traffic_streaming && docker-compose up --build -d
	cd app && docker-compose up --build -d
	cd api-gateway-mono && docker-compose up --build -d
	
run_prod:
	cd traffic_streaming && sudo docker-compose up --build -d
	cd app && sudo docker-compose up --build -d
	cd api-gateway-mono && sudo docker-compose up --build -d
