package net.holak.railclimber;

import android.media.AudioManager;
import android.media.SoundPool;
import android.os.Vibrator;
import android.os.Process;
import android.util.Log;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.URL;
import java.net.URLConnection;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;

import org.apache.http.entity.ByteArrayEntity;
import org.apache.http.util.ByteArrayBuffer;



import android.content.SharedPreferences;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.Typeface;

public class RailClimberNative {
	
	public static AssetManager amgr;
	public static RailClimberActivity activity;
	public static HashMap<String, Typeface> typefaces = new HashMap<String, Typeface>();
	public static Paint p = new Paint();
	public static SharedPreferences sharedPrefs;
	public static SharedPreferences.Editor sharedPrefsEditor;
	public static Vibrator vibrator;
	public static SoundPool soundPool;
	public static String locale;
	
	public static native void init(int w, int h);
	public static native void createGraphics();
	public static native void cleanup();
	public static native void draw();
	public static native void update();
	public static native void pause();
	public static native void resume();
	public static native void touchPress(float x, float y);
	public static native void touchMove(float x, float y);
	public static native void touchRelease(float x, float y);
	public static native void backButtonPressed();
	public static native void setFPS(float fps);
	public static native void requestDebugAction();
	
	public static Bitmap LoadBitmap(String assetName) {
		//Log.i("RailClimber", "LoadBitmap " + Process.myTid());
		try {
			return BitmapFactory.decodeStream(amgr.open(assetName));
		}catch(Exception e) {}
		return null;
	}
	
	public static Bitmap CreateTextBitmap(String filename, int ptsize, int color, String text) {
		//Log.i("RailClimber", "CTB " + text);
		if (!typefaces.containsKey(filename)) {
			Typeface tf = Typeface.createFromAsset(amgr, filename);
			typefaces.put(filename, tf);
			p.setAntiAlias(true);
			p.setLinearText(true);
		}
		Typeface tf = typefaces.get(filename);
		p.setTypeface(tf);
		p.setTextSize(ptsize);
		int h = (int)(Math.abs(p.getFontMetrics().ascent) + Math.abs(p.getFontMetrics().descent));
		float[] widths = new float[text.length()];
		float sum = 0;
		p.getTextWidths(text, widths);
		for (float f : widths)
			sum += f;
		Bitmap bmp = Bitmap.createBitmap(Math.max(1, (int)sum), Math.max(1, h), Config.ARGB_8888);
		Canvas c = new Canvas(bmp);
		p.setColor(0xFF000000 | color);
		c.drawText(text, 0, h-Math.abs(p.getFontMetrics().descent), p);
		return bmp;
	}
	
	public static Bitmap MultiLineTextBitmap(String filename, int ptsize, int color, String text, int w) {
		if (!typefaces.containsKey(filename)) {
			Typeface tf = Typeface.createFromAsset(amgr, filename);
			typefaces.put(filename, tf);
			p.setAntiAlias(true);
			p.setLinearText(true);
			p.setColor(0xFF000000 | color);
		}
		Typeface tf = typefaces.get(filename);
		p.setTypeface(tf);
		p.setTextSize(ptsize);
		p.setColor(0xFF000000 | color);

		class WordPair { public WordPair(String s, char p) { str = s; sep = p; } public String str; public char sep; }
		List<WordPair> words = new LinkedList<WordPair>();
		List<Integer> lineWidth = new ArrayList<Integer>();

		if (w == 0)
			w = 1;
		
		int h = (int)(Math.abs(p.getFontMetrics().ascent) + Math.abs(p.getFontMetrics().descent));

		String word = "";
		char lastchar = 0;
		int pos = 0;
		for (int i=0; i<text.length(); i++) {
			char tca = text.charAt(i);
			if (tca == '\n' && lastchar == '\r')
				continue;
			if (tca == ' ' || tca == '\n' || tca == '\r') {
				if (pos == 0) {
					word = " ";
				}
				words.add(new WordPair(word, tca));
				word = "";
				pos = 0;
			}else{
				word += tca;
				pos++;
			}
			lastchar = tca;
		}
		words.add(new WordPair(word, lastchar));
		
		float[] spw = new float[1];
		p.getTextWidths(" ", spw);
		int tw, spacew = (int)(Math.abs(spw[0])*1.5);
		Rect r = new Rect();
		int curX = 0, lines = 1;
		for (WordPair wp : words) {
			String s = wp.str;
			char sep = wp.sep;
			p.getTextBounds(s, 0, s.length(), r);
			tw = r.right;
			if (curX + tw + spacew > w) {
				lineWidth.add(curX);
				curX = 0;
				lines++;
			}
			curX += tw + spacew;
			if (sep != ' ' && sep != 0) {
				lineWidth.add(curX);
				curX = 0;
				lines++;
			}
		}
		lineWidth.add(curX);
		lines++;
		
		curX = 0;
		int curY = h;
		p.getTextBounds("TEST", 0, "TEST".length(), r);
		Bitmap bmp = Bitmap.createBitmap(Math.max(1, w), Math.max(1, lines)*Math.max(1, h), Config.ARGB_8888);
		Canvas c = new Canvas(bmp);
		int line = 0;
		for (WordPair wp : words) {
			String s = wp.str;
			char sep = wp.sep;
			p.getTextBounds(s, 0, s.length(), r);
			if (curX + r.right + spacew > w) {
				line++;
				curX = 0;
				curY += h;
			}
			c.drawText(s, curX+((w - lineWidth.get(line))/2), curY, p);
			curX += r.right + spacew;
			if (sep != ' ' && sep != 0) {
				line++;
				curX = 0;
				curY += h;
			}
		}
		return bmp;
	}
	
