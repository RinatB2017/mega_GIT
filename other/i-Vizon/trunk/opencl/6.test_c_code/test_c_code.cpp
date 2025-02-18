
#define _CRT_SECURE_NO_WARNINGS
#define PROGRAM_FILE "interp.cl"
#define KERNEL_FUNC "interp"

#define SCALE_FACTOR 3

#define PNG_DEBUG 3
#include <png.h>

#define INPUT_FILE "input.png"
#define OUTPUT_FILE "output.png"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef MAC
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

/* Find a GPU or CPU associated with the first available platform */
cl_device_id create_device() {

   cl_platform_id platform;
   cl_device_id dev;
   int err;

   /* Identify a platform */
   err = clGetPlatformIDs(1, &platform, NULL);
   if(err < 0) {
      perror("Couldn't identify a platform");
      exit(1);
   }

   /* Access a device */
   err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &dev, NULL);
   if(err == CL_DEVICE_NOT_FOUND) {
      err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &dev, NULL);
   }
   if(err < 0) {
      perror("Couldn't access any devices");
      exit(1);
   }

   return dev;
}

/* Create program from a file and compile it */
cl_program build_program(cl_context ctx, cl_device_id dev, const char* filename) {

   cl_program program;
   FILE *program_handle;
   char *program_buffer, *program_log;
   size_t program_size, log_size;
   int err;
   char arg[20];

   /* Read program file and place content into buffer */
   program_handle = fopen("intrep.cl", "r");
   if(program_handle == NULL) {
      perror("Couldn't find the program file");
      exit(1);
   }
   fseek(program_handle, 0, SEEK_END);
   program_size = ftell(program_handle);
   rewind(program_handle);
   program_buffer = (char*)malloc(program_size + 1);
   program_buffer[program_size] = '\0';
   fread(program_buffer, sizeof(char), program_size, program_handle);
   fclose(program_handle);

   /* Create program from file */
   program = clCreateProgramWithSource(ctx, 1,
      (const char**)&program_buffer, &program_size, &err);
   if(err < 0) {
      perror("Couldn't create the program");
      exit(1);
   }
   free(program_buffer);

   /* Create build argument */
   sprintf(arg, "-DSCALE=%u", SCALE_FACTOR);

   /* Build program */
   err = clBuildProgram(program, 0, NULL, arg, NULL, NULL);
   if(err < 0) {

      /* Find size of log and print to std output */
      clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG,
            0, NULL, &log_size);
      program_log = (char*) malloc(log_size + 1);
      program_log[log_size] = '\0';
      clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG,
            log_size + 1, program_log, NULL);
      printf("%s\n", program_log);
      free(program_log);
      exit(1);
   }

   return program;
}

void read_image_data(const char* filename, png_bytep* input, png_bytep* output, size_t* w, size_t* h) {

   int i;

   /* Open input file */
   FILE *png_input;
   if((png_input = fopen(filename, "rb")) == NULL) {
      perror("Can't read input image file");
      exit(1);
   }

   /* Read image data */
   png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
   png_infop info_ptr = png_create_info_struct(png_ptr);
   png_init_io(png_ptr, png_input);
   png_read_info(png_ptr, info_ptr);
   *w = png_get_image_width(png_ptr, info_ptr);
   *h = png_get_image_height(png_ptr, info_ptr);

   /* Allocate input/output memory and initialize data */
   *input = (png_bytep)malloc(*h * png_get_rowbytes(png_ptr, info_ptr));
   *output = (png_bytep)malloc(*h * png_get_rowbytes(png_ptr, info_ptr) * SCALE_FACTOR  * SCALE_FACTOR );
   for(i=0; i<*h; i++) {
      png_read_row(png_ptr, *input + i * png_get_rowbytes(png_ptr, info_ptr), NULL);
   }

   /* Close input file */
   png_read_end(png_ptr, info_ptr);
   png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
   fclose(png_input);
}

