// estimateから推論結果を受信したら，特定材質がないか判定・エラー表示し，（何を？）共有メモリに書き込む．


#include <iostream>
#include "zmq.hpp"
 
int main(int argc, char *argv[]) {
	// ZMQ setting
	zmq::context_t context(1);
	zmq::socket_t socket(context, zmq::socket_type::rep);
	socket.connect("tcp://localhost:5557");


	while(true){
		zmq::message_t req1;
		auto rcv1 = socket.recv(req1, zmq::recv_flags::none);
		std::string num = std::string(static_cast<char*>(req1.data()), req1.size());
		int data1 = 0;
		zmq::message_t rep1((void*)&data1, sizeof(int), NULL);
		auto snd1 = socket.send(rep1, zmq::send_flags::none);
		zmq::message_t req2;
		auto rcv2 = socket.recv(req2, zmq::recv_flags::none);
		std::string result = std::string(static_cast<char*>(req2.data()), req2.size());
		int data2 = 0;
		zmq::message_t rep2((void*)&data2, sizeof(int), NULL);
		auto snd2 = socket.send(rep2, zmq::send_flags::none);
		std::cout << "num : " << num << ", result : " << result << ", ";

		if(result == "1"){
			std::cout << "-----\n";
		}

		if(result == "0"){
			std::cout << "ERROR\n";
		}
	}

	return 0;
}
