# E2EE File Transfer
A C++ implementation of end to end encrypted file transfer.

##  Building and running Docker 
```bash
docker-compose up --build -d

docker exec -it client /bin/bash 
docker exec -it server /bin/bash 
```

## Running executables
The server executable should auto start.
The client can be ran with:
```bash
./client <hostname> <filepath>
```