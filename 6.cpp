#include <cstdio>
#include <map>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <stdlib.h>
#include <iostream>

int NUM_THREADS = 4;

typedef struct {
  int index;
  float value;
}
instance;

typedef struct {
  std::vector <instance> instances;
  int len;
  int id;
}
line;

float resultado[4][4];
float resultado_v_denso[4][1];
float resultado_m_denso[4][4];

std::vector <line> matriz1;
std::vector <line> matriz2;
std::vector <std::vector <int>> vetor_denso;
std::vector <std::vector <float>> matriz_densa;

int linhas = 4;
int colunas = 4;

std::map <int, int> get_row(int index, std::vector <line> matriz) {
  int i = 0;
  std::map <int, int> row;

  for (; i < matriz[index].len; i++) {
    row[matriz[index].instances[i].index] = matriz[index].instances[i].value;
  }

  return row;
}

int get_value(int i, int j, std::vector <line> matrix) {

  std::map < int, int > row = get_row(i, matrix);

  if (row.find(j) != row.end()) {
    return row[j];
  }

  return 0;
}

void * multiplica_matriz_esparsa_x_esparsa(void * tid) {
  int threadId = ( * (int * ) tid);

  int i, j, k;
  for (i = threadId; i < linhas; i = i + NUM_THREADS) {

    for (j = 0; j < colunas; j++) {
      resultado[i][j] = 0;

      for (k = 0; k < colunas; k++) {
        resultado[i][j] = resultado[i][j] +
          get_value(i, k, matriz1) * get_value(k, j, matriz2);
      }
    }
  }
}

void * multiplica_matriz_esparsa_x_v_denso(void * tid) {
  int threadId = ( * (int * ) tid);

  int i, j, k;
  for (i = threadId; i < linhas; i = i + NUM_THREADS) {

    for (j = 0; j < 1; j++) {
      resultado_v_denso[i][j] = 0;

      for (k = 0; k < colunas; k++) {
        resultado_v_denso[i][j] = resultado_v_denso[i][j] +
          get_value(i, k, matriz1) * vetor_denso[k][0];
      }
    }
  }
}
void * multiplica_matriz_esparsa_x_m_densa(void * tid) {
  int threadId = ( * (int * ) tid);

  int i, j, k;
  for (i = threadId; i < linhas; i = i + NUM_THREADS) {

    for (j = 0; j < colunas; j++) {
      resultado_m_denso[i][j] = 0;

      for (k = 0; k < colunas; k++) {
        resultado_m_denso[i][j] = resultado_m_denso[i][j] +
          get_value(i, k, matriz2) * matriz_densa[k][j];
      }
    }
  }
}