void write_image_data(const char* filename, png_bytep data, size_t w, size_t h) {

   int i;

   /* Open output file */
   FILE *png_output;
   if((png_output = fopen(filename, "wb")) == NULL) {
      perror("Create output image file");
      exit(1);
   }

   /* Write image data */
   png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
   png_infop info_ptr = png_create_info_struct(png_ptr);
   png_init_io(png_ptr, png_output);
   png_set_IHDR(png_ptr, info_ptr, w, h, 16,
         PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
         PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
   png_write_info(png_ptr, info_ptr);
   for(i=0; i<h; i++) {
      png_write_row(png_ptr, data + i * png_get_rowbytes(png_ptr, info_ptr));
   }

   /* Close file */
   png_write_end(png_ptr, NULL);
   png_destroy_write_struct(&png_ptr, &info_ptr);
   fclose(png_output);
}

int main(int argc, char **argv) {

   /* Host/device data structures */
   cl_device_id device;
   cl_context context;
   cl_command_queue queue;
   cl_program program;
   cl_kernel kernel;
   cl_int err;
   size_t global_size[2];

   /* Image data */
   png_bytep input_pixels, output_pixels;
   cl_image_format png_format;
   cl_mem input_image, output_image;
   size_t width, height;
   size_t origin[3], region[3];

   /* Open input file and read image data */
   read_image_data(INPUT_FILE, &input_pixels, &output_pixels, &width, &height);
   printf("Image Width is %d\n", width);
   printf("Image height is %d\n", height);
   /* Create a device and context */
   device = create_device();
   context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
   if(err < 0) {
      perror("Couldn't create a context");
      exit(1);
   }

   /* Create kernel */
   program = build_program(context, device, PROGRAM_FILE);
   kernel = clCreateKernel(program, KERNEL_FUNC, &err);
   if(err < 0) {
      printf("Couldn't create a kernel: %d", err);
      exit(1);
   };

   /* Create input image object */
   png_format.image_channel_order = CL_R;
   png_format.image_channel_data_type = CL_UNORM_INT16;
   input_image = clCreateImage2D(context,
         CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
         &png_format, width, height, 0, (void*)input_pixels, &err);
   if(err < 0) {
      perror("Couldn't create the image object");
      exit(1);
   };

   /* Create output image object */
   output_image = clCreateImage2D(context,
         CL_MEM_WRITE_ONLY, &png_format, SCALE_FACTOR * width,
         SCALE_FACTOR * height, 0, NULL, &err);
   if(err < 0) {
      perror("Couldn't create the image object");
      exit(1);
   };

   /* Create buffer */
   if(err < 0) {
      perror("Couldn't create a buffer");
      exit(1);
   };

   /* Create kernel arguments */
   err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_image);
   err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_image);
   if(err < 0) {
      printf("Couldn't set a kernel argument");
      exit(1);
   };

   /* Create a command queue */
   queue = clCreateCommandQueue(context, device, 0, &err);
   if(err < 0) {
      perror("Couldn't create a command queue");
      exit(1);
   };

   /* Enqueue kernel */
   global_size[0] = width; global_size[1] = height;
   err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_size,
         NULL, 0, NULL, NULL);
   if(err < 0) {
      perror("Couldn't enqueue the kernel");
      exit(1);
   }

   /* Read the image object */
   origin[0] = 0; origin[1] = 0; origin[2] = 0;
   region[0] = SCALE_FACTOR * width; region[1] = SCALE_FACTOR * height; region[2] = 1;
   err = clEnqueueReadImage(queue, output_image, CL_TRUE, origin,
         region, 0, 0, (void*)output_pixels, 0, NULL, NULL);
   if(err < 0) {
      perror("Couldn't read from the image object");
      exit(1);
   }

   /* Create output PNG file and write data */
   write_image_data(OUTPUT_FILE, output_pixels, SCALE_FACTOR * width, SCALE_FACTOR * height);

   /* Deallocate resources */
   free(input_pixels);
   free(output_pixels);
   clReleaseMemObject(input_image);
   clReleaseMemObject(output_image);
   clReleaseKernel(kernel);
   clReleaseCommandQueue(queue);
   clReleaseProgram(program);
   clReleaseContext(context);
   return 0;
}


