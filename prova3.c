#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>  // Necessário para ftruncate
#include <fcntl.h>   // Necessário para fileno
#include "Persistencia.h"
#include "Cap3/Lista.h"
#include "Utils.h"


struct Editora{
    int id;
    char nome[30];
    char cidade[30];
};
struct Livro {
    int isbn;
    char titulo[30];
    int ano_publicacao;
    int id_autor;
    int id_editora;
};

struct Autor {
    int id;
    char nome[30];
};

int comparaLivro(void* a, void* b){
    struct Livro *livro1 = (struct Livro *)a;
    struct Livro *livro2 = (struct Livro *)b;

    if(livro1->isbn == livro2->isbn){
        return 1;
    } else {
        return 0;
    }
}


//Inicio Funcoes Autor
int comparaAutor(void* a,  void* b){
   struct Autor *autor1 = (struct Autor *)a;
   struct Autor *autor2 = (struct Autor *)b;

    if(autor1->id == autor2->id){
        return 1;
    } else {
        return 0;
    }
}

int comparaFKLivroAutor(void* l, void* a){
    struct Livro * livro = (struct Livro *)l;
    struct Autor * autor = (struct Autor *)a;
    if(livro->id_autor == autor->id){
        return 1;
    } else {
        return 0;
    }
}

void imprimeAutor(void *info){
    struct Autor *a = (struct Autor *) info;
    printf("Id: %d - Nome: %s\n", a->id, a->nome); 
}

void* alocaAutor(){
   return malloc(sizeof(struct Autor));
}

void criarAutor(){
    struct Autor autor, ultimoAutor;
    pDFile arq;
    int id = 0;

    arq = openn("Autor.dat", sizeof(struct Autor));

    if(arq == NULL) {
        printf("Não foi possível abrir o arquivo Autor.dat\n");
        return;
    }

    //Recebendo o id do ultimo autor
    fseek(arq->arquivo, -(int)sizeof(struct Autor), SEEK_END);
    int result = fread(&ultimoAutor, sizeof(struct Autor), 1, arq->arquivo);

    if(result == 0){
        id = 1;
    } else{
        id = 1 + ultimoAutor.id;
    }


    printf("--Criar Autor-- \n\n");
    printf("Digite o nome do Autor: ");
    fgets(autor.nome, 30, stdin);
    fflush(stdin);

    autor.id = id;

    createe(arq, &autor);
    closee(arq);
}

void editarAutor(){
    struct Autor autor;
    pDFile arq;
    int id = 0;
    FuncaoComparacao compara = comparaAutor;
    

    arq = openn("Autor.dat", sizeof(struct Autor));

    printf("--Editar Autor--\n");

    printf("Digite o Id do autor que deseja alterar: ");
    scanf("%d", &id);
    fflush(stdin);
    autor.id = id;

    printf("Digite o nome para alterar: ");
    fgets(autor.nome, 30, stdin);
    fflush(stdin);

    updatee(arq, &id, &autor, compara); 
    closee(arq);
}

void exibirListaAutores(){
    struct Autor autor;
    pDFile arq;

    arq = openn("Autor.dat", sizeof(struct Autor));

    printf("--Lista de autores--\n");
    
    pDLista listaAutores = queryAll(arq, alocaAutor);
    for(int i = 1; i <= listaAutores->quantidade; i++){
        struct Autor* a = (struct Autor*)buscarInfoPos(listaAutores, i);
        imprimeAutor(a);
    }

    closee(arq);
}

void excluirAutor(){
    struct Autor autor;
    pDFile arq, arqLivro;
    int id = 0;
    FuncaoComparacao compara = comparaAutor;

    arq = openn("Autor.dat", sizeof(struct Autor));
    arqLivro = openn("Livro.dat", sizeof(struct Livro));

    if(arq == NULL) {
        printf("Não foi possível abrir o arquivo Autor.dat\n");
        return;
    } else if(arqLivro == NULL){
        printf("Não foi possível abrir o arquivo Livro.dat\n");
        return;
    }

    printf("--Excluir Autor--\n\n");
    printf("Digite o ID do Autor que deseja excluir: ");
    scanf("%d", &id);
    fflush(stdin);

    autor.id = id;

    //Delete Cascade Livro
    struct Autor * autorVerf = (struct Autor *) retrieve(arq, &autor, comparaAutor);

    FuncaoComparacao comparaFkLivroAut = comparaFKLivroAutor;
    //Comparar as FKs
    delete(arqLivro, autorVerf, comparaFkLivroAut);
    delete(arq, &autor, compara);

    closee(arq);
    closee(arqLivro);
}

