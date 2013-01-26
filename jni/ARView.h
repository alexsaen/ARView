/*
	Copyright (c) 2013, Alexey Saenko
	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#ifndef ARVIEW_H_
#define ARVIEW_H_

#include "math3d.h"

class ARView {
public:
			ARView();
			~ARView()		{}

	void	draw() const;
	void	reshape(int width, int height);

	void 	accelerometerChanged(const vec3 &v);
	void 	gyroscopeChanged(const vec3 &v, float timeDelta);
	void 	magnetometerChanged(const vec3 &v);

private:
	mat4 	mProjection;
	quat 	mOrientation;

	// Sensors data
	vec3	mAccelerometer, mGyroscope, mMagnetometer;

	bool 	calcRotationMatrix(const vec3 &gravity, const vec3 &geomagnetic, mat4 &rotMatrix) const;
	void 	calcOrientation();
	mat4 	getOrientationMatrix() const;
	quat 	getOrientation() const;
	float 	getAzimuth() const;

	void 	drawGrid() const;
	void 	drawAxis() const;
};


#endif
