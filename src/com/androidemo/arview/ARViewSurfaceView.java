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

package com.androidemo.arview;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.graphics.PixelFormat;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

public class ARViewSurfaceView extends GLSurfaceView implements SensorEventListener {
    private static final float NS2S = 1.0f / 1000000000.0f;
	private Engine mEngine;
	private long mGyrTimestamp = 0;
		
	public ARViewSurfaceView(Context context, AttributeSet attrs) {
		super(context, attrs);		
		
        setEGLConfigChooser(8, 8, 8, 8, 0, 0);
        getHolder().setFormat(PixelFormat.TRANSLUCENT);    
        setZOrderOnTop(true);
        
        mEngine = new Engine();
        setRenderer(mEngine);
	}
	
	class Engine implements Renderer {
	   	 
    	public Engine() {}    		

        public void onDrawFrame(GL10 gl) {
        	draw();
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {	    	
        	reshape(width, height);
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) 	{
        }
				
		public native void 		reshape(int width, int height);	        		
		public native void 		draw();	        
		public native void		accelerometerChanged(float x, float y, float z);
		public native void		magnetometerChanged(float x, float y, float z);
		public native void		gyroscopeChanged(float x, float y, float z, float dt);
    }

	public void onAccuracyChanged(Sensor sensor, int accuracy) {}

	public void onSensorChanged(SensorEvent event) {

		class AccelerometerEvent implements Runnable {
    		private float[] mValue; 
    		
    		AccelerometerEvent(float[] v) { 
    			mValue = v.clone(); 
    		}
    		
    		public void run() {
    			mEngine.accelerometerChanged(mValue[0], mValue[1], mValue[2]);
    		}
    	};

    	class MagnetometerEvent implements Runnable {
    		private float[] mValue; 
    		
    		MagnetometerEvent(float[] v) { 
    			mValue = v.clone(); 
    		}
    		
    		public void run() {
    			mEngine.magnetometerChanged(mValue[0], mValue[1], mValue[2]);
    		}
    	};
			
    	class GyroscopeEvent implements Runnable {
    		private float[] mValue;
    		private float 	mTimeDelta;    		
  
    		GyroscopeEvent(float[] v, float dt) { 
    			mValue = v.clone(); 
    			mTimeDelta=dt; 
    		}
    		
    		public void run() {
    			mEngine.gyroscopeChanged(mValue[0], mValue[1], mValue[2], mTimeDelta);
    		}
    	};

    	switch( event.sensor.getType() ) {
    	
			case Sensor.TYPE_ACCELEROMETER:
				queueEvent(new AccelerometerEvent(event.values) );
				break;
				
			case Sensor.TYPE_MAGNETIC_FIELD:
				queueEvent(new MagnetometerEvent(event.values) );
				break;
				
			case Sensor.TYPE_GYROSCOPE:
				if(mGyrTimestamp==0) {
					mGyrTimestamp = event.timestamp;
				} else {
					queueEvent(new GyroscopeEvent(event.values, (event.timestamp - mGyrTimestamp) * NS2S));
					mGyrTimestamp = event.timestamp;
				}	
				break;				
		}
	}
	

}
