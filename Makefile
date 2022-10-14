CC = i686-w64-mingw32-g++
CFLAGS = -leasyx -lstdc++fs -static -std=c++17 -ldwmapi 
LDFLAGS = -L3rdparty/easyx/lib-for-devcpp_5.4.0/

Game: main.o Game.o
	${CC} -o main main.o Game.o ${LDFLAGS} ${CFLAGS}

main.o:
	${CC} -o main.o -c src/main.cc -Iinclude -I3rdparty/easyx/include 

Game.o:
	${CC} -o Game.o -c src/Game.cc -Iinclude -I3rdparty/easyx/include 
