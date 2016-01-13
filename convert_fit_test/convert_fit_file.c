#include <stdio.h>
#include <stdlib.h>

int convert_fit_file(char *ptr_file_name)
{

//   char buffer[30];
//   unsigned int buffer;
   unsigned char buffer; // 1 byte
   int bufferInt;
   FILE *ptr_myfile;
   int ii;

   unsigned char *bufferArray;   
   bufferArray = malloc(sizeof(unsigned char));
    
   // Open the file
   // hard code for now
   ptr_myfile = fopen("jan11-trainer.fit","rb");

   // Test if file is present
   if (!ptr_myfile)
   {
      printf("*** Cannot open file. *** \n");
   }
   // Read the file 
   // format is: (ptr to the memory in which to store, 
   //             size in bytes of each element to read,
   //             number of elements each with size of the previous size,
   //             pointer to a FILE that is the input stream)
   fread(&buffer,1,1,ptr_myfile);

   printf("*** Here is the first buffer: one byte, unsigned char ***\n");
   printf("%u\n",buffer);

   /// now we know the size of the header. we should allocate an array of unsigned chars 
   // (each unsigned char is 1 byte (= 8 bits) that can hold the remainder of the items 
   // in the header
   bufferInt = buffer;
   printf("Here is the buffer as an int: %d\n",bufferInt);
   
   // Decrement buffer (we already read the first byte as the size of the header)
   bufferInt--;
   //
   bufferArray = realloc(bufferArray,bufferInt*sizeof(unsigned char));
   
   fread(bufferArray,sizeof(unsigned char), bufferInt, ptr_myfile);    

   printf(" 0 byte of the header: %u\n",buffer);
   for(ii = 0;ii < bufferInt;ii++)
   {
      printf("%2d byte of the header: %u\n",ii+1,bufferArray[ii]);
   }

   return(0);
} 
