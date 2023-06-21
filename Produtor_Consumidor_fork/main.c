#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
void create_lock_file() {
        FILE* lock_file = fopen("buffer.lock.txt", "w");
        fclose(lock_file);
    }
int file_exists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    return 0;
}

void remove_file(const char* filename) {
    if (remove(filename) != 0) {
        printf("Falha ao remover o arquivo %s.\n", filename);
    } 
}
void insert_number(const char* filename, int number) {
    FILE* file = fopen(filename, "a");
    if (file != NULL) {
        fprintf(file, "%d\n", number);
        fclose(file);
        printf("Produtor [%d].\n", number);
    } 
}

void lerNumeroDoBuffer(const char* nomeArquivo) {
    // Abrir o arquivo buffer para leitura
    FILE* file = fopen(nomeArquivo, "r");

    // Ler o primeiro número do arquivo
    int numero;
    fscanf(file, "%d", &numero);

    // Imprimir o número na tela
    printf("Consumirdor [%d]\n",numero);

    // Criar um arquivo temporário
    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Erro ao criar o arquivo temporário\n");
        fclose(file);
        return;
    }

    // Copiar os números restantes para o arquivo temporário
    int valor;
    while (fscanf(file, "%d", &valor) == 1) {
        fprintf(tempFile, "%d\n", valor);
    }

    // Fechar os arquivos
    fclose(file);
    fclose(tempFile);

    // Remover o arquivo original e renomear o arquivo temporário
    remove(nomeArquivo);
    rename("temp.txt", nomeArquivo);
}


int main(){
    int a = 0;
    int pid = fork();
    while(1){
        if(pid == 0){
            if(file_exists("buffer.lock.txt") == 0){
                create_lock_file();
                insert_number("buffer.txt",1 + rand() % 100);
                remove_file("buffer.lock.txt");
                int s = 1 + rand() % 3;
                sleep(s);
            }
        }
        else{
            create_lock_file();
            //printf("a = %d\n", a);
            lerNumeroDoBuffer("buffer.txt");
            remove_file("buffer.lock.txt");
            int s = 1 + rand() % 3;
            sleep(s);
        }
    }
}