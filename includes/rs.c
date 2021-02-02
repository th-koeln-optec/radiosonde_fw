/// @file rs.c
/*
 * Filename:            rs.c
 * Description:         TBD
 * Author:              zilog80 (https://github.com/rs1729/RS/blob/master/demod/mod/bch_ecc_mod.c)
 * Edited by:           M. Malyska; The original file can be found in the same directory: bch_ecc_mod.c.bak
 */

#include "rs.h"

void test(GF_t* gf){
  uint8_t test_a[MAX_DEG+1] = {0x01, 0x02, 0x03, 0x05};
  uint8_t test_b[MAX_DEG+1] = {0x00, 0x02, 0x04, 0x06, 0x08};
  uint8_t test_ab[MAX_DEG+1] = {0};
  uint8_t d[MAX_DEG+1] = {0};
  //poly_multiply(gf, test_a, test_b, test_ab);
  poly_divide(gf, test_a, test_b, d, test_ab);
  asm ("nop");
}

/**
 * This function determines the degree of a galois element.
 * @pram p This is the galois elemt whose degree will be determined.
 * @return If an emtpy galois element is passed, -1 will be returned, else the degree of the galois element is returned.
 */
 //todo: somewhere in here might be a problem, since execution is sometimes awfully slow, please investigate Watson!
static int16_t gf_degree(uint32_t p){
  uint8_t degree = 31; //The 31st bit of p is its most significant bit.

  if (p == 0){
    return -1; //deg(0) = -infinity
  }

  //Iterate through p and check every bit position for an 1-bit, begining with the most significant bit.
  for(; (degree && !(p & (1 << degree))); degree--);

  return degree;
}

/**
 * This function determines the degree of a galois field polynomial.
 * @param poly Address of an array that holds the galois field polynomial.
 * @return If an emtpy galois field polynomial is passed, -1 will be returned, else the degree of the galois field polynomial is returned.
 */
static int16_t poly_degree(uint8_t poly[]){
  int16_t degree = MAX_DEG;

  for(; poly[degree] == 0 && degree > 0; degree--);

  if(poly[degree] == 0){
    degree--;  // deg(0) = -infinity
  }

  return degree;
}

/**
 * This function multiplies two galois elements generating a result that is a galois element itself. The multiplication is based on pure
 * calculation and doesn't take use of any lookup tables (LUT). This is important since generating the LUTs requires a multiplication itself,
 * a chicken-egg-problem would arise here.
 * @param gf Points to a gt_t struct which holds all the necessary parameters that characterize the galois fields used.
 * @param p The first factor for the multiplication, this has to be a galois element.
 * @param q The sencond factor for the multiplication, this has to be a galois element.
 * @return The product of the multiplication, this will be a galois element itself.
 */
static uint8_t gf_multiply_calculate(GF_t *gf, uint8_t p, uint8_t q){
  uint32_t _p = p; //permanently cast into a 32bit wide variable
  uint8_t buffer = 0; //buffer for imtermediate results during multiplication
  int8_t q_length = (gf_degree(q) + 1); //cut unnecessary for looping by predetermination of the length of q (number of coefficients (length in bits) is allways one more then degree)
  int8_t prime_degree = gf_degree(gf->prime); //Since prime doesn't change, determine it here once, and not repeatedly inside the for loop.

//This is binary carryless multiplication, therefore addition is substituted by xoring
  for (uint8_t i = 0; i < q_length; i++){
    if(gf_degree(_p) >= prime_degree){
      /*This saves the long division at the end, which catches a product that is bigger than the 
       *galois field and reduces it. You can apperently achieve the same if you catch overflown
       *numbers before xoring into the buffer and already reduce them at this piont.*/
       //Todo: Maybe it is nonetheless faster to do a long division at the end, then repeatedly checking both galois element degrees?
      _p ^= gf->prime;
    }
    if(q & 1){
      buffer ^= (uint8_t)_p;
    }
    _p <<= 1;
    q >>=1;
  }

  return buffer;
}

/**
 * This function will calculate the LUTs content, which holds the exponents of alpha (the primitive element) and the logarithms of alpha.
 * @param gf Points to a gt_t struct which holds all the necessary parameters that characterize the galois fields used.
 * @return Returns 0 if both LUT where sucessfully initialized, if an error occured -1 will be returned.
 */
