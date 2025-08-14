#pragma once

#include "DrawDebugHelpers.h"

// Macro Function
#define DRAW_SPHERE(Location)                                                                                          \
    if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, true);

#define DRAW_SPHERE_COLOR(Location, Color)                                                                             \
    if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 8.f, 12, Color, false, 5.f);

#define DRAW_SPHERE_Singleframe(Location)                                                                              \
    if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, false, -1.f);

#define DRAW_LINE(StartLocation, EndLocation)                                                                          \
    if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f);

#define DRAW_LINE_Singleframe(StartLocation, EndLocation)                                                              \
    if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f);

#define DRAW_POINT(Location)                                                                                           \
    if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, true);

#define DRAW_POINT_Singleframe(Location)                                                                               \
    if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, false, -1.f);

#define DRAW_VECTOR(StartLocation, EndLocation)                                                                        \
    if (GetWorld())                                                                                                    \
    {                                                                                                                  \
        DRAW_LINE(StartLocation, EndLocation);                                                                         \
        DRAW_POINT(EndLocation);                                                                                       \
    }

#define DRAW_VECTOR_Singleframe(StartLocation, EndLocation)                                                            \
    if (GetWorld())                                                                                                    \
    {                                                                                                                  \
        DRAW_LINE_Singleframe(StartLocation, EndLocation);                                                             \
        DRAW_POINT_Singleframe(EndLocation);                                                                           \
    }