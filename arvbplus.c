#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int t = 2;

typedef struct aluno
{
	int chave;
	float cr;
	char nome[31];
}Aluno;

typedef struct ArvB{
  int nchaves;
  int folha;
  int *chave;
  void **filho;
  struct ArvB *prox;
}TAB;


TAB *Cria(int t){
  TAB* novo = (TAB*)malloc(sizeof(TAB));
  novo->nchaves = 0;
  novo->chave =(int*)malloc(sizeof(int*)*((t*2)-1));
  novo->folha=1;
  novo->filho = malloc(sizeof(void *)*t*2);
  novo->prox = NULL;
  int i;
  for(i=0; i<(t*2); i++) novo->filho[i] = NULL;
  return novo;
}


TAB *Libera(TAB *a){
  if(a){
    int i;
    if(!a->folha){
      for(i = 0; i <= a->nchaves; i++)
          Libera((TAB*)a->filho[i]);
    }
    for (i = 0; i < a->nchaves; i++)
    {
        free(a->filho[i]);
    }
    free(a->chave);
    free(a->filho);
    free(a);
    return NULL;
  }
}


void Imprime(TAB *a, int andar){
  if(a){
    int i,j;
    for(i=0; i<=a->nchaves-1; i++){
      if (!a->folha)
        Imprime((TAB*)a->filho[i],andar+1);
      for(j=0; j<=andar; j++)
          printf("   ");
      if(!a->folha)
          printf("%d\n", a->chave[i]);
      else
      {
          Aluno *aluno = (Aluno*) a->filho[i];
    	  printf("%d*\t%s\t%.2f\n", aluno->chave, aluno->nome, aluno->cr);
      }

    }
    if (!a->folha)
      Imprime((TAB*) a->filho[i],andar+1);
  }
}


TAB *Busca(TAB* x, int ch){
  TAB *resp = NULL;
  if(!x) return resp;
  int i = 0;
  while(i < x->nchaves && ch >= x->chave[i]) i++;
  if((i-1) < x->nchaves && ch == x->chave[i-1] && x->folha) return x;
  if(x->folha) return resp;
  return Busca((TAB*)x->filho[i], ch);
}


TAB *Inicializa(){
  return NULL;
}


TAB *Divisao(TAB *x, int i, TAB* y, int t){
  TAB *z = Cria(t);
  z->nchaves= t - 1;
  z->folha = y->folha;
  int j;
  for(j=0;j<t-1;j++)
	 z->chave[j] = y->chave[j+t];

  if(z->folha)
  {
	  for(j=0; j<t-1; j++)
	  {
		  z->filho[j] = y->filho[j+t];
	  }
  }

  if(!y->folha){
    for(j=0;j<t;j++){
      z->filho[j] = y->filho[j+t];
      y->filho[j+t] = NULL;
    }
  }

  y->nchaves = t-1;
  if(y->folha)
  {
	  y->nchaves++;
      y->prox = z;
  }
  for(j=x->nchaves; j>=i; j--)
      x->filho[j+1]=x->filho[j];
  x->filho[i] = z;
  for(j=x->nchaves; j>=i; j--)
      x->chave[j] = x->chave[j-1];
  x->chave[i-1] = y->chave[t-1];
  if(y->folha)
	  x->chave[i-1] = y->chave[t];
  x->nchaves++;
  return x;
}


TAB *Insere_Nao_Completo(TAB *x, int chave, float cr, char *nome, int t){
  int i = x->nchaves-1;
  if(x->folha){
    while((i>=0) && (chave<x->chave[i])){
      x->chave[i+1] = x->chave[i];
      x->filho[i+1] = x->filho[i];
      i--;
    }
    x->chave[i+1] = chave;
    
    Aluno *novo_aluno = (Aluno*) malloc(sizeof(Aluno));
    novo_aluno->chave = chave;
    novo_aluno->cr = cr;
    strcpy(novo_aluno->nome, nome);
    
    x->filho[i+1] = novo_aluno;
    x->nchaves++;
    return x;
  }
  while((i>=0) && (chave<x->chave[i])) i--;
  i++;
  TAB *aux = (TAB*) x->filho[i];
  if(aux->nchaves == ((2*t)-1)){
    x = Divisao(x, (i+1), (TAB*)x->filho[i], t);
    if(chave>x->chave[i]) i++;
  }
  x->filho[i] = Insere_Nao_Completo((TAB*)x->filho[i], chave, cr, nome, t);
  return x;
}


