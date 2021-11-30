//
// Created by LEI XU on 5/16/19.
//

#ifndef RAYTRACING_BOUNDS3_H
#define RAYTRACING_BOUNDS3_H
#include "Ray.hpp"
#include "Vector.hpp"
#include <limits>
#include <array>

class Bounds3
{
  public:
    Vector3f pMin, pMax; // two points to specify the bounding box
    Bounds3()
    {
        double minNum = std::numeric_limits<double>::lowest();
        double maxNum = std::numeric_limits<double>::max();
        pMax = Vector3f(minNum, minNum, minNum);
        pMin = Vector3f(maxNum, maxNum, maxNum);
    }
    Bounds3(const Vector3f p) : pMin(p), pMax(p) {}
    Bounds3(const Vector3f p1, const Vector3f p2)
    {
        pMin = Vector3f(fmin(p1.x, p2.x), fmin(p1.y, p2.y), fmin(p1.z, p2.z));
        pMax = Vector3f(fmax(p1.x, p2.x), fmax(p1.y, p2.y), fmax(p1.z, p2.z));
    }

    Vector3f Diagonal() const { return pMax - pMin; }
    int maxExtent() const
    {
        Vector3f d = Diagonal();
        if (d.x > d.y && d.x > d.z)
            return 0;
        else if (d.y > d.z)
            return 1;
        else
            return 2;
    }

    double SurfaceArea() const
    {
        Vector3f d = Diagonal();
        return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
    }

    Vector3f Centroid() { return 0.5 * pMin + 0.5 * pMax; }
    Bounds3 Intersect(const Bounds3& b)
    {
        return Bounds3(Vector3f(fmax(pMin.x, b.pMin.x), fmax(pMin.y, b.pMin.y),
                                fmax(pMin.z, b.pMin.z)),
                       Vector3f(fmin(pMax.x, b.pMax.x), fmin(pMax.y, b.pMax.y),
                                fmin(pMax.z, b.pMax.z)));
    }

    Vector3f Offset(const Vector3f& p) const
    {
        Vector3f o = p - pMin;
        if (pMax.x > pMin.x)
            o.x /= pMax.x - pMin.x;
        if (pMax.y > pMin.y)
            o.y /= pMax.y - pMin.y;
        if (pMax.z > pMin.z)
            o.z /= pMax.z - pMin.z;
        return o;
    }

    bool Overlaps(const Bounds3& b1, const Bounds3& b2)
    {
        bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
        bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);
        bool z = (b1.pMax.z >= b2.pMin.z) && (b1.pMin.z <= b2.pMax.z);
        return (x && y && z);
    }

    bool Inside(const Vector3f& p, const Bounds3& b)
    {
        return (p.x >= b.pMin.x && p.x <= b.pMax.x && p.y >= b.pMin.y &&
                p.y <= b.pMax.y && p.z >= b.pMin.z && p.z <= b.pMax.z);
    }
    inline const Vector3f& operator[](int i) const
    {
        return (i == 0) ? pMin : pMax;
    }

    inline bool IntersectP(const Ray& ray, float* hitt0, float* hitt1) const;
    inline bool IntersectP(const Ray& ray, const Vector3f& invDir,
                           const std::array<int, 3>& dirisNeg) const;
};

inline bool Bounds3::IntersectP(const Ray& ray, float* hitt0,
                                float* hitt1) const
{
    float t0 = 0, t1 = ray.t_max;
    for (int i = 0; i < 3; i++) {
        float invRay = ray.direction_inv[i];
        float tNear = (pMin[i] - ray.origin[i]) * invRay;
        float tFar = (pMax[i] - ray.origin[i]) * invRay;
        if (tNear > tFar)
            std::swap(tNear, tFar);
        t0 = tNear > t0 ? tNear : t0;
        t1 = tFar < t1 ? tFar : t1;
        if (t0 > t1)
            return false;
    }

    if (hitt0)
        *hitt0 = t0;
    if (hitt1)
        *hitt1 = t1;
    return true;
}

inline bool Bounds3::IntersectP(const Ray& ray, const Vector3f& invDir,
                                const std::array<int, 3>& dirIsNeg) const
{
    // TODO test if ray bound intersects
    /*RAY*/
    // //Destination = origin + t*direction
    // Vector3f origin;
    // Vector3f direction, direction_inv;
    // double t;//transportation time,
    // double t_min, t_max;
    /*Bounding Box */
    // Vector3f pMin, pMax; // two points to specify the bounding box

    // Vector3f tMin = (pMin - ray.origin) * invDir;
    // Vector3f tMax = (pMax - ray.origin) * invDir;

    float t1, t2, tStart = -10000000.0, tEnd = 10000000000.0;
    // X
    // if (ray.direction.x == 0 and ray.origin.x < pMin.x or ray.origin.x < pMax.x) {
    //     return false;
    // }
    t1 = (pMin.x - ray.origin.x) * invDir.x;
    t2 = (pMax.x - ray.origin.x) * invDir.x;
    if (t1 > t2) {
        float temp = t2;
        t2 = t1;
        t1 = temp;
    }
    if (t1 > tStart) {
        tStart = t1;
    }
    if (t2 < tEnd) {
        tEnd = t2;
    }
    // Y
    t1 = (pMin.y - ray.origin.y) * invDir.y;
    t2 = (pMax.y - ray.origin.y) * invDir.y;

    if (t1 > t2) {
        float temp = t2;
        t2 = t1;
        t1 = temp;
    }
    if (t1 > tStart) {
        tStart = t1;
    }
    if (t2 < tEnd) {
        tEnd = t2;
    }

    // Z
    t1 = (pMin.z - ray.origin.z) * invDir.z;
    t2 = (pMax.z - ray.origin.z) * invDir.z;

    if (t1 > t2) {
        float temp = t2;
        t2 = t1;
        t1 = temp;
    }
    if (t1 > tStart) {
        tStart = t1;
    }
    if (t2 < tEnd) {
        tEnd = t2;
    }

    if (tStart > tEnd) {
        return false;
    }
    if (tEnd < ray.t_min) {
        return false;
    }

    if (tStart > ray.t_min) {
        return true;
    }
    return true;








    // float t_enter = std::max(tMin.x, tMin.y);
    // t_enter = std::max(t_enter, tMin.z);


    // float t_exit = std::min(tMax.x, tMax.y);
    // t_exit = std::min(tMax.z, t_exit);
    // std::cout << t_enter << " " << t_exit << std::endl;


    // std::cout << ray.t_min << " " << ray.t_max << std::endl;

    // if(t_enter < t_exit and t_exit >= 0) {
    //     std::cout << "RETURNING TRUE" << std::endl;
    //     return true;
    // }
    // return false;



    

}

inline Bounds3 Union(const Bounds3& b1, const Bounds3& b2)
{
    Bounds3 ret;
    ret.pMin = Vector3f::Min(b1.pMin, b2.pMin);
    ret.pMax = Vector3f::Max(b1.pMax, b2.pMax);
    return ret;
}

inline Bounds3 Union(const Bounds3& b, const Vector3f& p)
{
    Bounds3 ret;
    ret.pMin = Vector3f::Min(b.pMin, p);
    ret.pMax = Vector3f::Max(b.pMax, p);
    return ret;
}

#endif // RAYTRACING_BOUNDS3_H
