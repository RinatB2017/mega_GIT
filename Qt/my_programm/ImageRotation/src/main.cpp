#include "main.h"

//To switch between Camera or JPEG Image
#define NO_CAMERA true
int main()
{
    F_LOG;
    cv::Mat image;
    cv::VideoCapture cap(-1);

    //OpenCL Initialization
    cl::Platform::get(&platforms);
    platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);
    context = cl::Context(devices);
    queue = cl::CommandQueue(context, devices[0]);
    createKernel("opencl/rotation.cl");

    float theta = 3.14159/6;
    float cos_theta = cos(theta);
    float sin_theta = sin(theta);
#if NO_CAMERA
    //Image file read using OpenCV
    image = cv::imread("../ImageRotation/shiva.jpg");
#else
    while(1)
    {
        cap >> image;
#endif
        cv::cvtColor(image, image, cv::COLOR_BGRA2GRAY);
        std::cout<<"===> Image channeles :\t"<< image.channels()<<std::endl;
        std::cout<<"===> Image width :\t"<< image.size().width<<std::endl;
        std::cout<<"===> Image size :\t"<< image.size()<<std::endl;
        cv::imshow("Original", image);
        cv::waitKey(10); //Waits for user to close the window


        int W = image.size().width;
        int H = image.size().height;

        cl::Buffer src, dst;
        src = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(unsigned char) * W * H, image.data );
        dst = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(unsigned char) * W * H);

        kernels[0].setArg(0, src);
        kernels[0].setArg(1, dst);
        kernels[0].setArg(2, W);
        kernels[0].setArg(3, H);
        kernels[0].setArg(4, sin_theta);
        kernels[0].setArg(5, cos_theta);


        //Time to run the Kernel
        cl::NDRange offset(cl::NullRange);
        cl::NDRange globalSize(image.size().width, image.size().height);
        //cl::NDRange localSize(cl::NullRange);
        cl::NDRange localSize(16, 16);
        queue.enqueueNDRangeKernel(kernels[0], offset, globalSize, localSize);

        queue.enqueueReadBuffer(dst, CL_TRUE, 0, W*H*sizeof(unsigned char), (unsigned char*)image.data);

        cv::imshow("Rotated", image);

#if NO_CAMERA
        cv::waitKey();
#else
        cv::waitKey(10);
    }
#endif
    return 0;
}

