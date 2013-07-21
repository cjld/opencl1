
//#define USE_CL_DEVICE_FISSION 1

#include <CL/cl.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

#define LIST_SIZE 1000

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


void printPlatformInfo(vector<cl::Platform> &list) {
  cout<<endl<<"Platform size : "<<list.size()<<endl;
  const char name[][20]={"PROFILE","VERSION","NAME","VENDOR","EXTENSIONS"};
  for (size_t i=0; i<list.size(); i++) {
    cout<<endl;
    for (size_t j=0; j<=4; j++) {
      string info;
      list[i].getInfo((cl_platform_info)(CL_PLATFORM_PROFILE+j),&info);
      cout<<name[j]<<" : "<<info<<endl;
    }
  }
}

void printDeviceInfo(vector<cl::Device> &list) {
  cout<<endl<<"Device size : "<<list.size()<<endl;
  for (size_t i=0; i<list.size(); i++) {
    cout<<endl;
    string info;
    list[i].getInfo(CL_DEVICE_NAME,&info);
    cout<<"name : "<<info<<endl;
    cl_uint size;
    list[i].getInfo(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,&size);
    cout<<"max dim number : "<< list[i].getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS>() <<endl;
    vector<size_t> sizes=list[i].getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();
    cout<<"max work item size : ";
    for (size_t j=0;j<sizes.size();j++)
      cout<<sizes[j]<<" ";
    cout<<endl;
 //   cout<<"max sub device : "<<CL_DEVICE_PARTITION_MAX_SUB_DEVICES<<endl;
  }
}

int main() {
  try {
    vector<cl::Platform> platformList;
    cl::Platform::get(&platformList);
    printPlatformInfo(platformList);

    vector<cl::Device> deviceList;
    platformList[0].getDevices(CL_DEVICE_TYPE_ALL,&deviceList);
    printDeviceInfo(deviceList);
    /*
    {
      cl_device_partition_property prop[]={
        CL_DEVICE_PARTITION_BY_COUNTS,
        3,1,
        CL_DEVICE_PARTITION_BY_COUNTS_LIST_END,0};
      vector<Device> subDev;
      deviceList[0].createSubDevices(prop,&subDev);
    }
    */

    cl::Context context(deviceList);

    cl::CommandQueue cmdQueue(context,deviceList[0]);

    cl::Program::Sources src(
        1,
        make_pair(getAllFile("kernal1.cpp"),0)
    );

    cl::Program program(context,src);
    program.build(deviceList);

    cl::Buffer bufferA(context, CL_MEM_READ_ONLY, LIST_SIZE * sizeof(int));
    cl::Buffer bufferB(context, CL_MEM_READ_ONLY, LIST_SIZE * sizeof(int));
    cl::Buffer bufferC(context, CL_MEM_WRITE_ONLY, LIST_SIZE * sizeof(int));

    cl::Kernel kernel(program, "adder");
    kernel.setArg(0, bufferA);
    kernel.setArg(1, bufferB);
    kernel.setArg(2, bufferC);

    int A[LIST_SIZE];
    for (int i=0;i<LIST_SIZE;i++) A[i]=i;
    cmdQueue.enqueueWriteBuffer(bufferA, CL_TRUE, 0, LIST_SIZE * sizeof(int), A);
    cmdQueue.enqueueWriteBuffer(bufferB, CL_TRUE, 0, LIST_SIZE * sizeof(int), A);

    cmdQueue.enqueueNDRangeKernel(kernel, cl::NDRange(), cl::NDRange(LIST_SIZE), cl::NDRange());

    long long tt=clock();

      cmdQueue.enqueueReadBuffer(bufferC, CL_TRUE, 0, LIST_SIZE * sizeof(int), A);

    cout<<"\nresult : "<<LIST_SIZE-1<<' '<<A[LIST_SIZE-1]<<endl;
    cout<<"cost time : "<<clock()-tt<<endl;

  } catch (cl::Error err) {
    cerr<<"ERROR : "<<err.what()<<'('<<err.err()<<')'<<endl;
  }
}
