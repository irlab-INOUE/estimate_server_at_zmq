// 共有メモリから最新の自己位置を読み出し，タイムスタンプと共にget3Durgに送信する．


#include <iostream>
#include "zmq.hpp"

int main(int argc, char *argv[]) {

	// ZMQ setting
	zmq::context_t context(1);
	zmq::socket_t socket(context, zmq::socket_type::req);
	socket.connect("tcp://localhost:5555");


	for (int request_nbr = 0; request_nbr != 10; request_nbr++) {
		zmq::message_t request (5);
		memcpy (request.data (), "Hello", 5);
		std::cout << "Sending Hello " << request_nbr << "..." << std::endl;
		socket.send (request, zmq::send_flags::none);

		//  Get the reply.
		zmq::message_t reply;
		socket.recv (reply, zmq::recv_flags::none);
		std::cout << "Received World " << request_nbr << std::endl;
    }
	/*
	while(true){
		double x;
		std::cout << "x[m] : ";
		std::cin >> x;
		double y;
		std::cout << "y[m] : ";
		std::cin >> y;
		double angle;
		std::cout << "angle[deg] : ";
		std::cin >> angle;

		std::cout << "x : " << x << ", y : " << y << ", angle : " << angle << "\n";

		zmq::message_t req1((void*)&x, sizeof(double), NULL);
		std::cout << req1 << "\n";
		//auto snd1 = socket.send(req1, zmq::send_flags::none);
		socket.send(req1, zmq::send_flags::none);
		zmq::message_t rep1;
		auto rcv1 = socket.recv(rep1, zmq::recv_flags::none);
		std::string msg1 = std::string(static_cast<char*>(rep1.data()), rep1.size());
		std::cout << msg1 << "\n";

		zmq::message_t req2((void*)&y, sizeof(double), NULL);
		auto snd2 = socket.send(req2, zmq::send_flags::none);
		zmq::message_t rep2;
		auto rcv2 = socket.recv(rep2, zmq::recv_flags::none);
		//std::string rplX = std::string(static_cast<char*>(rcv_msgX.data()), rcv_msgX.size());

		zmq::message_t req3((void*)&angle, sizeof(double), NULL);
		auto snd3 = socket.send(req3, zmq::send_flags::none);
		zmq::message_t rep3;
		auto rcv3 = socket.recv(rep3, zmq::recv_flags::none);
		//std::string rplX = std::string(static_cast<char*>(rcv_msgX.data()), rcv_msgX.size());
	}
	*/

	return 0;
}

