.PHONY:clean

bpnn: main.o bp.o headMoveGenerator.o sunglassGenerator.o personGenerator.o moodGenerator.o
	g++ -o bpnn main.o bp.o headMoveGenerator.o sunglassGenerator.o personGenerator.o moodGenerator.o

main.o: main.cpp
	g++ -c main.cpp

bpnn.o: bp.cpp
	g++ -c bp.cpp

headMoveGenerator.o: headMoveGenerator.cpp
	g++ -c headMoveGenerator.cpp

sunglassGenerator.o: sunglassGenerator.cpp
	g++ -c sunglassGenerator.cpp

personGenerator.o: personGenerator.cpp
	g++ -c personGenerator.cpp

moodGenerator.o: moodGenerator.cpp
	g++ -c moodGenerator.cpp

clean:
	rm -f *.o bpnn
