#include <iostream>
#include <memory>
#include <algorithm>

#include <Eigen>
#include "NearestNeighbor.h"


/**
 * Convert world coordinate to the a pixel coordinate
 * Then, we take this pixel and get the nearest pixel in the current frame
 **/
Vector2f convertWorldToPixel(Matrix3f &depthIntrinsics, Matrix4f &currentPose, Vector3f &globalPoint) {
    // homogenised world coordinate
    Vector4f hWorld = Vector4f(globalPoint(0), globalPoint(1), globalPoint(2), 1.0);
    
    // homogenised camera coordinate from current pose w.r.t to the world
    // currentPose global to camera point
    Vector4f hCamera = currentPose.inverse() * hWorld;
    Vector3f cameraPoint = Vector3f(hCamera(0), hCamera(1), hCamera(2));

    // get homogenised pixel coord    
    Vector3f pixelPoint = depthIntrinsics * cameraPoint;
    // project to pixel space (x/z, y/z)
    return Vector2f(pixelPoint(0) / pixelPoint(2), pixelPoint(1) / pixelPoint(2));
}

/**
 * Get the nearest pixel in the current frame pixels
 **/
Vector2f getNearestPixel(Eigen::Vector2f &pixel, std::vector<Eigen::Vector2f> &framePixels) {
  // use nearest neighbour library / FLANN to get nearest pixel
  std::unique_ptr<NearestNeighborSearch> nearestNeighborSearch = std::make_unique<NearestNeighborSearchFlann>();
  nearestNeighborSearch->setMatchingMaxDistance(0.0001f);
  
  // build index based on pixels 
  nearestNeighborSearch->buildIndex(framePixels);
  auto matches = nearestNeighborSearch->queryMatches({ pixel });
  if (matches.size() >= 1) {
    return framePixels.at(matches.at(0));
  } 
  return Vector2f(MINF, MINF); 
}

float computePsi(float eta, float micro) {
  if (eta >= -micro) {
    signed char	sign = (eta >= 0) ? 1 : -1;
    float psi = std::min(1, eta / micro) * sign;
    return psi;
  } 
  return MINF;
}

float computeLambda(Matrix3f &depthIntrinsics, Vector3f &homX) {
    return (depthIntrinsics.inverse() * homX).norm();
}

/**
 * Compute the angle between the ray vector going into the surface
 * and its corresponding normal at the same surface 
 **/
float computeTheta(Vector3f &rayVector, Vector &normal) {
  auto nominator = sourceNormal.dot(targetNormal);
	auto denominator = sourceNormal.norm() * targetNormal.norm();
	return acos(nominator/sqrt(denominator));
}

float computeFrameProjectiveTSDF(float &lambda, Matrix4f &currentPose, Vector3f &globalPoint, float &depthX) {
  float eta = (currentPose.block(3, 3, 3, 1) - globalPoint).norm() / lambda - depthX;
  return computePsi(eta, micro);
}

float computeWeightProjectiveTSDF(float &theta, float& depth) {
  return cos(theta) / depth;
}

float computeFrameSDF(Vector3f &globalPoint) {
  
}

float computeWeightSDF(Vector3f &globalPoint) {

}