	public static void PlatformQuit() {
		//Log.i("RailClimber", "Quit " + Process.myTid());
		cleanup();
		activity.finish();
		System.exit(0);
	}
	
	public static String GetSetting(String section, String key) {
		return sharedPrefs.getString(section + "/" + key, null);
	}
	
	public static void SetSetting(String section, String key, String value) {
		sharedPrefsEditor.putString(section + "/" + key, value);
		sharedPrefsEditor.commit();
	}
	
	public static void Vibrate(int duration) {
		vibrator.vibrate(duration);
	}

	public static void SoundInit() {
		soundPool = new SoundPool(2, AudioManager.STREAM_MUSIC, 0);
	}

	public static void SoundCleanup() {
		soundPool.release();
	}

	public static int SoundLoad(String fileName) {
		fileName = fileName.replace(".wav", ".ogg");
		AssetFileDescriptor afd;
		try {
			afd = amgr.openFd(fileName);
			int id = soundPool.load(afd, 0);
			afd.close();
			return id;
		} catch (IOException e) {
			return -1;
		}
	}

	public static void SoundPlay(int soundId) {
		soundPool.play(soundId, 1.0f, 1.0f, 0, 0, 1.0f);
	}

	public static void SoundUnload(int soundId) {
		soundPool.unload(soundId);
	}
	
	public static String LoadOnlineLevelBlocking(String path) {
	/*	String levelSvg = "Not yet loaded";
		try {
			URL url = new URL("http://www.holak.net" + path + "/level.svg");
			HttpURLConnection urlConnection = (HttpURLConnection) url.openConnection();
			try {
				InputStream in = new BufferedInputStream(urlConnection.getInputStream());
				byte[] block = new byte[10240];
				String response = "";
				int len = 0;
				do {
					len = in.read(block);
					response = response + new String(block).substring(0, len);
				}while(len == 10240);
				Log.i("RailClimber", "Response: " + response.length());
				levelSvg = response;
			}finally {
				urlConnection.disconnect();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		Log.i("RailClimber", "done");
		while ("Not yet loaded".equals(levelSvg)) {}
		return levelSvg;*/
		return null;
	}

	public static String LoadTextFile(String assetName) {
		try {
			InputStream is = amgr.open(assetName);
			byte[] b = new byte[(int) is.available()];
			is.read(b);
			is.close();
			return new String(b);
		} catch (IOException e) {
			//Log.i("RailClimber", e.getMessage());
			return null;
		}
	}
	
	public static String GetLangCode() {
		return locale;
	}
	
	public static void PlatformLog(String str) {
		Log.i("AppLog", "[tid " + Process.myTid() + "]" + str);
	}
	
	static {
		System.loadLibrary("gnustl_shared");
		System.loadLibrary("Box2D");
		System.loadLibrary("RailClimberNative");
	}
}
