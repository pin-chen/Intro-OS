#!/bin/env bash
read P1W P1T
read P2W P2T
read P3W P3T
read TW
read TT
echo -e "Process\t\tWaiting Time\tTurnaround Time"
echo -e "P[1]\t\t$P1W\t\t$P1T"
echo -e "P[2]\t\t$P2W\t\t$P2T"
echo -e "P[3]\t\t$P3W\t\t$P3T"
echo "Total Waiting Time: $TW"
echo "Total Turnaround Time: $TT"
