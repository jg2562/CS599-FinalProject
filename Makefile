CC=gcc
DEBUG_LEVEL=-O0
DEBUG=-ggdb
OPTIMIZE= -O3
CFLAGS= -Wall -Wextra -std=c11 -pedantic -Wwrite-strings
OUTPUT=simulation
LDFLAGS=
LDLIBS=-lm

# Good source for make file ideas
# https://stackoverflow.com/questions/14639794/getting-make-to-create-object-files-in-a-specific-directory
# https://www2.cs.duke.edu/courses/spring04/cps108/resources/makefiles/sample.html

all: $(OUTPUT)

$(OUTPUT): simulation.o parser.o model.o parameters.o cellMap.o cell.o

simulation.o: parser.h model.h
parser.o: parser.h model.h parameters.h cellMap.h
model.o: model.h parameters.h cellMap.h
parameters.o: parameters.h
cellMap.o: cellMap.h cell.h
cell.o: cell.h


clean:
	rm -f *.o $(OUTPUT)

debug: CPPFLAGS += $(DEBUG)
debug: all

optimize: CPPFLAGS += $(OPTIMIZE)
optimize: all
