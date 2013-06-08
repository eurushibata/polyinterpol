#include <stdio.h>
#include <stdlib.h>

// protótipo da função
int aprox(float pto, float mat[100][2], int total);

int main(void) {
    FILE *fp;
    int total, i, j, apr;
    float mat[100][2], pto = -1,f,r;
    float x0,x1,x2,x3,xaux;
    float o00,o01,o02,o03,o0aux;
    float o10,o11,o12,o1aux;
    float o20,o21,o2aux;
    float o30,o3aux;
    float o4aux;
    
    // Abre o arquivo
    fp = fopen("dadoscap.txt", "r");
    
    // Verifica se o arquivo dadoscap.txt existe
    if (fp == NULL) {
        printf("Erro: Não foi possível encontrar o arquivo \'dadoscap.txt\'\n");
        exit(0);
    } else {
        // Leitura da primeira linha (total de pontos coletados)
        fscanf(fp, "%d", &total);
        i = 0;
        // Executar a iteração até que os termos do arquivo dadoscap.txt acabem
        // Armazena os valores de t em mat[i][0] e V(t) em mat[i][1]
        while (!feof(fp)) {
            // Leitura das colunas do arquivo
            fscanf(fp, "%f %f", &mat[i][0], &mat[i][1]);
            i++;
        }
    }

    // Fecha o arquivo
    fclose(fp);

    // Leitura do ponto de aproximação
    while (pto<0||pto>155) {
        printf("\nEntre com um ponto de aproximação no intervalo [0,155]: ");
        scanf("%f", &pto);
        if (pto<0||pto>155)
            printf("Erro: O valor digitado está fora do intervalo [0, 155].");
    }

    // Chamada para a função que retorna o valor aproximado entre os índices 0 e 30.
    // O retorno é um valor de 0 a 28, pois o grau 3, necessita de 3 valores
    // pondendo ser os índices (0,1,2,3), (1,2,3,4), ..., (28,29,30,31).
    apr = aprox(pto, mat, total);

    // Cálculo da Diferença Dividida
    // x
    x0 = mat[apr][0];
    x1 = mat[apr+1][0];
    x2 = mat[apr+2][0];
    x3 = mat[apr+3][0];

    // ordem 0
    o00 = mat[apr][1];
    o01 = mat[apr+1][1];
    o02 = mat[apr+2][1];
    o03 = mat[apr+3][1];

    // ordem 1
    o10 = (o01-o00)/(x1-x0);
    o11 = (o02-o01)/(x2-x1);
    o12 = (o03-o02)/(x3-x2);

    // ordem 2
    o20 = (o11-o10)/(x2-x0);
    o21 = (o12-o11)/(x3-x1);

    // ordem 3
    o30 = (o21-o20)/(x3-x0);

    // f aproximado
    f = o00 + (pto-x0)*o10 + (pto-x0)*(pto-x1)*o20 + (pto-x0)*(pto-x1)*(pto-x2)*o30;
    
    // Cálculo da aproximação de erro
    if (apr==28) {
    // Caso tenha que adicionar um valor para baixo 
        xaux = mat[apr-1][0];
        o0aux = mat[apr-1][1];
        o1aux = (o00-o0aux)/(x0-xaux);
        o2aux = (o10-o1aux)/(x1-xaux);
        o3aux = (o20-o2aux)/(x2-xaux);
        o4aux = (o30-o3aux)/(x3-xaux);
    } else {
    // Caso tenha que adicionar um valor para cima
        xaux = mat[apr+4][0];
        o0aux = mat[apr+4][1];
        o1aux = (o0aux-o03)/(xaux-x3);
        o2aux = (o1aux-o12)/(xaux-x2);
        o3aux = (o2aux-o21)/(xaux-x1);
        o4aux = (o3aux-o30)/(xaux-x0);
    }

    // Erro aproximado
    r = (pto-xaux)*(pto-x0)*(pto-x1)*(pto-x2)*(pto-x3)*o4aux;

    // Impressão do resultado
    printf("\nValor: %.2f\nAproximação Solicitada:%f\nAproximação do erro: %f\n", pto, f, r);

    return 0;
}

// Função que retorna o índice do números aproximados de pto
int aprox(float pto, float mat[100][2], int total) {
    int i;
    for (i=0; i<total-2; i++) {
        if (pto>=mat[i][0] && pto<mat[i+1][0]) {
          return i;
        }
    }
    // caso nenhum valor for encontrado entre os indices 0 e 27, atribui-se 28.
    return 28;
}

