package net.holak.railclimber;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView.Renderer;

public class GameRenderer implements Renderer {
	
	public GameRenderer(int w, int h) {
		lastUpdate = 0;
		lastFps = 0;
		frames = 0;
		//Log.i("RailClimber", "res" + w + " " + h);
	}

	@Override
	public void onDrawFrame(GL10 unused) {
		GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
		long curTime = System.currentTimeMillis();
		if (curTime - lastUpdate >= 20) {
			RailClimberNative.update();
			if (curTime - lastFps >= 1000) {
				RailClimberNative.setFPS((float)(frames*(1000.0/(curTime-lastFps))));
				frames = 0;
				lastFps = curTime;
			}
		}
		RailClimberNative.draw();
		frames++;
	}

	@Override
	public void onSurfaceChanged(GL10 unused, int width, int height) {
		GLES20.glViewport(0, 0, width, height);
	}

	@Override
	public void onSurfaceCreated(GL10 unused, EGLConfig config) {
		RailClimberNative.createGraphics();
	}
	
	private long lastUpdate, lastFps;
	private int frames;
}
