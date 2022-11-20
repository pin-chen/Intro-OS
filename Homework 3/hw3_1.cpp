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

bool compareWithArrivalTime(const struct process &x, const struct process &y){
	return x.arrivalTime < y.arrivalTime;
}

bool compareWithID(const struct process &x, const struct process &y){
	return x.id < y.id;
}

struct cmp{
	bool operator()(const struct process &x, const struct process &y){
		if(x.leftTime == y.leftTime) return x.arrivalTime > y.arrivalTime;
		return x.leftTime > y.leftTime;
	}
};

class SRTF{
	private:
		priority_queue<process, vector<process>, cmp> readyQueue;
		process execute;
		bool isExecute;
	public:
		SRTF();
		int checkWatingQueue(const int time, queue<process> &waitingQueue);
		void pushReadyProcess(process &readyProcess);
		int setExecuteProcess(const int time);
		bool isInterrupt(const int time);
		process getPauseProcess();
};

SRTF::SRTF(){
	this->isExecute = false;
}

int SRTF::checkWatingQueue(const int time, queue<process> &waitingQueue){
	for(;;){
		process p = waitingQueue.front();
		if(p.arrivalTime < time){
			return -1;
		}else if(p.arrivalTime == time){
			this->pushReadyProcess(p);
			waitingQueue.pop();
		}else{
			break;
		}
	}
	return 0;
}

void SRTF::pushReadyProcess(process &readyProcess){
	this->readyQueue.push(readyProcess);
}

int SRTF::setExecuteProcess(const int time){
	if(isExecute) return -1;
	if(readyQueue.size() == 0) return -1;
	this->execute = this->readyQueue.top();
	this->readyQueue.pop();
	this->isExecute = true;
	return 0;
}

bool SRTF::isInterrupt(const int time){
	if(!this->isExecute) return false;
	this->execute.leftTime--;
	if(this->execute.leftTime == 0){
		this->execute.finishTime = time;
		this->isExecute = false;
		return true;
	}else if(this->execute.leftTime > this->readyQueue.top().leftTime){
		this->isExecute = false;
		return true;
	}
	return false;
}

process SRTF::getPauseProcess(){
	return this->execute;
}

void input(queue<process> &waitingQueue){
	int n;
	cin >> n;
	vector<process> p(n);
	for(int i = 0; i < n; i++) p[i].id = i;
	for(int i = 0; i < n; i++) cin >> p[i].arrivalTime;
	for(int i = 0; i < n; i++) cin >> p[i].burstTime;
	for(int i = 0; i < n; i++) p[i].leftTime = p[i].burstTime;
	sort(p.begin(), p.end(), compareWithArrivalTime);
	for(int i = 0; i < n; i++) waitingQueue.push(p[i]);
}

void output(vector<process> &p){
	sort(p.begin(), p.end(), compareWithID);
	int waitingTime, turnaroundTime, totalWaitingTime = 0, totalTurnaroundTime = 0;
	for(int i = 0; i < p.size(); i++){
		turnaroundTime = p[i].finishTime - p[i].arrivalTime;
		waitingTime = turnaroundTime - p[i].burstTime;
		totalWaitingTime += waitingTime;
		totalTurnaroundTime += turnaroundTime;
		cout << waitingTime << " " << turnaroundTime << '\n';
	}
	cout << totalWaitingTime << '\n' << totalTurnaroundTime << '\n';
}

void simulate(SRTF &processSchedule, queue<process> &waitingQueue, vector<process> &finishState){
	int n = waitingQueue.size();
	for(int t = 0; finishState.size() < n; t++){
		processSchedule.checkWatingQueue(t, waitingQueue);
		if( processSchedule.isInterrupt(t) ){
			process pauseProcess = processSchedule.getPauseProcess();
			if(pauseProcess.leftTime == 0){
				finishState.push_back(pauseProcess);
			}else{
				processSchedule.pushReadyProcess(pauseProcess);
			}
		}
		processSchedule.setExecuteProcess(t);
	}
}

int main(){
	queue<process> waitingQueue;
	input(waitingQueue);
	SRTF processSchedule;
	vector<process> finishState;
	simulate(processSchedule, waitingQueue, finishState);
	output(finishState);
	return 0;
}
