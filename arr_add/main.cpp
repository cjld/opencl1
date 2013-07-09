#include <CL/opencl.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <ctime>
using namespace std;

#define DATA_SIZE 501
#define DID 1

unsigned int num;
int err;

char* getAllFile(const char name[]) {
  ifstream fin(name,ios::binary);
  fin.seekg(0,ios::end);
  int len=fin.tellg();
  fin.seekg(0,ios::beg);
  char *a=new char[len+1];
  fin.read(a,len);
  a[len]='\0';
  return a;
}

int main() {
  //get platform numbers
  err = clGetPlatformIDs(0, 0, &num);
  cout<<"number of platform: "<<num<<endl;
 
  //get all platforms
  vector<cl_platform_id> platforms(num);
  err = clGetPlatformIDs(num, &platforms[0], &num);

  //get platforms info
  size_t infoSize=1000;
  char info[infoSize];
  clGetPlatformInfo(platforms[DID],
 	CL_PLATFORM_NAME,
 	infoSize,
 	info,
 	&infoSize);
  cout<<info<<endl;
  
  //get device num
  err=clGetDeviceIDs(platforms[DID],CL_DEVICE_TYPE_ALL,0,0,&num);
  vector<cl_device_id> did(num);
  //get all device
  err=clGetDeviceIDs(platforms[DID],CL_DEVICE_TYPE_ALL,num,&did[0],&num);
  cout<<"number of device: "<<num<<endl;
  clGetDeviceInfo(did[0],
 	CL_DEVICE_TYPE,
 	infoSize,
 	info,
 	&infoSize);
  cout<<*((cl_device_type*)info)<<' '<<infoSize<<" "<<CL_DEVICE_TYPE_GPU<<endl;
  //return 0;
  
  //set property with certain platform
  cl_context_properties prop[] = { CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(platforms[DID]), 0 };
  cl_context context = clCreateContextFromType(prop, CL_DEVICE_TYPE_ALL, NULL, NULL, &err);
  cout<<"err:"<<err<<endl;
  
  cl_command_queue cqueue = clCreateCommandQueue(context, did[0], 0, &err);
  cout<<"err:"<<err<<endl;
  
  cout<<"read the .cl file"<<endl;
  const char *src=getAllFile("kernal1.cpp");
  cout<<src<<endl;
  cl_program program = clCreateProgramWithSource(context, 1, &src, 0, 0);
  err = clBuildProgram(program, 0, 0, 0, 0, 0);
  cout<<"err:"<<err<<endl;

  cl_kernel adder = clCreateKernel(program, "adder", &err);
  cout<<"err:"<<err<<endl;
  

  vector<float> a(DATA_SIZE), b(DATA_SIZE);
  for(int i = 0; i < DATA_SIZE; i++) {
    a[i] = i;
    b[i] = i;
  }


  cl_mem cl_a = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * DATA_SIZE, &a[0], NULL);
  cl_mem cl_b = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * DATA_SIZE, &b[0], NULL);
  cl_mem cl_res = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_float) * DATA_SIZE, NULL, NULL);
  clSetKernelArg(adder, 0, sizeof(cl_mem), &cl_a);
  clSetKernelArg(adder, 1, sizeof(cl_mem), &cl_b);
  clSetKernelArg(adder, 2, sizeof(cl_mem), &cl_res);
  
  size_t work_size=DATA_SIZE-1;
  cout<<"begin run...."<<endl;
  long long tt=clock();
  err = clEnqueueNDRangeKernel(cqueue, adder, 1, 0, &work_size, 0, 0, 0, 0);
  cout<<"err:"<<err<<' '<<work_size<<' '<<clock()-tt<<endl;
  
  tt=clock();
  vector<float> res(DATA_SIZE);
  err = clEnqueueReadBuffer(cqueue, cl_res, CL_TRUE, 0, sizeof(float) * DATA_SIZE, &res[0], 0, 0, 0);
  cerr<<res[work_size-1]<<endl;
  cout<<clock()-tt<<endl;
  //for (size_t i=0;i<res.size();i++) cout<<res[i]<<endl;
  
}
