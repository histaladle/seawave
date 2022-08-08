#include "PhillipSpectrum.h"
#include <random>
#include <iostream>
#include <QDebug>
#include <complex>
PhillipSpectrum::PhillipSpectrum(int row,int column,double width,double height):QRunnable ()
  ,row(row),column(column),width(width),height(height)
{
    setAutoDelete(false);
    std::default_random_engine random;
    std::normal_distribution<double> n_dist(0.0,1.0);
    surface=new FT::Complex[row*column];
    h_array=new FT::Complex[row*column];
    delta_width_array=new FT::Complex[row*column];
    delta_height_array=new FT::Complex[row*column];
    grad_width_array=new FT::Complex[row*column];
    grad_height_array=new FT::Complex[row*column];
}

void PhillipSpectrum::run()
{
    int masked_row=(1<<FT::left_mask_bit(row));
    int masked_column=(1<<FT::left_mask_bit(column));
    if(masked_row!=row||masked_column!=column) {
        return;
    }
    std::default_random_engine random;
    std::normal_distribution<double> n_dist(0.0,1.0);
    ksi.re=n_dist(random);
    ksi.im=n_dist(random);
    FT::Complex h;
    double kx=0,kz=0,k_len=0;
    for(int n=0;n<row;n++) {
        for(int m=0;m<column;m++) {
            kx=(m-column/2)*2*M_PI/width;
            kz=(n-row/2)*2*M_PI/height;
            k_len=sqrt(kx*kx+kz*kz);
            h_array[n*column+m]=calculate_h(kx,kz,time);
            if(k_len<0.001) {
                delta_width_array[n*column+m]=0;
                delta_height_array[n*column+m]=0;
            }
            else {
                delta_width_array[n*column+m]=h_array[n*column+m]*FT::Complex(0,-kx/k_len);
                delta_height_array[n*column+m]=h_array[n*column+m]*FT::Complex(0,-kz/k_len);
            }
            grad_width_array[n*column+m]=h_array[n*column+m]*FT::Complex(0,kx);
            grad_height_array[n*column+m]=h_array[n*column+m]*FT::Complex(0,kz);
//            qDebug() << "debug30" << n << m << spectrum[n*column+m].re
//                     << spectrum[n*column+m].im;
        }
    }
    FT::Complex *h_rfou=new FT::Complex[row],*h_cfou=new FT::Complex[column];
    FT::Complex *delta_width_rfou=new FT::Complex[row],*delta_width_cfou=new FT::Complex[column],*delta_height_rfou=new FT::Complex[row],*delta_height_cfou=new FT::Complex[column];
    FT::Complex *grad_width_rfou=new FT::Complex[row],*grad_width_cfou=new FT::Complex[column],*grad_height_rfou=new FT::Complex[row],*grad_height_cfou=new FT::Complex[column];
    for(int r=0;r<row;r++) {
        for(int c=0;c<column;c++) {
            FT::Complex c1(cos(2*M_PI/column*(-c*column/2+column*column/4)),sin(2*M_PI/column*(-c*column/2+column*column/4)));
            h_cfou[c]=h_array[r*column+c]*c1;
            delta_width_cfou[c]=delta_width_array[r*column+c]*c1;
            delta_height_cfou[c]=delta_height_array[r*column+c]*c1;
            grad_width_cfou[c]=grad_width_array[r*column+c]*c1;
            grad_height_cfou[c]=grad_height_array[r*column+c]*c1;
        }
        FT::fast_recur(h_cfou,column,1);
        FT::fast_recur(delta_width_cfou,column,1);
        FT::fast_recur(delta_height_cfou,column,1);
        FT::fast_recur(grad_width_cfou,column,1);
        FT::fast_recur(grad_height_cfou,column,1);
        for(int c=0;c<column;c++) {
            FT::Complex c2(cos(2*M_PI/column*-column/2*c),sin(2*M_PI/column*-column/2*c));
            surface[r*column+c]=h_cfou[c]*c2;
            delta_width_array[r*column+c]=delta_width_cfou[c]*c2;
            delta_height_array[r*column+c]=delta_height_cfou[c]*c2;
            grad_width_array[r*column+c]=grad_width_cfou[c]*c2;
            grad_height_array[r*column+c]=grad_height_cfou[c]*c2;
        }
    }
    for(int c=0;c<column;c++) {
        for(int r=0;r<row;r++) {
            FT::Complex r1(cos(2*M_PI/row*(-r*row/2+row*row/4)),sin(2*M_PI/row*(-r*row/2+row*row/4)));
            h_rfou[r]=surface[r*column+c]*r1;
            delta_width_rfou[r]=delta_width_array[r*column+c]*r1;
            delta_height_rfou[r]=delta_height_array[r*column+c]*r1;
            grad_width_rfou[r]=grad_width_array[r*column+c]*r1;
            grad_height_rfou[r]=grad_height_array[r*column+c]*r1;
        }
        FT::fast_recur(h_rfou,row,1);
        FT::fast_recur(delta_width_rfou,row,1);
        FT::fast_recur(delta_height_rfou,row,1);
        FT::fast_recur(grad_width_rfou,row,1);
        FT::fast_recur(grad_height_rfou,row,1);
        for(int r=0;r<row;r++) {
            FT::Complex r2(cos(2*M_PI/row*-r*row/2),sin(2*M_PI/row*-r*row/2));
            surface[r*column+c]=h_rfou[r]*r2;
            delta_width_array[r*column+c]=delta_width_rfou[r]*r2;
            delta_height_array[r*column+c]=delta_height_rfou[r]*r2;
            grad_width_array[r*column+c]=grad_width_rfou[r]*r2;
            grad_height_array[r*column+c]=grad_height_rfou[r]*r2;
//            qDebug() << "debug36" << surface[r*column+c].re << surface[r*column+c].im;
//            qDebug() << "debug39" << delta_width_array[r*column+c].re << delta_width_array[r*column+c].im
//                     << delta_height_array[r*column+c].re << delta_height_array[r*column+c].im
//                     << grad_width_array[r*column+c].re << grad_width_array[r*column+c].im
//                     << grad_height_array[r*column+c].re << grad_height_array[r*column+c].im;
        }
    }
    time+=0.002;
//    time+=(clock()-lasttick)*1.0/1000.0;
//    lasttick=clock();
    delete [] h_rfou;delete [] h_cfou;
    delete [] delta_width_rfou;delete [] delta_width_cfou;
    delete [] grad_width_rfou;delete [] grad_width_cfou;
}