int main() {
  int i, j, k;
  int linhas = 4;
  int colunas = 4;

  /*
    2.0 -1.0 0.0 0.0
    -1.0 2.0 -1.0 0.0
    0.0 -1.0 2.0 -1.0
    0.0 0.0 -1.0 2.0
  */
  // Criando a primeira matriz esparsa
  for (i = 0; i < linhas; i++) {
    line l;
    l.len = 0;
    l.id = 1;
    for (j = 0; j < colunas; j++) {
      if (i == 0) {
        if (j == 0) {
          instance inst;
          inst.index = j;
          inst.value = 2.0;
          l.instances.push_back(inst);
          l.len++;
        } else if (j == 1) {
          instance inst;
          inst.index = 1;
          inst.value = -1.0;
          l.instances.push_back(inst);
          l.len++;
        } else {
          break;
        }
      } else if (i == 1) {
        if (j == 0) {
          instance inst;
          inst.index = 0;
          inst.value = -1.0;
          l.instances.push_back(inst);
          l.len++;
        } else if (j == 1) {
          instance inst;
          inst.index = 1;
          inst.value = 2.0;
          l.instances.push_back(inst);
          l.len++;
        } else if (j == 2) {
          instance inst;
          inst.index = 2;
          inst.value = -1.0;
          l.instances.push_back(inst);
          l.len++;
        } else {
          break;
        }
      } else if (i == 2) {
        if (j == 1) {
          instance inst;
          inst.index = 1;
          inst.value = -1.0;
          l.instances.push_back(inst);
          l.len++;
        } else if (j == 2) {
          instance inst;
          inst.index = 2;
          inst.value = 2.0;
          l.instances.push_back(inst);
          l.len++;
        } else if (j == 3) {
          instance inst;
          inst.index = 3;
          inst.value = -1.0;
          l.instances.push_back(inst);
          l.len++;
        } else {
          continue;
        }
      } else if (i == 3) {
        if (j == 2) {
          instance inst;
          inst.index = 2;
          inst.value = -1.0;
          l.instances.push_back(inst);
          l.len++;
        } else if (j == 3) {
          instance inst;
          inst.index = 3;
          inst.value = 2.0;
          l.instances.push_back(inst);
          l.len++;
        } else {
          continue;;
        }
      }
    }
    matriz1.push_back(l);
  }

  // Criando a segunda matriz esparsa
  /*
    4.0 -10.0 0.0 0.0
    -1.0 5.0 0.0 0.0
    0.0 -6.0 0.0 -9.0
    0.0 0.0 -2.0 0.0
  */
  for (i = 0; i < linhas; i++) {
    line l;
    l.len = 0;
    l.id = 2;
    for (j = 0; j < colunas; j++) {
      if (i == 0) {
        if (j == 0) {
          instance inst;
          inst.index = j;
          inst.value = 4.0;
          l.instances.push_back(inst);
          l.len++;
        } else if (j == 1) {
          instance inst;
          inst.index = 1;
          inst.value = -10.0;
          l.instances.push_back(inst);
          l.len++;
        } else {
          break;
        }
      } else if (i == 1) {
        if (j == 0) {
          instance inst;
          inst.index = 0;
          inst.value = -1.0;
          l.instances.push_back(inst);
          l.len++;
        } else if (j == 1) {
          instance inst;
          inst.index = 1;
          inst.value = 5.0;
          l.instances.push_back(inst);
          l.len++;
        } else {
          break;
        }
      } else if (i == 2) {
        if (j == 1) {
          instance inst;
          inst.index = 1;
          inst.value = -6.0;
          l.instances.push_back(inst);
          l.len++;
        } else if (j == 3) {
          instance inst;
          inst.index = 3;
          inst.value = -9.0;
          l.instances.push_back(inst);
          l.len++;
        } else {
          continue;
        }
      } else if (i == 3) {
        if (j == 2) {
          instance inst;
          inst.index = 2;
          inst.value = -2.0;
          l.instances.push_back(inst);
          l.len++;
        } else {
          continue;;
        }
      }
    }
    matriz2.push_back(l);
  }

  // Criando vetor denso.
  /*
  [[4],
   [9],
   [6],
   [7]]
  */
  for (i = 0; i < linhas; i++) {
    std::vector <int> aux;
    for (j = 0; j < 1; j++) {
      if (i == 0) {
        aux.push_back(4);
      } else if (i == 1) {
        aux.push_back(9);
      } else if (i == 2) {
        aux.push_back(6);
      } else if (i == 3) {
        aux.push_back(7);
      }
    }
    vetor_denso.push_back(aux);
  }

  // criando matriz densa
  /*
    2.0 -7.0 9.0 -21.0
    -1.0 9.5 2.0 4.0
    3.8 -6.0 6.0 -1.7
    22.0 13.7 -2.5 -8.9
  */
  for (i = 0; i < linhas; i++) {
    std::vector <float> aux;
    for (j = 0; j < colunas; j++) {
      if (i == 0) {
        if (j == 0) {
          aux.push_back(2.0);
        } else if (j == 1) {
          aux.push_back(-7.0);
        } else if (j == 2) {
          aux.push_back(9.0);
        } else if (j == 3) {
          aux.push_back(-21.0);
        }
      } else if (i == 1) {
        if (j == 0) {
          aux.push_back(-1.0);
        } else if (j == 1) {
          aux.push_back(9.5);
        } else if (j == 2) {
          aux.push_back(2.0);
        } else if (j == 3) {
          aux.push_back(4.0);
        }
      } else if (i == 2) {
        if (j == 0) {
          aux.push_back(3.8);
        } else if (j == 1) {
          aux.push_back(-6.0);
        } else if (j == 2) {
          aux.push_back(6.0);
        } else if (j == 3) {
          aux.push_back(-1.7);
        }
      } else if (i == 3) {
        if (j == 0) {
          aux.push_back(22.0);
        } else if (j == 1) {
          aux.push_back(13.7);
        } else if (j == 2) {
          aux.push_back(-2.5);
        } else if (j == 3) {
          aux.push_back(-8.9);
        }
      }
    }
    matriz_densa.push_back(aux);
  }

  // print matriz_densa
  for (i = 0; i < linhas; i++) {
    for (j = 0; j < colunas; j++) {
      std::cout << matriz_densa[i][j] << " ";
    }
    std::cout << std::endl;
  }

  pthread_t threads_esparsas[NUM_THREADS];
  int threadId_esparsa[NUM_THREADS];

  for (i = 0; i < NUM_THREADS; i++) {
    threadId_esparsa[i] = i;
    pthread_create( & threads_esparsas[i], NULL, multiplica_matriz_esparsa_x_esparsa, (void * ) & threadId_esparsa[i]);
  }

  for (i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads_esparsas[i], NULL);
  }
  printf("Resultado da multiplicação das matrizes esparsas\n");
  for (i = 0; i < linhas; i++) {
    for (j = 0; j < colunas; j++) {
      printf("%f ", resultado[i][j]);
    }
    printf("\n");
  }

  pthread_t threads_v_densor[NUM_THREADS];
  int threadId_v_denso[NUM_THREADS];

  for (i = 0; i < NUM_THREADS; i++) {
    threadId_v_denso[i] = i;
    pthread_create( & threads_v_densor[i], NULL, multiplica_matriz_esparsa_x_v_denso, (void * ) & threadId_v_denso[i]);
  }

  for (i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads_v_densor[i], NULL);
  }
  printf("Resultado da multiplicação de matriz esparsa vs vetor denso\n");
  for (i = 0; i < linhas; i++) {
    for (j = 0; j < 1; j++) {
      printf("%f ", resultado_v_denso[i][j]);
    }
    printf("\n");
  }

  pthread_t threads_m_densor[NUM_THREADS];
  int threadId_m_denso[NUM_THREADS];

  for (i = 0; i < NUM_THREADS; i++) {
    threadId_m_denso[i] = i;
    pthread_create( & threads_m_densor[i], NULL, multiplica_matriz_esparsa_x_m_densa, (void * ) & threadId_m_denso[i]);
  }

  for (i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads_m_densor[i], NULL);
  }

  printf("Resultado da multiplicação de matriz esparsa vs matriz denso\n");
  for (i = 0; i < linhas; i++) {
    for (j = 0; j < colunas; j++) {
      printf("%f ", resultado_m_denso[i][j]);
    }
    printf("\n");
  }
}