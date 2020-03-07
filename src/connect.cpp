#include "connect.h"

Connect::Connect()
{
    printf("Connect Class called!\n");
}
Connect::~Connect()
{

}

void Connect::init(std::string _ip, uint16_t _port_num,     \
                    int _grid_x, int _grid_y, int _grid_z,  \
                    char (*_map)[30][100], float * _camera_pose)
{
    std::cout << "_ip: " << _ip << std::endl;
    ip.assign(_ip);
    port_num = _port_num;

    /*map = new char**[grid_x];
    for(int i = 0; i < grid_x; ++i){
        map[i] = new char*[grid_y];
        for(int j = 0; j < grid_y; ++j){
            map[i][j] = new char[grid_z];
            for(int k = 0; k < grid_z; ++k){
                map[i][j][k] = 0;
            }
        }
    }*/
    //map = _map;
    std::cout << "_map: " << _map << std::endl;
    camera_pose = _camera_pose;

    // convert ip type (string->char[])
    char char_ip[ip.length()+1];
    //std::strcpy(char_ip, ip.c_str()); 
    std::copy(ip.begin(), ip.end(), char_ip);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        printf("Fail to create a socket!\n");
    }
    int nSendBuf=512*1024;//512K
    setsockopt(sockfd,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf, sizeof(int));
    //int set = 1;
    //setsockopt(sockfd, SOL_SOCKET, MSG_NOSIGNAL, (void *)&set, sizeof(int));
    signal(SIGPIPE, SIG_IGN);
    //************************
    bzero(&serverInfo,sizeof(serverInfo));
    serverInfo.sin_family = PF_INET;
    // local host test
    serverInfo.sin_addr.s_addr = inet_addr(char_ip);	//127.0.0.1		104.39.160.46	104.39.89.30
    serverInfo.sin_port = htons(port_num);

    err = connect(sockfd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
    if(err==-1){
        printf("Connection error\n");
        socket_connect = false;
    }
    else{
        printf("Connected!\n");
        socket_connect = true;
    }
}

void Connect::end()
{
    close(sockfd);
    printf("Socket Closed!\n");
}

void Connect::senddata(bool mapper_status)
{
    while(mapper_status){
			if(!socket_connect){
				printf("\033[1A"); //go back to previous row
				printf("\033[K");  //flush
				printf("\033[1A");
				printf("\033[K");
				printf("\033[1A");
        		printf("\033[K");
				end();
				printf("Reinitializing...\n");
				//init();
				std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(100));
			}
			else{
				clock_t t01;
				sendmap();
				sendcam();
				clock_t t04;
				//double duration = t04 - t01;
				//std::cout<< "\r" << "time spend: " << duration << std::flush;
			}
		}
}

void Connect::recvdata()
{
    
}

void Connect::sendmap()
{
    unsigned char buf[grid_x][grid_y][grid_z];
    int buf_size = sizeof(buf);
    //std::lock_guard<std::mutex> mlcok(mutex_t_s2g);
    mutex.lock();
    for (int i = 0; i < grid_x; i++) {
        for (int j = 0; j < grid_y; j++) {
            for (int k = 0; k < grid_z; k++) {
                buf[i][j][k] = map[i][j][k];
            }
        }
    }
    mutex.unlock();
    if(err != -1){
        process_sending(buf, buf_size, 0);
    }
}

void Connect::sendcam()
{
    float buf[9];
    int buf_size = sizeof(buf);
    //std::lock_guard<std::mutex> mlcok(mutex_t_c2g);
    mutex.lock();
    for (int i = 0; i < 9; i++) {
        buf[i]=camera_pose[i];
    }
    mutex.unlock();
    if(err != -1){
        process_sending(buf, buf_size, 1);
    }	
}

int Connect::process_sending(void * sendbuf, int SIZE, bool flag)
{
    if(flag == 0){
        sen = send(sockfd, sendbuf, SIZE, 0);
    }
    else if(flag == 1){
        sen = send(sockfd, sendbuf, SIZE, MSG_DONTWAIT);
    }
    //mutex_t.unlock();
    //std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(1));
    //printf("sen:%i\n",sen);
    if(sen == -1){
        //std::cout << "\r" << "Sending error!!\n" << std::flush;
        printf("Sending error!\n");
        socket_connect = false;
        end();
    }
    else if(sen == 0){
        //std::cout << "\r" << "Sending disconnected!\n" << std::flush;
        printf("Sending disconnected!\n");
        socket_connect = false;
        end();
    }
}

int Connect::process_receiving(void * recvbuf, int SIZE)
{
    ret = recv(Clientsockfd, recvbuf, SIZE, 0);
    if(ret == -1){
        printf("Receiving error!\n");
    }
    
    return ret;
}