void menuAutor(){
    int op = 1;
    while(op != 0){
        printf("--Menu do Autor (CRUD)--\n\n");
        printf("Selecione a opcao desejada: \n");
        printf("1 - Criar novo autor\n");
        printf("2 - Editar autor\n");
        printf("3 - Exibir lista de autores\n");
        printf("4 - Excluir autor\n");
        printf("0 - Sair\n");

        printf("Digite a opcao desejada: ");
        scanf("%d", &op);
        fflush(stdin);

        switch (op)
        {
        case 1:
            criarAutor();
            break;
        case 2:
            editarAutor();
            break;
        case 3:
            exibirListaAutores();
            break;
        case 4: 
            excluirAutor();
            break;
        default:
            break;
        }
    }
   

}
//Fim Funcoes Autor

//Inicio Funcoes Editora
void* alocaEditora(){
    return malloc(sizeof(struct Editora));
}

void imprimeEditora(void* info){
    struct Editora *e = (struct Editora *) info;
    printf("Id: %d - Nome: %s - Cidade: %s\n", e->id, e->nome, e->cidade);
}

int comparaEditora(void* a,  void* b){
   struct Editora *editora1 = (struct Editora *)a;
   struct Editora *editora2 = (struct Editora *)b;

    if(editora1->id == editora2->id){
        return 1;
    } else {
        return 0;
    }
}

int comparaFKLivroEditora(void* l, void* e){
    struct Livro * livro = (struct Livro *)l;
    struct Editora * editora = (struct Editora *)e;
    if(livro->id_editora == editora->id){
        return 1;
    } else {
        return 0;
    }
}

void criarEditora(){
    struct Editora  editora, ultimaEditora;
    pDFile arq;
    int id = 0;

    arq = openn("Editora.dat", sizeof(struct Editora));

    if(arq == NULL){
        printf("Não foi possivel abrir o arquivo Editora.dat\n");
        return;
    }

    //Recebendo o id da ultima editora
    fseek(arq->arquivo, -(int)sizeof(struct Editora), SEEK_END);
    int result = fread(&ultimaEditora, sizeof(struct Editora), 1, arq->arquivo);

    if(result == 0 ){
        id = 1;
    } else{
        id = 1 + ultimaEditora.id;
    }

    printf("--Criar Editora-- \n\n");
    printf("Digite o nome da Editora: ");
    fgets(editora.nome, 30, stdin);
    fflush(stdin);

    printf("Digite o nome da cidade da Editora: ");
    fgets(editora.cidade, 30, stdin);
    fflush(stdin);

    editora.id = id;

    createe(arq, &editora);
    closee(arq);
}

void editarEditora(){
    struct Editora editora;
    pDFile arq;
    int id = 0;
    FuncaoComparacao compara = comparaEditora;

    arq = openn("Editora.dat", sizeof(struct Editora));
    printf("--Editar Editora--\n");

    printf("Digite o ID da Editora que deseja alterar: ");
    scanf("%d", &id);
    fflush(stdin);
    editora.id = id;

    printf("Digite o nome para alterar: ");
    fgets(editora.nome, 30, stdin);
    fflush(stdin);

    printf("Digite o nome da cidade para alterar: ");
    fgets(editora.cidade, 30, stdin);
    fflush(stdin);

    updatee(arq, &id, &editora, compara);
    closee(arq);
}

void exibirListaEditoras(){
    struct Editora editora;
    pDFile arq;

    arq = openn("Editora.dat", sizeof(struct Editora));

    printf("--Lista de Editoras--\n");

    pDLista listaEditoras = queryAll(arq, alocaEditora);
    for(int i = 1; i <= listaEditoras->quantidade; i++){
        struct Editora* e = (struct Editora*)buscarInfoPos(listaEditoras, i);
        imprimeEditora(e);
    }
}

