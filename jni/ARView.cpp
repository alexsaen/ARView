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

#include "ARView.h"
#include <GLES/gl.h>

ARView::ARView(): mProjection(1.0f),
			mOrientation(1.0f),
			mAccelerometer(1.0f), mGyroscope(1.0f), mMagnetometer(1.0f)
{}

void ARView::drawGrid() const {
	const int cnt = 10;

	glEnableClientState(GL_VERTEX_ARRAY);

	for(int i=-cnt; i<=cnt; ++i) {
		vec2 v[4] = { vec2(-cnt, i), vec2( cnt, i), vec2(i, -cnt), vec2( i, cnt) };

		glColor4f(1, 1, 1, (i%5) ? 0.5f : 1);
		glVertexPointer(2, GL_FLOAT, 0, v);
		glDrawArrays(GL_LINES, 0, 4);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
}

void ARView::drawAxis() const {
	vec3 axis[6] = { vec3(0,0,0), vec3(1,0,0), vec3(0,0,0), vec3(0,1,0), vec3(0,0,0), vec3(0,0,1) };

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, axis);

	glColor4f(1,0,0,1);
	glDrawArrays(GL_LINES, 0, 2);
	glColor4f(0,1,0,1);
	glDrawArrays(GL_LINES, 2, 2);
	glColor4f(0,0,1,1);
	glDrawArrays(GL_LINES, 4, 2);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void ARView::draw() const {
	glClearColor(0, 0, 0, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf(mProjection);

	mat4 matOrientation = getOrientationMatrix();
	mat4 matModel = matOrientation * mat4::get_translate(0, 0, -1.5f);
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf(matModel);

	drawGrid();

	matModel = mat4::get_translate(0, 0, -5) * matOrientation;
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf(matModel);

	drawAxis();
}


void ARView::reshape(int width, int height) {
	float aspect = float(width) / height;
	mProjection.perspective(60, aspect, 0.5f, 10000);
	glViewport(0, 0, width, height);
}

void ARView::accelerometerChanged(const vec3 &v) {
	mAccelerometer = v;
	calcOrientation();
}

void ARView::gyroscopeChanged(const vec3 &v, float timeDelta) {
	mGyroscope = v;

	vec3 vv(mGyroscope);
	float mag = vv.normalize();
	quat delta(vv, rad2deg(mag*timeDelta));
	mOrientation *= delta;
	mOrientation.normalize();
}

void ARView::magnetometerChanged(const vec3 &v) {
	mMagnetometer = v;
	calcOrientation();
}

bool ARView::calcRotationMatrix(const vec3 &gravity, const vec3 &geomagnetic, mat4 &r) const {
	vec3 h = cross(geomagnetic, gravity);
	float norm = h.normalize();
	if(norm < 0.1f)
		return false;

	vec3 g(gravity);
	g.normalize();
	vec3 m = cross(g, h);

	r[0]  = h.x;   r[1]  = h.y;   r[2]  = h.z;
	r[4]  = m.x;   r[5]  = m.y;   r[6]  = m.z;
	r[8]  = g.x;   r[9]  = g.y;   r[10] = g.z;

	r[3] = r[7] = r[11] = r[12] = r[13] = r[14] = 0;
	r[15] = 1;

	return true;
}

void ARView::calcOrientation() {
	const float factor = 0.05f;
	mat4 r;
	calcRotationMatrix(mAccelerometer, mMagnetometer, r);
//	quat q = quat::get_rotate_z(-geomagneticDeclination) * quat(r);
	quat q = quat(r);
	mOrientation = slerp(mOrientation, q, factor);
}

mat4 ARView::getOrientationMatrix() const {
	return mat4(vec3(0,0,0), mOrientation);
}

quat ARView::getOrientation() const {
	return mOrientation;
}

float ARView::getAzimuth() const {
	return rad2deg( atan2f(mOrientation.w, mOrientation.z) * 2.0f );
}

