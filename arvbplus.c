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
  int nchaves, folha, *chave;
  Aluno *aluno;
  struct ArvB **filho;
  struct ArvB *prox;
}TAB;


TAB *Cria(int t){
  TAB* novo = (TAB*)malloc(sizeof(TAB));
  novo->nchaves = 0;
  novo->chave =(int*)malloc(sizeof(int*)*((t*2)-1));
  novo->aluno = (Aluno*) malloc(sizeof(Aluno) * ((t*2)-1));
  novo->folha=1;
  novo->filho = (TAB**)malloc(sizeof(TAB*)*t*2);
  int i;
  for(i=0; i<(t*2); i++) novo->filho[i] = NULL;
  return novo;
}

int folha(TAB *a)
{
	int i;
	for(i=0; i<=a->nchaves; i++)
		if(a->filho[i])
			return 0;
	return 1;
}


TAB *Libera(TAB *a){
  if(a){
    if(!a->folha){
      int i;
      for(i = 0; i <= a->nchaves; i++) Libera(a->filho[i]);
    }
    free(a->chave);
    free(a->filho);
    free(a->aluno);
    free(a);
    return NULL;
  }
}


void Imprime(TAB *a, int andar){
  if(a){
    int i,j;
    for(i=0; i<=a->nchaves-1; i++){
      Imprime(a->filho[i],andar+1);
      for(j=0; j<=andar; j++) printf("   ");
      if(!a->folha) printf("%d\n", a->chave[i]);
      else
    	  printf("%d*\t%s\t%.2f\n", a->chave[i], a->aluno[i].nome, a->aluno[i].cr);

    }
    Imprime(a->filho[i],andar+1);
  }
}


TAB *Busca(TAB* x, int ch){
  TAB *resp = NULL;
  if(!x) return resp;
  int i = 0;
  while(i < x->nchaves && ch >= x->chave[i]) i++;
  if((i-1) < x->nchaves && ch == x->chave[i-1] && x->folha) return x;
  if(x->folha) return resp;
  return Busca(x->filho[i], ch);
}


TAB *Inicializa(){
  return NULL;
}


TAB *Divisao(TAB *x, int i, TAB* y, int t){
  TAB *z=Cria(t);
  z->nchaves= t - 1;
  z->folha = y->folha;
  int j;
  for(j=0;j<t-1;j++)
	 z->chave[j] = y->chave[j+t];

  if(z->folha)
  {
	  for(j=0; j<t-1; j++)
	  {
		  z->aluno[j].chave = y->aluno[j+t].chave;
		  z->aluno[j].cr = y->aluno[j+t].cr;
		  strcpy(z->aluno[j].nome, y->aluno[j+t].nome);
	  }
  }

  if(!y->folha){
    for(j=0;j<t-1;j++){
      z->filho[j+1] = y->filho[j+t+1];
      y->filho[j+t+1] = NULL;
    }

    z->filho[0] = NULL;
  }

  y->nchaves = t;
  for(j=x->nchaves; j>=i; j--) x->filho[j+1]=x->filho[j];
  x->filho[i] = z;
  for(j=x->nchaves; j>=i; j--) x->chave[j] = x->chave[j-1];
  x->chave[i-1] = y->chave[t];
  x->nchaves++;
  return x;
}


TAB *Insere_Nao_Completo(TAB *x, int chave, float cr, char *nome, int t){
  int i = x->nchaves-1;
  if(x->folha){
    while((i>=0) && (chave<x->chave[i])){
      x->chave[i+1] = x->chave[i];
      x->aluno[i+1].chave = x->aluno[i].chave;
	  x->aluno[i+1].cr = x->aluno[i].cr;
	  strcpy(x->aluno[i+1].nome, x->aluno[i].nome);
      i--;
    }
    x->chave[i+1] = chave;
    x->aluno[i+1].chave = chave;
    x->aluno[i+1].cr = cr;
    strcpy(x->aluno[i+1].nome, nome);
    x->nchaves++;
    return x;
  }
  while((i>=0) && (chave<x->chave[i])) i--;
  i++;
  if(x->filho[i]->nchaves == ((2*t)-1)){
    x = Divisao(x, (i+1), x->filho[i], t);
    if(chave>x->chave[i]) i++;
  }
  x->filho[i] = Insere_Nao_Completo(x->filho[i], chave, cr, nome, t);
  return x;
}


TAB *Insere(TAB *T, int chave, float cr, char *nome, int t){
  if(Busca(T,chave)) return T;
  if(!T){
    T=Cria(t);
    T->chave[0] = chave;
    T->aluno[0].chave = chave;
    T->aluno[0].cr = cr;
    strcpy(T->aluno[0].nome, nome);
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
    	  arv->aluno[j].chave = arv->aluno[j+1].chave;
    	  arv->aluno[j].cr = arv->aluno[j+1].cr;
    	  strcpy(arv->aluno[j].nome, arv->aluno[j+1].nome);
      }
      arv->nchaves--;
      return arv;      
    }
