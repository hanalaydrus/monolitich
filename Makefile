run_local:
	cd app && docker-compose up --build -d
	cd api-gateway-mono && docker-compose up --build -d
	
run_prod:
	cd app && sudo docker-compose up --build -d
	cd api-gateway-mono && sudo docker-compose up --build -d
