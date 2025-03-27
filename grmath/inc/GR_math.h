/*一些数学、数值定义*/
#ifndef _GR_MATH_H
#define _GR_MATH_H
#include <cmath>
#include <cstdlib>
#include <limits>
#define GRfloat float
#define GRUINT unsigned int
constexpr auto GRfloat_epsilon = std::numeric_limits<GRfloat>::epsilon();
constexpr auto GRfloat_max = std::numeric_limits<GRfloat>::max();
constexpr auto GRfloat_min = std::numeric_limits<GRfloat>::lowest();
inline bool GRfloat_equal(GRfloat a, GRfloat b)
{
    return std::fabs(a - b) <= GRfloat_epsilon;
}

#endif // !_GR_MATH_H

