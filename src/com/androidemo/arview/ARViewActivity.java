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

import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.util.Log;
import com.androidemo.arview.R;

public class ARViewActivity extends Activity {
 
	static {
	    System.loadLibrary("Argo");
	}
	
	private static final String TAG = "ArgoActivity";
	
	private ARViewSurfaceView surfaceView=null;
	private SensorManager 	sensorManager=null;
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
  
        setRequestedOrientation (ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
		
		sensorManager = (SensorManager)getSystemService(SENSOR_SERVICE);
		if( sensorManager == null ) {
			Log.e(TAG, "SensorManager wasn't found.");
			return;
		}

		setContentView(R.layout.main);
        surfaceView = (ARViewSurfaceView) findViewById(R.id.mainview);        
    }

	public void onResume() {
	    surfaceView.onResume();		
		super.onResume();
		registerListeners();
	}

	public void onPause() {
		surfaceView.onPause();
		super.onPause();
		unregisterListeners();
	}
 	 		
    private void registerListeners() {
    	if(sensorManager != null) {
			Sensor accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
			if(accelerometer != null)
		        sensorManager.registerListener(surfaceView, accelerometer, SensorManager.SENSOR_DELAY_GAME);
			else
				Log.w(TAG, "Failed to init accelerometer.");
			
			Sensor magneticSensor = sensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);
			if(magneticSensor != null)
		        sensorManager.registerListener(surfaceView, magneticSensor, SensorManager.SENSOR_DELAY_GAME);
			else
				Log.w(TAG, "Failed to init magnetic sensor.");		

			Sensor gyroscopeSensor = sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE);
			if(gyroscopeSensor != null)
		        sensorManager.registerListener(surfaceView, gyroscopeSensor, SensorManager.SENSOR_DELAY_GAME);
			else
				Log.w(TAG, "Failed to init gyroscope sensor.");		
    	}
    }

	private void unregisterListeners() {
    	if(sensorManager != null) 
    		sensorManager.unregisterListener(surfaceView);		
	}


	
}
	
	