/// @file rs.h
/*
 * Filename:            rs.h
 * Description:         This file containts rs.c related function headers.
 * Author:              M. Malyska
 */

#ifndef RS_H
#define RS_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*INCLUDES*/
#include <stdint.h>

/*DEFINES*/
#define MAX_DEG 254  // max N-1
#define RS_ORDER 256

/*enumERATORS*/

/*typedefINITIONS*/
typedef struct{
  uint32_t prime; //primitive irreducible polynomial
  uint32_t order; //size of the galois field used
  uint8_t alpha; //primitive element
  uint8_t exp_a[RS_ORDER];
  uint8_t log_a[RS_ORDER];
}GF_t;

typedef struct{
  uint8_t N; //
  uint8_t t;
  uint8_t R;  // RS: R=2t, BCH: R<=mt
  uint8_t K;  // K=N-R
  uint8_t b;
  uint8_t p; uint8_t ip; // p*ip = 1 mod N
  uint8_t g[MAX_DEG+1]; // MAX_DEG+1 = 255
  GF_t* gf;
}RS_t;

/*GLOBAL VARIABLES*/
extern RS_t RS256;

/*PUBLIC PROTOTYPES*/
static int16_t gf_degree(uint32_t p);
static int16_t poly_degree(uint8_t poly[]);
static uint8_t gf_multiply_calculate(GF_t *gf, uint8_t a, uint8_t b);
static int8_t gf_lut_init(GF_t *gf);
static uint8_t gf_multiply_lut(GF_t *gf, uint8_t p, uint8_t q);
static uint8_t gf_inverse(GF_t *gf, uint8_t p);
static int8_t poly_multiply(GF_t *gf, uint8_t a[], uint8_t b[], uint8_t *ab);
static int poly_divide(GF_t *gf, uint8_t p[], uint8_t q[], uint8_t *d, uint8_t *r);
int8_t rs_init(RS_t* rs, GF_t* gf);
void rs_encode(RS_t* rs, uint8_t cw[], uint8_t dest[]);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* RS_H */