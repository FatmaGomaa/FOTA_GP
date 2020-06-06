#ifndef		STD_TYPES_H
#define		STD_TYPES_H


typedef unsigned char 				u8,uint8;
typedef unsigned short 				u16,uint16;
typedef unsigned long				u32,uint32;

typedef signed char				 	s8,sint8;
typedef signed short			 	s16,sint16;
typedef signed long 			 	s32,sint32;

typedef float					 	f32;
typedef double					 	f64;
typedef long double				 	f96;


typedef u8*							pu8,puint8;
typedef u16*						pu16,puint16;
typedef u32*						pu32,puint32;

typedef s8*							ps8,psint8;
typedef s16*						ps16,psint16;
typedef s32*						ps32,psint32;

typedef f32*		 				pf32;
typedef f64*		 				pf64;
typedef f96*						pf96;

typedef void*						void_p;

typedef enum{
 NOT_OK =0, OK=1
}STD_ERROR;

typedef enum{
	STATE_OK,
	STATE_NOK,
	STATE_BUSY
}ProcessState_t;

#define NULL 				((void*)0)

#define HIGH				1
#define LOW					0

#endif
