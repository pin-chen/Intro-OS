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
		int stopTime;
		void setStopTime(const int time);
	public:
		RR(int timeQuantum);
		int checkWatingQueue(const int time, queue<process> &waitingQueue);
		int pushReadyProcess(process &readyProcess);
		int setExecuteProcess(const int time);
		bool isInterrupt(const int time);
		process getPauseProcess();
		bool isExecuting();
		bool preempt();
};

RR::RR(int timeQuantum){
	this->timeQuantum = timeQuantum;
	this->isExecute = false;
}

void RR::setStopTime(const int time){
	this->stopTime = time + this->timeQuantum;
}

int RR::checkWatingQueue(const int time, queue<process> &waitingQueue){
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

int RR::pushReadyProcess(process &readyProcess){
	this->readyQueue.push(readyProcess);
	return 0;
}

int RR::setExecuteProcess(const int time){
	if(this->isExecute) return -1;
	if(this->readyQueue.size() == 0) return -1;
	this->execute = this->readyQueue.front();
	this->readyQueue.pop();
	this->setStopTime(time);
	this->isExecute = true;
	return 0;
}

bool RR::isInterrupt(const int time){
	if(!this->isExecute) return false;
	this->execute.leftTime--;
	if(this->execute.leftTime == 0){
		this->execute.finishTime = time;
		this->isExecute = false;
		return true;
	}else if(this->stopTime == time){
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

class FCFS{
	private:
		queue<process> readyQueue;
		process execute;
		bool isExecute;
	public:
		FCFS();
		void pushReadyProcess(process &readyProcess);
		int setExecuteProcess();
		bool isInterrupt(const int time);
		process getPauseProcess();
		void bePreempted();
};

FCFS::FCFS(){
	this->isExecute = false;
}

void FCFS::pushReadyProcess(process &readyProcess){
	this->readyQueue.push(readyProcess);
}

int FCFS::setExecuteProcess(){
	if(this->isExecute) return -1;
	if(this->readyQueue.size() == 0) return -1;
	this->execute = this->readyQueue.front();
	this->readyQueue.pop();
	this->isExecute = true;
	return 0;
}

bool FCFS::isInterrupt(const int time){
	if(!this->isExecute) return false;
	this->execute.leftTime--;
	if(this->execute.leftTime == 0){
		this->execute.finishTime = time;
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
};

void simulate(RR &processSchedule1, FCFS &processSchedule2, queue<process> &waitingQueue, vector<process> &finishState){
	int n = waitingQueue.size();
	for(int t = 0; finishState.size() < n; t++){
		processSchedule1.checkWatingQueue(t, waitingQueue);
		if( processSchedule1.isInterrupt(t) ){
			process pauseProcess = processSchedule1.getPauseProcess();
			if(pauseProcess.leftTime == 0){
				finishState.push_back(pauseProcess);
			}else{
				processSchedule2.pushReadyProcess(pauseProcess);
			}
		}else if( processSchedule2.isInterrupt(t) ){
			process pauseProcess = processSchedule2.getPauseProcess();
			finishState.push_back(pauseProcess);
		}else if( processSchedule1.preempt() ){
			processSchedule2.bePreempted();
		}
		processSchedule1.setExecuteProcess(t);
		if( !processSchedule1.isExecuting() ){
			processSchedule2.setExecuteProcess();
		}
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