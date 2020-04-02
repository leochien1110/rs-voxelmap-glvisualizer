#ifndef MAPPER_H
#define MAPPER_H

#include <iostream>
#include <iomanip>
#include <string.h>
#include <math.h>
#include <thread>
#include <mutex>

#include "rs_camera.h"
#include "data.h"

class Mapper
{
public:
    Mapper(float3 *, int *, float *, float *, float [3][3], int, int);
    ~Mapper();

    //Voxel map size
    const float block_unit_m = 0.2;	//(meter/cell)
    const int unit_length_x = 5;	//(pixel/cell)
    const int unit_length_y = 5;	//(pixel/cell)
    const int unit_length_z = 5;	//(pixel/cell)
    float block_unit = block_unit_m;
    float unit_length = unit_length_x;

    //Map scale in real world (pixel per meter)..
    //can use to transfer map from real world to voxel world
    const float mapscale_x = unit_length_x / block_unit_m;	//
    const float mapscale_y = unit_length_y / block_unit_m;
    const float mapscale_z = unit_length_z / block_unit_m;	//
    float camera_scaled_pose[3] = { 0 };
    
    //static const int grid_x = 100;
    //static const int grid_y = 30;
    //static const int grid_z = 100;

    unsigned char voxelmap[grid_x][grid_y][grid_z];
    unsigned char voxelmap_old[grid_x][grid_y][grid_z];
    bool initial_voxelmap[grid_x][grid_y][grid_z];

    // ray tracing parameter
    double coarray[3];

    // Reduced resolution
    static const int reduced_ratio = 8;	//1,2,4,8,10,16,20
    const int reduced_w = width / reduced_ratio;
    const int reduced_h = height / reduced_ratio;
    const int reduced_res = reduced_w * reduced_h;
    const int reduced_ratio_square = reduced_ratio * reduced_ratio;
    int si, sj, sn, ri, rj;
    
    // Map shift para
    int mps = 0;

    // func
    void start();
    void stream();
    void reduce_resolution();
    void ray_tracing();
    void render_vmap(bool, int [], float, int, bool, bool);
    void shift_map(int);
    void stop();
    bool mapper_status;
    
private:
    // float
    
    float *camera_pose;
    float *specific_point;
    float (*inv_C)[3];
    //float inv_C[3][3];
    //vertices;
    int *pc_points;
    float3 * pc_vertices;
    float3 *reduced_vertices;

    //Threading
    std::mutex mutex;
    std::thread streamThread;

    // Class
    RS_Camera rs_camera;
};


#endif //MAPPER_H