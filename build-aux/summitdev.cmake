# Initial cache list for Summitdev
#
# Usage: cmake -C /path/to/this/file /path/to/DCA/source -D<option>=<value> ...

set(TEST_RUNNER "jsrun" CACHE STRING "Command for executing (MPI) programs.")
set(MPIEXEC_NUMPROC_FLAG "-a" CACHE STRING
  "Flag used by TEST_RUNNER to specify the number of processes.")
set(MPIEXEC_PREFLAGS "-g1 -n1" CACHE STRING
  "Flags to pass to TEST_RUNNER directly before the executable to run.")

option(DCA_WITH_CUDA "Enable GPU support." ON)
set(CUDA_GPU_ARCH "sm_60" CACHE STRING "Name of the *real* architecture to build for.")
set(MAGMA_DIR $ENV{OLCF_MAGMA_ROOT} CACHE PATH
  "Path to the MAGMA installation directory. Hint for CMake to find MAGMA.")

set(FFTW_INCLUDE_DIR $ENV{OLCF_FFTW_ROOT}/include CACHE PATH "Path to fftw3.h.")
set(FFTW_LIBRARY $ENV{OLCF_FFTW_ROOT}/lib/libfftw3.a CACHE FILEPATH
  "The FFTW3(-compatible) library.")
