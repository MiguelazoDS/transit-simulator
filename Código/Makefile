CC = g++
CFLAGS = -I.
OBJ = Simulación_de_tránsito.o

%.o: %.cpp 
	$(CC) -c -o $@ $< $(CFLAGS)

output: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o output

