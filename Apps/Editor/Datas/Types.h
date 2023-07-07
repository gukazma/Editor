#pragma once
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
namespace soarscape
{
typedef ::CGAL::Simple_cartesian<float>                     Kernel;
typedef Kernel::Point_3										Point3;
typedef Kernel::Point_2										Point2;
typedef ::CGAL::Surface_mesh<Point3>                        SurfaceMesh;
}

