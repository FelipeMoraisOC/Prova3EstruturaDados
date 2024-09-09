#ifndef UPDATE_H
#define UPDATE_H

void updatee(pDFile arq, void* chave, void* dados, FuncaoComparacao pfc/*, FuncaoAloca pfa*/){
    void *buffer = malloc(arq->tamanhoRegistro);

    if(buffer == NULL){
        printf("Erro ao alocar memória no update\n");
        return;
    }

    rewind(arq->arquivo);

    while(fread(buffer, arq->tamanhoRegistro, 1, arq->arquivo)){
        if(pfc(buffer, chave) == 1){
            fseek(arq->arquivo, -arq->tamanhoRegistro, SEEK_CUR);
            printf("%d - Registro(s) autalizado(s)!\n", fwrite(dados, arq->tamanhoRegistro, 1, arq->arquivo));
            
            break;
        }
    }

    free(buffer);
}

#endif
