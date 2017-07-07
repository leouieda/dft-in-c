/* Functions for complex operations */
#ifndef _COMPLEX_H_
#define _COMPLEX_H_

#ifndef _STDLIB_H_
#include <stdlib.h>
#endif

/* Macro that returns the index of position ij in a matrix with ncol */
#ifndef POS_X__Y__COLS_
#define POS(x, y, cols) (((x)*(cols))+(y))
#endif

/* Complex number struct */
typedef struct complex_num {
        double Re, Im;
}complex;

typedef unsigned long int ulint;

/* HEADERS */
complex cmult(complex z1, complex z2);
complex cadd(complex z1, complex z2);
complex csub(complex z1, complex z2);
double cmod(complex z);  
complex *cm_mult(complex *m1, ulint nl1, ulint nc1, complex *m2, ulint nl2, ulint nc2);
void cm_zero(complex *m, ulint nl, ulint nc);
complex *makecm(ulint nl, ulint nc);
        
        
/* FUNCTIONS */
/**********************************************************************************************/
complex cmult(complex z1, complex z2){
        
        complex res;
        
        res.Re = (z1.Re*z2.Re)-(z1.Im*z2.Im);
        res.Im = (z2.Re*z1.Im)+(z1.Re*z2.Im);
        
        return res;
}

complex cadd(complex z1, complex z2){
        
        complex res;
        
        res.Re = z1.Re + z2.Re;
        res.Im = z1.Im + z2.Im;
        
        return res;
}

complex csub(complex z1, complex z2){
        
        complex res;
        
        res.Re = z1.Re - z2.Re;
        res.Im = z1.Im - z2.Im;
        
        return res;
}

double cmod(complex z){        
        return sqrt((z.Re*z.Re)+(z.Im*z.Im));
}

complex *makecm(ulint nl, ulint nc){
        return (complex *)malloc(sizeof(complex)*nl*nc);
}

complex *cm_mult(complex *m1, ulint nl1, ulint nc1, complex *m2, ulint nl2, ulint nc2){
       
       register ulint i, j, k;
       complex *res;
       
       /* Can't multiply two matrixes like this */
       if( nc1!=nl2 )
           return 0; /* in case of error */
       
       /* Alloc the memory for the answer */    
       res = (complex *)malloc(sizeof(complex)*nl1*nc2);       
       if(!res)
           return 0;   /* in case of error */
       
       cm_zero(res, nl1, nc2); /* Inicialize */
       
       for(i=0; i<nl1; i++){
           for(j=0; j<nc2; j++){
               for(k=0; k<nc1; k++){                  
                   res[POS(i, j, nc2)]=cadd(res[POS(i, j, nc2)], cmult(m1[POS(i, k, nc1)],m2[POS(k, j, nc2)]));
               }
           }
       }
       
       return res;
}

void cm_zero(complex *m, ulint nl, ulint nc){
     
     register ulint i, j;
     
     for(i=0; i<nl; i++){
         for(j=0; j<nc; j++){
             m[POS(i, j, nc)].Re=0;
             m[POS(i, j, nc)].Im=0;
         }
     }
}


#endif
