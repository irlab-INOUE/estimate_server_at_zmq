#include <iostream>
#include "zmq.hpp"
#include "GetUrg3d.h"

int main(int argc, char *argv[]) {

	// ZMQ setting
	zmq::context_t context(1);
	zmq::socket_t socket(context, zmq::socket_type::req);
	socket.connect("tcp://localhost:5555");
	std::cout << "Start entrance server.\n";

	// 3D-LiDAR setting
	std::string addr = "172.16.20.214";
	long port = 10904;
	GetUrg3d urg3d(addr, port);
	if(urg3d.initUrg3d() == -1) {
		std::cerr << "Error: Can't open 3D-LiDAR. Stoped." << std::endl;
		return -1;
	}

	while(true){
		// loc input
		std::cout << "Please enter location.\n";
		double x;
		std::cout << "x[m] : ";
		std::cin >> x;
		double y;
		std::cout << "y[m] : ";
		std::cin >> y;
		double angle;
		std::cout << "angle[deg] : ";
		std::cin >> angle;

		//std::cout << "x : " << x << ", y : " << y << ", angle : " << angle << "\n";


		// 自己位置を送信
		std::cout << "Sending location...\n";
		zmq::message_t reqX((void*)&x, sizeof(double), NULL);
		auto sndX = socket.send(reqX, zmq::send_flags::none);
		zmq::message_t repX;
		auto rcvX = socket.recv(repX, zmq::recv_flags::none);

		zmq::message_t reqY((void*)&y, sizeof(double), NULL);
		auto sndY = socket.send(reqY, zmq::send_flags::none);
		zmq::message_t repY;
		auto rcvY = socket.recv(repY, zmq::recv_flags::none);

		zmq::message_t reqA((void*)&angle, sizeof(double), NULL);
		auto sndA = socket.send(reqA, zmq::send_flags::none);
		zmq::message_t repA;
		auto rcvA = socket.recv(repA, zmq::recv_flags::none);


		// LiDARデータを送信
		std::cout << "Sending LiDAR data...\n\n";
		std::vector<pointUrg3d> data = urg3d.get1Frame();
		// y, z は-符号をつけることで3D-LiDARの実装と同じ向きになる
		// この段階では逆さまの状態で値を保存している

		size_t size = data.size();
		zmq::message_t msg_data_size((void*)&size, sizeof(size_t), NULL);
		auto res = socket.send(msg_data_size, zmq::send_flags::none);
		zmq::message_t rcv_msg_data_size;
		res = socket.recv(msg_data_size, zmq::recv_flags::none);

		for (int i = 0; i < data.size(); i++) {
			zmq::message_t msg((void*)&data[i], sizeof(pointUrg3d), NULL);
			auto recv = socket.send(msg, zmq::send_flags::none);

			zmq::message_t rcv_msg;
			auto recv2 = socket.recv(rcv_msg, zmq::recv_flags::none);
			std::string rpl = std::string(static_cast<char*>(rcv_msg.data()), rcv_msg.size());
		}
	}
	return 0;
}

