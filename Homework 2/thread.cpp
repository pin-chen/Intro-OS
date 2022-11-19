# include <iostream>
# include <thread>
# include <unistd.h>
# define ll long long int
# define MAX 500
# define NUM 20
# define RANGE MAX / NUM
using namespace std;

int matA[MAX][MAX] = {};
int matB[MAX][MAX] = {};
int matC[MAX][MAX] = {}; // Result of Addition
int matD[MAX][MAX] = {}; // Result of Multiplication
ll sum_C[NUM], sum_D[NUM];

void* add(void* data){
    int index = *(int*) data;
	delete (int*) data;
	for (int i = RANGE * index; i < RANGE * (index + 1); i++) {
        for (int j = 0; j < MAX; j++){
            matC[i][j] = matA[i][j] + matB[i][j];
        }   
    }
    pthread_exit(NULL); // exit child thread 
}

void* mult(void* data){
    int index = *(int*) data;
	delete (int*) data;
	for (int i = RANGE * index; i < RANGE * (index + 1); i++) {
        for (int j = 0; j < MAX; j++) {
            for (int k = 0; k < MAX; k++) {
                matD[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
    pthread_exit(NULL); // exit child thread 
}

void* sum(void* data){
    int index = *(int*) data;
	delete (int*) data;
	for (int i = RANGE * index; i < RANGE * (index + 1); i++) {
        for (int j = 0; j < MAX; j++)
            sum_C[index] +=  matC[i][j];     
    }
    pthread_exit(NULL); // exit child thread 
}

void* sum2(void* data){
    int index = *(int*) data;
	delete (int*) data;
	for (int i = RANGE * index; i < RANGE * (index + 1); i++) {
        for (int j = 0; j < MAX; j++)
            sum_D[index] +=  matD[i][j];     
    }
    pthread_exit(NULL); // exit child thread 
}

int main(){
    cin.tie(0);
	ios::sync_with_stdio(0);
	//get input
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            cin >> matA[i][j];
        }
    }
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            cin >> matB[i][j];
        }
    }
    //Addition();
    //Multiplication();
	int *in1, *in2;
	pthread_t t1[NUM], t2[NUM];    // define thread 
	for (int i = 0; i < NUM; i++) {
		in1 = new int(i);
		in2 = new int(i);
		pthread_create(&t1[i], NULL, add, (void*)in1 );
		pthread_create(&t2[i], NULL, mult, (void*)in2 );
	}
	// wait add
	for (int i = 0; i < NUM; i++) {
		pthread_join(t1[i],NULL);
	}
    // calculate SUM of matC
	for (int i = 0; i < NUM; i++) {
		in1 = new int(i);
		pthread_create(&t1[i], NULL, sum, (void*)in1 );
	}
	// wait mult
	for (int i = 0; i < NUM; i++) {
		pthread_join(t2[i],NULL);
	}
    // calculate SUM of matD
	for (int i = 0; i < NUM; i++) {
		in2 = new int(i);
		pthread_create(&t2[i], NULL, sum2, (void*)in2 );
	}
	// wait child sum
	for (int i = 0; i < NUM; i++) {
		pthread_join(t1[i],NULL);
		pthread_join(t2[i],NULL);
	}
	// sum 
	ll sumC = 0, sumD = 0;
	for(int i = 0; i < NUM; i++){
		sumC += sum_C[i];
	}
	for(int i = 0; i < NUM; i++){
		sumD += sum_D[i];
	}
	// output
    cout << sumC << '\n' << sumD << endl;

    return 0;
}