.PHONY:clean
CC=g++
CFLAGS=-g 
LIBS=-lGLU -lglut
BIN=test.exe
OBJS=main.o mesh.o LapMatrixMesh.o Matrix.o DLRS.o Model_OBJ.o LaplacianSmooth.o
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN) core
