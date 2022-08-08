#ifndef FT_H
#define FT_H

#include <cmath>
#include <iostream>
namespace FT {

extern int left_mask_bit(int n);
extern void rader(int *order,int len);

class Complex
{
public:
    Complex(double re=0,double im=0) : re(re),im(im)
    {}
    double mod()
    {
        return sqrt(re*re+im*im);
    }
    Complex operator+(const Complex &comp)
    {
        Complex comp1;
        comp1.re=re+comp.re;
        comp1.im=im+comp.im;
        return comp1;
    }
    Complex operator-(const Complex &comp)
    {
        Complex comp1;
        comp1.re=re-comp.re;
        comp1.im=im-comp.im;
        return comp1;
    }
    Complex operator*(const double x)
    {
        Complex comp1;
        comp1.re=re*x;
        comp1.im=re*x;
        return comp1;
    }
    Complex operator*(const Complex &comp)
    {
        Complex comp1;
        comp1.re=re*comp.re-im*comp.im;
        comp1.im=re*comp.im+im*comp.re;
        return comp1;
    }
    Complex operator/(const double x)
    {
        Complex comp1;
        comp1.re=re/x;
        comp1.im=im/x;
        return comp1;
    }
    Complex operator/(const Complex &comp)
    {
        Complex comp1(re,im);
        Complex comp2(comp.re,-comp.im);
        comp1=comp1*comp2/comp2.mod();
        return comp1;
    }
    double arg() //rad
    {
        double a=0.0;
        if(re==0.0) {
            return M_PI/2;
        }
        a=atan(im/re);
        return a;
    }
    double re;
    double im;
};

extern void fast_recur(Complex *fourier,int len,int type);
extern void fast_iterate(Complex *fourier, int len, int type);
extern void discrete(Complex *fourier,int len,int type);
}
#endif // FT_H
