
/**
* PPMException Class to be used with PPMImage and PPMFigure classes
*
* @author 	Majid Ghaderi
* @version	1.0, Mar 5, 2015
*/
public class PPMException extends RuntimeException {

    /**
    * Constructor calls the super class with default message
    */
	public PPMException() {
		super("PPM exception");
	}

    /**
    * Constructor calls super class with user provided message
    * @param message The message of the exception
    */
	public PPMException(String message) {
		super(message);
	}
}
