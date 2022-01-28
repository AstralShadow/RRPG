NAME=my_app
CXX=g++
__CXXFLAGS= -Og -g -std=c++11 \
	-Wall -Wpedantic -Wextra \
	-pthread \
	-lSDL2 -lSDL2_image # -lSDL2_ttf


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

build: depend tags ${SRC} ${BDIR}/${NAME}

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
	$(CXX) -o $@ ${OBJ} ${_CXXFLAGS}

clean:
	echo "Cleaning build files"
	rm -r ${ODIR} ${BDIR} ${DDIR} tags

run: build
	echo "Running ${NAME}"
	${BDIR}/${NAME}

tags: ${SRC}
	echo "Generating ctags"
	ctags -R ${IDIR} ${SDIR}
