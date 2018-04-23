#include "clruntime.h"

CLRuntime::CLRuntime() : _mBufferIndex(0), _mImageIndex(0)
{
    F_LOG;
}

CLRuntime::~CLRuntime()
{
    F_LOG;

}
/**
 * @brief CLRuntime::init
 * Initializes the platform and get the devices and sets the context(device handle) for
 * interested device
 */
void CLRuntime::initCL()
{
    F_LOG;
    try
    {
        cl::Platform::get(&_mPlatforms);
        _mPlatforms[0].getDevices(CL_DEVICE_TYPE_GPU, &_mDevices);
        _mContext = cl::Context(_mDevices);
        _mQueue = cl::CommandQueue(_mContext, _mDevices[0]);


        if(CLDEBUG)
        {
            _mPlatforms[0].getInfo((cl_platform_info)CL_PLATFORM_VENDOR, &_mParameterName);
            std::cout<<"===> CL_PLATFORM_VENDOR           \t: "<<_mParameterName.c_str()<<std::endl;

            _mDevices[0].getInfo((cl_device_info)CL_DEVICE_NAME, &_mParameterName);
            std::cout<<"===> CL_DEVICE_NAME               \t: "<<_mParameterName.c_str()<<std::endl;

            _mDevices[0].getInfo((cl_device_info)CL_DEVICE_IMAGE2D_MAX_HEIGHT, &_mParameterValue);
            std::cout<<"===> CL_DEVICE_IMAGE2D_MAX_HEIGHT \t: "<<_mParameterValue<<std::endl;

            _mDevices[0].getInfo((cl_device_info)CL_DEVICE_IMAGE2D_MAX_WIDTH, &_mParameterValue);
            std::cout<<"===> CL_DEVICE_mIMAGE2D_MAX_WIDTH  \t: "<<_mParameterValue<<std::endl;

            _mDevices[0].getInfo((cl_device_info)CL_DEVICE_MAX_WORK_GROUP_SIZE, &_mParameterValue);
            std::cout<<"===> CL_DEVICE_MAX_WORK_GROUP_SIZE\t: "<<_mParameterValue<<std::endl;

            _mDevices[0].getInfo((cl_device_info)CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, &_mParameterValue);
            std::cout<<"===> CL_DEVICE_MAX_WORK_mITEM_DIMENSIONS \t: "<<_mParameterValue<<std::endl;

            _mContext.getInfo((cl_context_info)CL_CONTEXT_NUM_DEVICES, &_mParameterValue);
            std::cout<<"===> CL_mContext_NUM_mDevices       \t: "<<_mParameterValue<<std::endl;
        }
    }
    catch(cl::Error err)
    {
        std::cout<<err.what()<<"Error code: "<<get_error_string(err.err())<<std::endl;
    }
}

/**
 * @brief CLRuntime::createKernel
 * @param kernelFileName
 * With given @param kernelFileName this method extracts the kernels from
 * source file
 */
void CLRuntime::createKernel(std::string kernelFileName)
{
    F_LOG;
    try
    {
        std::ifstream programFile(kernelFileName.c_str());
        std::string programString(std::istreambuf_iterator<char>(programFile),
                                  (std::istreambuf_iterator<char>()));
        _mSource = cl::Program::Sources(1, std::make_pair(programString.c_str(),
                                                          programString.length()+1));

        _mProgram = cl::Program(_mContext, _mSource);
        _mProgram.build(_mDevices);
        _mProgram.createKernels(&_mKernels);

        if(CLDEBUG)
        {
            for(_mI=0; _mI<_mKernels.size(); _mI++) {
                _mParameterName = _mKernels[_mI].getInfo<CL_KERNEL_FUNCTION_NAME>();
                std::cout << "Kernel: " << _mParameterName << std::endl;
            }
        }
    }
    catch(cl::Error err)
    {
        std::cout<<err.what()<<"Error code: "<<get_error_string(err.err())<<std::endl;

    }
}

/**
 * @brief CLRuntime::createInteger transfers the integer data to kernel
 * @param argNum To which kernel argument you want to pass
 * @param data Data
 *
 */
void CLRuntime::createIntegerArg(int argNum, int data)
{
    _mKernels[0].setArg(argNum, data);
}

void CLRuntime::createFloatArg(int argNum, float data)
{
    _mKernels[0].setArg(argNum, data);
}

/**
 * @brief CLRuntime::create1DBuffer Creates a 1D memory buffer
 * @param argNum: To which kernel argument you want to pass
 * @param size: Buffer Size
 * @param data: Void pointer to ths data memory
 */