static int8_t gf_lut_init(GF_t *gf){
  uint16_t i, j; //Todo: shift variable definition into forloops
  uint8_t b = 0x01;

  //Fill the exp_a lookup-table by gf_multiplying: a^n = a^(n-1) * a^1 (for n > 2), a^0 is allways 1, a^1 is a(alpha) by defitition here
  //Todo: b might be redudant here, give it a thought if it could be cut out.
  for (i = 0; i < gf->order; i++){
    gf->exp_a[i] = b; // b_i = a^i
    b = gf_multiply_calculate(gf, gf->alpha, b);
  }

  gf->log_a[0] = -00;  //log(0) = -inf, log(0) is not defined, therefor the following for loop will start with index 1
  for (i = 1; i < gf->order; i++){
    //a^j = i, this can be solved for j if you look up every a^j (this is the exp_a[]-LUT that was created one step before, so lets use it)
    //and check if it is equal to i, if so save it into log_a[i] = j
    for(j = 0; gf->exp_a[j] != i; j++){
      if(j > (gf->order - 1)){
        return -1; //If no j is found that solves a^j=i, something is wrong with the primitive itself
      }
    }
    gf->log_a[i] = j;
  }

  return 0;
}

/**
 * This function multiplies two galois elements generating a result that is a galois element itself. The multiplication is based on LUT
 * calculation and does take use of lookup tables (LUT). It is important to initialize the LUTs by calling gf_lut_init before using this function.
 * @param gf Points to a gt_t struct which holds all the necessary parameters that characterize the galois fields used.
 * @param p The first factor for the multiplication, this has to be a galois element.
 * @param q The sencond factor for the multiplication, this has to be a galois element.
 * @return The product of the multiplication, this will be a galois element itself.
 */
static uint8_t gf_multiply_lut(GF_t *gf, uint8_t p, uint8_t q){
  uint32_t x = 0;

  if((p == 0) || (q == 0)){
    return 0;
  }
  //This multiplication is based on the fact that p*q=x can be expressed as 
  //p*q=x <=> log_a(p*q)=log_a(x) <=> log_a(p)+log_a(q)=log_a(x) <=> a^(log_a(p)+log_a(q))=x
  x = (gf->log_a[p] + gf->log_a[q]);
  x %= (gf->order - 1); //The addition of two galois elements may produce an overflow out of the galoisfield bounds, the remainder will be the correct galois element.
  x = gf->exp_a[x];
  return (uint8_t)x;
}

/**
 * This function will compute the inverse of a galois element which is a galois element itself.
 * @param gf Points to a gt_t struct which holds all the necessary parameters that characterize the galois fields used.
 * @param p This is the galois elemt which is to get inversed.
 * @return The inverse of the galois element will be returned.
 */
static uint8_t gf_inverse(GF_t *gf, uint8_t p){
  if(p == 0){
    return 0; //log_a[0] is not defined, however the inverse of 0 is still 0, so this exception must be handled here.
  }

  return gf->exp_a[gf->order-1-gf->log_a[p]]; //p'=a^((2^m)-1-log_a[p])
}

/**
 * This function multiplies two galois field polynomials generating a result that is a galois element itself.
 * The multiplication is based on LUT calculation and does take use of lookup tables (LUT).
 * It is important to initialize both LUTs by calling gf_lut_init before using this function.
 * @param gf Points to a gt_t struct which holds all necessary parameters that characterize galois fields.
 * @param a Address of an array that holds factor a's galois field polynomial.
 * @param b Address of an array that holds factos b's galois field polynomial.
 * @param ab Pointer to an array's start address, where computation result will be written to.
 * @return 0 will be returned if computation succeeds, otherwise 1 will be returned.
 */
static int8_t poly_multiply(GF_t *gf, uint8_t a[], uint8_t b[], uint8_t *ab){
  int16_t degree_a = 0;
  int16_t degree_b = 0;
  uint8_t c[MAX_DEG+1] = {0};

  degree_a = poly_degree(a);
  degree_b = poly_degree(b);

  //If multiplication results in a polynomial with a degree bigger than MAX_DEG, LUT multiplication would result in an error and
  //array length would be insufficient.
  if (degree_a + degree_b > MAX_DEG){
   return -1;
  }

  //Multiplication is performed with usual outer product calculation, but using galois element multiplication.
  for (uint16_t i = 0; i <= degree_a; i++){
    for (uint16_t j = 0; j <= degree_b; j++){
      c[i+j] ^= gf_multiply_lut(gf, a[i], b[j]);
    }
  }

  //Array c is necessary to buffer computation results if result register and factor register are identical.
  for (uint16_t i = 0; i <= MAX_DEG; i++){
    ab[i] = c[i];
  }

  return 0;
}

/**
 * 
 * @param gf Points to a gt_t struct which holds all necessary parameters that characterize galois fields.
 * @param p Address of an array that holds dividend p's galois field polynomial.
 * @param q Address of an array that holds divisor q's galois field polynomial.
 * @param d Pointer to an array's start address, where division quotient will be written to.
 * @param r Pointer to an array's start address, where division remainder will be written to.
 * @return 0 will be returned if computation succeeds, otherwise 1 will be returned.
 */
