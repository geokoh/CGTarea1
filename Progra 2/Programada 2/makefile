OBJECTS=rayTrace.o
OUTPUT=rayTrace

CFLAGS=-I/usr/local/Mesa-3.4/include
LDLIBS=-lX11 -lglut -lGLU -lGL -lm -lXext -lXmu
LDFLAGS=-L/usr/local/Mesa-3.4/lib -L/usr/X11R6/lib

$(OUTPUT): $(OBJECTS)
	gcc -g $(OBJECTS) -o $(OUTPUT) $(LDLIBS) $(CFLAGS) $(LDFLAGS)

$(OBJECTS): rayTrace.h

clean:
	rm -f *.o
	rm -f rayTrace
