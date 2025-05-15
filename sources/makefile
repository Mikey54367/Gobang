test:test.cc
	g++ -o $@ $^ -L/usr/lib64/mysql/ -lmysqlclient -ljsoncpp -lpthread -lboost_system
.PHONY:clean
clean:
	rm -f test