TAB *Insere(TAB *T, int chave, float cr, char *nome, int t){
  if(Busca(T,chave)) return T;
  if(!T){
    T=Cria(t);
    T->chave[0] = chave;
    
    Aluno *novo_aluno = (Aluno*) malloc(sizeof(Aluno));
    novo_aluno->chave = chave;
    novo_aluno->cr = cr;
    strcpy(novo_aluno->nome, nome);
    
    T->filho[0] = novo_aluno;
    T->nchaves=1;
    return T;
  }
  if(T->nchaves == (2*t)-1){
    TAB *S = Cria(t);
    S->nchaves=0;
    S->folha = 0;
    S->filho[0] = T;
    S = Divisao(S,1,T,t);
    S = Insere_Nao_Completo(S,chave, cr, nome, t);
    return S;
  }
  T = Insere_Nao_Completo(T,chave, cr, nome, t);
  return T;
}


TAB* remover(TAB* arv, int ch, int t){
  if(!arv) return arv;
  int i;
  printf("Removendo %d...\n", ch);
  for(i = 0; i<arv->nchaves && arv->chave[i] <= ch; i++);
  if((i-1) < arv->nchaves && ch == arv->chave[i-1]){ //CASOS 1, 2A, 2B e 2C
    if(arv->folha){ //CASO 1
      printf("\nCASO 1\n");
      int j;
      for(j=i-1; j<arv->nchaves-1;j++)
      {
    	  arv->chave[j] = arv->chave[j+1];
    	  arv->filho[j] = arv->filho[j+1];
      }
      arv->nchaves--;
      return arv;      
    }
  }

  TAB *y = (TAB*) arv->filho[i], *z = NULL, *r = NULL;
  if (y->nchaves == t-1){ //CASOS 3A e 3B
    r = (TAB*) arv->filho[i+1];
    if((r) && (i < arv->nchaves) && (r->nchaves >=t)){ //CASO 3A
      printf("\nCASO 3A: i menor que nchaves\n");
      z = r;
      if(!y->folha)
      {
		  y->chave[t-1] = arv->chave[i];   //dar a y a chave i da arv
		  y->nchaves++;
		  arv->chave[i] = z->chave[0];
		  int j;
		  for(j=0; j < z->nchaves-1; j++)  //ajustar chaves de z
			z->chave[j] = z->chave[j+1];
		  y->filho[y->nchaves] = z->filho[0]; //enviar ponteiro menor de z para o novo elemento em y
		  for(j=0; j < z->nchaves; j++)       //ajustar filhos de z
			z->filho[j] = z->filho[j+1];
		  z->nchaves--;
		  arv->filho[i] = remover(arv->filho[i], ch, t);
      }

      else
      {
		  y->nchaves++;
		  y->chave[t-1] = z->chave[0];   //dar a y a chave i da arv
		  y->filho[t-1] = z->filho[0];
		  int j;
		  for(j=0; j < z->nchaves-1; j++)  //ajustar chaves de z
		  {
			z->chave[j] = z->chave[j+1];
			z->filho[j] = z->filho[j+1];
		  }
		  printf("(%d,%d)\n", arv->chave[i], z->chave[0]);
		  arv->chave[i] = z->chave[0];
		  z->nchaves--;
		  arv->filho[i] = remover((TAB*) arv->filho[i], ch, t);
      }

      return arv;
    }
    r = (TAB*) arv->filho[i-1];
    if((i > 0) && (!z) && (r) && (r->nchaves >=t)){ //CASO 3A
      printf("\nCASO 3A: i igual a nchaves\n");
      z = r;
      if(!y->folha)
      {
		  int j;
		  for(j = y->nchaves; j>0; j--)               //encaixar lugar da nova chave
		    y->chave[j] = y->chave[j-1];
		  for(j = y->nchaves+1; j>0; j--)             //encaixar lugar dos filhos da nova chave
			y->filho[j] = y->filho[j-1];
		  y->chave[0] = arv->chave[i-1]; //dar a y a chave i da arv
		  y->nchaves++;
		  arv->chave[i-1] = z->chave[z->nchaves-1]; //dar a arv uma chave de z
    	  y->filho[0] = z->filho[z->nchaves];         //enviar ponteiro de z para o novo elemento em y
		  z->nchaves--;
		  arv->filho[i] = remover(y, ch, t);
      }

      else
      {
			int j;
			for(j = y->nchaves; j>0; j--)               //encaixar lugar da nova chave
			{
			  y->chave[j] = y->chave[j-1];
			  y->filho[j] = y->filho[j-1];
			}
			y->chave[0] = z->chave[z->nchaves-1]; //dar a y a chave i da arv
			y->filho[0] = z->filho[z->nchaves-1];
			y->nchaves++;
			arv->chave[i-1] = z->chave[z->nchaves-1]; //dar a arv uma chave de 
			z->nchaves--;
			arv->filho[i] = remover(y, ch, t);
      }

      return arv;
    }
    if(!z){ //CASO 3B
      r = (TAB*) arv->filho[i+1];
      if(i < arv->nchaves && r->nchaves == t-1){
        printf("\nCASO 3B: i menor que nchaves\n");
        z = r;
        int j;
        if(!y->folha)
        {
        	y->chave[t-1] = arv->chave[i];     //pegar chave [i] e coloca ao final de filho[i]
        	y->nchaves++;

			for(j=0; j < t-1; j++){
			  y->chave[t+j] = z->chave[j];  //passar filho[i+1] para filho[i]
			  //y->filho[t+j] = z->filho[j];
			  y->nchaves++;
			}

			for(j=0; j<t; j++){
				y->filho[t+j] = z->filho[j];
			}
        }

        else
        {
			for(j=0; j < t-1; j++){
			  y->chave[t-1+j] = z->chave[j];  //passar filho[i+1] para filho[i]
			  y->filho[t-1+j] = z->filho[j];
			  y->nchaves++;
			}
        }

        for(j=i; j < arv->nchaves-1; j++){ //limpar referências de i
          arv->chave[j] = arv->chave[j+1];
          arv->filho[j+1] = arv->filho[j+2];
        }
        y->prox = arv->filho[i+1];
        //Libera(z);
        arv->nchaves--;
        arv = remover(arv, ch, t);
        return arv;
      }
      r = (TAB*) arv->filho[i-1];
      if((i > 0) && (r->nchaves == t-1)){
        printf("\nCASO 3B: i igual a nchaves\n");
        z = r;
        int j;
        if(!z->folha)
        {
			if(i == arv->nchaves)
			  z->chave[t-1] = arv->chave[i-1]; //pegar chave[i] e poe ao final de filho[i-1]
			else
			  z->chave[t-1] = arv->chave[i];   //pegar chave [i] e poe ao final de filho[i-1]
			z->nchaves++;

			for(j=0; j < t-1; j++){
			  z->chave[t+j] = y->chave[j]; //passar filho[i+1] para filho[i]
			  z->filho[t+j] = y->filho[j];
			  z->nchaves++;
			}

			for(j=0; j<t; j++){
				z->filho[t+j] = y->filho[j];
			}
        }

        else
        {
			for(j=0; j < t-1; j++){
			  z->chave[t-1+j] = y->chave[j]; //passar filho[i+1] para filho[i]
			  z->filho[t-1+j] = y->filho[j];
			  z->nchaves++;
			}
        }

        z->prox = NULL;
        //Libera(y);
        arv->nchaves--;
        //arv->filho[i-1] = z;
        arv = remover(arv, ch, t);
        return arv;
      }
    }
  }  
  arv->filho[i] = remover(arv->filho[i], ch, t);
  return arv;
}


