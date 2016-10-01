public class PPMFigureDriver {
	public static void main(String[] args) {
		PPMFigureDriver ob = new PPMFigureDriver();
		double m1 = ob.checkLine();
		System.out.println();
		double m2 = ob.checkRectangle();
		System.out.println();
		double m3 = ob.checkClear();
		System.out.println();
		System.out.println();
		double sum = m1 + m2 + m3;
		System.out.println("TOTAL OBTAINED SCORE: " + sum);
		System.out.println();

	}

	private double checkLine() {

		double val = 0.0d;

		System.out.println("PPM Figure Draw Line Test:");
		System.out.println("Test 1:");
		try {
			PPMFigure figure = new PPMFigure(200, 300, new Pixel(255, 255, 255));

			figure.drawLine(0, 0, 200, 200, new Pixel(123, 100, 90));
			figure.drawLine(0, 99, 99, 0, new Pixel(0, 0, 0));

			figure.save("line_1.ppm");

			PPMImage img1 = new PPMImage("line_1.ppm");
			PPMImage img2 = new PPMImage("line1.ppm");

			if (check(img1, img2)) {
				System.out.println("Passed");
				val += 3;
			} else {
				System.out.println("Failed. Check line_1.ppm visually.");
			}

		} catch (Exception e) {
			System.out.println("Exception thrown. Check line_1.ppm visually");
		}

		System.out.println("Test 2:");
		try {
			PPMFigure figure = new PPMFigure(100, 100, new Pixel(255, 255, 255));

			figure.drawLine(0, 0, 99, 99, new Pixel(0, 0, 0));
			figure.drawLine(0, 99, 99, 0, new Pixel(20, 50, 100));

			figure.save("line_2.ppm");
			PPMImage img1 = new PPMImage("line_2.ppm");
			PPMImage img2 = new PPMImage("line2.ppm");

			if (check(img1, img2)) {
				System.out.println("Passed");
				val += 3;
			} else {
				System.out.println("Failed. Check line_2.ppm visually.");
			}

		} catch (Exception e) {
			System.out.println("Exception thrown. Check line_2.ppm visually");
		}

		return val;
	}

	private double checkRectangle() {

		double val = 0.0d;

		System.out.println("PPM Figure Draw Rectangle Test:");
		System.out.println("Test 1:");
		try {
			PPMFigure figure = new PPMFigure(200, 300, new Pixel(255, 255, 255));

			figure.drawRectangle(0, 0, 50, 100, new Pixel(255, 0, 0));
			figure.drawRectangle(0, 100, 80, 20, new Pixel(0, 255, 0));

			figure.save("Rectangle_1.ppm");

			PPMImage img1 = new PPMImage("Rectangle_1.ppm");
			PPMImage img2 = new PPMImage("Rectangle1.ppm");

			if (check(img1, img2)) {
				System.out.println("Passed");
				val += 2;
			} else {
				System.out.println("Failed. Check Rectangle_1.ppm visually.");
			}

		} catch (Exception e) {
			System.out
					.println("Exception thrown. Check Rectangle_1.ppm visually");
		}

		System.out.println("Test 2:");
		try {
			PPMFigure figure = new PPMFigure(100, 100, new Pixel(255, 255, 255));

			figure.drawRectangle(0, 0, 50, 50, new Pixel(255, 0, 0));
			figure.drawRectangle(0, 50, 50, 50, new Pixel(0, 255, 0));
			figure.drawRectangle(50, 50, 50, 50, new Pixel(255, 0, 0));
			figure.drawRectangle(50, 0, 50, 50, new Pixel(0, 255, 0));

			figure.save("Rectangle_2.ppm");
			PPMImage img1 = new PPMImage("Rectangle_2.ppm");
			PPMImage img2 = new PPMImage("Rectangle2.ppm");

			if (check(img1, img2)) {
				System.out.println("Passed");
				val += 2;
			} else {
				System.out.println("Failed. Check Rectangle_2.ppm visually.");
			}

		} catch (Exception e) {
			System.out
					.println("Exception thrown. Check Rectangle_2.ppm visually");
		}

		return val;
	}

	private double checkClear() {

		double val = 0.0d;

		System.out.println("PPM Figure Clear Test:");
		System.out.println("Test 1:");
		try {
			PPMFigure Fig1 = new PPMFigure(100, 100, new Pixel(255, 255, 255));
			Pixel color = new Pixel(200, 100, 50);
			Fig1.clear(color);
			Fig1.save("clear_1.ppm");

			PPMImage img1 = new PPMImage("clear_1.ppm");
			PPMImage img2 = new PPMImage("clear1.ppm");

			if (check(img1, img2)) {
				System.out.println("Passed");
				val += 3;
			} else {
				System.out.println("Failed. Check clear_1.ppm visually.");
			}

		} catch (Exception e) {
			System.out.println("Exception thrown. Check clear_1.ppm visually");
		}

		return val;
	}

	private boolean check(PPMImage img1, PPMImage img2) {
		Pixel[][] p1 = img1.getPixels();
		Pixel[][] p2 = img2.getPixels();

		if (p1.length != p2.length) {
			return false;
		}

		for (int i = 0; i < p1.length; i++) {
			if (p1[i].length != p2[i].length) {
				return false;
			}

			for (int j = 0; j < p1[i].length; j++) {
				if (p1[i][j].getRed() != p2[i][j].getRed() || p1[i][j].getGreen() != p2[i][j].getGreen() || p1[i][j].getBlue() != p2[i][j].getBlue())  {
					System.out.println("P1 RGB: " + p1[i][j].getRed()  + " " + p1[i][j].getGreen() + " " + p1[i][j].getBlue() + " " + "P2 RGB: " + p2[i][j].getRed() + " " + p1[i][j].getGreen() + " " + p2[i][j].getBlue());
					return false;
				}

			}

		}
		return true;
	}
}
