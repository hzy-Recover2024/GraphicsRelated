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

#define GLOBAL_STATE_ENUM \
    ENUM_MAP(RASTERIZER_DISCARD)\
    ENUM_MAP(FRONT)\
    ENUM_MAP(BACK)\
    ENUM_MAP(FRONT_AND_BACK)\
    ENUM_MAP(CW)\
    ENUM_MAP(CCW)\
    ENUM_MAP(STENCIL_TEST)\
    ENUM_MAP(DEPTH_TEST)\
    ENUM_MAP(NEVER)\
    ENUM_MAP(ALWAYS)\
    ENUM_MAP(LESS)\
    ENUM_MAP(LEQUAL)\
    ENUM_MAP(GREATER)\
    ENUM_MAP(GEQUAL)\
    ENUM_MAP(EQUAL)\
    ENUM_MAP(NOTEQUAL)\
    ENUM_MAP(KEEP)\
    ENUM_MAP(ZERO)\
    ENUM_MAP(REPLACE)\
    ENUM_MAP(INCR)\
    ENUM_MAP(INCR_WRAP)\
    ENUM_MAP(DECR)\
    ENUM_MAP(DECR_WRAP)\
    ENUM_MAP(INVERT)\
    ENUM_MAP(BLEND)\
    ENUM_MAP(FUNC_ADD)\
    ENUM_MAP(FUNC_SUBTRACT)\
    ENUM_MAP(FUNC_REVERSE_SUBTRACT)\
    ENUM_MAP(MIN)\
    ENUM_MAP(MAX)\
    ENUM_MAP(ONE)\
    ENUM_MAP(ONE_MINUS_SRC_COLOR)\
    ENUM_MAP(ONE_MINUS_DST_COLOR)\
    ENUM_MAP(ONE_MINUS_SRC_ALPHA)\
    ENUM_MAP(ONE_MINUS_DST_ALPHA)\
    ENUM_MAP(SRC_ALPHA)\
    ENUM_MAP(DST_ALPHA)\

    enum GlobalState {
#define ENUM_MAP(NAME) k##NAME,
        // 背面剔除
        //kFront, kBack, kFront_and_back, kCW, kCCW,
        // 逐片元测试
        //Stencil_test, kDepth_test, kNever, kAlways, kLess, kLequal, kGreater, kGequal, kEqual, kNotequal,
        // 模板测试通过后的行为
        //kKeep, kZero, kReplace, kIncr, kIncr_wrap, kDecr, kDecr_wrap, kInvert,
        // 混合
        //kBlend, kFunc_add, kFunc_subtract, kFunc_reverse_subtract, kMin, kMax,
        //kOne, kOne_minus_src_color, kOne_minus_dst_color, kOne_minus_src_alpha, kOne_minus_dst_alpha,
        //kSrc_alpha, kDst_alpha
        GLOBAL_STATE_ENUM
#undef ENUM_MAP
    };

    extern GRGL_enum transGlobalState(GlobalState);
}
#endif // !_GRGL_TYPE_H