void CLRuntime::create1DBufferArg(int argNum, size_t size, void *data)
{
    F_LOG;
    try
    {
        std::cout << (int*) data<<std::endl;
        std::cout << "SIze :"<<size<<std::endl;
        if(data != NULL)
        {
            _mBuffer.push_back(cl::Buffer(_mContext, CL_MEM_READ_ONLY,
                                          size, data));
            _mQueue.enqueueWriteBuffer(_mBuffer[argNum], CL_TRUE, 0, size, data);
            std::cout<<"$$$Buffer Input 2222222222222222222\n";
        }
        else
        {
            _mBuffer.push_back(cl::Buffer(_mContext, CL_MEM_WRITE_ONLY,
                                          size));
            std::cout<<"$$$Buffer Output 111111111111111111111\n";
        }


        _mKernels[0].setArg(argNum, _mBuffer[argNum]);


        if(CLDEBUG)
        {
            std::cout << "===> 1D Buffer Element No: "<<_mBuffer.size()<<"\n";
            std::cout << "===> 1D Buffer MEM Size\t: "
                      <<_mBuffer[argNum].getInfo<CL_MEM_SIZE>()<<"bytes\n" ;
        }
        _mBufferIndex++;
    }
    catch(cl::Error err)
    {
        std::cout<<err.what()<<"Error code: "<<get_error_string(err.err())<<std::endl;
    }
}

void CLRuntime::readBuffer(void *data, size_t size)
{
    F_LOG;
    try
    {
        _mQueue.enqueueReadBuffer(_mBuffer[1], CL_TRUE, size_t(0), (size_t) 2048, data);
    }
    catch(cl::Error err)
    {
        std::cout<<err.what()<<"Error code: "<<get_error_string(err.err())<<std::endl;
    }
}

void CLRuntime::create2DImageBufferArg(int argNum, cl::ImageFormat &format,
                                       size_t width, size_t height, size_t rowPitch, void *data)
{
    F_LOG;
    try
    {
        if(data != NULL)
            _mImage2D.push_back(cl::Image2D(_mContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, format, width, height, rowPitch, data));
        else
            _mImage2D.push_back(cl::Image2D(_mContext, CL_MEM_WRITE_ONLY, format, width, height, rowPitch, NULL));

        _mKernels[0].setArg(argNum, _mImage2D[argNum]);

        std::cout << "===> Image2D _mImageIndex size \t: "<<argNum<<"\n";

        if(CLDEBUG)
        {
            std::cout << "===> Image2D Buffer elements \t: "<<_mImage2D.size()<<"\n";
            std::cout << "===> Image2D Buffer MEM Size \t: "
                      <<_mImage2D[argNum].getInfo<CL_MEM_SIZE>()<<" bytes\n" ;
            std::cout << "===> Image2D Buffer MEM Type \t: "
                      <<_mImage2D[argNum].getInfo<CL_MEM_TYPE>()<<" bytes\n" ;
        }
    }
    catch(cl::Error err)
    {
        std::cout<<err.what()<<"Error code: "<<get_error_string(err.err())<<std::endl;
    }
}

void CLRuntime::read2DImageBuffer(int argNum, size_t width,size_t height, void *data)
{
    F_LOG;
    try
    {
        cl::size_t<3> origin, region;
        origin[0] = 0; origin[1] = 0; origin[2] = 0;
        region[0] = width; region[1] = height; region[2] = 1;
        _mQueue.enqueueReadImage(_mImage2D[argNum], CL_TRUE, origin, region, 0, 0, (void*)data);
    }
    catch(cl::Error err)
    {
        std::cout<<err.what()<<"Error code: "<<get_error_string(err.err())<<std::endl;
    }

}

void CLRuntime::kernelExecute(cl::NDRange &offset, cl::NDRange &globalSize, cl::NDRange &localSize)
{
    F_LOG;
    try
    {
        _mQueue.enqueueNDRangeKernel(_mKernels[0], offset, globalSize, localSize);
        if(CLDEBUG)
        {
            std::cout << "===> CL_KERNEL_WORK_GROUP_SIZE\t:"
                      <<_mKernels[0].getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(_mDevices[0])<<"\n" ;
            std::cout << "===> CL_KERNEL_COMPILE_WORK_GROUP_SIZE\t:"
                      <<_mKernels[0].getWorkGroupInfo<CL_KERNEL_COMPILE_WORK_GROUP_SIZE>(_mDevices[0])<<"\n" ;
        }
    }
    catch(cl::Error err)
    {
        std::cout<<err.what()<<"Error code: "<<get_error_string(err.err())<<std::endl;
    }
}

/**
 * @brief CLRuntime::get_error_string Return coresponding error String
 * @param err: Error Number
 * @return
 */
const char * CLRuntime::get_error_string(cl_int err)
{
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
std::string CLRuntime::getImgType(int imgTypeInt)
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
