# E2EE File Transfer
A C++ implementation of end to end encrypted file transfer.

##  Building and running Docker 

### 1. Build the Docker Image
```bash
docker build -t e2ee-file-transfer .
```

### 2. Set up the network
```bash
docker network create e2ee-net
```

### 3. Run the containers
```bash
# Server
docker run -d --network e2ee-net --name server -v "$(pwd)/server":/app e2ee-file-transfer tail -f /dev/null

# Client
docker run -d --network e2ee-net --name client -v "$(pwd)/client":/app e2ee-file-transfer tail -f /dev/null
```
NOTE: These commands use a volume to link to the source code in your local machine to the containers.


### 4. Test the network connection
```bash
docker exec -it client /bin/bash   
```
```bash
apt-get update && apt-get install -y iputils-ping

ping server
```
