mysql:mysql.c
	gcc -o $@ $^ -L/usr/lib64/mysql/ -lmysqlclient
json:json.cc
	g++ -o $@ $^ -std=c++11 -ljsoncpp
Server:wsServer.cc
	g++ -o $@ $^ -std=c++11 -lpthread -lboost_system