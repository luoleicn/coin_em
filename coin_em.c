#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define NUM_INSTANCE    5
#define NUM_SAMPLE      10
#define MAX_EPOCH       1000
#define EPSILON         1e-4

void debugMatrix(float (*m)[2], int , int );

int main(int args, char** argv)
{
    int data_set[NUM_INSTANCE][NUM_SAMPLE] = {
        {1, 0, 0, 0, 1, 1, 0, 1, 0, 1},
        {1, 1, 1, 1, 0, 1, 1, 1, 1, 1}, 
        {1, 0, 1, 1, 1, 1, 1, 0, 1, 1}, 
        {1, 0, 1, 0, 0, 0, 1, 1, 0, 0},
        {0, 1, 1, 1, 0, 1, 1, 1, 0, 1}
    };

    float pa = 0.6, pb=0.5;

    float loglikelyhood[NUM_INSTANCE][2];

    for (int epoch = 0; epoch < MAX_EPOCH; epoch ++) {

        printf("[epoch %2d] pa=%f pb=%f\n", epoch, pa, pb);
        float log_pa = log(pa), log_pb = log(pb);
        float log_n_pa = log(1.0f - pa), log_n_pb = log(1.0f - pb);

        memset(loglikelyhood, 0, sizeof(loglikelyhood));
        //debugMatrix(loglikelyhood, NUM_INSTANCE, 2);

        for (int instance = 0; instance < NUM_INSTANCE; instance ++) {

            for (int sample = 0; sample < NUM_SAMPLE; sample ++) {
                if (data_set[instance][sample]) {
                    loglikelyhood[instance][0] += log_pa;
                    loglikelyhood[instance][1] += log_pb;
                }
                else {
                    loglikelyhood[instance][0] += log_n_pa;
                    loglikelyhood[instance][1] += log_n_pb;
                }
            }
        }

        float head_a = 0, tail_a = 0, head_b = 0, tail_b = 0;
        for (int instance = 0; instance < NUM_INSTANCE; instance ++) {
            float instance_pa = exp(loglikelyhood[instance][0]) / (exp(loglikelyhood[instance][0]) + exp(loglikelyhood[instance][1]));
            float instance_pb = 1.0f - instance_pa;

            for (int sample = 0; sample < NUM_SAMPLE; sample ++) {
                if (data_set[instance][sample]) {
                    head_a += instance_pa;
                    head_b += instance_pb;
                }
                else {
                    tail_a += instance_pa;
                    tail_b += instance_pb;
                }
            }
            printf("instance %2d instance_pa=%f instance_pb=%f\n", instance, instance_pa, instance_pb);
        }
        float new_pa = head_a / (head_a + tail_a);
        float new_pb = head_b / (head_b + tail_b);

        //debugMatrix(loglikelyhood, NUM_INSTANCE, 2);
        printf("new head_a=%f, tail_a=%f, head_b=%f, tail_b=%f, pa=%f pb=%f\n", 
                head_a, tail_a, head_b, tail_b, new_pa, new_pb);

        if (fabs(new_pa - pa) < EPSILON && fabs(new_pb - pb) < EPSILON) {
            printf("ealy stop delta1=%f delta2=%f\n", fabs(new_pa - pa), fabs(new_pb - pb));
            break;
        }
        pa = new_pa;
        pb = new_pb;
    }

    return 0;
}

void debugMatrix(float (*m)[2], int row, int col) {
    for (int i = 0; i < row; i ++) {
        for (int j = 0; j < col; j ++) {
            printf("%f ", m[i][j]);
        }
        printf("\n");
    }
}