void excluirEditora(){
    struct Editora editora;
    pDFile arq, arqLivro;
    int id = 0;
    FuncaoComparacao compara = comparaEditora;

    arq = openn("Editora.dat", sizeof(struct Editora));
    arqLivro = openn("Livro.dat", sizeof(struct Livro));

    if(arq == NULL){
        printf("Não foi possivel abrir o arquivo Editora.dat\n");
        return;
    } else if(arqLivro == NULL){
        printf("Não foi possível abrir o arquivo Livro.dat\n");
        return;
    }

    printf("--Excluir Editora--\n\n");
    printf("Digite o ID da Editora que deseja excluir: ");
    scanf("%d", &id);
    fflush(stdin);

    editora.id = id;

    //Delete Cascade Livro
    struct Editora * editoraVerf = (struct Editora *) retrieve(arq, &editora, comparaEditora);

    FuncaoComparacao comparafkLivroEd = comparaFKLivroEditora;
    delete(arqLivro, editoraVerf, comparafkLivroEd);
    delete(arq, &editora, compara);
    
    closee(arq);
    closee(arqLivro);

}

void menuEditora(){
    int op = 1;
    while(op != 0){
        printf("--Menu da Editora (CRUD)--\n\n");
        printf("Selecione a opcao desejada: \n");
        printf("1 - Criar nova Editora\n");
        printf("2 - Editar Editora\n");
        printf("3 - Exibir lista de Editoras\n");
        printf("4 - Excluir Editora\n");
        printf("0 - Sair\n");

        printf("Digite a opcao desejada: ");
        scanf("%d", &op);
        fflush(stdin);

        switch (op)
        {
        case 1:
            criarEditora();
            break;
        case 2:
            editarEditora();
            break;
        case 3:
            exibirListaEditoras();
            break;
        case 4: 
            excluirEditora();
            break;
        default:
            break;
        }
    }
   
}
//FIm Funcoes Editora

//Inicio Funcoes Livro
void * alocaLivro(){
    return malloc(sizeof(struct Livro));
}

void imprimeLivro(void* livro, void* autor, void* editora){
    struct Livro *l = (struct Livro *) livro;
    struct Autor *a = (struct Autor *) autor;
    struct Editora *e = (struct Editora *) editora;

    printf("------------------------------\n");
    printf("ISBN do Livro: %d\n", l->isbn);
    printf("Titulo do Livro: %s", l->titulo);
    printf("Ano de publicacao do Livro: %d\n", l->ano_publicacao);
    printf("Nome do(a) Autor(a): %s", a->nome);
    printf("Nome da Editora: %s", e->nome);
    printf("Cidade da Editora: %s", e->cidade);

}


int comparaFKAutorLivro(void* au, void* li){
    struct Autor *autor = (struct Autor *)au;
    struct Livro *livro = (struct Livro *)li;

    if(autor->id == livro->id_autor){
        return 1;
    } else {
        return 0;
    }
}

int comparaFKEditoraLivro(void* ed, void* li){
    struct Editora *editora = (struct Editora *)ed;
    struct Livro *livro = (struct Livro *)li;

    if(editora->id == livro->id_editora){
        return 1;
    } else {
        return 0;
    }
}

