import java.awt.Graphics2D;
import java.awt.Color;
import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.io.File;
import java.util.Random;
import javax.imageio.ImageIO;

public class NoiseCLI {
	public static void main(String[] args) throws Exception {
		final int   SIZE = 128;
		final float STEP = 8.0f;
		final int   FRAMES = 64;
		final float ANIMSTEP = 4.0f;
		final int SEED = new Random().nextInt();

		PerlinNoiseGenerator noiseGen = new PerlinNoiseGenerator(SEED);

		float[][][] n = new float[FRAMES][SIZE][SIZE];
		
		for (int i=0; i<FRAMES; i++) {
			for (int j=0; j<SIZE; j++) {
				for (int k=0; k<SIZE; k++) {
					n[i][j][k] = noiseGen.tileableNoise3(j/(SIZE / STEP), k/(SIZE / STEP), i/(FRAMES / ANIMSTEP), STEP, STEP, ANIMSTEP);
				}
			}
		}

		for (int i=0; i<FRAMES; i++) {
			BufferedImage resultImg = new BufferedImage(SIZE, SIZE, BufferedImage.TYPE_INT_ARGB);
			Graphics2D g = resultImg.createGraphics();
			for (int j=0; j<SIZE; j++) {
				for (int k=0; k<SIZE; k++) {
					int clr = (int)((n[i][j][k]+1.0f)*127.0f);
					g.setColor(new Color(clr, clr, clr));
					g.fill(new Rectangle(j, k, 1, 1));
				}
			}
			ImageIO.write(resultImg, "png", new File(String.format("noise%03d.png", i+1)));
		}
	}
}
