/*
 * Filters.c
 *
 *  Created on: 4 abr. 2018
 *      Author: jcala
 */


#include "Filters.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

float get_div(int bit_resolution);
int get_high_limit(int bits_per_sample);
int get_minus(int bits_per_sample);
inline float filter_sample(const float in_sample,filter_t* _filter);

filter_t* filter_create(float a0, float a1, float b0, float b1, float b2, uint8_t filter_resolution){
	filter_t* new_filter = (filter_t*)malloc(sizeof(filter_t));
	//Inicializa la estructura con los parametros
	new_filter->a_coef[0] = a0;
	new_filter->a_coef[1] = a1;
	new_filter->b_coef[0] = b0;
	new_filter->b_coef[1] = b1;
	new_filter->b_coef[2] = b2;
	new_filter->q = filter_resolution;
	//Inicializa las memorias a 0
	filter_reset(new_filter);

	return new_filter;
}

void filter_reset(filter_t* _filter){
	_filter->m_x[0] = 0;
	_filter->m_x[1] = 0;
	_filter->m_y[0] = 0;
	_filter->m_y[1] = 0;
}

void filter_samples(float* samples,int num_samples,filter_t* _filter){
	int i;

	for(i = 0; i < num_samples; i++){
		*(samples+i) = filter_sample(*(samples+i),_filter);
	}
}

inline float filter_sample(const float in_sample,filter_t* _filter){
	//Calcula la salida
	register float out_sample = _filter->b_coef[0]*in_sample
							  + _filter->b_coef[1]*_filter->m_x[0]
							  + _filter->b_coef[2]*_filter->m_x[1]
							  - _filter->a_coef[0]*_filter->m_y[0]
							  - _filter->a_coef[1]*_filter->m_y[1];
	fprintf(tst,"out:%f,in:%f \n",out_sample,in_sample);
	//Actualizamos memorias
	_filter->m_x[1] = _filter->m_x[0];
	_filter->m_y[1] = _filter->m_y[0];
	_filter->m_x[0] = in_sample;
	_filter->m_y[0] = out_sample;
	return out_sample;
}


/*Conversion de formatos*/

//TODO float_2_uint float_2_int

void float_2_int(int* int_buff, int len_int_buff, float* float_buff,int bit_resolution){
	int i;
	float div = get_div(bit_resolution);
	if(!div){
		//Traza de error
		return;
	}
	for(i = 0; i < len_int_buff; i++){
		*(float_buff+i) = (*(float_buff+i))*div;
		if(*(float_buff+i) > div){
			*(float_buff+i) = div;
		}else if(*(float_buff+i) < -div){
			*(float_buff+i) = -div;
		}
		*(int_buff+i) = (int)*(float_buff+i);
	}
}

void uint_2_float(int* int_buff, int len_int_buff, float* float_buff,int bit_resolution){
	int i;
	int minus = get_minus(bit_resolution);
	int high_limit = get_high_limit(bit_resolution);
	float div = get_div(bit_resolution);
	if(!div){
		//Traza de error
		return;
	}
	for(i = 0; i < len_int_buff; i++){
		if(*(int_buff+i) > high_limit){
			*(float_buff+i) = (float)(*(int_buff+i)-minus)/div;
		}else{
			*(float_buff+i) = (float)(*(int_buff+i))/div;
		}
		if(*(float_buff+i) > 1){
			*(float_buff+i) = 1;
		}else if(*(float_buff+i) < -1){
			*(float_buff+i) = -1;
		}
	}
}

void int_2_float(int* int_buff, int len_int_buff, float* float_buff,int bit_resolution){
	int i;
	float div = get_div(bit_resolution);
	if(!div){
		//Traza de error
		return;
	}
	for(i = 0; i < len_int_buff; i++){
		*(float_buff+i) = (float)(*(int_buff+i))/div;
		if(*(float_buff+i) > 1){
			*(float_buff+i) = 1;
		}else if(*(float_buff+i) < -1){
			*(float_buff+i) = -1;
		}
	}
}

int get_high_limit(int bits_per_sample){
	switch (bits_per_sample) {
	case 8:
		return CHAR_MAX;
	case 16:
		return SHRT_MAX;
	case 32:
		return INT_MAX;
	default:
		return 0;
	}
}

int get_minus(int bits_per_sample){
	switch (bits_per_sample) {
	case 8:
		return UCHAR_MAX;
	case 16:
		return USHRT_MAX;
	case 32:
		return  UINT_MAX;
	default:
		return 0;
	}
}


float get_div(int bit_resolution){
	switch(bit_resolution){
	case 8 :
		return (float)128;
	case 16 :
		return (float)32768;
	case 32:
		return (float)2147483648;
	default:
		return 0;
	}
}


