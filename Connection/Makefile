all: final

final: server.o
	@echo "Linking and producing final app"
	g++ server.o -o final
	@chmod +x final
	
server.o: server.cpp
	@echo "Compiling the main file"
	g++ -c server.cpp
