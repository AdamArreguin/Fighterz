CFLAGS = -I ./include
#LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm
LFLAGS = -lrt -lX11 -lGL

all: fighterz

fighterz: fighterz.cpp log.cpp timers.cpp
	g++ $(CFLAGS) fighterz.cpp log.cpp timers.cpp \
	libggfonts.a -Wall $(LFLAGS) -o fighterz

clean:
	rm -f fighterz
	rm -f *.o

