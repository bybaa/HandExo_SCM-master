//
// Created by 包洋斌 on 2024/3/2.
//
#include "ODcalc.h"
#include "math.h"

void TM_calc(struct TransMatrix* TM,float degree[])
{
	float d2r = pi/180;
    float t1 = degree[0]*d2r,t2=degree[1]*d2r,t3=degree[2]*d2r,t4=degree[3]*d2r,t5=degree[4]*d2r,t6=degree[5]*d2r;

    TM->R1_1 = cos(t6)*((sqrt(3)*cos(t1 + t2 + t4 + pi/4))/4 + (sqrt(3)*cos(t1 + t2 - t4 + pi/4))/4 + (sin(t1 + t2 + pi/4)*cos(t3)*cos(t4))/2 - sin(t1 + t2 + pi/4)*sin(t3)*sin(t4)) + sin(t6)*(sin(t5)*((sqrt(3)*sin(t1 + t2 + t4 + pi/4))/4 - (sqrt(3)*sin(t1 + t2 - t4 + pi/4))/4 + (sin(t1 + t2 + pi/4)*cos(t3)*sin(t4))/2 + sin(t1 + t2 + pi/4)*cos(t4)*sin(t3)) - cos(t5)*((sqrt(3)*sin(t1 + t2 + t3 + pi/4))/4 - cos(t1 + t2 + pi/4)/2 + (sqrt(3)*sin(t1 + t2 - t3 + pi/4))/4));

    TM->R1_2 = cos(t6)*(sin(t5)*((sqrt(3)*sin(t1 + t2 + t4 + pi/4))/4 - (sqrt(3)*sin(t1 + t2 - t4 + pi/4))/4 + (sin(t1 + t2 + pi/4)*cos(t3)*sin(t4))/2 + sin(t1 + t2 + pi/4)*cos(t4)*sin(t3)) - cos(t5)*((sqrt(3)*sin(t1 + t2 + t3 + pi/4))/4 - cos(t1 + t2 + pi/4)/2 + (sqrt(3)*sin(t1 + t2 - t3 + pi/4))/4)) - sin(t6)*((sqrt(3)*cos(t1 + t2 + t4 + pi/4))/4 + (sqrt(3)*cos(t1 + t2 - t4 + pi/4))/4 + (sin(t1 + t2 + pi/4)*cos(t3)*cos(t4))/2 - sin(t1 + t2 + pi/4)*sin(t3)*sin(t4));

    TM->R1_3 = cos(t5)*((sqrt(3)*sin(t1 + t2 + t4 + pi/4))/4 - (sqrt(3)*sin(t1 + t2 - t4 + pi/4))/4 + (sin(t1 + t2 + pi/4)*cos(t3)*sin(t4))/2 + sin(t1 + t2 + pi/4)*cos(t4)*sin(t3)) + sin(t5)*((sqrt(3)*sin(t1 + t2 + t3 + pi/4))/4 - cos(t1 + t2 + pi/4)/2 + (sqrt(3)*sin(t1 + t2 - t3 + pi/4))/4);
    
    TM->P1_4 = (3559*cos(t1 + t2 + pi/4))/40 - (3653420149411407*sin(t1 + t2 - t3 + pi/4))/140737488355328 - (3653420149411407*sin(t1 + t2 + t3 + pi/4))/140737488355328 + (13669*cos(t1 + (337*pi)/3600))/125;
//////////////////////////////////////////
    TM->R2_1 = (sqrt(3)*cos(t5)*sin(t3)*sin(t6))/2 - (cos(t4)*cos(t6)*sin(t3))/2 - cos(t3)*cos(t6)*sin(t4) + cos(t3)*cos(t4)*sin(t5)*sin(t6) - (sin(t3)*sin(t4)*sin(t5)*sin(t6))/2;

    TM->R2_2 = cos(t3)*sin(t4)*sin(t6) + (cos(t4)*sin(t3)*sin(t6))/2 + (sqrt(3)*cos(t5)*cos(t6)*sin(t3))/2 + cos(t3)*cos(t4)*cos(t6)*sin(t5) - (cos(t6)*sin(t3)*sin(t4)*sin(t5))/2;

    TM->R2_3 = cos(t3)*cos(t4)*cos(t5) - (sqrt(3)*sin(t3)*sin(t5))/2 - (cos(t5)*sin(t3)*sin(t4))/2;

    TM->P2_4 = (3653420149411407*sin(t3))/70368744177664;

    ///////////////////////////////////////////
    TM->R3_1 = sin(t6)*(sin(t5)*((sqrt(3)*cos(t1 + t2 + t4 + pi/4))/4 - (sqrt(3)*cos(t1 + t2 - t4 + pi/4))/4 + (cos(t1 + t2 + pi/4)*cos(t3)*sin(t4))/2 + cos(t1 + t2 + pi/4)*cos(t4)*sin(t3)) - cos(t5)*(sin(t1 + t2 + pi/4)/2 + (sqrt(3)*cos(t1 + t2 + t3 + pi/4))/4 + (sqrt(3)*cos(t1 + t2 - t3 + pi/4))/4)) - cos(t6)*((sqrt(3)*sin(t1 + t2 + t4 + pi/4))/4 + (sqrt(3)*sin(t1 + t2 - t4 + pi/4))/4 - (cos(t1 + t2 + pi/4)*cos(t3)*cos(t4))/2 + cos(t1 + t2 + pi/4)*sin(t3)*sin(t4));

    TM->R3_2 = sin(t6)*((sqrt(3)*sin(t1 + t2 + t4 + pi/4))/4 + (sqrt(3)*sin(t1 + t2 - t4 + pi/4))/4 - (cos(t1 + t2 + pi/4)*cos(t3)*cos(t4))/2 + cos(t1 + t2 + pi/4)*sin(t3)*sin(t4)) + cos(t6)*(sin(t5)*((sqrt(3)*cos(t1 + t2 + t4 + pi/4))/4 - (sqrt(3)*cos(t1 + t2 - t4 + pi/4))/4 + (cos(t1 + t2 + pi/4)*cos(t3)*sin(t4))/2 + cos(t1 + t2 + pi/4)*cos(t4)*sin(t3)) - cos(t5)*(sin(t1 + t2 + pi/4)/2 + (sqrt(3)*cos(t1 + t2 + t3 + pi/4))/4 + (sqrt(3)*cos(t1 + t2 - t3 + pi/4))/4));

    TM->R3_3 = cos(t5)*((sqrt(3)*cos(t1 + t2 + t4 + pi/4))/4 - (sqrt(3)*cos(t1 + t2 - t4 + pi/4))/4 + (cos(t1 + t2 + pi/4)*cos(t3)*sin(t4))/2 + cos(t1 + t2 + pi/4)*cos(t4)*sin(t3)) + sin(t5)*(sin(t1 + t2 + pi/4)/2 + (sqrt(3)*cos(t1 + t2 + t3 + pi/4))/4 + (sqrt(3)*cos(t1 + t2 - t3 + pi/4))/4);

    TM->P3_4 = - (3653420149411407*cos(t1 + t2 - t3 + pi/4))/140737488355328 - (3559*sin(t1 + t2 + pi/4))/40 - (3653420149411407*cos(t1 + t2 + t3 + pi/4))/140737488355328 - (13669*sin(t1 + (337*pi)/3600))/125;
}