TAB* retira(TAB* arv, int k, int t){
  if(!arv || !Busca(arv, k)) return arv;
  return remover(arv, k, t);
}


int main(void)
{
	  TAB * arvore = Inicializa();
	  int chave, cf = 0, i;
	  char nome[31], n_arq[31];
	  float cr;
      TAB *aux;

	  while(cf != -1)
	  {
		  printf("0 - Inserir registro manualmente\n"
				 "1 - Inserir registros por arquivo\n"
				 "2 - Remover um registro\n"
				 "3 - Imprimir estrutura\n"
				 "4 - Buscar um registro\n"
				 "5 - Alterar um registro\n"
				 "-1 - Sair do programa\n");

		  printf("Digite uma opcao do menu: ");
		  scanf("%d", &cf);

		  switch(cf)
		  {
		  	  case 0:
		  		  printf("Digite a matricula do aluno: ");
		  		  scanf("%d", &chave);
		  		  printf("Digite o nome do aluno: ");
		  		  scanf(" %30[^\n]", nome);
		  		  printf("Digite o CR do aluno: ");
		  		  scanf("%f", &cr);
		  		  arvore = Insere(arvore, chave, cr, nome, t);
		  	  break;

		  	  case 1:
		  		  printf("Digite o nome do arquivo de registros: ");
		  		  scanf(" %30[^\n]", n_arq);
		  		  FILE *f = fopen(n_arq, "r");
		  		  if(!f)
		  		  {
		  			  printf("Arquivo nao encontrado!!\n");
		  			  continue;
		  		  }

		  		  while(!feof(f))
		  		  {
		  			  fscanf(f, "%d %30[^\t] %f", &chave, nome, &cr);
		  			  arvore = Insere(arvore, chave, cr, nome, t);
		  		  }
		  		  fclose(f);
		  	  break;

		  	  case 2:
		  		  printf("Digite a matricula do aluno a ser removido: ");
		  		  scanf("%d", &chave);
		  		  arvore = retira(arvore, chave, t);
		  	  break;

		  	  case 3:
		  		  Imprime(arvore, t);
		  	  break;

		  	  case 4:
		  		  printf("Digite a matricula do aluno a ser buscado: ");
		  		  scanf("%d", &chave);
		  		  TAB *arv = Busca(arvore, chave);
		  		  if(!arv)
		  		  {
		  			  printf("\nAluno nao registrado!!\n");
		  			  continue;
		  		  }
		  		  for(i=0; (i<arv->nchaves && arv->chave[i]<chave); i++);
                  Aluno *aluno = (Aluno*) arv->filho[i];
		  		  printf("\nMatricula: %d\n"
		  				 "Nome: %s\n"
		  				 "CR: %.2f", aluno->chave, aluno->nome, aluno->cr);
		  	  break;

		  	  case 5:
		  		printf("Digite a matricula do aluno a ser modificado: ");
		  		scanf("%d", &chave);
		  		TAB *a = Busca(arvore, chave);
		  		if(!a)
				{
		  			printf("\nAluno nao registrado!!\n");
					continue;
				}
				for(i=0; (i<a->nchaves && a->chave[i]<chave); i++);
                Aluno *pAluno = (Aluno*) a->filho[i];
				printf("\nDigite o novo nome: ");
				scanf(" %30[^\n]", nome);
				printf("Digite o novo CR: ");
				scanf("%f", &cr);
				pAluno->cr = cr;
				strcpy(pAluno->nome, nome);
		  	  break;
              case 6:
                  aux = arvore;
                  aux = (TAB*) aux->filho[0];
                  while (aux && (!aux->folha))
                  {
                      aux = (TAB*) aux->filho[0];
                  }
                  while (aux)
                  {
                      int k;
                      for (k = 0; k < aux->nchaves; k++)
                          printf("%d", aux->chave[k]);
                      printf(" ");
                      aux = aux->prox;
                  }
                  break;

		  	  case -1:
		  		  Libera(arvore);
		  		  return 0;
		  	  break;

		  	  default:
		  		  Imprime(arvore, t);
		  }

		  printf("\n\n");
	  }
}
