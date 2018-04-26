// Use cl::vector instead of STL version
#define __NO_STD_VECTOR
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include <iostream>
#include <iostream>
#include <fstream>
#include <iostream>
#include <iterator>
#include <math.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>

//To print entry and exit of functions
#define F_LOG LogBlock _l(__func__)
struct LogBlock {
    const char *mLine;
    LogBlock(const char *line) : mLine(line) {
        std::cout<<mLine <<"  -----> Enter \n";
    }
    ~LogBlock() {
        std::cout<<mLine <<" <----- Leave \n";
    }
};
//clGetPlatformInfo(_platformID,CL_PLATFORM_NAME, NULL, NULL, &_infoCLSize);
//_infoName = (char*)malloc(sizeof(char)*_infoCLSize);
//clGetPlatformInfo(_platformID,CL_PLATFORM_NAME, sizeof(char) * _infoCLSize, _infoName, NULL );
//std::cout<<"CL_PLATFORM_NAME          : "<<_infoName<<std::endl;
//free(_infoName);
//CL variables
cl::vector<cl::Platform> platforms;
cl::vector<cl::Device> devices;
cl::Context context;
cl::CommandQueue queue;
cl::Program::Sources source;
cl::Program program;
cl::vector<cl::Kernel> kernels;
std::string parameterName; //To store retrived string values from CL structures /**< TODO */
cl_long parameterValue;


/**
 * @brief CLRuntime::get_error_string Return coresponding error String
 * @param err: Error Number
 * @return
 */
const char * get_error_string(cl_int err)
{
    F_LOG;
    switch(err)
    {
    case 0: return "CL_SUCCESS";
    case -1: return "CL_DEVICE_NOT_FOUND";
    case -2: return "CL_DEVICE_NOT_AVAILABLE";
    case -3: return "CL_COMPILER_NOT_AVAILABLE";
    case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    case -5: return "CL_OUT_OF_RESOURCES";
    case -6: return "CL_OUT_OF_HOST_MEMORY";
    case -7: return "CL_PROFILING_mINFO_NOT_AVAILABLE";
    case -8: return "CL_MEM_COPY_OVERLAP";
    case -9: return "CL_mIMAGE_FORMAT_MISMATCH";
    case -10: return "CL_mIMAGE_FORMAT_NOT_SUPPORTED";
    case -11: return "CL_BUILD_mProgram_FAILURE";
    case -12: return "CL_MAP_FAILURE";

    case -30: return "CL_mINVALID_VALUE";
    case -31: return "CL_mINVALID_DEVICE_TYPE";
    case -32: return "CL_mINVALID_PLATFORM";
    case -33: return "CL_mINVALID_DEVICE";
    case -34: return "CL_mINVALID_mContext";
    case -35: return "CL_mINVALID_mQueue_PROPERTIES";
    case -36: return "CL_mINVALID_COMMAND_mQueue";
    case -37: return "CL_mINVALID_HOST_PTR";
    case -38: return "CL_mINVALID_MEM_OBJECT";
    case -39: return "CL_mINVALID_mIMAGE_FORMAT_DESCRIPTOR";
    case -40: return "CL_mINVALID_mIMAGE_SIZE";
    case -41: return "CL_mINVALID_SAMPLER";
    case -42: return "CL_mINVALID_BINARY";
    case -43: return "CL_mINVALID_BUILD_OPTIONS";
    case -44: return "CL_mINVALID_mProgram";
    case -45: return "CL_mINVALID_mProgram_EXECUTABLE";
    case -46: return "CL_mINVALID_KERNEL_NAME";
    case -47: return "CL_mINVALID_KERNEL_DEFINITION";
    case -48: return "CL_mINVALID_KERNEL";
    case -49: return "CL_mINVALID_ARG_mINDEX";
    case -50: return "CL_mINVALID_ARG_VALUE";
    case -51: return "CL_mINVALID_ARG_SIZE";
    case -52: return "CL_mINVALID_KERNEL_ARGS";
    case -53: return "CL_mINVALID_WORK_DIMENSION";
    case -54: return "CL_mINVALID_WORK_GROUP_SIZE";
    case -55: return "CL_mINVALID_WORK_mITEM_SIZE";
    case -56: return "CL_mINVALID_GLOBAL_OFFSET";
    case -57: return "CL_mINVALID_EVENT_WAIT_LIST";
    case -58: return "CL_mINVALID_EVENT";
    case -59: return "CL_mINVALID_OPERATION";
    case -60: return "CL_mINVALID_GL_OBJECT";
    case -61: return "CL_mINVALID_mBuffer_SIZE";
    case -62: return "CL_mINVALID_MIP_LEVEL";
    case -63: return "CL_mINVALID_GLOBAL_WORK_SIZE";
    default: return "Unknown OpenCL error";
    }
}



