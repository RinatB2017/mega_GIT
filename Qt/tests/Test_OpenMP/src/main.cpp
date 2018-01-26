#include <QTime>

#   include <QDebug>

#include <qmath.h>

#include <omp.h>

/*
 *  omp_set_dynamic(0);
    omp_set_nested(4);
    #pragma omp parallel num_threads(4)
 *
*/

int main(void)
{
    #pragma omp parallel
    qDebug() << "Hello, world.";

#if defined(_OPENMP) 
    qDebug() << "Compiled by an OpenMP-compliant implementation.";
    qDebug() << "The result of omp_get_num_threads " << omp_get_num_threads();
#else
    qFatal() << "OpenMP not found!");
#endif

    //---
#if 1
    QTime timer;
    timer.start();

    double x = 0;
    for(int a=0; a<1000; a++)
    {
        for(int b=0; b<1000; b++)
        {
            for(int c=0; c<1000; c++)
            {
                x = sin(a*b*c);
            }
        }
    }

    qDebug() << x;
    qDebug() << "time elapsed" << timer.elapsed() << "msec";
#endif
    //---

    return 0;
}
