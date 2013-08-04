    //----------------------------------------------//
   // Sample openCL api, really super sample       //
  // Author       : CJLD                          //
 // Versions     : 1.1 beta                      //
//----------------------------------------------//

#ifndef LDCL_H
#define LDCL_H

#include <CL/cl.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define LDCL_NEED_READ 1

struct LDCL {

  struct myBuffer {
    cl::Buffer *buffer;
    void *ptr;
    int size;
    cl_mem_flags flag;
    bool needRead;

    myBuffer(cl::Context *context, cl_mem_flags flags, void *p, int sz, bool nd) : ptr(p),size(sz),needRead(nd),flag(flags) {
      buffer=new cl::Buffer(*context,flag,size);
    }
    ~myBuffer() {delete buffer;}
  };

  vector<cl::Platform>  platformList;
  vector<cl::Device>    deviceList;
  cl::Context           *context;
  cl::CommandQueue      *cmdQueue;
  cl::Program::Sources  *src;
  cl::Program           *program;
  vector<myBuffer*>     bufferList;
  cl::Kernel            *kernel;
  cl::NDRange           global,local;

  int pid,did;
  string fileName,funcName;

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

  void printPlatformInfo() {
    cout<<endl<<"Platform size : "<<platformList.size()<<endl;
    const char name[][20]={"PROFILE","VERSION","NAME","VENDOR","EXTENSIONS"};
    for (size_t i=0; i<platformList.size(); i++) {
      cout<<endl;
      for (size_t j=0; j<=4; j++) {
        string info;
        platformList[i].getInfo((cl_platform_info)(CL_PLATFORM_PROFILE+j),&info);
        cout<<name[j]<<" : "<<info<<endl;
      }
    }
  }

  void printDeviceInfo() {
    cout<<endl<<"Device size : "<<deviceList.size()<<endl;
    for (size_t i=0; i<deviceList.size(); i++) {
      cout<<endl;
      string info;
      deviceList[i].getInfo(CL_DEVICE_NAME,&info);
      cout<<"CL_DEVICE_NAME : "<<info<<endl;
      cl_uint size;
      deviceList[i].getInfo(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,&size);
      cout<<"CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS : "<< deviceList[i].getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS>() <<endl;
      vector<size_t> sizes=deviceList[i].getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();
      cout<<"CL_DEVICE_MAX_WORK_ITEM_SIZES : ";
      for (size_t j=0;j<sizes.size();j++)
        cout<<sizes[j]<<" ";
      cout<<endl;
      cout<<"CL_DEVICE_LOCAL_MEM_SIZE : "<<deviceList[i].getInfo<CL_DEVICE_LOCAL_MEM_SIZE>()<<endl;
      cout<<"CL_DEVICE_MAX_WORK_GROUP_SIZE : "<<deviceList[i].getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>()<<endl;
      cout<<"CL_DEVICE_MAX_COMPUTE_UNITS : "<<deviceList[i].getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>()<<endl;
    }
  }

  int calcUnit() {
    return deviceList[did].getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() *
           deviceList[did].getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
  }

  void releaseAll() {
    #define DEL(x) if (x) delete x;
    for (size_t i=0; i<bufferList.size(); i++) 
      delete bufferList[i];
    DEL(kernel);
    DEL(program);
    DEL(src);
    DEL(cmdQueue);
    DEL(context);
    deviceList.clear();
    platformList.clear();
    #undef DEL
  }

  void init() {
    #ifdef __CL_ENABLE_EXCEPTIONS
    try {
    #endif

      cl::Platform::get(&platformList);
      printPlatformInfo();

      platformList[pid].getDevices(CL_DEVICE_TYPE_ALL,&deviceList);
      printDeviceInfo();

      context=new cl::Context(deviceList);

      cmdQueue=new cl::CommandQueue(*context,deviceList[did]);

    #ifdef __CL_ENABLE_EXCEPTIONS
    } catch (cl::Error err) {cerr<<"init ERROR : "<<err.what()<<'('<<err.err()<<')'<<endl;exit(0);}
    #endif
  }

  void loadFunc(string fn, string fc, cl::NDRange global=cl::NDRange(2048), cl::NDRange local=cl::NDRange()) {
    this->global=global;
    this->local=local;
    #ifdef __CL_ENABLE_EXCEPTIONS
    try {
    #endif
      src=new cl::Program::Sources(
          1,
          make_pair(getAllFile(fn.c_str()),0)
      );

      program=new cl::Program(*context,*src);
      program->build(deviceList);

      string buildInfo=program->getBuildInfo<CL_PROGRAM_BUILD_LOG>(deviceList[did]);
      if (buildInfo.size()>1) {
        cerr<<buildInfo<<endl;
        exit(1);
      } else
        cout<<fn<<": Build success\n";

      kernel=new cl::Kernel(*program, fc.c_str());
    #ifdef __CL_ENABLE_EXCEPTIONS
    } catch (cl::Error err) {
      cerr<<"loadFunc ERROR : "<<err.what()<<'('<<err.err()<<')'<<endl;
      cerr<<"Build Log : "<<program->getBuildInfo<CL_PROGRAM_BUILD_LOG>(deviceList[did]);
      exit(0);
    }
    #endif
  }

  void setArg(void *a, int size, cl_mem_flags flags, bool needRead=0) {
    #ifdef __CL_ENABLE_EXCEPTIONS
    try {
    #endif
      myBuffer *newBuffer=new myBuffer(context,flags,a,size,needRead);
      bufferList.push_back(newBuffer);
      kernel->setArg(bufferList.size()-1,*(newBuffer->buffer));
    #ifdef __CL_ENABLE_EXCEPTIONS
    } catch (cl::Error err) {cerr<<"setArg ERROR : "<<err.what()<<'('<<err.err()<<')'<<endl;exit(0);}
    #endif
  }

  void run(int notsync=0) {
    #ifdef __CL_ENABLE_EXCEPTIONS
    try {
    #endif
    
      if (!notsync) {
        for (size_t i=0; i<bufferList.size(); i++)
          if (bufferList[i]->ptr && (bufferList[i]->flag==CL_MEM_READ_ONLY ||
                                     bufferList[i]->flag==CL_MEM_READ_WRITE))
            cmdQueue->enqueueWriteBuffer(*(bufferList[i]->buffer), CL_TRUE, 0, bufferList[i]->size, bufferList[i]->ptr);
      }

      cmdQueue->enqueueNDRangeKernel(*kernel, cl::NDRange(), global, local);

      for (size_t i=0; i<bufferList.size(); i++)
        if (bufferList[i]->ptr && bufferList[i]->needRead)
          cmdQueue->enqueueReadBuffer(*(bufferList[i]->buffer), CL_TRUE, 0, bufferList[i]->size, bufferList[i]->ptr);

    #ifdef __CL_ENABLE_EXCEPTIONS
    } catch (cl::Error err) {cerr<<"setArg ERROR : "<<err.what()<<'('<<err.err()<<')'<<endl;exit(0);}
    #endif
  }

  LDCL(
      string _fileName=string(), string _funcName=string(), 
      cl::NDRange global=cl::NDRange(2048), cl::NDRange local=cl::NDRange(),
      int _pid=0, int _did=0
    ) :
      pid(_pid),did(_did),
      context(0),cmdQueue(0),src(0),program(0),kernel(0),
      fileName(_fileName),funcName(_funcName) {      
    this->global=global;
    this->local=local;
    init();
    if (funcName.size()) loadFunc(fileName,funcName,global,local);
  }
  ~LDCL() {releaseAll();}
};

#endif //ld_cl_template.h
