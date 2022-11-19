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

struct cmp2{
	bool operator()(const struct process &x, const struct process &y){
		return x.leftTime > y.leftTime;
	}
};

bool cmp3(const struct process &x, const struct process &y){
	return x.id < y.id;
}

int main(){
	int n;
	cin >> n;
	vector<process> p(n);
	for(int i = 0; i < n; i++) p[i].id = i;
	for(int i = 0; i < n; i++) cin >> p[i].arrivalTime;
	for(int i = 0; i < n; i++) cin >> p[i].burstTime;
	for(int i = 0; i < n; i++) p[i].leftTime = p[i].burstTime;
	sort(p.begin(), p.end(), cmp1);
	priority_queue<process, vector<process>, cmp2> readyState;
	vector<process> finishState;
	int ready = 0;
	for(int t = 0; finishState.size() < n; t++){
		if(p[ready].arrivalTime == t){
			readyState.push(p[ready]);
			ready++;
		}
		if(readyState.size() <= 0) continue;
		process excute = readyState.top();
		readyState.pop();
		excute.leftTime--;
		if(excute.leftTime == 0){
			excute.finishTime = t + 1;
			finishState.push_back(excute);
		}else{
			readyState.push(excute);
		}
	}
	sort(finishState.begin(), finishState.end(), cmp3);	
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