void criarLivro(){
    struct Livro livro, ultimoLivro;
    struct Autor autor;
    struct Editora editora;

    pDFile arqLivro, arqEditora, arqAutor;

    int id = 0, ano_publicacao = 0, id_autor = 0, id_editora = 0;

    arqLivro = openn("Livro.dat", sizeof(struct Livro));
    arqEditora = openn("Editora.dat", sizeof(struct Editora));
    arqAutor = openn("Autor.dat", sizeof(struct Autor));

    if(arqLivro == NULL){
        printf("Não foi possivel abrir o arquivo Livro.dat\n");
        return;
    } else if (arqEditora == NULL){
        printf("Não foi possivel abrir o arquivo Editora.dat\n");
        return;
    } else if(arqAutor == NULL){
        printf("Não foi possivel abrir o arquivo Autor.dat\n");
        return;
    }

    //Recebendo o id do ultimo livro
    fseek(arqLivro->arquivo, -(int)sizeof(struct Livro), SEEK_END);
    int result = fread(&ultimoLivro, sizeof(struct Livro), 1, arqLivro->arquivo);

    if(result == 0){
        id = 1;
    } else {
        id = 1 + ultimoLivro.isbn;
    }

    livro.isbn = id;

    printf("--Criar Livro--\n\n");
    printf("Digite o Titulo do Livro: ");
    fgets(livro.titulo, 30, stdin);
    fflush(stdin);

    printf("Digite o Ano de Publicacao do Livro: ");
    scanf("%d", &ano_publicacao);
    fflush(stdin);
    livro.ano_publicacao = ano_publicacao;

    //Exibir Lista de Autores
    printf("Selecione o Autor do Livro: ");
    exibirListaAutores();
    
    printf("Digite o ID do Autor do Livro: ");
    scanf("%d", &id_autor);
    fflush(stdin);

    livro.id_autor = id_autor;
    autor.id = id_autor;

    //Verifica se o Autor realmente existe;
    FuncaoComparacao fkAutorLivro = comparaFKAutorLivro;
    struct Autor *autorVerf = (struct Autor *) retrieve(arqAutor, &livro, fkAutorLivro);
    
    if(autorVerf == NULL){
        printf("Autor não existente! Erro ao criar Livro\n");
        return;
    }

    //Exibir Lista de Editoras
    printf("Selecione a Editora do Livro: ");
    exibirListaEditoras();
    printf("Digite o ID da Editora do Livro: ");
    scanf("%d", &id_editora);
    fflush(stdin);

    livro.id_editora = id_editora;
    editora.id = id_editora;

    //Verifica se a Editora realmente existe
    FuncaoComparacao fkEditoraLivro = comparaFKEditoraLivro;
    struct Editora *editoraVerf = (struct Editora *) retrieve(arqEditora, &livro, fkEditoraLivro);
    
    if(editoraVerf == NULL){
        printf("Editora não existente! Erro ao criar Livro!\n");
        return;
    } 

    createe(arqLivro, &livro);

    closee(arqLivro);
    closee(arqAutor);
    closee(arqEditora);
}

void editarLivro(){
    struct Livro livro;
    pDFile arq;
    int op = 0, isbn;

    arq = openn("Livro.dat", sizeof(struct Livro));

    printf("Digite o ISBN do Livro que deseja Alterar: ");
    scanf("%d", &isbn);
    fflush(stdin);
    livro.isbn = isbn;

    

    //Adquire e veirifica se ISBN existe
    FuncaoComparacao compara = comparaLivro;
    struct Livro *livroVerf = (struct Livro *) retrieve(arq, &livro, compara);
    if(livroVerf == NULL){
        printf("Livro com o ISBN inserido inexistente!");
        return;
    }

    do{
        printf("--Editar Livro--\n");
        printf("Selecione o campo que deseja editar:\n");
        printf("1 - Editar Titulo\n");
        printf("2 - Editar o Ano de Publicacao\n");
        printf("3 - Alterar o Autor\n");
        printf("4 - Alterar a Editora\n");
        printf("0 - Salvar e sair\n");
        printf("Digite a opcao selecionada: ");
        scanf("%d", &op);
        fflush(stdin);

        switch (op){
        case 1:
            printf("Titulo atual: %s", livroVerf->titulo);
            printf("Digite o novo titulo: ");

            fgets(livroVerf->titulo, 30, stdin);
            fflush(stdin);
            break;
        case 2:
            printf("Ano de publicacao atual:%d\n", livroVerf->ano_publicacao);
            printf("Digite o novo ano de publicacao: ");

            int ano;
            scanf("%d", &ano);
            fflush(stdin);

            livroVerf->ano_publicacao = ano;
            break;
        case 3:
            printf("Id do autor atual: %d\n", livroVerf->id_autor);
            printf("Digite o novo Id do autor: ");

            int autor;
            scanf("%d", &autor);
            fflush(stdin);

            livroVerf->id_autor = autor;
            break;
        case 4:
            printf("Id da Editora atual: %d\n", livroVerf->id_editora);
            printf("Digite o novo Id da Editora: ");

            int editora;
            scanf("%d", &editora);
            fflush(stdin);

            livroVerf->id_editora = editora;
            break;
        default:
            break;
        }
    }
    while(op != 0);

    updatee(arq, &isbn, livroVerf, compara);
    closee(arq);
}   

