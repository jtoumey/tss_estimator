#include <stdio.h>
#include <stdlib.h>

int convert_fit_file(char *ptr_file_name)
{

   unsigned char buffer; // 1 byte to store header length
   int bufferInt;
   FILE *ptr_myfile;
   int ii;

   
   unsigned char recordHeaderBuffer;
   unsigned char recordHeaderMSB;
   int recordHeader;

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
   //             number of elements to read (each with size of the previous size),
   //             pointer to a FILE that is the input stream)
   fread(&buffer,1,1,ptr_myfile);

   printf("*** Here is the first buffer: one byte, unsigned char ***\n");
   printf("%u\n",buffer);

   /// now we know the size of the header. we should allocate an array of unsigned chars 
   // (each unsigned char is 1 byte (= 8 bits) that can hold the remainder of the items 
   // in the header
   bufferInt = buffer;
   
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

   // Read the record header (1 byte)
   fread(&recordHeaderBuffer,1,1,ptr_myfile);
   // examine bits of this byte (jesus christ)
   // http://stackoverflow.com/questions/1682996/bytes-to-binary-in-c
   // 
   unsigned char mask = 1;
   unsigned char bits[8];
   for(ii = 0;ii < 8;ii++)
   {
      bits[ii] = (recordHeaderBuffer & (mask << ii)) != 0;    
   }
   // debug
   for(ii = 0;ii < 8;ii++)
   {
      printf("%2d bit: %d\n",ii,bits[ii]);
   }

   //recordHeaderMSB = recordHeaderBuffer 



   recordHeader = recordHeaderBuffer;
   
   printf("*** Here is the first record header buffer: one byte, unsigned char ***\n");
   printf("%u\n",recordHeaderBuffer);
   printf("*** Here is the first record header buffer, cast as integer ***\n");
   printf("%2d\n",recordHeader);
 

   return(0);
} 
