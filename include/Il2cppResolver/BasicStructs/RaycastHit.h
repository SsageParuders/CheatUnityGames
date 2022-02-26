#pragma once

#include "Vector2.h"

struct RaycastHit {
    Vector3 Point, Normal;
    uint32_t FaceID;
    float Distance;
    Vector2 UV;
    int32_t m_Collider;
};

string to_string(RaycastHit a) {
    return "Point: " + to_string(a.Point) + ", Normal: " +
           to_string(a.Normal) + ", FaceID: " + to_string(a.FaceID) +
           ", Distance: " + to_string(a.Distance) + ", UV: " +
           to_string(a.UV) + ", m_Collider: " +
           to_string(a.m_Collider);
}