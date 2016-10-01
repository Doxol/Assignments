/**
* Creates a class to represent a pixel and holds the RGB values of that pixel.
*
* @author David Sepulveda
* @version 1.0, April 10, 2015
*/

public class Pixel {
	private int red, green, blue;

	/**
	* Default no argument constructor.
	*/
	public Pixel(){
		red = green = blue = 255;
	}

	/**
	* Constructor taking in the integer RGB values of the pixel.
	*
	* @param red the red value for the pixel
	* @param green the green value for the pixel
	* @param blue the blue value for the pixel
	*/
	public Pixel(int red, int green, int blue){
		this.red = red;
		this.green = green;
		this.blue = blue;
	}

	/**
	* Copy constructor that takes in a parameter of type pixel and copies it.
	*
	* @param p the pixel to be copied
	*/
	public Pixel(Pixel p){
		red = p.getRed();
		green = p.getGreen();
		blue = p.getBlue();
	}

	/**
	* Accesor method for the red value of the class.
	*/
	public int getRed(){
		return red;
	}

	/**
	* Accesor method for the green value of the class.
	*/
	public int getGreen(){
		return green;
	}

	/**
	* Accesor method for the blue value of the class.
	*/
	public int getBlue(){
		return blue;
	}

	/**
	* Mutator method for the red value of the class.
	*
	* @param red the value to set red to
	*/
	public void setRed(int red){
		this.red = red;
	}

	/**
	* Mutator method for the green value of the class.
	*
	* @param green the value to set green to
	*/
	public void setGreen(int green){
		this.green = green;
	}

	/**
	* Mutator method for the blue value of the class.
	*
	* @param blue the value to set blue to
	*/
	public void setBlue(int blue){
		this.blue = blue;
	}

	/**
	* Mutator method for every value in the class.
	* @param avg value to set red, blue, and green to
	*/
	public void setAll(int avg){
		red = green = blue = avg;
	}
}
