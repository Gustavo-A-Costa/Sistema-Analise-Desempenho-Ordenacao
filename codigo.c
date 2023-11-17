#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>


int readTextFile(const char *filename, int **array) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Print erro %s\n", filename);
        return 0;
    }

    int capacity = 10;
    int size = 0;
    *array = (int *)malloc(capacity * sizeof(int));

    int num;
    while (fscanf(file, "%d", &num) == 1) {
        if (size == capacity) {
            capacity *= 2;
            *array = (int *)realloc(*array, capacity * sizeof(int));
        }
        (*array)[size] = num;
        size++;
    }

    fclose(file);
    return size;
}

// geral troca de item
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

//Quick Sort
//int inc = 0;
void quickSort(float *array, int left, int right) {
    if (left < right) {
        float pivot = array[right];
        int i = left - 1;

        for (int j = left; j < right; j++) {
            if (array[j] < pivot) {
                i++;
                // printf("%d, %d", array[i], array[j]);
                swap(&array[i], &array[j]);
            }
        }

        swap(&array[i + 1], &array[right]);
        float partitionIndex = i + 1;

        //inc = inc + 1;
        //printf("%d /n", inc);
        quickSort(array, left, partitionIndex - 1);
        quickSort(array, partitionIndex + 1, right);
    }
}

//Heap Sort
void heapify(int *array, int size, int root) {
    int largest = root;
    int left = 2 * root + 1;
    int right = 2 * root + 2;

    if (left < size && array[left] > array[largest])
        largest = left;

    if (right < size && array[right] > array[largest])
        largest = right;

    if (largest != root) {
        swap(&array[root], &array[largest]);
        heapify(array, size, largest);
    }
}

void heapSort(int *array, int size) {
    for (int i = size / 2 - 1; i >= 0; i--)
        heapify(array, size, i);

    for (int i = size - 1; i >= 0; i--) {
        swap(&array[0], &array[i]);
        heapify(array, i, 0);
    }
}

void merge(int *array, int left, int middle, int right) {
    int n1 = middle - left + 1;
    int n2 = right - middle;

    int *leftArray = (int *)malloc(n1 * sizeof(int));
    int *rightArray = (int *)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++)
        leftArray[i] = array[left + i];
    for (int i = 0; i < n2; i++)
        rightArray[i] = array[middle + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArray[i] <= rightArray[j]) {
            array[k] = leftArray[i];
            i++;
        } else {
            array[k] = rightArray[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        array[k] = leftArray[i];
        i++;
        k++;
    }

    while (j < n2) {
        array[k] = rightArray[j];
        j++;
        k++;
    }

    free(leftArray);
    free(rightArray);
}

// Merge Sort
void mergeSort(int *array, int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;
        mergeSort(array, left, middle);
        mergeSort(array, middle + 1, right);
        merge(array, left, middle, right);
    }
}

int main() {
    FILE *reportFile = fopen("relatorio.txt", "w");

    if (reportFile == NULL) {
        printf("Erro ao criar o arquivo de relatório\n");
        return 1;
    }
    fprintf(reportFile, "MÉTODO, TEMPO DE EXECUÇÃO (us), TAMANHO VETOR, ARQUIVO\n");

    // Diretório da pasta 1
    system("dir /b pasta1 > files.txt");

    // Abre o arquivo que lista os arquivos
    FILE *filesFile = fopen("files.txt", "r");
    if (filesFile == NULL) {
        printf("Erro ao abrir o arquivo com a lista de arquivos\n");
        return 1;
    }

    char filename[100];
    while (fscanf(filesFile, "%s", filename) == 1) {
        // caminho completo do arquivo
        char filepath[100];
        sprintf(filepath, "pasta1\\%s", filename);

        // armazena no vetor
        int *array;
        int size = readTextFile(filepath, &array);

        if (size == 0) {
            continue;
        }

        // Tempo Quick Sort
        struct timeval start, end;
        ///gettimeofday(&start, NULL);
        //printf("Iniciando Quick Sort");
        ///quickSort(array, 0, size - 1);
        //printf("Quick Sort Ok");
        ///gettimeofday(&end, NULL);
        ///long quickSortTime = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

        // Tempo Heap Sort
        gettimeofday(&start, NULL);
        heapSort(array, size);
        gettimeofday(&end, NULL);
        long heapSortTime = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
        //printf("Heap Sort Ok");

        int *arrayCopy = (int *)malloc(size * sizeof(int));
        memcpy(arrayCopy, array, size * sizeof(int));

        // Tempo Merge Sort
        gettimeofday(&start, NULL);
        mergeSort(arrayCopy, 0, size - 1);
        gettimeofday(&end, NULL);
        long mergeSortTime = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
        //printf("Merge Sort Ok");

        // Resultado no arquivo
        ///fprintf(reportFile, "Quick Sort, %ld us, %d, %s\n", quickSortTime, size, filename);
        fprintf(reportFile, "Heap Sort, %ld us, %d, %s\n", heapSortTime, size, filename);
        fprintf(reportFile, "Merge Sort, %ld us, %d, %s\n", mergeSortTime, size, filename);

        free(array);
        free(arrayCopy);
    }

    fclose(reportFile);
    fclose(filesFile);
    remove("files.txt");

    printf("Relatório gerado com sucesso no arquivo 'relatorio.txt'\n");
    return 0;
}
