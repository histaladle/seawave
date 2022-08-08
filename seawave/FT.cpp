#include "FT.h"
#include <QDebug>
namespace FT
{
void fast_iterate(Complex *fourier,int len,int type)
{
    if(len==1) {
        return;
    }
    int layer=left_mask_bit(len);
    if(len!=(1<<layer)) return;
    int *rev_bit_order=new int[len]{0};
    rader(rev_bit_order,len);
    Complex wi,w;
    Complex *fourier1=new Complex[len];
    Complex *fourier2=new Complex[len];
    for(int i=0;i<len;i++) {
//        qDebug() << "debug89" << i << rev_bit_order[i];
        fourier1[i]=fourier[rev_bit_order[i]];
    }
    for(int i=1;i<=layer;i++) {
        if(i==1) {
            for(int j=0;j<len;j+=2) {
                discrete(fourier1+j,2,1);
            }
            continue;
        }
        int ftlen=1<<(i-1);
        w=Complex(cos(2.0*M_PI/ftlen),type*sin(2.0*M_PI/ftlen));
        wi=Complex(1.0,0.0);
        for(int j=0;j<len;j+=2*ftlen) {
            for(int k=0;k<ftlen;k++) {
                wi=Complex(cos(k*2.0*M_PI/(2*ftlen)),type*sin(k*2.0*M_PI/(2*ftlen)));
                fourier2[j+k]      =fourier1[j+k]+wi*fourier1[j+k+ftlen];
                wi=Complex(cos((k+ftlen)*2.0*M_PI/(2*ftlen)),type*sin((k+ftlen)*2.0*M_PI/(2*ftlen)));
                fourier2[j+k+ftlen]=fourier1[j+k]+wi*fourier1[j+k+ftlen];
            }
        }
        for(int j=0;j<len;j++) {
            fourier1[j]=fourier2[j];
        }
    }
//    for(int mid=1;mid<len;mid*=2) {
//        Complex w=Complex(cos(M_PI/mid),type*sin(M_PI/mid));
//        for(int i=0;i<len;i+=mid*2) {
//            wi=Complex(1,0);
//            for(int j=0;j<mid;j++,wi=wi*w) {
//                Complex x=fourier1[i+j],y=wi*fourier1[i+j+mid];
//                fourier1[i+j]=x+y;
//                fourier1[i+j+mid]=x-y;
//            }
//        }
//    }
    for(int i=0;i<len;i++) {
        fourier[i]=fourier1[i];
    }
    delete [] rev_bit_order;
    delete [] fourier1;
    delete [] fourier2;
}

void fast_recur(Complex *fourier,int len,int type)
{
    if(len==1) {
        return;
    }
    int maskbit=left_mask_bit(len);
    if(len!=1<<maskbit) return;
    Complex *fou1=new Complex[len/2];
    Complex *fou2=new Complex[len/2];
//    for(int i=0;i<len/2;i++) {
//        fou1[i]=fourier[2*i];
//        fou2[i]=fourier[2*i+1];
//    }
    for(int i=0;i<len;i+=2) {
        fou1[i/2]=fourier[i];
        fou2[i/2]=fourier[i+1];
    }
    fast_recur(fou1,len/2,type);
    fast_recur(fou2,len/2,type);
    Complex wn(cos(2.0*M_PI/len),type*sin(2.0*M_PI/len));
    Complex wi(1.0,0.0);
    for(int i=0;i<len/2;i++) {
        fourier[i]=fou1[i]+wi*fou2[i];
        fourier[len/2+i]=fou1[i]-wi*fou2[i];
        wi=wi*wn;
    }
    delete [] fou1;
    delete [] fou2;
}

void rader(int *order,int len)
{
    int leftbit=left_mask_bit(len);
    if(leftbit<0) return;
    int totallen=(1<<leftbit);
    if(totallen!=len) return;
    int next_bitrev=0;
    for(int i=0;i<totallen;i++) {
        order[i]=i;
    }
    for(int i=0;i<totallen-1;i++) {
        order[i]=next_bitrev;
        int mask=totallen>>1;
        /*
         * if next_bitrev<mask:
         *  next_bitrev: 0 x x x
         *         mask: 1 0 0 0
        */
        while(next_bitrev>=mask) {
            next_bitrev=next_bitrev-mask;
            mask=mask>>1;
        }
        next_bitrev=next_bitrev+mask;
    }
}

int left_mask_bit(int n)
{
    if(n<0) {
        return -1;
    }
    if(n==0) {
        return -1;
    }
    if(n>1<<30) {
        return -1;
    }
    int leftbit=0;
    for(int i=0;i<30;i++) {
        if(1<<i>=n) {
            leftbit=i;
            break;
        }
    }
    return leftbit;
}

void discrete(Complex *fourier,int len,int type)
{
    Complex *fourier1=new Complex[len];
    Complex wi;
    for(int k=0;k<len;k++) {
        for(int n=0;n<len;n++) {
            wi=Complex(cos(k*n*2.0*M_PI/len),type*sin(k*n*2.0*M_PI/len));
            fourier1[k]=fourier1[k]+fourier[n]*wi;
        }
    }
    for(int i=0;i<len;i++) {
        fourier[i]=fourier1[i];
    }
    delete [] fourier1;
}

}