/**
 * @brief CLRuntime::getImgType // take number
 *        image type number (from cv::Mat.type()), get OpenCV's enum string.
 * @param imgTypeInt
 * @return
 */
std::string getImgType(int imgTypeInt)
{
    int numImgTypes = 35; // 7 base types, with five channel options each (none or C1, ..., C4)

    int enum_mInts[] =       {CV_8U,  CV_8UC1,  CV_8UC2,  CV_8UC3,  CV_8UC4,
                              CV_8S,  CV_8SC1,  CV_8SC2,  CV_8SC3,  CV_8SC4,
                              CV_16U, CV_16UC1, CV_16UC2, CV_16UC3, CV_16UC4,
                              CV_16S, CV_16SC1, CV_16SC2, CV_16SC3, CV_16SC4,
                              CV_32S, CV_32SC1, CV_32SC2, CV_32SC3, CV_32SC4,
                              CV_32F, CV_32FC1, CV_32FC2, CV_32FC3, CV_32FC4,
                              CV_64F, CV_64FC1, CV_64FC2, CV_64FC3, CV_64FC4};

    std::string enum_strings[] = {"CV_8U",  "CV_8UC1",  "CV_8UC2",  "CV_8UC3",  "CV_8UC4",
                                  "CV_8S",  "CV_8SC1",  "CV_8SC2",  "CV_8SC3",  "CV_8SC4",
                                  "CV_16U", "CV_16UC1", "CV_16UC2", "CV_16UC3", "CV_16UC4",
                                  "CV_16S", "CV_16SC1", "CV_16SC2", "CV_16SC3", "CV_16SC4",
                                  "CV_32S", "CV_32SC1", "CV_32SC2", "CV_32SC3", "CV_32SC4",
                                  "CV_32F", "CV_32FC1", "CV_32FC2", "CV_32FC3", "CV_32FC4",
                                  "CV_64F", "CV_64FC1", "CV_64FC2", "CV_64FC3", "CV_64FC4"};

    for(int i=0; i<numImgTypes; i++)
    {
        if(imgTypeInt == enum_mInts[i]) return enum_strings[i];
    }
    return "unknown image type";
}


/**
 * @brief Reads the .cl file and creates the kernels out of it.
 *
 * @param kernelFileName
 */
void createKernel(std::string kernelFileName)
{
    F_LOG;
    std::cout<<kernelFileName.c_str()<<std::endl<<std::endl<<std::endl;
    try
    {
        std::ifstream programFile(kernelFileName.c_str());
        std::string programString(std::istreambuf_iterator<char>(programFile),
                                  (std::istreambuf_iterator<char>()));
        std::cout<<"program STRING: "<<programString.c_str()<<std::endl<<std::endl<<std::endl;
        source = cl::Program::Sources(1, std::make_pair(programString.c_str(),
                                                          programString.length()+1));

        program = cl::Program(context, source);
        program.build(devices);
        program.createKernels(&kernels);

        if(1)
        {
            for(int _mI=0; _mI<kernels.size(); _mI++)
            {
                parameterName = kernels[_mI].getInfo<CL_KERNEL_FUNCTION_NAME>();
                std::cout << "Kernel: " << parameterName << std::endl;
            }
        }
    }
    catch(cl::Error err)
    {
        std::cout<<err.what()<<"Error code: "<<get_error_string(err.err())<<std::endl;

    }
}


