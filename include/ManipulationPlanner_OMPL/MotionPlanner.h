#ifndef _MOTION_PLANNER_H_
#define _MOTION_PLANNER_H_

#include <ompl/geometric/SimpleSetup.h>

#include <ompl/geometric/planners/prm/PRM.h>
#include <ompl/geometric/planners/rrt/RRTstar.h>
#include <ompl/geometric/planners/rrt/LBTRRT.h>
#include <ompl/geometric/planners/rrt/LazyRRT.h>
#include <ompl/geometric/planners/rrt/RRT.h>
#include <ompl/geometric/planners/rrt/RRTConnect.h>
#include <ompl/geometric/planners/rrt/TRRT.h>
#include <ompl/geometric/planners/rrt/pRRT.h>
#include <ompl/geometric/planners/est/EST.h>
#include <ompl/base/spaces/SE3StateSpace.h>
#include <ompl/base/PlannerData.h>
#include <cmath>
#include <ctime>
#include <iostream>
#include <fstream>
#include <ostream>

namespace ob = ompl::base;
namespace og = ompl::geometric;

#include "TrajectoryPlannerSkel.h"

template <typename T>
inline T Sq(const T &x)
{
  return x * x;
}

typedef struct{
  double x;
  double y;
  double z;
} POINT;

typedef struct {
  double xrange[2];
  double yrange[2];
  double zrange[2];
} RANGE;

typedef boost::numeric::ublas::matrix<double> TMatrix;
typedef boost::numeric::ublas::vector<double> TVector;

struct TLink {
  TVector Axis;  // Joint axis
  TVector End;   // End-point position of the link defined on the local frame
  TLink(const TVector &a, const TVector &e) : Axis(a), End(e) {}
};

class Planning{
  public:
    Planning(std::string fileName);
    void setPlanningMethod(int m){selector = m;}
    void SetArm();
    void PlannerSelector();
    void initFromFile(std::string fileName);
    void CreateCube(std::ostream &cube);
    bool clear(const double* xMin, const double* xMax,
               const double* yMin, const double* yMax,
               const double* zMin, const double* zMax,
               int numObstacles,
               double xTest, double yTest, double zTest);
    void CreateGridPoint(const double* xMin, const double* xMax,
                         const double* yMin, const double* yMax,
                         const double* zMin, const double* zMax, POINT *P, int i);
    void PlaneEquation(POINT p[], int i0[], int i1[], int i2[], int i, double a[]);
    void Pcompare(POINT A, POINT B, POINT *compare);
    bool link(const double* xMin, const double* xMax,
              const double* yMin, const double* yMax,
              const double* zMin, const double* zMax,
              int numObstacles,
              double xStart, double yStart, double zStart,
              double xDest, double yDest, double zDest);
    bool isStateValid(const ob::State *state);
    void ForwardKinematics(const std::vector<TLink> &linkes,
                           const std::vector<double> &angles, const TVector &base,
                           std::vector<TVector> &result);
    void printEdge(std::ostream &os, const ob::StateSpacePtr &space, const ob::PlannerDataVertex &vertex);
    // double distance_vertex(const TVector &p1, const TVector &p2);
    // double length_vector(const TVector &v);
    // TVector cross_vector(const TVector &vl, const TVector &vr);
    // double Distance_DotAndLine(const TVector &P, const TVector &A, const TVector &B);
    void printDistance();
    void planWithSimpleSetup(const RTC::JointPose& start, const RTC::JointPose& goal, RTC::JointTrajectory_out trajectory);

    void PrintSolution(const char *filename, const og::PathGeometric &path, int skip = 1);

    int OpenGnuplot(const char *filename, const og::PathGeometric &path, int skip = 1);
    void PrintArmSequence(const char *filename, const og::PathGeometric &path, int skip=1);
    void CheckArmSequence();
    void PrintArmSolution(const char *filename, const og::PathGeometric &path, int skip=1);
    void PrintArmSequenceAnime(const char *filename, const og::PathGeometric &path, int skip=1);

  protected:
    int m_planningMethod = 1;
    double SizeZ = 5;
    std::vector<TLink> Arm;  // Manipulator
    TVector ArmBase;  // The base position of Manipulator
    double total_len;
    int num = 6;//joint num?

    int selector;
    std::vector<TVector> Obstacles;

    double* xMin;
    double* xMax;
    double* yMin;
    double* yMax;
    double* zMin;
    double* zMax;

    /// Number of obstacles in space.
    int numObstacles;

    /// Start position in space
    //double* Start;

    /// Goal position in space
    //double* Goal;

    /// Boundaries of the space
    double xLeft;
    double xRight;
    double yTop;
    double yBottom;
    double zTop;
    double zBottom;
};
#endif
