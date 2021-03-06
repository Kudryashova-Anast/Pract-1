#include <iostream>
#include <cstdlib>//exit() - стандартный функции СИ
#include <cstring>//strcpy(), strlen() - строки СИ
#include <unistd.h>//close()
#include <netinet/in.h>//sockaddr_in and include<sys/socket.h>
#include <arpa/inet.h>//inet_aton()

void Error(const char *why, const int exitCode=1)//функция обработки ошибок
{
std::cerr«why«std::endl;
exit(exitCode);
}



int main(int argc, char **argv)
{
//структура адреса клиента
sockaddr_in* selfAddr=new(sockaddr_in);
selfAddr->sin_family=AF_INET;//интернет-протокол IPv4
selfAddr->sin_port=0; //любой порт на усмотрение ОС
selfAddr->sin_addr.s_addr=0; //все адреса компьютера

//структура адреса сервера
sockaddr_in* remoteAddr=new(sockaddr_in);
remoteAddr->sin_family=AF_INET;
remoteAddr->sin_port=htons(7); //порт, преобразованный в сетевой
remoteAddr->sin_addr.s_addr=inet_addr("82.179.90.12");//адрес,преобразованный  в число


//буфер для передачи и приема данных
char* buf=new char [256];
strcpy(buf,"Are the echo here?"); //копировать в буфер
int msgLen=strlen(buf);//вычислить длину строки

//создать сокет
int mySocket=socket(AF_INET, SOCK_DGRAM,0);//UDP
if (mySocket==-1) {
Error("Error open socket",11);
}

//связать сокет с адресом
int rc=bind(mySocket,(const sockaddr*) selfAddr, sizeof(sockaddr_in));
if (rc==-1) {
Error("Error bind socket with local address",12);
}


//установить соединение
rc=connect(mySocket, (const sockaddr*) remoteAddr, sizeof(sockaddr_in));
if (rc==-1) {
close(mySocket);
Error("Error connect socet with remote addr",13);
}

//передать данные
rc=send(mySocket,buf,msgLen,0);
if (rc==-1) {
close(mySocket);
Error("Error send message",14);
}
std::cout«"We send:"«buf«std::endl;

//принять ответ
rc=recv(mySocket,buf, 256, 0);
if (rc==-1) {
close(mySocket);
Error("Error recieve answer",15);
}
buf[rc]='\0';
std::cout«"We recieve:"«buf«std::endl;

//закрыть сокет
close(mySocket);

delete selfAddr;удалить динамическую структуру
delete remoteAddr;удалить динамическую структуру
delete[] buf;//освободить массив
return 0;


}