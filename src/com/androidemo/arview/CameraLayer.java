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

import java.io.IOException;
import android.content.Context;
import android.hardware.Camera;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class CameraLayer extends SurfaceView implements SurfaceHolder.Callback {
	private Camera mCamera;

	private static final String TAG = "CameraLayer";
   
	public CameraLayer(Context context, AttributeSet attrs) {
        super(context, attrs);
        
        SurfaceHolder holder = getHolder();
        holder.addCallback(this);
        holder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
    }

	public void surfaceCreated(SurfaceHolder holder) {
        mCamera = Camera.open();
      	try {
			mCamera.setPreviewDisplay(holder);
		} catch (IOException e) {
			Log.e(TAG, "camera.setPreviewDisplay(holder);");
		}
       	mCamera.setDisplayOrientation(90);
        mCamera.startPreview();
	}

    public void surfaceDestroyed(SurfaceHolder holder) {    	
        mCamera.stopPreview();
        mCamera.release();
        mCamera = null;
    }

    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
    }
}
