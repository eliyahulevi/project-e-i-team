#pragma once

#include "MeshModel.h"
#include <memory>
#include <glm/glm.hpp>

#define FFOVY_BASIC_UNIT 1.0f // must be some degree > 0 to justify its existance
#define MIN_FFOVY 1.0f
#define MAX_FFOVY M_PI

#define MIN_FNEAR 1.0f // should be at least 0.1 > MIN_FFAR to create some little gap even
#define MAX_FNEAR 1.0f

#define MIN_FFAR -1.0f
#define MAX_FFAR -1.0f

#define MIN_FLEFT -1.0f
#define MAX_FLEFT -1.0f

#define MIN_FRIGHT 1.0f
#define MAX_FRIGHT 1.0f

#define MIN_FTOP 1.0f

#define MIN_FBOTTOM -1.0f

/*
* Pseudo Algorithm of the camera behavior in transformation's relationship:
* ========================================================================
* if Orthogonal:
*	* M_projectionTransformation = SetOrthographicProjection()
* if perspective:
*	* M_projectionTransformation = SetPerspectiveProjection()
* M_viewTransformation = SetCameraLookAt()
* newVector = M_projectionTransformation*M_viewTransformation*M_worldTransform*vector
* newVector -> newVector/newVector[w]
* newVector = (x/w , y/w , z/w , 1)
* remove z/w and 1
* newVector = (x/w , y/w)
* bresenham(x/w , y/w)
*/
/*
 * Camera class. This class takes care of all the camera transformations and manipulations.
 *
 * Implementation suggestion:
 * --------------------------
 * Make the Camera class be a subclass of MeshModel, so you can easily and elegantly render 
 * the cameras you have added to the scene.
 */
class Camera : public MeshModel
{
private:
	glm::mat4x4 viewTransformation; // Mc
	glm::mat4x4 projectionTransformation; // Mp

public:
	float worldfRotatex, worldfRotatey, worldfRotatez;
	glm::vec3 origin_eye;
	int transType;
	float ffovy,fnear,ffar, fleft, fright, ftop, fbottom;
	Camera(std::shared_ptr<MeshModel> model,const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up, const float frameBufferWidth);
	~Camera();
	
	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);

	void SetOrthographicProjection(
		float fovy,
		float aspectRatio,
		float pnear,
		float pfar,
		float pleft,
		float pright,
		float ptop,
		float pbottom);

	void SetPerspectiveProjection(
		float fovy,
		float aspectRatio,
		float pnear,
		float pfar,
		float pleft,
		float pright,
		float ptop,
		float pbottom);

	glm::mat4x4 Getview() { return viewTransformation; }
	glm::mat4x4 GetProjection() { return projectionTransformation;  }
	double deg2rad(double degrees) { return degrees * 4.0 * atan(1.0) / 180.0; }
};
