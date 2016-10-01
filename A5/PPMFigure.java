/**
* A class to create simple PPM images.
*
* @author David Sepulveda
* @version 1.0, April 10, 2015
*/

public class PPMFigure extends PPMImage {

	// constructors
	/**
	* Creates an image of dimensions rows*cols by initializing the array pixels.
	* Sets the color of every pixel of the image to "white".
	* @param rows number of rows
	* @param cols number of columns
	* @throws PPMException if rows <= 0 or cols <= 0
	*/
	public PPMFigure(int rows, int cols) throws PPMException{
		super(); // sets maxColor, imageType, and pixels
		if ((rows <= 0) || cols <= 0)
			throw new PPMException((rows <= 0) ? "Rows is less than or equal to 0." : "Rows is less than or equal to 0.");
		Pixel p = new Pixel(maxColor, maxColor, maxColor);
		pixels = new Pixel[rows][cols]; // adjusts the pixels array to have the correct demensions
		for (int r = 0; r < rows; r++)
			for (int c = 0; c < cols; c++)
				setPixelAt(r, c, p);
	}

	/**
	* Creates an image of dimensions rows*cols.
	* Sets the color of every pixel of the image to color.
	* Precondition: color is not null
	* @param rows number of rows
	* @param cols number of columns
	* @param color color to be set
	* @throws PPMException if rows <= 0 or cols <= 0
	*/
	public PPMFigure(int rows, int cols, Pixel color) throws PPMException{
		super(); // sets maxColor, imageType, and pixels
		if ((rows <= 0) || cols <= 0)
			throw new PPMException((rows <= 0) ? "Rows is less than or equal to 0." : "Rows is less than or equal to 0.");
		pixels = new Pixel[rows][cols]; // adjusts the pixels array to have the correct demensions
		clear(color);
	}

	// drawing methods
	/**
	* Clears the image by setting the value of every pixel in the image to color
	* Precondition: color is not null
	* @param color color to be set
	*/
	public void clear(Pixel color){
		if (color != null){
			for (int r = 0; r < pixels.length; r++)
				for (int c = 0; c < pixels[0].length; c++)
					setPixelAt(r, c, color);
		}
	}

	/**
	* Draws a straight line between (startRow, startCol) and (endRow, endCol) with the specified color
	* Precondition: color is not null
	* @param startRow row index of the first pixel
	* @param startCol column index of the first pixel
	* @param endRow row index of the second pixel
	* @param endCol column index of the second pixel
	* @param color color of the line connecting the two pixels
	* @return false, if the start point or end point is outside the image coordinates; otherwise true
	*/
	public boolean drawLine(int startRow, int startCol, int endRow, int endCol, Pixel color){
		try{ // if indices are out of bounds, returns false.
			Pixel p = pixels[startRow][startCol];
			p = pixels[endRow][endCol];
		}
		catch (IndexOutOfBoundsException e){
			return false;
		}

		// loop reads from left to right, if the second pixel is to the left of the first pixel
		// then the loop won't work. This if statement ensures that the first pixel is on the left
		// and interchanges them if they are in the wrong order.
			if (startCol > endCol){
				int temp = startCol;
				startCol = endCol;
				endCol = temp;
				temp = startRow;
				startRow = endRow;
				endRow = temp;
			}
			double dx = endCol - startCol;
			double dy = endRow - startRow;
			// draws the line
			for (int col = startCol; col <= endCol; col++){
				int row = (int)Math.round(startRow + (col - startCol) * (dy / dx));
				pixels[row][col] = new Pixel(color);
			}
			return true;
	}

	/**
	* Draws a rectangle from the point (startRow, startCol) with the given width and height.
	* Parameter color specifies the fill color of the rectangle.
	* Precondition: color is not null
	* Precondition: width > 0 and height > 0
	* @param startRow row index of the first pixel
	* @param startCol column index of the first pixel
	* @param width the width of the rectangle
	* @param height the height of the rectangle
	* @param color the color of the rectangle
	* @return false, if any part of the rectangle is outside the image coordinates; otherwise true
	*/
	public boolean drawRectangle(int startRow, int startCol, int width, int height, Pixel color){
		boolean b = true; // default true because drawline will return false if anything is invalid
		int endRow = startRow + height, endCol = startCol + (width - 1);
		for (int r = startRow; r < endRow; r++) // creates lines right next to each other in the dimensions of the rectangle
			b = drawLine(r, startCol, r, endCol, color);
		return b;
	}
}
// #semesterOver
