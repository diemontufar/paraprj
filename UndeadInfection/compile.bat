@echo off
echo "Compiling UndeadInfection..."
MKDIR target
MKDIR build
CD src
g++ -I/user/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fopenmp -o "../target/Grid.o" "Model/Grid.cpp" 
g++ -I/user/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fopenmp -o "../target/UndeadInfection.o" "UndeadInfection.cpp" 
g++ -I/user/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fopenmp -o "../target/Agent.o" "Agents/Agent.cpp" 
g++ -I/user/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fopenmp -o "../target/RandomClass.o" "Model/RandomClass.cpp" 
g++ -std=c++11 -fopenmp -o "../build/UndeadInfection.exe" "../target/UndeadInfection.o" "../target/RandomClass.o" "../target/Grid.o" "../target/Agent.o" 
pause