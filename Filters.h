/*
 * Filters.h
 *
 *  Created on: 4 abr. 2018
 *      Author: jcala
 */

#include <stdint.h>
#include <stdio.h>
typedef struct filter{
		float a_coef[2];
		float b_coef[3];
		float m_x[2];
		float m_y[2];
		uint8_t q;
}filter_t;

filter_t* filter_create(float a0, float a1, float b0, float b1, float b2, uint8_t filter_resolution);
void filter_reset(filter_t* _filter);
void filter_samples(float* samples,int num_samples,filter_t* _filter);
void float_2_int(int* int_buff, int len_int_buff, float* float_buff,int bit_resolution);
void int_2_float(int* int_buff, int len_int_buff, float* float_buff,int bit_resolution);
void uint_2_float(int* int_buff, int len_int_buff, float* float_buff,int bit_resolution);
FILE* tst;
