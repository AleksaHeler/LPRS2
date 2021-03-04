
###############################################################################
# Config.

#TODO Fill these variables.
STUDENT_NO=005
STUDENT_NAME="Aleksa Heler"
STUDENT_INDEX="RA22-2018"

# Debug mode off by default.
DEBUG ?= 0

###############################################################################
# Variables.

ifneq ($(ASSISTANT),1)
ifeq ($(STUDENT_NO),000)
$(error "STUDENT_NO is not filled!")
endif
endif

ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
	EXE := .exe
else
	EXE := 
endif

CPPFLAGS := -DDEBUG=$(DEBUG)

###############################################################################
# Build.

.PHONY: run all

run: all
	./main$(EXE) | tee log.txt
	@echo "End"

all: main$(EXE)

main$(EXE): main.o
	$(CC) -o $@ $< $(LDFLAGS) $(LIBS)

###############################################################################
# House keeping.

.PHONY: clean dist

clean:
	rm -f *.o *.exe main$(EXE) *.txt

MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
PROJECT_DIR := $(notdir $(patsubst %/,%,$(dir $(MAKEFILE_PATH))))
ZIP_NAME := ${PROJECT_DIR}-Student_No_${STUDENT_NO}.zip
dist: clean
	cd ../ && zip -9r ${ZIP_NAME} ${PROJECT_DIR}

###############################################################################