void exibirListaLivros(){
    pDFile arqLivro, arqEditora, arqAutor;

    arqLivro = openn("Livro.dat", sizeof(struct Livro));
    arqEditora = openn("Editora.dat", sizeof(struct Editora));
    arqAutor = openn("Autor.dat", sizeof(struct Autor));

    if(arqLivro == NULL){
        printf("Não foi possivel abrir o arquivo Livro.dat\n");
        return;
    } else if (arqEditora == NULL){
        printf("Não foi possivel abrir o arquivo Editora.dat\n");
        return;
    } else if(arqAutor == NULL){
        printf("Não foi possivel abrir o arquivo Autor.dat\n");
        return;
    }

    printf("--Lista de Livros--\n");

    pDLista listaLivros = queryAll(arqLivro, alocaLivro);
    for(int i = 1; i <= listaLivros->quantidade; i++){
        struct Livro* livro = (struct Livro *)buscarInfoPos(listaLivros, i);

        //Adquirir informacoes do Autor do Livro
        FuncaoComparacao fkAutorLivro = comparaFKAutorLivro;
        struct Autor *autor = (struct Autor *) retrieve(arqAutor, livro, fkAutorLivro);
        
        //Adquirir informacoes da Editora do Livro
        FuncaoComparacao fkEditoraLivro = comparaFKEditoraLivro;
        struct Editora *editora = (struct Editora *)retrieve(arqEditora, livro, fkEditoraLivro);
        
        imprimeLivro(livro, autor, editora);
    }
    printf("------------------------------\n");


    closee(arqLivro);
    closee(arqAutor);
    closee(arqEditora);
}

void excluirLivro(){
    struct Livro livro;
    pDFile arq;
    int id = 0;
    FuncaoComparacao compara = comparaLivro;

    arq = openn("Livro.dat", sizeof(struct Livro));

    if(arq == NULL){
        printf("Não foi possivel anrir o arquivo Livro.dat\n");
        return;
    }

    printf("--Excluir Livro--\n\n");
    printf("Digite o Id do Livro que deseja excluir: ");
    scanf("%d", &id);
    livro.isbn = id;

    delete(arq, &livro, compara);
    closee(arq);
}

void menuLivro(){
    int op = 1;
    while(op != 0){
        printf("--Menu do Livro (CRUD)--\n\n");
        printf("Selecione a opcao desejada: \n");
        printf("1 - Criar novo Livro\n");
        printf("2 - Editar Livro\n");
        printf("3 - Exibir lista de Livros\n");
        printf("4 - Excluir Livro\n");
        printf("0 - Sair\n");

        printf("Digite a opcao desejada: ");
        scanf("%d", &op);
        fflush(stdin);

        switch (op)
        {
        case 1:
            criarLivro();
            break;
        case 2:
            editarLivro();
            break;
        case 3:
            exibirListaLivros();
            break;
        case 4: 
            excluirLivro();
            break;
        default:
            break;
        }
    }
}
//Fim Funcoes Livro

void main(int argc,char *argv[]) {
    int op = 1;

    while(op != 0){
        printf("--Biblioteca - UTFPR 2024/2--\n\n");
        printf("Selecione a opcao desejada: \n");
        printf("1 - Menu Autor\n");
        printf("2 - Menu Editora\n");
        printf("3 - Menu Livro\n");

        printf("Digite a opcao: ");
        scanf("%d", &op);
        fflush(stdin);

        switch(op){
            case 1:
                menuAutor();
                break;
            case 2:
                menuEditora();
                break;
            case 3:
                menuLivro();
                break;
            default:
                break;
        }
    }
}