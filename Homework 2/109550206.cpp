# include <iostream>
# include <thread>
# include <unistd.h>
# define ll unsigned long long
# define int unsigned int
# define MAX 500
# define NUM 3
# define RANGE MAX / (NUM + 1)
using namespace std;

int matA[MAX][MAX];
int matB[MAX][MAX];
int matC[MAX][MAX];
int matD[MAX][MAX] = {};

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
    pthread_exit(NULL);
}

int32_t main(){
    cin.tie(0);
	ios::sync_with_stdio(0);
	int i, j, k;
	//get input
    for (i = 0; i < MAX; i++) {
        for (j = 0; j < MAX; j++) {
            cin >> matA[i][j];
        }
    }
    for (i = 0; i < MAX; i++) {
        for (j = 0; j < MAX; j++) {
            cin >> matB[i][j];
        }
    }
    // Multiplication();
	int *in;
	pthread_t t[NUM];
	for (i = 0; i < NUM; i++) {
		in = new int(i);
		pthread_create(&t[i], NULL, mult, (void*)in );
	}
	for (i = RANGE * (NUM); i < MAX; i++) {
        for (j = 0; j < MAX; j++) {
            for (k = 0; k < MAX; k++) {
                matD[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
	// Addition();
	for (i = 0; i < MAX; i++) {
        for (j = 0; j < MAX; j++){
            matC[i][j] = matA[i][j] + matB[i][j];
        }   
    }
	// calculate SUM of matC
    ll sum_C = 0, sum_D = 0;
    for (i = 0; i < MAX; i++) {
        for (j = 0; j < MAX; j++)
            sum_C +=  matC[i][j];     
    }
	cout << sum_C << '\n';
	// wait mult
	for (i = 0; i < NUM; i++) {
		pthread_join(t[i],NULL);
	}
	// calculate SUM of matD
    for (i = 0; i < MAX; i++) {
        for (j = 0; j < MAX; j++)
            sum_D += matD[i][j];
    }
    cout << sum_D << '\n';
    return 0;
}
