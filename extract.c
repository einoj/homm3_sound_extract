#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  int num_files;
  char **filenames;
  char *names;
  int *sizes;
  FILE *fp;
} snd;


int extract_files(snd *s)
{
  char *file;
  FILE *out;
  char tmp_n[13]; //13 because we need to add .wav to the end of filenames and the largest file name is 11
  int i;
  for (i = 0; i < s->num_files; i++) {
    file = malloc(s->sizes[i]*sizeof(char));
    fread(file,sizeof(char),s->sizes[i],s->fp);
    sprintf(tmp_n,"%s.wav",s->filenames[i]);
    out = fopen(tmp_n,"wb");
    fwrite(file,sizeof(char),s->sizes[i],out);
    free(file);
    fclose(out);
  }
}


int main(int argc, char *argv[])
{

  snd *s;
  s->fp = fopen(argv[1], "rb");
  printf("rw\n");
  int num_files;
  char *name;
  int temp[8];
  int start;
  int end;
  char padding[32];
  
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
    printf("%s\n",s->filenames[i]);
  }

  extract_files(s);
  
  free(s->sizes);
  free(s->names);
  free(s->filenames);
  fclose(s->fp);

  return 0;
}
