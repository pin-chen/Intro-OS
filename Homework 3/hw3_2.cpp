#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#define FINISH 0
#define TIMEUP 1
#define NOTYET 2
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

class RR{
	private:
		queue<process> readyQueue;
		process excute;
		bool isExcute;
		int timeQuantum;
		int stopTime;
		void setStopTime(const int time);
	public:
		RR(int timeQuantum);
		int checkWatingQueue(const int time, queue<process> &waitingQueue);
		int pushReadyProcess(process &readyProcess);
		int setExcuteProcess(const int time);
		bool isInterrupt(const int time);
		process getPauseProcess();
};

RR::RR(int timeQuantum){
	this->timeQuantum = timeQuantum;
	this->isExcute = false;
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

int RR::setExcuteProcess(const int time){
	if(isExcute) return -1;
	if(readyQueue.size() == 0) return -1;
	this->excute = this->readyQueue.front();
	this->readyQueue.pop();
	this->setStopTime(time);
	this->isExcute = true;
	return 0;
}

bool RR::isInterrupt(const int time){
	if(!this->isExcute) return false;
	this->excute.leftTime--;
	if(this->excute.leftTime == 0){
		this->excute.finishTime = time;
		this->isExcute = false;
		return true;
	}else if(this->stopTime >= time){
		this->isExcute = false;
		return true;
	}
	return false;
}

process RR::getPauseProcess(){
	return excute;
}

void input(queue<process> &waitingQueue){
	int n;
	cin >> n;
	vector<process> p(n);
	for(int i = 0; i < n; i++) p[i].id = i;
	for(int i = 0; i < n; i++) cin >> p[i].arrivalTime;
	for(int i = 0; i < n; i++) cin >> p[i].burstTime;
	for(int i = 0; i < n; i++) p[i].leftTime = p[i].burstTime;
	sort(p.begin(), p.end(), cmp1);
	for(int i = 0; i < n; i++) waitingQueue.push(p[i]);
}

void output(vector<process> &p){
	sort(p.begin(), p.end(), cmp2);
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

void simulate(RR &processSchedule, queue<process> &waitingQueue, vector<process> &finishState){
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
		processSchedule.setExcuteProcess(t);
	}
}

int main(){
	int timeQuantum;
	queue<process> waitingQueue;
	input(waitingQueue);
	cin >> timeQuantum;
	RR processSchedule(timeQuantum);
	vector<process> finishState;
	simulate(processSchedule, waitingQueue, finishState);
	output(finishState);
	return 0;
}