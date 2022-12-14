#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int *files_available = NULL; // array with the id of the files available
int *global_votes = NULL;

int n = 0; // number of files
int t = 0; // number of threads
int c = 0; // number of candidates

int number_of_files_read = 0;
pthread_mutex_t *mymutexs = NULL;
pthread_mutex_t mutex;

typedef struct file_info {
  int len;
  int *votes;
} file_info;

int decide_file_available() {
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
  // printf("arquivo livre %d\n", file);
  return file;
}

file_info readFile(int id) {
  // printf("lendo arquivo data/%d.in\n", id);
  char file_name[100];
  sprintf(file_name, "data2/%d.in", id);

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
  // printf("arquivo data/%d.in lido\n", id);
  return info;
};

void *count_votes(void *threadid) {
  int file = decide_file_available();
  while (file != -1) {
    file_info info = readFile(file);
    int i = 0;
    for (; i < info.len; i++) {
      pthread_mutex_lock(&mymutexs[info.votes[i]]);
      global_votes[info.votes[i]] = global_votes[info.votes[i]] + 1;
      pthread_mutex_unlock(&mymutexs[info.votes[i]]);
    }
    file = decide_file_available();
  }
};

int findMaxIndex() {
  int i = 1;
  int max = 0;
  int max_index = 0;
  for (; i <= c; i++) {
    if (global_votes[i] > max) {
      max = global_votes[i];
      max_index = i;
    }
  }
  return max_index;
}
int find_total_votes() {
  int i = 0;
  int total = 0;
  for (; i <= c; i++) {
    total += global_votes[i];
  }
  return total;
}
int main() {
  printf("insira o numero de arquivos: ");
  scanf("%d", &n);

  printf("insira o numero de threads: ");
  scanf("%d", &t);

  printf("insira o numero de candidatos: ");
  scanf("%d", &c);

  free(mymutexs);
  free(global_votes);
  free(files_available);

  files_available = (int *)realloc(files_available, (n + 1) * sizeof(int));

  int i = 1;

  for (; i <= n; i++) {
    // printf("arquivo %d disponivel\n", i);
    files_available[i] = i;
  }

  // printf("arquivos disponiveis\n");

  global_votes = (int *)malloc((c + 1) * sizeof(int));
  for (i = 0; i < c + 1; i++) {
    global_votes[i] = 0;
  }

  // printf("votos zerados\n");

  mymutexs =
      (pthread_mutex_t *)realloc(mymutexs, ((c + 1) * sizeof(pthread_mutex_t)));

  // printf("mutexs criados\n");

  for (i = 0; i < c + 1; i++) {
    pthread_mutex_init(&mymutexs[i], NULL);
  }

  // printf("criei o array mutex\n");

  pthread_t *threads = (pthread_t *)malloc(t * sizeof(pthread_t));
  for (i = 0; i < t; i++) {
    pthread_create(&threads[i], NULL, count_votes, NULL);
  }

  for (i = 0; i < t; i++) {
    pthread_join(threads[i], NULL);
  }

  int total_votes = find_total_votes();
  int index = findMaxIndex();

  printf("votos totais: %d\n", total_votes);
  printf("Votos nulos: %.2f%%(%d)\n",
         (float)global_votes[0] / total_votes * 100.0, global_votes[0]);
  for (i = 1; i < c + 1; i++) {
    printf("candidato %d com %.2f%%(%d)dos votos\n", i,
           (float)global_votes[i] / total_votes * 100, global_votes[i]);
  }

  printf("candidato vencedor: %d\n", index);
}