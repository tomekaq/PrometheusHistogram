CCC = g++       # compiler
DICT = rootcint # root dictionary-generator
AR = ar         # archiver
LD = g++        # linker

ifndef COAST_DIR
  COAST_DIR=../../../..
endif

ARFLAGS = rcs

LDFLAGS += -fPIC -ggdb3 -lstdc++fs -Wl,--no-as-needed
CPPFLAGS += -c -fPIC -ggdb3 -D_FILE_OFFSET_BITS=64


LDFLAGS += $(shell root-config --libs)
CPPFLAGS += $(shell root-config --cflags)

SRCFILES = $(wildcard *.cc)
OBJECTS = $(patsubst %.cc, %.o, ${SRCFILES})

ROOTPROGRAM = ROOTHistProgram

all: ${ROOTPROGRAM} 

.cc.o:
	${CCC} ${CPPFLAGS} $^

${ROOTPROGRAM}: ROOTHistProgram.o
	${CCC} $^ -o $@ ${LDFLAGS}
clean: 
	@rm -f *.o *.a *.so *~ core *.root $(EXE)

