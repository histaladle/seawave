#ifndef PHILLIPSPECTRUM_H
#define PHILLIPSPECTRUM_H

#include <QRunnable>
#include "FT.h"
#include <cmath>
#include <ctime>
class PhillipSpectrum : public QRunnable
{
public:
    PhillipSpectrum(int row,int column,double width,double height);
    ~PhillipSpectrum();
    const int row,column;
    const double width,height;
    void run();
    FT::Complex *surface=nullptr;
    FT::Complex *h_array=nullptr;
    FT::Complex *delta_width_array=nullptr;
    FT::Complex *delta_height_array=nullptr;
    FT::Complex *grad_width_array=nullptr;
    FT::Complex *grad_height_array=nullptr;
    double gravity=9.8,windx=1.2,windz=1.0;
    double time=62;
    FT::Complex ksi;
    clock_t lasttick=0;
private:
    FT::Complex calculate_h0(double kx,double kz);
    FT::Complex calculate_h(double kx,double kz,double t);
};

#endif // PHILLIPSPECTRUM_H