FT::Complex PhillipSpectrum::calculate_h0(double kx, double kz)
{
    FT::Complex res;
    double ph=0.0;
    double k_len=sqrt(kx*kx+kz*kz);
    if(k_len<=0.0001) {
        return res;
    }
//    k_len=std::max(0.001,k_len);
    std::default_random_engine random;
    std::normal_distribution<double> n_dist(0.0,1.0);
    double windlen=sqrt(windx*windx+windz*windz);
    FT::Complex h0;
    double l=windlen*windlen/gravity,damping=0.001;
    ph=5.0*exp(-1.0/(k_len*l*k_len*l))*(kx*windx+kz*windz)/k_len/windlen*(kx*windx+kz*windz)/k_len/windlen/(k_len*k_len*k_len*k_len)/**exp(-k_len*k_len*l*l*damping*damping)*/;
    FT::Complex ksi;
    ksi.re=n_dist(random);
    ksi.im=n_dist(random);
    res=ksi*1.0/sqrt(2)*sqrt(ph);

    return res;
}

FT::Complex PhillipSpectrum::calculate_h(double kx, double kz, double t)
{
    FT::Complex res,c1,c2,h1,h2;
    double omega=0;
    double k_len=sqrt(kx*kx+kz*kz);
//    double w_0=2.0*M_PI/200;
//    omega=floor(sqrt(gravity*k_len)/w_0)*w_0;
    omega=sqrt(gravity*k_len);
    c1.re=cos( omega*t);c1.im=sin( omega*t);
    c2.re=cos(-omega*t);c2.im=sin(-omega*t);
    h1=calculate_h0( kx, kz);
    h2=calculate_h0(-kx,-kz);
    h2.im=-h2.im;
    res=h1*c1+h2*c2;
    return res;
}

PhillipSpectrum::~PhillipSpectrum()
{
    delete [] surface;
}
