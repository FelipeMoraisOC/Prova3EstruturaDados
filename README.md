# Este trabalho prático engloba dois tópicos distintos:
1) A implementaçao de uma biblioteca de presistência de dados em arquivo binário em
linguagem C.
2) A implementação de um sistema com 3 entidades relacionadas, como por exemplo, Livro,
Autor e Editora. O sistema deve usar a biblioteca de persistência para gravar e recuperar
as informações gravadas em disco.

## 1) Biblioteca de Persistência
Este tópico do trabalho refere-se à implementação da biblioteca de persistência de dados, cujo objetivo é centralizar as operações mais comuns de gravação de dados em arquivos, tais como o CRUD(Create, Read, Update, Delete)

Cada Arquivo será representado por um descritor com a seguinte estrutura:

```c
struct dFile{
  FILE* arquivo;         // nome do arquivo em disco
  int   tamanhoRegistro; // qtde bytes do tipo de dado (struct)
};
```

Os tipos de dados e as operações da bilbioteca de persistência estão descritos no quadro a seguir: 

```c
/* --------------------------
   Tipos de dados
   -------------------------- */
typedef struct dFile DFile;
typedef DFile*       pDFile;

typedef int   (*FuncaoComparacao)(void *, void *);
typedef void* (*FuncaoAloca)     ();
typedef void  (*FuncaoImpressao) (void *);
typedef int   (*FuncaoPredicado) (void *);

/* --------------------------
   Opera��es CRUD
   -------------------------- */
pDFile  openn      (char[30], int);
void    createe   (pDFile, void*);                          // dados
void*   retrieve  (pDFile, void*, FuncaoComparacao);        // chave
void    updatee   (pDFile, void*, void*, FuncaoComparacao/*, FuncaoAloca*/); // chave e os dados
void    delete    (pDFile, void*, FuncaoComparacao);        // chave
void    closee     (pDFile);

pDLista queryAll  (pDFile, FuncaoAloca);
pDLista queryBy   (pDFile, FuncaoPredicado);
void   persistAll (pDFile, pDLista);
```

## 2) Sistema com 3 entidades relacionadas
O segundo tópico deste trabalho trata da implementação de um sistema contendo, no mínimo 3 entidades relacionadas, conforme o exemplo de diagrama ilustrado na figura a seguir:

![image](https://github.com/user-attachments/assets/71ffe62b-b899-4d03-8271-cda4bd091b73)
