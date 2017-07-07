/* Functions for calculating dft */
#ifndef _DFT_H_
#define _DFT_H_

#ifndef _COMPLEX_H_
#include "complex.h"
#endif

#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif

/* HEADERS */
complex *dft(complex *h, ulint N);
complex *idft(complex *H, ulint N);

/* FUNCTIONS */
/**********************************************************************************************/

/* DFT: h is the input vector of lenth N */
complex *dft(complex *h, ulint N){
        
        complex *H, aux;
        register ulint n, k;
                
        H=makecm(N, 1);
        cm_zero(H, N, 1);
        
        for(n=0; n<N; n++){
            for(k=0; k<N; k++){
            	aux.Re=cos((double)(2*PI*k*n)/N);
            	aux.Im=sin((double)(2*PI*k*n)/N);
            	H[n]=cadd(H[n], cmult(h[k], aux));
            }
        }
        
        return H;
}

complex *idft(complex *H, ulint N){
        
        complex *h, aux;
        register ulint n, k;
                
        h=makecm(N, 1);
        cm_zero(h, N, 1);
        
        for(k=0; k<N; k++){
            for(n=0; n<N; n++){
            	aux.Re=(double)cos((double)(2*PI*k*n)/N)/N;
            	aux.Im=(double)-1*sin((double)(2*PI*k*n)/N)/N;
            	h[k]=cadd(h[k], cmult(H[n], aux));
           }
       }
        
        return h;
}
#endif
