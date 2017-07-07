#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "complex.h"
#include "dft.h"

int strcmp(char *str1, char *str2){

    register int i;
    
    for(i=0; str1[i]!='\0' || str2[i]!='\0'; i++){
        if(str1[i]!=str2[i])
            return 0;
    }
    
    return 1;
}

void strcp(char *str1, char *str2){
     
     register int i;
     
     for(i=0; str1[i]!='\0'; i++)
         str2[i]=str1[i];
     str2[i]='\0';
}

int main(int argc, char *argv[]){
    
    if( argc == 1){
        fprintf(stderr, "\nA Leosoft C program for dft calculations.\n    use 'dft --help' for instructions\n\n");
        return 0;
    }

    if( strcmp(argv[1], "--help") ){
        fprintf(stderr, "\n    dft: A Leosoft C program for dft calculations.\n\n");
        fprintf(stderr, "    The DFT of <inputfile> will be calculated and printed on stdout.\n");
        fprintf(stderr, "    <inputfile> should be a ASCII format <x,y> pairs separated by space \n");
        fprintf(stderr, "    with each pair in different a line. Output will be in the same format\n");
        fprintf(stderr, "    but with <x,Re(y),Im(y)> trios instead.\n");
        fprintf(stderr, "    OPTIONS:\n");
        fprintf(stderr, "        <inputfile>: name of input file;\n");
        fprintf(stderr, "        -s: outputs the |H| instead of the complex H;\n");
        fprintf(stderr, "        -g: makes and executes a script to plot <inputfile> and |H|\n");
        fprintf(stderr, "            on GMT. In this case an temp output file 'tmp.dft' will created;\n\n");


        return 0;
    }

    register ulint i;
    short gmt=0, input=0, mod=0;
    complex *f, *H, *h, *t;    
    ulint N, max=10000;
    double delta;
    FILE *inh, *outH, *plot;
    char ofile[]="tmp.dft", ifile[100];


    for(i=1; i<argc; i++){
        
        if( strcmp(argv[i], "-g") )
            gmt=1;

        if( strcmp(argv[i], "-s") )
            mod=1;
    
        if( argv[i][0] != '-' ){
            strcp(argv[i], ifile);
            input=1;
        }
    }
    
    if( input==0 ){
        fprintf(stderr,"\nError! The input filename must be given!\n    use 'dft --help' for instructions\n");
        return 0;
    }

        
    if( (inh=fopen(ifile, "r"))== NULL ){
        fprintf(stderr, "\nError opening input file '%s'!\n", ifile);
        return 0;
    }

    if(gmt){    
        if( (outH=fopen(ofile, "w"))== NULL ){
            fprintf(stderr, "\nError making output file '%s'!\n", ofile);
            return 0;
        }
    }
            
    /* Read the input file */
    h=(complex *)malloc(sizeof(complex)*max);
    t=(complex *)malloc(sizeof(complex)*max);
    for(N=0; !feof(inh); N++){
        /* if N exceeds max, alloc more memory */
        if( N>=max ){
            max*=2;
            h=(complex *)realloc(h, sizeof(complex)*max);
            t=(complex *)realloc(t, sizeof(complex)*max);
        }
        fscanf(inh, "%lf %lf", &t[N].Re, &h[N].Re);
        t[N].Im=0;
        h[N].Im=0;
    }
    /* Use only what's needed */
    h=(complex *)realloc(h, sizeof(complex)*N);
    t=(complex *)realloc(t, sizeof(complex)*N);
    
    fclose(inh);

    delta=t[1].Re-t[0].Re;

    /* Make the transform */
    H=dft(h, N);
    
    /* Determine the frequencies */
    f=(complex *)malloc(sizeof(complex)*N);
    for(i=0; i<N; i++){
        f[i].Re=(double)i/(N*delta);
        f[i].Im=0;
    }

                 
    if( gmt ){    
    
        if( (plot=fopen("dftplot.sh", "w"))== NULL ){
            printf("\nError creating plot script 'dftplot.sh'!\n");
            return 0;
        }
        
        /* Imprime em tmp.dft o |H| */
        for(i=N/2; i<N; i++){        
            fprintf(outH, "%lf %lf\n", -1*f[N-i].Re, cmod(H[i]));        
        }
        for(i=0; i<=N/2; i++){        
            fprintf(outH, "%lf %lf\n", f[i].Re, cmod(H[i]));        
        }
    
        /* Plota o espectro de frequencia */
        fprintf(plot, "#!/bin/sh\n");  
        fprintf(plot, "gmtset BASEMAP_TYPE plain HEADER_FONT_SIZE 22p PAPER_MEDIA a4 X_AXIS_LENGTH 25c\n");
        fprintf(plot, "psbasemap `minmax -I0.001 %s` -B:.\"Amplitude Specter\":WeSn -JXh -K  > spec.ps\n", ofile);
        fprintf(plot, "psxy %s -R -JXh -W1p,0/0/0 -O  >> spec.ps\n", ofile);
        /* Plota a funcao */
        fprintf(plot, "psbasemap `minmax -I0.001 %s` -B:.\"Function\":WeSn -JXh -K  > func.ps\n", ifile);
        fprintf(plot, "psxy %s -R -JXh -W1p,0/0/0 -O  >> func.ps\n", ifile);  
        fprintf(plot, "gv spec.ps &\ngv func.ps &\n");
           
        fclose(plot);
        fclose(outH);
            
        system("chmod +x dftplot.sh");
        system("./dftplot.sh");        
    }

    if( mod ){
        for(i=0; i<N; i++){        
            printf("%lf %lf\n", f[i].Re, cmod(H[i]));        
        }
    }
    else{
        for(i=0; i<N; i++){        
            printf("%lf %lf %lf\n", f[i].Re, H[i].Re, H[i].Im);        
        }
    }

    if( gmt )
        system("rm tmp.dft");

    return 0;
}
