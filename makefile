all: main $(LIBSO)

LIBSO = Rosetta.so

OBJECTS = interface.o nn_model_mem.o matrix.o data.o txt_class.o ros_base.o 

.SUFFIXES:	.o .cc 

.cc.o :
	g++-6 -c -fPIC -Wall -o $@ $<

.c.o :
	g++-6 -c -fPIC -Wall -o $@ $<

all:    $(LIBSO)

$(LIBSO):	$(OBJECTS)
	g++-6 -shared -o $(LIBSO) $(OBJECTS) 

main: 	$(OBJECTS)
	g++-6 main.cc $(OBJECTS) -o test_rosetta -std=c++17

clean: 
	rm -f *.o $(LIBSO)


