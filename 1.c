#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int *files_available; // array with the id of the files available
int *global_votes;

int n; // number of files
int t; // number of threads
int c; // number of candidates
int number_of_files_read = 0;
pthread_mutex_t *mymutexs; // array of mutexs
pthread_mutex_t mutex;
typedef struct file_info {
  int len;
  int *votes;
} file_info;

// here we will return the top of the array and remove it from the array
int decite_file_available() {
  printf("verificando arquivo livre\n");
  pthread_mutex_lock(&mutex);

  int i = 1;
  int file = -1;

  for (; i <= n; i++) {
    if (files_available[i] != -1) {
      file = files_available[i];
      files_available[i] = -1;
      break;
    }
  }

  pthread_mutex_unlock(&mutex);
  printf("arquivo livre %d\n", file);
  return file;
}
// the read file recieves an id and the name of each file is data/id.in
file_info readFile(int id) {
  printf("lendo arquivo data/%d.in\n", id);
  char file_name[100];
  sprintf(file_name, "data/%d.in", id);

  FILE *file = fopen(file_name, "r");

  if (file == NULL) {
    printf("Error opening file %d!\n", id);
    exit(1);
  }
  int len;
  fscanf(file, "%d", &len);
  int *votes = malloc(len * sizeof(int));
  int i = 0;
  for (; i < len; i++) {
    fscanf(file, "%d", &votes[i]);
  }
  fclose(file);
  file_info info;
  info.len = len;
  info.votes = votes;
  printf("arquivo data/%d.in lido\n", id);
  return info;
};

void *count_votes(void *threadid) {
  // printf("Thread %d started", (int)&threadid);
  int file = decite_file_available();
  while (file != -1) {
    file_info info = readFile(file);
    int i = 0;
    // printf("contando votos do arquivo %d\n", info.len);
    for (; i < info.len; i++) {
      // printf("info vote: %d do arquivo: %d, da thread: %d\n", info.votes[i],
      //        file, (int)threadid);
      pthread_mutex_lock(&mymutexs[info.votes[i]]);
      global_votes[info.votes[i]] = global_votes[info.votes[i]] + 1;
      pthread_mutex_unlock(&mymutexs[info.votes[i]]);
    }
    file = decite_file_available();
  }
};

int main() {
  printf("insira o numero de arquivos: ");
  scanf("%d", &n);

  printf("insira o numero de threads: ");
  scanf("%d", &t);

  printf("insira o numero de candidatos: ");
  scanf("%d", &c);

  files_available = (int *)malloc(n * sizeof(int));
  int i = 1;
  for (; i <= n; i++) {
    files_available[i] = i;
  }

  mymutexs = (pthread_mutex_t *)malloc((c + 1) * sizeof(pthread_mutex_t));
  for (i = 0; i < c + 1; i++) {
    pthread_mutex_init(&mymutexs[i], NULL);
  }
  printf("criei o array mutex\n");

  global_votes = (int *)malloc(c * sizeof(int));
  for (i = 0; i < c + 1; i++) {
    global_votes[i] = 0;
  }

  pthread_t *threads = (pthread_t *)malloc(t * sizeof(pthread_t));
  for (i = 0; i < t; i++) {
    pthread_create(&threads[i], NULL, count_votes, NULL);
  }

  for (i = 0; i < t; i++) {
    pthread_join(threads[i], NULL);
  }

  for (i = 0; i < c + 1; i++) {
    printf("candidate: %d with %d votes\n", i, global_votes[i]);
  }
}