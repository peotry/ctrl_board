/*
 *           Copyright 2012 Availink, Inc.
 *
 *  This software contains Availink proprietary information and
 *  its use and disclosure are restricted solely to the terms in
 *  the corresponding written license agreement. It shall not be 
 *  disclosed to anyone other than valid licensees without
 *  written permission of Availink, Inc.
 *
 */


///$Date: 2012-3-8 21:47 $
///
///
/// @file
/// @brief Defines the primary data type according the target platform. All data 
/// types defined in this file should be overwritten by Users according to 
/// their own target platform
/// 
#ifndef bspdatadef_h_h
#define bspdatadef_h_h

#include <semaphore.h>

#ifndef NULL
#define NULL    ((void *)0)
#endif

//#define AVL_CPLUSPLUS     ///< Used to switch the C++ and C compiler. Comment the macro AVL_CPLUSPLUS if you use C compiler

typedef  char AVL_char;     ///< 8 bits signed char data type.
typedef  unsigned char AVL_uchar;   ///< 8 bits unsigned char data type.

typedef  short AVL_int16;   ///< 16 bits signed char data type.
typedef  unsigned short AVL_uint16; ///< 16 bits unsigned char data type.

typedef  int AVL_int32;     ///< 32 bits signed char data type.
typedef  unsigned int AVL_uint32;   ///< 32 bits unsigned char data type.

typedef  char * AVL_pchar;  ///< pointer to a 8 bits signed char data type.
typedef  unsigned char * AVL_puchar; ///< pointer to a 8 bits unsigned char data type.

typedef  short * AVL_pint16;    ///< pointer to a 16 bits signed char data type.
typedef  unsigned short * AVL_puint16;  ///< pointer to a 16 bits unsigned char data type.

typedef  int * AVL_pint32;  ///< pointer to a 32 bits signed char data type.
typedef  unsigned int * AVL_puint32; ///< pointer to a 32 bits unsigned char data type.


//typedef unsigned int AVL_semaphore;     ///< the semaphore data type.
//typedef unsigned int * AVL_psemaphore;      ///< the pointer to a semaphore data type.

typedef sem_t AVL_semaphore;    ///< the semaphore data type.
typedef sem_t * AVL_psemaphore;



#endif
