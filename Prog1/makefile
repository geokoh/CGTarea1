OBJECTS=PRO1_Astorga_Castillo.o
OUTPUT=CG-PROG1_Astorga_Castillo

CFLAGS=-I/usr/local/Mesa-3.4/include
LDLIBS=-lX11 -lglut -lGLU -lGL -lm -lXext -lXmu
LDFLAGS=-L/usr/local/Mesa-3.4/lib -L/usr/X11R6/lib

$(OUTPUT): $(OBJECTS)
	gcc $(OBJECTS) -o $(OUTPUT) $(LDLIBS) $(CFLAGS) $(LDFLAGS)

$(OBJECTS): PRO1_Astorga_Castillo.h

clean:
	rm -f *.o
	rm -f CG-PROG1_Astorga_Castillo