//    if(!arv->folha && arv->filho[i]->nchaves >= t){ //CASO 2A
//      printf("\nCASO 2A\n");
//      TAB *y = arv->filho[i];  //Encontrar o predecessor k' de k na árvore com raiz em y
//      while(!y->folha) y = y->filho[y->nchaves];
//      int temp = y->chave[y->nchaves-1];
//      arv->filho[i] = remover(arv->filho[i], temp, t);
//      //Eliminar recursivamente K e substitua K por K' em x
//      arv->chave[i] = temp;
//      return arv;
//    }
//    if(!arv->folha && arv->filho[i+1]->nchaves >= t){ //CASO 2B
//      printf("\nCASO 2B\n");
//      TAB *y = arv->filho[i+1];  //Encontrar o sucessor k' de k na árvore com raiz em y
//      while(!y->folha) y = y->filho[0];
//      int temp = y->chave[0];
//      y = remover(arv->filho[i+1], temp, t); //Eliminar recursivamente K e substitua K por K' em x
//      arv->chave[i] = temp;
//      return arv;
//    }
//    if(!arv->folha && arv->filho[i+1]->nchaves == t-1 && arv->filho[i]->nchaves == t-1){ //CASO 2C
//      printf("\nCASO 2C\n");
//      TAB *y = arv->filho[i];
//      TAB *z = arv->filho[i+1];
//      y->chave[y->nchaves] = ch;          //colocar ch ao final de filho[i]
//      int j;
//      for(j=0; j<t-1; j++)                //juntar chave[i+1] com chave[i]
//        y->chave[t+j] = z->chave[j];
//      for(j=0; j<=t; j++)                 //juntar filho[i+1] com filho[i]
//        y->filho[t+j] = z->filho[j];
//      y->nchaves = 2*t-1;
//      for(j=i; j < arv->nchaves-1; j++)   //remover ch de arv
//        arv->chave[j] = arv->chave[j+1];
//      for(j=i+1; j <= arv->nchaves; j++)  //remover ponteiro para filho[i+1]
//        arv->filho[j] = arv->filho[j+1];
//      arv->filho[j] = NULL; //Campello
//      arv->nchaves--;
//      arv->filho[i] = remover(arv->filho[i], ch, t);
//      return arv;
//    }
  }

  TAB *y = arv->filho[i], *z = NULL;
  if (y->nchaves == t-1){ //CASOS 3A e 3B
    if((arv->filho[i+1]) && (i < arv->nchaves) && (arv->filho[i+1]->nchaves >=t)){ //CASO 3A
      printf("\nCASO 3A: i menor que nchaves\n");
      z = arv->filho[i+1];
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
		  y->aluno[t-1].chave = z->aluno[0].chave;
		  y->aluno[t-1].cr = z->aluno[0].cr;
		  strcpy(y->aluno[t-1].nome, z->aluno[0].nome);
		  int j;
		  for(j=0; j < z->nchaves-1; j++)  //ajustar chaves de z
		  {
			z->chave[j] = z->chave[j+1];
			z->aluno[j].chave = z->aluno[j+1].chave;
			z->aluno[j].cr = z->aluno[j+1].cr;
			strcpy(z->aluno[j].nome, z->aluno[j+1].nome);
		  }
		  //z->chave[j] = 0; Rosseti
		  //y->filho[y->nchaves] = z->filho[0]; //enviar ponteiro menor de z para o novo elemento em y
		  //for(j=0; j < z->nchaves; j++)       //ajustar filhos de z
			//z->filho[j] = z->filho[j+1];
		  printf("(%d,%d)\n", arv->chave[i], z->chave[0]);
		  arv->chave[i] = z->chave[0];
		  z->nchaves--;
		  arv->filho[i] = remover(arv->filho[i], ch, t);
      }

      return arv;
    }
    if((i > 0) && (!z) && (arv->filho[i-1]) && (arv->filho[i-1]->nchaves >=t)){ //CASO 3A
      printf("\nCASO 3A: i igual a nchaves\n");
      z = arv->filho[i-1];
      if(!y->folha)
      {
		  //int j;
		  //for(j = y->nchaves; j>0; j--)               //encaixar lugar da nova chave
		    //y->chave[j] = y->chave[j-1];
		  //for(j = y->nchaves+1; j>0; j--)             //encaixar lugar dos filhos da nova chave
			//y->filho[j] = y->filho[j-1];
		  //y->chave[0] = z->chave[z->nchaves-1]; //dar a y a chave i da arv
		  //y->nchaves++;
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
			  y->aluno[j].chave = y->aluno[j-1].chave;
			  y->aluno[j].cr = y->aluno[j-1].cr;
			  strcpy(y->aluno[j].nome, y->aluno[j-1].nome);
			}
			//for(j = y->nchaves+1; j>0; j--)             //encaixar lugar dos filhos da nova chave
			  //y->filho[j] = y->filho[j-1];
			y->chave[0] = z->chave[z->nchaves-1]; //dar a y a chave i da arv
			y->aluno[0].chave = z->aluno[z->nchaves-1].chave;
			y->aluno[0].cr = z->aluno[z->nchaves-1].cr;
			strcpy(y->aluno[0].nome, z->aluno[z->nchaves-1].nome);
			y->nchaves++;
			arv->chave[i-1] = z->chave[z->nchaves-1]; //dar a arv uma chave de z
			//y->filho[0] = z->filho[z->nchaves];         //enviar ponteiro de z para o novo elemento em y
			z->nchaves--;
			arv->filho[i] = remover(y, ch, t);
      }

      return arv;
    }
    if(!z){ //CASO 3B
      if(i < arv->nchaves && arv->filho[i+1]->nchaves == t-1){
        printf("\nCASO 3B: i menor que nchaves\n");
        z = arv->filho[i+1];
        int j;
        if(!y->folha)
        {
        	y->chave[t-1] = arv->chave[i];     //pegar chave [i] e coloca ao final de filho[i]
        	y->nchaves++;

			for(j=0; j < t-1; j++){
			  y->chave[t+j] = z->chave[j];  //passar filho[i+1] para filho[i]
			  y->aluno[t+j].chave = z->aluno[j].chave;
			  y->aluno[t+j].cr = z->aluno[j].cr;
			  strcpy(y->aluno[t-1+j].nome, z->aluno[j].nome);
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
			  y->aluno[t-1+j].chave = z->aluno[j].chave;
			  y->aluno[t-1+j].cr = z->aluno[j].cr;
			  strcpy(y->aluno[t-1+j].nome, z->aluno[j].nome);
			  y->nchaves++;
			}
        }

        /*if(!y->folha){
          for(j=0; j<t; j++){
            y->filho[t+j] = z->filho[j];
          }
        }*/

        for(j=i; j < arv->nchaves-1; j++){ //limpar referências de i
          arv->chave[j] = arv->chave[j+1];
          arv->filho[j+1] = arv->filho[j+2];
        }
        arv->nchaves--;
        arv = remover(arv, ch, t);
        return arv;
      }
      if((i > 0) && (arv->filho[i-1]->nchaves == t-1)){
        printf("\nCASO 3B: i igual a nchaves\n");
        z = arv->filho[i-1];
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
			  z->aluno[t+j].chave = y->aluno[j].chave;
			  z->aluno[t+j].cr = y->aluno[j].cr;
			  strcpy(z->aluno[t-1+j].nome, y->aluno[j].nome);
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
			  z->aluno[t-1+j].chave = y->aluno[j].chave;
			  z->aluno[t-1+j].cr = y->aluno[j].cr;
			  strcpy(z->aluno[t-1+j].nome, y->aluno[j].nome);
			  z->nchaves++;
			}
        }

        /*if(!z->folha){
          for(j=0; j<t; j++){
            z->filho[t+j] = y->filho[j];
          }
        }*/

        arv->nchaves--;
        arv->filho[i-1] = z;
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


