package net.holak.railclimber;

import android.app.Activity;
import android.content.Context;
import android.media.AudioManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Vibrator;
import android.util.DisplayMetrics;
import android.view.KeyEvent;

public class RailClimberActivity extends Activity {
	public static final boolean isRelease = false;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        //Boolean isRelease = Boolean.parseBoolean(getResources().getString(R.string.is_release));
        setVolumeControlStream(AudioManager.STREAM_MUSIC);
        
        RailClimberNative.amgr = getAssets();
        RailClimberNative.activity = this;
        RailClimberNative.sharedPrefs = getPreferences(MODE_PRIVATE);
        RailClimberNative.sharedPrefsEditor = RailClimberNative.sharedPrefs.edit();
        RailClimberNative.vibrator = (Vibrator)getSystemService(Context.VIBRATOR_SERVICE);
        RailClimberNative.locale = getApplicationContext().getResources().getConfiguration().locale.getLanguage();
        
        final DisplayMetrics dm = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);
        
		RailClimberNative.init(dm.widthPixels, dm.heightPixels);
		mView = new GameView(this, dm.widthPixels, dm.heightPixels);
		setContentView(mView);
		mReady = true;
    }
    
    @Override
    public void onBackPressed() {
    	debugState = 0;
    	if (mReady) {
	    	mView.queueEvent(new Runnable() {
	    		public void run() {
	    			RailClimberNative.backButtonPressed();
	    		}
	    	});
    	}else{
    		super.onBackPressed();
    	}
    }
    
    @Override
    public boolean onKeyUp(int keyCode, android.view.KeyEvent event) {
    	if (mReady) {
    		if (keyCode == KeyEvent.KEYCODE_SEARCH) {
    			if (debugState == 0 || debugState == 1 || debugState == 4 || debugState == 5 ||
    				debugState == 9 || debugState == 10)
    				debugState++;
    			else debugState = 0;
    			return true;
    		}else if (keyCode == KeyEvent.KEYCODE_MENU) {
    			if (debugState == 2 || debugState == 3 || debugState == 6 || debugState == 7 ||
    				debugState == 8 || debugState == 11 || debugState == 12 || debugState == 13)
    				debugState++;
    			else if (debugState == 14) {
    				RailClimberNative.requestDebugAction();
    				debugState = 0;
    			}else debugState = 0;
    			return true;
    		}else if (keyCode == KeyEvent.KEYCODE_BACK) {
    			onBackPressed();
    			return true;
    		}
    	}
    	return false;
    }
    
    @Override
    protected void onPause() {
    	super.onPause();
    	if (mReady) {
    		mView.onPause();
    		RailClimberNative.pause();
    	}
    }
    
    @Override
    protected void onResume() {
    	super.onResume();
    	if (mReady) {
    		mView.onResume();
    		RailClimberNative.resume();
    	}
    }; 
    
    @Override
    protected void onDestroy() {
    	super.onDestroy();
    };
    
    private GLSurfaceView mView;
    private boolean mReady = false;
    private int debugState = 0;
}