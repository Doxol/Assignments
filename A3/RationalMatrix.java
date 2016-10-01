
/**
 * A class to represent matrices of rational numbers.
 *
 * @author	David Sepulveda
 * @version	1.0, March 13, 2015
 */

public class RationalMatrix {

	// instance variables
	private Rational[][] matrix;

	// constructors

	/**
	* This constructor takes in two integer parameters and creates a rational matrix of them.
	* @param  rows the number of rows in the matrix
	* @param  columns the number of columns in the matrix
	*/
	public RationalMatrix(int rows, int columns){
		matrix = new Rational[rows][columns];
		for (int r = 0; r < rows; r++){
			for (int c = 0; c < columns; c++){
				matrix[r][c] = new Rational();
			}
		}
	}

	/**
	 * This constructor takes in the rows and columns of the matrix as integer parameters, creates a matrix, and
	 * initiates each value in the matrix to the rational parameter initial.
	 * @param  rows the number of rows in the matrix
	 * @param  columns the number of columns in the matrix
	 * @param  initial the rational value that each index of the matrix will be initialized to
	 */
	public RationalMatrix(int rows, int columns, Rational initial){
		matrix = new Rational[rows][columns];
		for (int r = 0; r < rows; r++){
			for (int c = 0; c < columns; c++){
				Rational newRational = new Rational(initial);
				matrix[r][c] = newRational;
			}
		}
	}

	/**
	* This constructor takes in a 2d list of rationals and makes a rational matrix of them.
	* @param  matrix 2d list of rations that will be turned into a matrix
	*/
	public RationalMatrix(Rational[][] matrix){
		this.matrix = new Rational[matrix.length][matrix[0].length];
		for (int r = 0; r < matrix.length; r++){
			for (int c = 0; c < matrix[0].length; c++){
				Rational newRational = new Rational(matrix[r][c]);
				this.matrix[r][c] = newRational;
			}
		}
	}

	/**
	 * This constructor takes in a rational matrix parameter and makes a copy of it.
	 * @param other rational matrix that will be copied
	 */
	public RationalMatrix(RationalMatrix other){ // copy constructor
		if (other != null){
			matrix = other.getMatrix();
		}

		else{
			System.out.println("Cannot copy a null object.");
			System.exit(0);
		}
	}

	// static methods

	/**
	 * A static method to add together two matrices.
	 * @param  m1 first matrix to be added
	 * @param  m2 second matrix to be added
	 * @return a new matrix that is the sum of the two parameter matrices
	 */
	public static RationalMatrix add(RationalMatrix m1, RationalMatrix m2){ // addition
		RationalMatrix temp = new RationalMatrix(m1.getMatrix().length, m1.getMatrix()[0].length);
		if (((m1.getMatrix().length == m2.getMatrix().length) && (m1.getMatrix()[0].length == m2.getMatrix()[0].length)) && ((m1 != null) && (m2 != null))){
			temp = new RationalMatrix(m1.getMatrix().length, m1.getMatrix()[0].length);
			for (int r = 0; r < m1.getMatrix().length; r++){
				for (int c = 0; c < m1.getMatrix()[0].length; c++){
					temp.setAt(r, c, Rational.add(m1.getAt(r, c), m2.getAt(r, c)));
				}
			}
		}

		else{
			System.out.println(((m1 == null) || (m2 == null)) ? "Cannot add null objects." : "These matrices must have the same number of rows and the same number of columns to be added.");
			System.exit(0);
		}

		return temp;
	}

	/**
	 * A static method to subtract a matrix from another matrix.
	 * @param  m1 first matrix
	 * @param  m2 matrix to be subtracted
	 * @return a new matrix that is the difference between the two parameter matrices
	 */
	public static RationalMatrix subtract(RationalMatrix m1, RationalMatrix m2){ // subtraction
		RationalMatrix temp = new RationalMatrix(m1.getMatrix().length, m1.getMatrix()[0].length);
		if (((m1.getMatrix().length == m2.getMatrix().length) && (m1.getMatrix()[0].length == m2.getMatrix()[0].length)) && ((m1 != null) && (m2 != null))){
			temp = new RationalMatrix(m1.getMatrix().length, m1.getMatrix()[0].length);
			for (int r = 0; r < m1.getMatrix().length; r++){
				for (int c = 0; c < m1.getMatrix()[0].length; c++){
					temp.setAt(r, c, Rational.subtract(m1.getAt(r, c), m2.getAt(r, c)));
				}
			}
		}

		else{
			System.out.println(((m1 != null) && (m2 != null)) ? "These matrices must have the same number of rows and the same number of columns to be subtracted." : "Cannot subtract null objects.");
			System.exit(0);
		}

		return temp;
	}

