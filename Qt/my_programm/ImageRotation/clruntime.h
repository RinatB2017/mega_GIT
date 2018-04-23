#ifndef CLRUNTIME_H
#define CLRUNTIME_H

#define __NO_STD_VECTOR // Use cl::vector instead of STL version
#define __CL_ENABLE_EXCEPTIONS
///#define __NO_STD_STRING
#define CLDEBUG 1




#include <CL/cl.hpp>

#include <iostream>
#include <fstream>
#include <iostream>
#include <iterator>

#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>

#define CL_DEBUG 1
#ifdef CL_DEBUG
#define F_LOG LogBlock _l(__func__)
/**
 * @brief
 *
 */
struct LogBlock {
    const char *mLine; /**< TODO */
    /**
 * @brief
 *
 * @param line
 */
    LogBlock(const char *line) : mLine(line) {
        std::cout<<mLine <<"  -----> Enter \n";
    }
    /**
     * @brief
     *
     */
    ~LogBlock() {
        std::cout<<mLine <<" <----- Leave \n";
    }
};
#else
FLOG
#endif

/**
 * @brief
 * ha ha ha ha
 */
class CLRuntime
{
    public:
    cl::vector<cl::Platform> _mPlatforms; //To store list of available plaforms in the current machine /**< TODO */
    cl::vector<cl::Device> _mDevices; //To store list of available OpenCL devices /**< TODO */
    cl::Context _mContext; //Device handle to execute the kernels /**< TODO */
    cl::Program::Sources _mSource; //To open to *.cl files /**< TODO */
    cl::Program _mProgram; //To store source code strings /**< TODO */
    cl::vector<cl::Kernel> _mKernels; //To load kernels in *.cl files /**< TODO */
    std::string _mParameterName; //To store retrived string values from CL structures /**< TODO */
    cl_long _mParameterValue; //To store retrived long values from CL structures /**< TODO */
    cl_uint _mI; /**< TODO */

    cl::vector<cl::Buffer> _mBuffer; //1D buffer on the device /**< TODO */
    cl_uint _mBufferIndex; /**< TODO */

    cl::vector<cl::Image2D> _mImage2D; /**< TODO */
    cl_uint _mImageIndex; /**< TODO */

    cl::CommandQueue _mQueue; /**< TODO */
    public:
    cl::Context* getContext() { return &_mContext; }
    cl::Program* getProgram() { return &_mProgram; }
    /**
     * @brief
     *
     */
    CLRuntime();
    /**
     * @brief
     *
     */
    ~CLRuntime();


    /**
     * @brief
     *
     */
    void initCL();
    /**
     * @brief
     *
     * @param kernelName
     */
    void createKernel(std::string kernelName);

    /**
     * create integer
     *
     * @param argNum oh oh oh
     * @param data
     */
    void createIntegerArg(int argNum, int data);

    /**
     * create integer
     *
     * @param argNum oh oh oh
     * @param data
     */
    void createFloatArg(int argNum, float data);
    /**
     * @brief
     *
     * @param argNum
     * @param size
     * @param data
     */
    void create1DBufferArg(int argNum, size_t size, void *data= NULL);
    /**
     * @brief
     *
     * @param data
     * @param size
     */
    void readBuffer(void *data, size_t size);

    /**
     * @brief
     *
     * @param argNum
     * @param format
     * @param width
     * @param height
     * @param row_pitch
     * @param data
     */
    void create2DImageBufferArg(int argNum, cl::ImageFormat &format, size_t width,
                                size_t height, size_t row_pitch = 0, void *data = NULL);
    /**
     * @brief
     *
     * @param argNum
     * @param width
     * @param height
     * @param data
     */
    void read2DImageBuffer(int argNum, size_t width,size_t height, void *data);

    /**
     * @brief
     *
     * @param offset
     * @param globalSize
     * @param localSize
     */
    void kernelExecute(cl::NDRange &offset,
                       cl::NDRange &globalSize,
                       cl::NDRange &localSize);



    /**
     * @brief
     *
     * @param err
     * @return const char
     */
    const char * get_error_string(cl_int err);
    /**
     * @brief
     *
     * @param imgTypeInt
     * @return std::string
     */
    std::string getImgType(int imgTypeInt);
};
#endif // CLRUNTIME_H
