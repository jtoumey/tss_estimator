#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*************************************************************************/
/*                                                                       */
/*  File:         get_input.c                                            */
/*                                                                       */
/*  Date:         November 2015                                          */
/*                                                                       */
/*  Description:  Read in user inputs                                    */
/*                                                                       */
/*  Inputs:       ptr_file_name  memory address of input file name       */
/*                ptr_lthr       memory address of threshold HR          */
/*                                                                       */
/*************************************************************************/

void get_input(char *ptr_file_name, float *ptr_lthr, int argc, char **argv)
{
   int r;
   int lthr_flag;
   char is_string_buffer[30];
   char *ptr_conv;
   char *extension_test;

   //
   // If the user supplied no cmd line args, prompt them for both
   if(argc < 2)
   {
      printf("\nEnter file name : ");
      fgets(ptr_file_name, 30, stdin);
      ptr_file_name[strlen(ptr_file_name) - 1] = '\0';

      printf("\nEnter LTHR [bpm]:");
      scanf("%f", ptr_lthr);
   }

   // The user supplied one argument. Find out if it containsi anything other than
   // digits or decimals
   else if(argc == 2)
   {
      if(strspn(argv[1], "0123456789.") == strlen(argv[1]))
      {
         printf("string only numbers\n");
         lthr_flag = 1;
      }      
      else
      {
         printf("string has characters that are not numbers\n ");
         lthr_flag = 0;
      }
      if(lthr_flag == 0)
      {
         printf("\nEnter LTHR [bpm]:");
         scanf("%f", ptr_lthr);
         //
         strcpy(ptr_file_name, argv[1]);
      }
      else
      {
         printf("\nEnter file name : ");
         fgets(ptr_file_name, 255, stdin);
         ptr_file_name[strlen(ptr_file_name) - 1] = '\0';
         //
         *ptr_lthr = strtol(argv[1], &ptr_conv, 10);
      }
   }

   // The user supplied both arguments. Find out which is which
   else if(argc == 3)
   {
      lthr_flag = 0;
      if(strspn(argv[1], "0123456789.") == strlen(argv[1]))
      {
         printf("string only numbers\n");
         lthr_flag = 1;
      }      
      if(lthr_flag == 0)
      {
         strcpy(ptr_file_name, argv[1]);
         *ptr_lthr = strtol(argv[2], &ptr_conv, 10);
      }
      if(lthr_flag == 1)
      {
         *ptr_lthr = strtol(argv[1], &ptr_conv, 10);
         strcpy(ptr_file_name, argv[2]);
      }
   } 
   printf("File name: %s\n", ptr_file_name);
   printf("LTHR     : %f\n", *ptr_lthr);

   //
   // Input file validation
   //

   // test if the file exists in the working directory
   FILE *file = fopen(ptr_file_name, "r");
   if(file == 0) 
   {
      // If the file pointer is null, terminate the program
      printf("File not found. Terminating...\n");
      exit(EXIT_SUCCESS);
   }

   // test if the lthr is *.gpx
   extension_test = strrchr(ptr_file_name, '.');
   printf("Extension: %s\n",extension_test);
   if(strcmp(extension_test,".gpx") != 0)
   {
      printf("File does not end in .gpx. Please rename.\n");
      printf("Terminating...\n");
      exit(EXIT_SUCCESS);
   }

   printf("input file name before exiting get_input: %s\n",ptr_file_name);

}
