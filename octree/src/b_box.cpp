#include "b_box.h"

namespace GRelated {
    bool Bounding_box::intersect(const Bounding_box& other) const
    {
        // 检查在三个轴上的投影线段是否全都有交集
        bool xCheck = (GRfloat_equal(x_max, other.x_min) || x_max > other.x_min) &&
            (GRfloat_equal(x_min, other.x_max) || other.x_max > x_max);
        bool yCheck = (GRfloat_equal(y_max, other.y_min) || y_max > other.y_min) &&
            (GRfloat_equal(y_min, other.y_max) || other.y_max > y_max);
        bool zCheck = (GRfloat_equal(z_max, other.z_min) || z_max > other.z_min) &&
            (GRfloat_equal(z_min, other.z_max) || other.z_max > z_max);
        return xCheck && yCheck && zCheck;
    }

    bool Bounding_box::valid() const
    {
        if (!GRfloat_equal(x_min, x_max) && x_min < x_max &&
            !GRfloat_equal(y_max, y_min) && y_min < y_max &&
            !GRfloat_equal(z_max, z_min) && z_min < z_max)
        {
            return true;
        }
        return false;
    }

    void Bounding_box::expand(GRfloat x, GRfloat y, GRfloat z)
    {
        if (x > x_max)
        {
            x_max = x;
        }
        if (x < x_min)
        {
            x_min = x;
        }
        if (y > y_max)
        {
            y_max = y;
        }
        if (y < y_min)
        {
            y_min = y;
        }
        if (z > z_max)
        {
            z_max = z;
        }
        if (z < z_min)
        {
            z_min = z;
        }
    }

    bool Bounding_box::contains(const Bounding_box& other) const
    {
        bool minCheck = (GRfloat_equal(other.x_min, x_min) || other.x_min > x_min) &&
            (GRfloat_equal(other.y_min, y_min) || other.y_min > y_min) &&
            (GRfloat_equal(other.z_min, z_min) || other.z_min > z_min);
        bool maxCheck = (GRfloat_equal(other.x_max, x_max) || other.x_max < x_min) &&
            (GRfloat_equal(other.y_max, y_max) || other.y_max < y_max) &&
            (GRfloat_equal(other.z_max, z_max) || other.z_max < z_max);
        return minCheck && maxCheck;
    }
}

