
.SUFFIXES:	.o .c .cpp .cc .C .a


RM = -rm


.cpp.o:	
	$(C++) $(CCFLAGS) $*.cpp


#############################################################

C++        = g++
CCFLAGS    = -c -Iinclude


VPATH   = Obj

OBJECTS1 = Grid.o UniformGrid.o ScalarAttributes.o ColormapRenderer.o sampleUniformGridScalars.o 
OBJECTS2 = Grid.o UniformGrid.o StructuredGrid.o ScalarAttributes.o ColormapRenderer.o sampleStructuredGridScalars.o 


default: sampleUniformGridScalars sampleStructuredGridScalars


sampleUniformGridScalars: $(OBJECTS1) 
	-mv $(OBJECTS1) Obj
	cd Obj ; $(C++) -o ../sampleUniformGridScalars $(OBJECTS1) -lGL  -lglut -lGLU; cd .

sampleStructuredGridScalars: $(OBJECTS2) 
	-mv $(OBJECTS2) Obj
	cd Obj ; $(C++) -o ../sampleStructuredGridScalars $(OBJECTS2)  -lGL  -lglut -lGLU; cd .


clean:
	$(RM) $(VPATH)/*.o sampleUniformGridScalars sampleStructuredGridScalars
 

