#ifndef MY_TYPES_H
#define MY_TYPES_H

typedef unsigned char       B1;
typedef unsigned char       U8;
typedef unsigned short      U16;
typedef unsigned long       U32;
typedef unsigned long long  U64;
typedef signed char         I8;
typedef signed short        I16;
typedef signed long         I32;
typedef signed long long    I64;

typedef union
{
  U8 u8;
  I8 i8;
  struct
  {
    U8 b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1;
  };
} UU8;

typedef union
{
  U16 u16;
  U8  u8[2];
  I16 i16;
  I8  i8[2];
  struct
  {
    U8 b0 :1, b1 :1, b2 :1, b3 :1, b4 :1, b5 :1, b6 :1, b7 :1,
       b8 :1, b9 :1, b10:1, b11:1, b12:1, b13:1, b14:1, b15:1;
  };
} UU16;

typedef union
{
  U32 u32;
  U16 u16[2];
  U8  u8[4];
  I32 i32;
  I16 i16[2];
  I8  i8[4];
  struct
  {
    U8 b0 :1, b1 :1, b2 :1, b3 :1, b4 :1, b5 :1, b6 :1, b7 :1,
       b8 :1, b9 :1, b10:1, b11:1, b12:1, b13:1, b14:1, b15:1,
       b16:1, b17:1, b18:1, b19:1, b20:1, b21:1, b22:1, b23:1,
       b24:1, b25:1, b26:1, b27:1, b28:1, b29:1, b30:1, b31:1;
  };
} UU32;


typedef union
{
  U64 u64;
  U32 u32[2];
  U16 u16[4];
  U8  u8[8];
  I64 i64;
  I32 i32[2];
  I16 i16[4];
  I8  i8[8];
  struct
  {
    U8 b0 :1, b1 :1, b2 :1, b3 :1, b4 :1, b5 :1, b6 :1, b7 :1,
       b8 :1, b9 :1, b10:1, b11:1, b12:1, b13:1, b14:1, b15:1,
       b16:1, b17:1, b18:1, b19:1, b20:1, b21:1, b22:1, b23:1,
       b24:1, b25:1, b26:1, b27:1, b28:1, b29:1, b30:1, b31:1,
       b32:1, b33:1, b34:1, b35:1, b36:1, b37:1, b38:1, b39:1,
       b40:1, b41:1, b42:1, b43:1, b44:1, b45:1, b46:1, b47:1,
       b48:1, b49:1, b50:1, b51:1, b52:1, b53:1, b54:1, b55:1,
       b56:1, b57:1, b58:1, b59:1, b60:1, b61:1, b62:1, b63:1;
  };
} UU64;



#endif  /* MY_TYPES_H */
