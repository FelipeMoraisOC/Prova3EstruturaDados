#ifndef QUERYALL_H
#define QUERYALL_H

pDLista queryAll(pDFile arq, FuncaoAloca pfa){

   if (arq->arquivo == NULL){
       printf("Arquivo n�o foi aberto no QueryAll!\n");
       return NULL;
   }

   pDLista lista = criarLista();

   rewind(arq->arquivo);
   int result;

   do{
      void *dados = pfa();
      result = fread (dados, arq->tamanhoRegistro, 1, arq->arquivo);
      if (result == 0)
        continue;
      //pfi(dados);
      incluirInfo(lista, dados);
   } while(result!=0);

   return lista;
}

#endif
