c++ -o server ReadUSB.cpp JSONServer.cpp -I ./boost_1_60_0
/dev/ttyACM0
c++ -o server ReadUSB.cpp JSONServer.cpp -I ./boost_1_60_0 -lpthread
./server /dev/ttyACM0 3001
c++ -o server ReadUSB.cpp JSONServer.cpp -I /media/haoran/Storage/penn\ learning/CIT595/Homework\ #4/boost_1_60_0 -lpthread