#include <cl_wrapper.hpp>

int main()
{
    std::cout << "Hello World!" << std::endl;

    //Test Specific Variables
    int a[100], b[100], c[100];
    for(int i=0; i<100; i++)
    {
        a[i] = 1;
        b[i] = 4;
    }

    //CLWrapper Variables
    iv::CLSetup cl;
    iv::KernelLauncher* kl;
    iv::Program* prog;
    std::vector<std::string> kernelFiles;
    std::string file = "matrix_ops.cl";
    kernelFiles.push_back(file);

    cl.init();

    prog = cl.createProgram(kernelFiles);
    prog->buildProgram();

    kl = prog->createKernelLauncher("matrix_add");

    //Create Kernal Arguments
    iv::Buffer* dA = cl.createBuffer(sizeof(a), CL_MEM_READ_WRITE  | CL_MEM_COPY_HOST_PTR, a);
    iv::Buffer* dB = cl.createBuffer(sizeof(b), CL_MEM_READ_WRITE  | CL_MEM_COPY_HOST_PTR, b);
    iv::Buffer* dC = cl.createBuffer(sizeof(c), CL_MEM_WRITE_ONLY, NULL);

    kl->pArg(dA->getMem())->pArg(dB->getMem())->pArg(dC->getMem());
    kl->pGlobal(100)->pLocal(10);
    kl->run();

    dC->read(c, sizeof(c));

    for(int i=0; i<100; i++)
        std::cout<<c[i]<<" ";

    return 0;
}

