#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <CL/cl.h>
#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

using std::cout;
using std::endl;

cl_device_id device_id = NULL; 
cl_context context = NULL; 
cl_command_queue command_queue = NULL; 
cl_mem memobj = NULL; 
cl_program program = NULL; 
cl_kernel kernel = NULL; 
cl_platform_id platform_id = NULL; 
cl_uint ret_num_devices; 
cl_uint ret_num_platforms; 
cl_int ret;
char string[MEM_SIZE];
FILE *fp;
char fileName[] = "./main.cpp"; 
char *source_str; 
size_t source_size;

int main(){	
  cout<<"Load the source code containing the kernel"<<endl;
  fp = fopen(fileName, "r"); 
  if (!fp) {
    fprintf(stderr, "Failed to load kernel.\n");
    exit(1);
  }
  source_str = (char*)malloc(MAX_SOURCE_SIZE);
  source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
  fclose(fp);
  cout<<"Get Platform and Device Info"<<endl;
  cl_platform_id arr[2];
  ret = clGetPlatformIDs(2, arr, &ret_num_platforms);
  platform_id=arr[1];
  cout<<platform_id<<' '<<ret_num_platforms<<endl;

  unsigned int myNum;
  clGetPlatformIDs(0, 0, &myNum);
  cout<<myNum<<endl;
  size_t temp3=10000;
  char temp2[temp3];
  clGetPlatformInfo(platform_id,
  	CL_PLATFORM_NAME,
        temp3,
  	temp2,
  	&temp3);
  cout<<temp2<<' '<<temp3<<endl;
  

  ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
  cout<<"Create OpenCL context"<<" "<<ret_num_devices<<endl;
  context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
  cout<<"Create Command Queue"<<ret<<endl;
  command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
  cout<<"Create Memory Buffer"<<ret<<endl;
  memobj = clCreateBuffer(context, CL_MEM_READ_WRITE,MEM_SIZE * sizeof(char), NULL, &ret);	
  cout<<"Create Kernel Program from the source"<<ret<<endl;
  program = clCreateProgramWithSource(context, 1, (const char **)&source_str,(const size_t *)&source_size, &ret);
  cout<<"Build Kernel Program"<<ret<<endl;
  ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
  cout<<"Create OpenCL Kernel"<<ret<<endl;
  kernel = clCreateKernel(program, "hello", &ret);
  cout<<"Set OpenCL Kernel Parameters"<<ret<<endl;
  ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);
  cout<<"Execute OpenCL Kernel"<<ret<<endl;
  ret = clEnqueueTask(command_queue, kernel, 0, NULL,NULL);
  cout<<"Copy results from the memory buffer"<<ret<<endl;
  ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0,	 MEM_SIZE * sizeof(char),string, 0, NULL, NULL);
  cout<<"Display Result"<<ret<<endl;
  puts(string);
  cout<<"Finalization"<<endl;
  ret = clFlush(command_queue);
  ret = clFinish(command_queue);
  ret = clReleaseKernel(kernel);
  ret = clReleaseProgram(program);
  ret = clReleaseMemObject(memobj);
  ret = clReleaseCommandQueue(command_queue);
  ret = clReleaseContext(context);
  free(source_str);
  return 0;
}

