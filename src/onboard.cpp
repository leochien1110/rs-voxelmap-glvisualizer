#include "onboard.h"

Onboard::Onboard(std::string gs_ip)
{
    ip = gs_ip;
    std::cout << "ip: " << ip << std::endl;
    onboard_status = true;
    port_num = 7777;

    // Test gobal variables
    std::cout << "ob.dc.specific_row:" << specific_row << std::endl;
    specific_row = 14;
    std::cout << "ob.dc.a:" << Data::a << std::endl;
    Data::a = 444;

    mapper_status = true;
}

Onboard::~Onboard()
{
    std::cout << "Onboard deconstructing..." << std::endl;
    stop();
}

void Onboard::update()
{
    std::cout << "Onboard updating...\n" << std::endl;
    std::cout << "ob.updata.specific_row:" << specific_row << std::endl;
    specific_row = 13;
    std::cout << "ob.updata.a:" << Data::a << std::endl;
    Data::a = 555;
    
    RS_Camera camera;
    Connect data2GS;
    Mapper mapper;
    Scene scene;
    
    // run flight controller

    // run camera
    camera.init();  // config devices and filter
    camera.start();
    //std::cout << camera_pose[0] << " "   \
        << camera_pose[1] << " "         \
        << camera_pose[2] << " "         \
        << " " << std::endl;
    
    // run mapper
    mapper.start();

    // run sock
    data2GS.init(ip, port_num);
    data2GS.start();

    scene.update(); // Press esc in the window to end the program

    /*
    while(camera_stream && mapper_stream && connect_stream && scene_stream)
    {
        // mapper running~~~
        std::cout << "Mapper is Healthy :)" << std::endl;
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(500));
    }
    */

    // Stop program
    camera_stream = false;
    mapper_stream = false;
    connect_stream = false;
    scene_stream = false;
    camera.stop();
    
    scene.stop();
    std::cout << "leaving onboard..." << std::endl;
}

void Onboard::stop()
{
    std::cout << "Onboard stop" << std::endl;
}