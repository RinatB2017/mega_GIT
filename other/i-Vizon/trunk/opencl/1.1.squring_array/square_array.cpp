#include <cl_wrapper.hpp>
#include <iv_profiler.h>
#include <math.h>
#include <time.h>

//#define NUM_ELEMENTS 64
//#define NUM_ELEMENTS 640
//#define NUM_ELEMENTS 6400
//#define NUM_ELEMENTS 64000
#define NUM_ELEMENTS 640000
int main()
{
    std::cout << "Squaring an array using OpenCL kernels" << std::endl;
    iv::Profiler cpuTime;
    struct timespec startTime, endTime;
    double cpuProfiling, gpuProfiling, profileDifference;
    double efficiency;
    //==============================CPU Section
    float a[NUM_ELEMENTS], b[NUM_ELEMENTS];
    int i;
    for(i=0; i<NUM_ELEMENTS; i++)
        a[i] = i * i;

    if(clock_gettime(CLOCK_REALTIME, &startTime))
        DEBUG_STRING("Error: clock_getres");

    //Testing iv::Profiler API
    cpuTime.startProfiler();

    for(i=0; i<NUM_ELEMENTS; i++)
        b[i] = sqrt(a[i]);

    //Testing iv::Profiler API
    cpuTime.endProfiler();

    if(clock_gettime (CLOCK_REALTIME, &endTime))
        DEBUG_STRING("Error: clock_getres");

    cpuProfiling = endTime.tv_nsec - startTime.tv_nsec;

    //Testing iv::Profiler API
    DEBUG_VALUE("Timer Value :", cpuTime.elapsedTimeInNanosecs());

    std::cout<<"\nCPU resuilts :"<<std::endl;
//    for(i=0; i<NUM_ELEMENTS; i++)
//            std::cout<<" "<<b[i];

    //==============================OpenCL Section
    iv::CLSetup cl;
    iv::Program* program;
    iv::KernelLauncher* kl;
    iv::Buffer* array_buffer_in;
    iv::Buffer* array_buffer_out;
    std::string file = "square_array.cl";
    std::vector<std::string> filesPath;
    filesPath.push_back(file);

    cl.init();
    cl.getDeviceName();
    cl.getSupportedExtensions();

    program = cl.createProgram(filesPath);
    program->buildProgram();
    kl = program->createKernelLauncher("cl_square_array_int");

    array_buffer_in = cl.createBuffer(sizeof(a),
                                      CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                      a);
    array_buffer_out = cl.createBuffer(sizeof(b),
                                       CL_MEM_WRITE_ONLY,
                                       b);
    kl->pArg(array_buffer_in->getMem())
            ->pArg(array_buffer_out->getMem());
    kl->pGlobal(16)->pLocal(4);

    if(clock_gettime(CLOCK_REALTIME, &startTime))
        DEBUG_STRING("Error: clock_getres");

    kl->run();

    if(clock_gettime (CLOCK_REALTIME, &endTime))
        DEBUG_STRING("Error: clock_getres");

    gpuProfiling = endTime.tv_nsec - startTime.tv_nsec;

    array_buffer_out->write(b, sizeof(b));

    std::cout<<"\nGPGPU results :"<<std::endl;
//    for(i=0; i<NUM_ELEMENTS; i++)
//            std::cout<<" "<<b[i];

    std::cout<<"\nCPU time taken : "<<cpuProfiling;
    std::cout<<"\nGPU time taken : "<<gpuProfiling;

    profileDifference = cpuProfiling - gpuProfiling;
    efficiency = (profileDifference / cpuProfiling) * 100;

    std::cout<<"\nEfficiency is "<<efficiency<<"%";
    std::cout<<std::endl;
    return 0;
}

