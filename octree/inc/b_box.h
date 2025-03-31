/*包围盒定义*/
#ifndef _B_BOX_H
#define _B_BOX_H

#include "GR_math.h"
namespace GRelated {
    // aabb包围盒
    struct alignas(alignof(GRfloat)) Bounding_box
    {
        GRfloat x_min;
        GRfloat y_min;
        GRfloat z_min;
        GRfloat x_max;
        GRfloat y_max;
        GRfloat z_max;
        Bounding_box()
            : x_min(GRfloat_max), y_min(GRfloat_max), z_min(GRfloat_max),
              x_max(GRfloat_min), y_max(GRfloat_min), z_max(GRfloat_min)
        {

        }
        Bounding_box(GRfloat xmin, GRfloat ymin, GRfloat zmin,
            GRfloat xmax, GRfloat ymax, GRfloat zmax)
            : x_min(xmin), y_min(ymin), z_min(zmin), x_max(xmax), y_max(ymax), z_max(zmax)
        {

        }
        // 是否包含
        bool contains(const Bounding_box&) const;
        // 是否相交
        bool intersect(const Bounding_box&) const;
        // 是否有效(不为空)
        bool valid() const;
        // 扩展
        void expand(GRfloat x, GRfloat y, GRfloat z);

        const GRfloat* max_pt() const
        {
            return &x_max;
        }

        const GRfloat* min_pt() const
        {
            return &x_min;
        }
    };

    struct alignas(alignof(GRfloat)) Ray
    {
        GRfloat pos[3];
        GRfloat dir[3];

        Ray(GRfloat x, GRfloat y, GRfloat z, GRfloat dir_x, GRfloat dir_y, GRfloat dir_z)
        {
            pos[0] = x;
            pos[1] = y;
            pos[2] = z;
            dir[0] = dir_x;
            dir[1] = dir_y;
            dir[2] = dir_z;
        }
    };
}
#endif // !_B_BOX_H

