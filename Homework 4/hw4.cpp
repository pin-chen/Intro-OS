#include <iostream>
#include <vector>
#define pii pair<int,int>
#define pip pair<int,pii>
#define f first
#define s second

using namespace std;

void FIFO(const int num, const vector<int>&in){
    int ans = 0;
    vector<pii> frame;
    for(int i = 0; i < num; i++) frame.push_back({-1, -1});
    for(int i = 0; i < in.size(); i++){
        bool isExists = false;
        int cur = in[i], minIn = in.size(), minIndex = -1;
        for(int j = 0; j < num; j++){
            if(frame[j].f == cur){
                isExists = true;
                break;
            } 
            if(frame[j].s < minIn){
                minIn = frame[j].s;
                minIndex = j;
            }
        }
        if(isExists) continue;
        ans++;
        frame[minIndex] = {cur, i};
    }
    cout << ans << '\n';
}

void LRU(const int num, const vector<int>&in){
    int ans = 0;
    vector<pii> frame;
    for(int i = 0; i < num; i++) frame.push_back({-1, -1});
    for(int i = 0; i < in.size(); i++){
        bool isExists = false;
        int cur = in[i], minIn = in.size(), minIndex = -1, existsIndex = -1;
        for(int j = 0; j < num; j++){
            if(frame[j].f == cur){
                existsIndex = j;
                isExists = true;
                break;
            } 
            if(frame[j].s < minIn){
                minIn = frame[j].s;
                minIndex = j;
            }
        }
        if(isExists){
            frame[existsIndex].s = i;
            continue;
        } 
        ans++;
        frame[minIndex] = {cur, i};
    }
    cout << ans << '\n';
}

void LFU(const int num, const vector<int>&in){
    int ans = 0;
    vector<pip> frame;
    for(int i = 0; i < num; i++) frame.push_back({-1, {0, -1}});
    for(int i = 0; i < in.size(); i++){
        bool isExists = false;
        int cur = in[i], freq = in.size(), minIn = in.size(), minIndex = -1, existsIndex = -1;
        for(int j = 0; j < num; j++){
            if(frame[j].f == cur){
                existsIndex = j;
                isExists = true;
                break;
            }
            if(frame[j].s.f < freq ){
                freq = frame[j].s.f;
                minIn = frame[j].s.s;
                minIndex = j;
            }else if(frame[j].s.f == freq){
                if(frame[j].s.s < minIn){
                    minIn = frame[j].s.s;
                    minIndex = j;
                }
            }
        }
        if(isExists){
            frame[existsIndex].s.f++;
            continue;
        } 
        ans++;
        frame[minIndex] = {cur, {1, i}};
    }
    cout << ans;
}

int main(){
    int numFrame, numRefStr;
    vector<int> refStr;
    cin >> numFrame >> numRefStr;
    refStr.resize(numRefStr);
    for(int i = 0; i < numRefStr; i++){
        cin >> refStr[i];
    }
    FIFO(numFrame, refStr);
    LRU(numFrame, refStr);
    LFU(numFrame, refStr);
    return 0;
}