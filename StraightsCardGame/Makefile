CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD -g
EXEC = straights
OBJECTS = cards.o player.o computerplayer.o humanplayer.o deck.o roundcontroller.o gamecontroller.o straights.o

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
