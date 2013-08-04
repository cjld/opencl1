CUDA_PATH = C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v5.0\\
CUDA_INCLUDE_PATH = $(CUDA_PATH)include
CUDA_LIB_PATH = $(CUDA_PATH)lib\Win32
ADD_LIB_INCLUDE = -L"$(CUDA_LIB_PATH)" -I"$(CUDA_INCLUDE_PATH)" -I"C:\Users\CJlegend\Desktop\openCLstd\ld_opencl_template"
CL_ERR = -D__CL_ENABLE_EXCEPTIONS
main.exe : main.cpp
	g++ $^ -o $@ $(ADD_LIB_INCLUDE) -g -lopencl $(CL_ERR)
