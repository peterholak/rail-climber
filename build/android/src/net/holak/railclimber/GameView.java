package net.holak.railclimber;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

public class GameView extends GLSurfaceView {

	public GameView(Context context, int w, int h) {
		super(context);
		
		setEGLContextClientVersion(2);
		setRenderer(new GameRenderer(w, h));
	}

	@Override
	public boolean onTouchEvent(final MotionEvent event) {
		int ac = event.getAction();
		if (ac == MotionEvent.ACTION_DOWN) {
			queueEvent(new Runnable() {
				public void run() {
					RailClimberNative.touchPress(event.getX(), event.getY());
				}
			});
			return true;
		}else if (ac == MotionEvent.ACTION_UP) {
			queueEvent(new Runnable() {
				public void run() {
					RailClimberNative.touchRelease(event.getX(), event.getY());
				}
			});
			return true;
		}else if (ac == MotionEvent.ACTION_MOVE) {
			queueEvent(new Runnable() {
				public void run() {
					RailClimberNative.touchMove(event.getX(), event.getY());
				}
			});
			return true;
		}
		return false;
	}
}
