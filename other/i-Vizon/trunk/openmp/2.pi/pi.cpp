#include <iostream>
#include <omp.h>

/****
 *
 *      ---- 1
 *      |
 *      | (4.0)/(1+x*x) dx = pi
 *      |
 * 0  ---
 *
 *
 */

#define NUM_THREADS 8
int main()
{
    std::cout << "Calcuating the pi by integrating the area under the curve" << std::endl;
    double startTime, endTime, serialProfiling, parallelProfiling;
    double efficiency;
    static long numSteps = 1000000;
    //===============================SERIAL VERSION
    /***
     *  Let us understand the pi maths and its implemention in below C code.
     *  This is very crusial in parllelsizing the algorithm.
     *  The better you know the algorithm the better you extract the parrallism in it.
     */
    {

        double step;
        int i;
        double x, pi, sum=0.0;
        step = 1.0/(double)numSteps;
        startTime = omp_get_wtime();
        for(i=0; i<numSteps; i++)
        {
            x = (i+0.5)*step;
            sum = sum + 4.0/(1.0+x*x);
        }
        endTime = omp_get_wtime();
        serialProfiling = endTime - startTime;
        std::cout<<"Timetaken is "<<serialProfiling<<std::endl;
        pi = step * sum;
        std::cout<<"Serial Version -----> pi value is "<<pi<<std::endl;
    }
    //================================OMP VERSION 1 with wrong implementation
    {
        //        double step;

        //        int i;
        //        double x, pi, sum=0.0;
        //        step = 1.0/(double)numSteps;
        //        omp_set_num_threads(NUM_THREADS);
        //        static double localSum[NUM_THREADS];
        //        startTime = omp_get_wtime();
        //#pragma omp parallel
        //        {
        //            int ID = omp_get_thread_num();
        //            for(i=0; i<numSteps; i++)
        //            {
        //                x = (i+0.5)*step;
        //                localSum[ID] +=  4.0/(1.0+x*x);
        //            }
        //        }
        //        endTime = omp_get_wtime();
        //        std::cout<<"Timetaken is "<<endTime - startTime<<std::endl;
        //        pi = step * sum;
        //        for(i=0; i<NUM_THREADS; i++)
        //            pi += step * localSum[i];
        //        std::cout<<"OMP Version 1 -----> pi value is "<<pi<<std::endl;
    }
    //================================OMP VERSION 2 using Algorithm Strategy SPMD
    // Sigfial Distribution way
    {
        int i;
        double step;
        step = 1.0/(double)numSteps;
        double x,pi;
        double startTime, endTime;
        double localSum[NUM_THREADS];
        omp_set_num_threads(NUM_THREADS);
        startTime = omp_get_wtime();
#pragma omp parallel
        {
            int i, numThreads, threadId;
            double x;
            numThreads = omp_get_num_threads();
            threadId = omp_get_thread_num();
            for(i=threadId,localSum[threadId]=0.0; i<numSteps; i = i + numThreads)
            {
                x = (i+0.5)*step;
                localSum[threadId] +=  4.0/(1.0+x*x);
            }
        }
        endTime = omp_get_wtime();
        parallelProfiling = endTime - startTime;
        std::cout<<"Timetaken is "<<parallelProfiling<<std::endl;
        for(i=0, pi=0.0; i<NUM_THREADS; i++)
        {
            pi += step * localSum[i];
        }
        std::cout<<"OMP Version 2 -----> pi value is "<<pi<<std::endl;
        //      threadId 0              threadId 1
        //  i   i+numThreads        i   i+numThreads
        //  0       2               1       3
        //  2       4               3       5
        //  4       6               5       7
        //  6       8               7       9 ....

        //      threadId 0          threadId 1          threadId 2          threadId 3
        //  i   i+numThreads    i   i+numThreads    i   i+numThreads    i   i+numThreads
        //  0       4           1       5           2       6           3       7
        //  4       8           5       9           6       10          7       11
        //  8       12          9    parallelProfiling   13          10      14          11      15
        //  12      16          13      17          14      18          15      19...
        efficiency = ((serialProfiling-parallelProfiling)/serialProfiling)*100;
        std::cout<<"Efficiency using OpenMP is "
                <<efficiency<<"%"<<std::endl;
    }

    //================================OMP VERSION 2 using Algorithm Strategy SPMD
    {

        int i;
        double step;
        step = 1.0/(double)numSteps;
        double x,pi;
        double startTime, endTime;
        #define PAD 8
        double localSum[NUM_THREADS][PAD];
        omp_set_num_threads(NUM_THREADS);
        startTime = omp_get_wtime();
#pragma omp parallel
        {
            int i, numThreads, threadId;
            double x;
            numThreads = omp_get_num_threads();
            threadId = omp_get_thread_num();
            for(i=threadId,localSum[threadId][0]=0.0; i<numSteps; i = i + numThreads)
            {
                x = (i+0.5)*step;
                localSum[threadId][0] +=  4.0/(1.0+x*x);
            }
        }
        endTime = omp_get_wtime();
        parallelProfiling = endTime - startTime;
        std::cout<<"Timetaken is "<<parallelProfiling<<std::endl;
        for(i=0, pi=0.0; i<NUM_THREADS; i++)
        {
            pi += step * localSum[i][0];
        }
        std::cout<<"OMP Version 2 -----> pi value is "<<pi<<std::endl;
        //      threadId 0              threadId 1
        //  i   i+numThreads        i   i+numThreads
        //  0       2               1       3
        //  2       4               3       5
        //  4       6               5       7
        //  6       8               7       9 ....

        //      threadId 0          threadId 1          threadId 2          threadId 3
        //  i   i+numThreads    i   i+numThreads    i   i+numThreads    i   i+numThreads
        //  0       4           1       5           2       6           3       7
        //  4       8           5       9           6       10          7       11
        //  8       12          9    parallelProfiling   13          10      14          11      15
        //  12      16          13      17          14      18          15      19...
        std::cout<<"serialProfiling :"<<serialProfiling<<std::endl;
        std::cout<<"parallelProfiling :"<<parallelProfiling<<std::endl;
        efficiency = ((serialProfiling-parallelProfiling)/serialProfiling)*100;

        std::cout<<"\nEfficiency using OpenMP is "
                <<efficiency<<"%"<<std::endl;
    }

    return 0;
}
