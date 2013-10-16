#pragma once

namespace util {
	/// Sets the "projection" matrix4fv uniform on the given shader
	void perspectiveMatrix(shader & s, float fov, float aspect = (16.0f/9.0f), float nearplane = .1f, float farplane=100.0f);
}