static int poly_divide(GF_t *gf, uint8_t p[], uint8_t q[], uint8_t *d, uint8_t *r){
  // p(x) = q(x)d(x) + r(x)

  int16_t degree_p = -1;
  int16_t degree_q = -1;
  uint8_t c;

  degree_p = poly_degree(p);
  degree_q = poly_degree(q);

  if(degree_q < 0){ //Detect if divisior is zero, if so abbort since division by zero is not possible.
    return -1;
  }

  //Initialize both result registers to zero since their state is unknown.
  for(uint16_t i = 0; i <= MAX_DEG; i++){
    d[i] = 0;
  }
  for(uint16_t i = 0; i <= MAX_DEG; i++){
    r[i] = 0;
  }

  c = gf_multiply_lut(gf, p[degree_p], gf_inverse(gf, q[degree_q]));

  //If divisor is of degree 0, every division is remainderless.
  if(degree_q == 0){
    for(uint16_t i = 0; i <= degree_p; i++){
      d[i] = gf_multiply_lut(gf, p[i], c);
    }
    //for(uint16_t i = 0; i <= MAX_DEG; i++){r[i] = 0;} //Since r was already initialized to zero, remaining indexes don't need to be zeroed again.
  }

  //Check if dividend is zero, if so quotient and remainder will be zero too, no calculation needed.
  else if(degree_p < 0){
    return 0;
  }
  
  //If divisor's degree is greater than dividend's degree quotient will be zero, only remainder needs to be calculated.
  //In this case dividend and remainder are equal, so only p array must be copied into r array.
  else if(degree_p < degree_q){
    //for(uint16_t i = 0; i <= MAX_DEG; i++){d[i] = 0;} //Since d is already initialized to zero, this hasn't to be repeated here.
    for(uint16_t i = 0; i <= degree_p; i++){
      r[i] = p[i];
    }
    //for(uint16_t i = degree_p+1; i <= MAX_DEG; i++){r[i] = 0;} //Since r was already initialized to zero, remaining indexes don't need to be zeroed again.
  }

  else{
    for(uint16_t i = 0; i <= degree_p; i++){
      r[i] = p[i];
    }
    while(degree_p >= degree_q){
      d[degree_p-degree_q] = c;
      for(uint16_t i = 0; i <= degree_q; i++){
        r[degree_p-i] ^= gf_multiply_lut(gf, q[degree_q-i], c);
      }
      while(r[degree_p] == 0 && degree_p > 0){
        degree_p--;
      }
      if(r[degree_p] == 0){
        degree_p--;
      }
      if(degree_p >= 0){
        c = gf_multiply_lut(gf, r[degree_p], gf_inverse(gf, q[degree_q]));
      }
    }
  }

  return 0;
}

/**
 * 
 * @param rs Points to a rs_t struct which holds all the necessary parameters that characterize the reed-solomon properties.
 * @param gf Points to a gt_t struct which holds all the necessary parameters that characterize the galois fields used.
 * @return Returns 0 if gf_lut_init sucessfully initialized, if an error occured -1 will be returned.
 */
int8_t rs_init(RS_t* rs, GF_t* gf){
    int8_t lut_success = -1;
    uint8_t Xalpha[MAX_DEG+1] = {0};

    rs->gf = gf;

    lut_success = gf_lut_init(gf);

    //for (uint16_t i = 0; i <= MAX_DEG; i++){
    //  rs->g[i] = 0;
    //}

    //g(X)=(X-alpha^b)...(X-alpha^(b+2t-1)), b=0
    rs->g[0] = 0x01;
    Xalpha[1] = 0x01; // X
    for (uint16_t i = 0; i < 2*rs->t; i++) {
        Xalpha[0] = gf->exp_a[(rs->b+i) % (gf->order-1)];  // Xalp[0..1]: X - alpha^(b+i)
        poly_multiply(gf, rs->g, Xalpha, rs->g);
    }

    return lut_success;
}

/**
 * 
 */
int rs_encode(RS_t *RS, uint8_t cw[], uint8_t dest[]){
    GF_t *gf = RS->gf;
    //int j;
    uint8_t _cw[MAX_DEG+1] = {0};
    uint8_t parity[MAX_DEG+1] = {0};
    uint8_t d[MAX_DEG+1];
    //for (j = 0; j <= MAX_DEG; j++) parity[j] = 0;
    //for (j = 0; j <= MAX_DEG; j++) __cw[j] = 0;
    //for (uint16_t j = RS->R; j < RS->N; j++) _cw[j] = cw[j];
    for(uint16_t j = RS->R; j < RS->N; j++){
      _cw[j] = cw[j];
    }
    poly_divide(gf, _cw, RS->g, d, parity);
    //if (poly_deg(parity) >= RS.R) return -1;
    //for (j = 0; j < RS->R; j++) cw[j] = parity[j];
    for (uint16_t j = 0; j < RS->R; j++){
      dest[j] = parity[j];
    }

    return 0;
}