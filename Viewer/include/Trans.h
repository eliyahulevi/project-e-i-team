#pragma once
#include <glm/glm.hpp>

class Trans {
public:
	static glm::mat4x4 getScale4x4(int factor) {
		return glm::mat4x4(factor);
	}
	static glm::mat4x4 getxRotate4x4(int teta) {
		return glm::mat4x4(glm::highp_mat4::col_type(1.0f, 0.0f, 0.0f, 0.0f),
			glm::highp_mat4::col_type(0.0f, cos(teta), (-1)*sin(teta), 0.0f),
			glm::highp_mat4::col_type(0.0f, sin(teta), cos(teta), 0.0f),
			glm::highp_mat4::col_type(0.0f, 0.0f, 0.0f, 1.0f));
	}
	static glm::mat4x4 getyRotate4x4(int teta) {
		return glm::mat4x4(glm::highp_mat4::col_type(cos(teta), 0.0f, (-1)*sin(teta), 0.0f),
			glm::highp_mat4::col_type(0.0f, 1.0f, 0.0f, 0.0f),
			glm::highp_mat4::col_type(sin(teta), 0.0f, cos(teta), 0.0f),
			glm::highp_mat4::col_type(0.0f, 0.0f, 0.0f, 1.0f));
	}
	static glm::mat4x4 getzRotate4x4(int teta) {
		return glm::mat4x4(glm::highp_mat4::col_type(cos(teta), (-1)*sin(teta), 0.0f, 0.0f),
			glm::highp_mat4::col_type(sin(teta), cos(teta), 0.0f, 0.0f),
			glm::highp_mat4::col_type(0.0f, 0.0f, 1.0f, 0.0f),
			glm::highp_mat4::col_type(0.0f, 0.0f, 0.0f, 1.0f));
	}
	static glm::mat4x4 getTranslate4x4(float tx, float ty,float tz) {
		return glm::mat4x4(glm::highp_mat4::col_type(1.0f, 0.0f, 0.0f, 0.0f),
			glm::highp_mat4::col_type(0.0f, 1.0f, 0.0f, 0.0f),
			glm::highp_mat4::col_type(0.0f, 0.0f, 1.0f, 0.0f),
			glm::highp_mat4::col_type(tx,ty, tz, 1.0f));
	}
};
