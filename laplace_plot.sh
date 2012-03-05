#!/bin/bash
#This script passes arguments to a binary, generates a pdf, and launches evince.
#This script uses default values if no arguments are provided.
#Usage with defaults:
# ./laplace_plot.sh
#Usage with arguments:
#./laplace_plot.sh 230 100 .05 30 90 10

NX=${1:-330}
NY=${2:-70}
EPS=${3:-0.05}
FX=${4:-30}
FY=${5:-55}
FG=${6:-30}

mkdir ./out

./bin/laplace -nx $NX -ny $NY -eps $EPS -fx $FX -fy $FY -fg $FG > "./out/output.csv"

R --no-save << EOT
A = scan(file="./out/output.csv",sep=",")
A = matrix(A,ncol=($NX*$NY),nrow=3)
A = t(A)
par(pty="s")
palette(rainbow(384,end=.75))
plot(A[,2],A[,1],col=A[,3],xlab="X",ylab="Y",main="Laplace Project",pch=" ")
points(A[,2],A[,1],col=A[,3])
quit("no")
EOT

#if evince is not installed, use another program to open Rplots.pdf
evince "./Rplots.pdf"
