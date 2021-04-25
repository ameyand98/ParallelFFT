default:
	g++ `pkg-config --cflags raftlib` example1.cc -o ex1.o `pkg-config --libs raftlib`

seriel:
	g++ serielFft.cc -o seriel.o
	./seriel

clean:
	rm -rf *.o