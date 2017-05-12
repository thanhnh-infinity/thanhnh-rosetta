all: main $(LIBSO)

LIBSO = Rosetta.so

OBJECTS = interface.o nn_model_mem.o matrix.o data.o txt_class.o ros_base.o 

.SUFFIXES:	.o .cc 

.cc.o :
	$(CXX) -c -fPIC -Wall -o $@ $<

.c.o :
	$(CXX) -c -fPIC -Wall -o $@ $<

all:    $(LIBSO)

$(LIBSO):	$(OBJECTS)
	$(CXX) -shared -o $(LIBSO) $(OBJECTS) 

main: 	$(OBJECTS)
	$(CXX) main.cc $(OBJECTS) -o test_rosetta

clean: 
	rm -f *.o $(LIBSO)



