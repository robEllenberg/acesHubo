#-leng -lmx -lut -lmwfl -lmat -licudata -licuuc 

#ARCH = a64
#ARCH = 86
MAT_INC = -I $(MATLAB_PATH)/extern/include/ 
MAT_LIB = -L $(MATLAB_PATH)/bin/glnx$(ARCH)/    \
          -L$(MATLAB_PATH)/sys/os/glnx$(ARCH)/
LIBS = -leng -lmx
LD_OPS = -Wl,-rpath-link=$(MATLAB_PATH)/bin/glnx$(ARCH)/     \
         -Wl,-rpath-link=$(MATLAB_PATH)/sys/os/glnx$(ARCH)/

all:
	gcc $(LD_OPS) $(MAT_INC) $(MAT_LIB) $(LIBS) engdemo.c -o matdemo

clean:
	@-rm matdemo
