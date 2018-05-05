export LD_LIBRARY_PATH=/usr/local/lib
protoc -I=. --cpp_out=. message.proto

g++ -o server server.cpp message.pb.cc -lrt -lprotobuf -lpthread
g++ -o client client.cpp message.pb.cc -lrt -lprotobuf -lpthread
