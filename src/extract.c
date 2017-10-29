/* Author Eino J. O.
 *
 * Copyright (c) 2016 Eino J. O. 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to 
 * deal in the Software without restriction, including without limitation the 
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

typedef struct
{
  int num_files;
  char **filenames;
  char *names;
  int *sizes;
  FILE *fp;
} snd;


/* Allocates memory for the pointers in the SND struct then reads the file pointed to by struct snd->fp. 
 * The first 4 bytes contains the number of files stored in the file. Next follows repeatedly number of files time,
 * 8 bytes or less for the .wav file name, 32 padding bytes, 4 bytes giving an offset 
 * telling where the .wav file start, and 4 bytes telling the size of the .wav file.
 */
static int read_file_info(snd *s)
{
  int temp[8];
  int start;
  fread(&s->num_files,sizeof(int),1,s->fp);
  s->filenames = malloc(s->num_files*sizeof(char*)); // allocate array of pointers that point to strings
  s->names = malloc(9*s->num_files*sizeof(char)); // allocate memory for #num_files strings of size 9, maximum name length plus one zero byte, this allocates all memory in a row
  s->sizes = malloc(s->num_files*sizeof(int)); // allocate memory for storing filesizes
  int i;
  /* Read filenames and file sizes */ 
  for (i = 0; i < s->num_files; i++) {
    s->filenames[i] = &(s->names[i*9]);
    fread(s->filenames[i],sizeof(char),8,s->fp);
    s->filenames[i][8] = 0; // remember to set the last bit of a string to zero
    fread(temp,sizeof(int),8,s->fp);
    fread(&start,sizeof(int),1,s->fp);
    fread(&s->sizes[i],sizeof(int),1,s->fp);
    //printf("%s\n",s->filenames[i]); //print file list
  }
  return 0;
}

/* Reads the files in the file pointed to by snd->fp.
 * By this point all the header data of the .snd should be read by read_file_info
 * Therefore if we start reading files of the sizes given by snd->sizes and write the files
 * to disk, all the files contrained in the .snd will be extracted.
 * It will cause undefined behaviour if called before read_file_info().
 */
static int extract_files(snd *s)
{
  char *file;
  FILE *out;
  char tmp_n[13]; //13 because we need to add .wav to the end of filenames and the largest file name is 11
  int i;
  struct stat st = {0};
  if (stat("./snd", &st) == -1) {
      mkdir("./snd", 0755);
  }
  for (i = 0; i < s->num_files; i++) {
    file = malloc(s->sizes[i]*sizeof(char));
    fread(file,sizeof(char),s->sizes[i],s->fp);
    sprintf(tmp_n,"snd/%s.wav",s->filenames[i]);
    out = fopen(tmp_n,"wb");
    fwrite(file,sizeof(char),s->sizes[i],out);
    free(file);
    fclose(out);
  }
  return 0;
}

static int free_struct(snd *s)
{
  free(s->sizes);
  free(s->names);
  free(s->filenames);
  return 0;
}

int main(int argc, char *argv[])
{

  if (argc < 2) {
    fprintf(stderr,"Usage: extract Heroes3.snd \n");
    exit(1);
  }

  snd s;
  s.fp = fopen(argv[1], "rb");
    
  read_file_info(&s);

  extract_files(&s);
 
  free_struct(&s); 

  fclose(s.fp);

  return 0;
}
