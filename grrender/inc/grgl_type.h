/*对应opengl中的 基本变量类型*/

#ifndef _GRGL_TYPE_H
#define _GRGL_TYPE_H
#include "GR_math.h"
#define GRGL_enum GRUINT32
#define GRGL_int GRINT32
#define GRGL_uint GRUINT32
#define GRGL_byte int8_t
#define GRGL_ubyte uint8_t
#define GRGL_int64 int64_t
#define GRGL_uint64 uint64_t
#define GRGL_sizei GRUINT32
#define GRGL_intptr uintptr_t
#define GRGL_sizeiptr uintptr_t
#define GRGL_sync uintptr_t
#define GRGL_float GRfloat
namespace GRelated {
    enum TypeEnum {
        kHALF_FLOAT,
        kFLOAT,
        kBYTE,
        kUNSIGNED_BYTE,
        kSHORT,
        kUNSIGNED_SHORT,
        kINT,
        kUNSIGNED_INT
    };
    extern GRGL_enum transEnum(TypeEnum);

    enum ImageFormat {
        kR8, //无符号8位归一化整数
        kRG8,
        kRGB8,
        kRGBA8,
        //无符号16位归一化整数
        kR16,
        kRG16,
        kRGB16,
        kRGBA16,
        // 16位浮点
        kR16F,
        kRG16F,
        kRGB16F,
        kRGBA16F,
        // 32位浮点
        kR32F,
        kRG32F,
        kRGB32F,
        kRGBA32F,

        // 深度格式
        kDepth24,
        kDepth32,
        kDepth32F,

        //深度模板格式
        kDepth24Stencil8

        //...TODO
    };
    // 定义宏来映射 ImageFormat 到 GL 枚举
#define MAP_FORMAT(fmt, gl_fmt) case fmt: return gl_fmt
    extern GRGL_enum transImageFormat(ImageFormat format);

}
#endif // !_GRGL_TYPE_H

