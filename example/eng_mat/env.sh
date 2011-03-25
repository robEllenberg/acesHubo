export MATLAB_PATH=/home/matlab/2010b

#for 64-bit installations
export ARCH=a64
#for 32-bit installations
#export ARCH=86

export LD_LIBRARY_PATH=$MATLAB_PATH/bin/glnx$ARCH:$MATLAB_PATH/sys/os/glnx$ARCH:$LD_LIBRARY_PATH
export PATH=$PATH:$MATLAB_PATH/bin
