#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_t threads[8];

FILE * desordenado;
FILE * ordenado;

void imprime_arquivos(FILE *arq,char *modo, char *nome, int * vet, int tam){
	arq=fopen(nome,modo);
	for(int i=0 ; i<tam ;i++){
		fprintf(arq,"[%d] ",vet[i]);
	};
	fclose(arq);
};


typedef struct{
	int inicio,fim;
}Valores;

int vetor_pivos[10];

int *vet;
int n;

void swap(int *a, int *b){
	*a=*a^*b;
	*b=*a^*b;
	*a=*a^*b;
};

Valores particionador(int *vet, int tam){ //essa função diviide o vetor em duas parte e retorna a posição do PIVÔ
	int pivo=vet[0];
	Valores val;
	val.inicio=1;
	val.fim=tam-1;
	do{
		while(val.inicio < tam && vet[val.inicio] <= pivo){
			val.inicio++;
		};
		while(vet[val.fim] > pivo){
			val.fim--;
		};
		if(val.inicio < val.fim){
			swap(&vet[val.inicio], &vet[val.fim]);
			val.inicio++;
			val.fim--;
		};
	}while(val.inicio <= val.fim);
	vet[0]=vet[val.fim];
	vet[val.fim]=pivo;
	return val;
};

void quicksort(int *vetor, int tamanho){
	if(tamanho <= 1)
		return ;
	else{
		int pivo=vetor[0];
		int inicio=1;
		int fim=tamanho-1;
	do{
		while(inicio < tamanho && vetor[inicio] <= pivo)
			inicio++;
		while(vetor[fim] > pivo)
			fim--;
		if(inicio < fim){
			swap(&vetor[inicio] , &vetor[fim]);
			inicio++;
			fim--;
		}
	}while(inicio<=fim);

	vetor[0]=vetor[fim];
	vetor[fim]=pivo;

	quicksort(vetor, fim);
	quicksort(&vetor[inicio] , tamanho-inicio);
	}
}

void divide_vetor(int *vet, int tam){
// vet                                         tam
//	0___1____2____3_____4_____5______6_____7____8
    Valores quatro=particionador(vet,tam);
    
    Valores dois=particionador(vet,quatro.fim);
    Valores um=particionador(vet,dois.fim);
    Valores tres=particionador(&vet[dois.inicio],quatro.fim-dois.inicio);

    Valores seis=particionador(&vet[quatro.inicio] , tam-quatro.inicio);
    Valores sete=particionador(&vet[seis.inicio], tam-seis.inicio);
    Valores cinco=particionador(&vet[quatro.inicio],seis.fim-quatro.inicio);
    
    vetor_pivos[0]=0;
    vetor_pivos[1]=um.inicio;
    vetor_pivos[2]=dois.inicio;
    vetor_pivos[3]=tres.inicio;
    vetor_pivos[4]=quatro.inicio;
    vetor_pivos[5]=cinco.inicio;
    vetor_pivos[6]=seis.inicio;
    vetor_pivos[7]=sete.inicio;
    vetor_pivos[8]=tam;

};

void * um(void *arg){
	quicksort(&vet[vetor_pivos[0]],vetor_pivos[1]-vetor_pivos[0]);
	return arg;
};
void * dois(void *arg){
	quicksort(&vet[vetor_pivos[1]],vetor_pivos[2]-vetor_pivos[1]);
	return arg;
};
void * tres(void *arg){
	quicksort(&vet[vetor_pivos[2]],vetor_pivos[3]-vetor_pivos[2]);
	return arg;
};
void * quatro(void *arg){
	quicksort(&vet[vetor_pivos[3]],vetor_pivos[4]-vetor_pivos[3]);
	return arg;
};
void * cinco(void *arg){
	quicksort(&vet[vetor_pivos[4]],vetor_pivos[5]-vetor_pivos[4]);
	return arg;
};
void * seis(void *arg){
	quicksort(&vet[vetor_pivos[5]],vetor_pivos[6]-vetor_pivos[5]);
	return arg;
};
void * sete(void *arg){
	quicksort(&vet[vetor_pivos[6]],vetor_pivos[7]-vetor_pivos[6]);
	return arg;
};
void * oito(void *arg){
	quicksort(&vet[vetor_pivos[7]],vetor_pivos[8]-vetor_pivos[7]);
	return arg;
};

int main(){
	int n;
	srand(time(NULL));
	printf("Digite N:");
	scanf("%d",&n);
	vet=(int *) malloc(n* sizeof(int));
	for(int i=0;i<n;i++){
		vet[i]=rand()%n;
	}
	imprime_arquivos(desordenado,"w+","Vetor_Desordenado.txt",vet,n);
//criando as threads
	divide_vetor(vet,n);
	printf("\nCriando Threads...\n");
	pthread_create(&(threads[0]), NULL, um, NULL);
	pthread_create(&(threads[1]), NULL, dois, NULL);
	pthread_create(&(threads[2]), NULL, tres, NULL);
	pthread_create(&(threads[3]), NULL, quatro, NULL);
	pthread_create(&(threads[4]), NULL, cinco, NULL);
	pthread_create(&(threads[5]), NULL, seis, NULL);
	pthread_create(&(threads[6]), NULL, sete, NULL);
	pthread_create(&(threads[7]), NULL, oito, NULL);

	for(int i=0; i<8; i++) {
		pthread_join(threads[i], NULL);
	}
	printf("\nFim das Threads...\n");

    imprime_arquivos(ordenado,"w+","Vetor_Ordenado.txt",vet,n);
	return 0;
};