	/**
	 * A static method to multiply a matrix my a scalar multiple.
	 * @param  m1 matrix to be multiplied
	 * @param  m2 scalar rational
	 * @return a new matrix that is the product of the parameter matrix and the scalar rational
	 */
	public static RationalMatrix multiply(RationalMatrix m, Rational r){ // scalar multiplication
		RationalMatrix temp = new RationalMatrix(m.getMatrix().length, m.getMatrix()[0].length);
		if ((m != null) && (r != null)){
			temp = new RationalMatrix(m.getMatrix().length, m.getMatrix()[0].length);
			for (int rows = 0; rows < m.getMatrix().length; rows++){
				for (int columns = 0; columns < m.getMatrix()[0].length; columns++){
					temp.setAt(rows, columns, Rational.multiply(m.getAt(rows, columns), r));
				}
			}
		}

		else{
			System.out.println("Cannot multiply null objects.");
			System.exit(0);
		}

		return temp;
	}

	/**
	 * A static method to transpose a matrix.
	 * @param  m  matrix to be transposed
	 * @return a new matrix that is the transpose of the parameter matrix
	 */
	public static RationalMatrix transpose(RationalMatrix m){
		RationalMatrix temp = new RationalMatrix(m.getMatrix().length, m.getMatrix()[0].length);
		if (m != null){
			temp = new RationalMatrix(m.getMatrix()[0].length, m.getMatrix().length);
			for (int r = 0; r < temp.getMatrix().length; r++){
				for (int c = 0; c < temp.getMatrix()[0].length; c++){
					temp.setAt(r, c, m.getAt(c, r)) ;
				}
			}
		}

		else{
			System.out.println("Cannot transpose a null object.");
			System.exit(0);
		}

		return temp;
	}

	/**
	 * A static method to negate a matrix.
	 * @param  m1 matrix to be negated
	 * @return a new matrix that is the negated parameter matrix
	 */
	public static RationalMatrix negate(RationalMatrix m){
		RationalMatrix temp = new RationalMatrix(m.getMatrix().length, m.getMatrix()[0].length);
		if (m != null){
			for (int r = 0; r < m.getMatrix().length; r++){
				for (int c = 0; c < m.getMatrix()[0].length; c++){
					temp.setAt(r, c, Rational.negate(m.getAt(r, c)));
				}
			}
		}

		else{
			System.out.println("Cannot negate a null object.");
			System.exit(0);
		}

		return temp;
	}

	// non-static methods

	/**
	 * A method to add a matrix onto your calling matrix.
	 * @param  m matrix to be added
	 */
	public void add(RationalMatrix m){ // addition
		if ((m != null) && (!((m.getMatrix().length != this.matrix.length) || (m.getMatrix()[0].length != this.matrix[0].length)))){
			for (int r = 0; r < m.getMatrix().length; r++){
				for (int c = 0; c < m.getMatrix()[0].length; c++){
					this.setAt(r, c, Rational.add(this.getAt(r, c), m.getAt(r, c)));
				}
			}
		}

		else{
			System.out.println("These matrices must have the same number of rows and the same number of columns to be subtracted.");

		}
	}

	/**
	 * A method to subtract a matrix from your calling matrix.
	 * @param  m matrix to be subtracted
	 */
	public void subtract(RationalMatrix m){ // subtraction
		if ((m != null) && (!((m.getMatrix().length != this.matrix.length) || (m.getMatrix()[0].length != this.matrix[0].length)))){
			for (int r = 0; r < m.getMatrix().length; r++){
				for (int c = 0; c < m.getMatrix()[0].length; c++){
					this.setAt(r, c, Rational.subtract(this.getAt(r, c), m.getAt(r, c)));
				}
			}
		}

		else{
			System.out.println("These matrices must have the same number of rows and the same number of columns to be subtracted.");

		}
	}

