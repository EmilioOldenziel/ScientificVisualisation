
.SUFFIXES:	.o .c .cpp .cc .C .a


RM = -rm


.cpp.o:	
	$(C++) $(CCFLAGS) $*.cpp


#############################################################

C++        = g++
CCFLAGS    = -c -Iinclude


VPATH   = Obj

OBJECTS1 = sample1.o
OBJECTS2 = sample2.o
OBJECTS3 = sample3.o
OBJECTS4 = sample4.o
OBJECTS5 = sample5.o
OBJECTS6 = sample6.o
OBJECTS7 = sample7.o

default: sample1 sample2 sample3 sample4 sample5 sample6 sample7

sample1: $(OBJECTS1) 
	-mv $(OBJECTS1) Obj
	cd Obj ; $(C++) -o ../sample1 $(OBJECTS1) -lGL  -lglut -lGLU; cd .

sample2: $(OBJECTS2) 
	-mv $(OBJECTS2) Obj
	cd Obj ; $(C++) -o ../sample2 $(OBJECTS2) -lGL  -lglut -lGLU; cd .

sample3: $(OBJECTS3) 
	-mv $(OBJECTS3) Obj
	cd Obj ; $(C++) -o ../sample3 $(OBJECTS3)-lGL  -lglut -lGLU; cd .

sample4: $(OBJECTS4) 
	-mv $(OBJECTS4) Obj
	cd Obj ; $(C++) -o ../sample4 $(OBJECTS4) -lGL  -lglut -lGLU; cd .

sample5: $(OBJECTS5) 
	-mv $(OBJECTS5) Obj
	cd Obj ; $(C++) -o ../sample5 $(OBJECTS5) -lGL  -lglut -lGLU; cd .

sample6: $(OBJECTS6) 
	-mv $(OBJECTS6) Obj
	cd Obj ; $(C++) -o ../sample6 $(OBJECTS6) -lGL  -lglut -lGLU; cd .

sample7: $(OBJECTS7) 
	-mv $(OBJECTS7) Obj
	cd Obj ; $(C++) -o ../sample7 $(OBJECTS7) -lGL  -lglut -lGLU; cd .


clean:
	$(RM) $(VPATH)/*.o sample1 sample2 sample3 sample4 sample5 sample6 sample7
 

