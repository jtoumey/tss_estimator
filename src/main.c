#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 200000
void get_input(char *ptr_file_name, float *ptr_lthr, int argc, char **argv);
void calc_hr_zones(float *ptr_lthr, float *zone_array);
void file_process(float *raw_data, char *ptr_file_name);
char parse_file_file(char *ptr_file_name, int *raw_time, int *raw_hr);
void classify_heartrate(int *raw_time, int *raw_hr, float *zone_array, int *zone_bin);
void calc_stress(int *zone_bin, float *total_stress);

/*************************************************************************/
/*                                                                       */
/*  File:         main.c                                                 */
/*                                                                       */
/*  Date:         November 2015                                          */
/*                                                                       */
/*  Description:  Main driver file                                       */
/*                                                                       */
/*************************************************************************/

int main(int argc, char *argv[])
{
   // Variable declaration
   int ii;
   char file_name[30] = { '\0' };
   float lthr;
   float zone_array [6];
   int raw_time [SIZE] = {0};
   int raw_hr [SIZE] = {0};
   int x,y;
   int zone_bin[7] = {0};
   float total_stress = 0.;
   //
   // GET INPUT: LTHR, desired *.fit file name
   //
   get_input(file_name, &lthr, argc, argv);
   // echo your input   
   printf("\nFile name   : %s\n", file_name);
//   printf("\nFile length : %zu",sizeof(file_name));   
//   printf("\nYour lthr is: %f BPM",lthr);

   //
   // CALCULATE HR ZONES: hr zones from Joe Friel
   // 
   //lthr = 145.0; // [bpm], hard-coded for testing 
   calc_hr_zones(&lthr, zone_array);
/*   for (ii = 0;ii < 6;ii++)
   {
      printf("\n%f\n",zone_array[ii]);
   }*/
   //
   // DECODE *.FIT FILE: using "fitSDK"
   //
   //decode_fit(&file_name);

   //
   // PARSE XML FILE: look at the HRs in the 'decoded' *.fit file
   //
//   file_process(&raw_data[1000][2], &file_name[30]);

   parse_file_line(file_name, &raw_time, &raw_hr);
   //for(x=0; x<10; x++)
   //{
   //   printf("raw_data[%i]: %i %i\n", x, raw_time[x], raw_hr[x]);
      
   //}


   //
   // SORT HR: figure out time spent in each zone
   //
   classify_heartrate(raw_time, raw_hr, zone_array, zone_bin);
   // ** test print ** 
  /* for(ii = 0; ii < 7;ii++)
   {
      printf("%i sec in zone %f \n",zone_bin[ii], zone_array[ii]);
   }*/

   //
   // CALCULATE STRESS: using formula from excel sheet
   //
   calc_stress(zone_bin, &total_stress);
   printf("Total stress [ndim]: %f\n",total_stress);   
//
   // WRITE RESULTS: to screen, or file 
   //
  
  printf("\n");  //clean up the end of prog

   return 0;
}
