//
// Created by 包洋斌 on 2024/3/2.
//

#ifndef ODISTANCE_ODCALC_H
#define ODISTANCE_ODCALC_H

#define pi 3.1415926535
typedef struct TransMatrix {
    double R1_1,R1_2,R1_3,P1_4;
    double R2_1,R2_2,R2_3,P2_4;
    double R3_1,R3_2,R3_3,P3_4;

}TransMatrix;

void TM_calc(struct TransMatrix* TM,float degree[]);

#endif //ODISTANCE_ODCALC_H
