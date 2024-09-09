#ifndef QUERY_H
#define QUERY_H

pDLista queryBy  (pDFile arq, FuncaoPredicado pred){
   if(arq->arquivo == NULL){
      printf("Arquivo nao foi aberto na Query!\n");
      return NULL;
   }

   rewind(arq->arquivo);
   int result;

   do{
      void *dados = malloc(arq->tamanhoRegistro);
      if(dados == NULL){
            printf("Erro ao alocar memoria na Query\n");
            return NULL;
      }

      result = fread(dados, arq->tamanhoRegistro, 1, arq->arquivo);
      if(result == 0){
         free(dados);
         continue;
      }

      if(pred(dados)){
         return dados;
      }
   }while(result != 0);

   return NULL;
}

#endif

