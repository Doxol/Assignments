import java.io.*;
import java.util.*;

/**
* A class for reading/writing/manipulating PPM images.
* @author David Sepulveda
* @version 1.0, April 10, 2015
*/
public class PPMImage{

	// image pixels
	// Defined protected for direct access in subclasses
	protected Pixel[][] pixels;
	protected int maxColor;
	protected String imageType;

	// constructors
	/**
	* No-argument constructor to be used in subclasses only, i.e., in PPMFigure
	* Initializes image type to "P3", maximum color value to 255, array pixels to null
	*/
	protected PPMImage(){
		imageType = "P3";
		maxColor = 255;
		pixels = null;
	}

	/**
	* Reads the image file parameter and initialize pixels accordingly
	* @param fileName file to be used
	* @throws PPMException if anything goes wrong while loading file
	*/
	public PPMImage(String fileName) throws PPMException{
		Scanner inputStream = null;
		try{
			inputStream = new Scanner(new FileInputStream(fileName));
		}
		catch (FileNotFoundException e){
			throw new PPMException(e.getMessage());
		}

		try{
			// tells program how to read the file
			imageType = inputStream.nextLine();
			int cols = inputStream.nextInt(), rows = inputStream.nextInt(); // takes columns and then rows
			maxColor = inputStream.nextInt();

			pixels = new Pixel[rows][cols];
			for (int r = 0; r < rows; r++)
				for (int c = 0; c < cols; c++)
					pixels[r][c] = new Pixel(inputStream.nextInt(), inputStream.nextInt(), inputStream.nextInt());
		}
		catch (NoSuchElementException e){
			throw new PPMException("Tried to call an integer that was not present.");
		}
		finally{
			if (inputStream == null) // throws exception if the file is not there to close
				throw new PPMException("Error closing file: " + fileName);
			inputStream.close();
		}
	}

	/**
	* Initializes array pixels by deep copying the content of newPixels
	* Precondition: newPixels is not null
	* @param newPixels array to be copied
	*/
	public PPMImage(Pixel[][] newPixels){
		if (newPixels != null){
			imageType = "P3";
			maxColor = 255;
			setPixels(newPixels);
		}
	}

	/**
	* Copy constructor that makes a deep copy of the parameter image
	* @param image object to be copied
	*/
	public PPMImage(PPMImage image){
		pixels = image.getPixels();
		maxColor = image.maxColor;
		imageType = image.imageType;
	}

	// accessor/mutator methods
	/**
	* Sets the pixel at (r, c) to a deep copy of pixel
	* Precondition: (r, c) is a valid index and pixel is not null
	* @param r row to set
	* @param c column to set
	* @param pixel pixel to set
	*/
	public void setPixelAt(int r, int c, Pixel pixel){
		if ((r < pixels.length) && (c < pixels[0].length))
			pixels[r][c] = new Pixel(pixel);
	}

	/**
	* Returns pixel at (r, c)
	* Precondition: (r, c) is a valid index
	* @param r row of pixel
	* @param c column of pixel
	* @return deep copy of the pixel at (r, c)
	*/
	public Pixel getPixelAt(int r, int c){
		Pixel p = null;
		if (((r >= 0) && (c >= 0)) && ((r < pixels.length) && (c < pixels[0].length)))
			p = new Pixel(pixels[r][c]);
		return p;
	}

	/**
	* Sets the instance variable pixels using a deep copy of newPixels
	* Precondition: newPixels is not null
	* @param newPixels pixel array to set the instance variable pixels to
	*/
	public void setPixels(Pixel[][] newPixels){
		if (newPixels != null){
			Pixel p = null;
			Pixel[][] otherPixels = new Pixel[newPixels.length][newPixels[0].length];

			// goes through each pixel and deep copies
			for (int r = 0; r < newPixels.length; r++)
				for (int c = 0; c < newPixels[0].length; c++)
					otherPixels[r][c] = new Pixel(newPixels[r][c]);
			pixels = otherPixels;
		}
	}

	/**
	* Returns a deep copy of instance variable pixels
	* @return deep copy of the instance variable pixels
	*/
	public Pixel[][] getPixels(){
		if (pixels != null){
			Pixel[][] temp = new Pixel[pixels.length][pixels[0].length];

			// goes through each pixel and deep copies it
			for (int r = 0; r < pixels.length; r++)
				for (int c = 0; c < pixels[0].length; c++)
					temp[r][c] = new Pixel(pixels[r][c]);
			return temp;
		}
		return null;
	}

	// image operations
	/**
	* Flattens the color in the image
	* @param color color to flatten
	* @throws PPMException if color is not one of "red", "green", "blue", ignores upper/lower case
	*/
	public void flattenColor(String color) throws PPMException{
		if (pixels != null){
			if (!((color.equalsIgnoreCase("red") || color.equalsIgnoreCase("green")) || color.equalsIgnoreCase("blue")))
				throw new PPMException("Invalid color to flatten.");

			// goes through each pixel and changes the color according to the parameter color
			for (int r = 0; r < pixels.length; r++)
				for (int c = 0; c < pixels[0].length; c++){
					if (color.equalsIgnoreCase("red"))
						pixels[r][c].setRed(0);
					else if (color.equalsIgnoreCase("green"))
						pixels[r][c].setGreen(0);
					else if (color.equalsIgnoreCase("blue"))
						pixels[r][c].setBlue(0);
				}
		}
	}

