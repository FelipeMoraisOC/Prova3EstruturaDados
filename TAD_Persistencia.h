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