//#include "main.h"

////To switch between Camera or JPEG Image
//#define NO_CAMERA 1
//int main()
//{
//    F_LOG;
//    cv::Mat image;
//    cv::VideoCapture cap(-1);

//    //OpenCL Initialization
//    cl::Platform::get(&platforms);
//    platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);
//    context = cl::Context(devices);
//    queue = cl::CommandQueue(context, devices[0]);
//    createKernel("rotation.cl");

//    float theta = 3.14159/6;
//    float cos_theta = cos(theta);
//    float sin_theta = sin(theta);
//#if NO_CAMERA
//    //Image file read using OpenCV
//    image = cv::imread("shiva.jpg");
//#else
//    while(1)
//    {
//        cap >> image;
//#endif
//        cv::cvtColor(image, image, cv::COLOR_BGRA2GRAY);
//        std::cout<<"===> Image channeles :\t"<< image.channels()<<std::endl;
//        std::cout<<"===> Image width :\t"<< image.size().width<<std::endl;
//        std::cout<<"===> Image size :\t"<< image.size()<<std::endl;
//        // cv::imshow("Original", image);
//        //cv::waitKey(10); //Waits for user to close the window


//        int W = image.size().width;
//        int H = image.size().height;

//        cl::Buffer src, dst;
//        src = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(unsigned char) * W * H, image.data );
//        dst = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(unsigned char) * W * H);

//        kernels[0].setArg(0, src);
//        kernels[0].setArg(1, dst);
//        kernels[0].setArg(2, W);
//        kernels[0].setArg(3, H);
//        kernels[0].setArg(4, sin_theta);
//        kernels[0].setArg(5, cos_theta);


//        //Time to run the Kernel
//        cl::NDRange offset(cl::NullRange);
//        cl::NDRange globalSize(image.size().width, image.size().height);
//        //cl::NDRange localSize(cl::NullRange);
//        cl::NDRange localSize(16, 16);
//        queue.enqueueNDRangeKernel(kernels[0], offset, globalSize, localSize);

//        queue.enqueueReadBuffer(dst, CL_TRUE, 0, W*H*sizeof(unsigned char), (unsigned char*)image.data);

//        cv::imshow("Rotated", image);


//#if NO_CAMERA
//        cv::waitKey(100);

//        //Testing Image Read and Write
//        image = cv::imread("shiva.jpg");
//        cv::cvtColor(image, image, CV_BGR2BGRA, 4);
//        //cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

//        uchar* camData = new uchar[image.total()*4];
//        cv::Mat RGBA(image.size(), CV_8UC4, camData);
//        cv::cvtColor(image, RGBA, CV_BGR2RGBA, 4);


//        cl::ImageFormat format(CL_R, CL_UNORM_INT8);
//        cl::Image2D dSourceImage, dDstImage;
//        dSourceImage = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
//                                   format,
//                                   image.size().width, image.size().height,
//                                   0, RGBA.data);
//        dDstImage = cl::Image2D(context, CL_MEM_WRITE_ONLY,
//                                format,
//                                image.size().width, image.size().height);

//        kernels[1].setArg(0, dSourceImage);
//        kernels[1].setArg(1, dDstImage);
//        queue.enqueueNDRangeKernel(kernels[0], offset,
//                globalSize, localSize);
//        // Elements to per dimension
//        cl::size_t<3> origin;
//        origin.push_back(0); origin.push_back(0); origin.push_back(0);
//        cl::size_t<3> region;
//        region.push_back(image.size().width);
//        region.push_back(image.size().height);
//        region.push_back(1);

//        queue.enqueueReadImage(dDstImage, CL_TRUE,
//                               origin, region,
//                               0, 0,
//                               RGBA.data);
//        cv::imshow("SimpeRW", RGBA);
//        cv::waitKey();
//#else
//        cv::waitKey(10);
//    }
//#endif
//    return 0;
//}

