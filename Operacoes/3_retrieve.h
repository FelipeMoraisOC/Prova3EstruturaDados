#ifndef RETRIEVE_H
#define RETRIEVE_H

void* retrieve  (pDFile arq, void* dados, FuncaoComparacao pfc){
    
    if(arq->arquivo == NULL){
      printf("Arquivo nao foi aberto no Retrieve!\n");
      return NULL;
    }

   rewind(arq->arquivo);
   int result;

   do{
      void *buffer = malloc(arq->tamanhoRegistro);
      if(buffer == NULL){
            printf("Erro ao alocar memoria no Retrieve\n");
            return NULL;
      }

      result = fread(buffer, arq->tamanhoRegistro, 1, arq->arquivo);
      if(result == 0){
         free(buffer);
         continue;
      }

      if(pfc(buffer, dados)){
         return buffer;
      }

      free(buffer);
   }while(result != 0);

   return NULL;
}


#endif
