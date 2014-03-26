# If the compilation fails, it may be because g++ doesn't know
# where to find the include files and the library for NTL.
# You may need to add -I include_directory and -L library_directory
# options.  For example, if NTL installed into /home/username/sw directory,
# then you should replace the CXXFLAGS line below with
# CXXFLAGS = -Wno-deprecated -I /home/username/sw/include -L /home/username/sw/lib
#
# To find out where NTL is, look for the directory that has a
# subdirectory called include with a subdirectory NTL in it 
# and a subdirectory called lib with a file libntl.a in it.
# NTL creates these when you install it according to NTL instructions.
# See http://www.shoup.net/ntl for more.

CXXFLAGS = -Wno-deprecated

all: sketch differ

bch.o: bch.cpp pinsketch.h
	g++ $(CXXFLAGS) -c bch.cpp 

io.o: io.cpp pinsketch.h
	g++ $(CXXFLAGS) -c io.cpp 

sketch.o: sketch.cpp pinsketch.h
	g++ $(CXXFLAGS) -c sketch.cpp 

differ.o: differ.cpp pinsketch.h
	g++ $(CXXFLAGS) -c differ.cpp 

sketch: sketch.o bch.o io.o pinsketch.h
	g++  $(CXXFLAGS) sketch.o io.o bch.o -lntl -o sketch

differ: differ.o bch.o io.o pinsketch.h
	g++ $(CXXFLAGS) differ.o io.o bch.o -lntl -o differ

clean:
	rm differ sketch bch.o io.o sketch.o differ.o
