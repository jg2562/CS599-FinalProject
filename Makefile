CC=gcc
DEBUG_LEVEL=-O0
DEBUG=-ggdb
OPTIMIZE= -O3
CFLAGS= -Wall -Wextra -std=c11 -pedantic -Wwrite-strings
OUTPUT=main
LDFLAGS=
LDLIBS=-lm

# Good source for make file ideas
# https://stackoverflow.com/questions/14639794/getting-make-to-create-object-files-in-a-specific-directory
# https://www2.cs.duke.edu/courses/spring04/cps108/resources/makefiles/sample.html

all: $(OUTPUT)

$(OUTPUT): main.o simulation.o parser.o model.o parameters.o cellMap.o cell.o conditionMap.o population.o condition.o utils.o

main.o: simulation.h model.h parser.h
simulation.o: simulation.h parser.h model.h conditionMap.h condition.h utils.h cell.h
parser.o: parser.h model.h parameters.h cellMap.h utils.h
model.o: model.h parameters.h cellMap.h population.h
parameters.o: parameters.h
cellMap.o: cellMap.h cell.h
cell.o: cell.h condition.h parameters.h utils.h
conditionMap.o: conditionMap.h condition.h
population.o: population.h cell.h
condition.o: condition.h
utils.o: utils.h

clean:
	rm -f *.o $(OUTPUT)

debug: CPPFLAGS += $(DEBUG)
debug: all

optimize: CPPFLAGS += $(OPTIMIZE)
optimize: all
