#ifndef DELETE_H
#define DELETE_H

void delete(pDFile arq, void* chave, FuncaoComparacao pfc){
    void *dados = malloc(arq->tamanhoRegistro);

    int cont = 0;
    long posicaoLeitura = 0,  posicaoEscrita = 0; ;

    if(dados == NULL){
        printf("Erro ao alocar memoria!\n");
        return;
    }

    rewind(arq->arquivo);

    while(fread(dados, arq->tamanhoRegistro, 1, arq->arquivo)){
        posicaoLeitura = ftell(arq->arquivo);
        if(!pfc(dados, chave)){ 
            fseek(arq->arquivo, posicaoEscrita, SEEK_SET);
            fwrite(dados, arq->tamanhoRegistro, 1, arq->arquivo);
            posicaoEscrita = ftell(arq->arquivo);
        } else{
            cont++;
        }

        fseek(arq->arquivo, posicaoLeitura, SEEK_SET);
    }

    
    ftruncate(fileno(arq->arquivo), posicaoEscrita);
    free(dados);
    printf("%d - Registros excluídos com sucesso\n", cont);
}


#endif