/*int main(int argc, char *argv[]){
  TAB * arvore = Inicializa();
  int num = 0, chave;
  char nome[31];
  float cr;
  while(num != -1){
    printf("Digite um numero para adicionar. 0 para imprimir. -9 para remover e -1 para sair\n");
    scanf("%i", &num);
    if(num != -9)
    {
		scanf(" %30[^\n]", nome);
		scanf("%f", &cr);
    }
    if(num == -9){
      scanf("%d", &chave);
      arvore = retira(arvore, chave, t);
      Imprime(arvore,0);
    }
    else if(num == -1){
      printf("\n");
      Imprime(arvore,0);
      Libera(arvore);
      return 0;
    }
    else if(!num){
      printf("\n");
      Imprime(arvore,0);
    }
    else
    {
    	arvore = Insere(arvore, num, cr, nome, t);
    	Imprime(arvore, 0);
    }
    printf("\n\n");
  }
}*/

int main(void)
{
	  TAB * arvore = Inicializa();
	  int chave, cf = 0;
	  char nome[31], n_arq[31];
	  float cr;

	  while(cf != -1)
	  {
		  printf("0 - Inserir registro manualmente\n"
				 "1 - Inserir registros por arquivo\n"
				 "2 - Remover um registro\n"
				 "3 - Imprimir estrutura\n"
				 "4 - Buscar um registro\n"
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
		  		  while(!feof(f))
		  		  {
		  			  fscanf(f, "%d %s %f", &chave, nome, &cr);
		  			  arvore = Insere(arvore, chave, cr, nome, t);
		  		  }
		  		  fclose(f);
		  	  break;

		  	  case 2:
		  		  printf("Digite a matriculo do aluno a ser removido: ");
		  		  scanf("%d", &chave);
		  		  arvore = retira(arvore, chave, t);
		  	  break;

		  	  case 3:
		  		  Imprime(arvore, t);
		  	  break;

		  	  case 4:
		  		  printf("Digite a matricula do aluno a ser buscado: ");
		  		  scanf("%d", &chave);
		  		  TAB *a = Busca(arvore, chave);
		  		  int i;
		  		  for(i=0; (i<a->nchaves && a->chave[i]<chave); i++);
		  		  printf("\nMatricula: %d\n"
		  				 "Nome: %s\n"
		  				 "CR: %.2f", a->aluno[i].chave, a->aluno[i].nome, a->aluno[i].cr);
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
