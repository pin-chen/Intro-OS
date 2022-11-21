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
		void pushReadyProcess(process &readyProcess);
		int setExecuteProcess();
		bool isInterrupt();
		process getPauseProcess();
		int through1s();
};

SRTF::SRTF(){
	this->isExecute = false;
}

void SRTF::pushReadyProcess(process &readyProcess){
	this->readyQueue.push(readyProcess);
}

int SRTF::setExecuteProcess(){
	if(isExecute) return -1;
	if(readyQueue.size() == 0) return -1;
	this->execute = this->readyQueue.top();
	this->readyQueue.pop();
	this->isExecute = true;
	return 0;
}

int SRTF::through1s(){
	if(!this->isExecute) return -1;
	this->execute.leftTime--;
	return 0;
}

bool SRTF::isInterrupt(){
	if(!this->isExecute) return false;
	if(this->execute.leftTime == 0 || this->execute.leftTime > this->readyQueue.top().leftTime){
		this->isExecute = false;
		return true;
	}
	return false;
}

process SRTF::getPauseProcess(){
	return this->execute;
}

int checkWaitingQueue(const int time, queue<process> &waitingQueue, SRTF &processSchedule){
	for(; waitingQueue.size() > 0;){
		process p = waitingQueue.front();
		if(p.arrivalTime < time){
			return -1;
		}else if(p.arrivalTime == time){
			processSchedule.pushReadyProcess(waitingQueue.front());
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

void simulate(SRTF &processSchedule, queue<process> &waitingQueue, vector<process> &finishState){
	int n = waitingQueue.size();
	for(int t = 0; finishState.size() < n; t++, processSchedule.through1s()){
		checkWaitingQueue(t, waitingQueue, processSchedule);
		if( processSchedule.isInterrupt() ){
			process pauseProcess = processSchedule.getPauseProcess();
			if(pauseProcess.leftTime == 0){
				pauseProcess.finishTime = t;
				finishState.push_back(pauseProcess);
			}else{
				processSchedule.pushReadyProcess(pauseProcess);
			}
		}
		processSchedule.setExecuteProcess();
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
