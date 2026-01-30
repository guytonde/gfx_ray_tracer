#include <cmath>
#include <iostream>

#include "light.h"
#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>

using namespace std;

double DirectionalLight::distanceAttenuation(const glm::dvec3 &) const {
  // distance to light is infinite, so f(di) goes to 0.  Return 1.
  return 1.0;
}

glm::dvec3 DirectionalLight::shadowAttenuation(const ray &r,
                                               const glm::dvec3 &p) const {
  // YOUR CODE HERE:
  (void)p; // p is not needed if the caller already built r from p

  const Scene* scene = this->getScene();              // NOT r.getScene() [file:1]
  glm::dvec3 atten(1.0, 1.0, 1.0);

  ray shadowRay(r);                                   // copy so we can march [file:1]
  isect hit;

  while (scene->intersect(shadowRay, hit)) {          // trace toward light [file:1]
      const Material& m = hit.getMaterial();          // material at blocker [file:1]
      glm::dvec3 kt = m.kt(hit);                      // transmissive term [file:1]

      atten *= kt;
      if (glm::length(atten) <= 0.0) return glm::dvec3(0.0, 0.0, 0.0);

      // Move origin past this intersection to keep looking for more blockers
      glm::dvec3 newOrigin =
          shadowRay.at(hit.getT()) + RAY_EPSILON * shadowRay.getDirection(); // epsilon [file:1]
      shadowRay.setPosition(newOrigin);               // ray supports setPosition [file:1]
      // direction stays the same
  }

  return atten;
  // You should implement shadow-handling code here.
  // return glm::dvec3(1.0, 1.0, 1.0);
}

glm::dvec3 DirectionalLight::getColor() const { return color; }

glm::dvec3 DirectionalLight::getDirection(const glm::dvec3 &) const {
  return -orientation;
}

double PointLight::distanceAttenuation(const glm::dvec3 &P) const {
  // YOUR CODE HERE
  double distance = glm::length(position - P);
  double denom = constantTerm + linearTerm * distance + quadraticTerm * distance * distance;
  if (denom < 0.0001) denom = 0.0001; // Avoid division by zero
  double atten = 1.0 / denom;
  return glm::clamp(atten, 0.0, 1.0); // Clamp to [0,1] as per spec

  // You'll need to modify this method to attenuate the intensity
  // of the light based on the distance between the source and the
  // point P.  For now, we assume no attenuation and just return 1.0
  // return 1.0;
}

glm::dvec3 PointLight::getColor() const { return color; }

glm::dvec3 PointLight::getDirection(const glm::dvec3 &P) const {
  return glm::normalize(position - P);
}

glm::dvec3 PointLight::shadowAttenuation(const ray &r,
                                         const glm::dvec3 &p) const {
  // YOUR CODE HERE:
  // You should implement shadow-handling code here.
  return glm::dvec3(1, 1, 1);
}

#define VERBOSE 0

