# To build sorting algorithm suites
CXX = g++
CXXFLAGS = -std=c++11 -g -Wall
LDFLAGS=

HEADERS=bubble_sort.h \
	cocktail_sort.h \
	heap_sort.h \
	insertion_sort.h \
	introsort.h \
	merge_sort.h \
	minmax_sort.h \
	quicksort.h \
	selection_sort.h
SOURCES=sorting.cpp
TARGET=sorting

.PHONY : all clean

all : $(SOURCES) $(HEADERS) 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SOURCES) -o $(TARGET)

clean :
	rm -f *.~ $(TARGET)