	/**
	* Negates the color in the image
	* @param color color to negate
	* @throws PPMException if the color is not one of "red", "green", "blue", ignores upper/lower case
	*/
	public void negateColor(String color) throws PPMException{
		if (pixels != null){
			if (!((color.equalsIgnoreCase("red") || color.equalsIgnoreCase("green")) || color.equalsIgnoreCase("blue")))
				throw new PPMException("Invalid color to negate.");

			// goes through each pixel and changes the color according to the parameter color
			for (int r = 0; r < pixels.length; r++)
				for (int c = 0; c < pixels[0].length; c++){
					if (color.equalsIgnoreCase("red"))
						pixels[r][c].setRed((maxColor - pixels[r][c].getRed()));
					else if (color.equalsIgnoreCase("green"))
						pixels[r][c].setGreen((maxColor - pixels[r][c].getGreen()));
					else if (color.equalsIgnoreCase("blue"))
						pixels[r][c].setBlue((maxColor - pixels[r][c].getBlue()));
				}
		}
	}

	/**
	* Makes the image in grey scale
	*/
	public void greyScale(){
		if (pixels != null)
			// goes through each pixel and replaces the color with the average of the current RGB value
			for (int r = 0; r < pixels.length; r++)
				for (int c = 0; c < pixels[0].length; c++)
					pixels[r][c].setAll((pixels[r][c].getRed() + pixels[r][c].getGreen() + pixels[r][c].getBlue()) / 3);
	}

	/**
	* Tiles the image horizontally/vertically by the factors xRatio/yRatio, respectively
	* @param xRatio number of horizontal copies
	* @param yRatio number of vertical copies
	* @throws PPMException of xRatio <= 0 or yRatio <= 0
	*/
	public void tile(int xRatio, int yRatio) throws PPMException{
		if ((xRatio <= 0) || (yRatio <= 0))
			throw new PPMException((xRatio <= 0) ? "X ratio is less than or equal to 0." : "Y ratio is less than or equal to 0.");
		int rows = pixels.length, cols = pixels[0].length;
		Pixel[][] newPixels = new Pixel[rows * yRatio][cols * xRatio];

		// goes through each pixel and then sends it to each tile
		for (int r = 0; r < rows; r++)
			for (int c = 0; c < cols; c++){
				Pixel temp = new Pixel(pixels[r][c]);
				for (int x = 0; x < xRatio; x++)
					for (int y = 0; y < yRatio; y++)
						newPixels[r + (rows * y)][c + (cols * x)] =  temp;
			}
		pixels = newPixels;
	}

	/**
	* Scales the image horizontally and vertically by the factors xRatio and yRatio, respectively
	* @param xRatio horizontal scale of the image
	* @param yRatio vertical scale of the image
	* @throws PPMException if xRatio or yRatio <= 0
	*/
	public void scale(double xRatio, double yRatio) throws PPMException{
		if ((xRatio <= 0) || (yRatio <= 0))
			throw new PPMException((xRatio <= 0) ? "X ratio is less than or equal to 0." : "Y ratio is less than or equal to 0.");
		int newRow = (int)(pixels.length * yRatio), newCol = (int)(pixels[0].length * xRatio);
		Pixel[][] newPixels = new Pixel[newRow][newCol];

		for (int r = 0; r < newRow; r++)
			for (int c = 0; c < newCol; c++)
				newPixels[r][c] = new Pixel(pixels[(int)(r / yRatio)][(int)(c / xRatio)]);
		pixels = newPixels;
	}

	/**
	* Saves the image in the file fileName. Should write the header and body of the image.
	* @param fileName file to be used
	* @throws PPMException if anything goes wrong during saving the image to the file
	*/
	public void save(String fileName) throws PPMException{
		if (pixels == null)
			throw new PPMException("Tried to save an empty image.");
		PrintWriter outputStream;
		try{
			outputStream = new PrintWriter(new FileOutputStream(fileName));
		}
		catch (FileNotFoundException e){
			throw new PPMException(e.getMessage());
		}

		// header
		// tells the renderer how to read the file
		outputStream.println(imageType + "\n" + pixels[0].length + " " + pixels.length + "\n" + maxColor);

		// goes through each pixel and prints out its RGB value
		for (int r = 0; r < pixels.length; r++){
			for (int c = 0; c < pixels[0].length; c++)
				outputStream.print("" + pixels[r][c].getRed() + " " + pixels[r][c].getGreen() + " " + pixels[r][c].getBlue() + "   ");
			outputStream.println();
		}
		outputStream.close();
	}
}
