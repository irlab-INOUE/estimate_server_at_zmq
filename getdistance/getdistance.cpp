// entranceから自己位置を受け取ったら，タイムスタンプを参照して対応するLiDARデータを自己位置とセットでestimateに送信する．

#include <iostream>
#include "zmq.hpp"
#include "GetUrg3d.h"

int main(int argc, char *argv[]) {

	// ZMQ setting
	zmq::context_t ctx1(1);
	zmq::socket_t sock1(ctx1, zmq::socket_type::rep);
	sock1.connect("tcp://localhost:5555");

	zmq::context_t ctx2(1);
	zmq::socket_t sock2(ctx2, zmq::socket_type::req);
	sock2.connect("tcp://localhost:5556");

	// 3D-LiDAR setting
	std::string addr = "172.16.20.214";
	long port = 10904;
	GetUrg3d urg3d(addr, port);
	if(urg3d.initUrg3d() == -1) {
		std::cerr << "Error: Can't open 3D-LiDAR. Stoped." << std::endl;
		return -1;
	}

	while(true){
		/*
		// 自己位置を受信するまで待機
		double tmp = 1.0;

		zmq::message_t req1x;
		auto rcv1x = sock1.recv(req1x, zmq::recv_flags::none);
		std::string msg1x = std::string(static_cast<char*>(req1x.data()), req1x.size());
		zmq::message_t rep1x((void*)&tmp, sizeof(double), NULL);
		//sock1.send(rep1x, zmq::send_flags::none);
		auto snd1x = sock1.send(rep1x, zmq::send_flags::none);

		zmq::message_t req1y;
		auto rcv1x = sock1.recv(req1y, zmq::recv_flags::none);
		std::string msg1y = std::string(static_cast<char*>(req1y.data()), req1y.size());
		zmq::message_t rep1y((void*)&tmp, sizeof(double), NULL);
		//sock1.send(rep1y, zmq::send_flags::none);
		auto snd1y = sock1.send(rep1y, zmq::send_flags::none);

		zmq::message_t req1a;
		auto rcv1a = sock1.recv(req1a, zmq::recv_flags::none);
		std::string msg1a = std::string(static_cast<char*>(req1a.data()), req1a.size());
		zmq::message_t rep1a((void*)&tmp, sizeof(double), NULL);
		//sock1.send(rep1a, zmq::send_flags::none);
		auto snd1a = sock1.send(rep1a, zmq::send_flags::none);

		std::cout << "x = " << msg1 << ", y = " << msg2 << ", angle = " << msg3 << "\n";
		*/


		// 自己位置を送信
		double msg1x = 0.0;
		double msg1y = 0.0;
		double msg1a = 0.0;
		zmq::message_t req2x((void*)&msg1x, sizeof(double), NULL);
		auto snd2x = sock2.send(req2x, zmq::send_flags::none);
		zmq::message_t rep2x;
		auto rcv2x = sock2.recv(rep2x, zmq::recv_flags::none);

		zmq::message_t req2y((void*)&msg1y, sizeof(double), NULL);
		auto snd2y = sock2.send(req2y, zmq::send_flags::none);
		zmq::message_t rep2y;
		auto rcv2y = sock2.recv(rep2y, zmq::recv_flags::none);

		zmq::message_t req2a((void*)&msg1a, sizeof(double), NULL);
		auto snd2a = sock2.send(req2a, zmq::send_flags::none);
		zmq::message_t rep2a;
		auto rcv2a = sock2.recv(rep2a, zmq::recv_flags::none);


		/*
		// LiDARデータを送信
		std::vector<pointUrg3d> data = urg3d.get1Frame();
		// y, z は-符号をつけることで3D-LiDARの実装と同じ向きになる
		// この段階では逆さまの状態で値を保存している

		size_t size = data.size();
		zmq::message_t msg_data_size((void*)&size, sizeof(size_t), NULL);
		auto res = sock2.send(msg_data_size, zmq::send_flags::none);
		zmq::message_t rcv_msg_data_size;
		res = sock2.recv(msg_data_size, zmq::recv_flags::none);

		for (int i = 0; i < data.size(); i++) {
			zmq::message_t msg((void*)&data[i], sizeof(pointUrg3d), NULL);
			auto recv = sock2.send(msg, zmq::send_flags::none);

			zmq::message_t rcv_msg;
			auto recv2 = sock2.recv(rcv_msg, zmq::recv_flags::none);
			std::string rpl = std::string(static_cast<char*>(rcv_msg.data()), rcv_msg.size());
		}
		*/
	}
	return 0;
}

