OBJECTS=raytrace.o
OUTPUT=raytrace

CFLAGS=-I/usr/local/Mesa-3.4/include
LDLIBS=-lX11 -lglut -lGLU -lGL -lm -lXext -lXmu
LDFLAGS=-L/usr/local/Mesa-3.4/lib -L/usr/X11R6/lib

$(OUTPUT): $(OBJECTS)
	gcc -g $(OBJECTS) -o $(OUTPUT) $(LDLIBS) $(CFLAGS) $(LDFLAGS)

$(OBJECTS): raytrace.h

clean:
	rm -f *.o
	rm -f raytrace
