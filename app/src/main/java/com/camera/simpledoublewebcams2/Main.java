package com.camera.simpledoublewebcams2;

import android.app.Activity;
import android.os.Bundle;

import java.io.IOException;

public class Main extends Activity {
	
	private CameraPreview cp;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		try {
			Runtime.getRuntime().exec("su -c chmod 777 /dev/video0");
		} catch (IOException e) {
			e.printStackTrace();
		}

		cp = (CameraPreview) findViewById(R.id.cp);
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
	}

	@Override
	public void onDetachedFromWindow() {
		super.onDetachedFromWindow();
	}
}