	/**
	 * A method to multiply your calling matrix by a scalar rational.
	 * @param  r rational to be multiplied
	 */
	public void multiply(Rational r){ // scalar multiplication
		if (r != null){
			for (int rows = 0; rows < matrix.length; rows++){
				for (int c = 0; c < matrix[0].length; c++){
					matrix[rows][c].multiply(r);
				}
			}
		}

		else{
			System.out.println("Cannot multiply a null object.");

		}
	}

	/**
	 * A method to transpose a matrix.
	 */
	public void transpose(){
		if (this != null){
			Rational[][] temp = new Rational[matrix[0].length][matrix.length];
			for (int r = 0; r < matrix[0].length; r++){
				for (int c = 0; c < matrix.length; c++){
					temp[r][c] = new Rational(matrix[c][r]);
				}
			}

			matrix = temp;
		}

		else{
			System.out.println("Cannot transpose a null object.");
			System.exit(0);
		}
	}

	/**
	 * A method to negate a matrix.
	 */
	public void negate(){
		if (this != null){
			for (int r = 0; r < matrix.length; r++){
				for (int c = 0; c < matrix[0].length; c++){
					matrix[r][c].negate();
				}
			}
		}

		else{
			System.out.println("Cannot negate a null object.");
			System.exit(0);
		}
	}

	// accessor and mutator methods

	/**
	 * The mutator method for the private variable matrix.
	 * @param  m the rational matrix which our matrix will be set to
	 */
	public void setMatrix(Rational[][] m){
		if ((m != null) && !((m.length > matrix.length) || (m[0].length > matrix[0].length))){
			for (int r = 0; r < m.length; r++){
				for (int c = 0; c < m[0].length; c++){
					Rational newRational = new Rational(m[r][c]);
					this.setAt(r, c, newRational);
				}
			}
		}
	}

	/**
	 * The accesor method for the private variable matrix.
	 * @return a copy of our calling matrix
	 */
	public Rational[][] getMatrix(){
		Rational[][] temp = new Rational[matrix.length][matrix[0].length];
		for (int r = 0; r < matrix.length; r++){
			for (int c = 0; c < matrix[0].length; c++){
				Rational newRational = new Rational(matrix[r][c]);
				temp[r][c] = newRational;
			}
		}

		return temp;
	}

	/**
	 * The mutator method for the specific rationals within our calling matrix.
	 * @param  row the row in which the rational we want to adjust is in
	 * @param  column the column in which the rational we want to adjust is in
	 */
	public void setAt(int row, int column, Rational r){
		if (!((row >= matrix.length ) || (column >= matrix[0].length))){
			Rational x = new Rational(r);
			this.matrix[row][column] = x;
		}

		else{
			System.out.println("Index array out of bounds.");
			System.exit(0);
		}
	}

	/**
	 * The accessor method for the specific rationals within our calling matrix.
	 * @param  row the row in which the desired rational is in
	 * @param  column the column in which the desired ration is in
	 * @return a copy of the desired rational
	 */
	public Rational getAt(int row, int column){
		Rational x = new Rational();
		if (!((row >= matrix.length) || (column >= matrix[0].length))){
			x = new Rational(this.matrix[row][column]);
		}

		else{
			System.out.println("Index array out of bounds.");
			System.exit(0);
		}

		return x;
	}

	// additional methods

	/**
	 * This method checks if two rational matrices are equal in their contents.
	 * @param  other the object we are comparing to our calling object
	 * @return a boolean that states whether or not the two objects are equal
	 */
	public boolean equals(RationalMatrix other){
		boolean equal = false;
		if ((other != null) && ((other.getMatrix().length == matrix.length) && (other.getMatrix()[0].length == matrix[0].length))){
			equal = true;

			for (int r = 0; r < matrix.length; r++){
				for (int c = 0; c < matrix[0].length; c++){
					if (!(this.getAt(r, c).equals(other.getAt(r, c)))){
						equal = false;
					}
				}
			}
		}

		return equal;
	}

	/**
	 * This method turns our rational matrix into a string.
	 * @return a string of the matrix of our calling object
	 */
	public String toString(){
		String temp = "";

		for (int r = 0; r < matrix.length; r++){
			for (int c = 0; c < matrix[0].length; c++){
				temp += "" + ((c == (matrix[0].length - 1)) ? matrix[r][c] + "\n" : matrix[r][c] + " ");
			}
		}

		return temp;
	}
}
