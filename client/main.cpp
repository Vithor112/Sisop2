#include "sock.hpp"

#include "packet_generated.h"

#include <iostream>
#include <memory>

#define IP "127.0.0.1"
#define PORT "20001"

/*
void echo(std::unique_ptr<net::Socket> s){
	char buff[256];
	flatbuffers::FlatBufferBuilder builder;
	builder.Clear();
	int seqn = 0;

	auto connect = Net::CreateConnect(builder, builder.CreateString("Meu Nome"));
	auto packet = Net::CreatePacket(builder, Net::Operation_Connect, connect.Union());
	builder.FinishSizePrefixed(packet);

	int sent_bytes = s->send(builder.GetBufferPointer(), builder.GetSize());

	if(sent_bytes != builder.GetSize()){
		std::cerr << "n enviou tudo ;-;" << std::endl;
		exit(2);
	}

	std::cout << "Enviou " << builder.GetSize() << " bytes " << std::endl;

	int read_bytes = s->read(buff, 256);

	auto expected_msg_size = flatbuffers::GetSizePrefixedBufferLength((u_int8_t*) buff);
	if(read_bytes != expected_msg_size){
		std::cerr << "pacote ainda n foi recebido inteiro" << std::endl;
		exit(2);
	}

	std::cout << "recebeu " << expected_msg_size << " bytes " << std::endl;

	auto msg = Net::GetSizePrefixedPacket(buff);

	if(msg->op_type() != Net::Operation_Response){
		std::cerr << "q operação eh ent?" << std::endl;
		exit(2);
	}		
	auto payload = static_cast<const Net::Response*>(msg->op());
	std::cout << "Reposta legal " << payload->msg()->str() << std::endl;
}
*/

void basic_op(std::unique_ptr<net::Socket> socket){
	std::string nome("pedro");
	try{
		socket->send_connect(nome);
		auto response = socket->read_operation();

		if(response->operation_type != Net::Operation_Response){
			std::cerr << "erro no teste, operação enviada errada" << std::endl;
			exit(2);
		}
		std::cout << "reposta do servidor: " << response->payload.response.msg << std::endl;
	}catch(const net::TransmissionException& e){
		std::cerr << "erro na execução das funções " << std::endl;
	}
}


int main() {
	net::ClientSocket socket;

	try{
		socket.connect(IP, PORT);
	}
	catch(const net::NetworkException& e){
		std::cerr << e.what() << '\n';
		exit(1);
	}

	basic_op(socket.build());
    return 0;
}

