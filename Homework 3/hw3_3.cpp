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

class RR{
	private:
		queue<process> readyQueue;
		process execute;
		bool isExecute;
		int timeQuantum;
		int leftQuantum;
		void setLeftQuantum();
	public:
		RR(int timeQuantum);
		int pushReadyProcess(process &readyProcess);
		bool setExecuteProcess();
		bool isInterrupt();
		process getPauseProcess();
		bool isExecuting();
		bool preempt();
		int through1s();
};

RR::RR(int timeQuantum){
	this->timeQuantum = timeQuantum;
	this->isExecute = false;
}

void RR::setLeftQuantum(){
	this->leftQuantum = this->timeQuantum;
}

int RR::pushReadyProcess(process &readyProcess){
	this->readyQueue.push(readyProcess);
	return 0;
}

bool RR::setExecuteProcess(){
	if(this->isExecute) return 1;
	if(this->readyQueue.size() == 0) return 0;
	this->execute = this->readyQueue.front();
	this->readyQueue.pop();
	this->setLeftQuantum();
	this->isExecute = true;
	return 1;
}

bool RR::isInterrupt(){
	if(!this->isExecute) return false;
	if(this->execute.leftTime == 0 || this->leftQuantum == 0){
		this->isExecute = false;
		return true;
	}
	return false;
}

process RR::getPauseProcess(){
	return this->execute;
}

bool RR::isExecuting(){
	return this->isExecute;
}

bool RR::preempt(){
	return this->readyQueue.size() != 0 && !this->isExecute;
}

int RR::through1s(){
	if(!this->isExecute) return -1;
	this->execute.leftTime--;
	this->leftQuantum--;
	return 0;
}

class FCFS{
	private:
		queue<process> readyQueue;
		process execute;
		bool isExecute;
	public:
		FCFS();
		void pushReadyProcess(process &readyProcess);
		bool setExecuteProcess();
		bool isInterrupt();
		process getPauseProcess();
		void bePreempted();
		int through1s();
};

FCFS::FCFS(){
	this->isExecute = false;
}

void FCFS::pushReadyProcess(process &readyProcess){
	this->readyQueue.push(readyProcess);
}

bool FCFS::setExecuteProcess(){
	if(this->isExecute) return 1;
	if(this->readyQueue.size() == 0) return 0;
	this->execute = this->readyQueue.front();
	this->readyQueue.pop();
	this->isExecute = true;
	return 1;
}

bool FCFS::isInterrupt(){
	if(!this->isExecute) return false;
	if(this->execute.leftTime == 0){
		this->isExecute = false;
		return true;
	}
	return false;
}

process FCFS::getPauseProcess(){
	return this->execute;
}

void FCFS::bePreempted(){
	if(!this->isExecute) return;
	this->isExecute = false;
	this->readyQueue.push(execute);
}

int FCFS::through1s(){
	if(!this->isExecute) return -1;
	this->execute.leftTime--;
	return 0;
}

int checkWatingQueue(const int time, queue<process> &waitingQueue, RR &processSchedule1){
	for(; waitingQueue.size() > 0;){
		process p = waitingQueue.front();
		if(p.arrivalTime < time){
			return -1;
		}else if(p.arrivalTime == time){
			processSchedule1.pushReadyProcess(p);
			waitingQueue.pop();
		}else{
			break;
		}
	}
	return 0;
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

void simulate(RR &processSchedule1, FCFS &processSchedule2, queue<process> &waitingQueue, vector<process> &finishState){
	int n = waitingQueue.size();
	for(int t = 0; finishState.size() < n; t++, processSchedule1.through1s(), processSchedule2.through1s()){
		checkWatingQueue(t, waitingQueue, processSchedule1);
		if( processSchedule1.isInterrupt() ){
			process pauseProcess = processSchedule1.getPauseProcess();
			if(pauseProcess.leftTime == 0){
				pauseProcess.finishTime = t;
				finishState.push_back(pauseProcess);
			}else{
				processSchedule2.pushReadyProcess(pauseProcess);
			}
		}else if( processSchedule2.isInterrupt() ){
			process pauseProcess = processSchedule2.getPauseProcess();
			pauseProcess.finishTime = t;
			finishState.push_back(pauseProcess);
		}else if( processSchedule1.preempt() ){
			processSchedule2.bePreempted();
		}
		processSchedule1.setExecuteProcess() || processSchedule2.setExecuteProcess();
	}
}

int main(){
	int timeQuantum;
	queue<process> waitingQueue;
	input(waitingQueue);
	cin >> timeQuantum;
	RR processSchedule1(timeQuantum);
	FCFS processSchedule2;
	vector<process> finishState;
	simulate(processSchedule1, processSchedule2, waitingQueue, finishState);
	output(finishState);
	return 0;
}