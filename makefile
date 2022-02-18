NAME?=game.x86_64
CXX?=g++
__CXXFLAGS= -Og -g -std=c++11 \
	-Wall -Wpedantic -Wextra \
	-pthread \
	-lSDL2 -lSDL2_image -lSDL2_ttf

LDXX?=g++


IDIR=include
SDIR=src
BDIR=bin
ODIR=obj
DDIR=dep

_CXXFLAGS = ${__CXXFLAGS} -I${IDIR} ${CXXFLAGS}
SRC = $(shell find ${SDIR} -type f -name '*.cpp' -o -name ".backup" -prune -type f)
OBJ = $(patsubst ${SDIR}/%.cpp,${ODIR}/%.o,${SRC})
DEP = $(patsubst ${SDIR}/%.cpp,${DDIR}/%.dep,${SRC})

ifndef VERBOSE
.SILENT:
endif

build: depend ${SRC} ${BDIR}/${NAME}

${DEP}: ${DDIR}/%.dep: ${SDIR}/%.cpp
	mkdir -p ${DDIR}
	echo "Calculating dependencies for $^"
	mkdir -p $$(dirname $@)
	${CXX} ${_CXXFLAGS} $^ -MM -MP -MT $(patsubst ${SDIR}/%.cpp,${ODIR}/%.o,$^) > $@

depend: ${DEP}
include ${DEP}

${OBJ}: ${ODIR}/%.o: ${SDIR}/%.cpp
	mkdir -p ${ODIR}
	echo "Compiling $@"
	mkdir -p $$(dirname $@)
	${CXX} -c -o $@ $< ${_CXXFLAGS}

${BDIR}/${NAME}: ${OBJ}
	mkdir -p ${BDIR}
	echo "Linking ${NAME}"
	${LDXX} -o $@ ${OBJ} ${_CXXFLAGS}

clean:
	echo "Cleaning build files"
	rm -r ${ODIR} ${DDIR}

run: build
	echo "Running ${NAME}"
	${BDIR}/${NAME}

ctags: ${SRC}
	echo "Generating ctags"
	ctags -R ${IDIR} ${SDIR}
