mkdir target 2>/dev/null
mkdir target/Agents 2>/dev/null
mkdir target/Model 2>/dev/null
g++ -c -fopenmp -std=c++11 -o target/Agents/Agent.o src/Agents/Agent.cpp
g++ -c -fopenmp -std=c++11 -o target/Model/RandomClass.o src/Model/RandomClass.cpp
g++ -c -fopenmp -std=c++11 -o target/UndeadInfection.o src/UndeadInfection.cpp
g++ -c -fopenmp -std=c++11 -o target/Model/Grid.o src/Model/Grid.cpp
g++ -fopenmp  -o UndeadInfection.exe target/UndeadInfection.o target/Model/RandomClass.o target/Model/Grid.o target/Agents/Agent.o
