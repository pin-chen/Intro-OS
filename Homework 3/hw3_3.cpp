#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

struct process{
	int id;
	int arrivalTime;
	int burstTime;
	int finishTime;
	int leftTime;
};

bool cmp1(const struct process &x, const struct process &y){
	return x.arrivalTime < y.arrivalTime;
}

bool cmp2(const struct process &x, const struct process &y){
	return x.id < y.id;
}

int main(){
	int n, timeQuantum;
	cin >> n;
	vector<process> p(n);
	for(int i = 0; i < n; i++) p[i].id = i;
	for(int i = 0; i < n; i++) cin >> p[i].arrivalTime;
	for(int i = 0; i < n; i++) cin >> p[i].burstTime;
	for(int i = 0; i < n; i++) p[i].leftTime = p[i].burstTime;
	cin >> timeQuantum;
	sort(p.begin(), p.end(), cmp1);
	int ready = 0;
	vector<process> finishState;
	queue<process> readyQueue;
	queue<process> FCFS;
	process excute;
	int waitQuantum = -1;
	for(int t = 0; finishState.size() < n; t++){
		if(p[ready].arrivalTime == t){
			readyQueue.push(p[ready]);
			ready++;
		}
		if(waitQuantum > 0) waitQuantum--;
		if(waitQuantum >= 0){
			excute.leftTime--;
			if(excute.leftTime == 0){
				excute.finishTime = t;
				finishState.push_back(excute);
				waitQuantum = 0;
			}else if(waitQuantum == 0){
				FCFS.push(excute);
			}
		}else if(waitQuantum <= -10){
			excute.leftTime--;
			if(excute.leftTime == 0){
				excute.finishTime = t;
				finishState.push_back(excute);
				waitQuantum = 0;
			}
		}
		if(waitQuantum <= 0){
			if(readyQueue.size() != 0){
				if(waitQuantum <= -10) FCFS.push(excute);
				excute = readyQueue.front();
				readyQueue.pop();
				waitQuantum = timeQuantum;
			}else if(waitQuantum > -5 && FCFS.size() != 0){
				excute = FCFS.front();
				FCFS.pop();
				waitQuantum = -10;
			}
		}
	}
	sort(finishState.begin(), finishState.end(), cmp2);
	int waitingTime, turnaroundTime, totalWaitingTime = 0, totalTurnaroundTime = 0;
	for(int i = 0; i < n; i++){
		turnaroundTime = finishState[i].finishTime - finishState[i].arrivalTime;
		waitingTime = turnaroundTime - finishState[i].burstTime;
		totalWaitingTime += waitingTime;
		totalTurnaroundTime += turnaroundTime;
		cout << waitingTime << " " << turnaroundTime << '\n';
	}
	cout << totalWaitingTime << '\n' << totalTurnaroundTime << '\n';
	return 0;
}
