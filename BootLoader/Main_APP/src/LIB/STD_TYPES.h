/* * * * * * * * * * * * * * * * * * * * * * * * * *
 * Author : Mohamed Adel Anis Ahmed				   *
 * date   : 13 May 2020                            *
 * version: 2.0                                    *
 * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef _STD_TYPES_H_
#define _STD_TYPES_H_

/*******************************Unsigned types******************************/

typedef unsigned char					uint8_t;
typedef unsigned char					u8;

typedef unsigned short int				uint16_t;
typedef unsigned short int				u16;

typedef	unsigned long int				uint32_t;
typedef unsigned long int				u32;


/*******************************Signed types********************************/

typedef signed char						int8_t;
typedef signed char		    			s8;

typedef	signed short int				int16_t;
typedef signed short int				s16;

typedef	signed long int					int32_t;
typedef signed long int					s32;

/****************************Float numbers types****************************/

typedef	float							float32_t;
typedef float		    				f32;

typedef double							float64_t;
typedef double		    				f64;

typedef long double						float96_t;
typedef long double						f96;


/*******************************Error types*****************************

typedef uint8_t	ErrorStatus_t	;
#define ERROR_OK	1
#define ERROR_NOK	0

typedef enum STD_ERROR
{
  OK,
  NOK,
  NULL_POINTER,
  NOT_VALID_INPUTS,
  BUSY

}STD_ERROR;
*/
/*******************************Miscellaneous******************************/

#define	  NULL						((void*)0)

#define SHIFT_1_BIT					(1)


#endif
