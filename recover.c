/**
 * recover.c
*skku learning center
 *
 * Recovers JPEGs from a forensic image.
 */

#include <stdio.h>

//Main with parameters/arguments
int main(int argc, char* argv[])
//by convention argv[0] is name by which program is invoked so argc is 
atleast 1.
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover card.raw\n");
        return 1;
    }
    
    char* infile = argv[1];
    //
    
    // size of the block that will be read and written
    const int BLOCK_SIZE = 512;
    //const is a keyword. More specifically, const is a type qualifier. Type qualifiers are part of C types. 
    //In the type int const, const is a type qualifier, and int is a type specifier. Here, const qualifies the type int.
    
    // open input file 
    FILE* inptr = fopen(infile, "r");
    //"r"- read from file
    //The fopen() function opens the file in a particular mode
    // FILE *fopen(const char* name,const char* mode);
    //inptr and outptr are similar to infile and outfile, are arbitrary variable names
    //They follow a certain pattern and are consistent, that makes the code easier to follow.
    //inptr corresponds to the file the program is reading from
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    
    unsigned char buffer[BLOCK_SIZE];
    
    // pointer to outfile
    FILE* outptr = NULL;
    // outptr corresponds to the file your program is writing to
    
    // make space for jpg file name
    char image[7];
    
    // number of image files created
    int n = 0;

    // search until jpg is found
    while(fread(buffer, BLOCK_SIZE, 1, inptr) == 1)
        //The fread() function reads up to count items of size length from the input stream 
        //stores them in the given buffer. 
        //The position in the file increases by the number of bytes read.
    {
        // find the beginning of an jpg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            buffer[3] >= 0xe0 && buffer[3] <= 0xef)
        {
            // close image file if one has been created
            if (n > 0)
            {
                fclose(outptr);
            }
            // make name for nth image
            sprintf(image, "%03d.jpg", n);
            //"string print", a file handling function that is used to send formatted output to the string.
            //Instead of printing on console, sprintf() function stores the output on char buffer that is specified in sprintf.

            
            // open nth image file
            outptr = fopen(image, "w");
            //"w"- write, starting at the beginning of the file
            if (outptr == NULL)
                //outprt==NULL is evluated to true. Hence, you are getting output putptr.
            {
                fprintf(stderr, "Could not create %s.\n", image);
                //format print function. 
                //writes and formats the output to a stream. print the message but not on stdout console.
                //standard error message, to print the output on the screen or windows terminal
                //Standard error is another output stream typically used by programs to output error messages. 
                //It is a stream independent of standard output and can be redirected separately.
                return 3;
            }
            
            // adds on the number it is created
            n++;
        }
        
        // write to image file only if one exists, detects when it is right or wrong
        if (outptr != NULL)
        {
            // write to image file
            fwrite(buffer, BLOCK_SIZE, 1, outptr);
        }
    }
    
    // outputs close
    fclose(outptr);
    
    // inputs close
    fclose(inptr);
    
    // return 0 to prevent explosion
    return 0;
}
