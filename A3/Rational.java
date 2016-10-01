
/**
 * A class to represent rational numbers.
 *
 * @author David Sepulveda
 * @version 1.0, March 13, 2015
 */

public class Rational {

	// instance variables
	private int numerator;
	private int denominator;

	// constructors

	/**
	 * This constructor creates a default rational of numerator 0 and denominator 1.
	 */
	public Rational(){
		numerator = 0;
		denominator = 1;
	}

	/**
	 * This constructor creates a rational of denominator 1 with the integer parameter.
	 * @param wholeNumber the numerator of the rational
	 */
	public Rational(int wholeNumber){
		numerator = wholeNumber;
		denominator = 1;
	}

	/**
	 * This constructor takes in two integer parameters and creates a rational object of them.
	 * @param  numerator the numerator of the rational
	 * @param  denominator the denomintor of the rational
	 */
	public Rational(int numerator, int denominator){
		if (denominator == 0){ // cannot divide by 0
			System.out.println("Denominator cannot be 0.");
			System.exit(0);
		}
		else{ // arranges in a way that the negative will be on the numerator and then creates the rational
			this.numerator = ((denominator > 0) ? (numerator) : (-1 * numerator));
			this.denominator = ((denominator > 0) ? (denominator) : (-1 * denominator));
			this.simplify();
		}
	}

	/**
	 * This constructor takes in a rational parameter and makes a copy of it.
	 * @param other rational that will be copied
	 */
	public Rational(Rational other){ // copy constructor
		if (other != null){
			numerator = other.getNumerator();
			denominator = other.getDenominator();
		}
		else{
			System.out.println("Cannot copy a null object.");
			System.exit(0);
		}
	}

	// static methods

	/**
	 * This method adds together the two rational parameters.
	 * @param  r1 rational that will be have the second rational parameter added to it
	 * @param  r2 rational that will be added to the first rational parameter
	 * @return rational sum of the parameters
	 */
	public static Rational add(Rational r1, Rational r2){
		Rational r3 = new Rational();
		if ((r1 != null) && (r2 != null)){ // adds using a common denominator
			int newNum = ((r1.getNumerator() * r2.getDenominator()) + (r2.getNumerator() * r1.getDenominator()));
			r3 = new Rational(newNum, (r1.getDenominator() * r2.getDenominator()));
			r3.simplify();
		}
		else{
			System.out.println("Cannot add null objects.");
			System.exit(0);
		}
		return r3;
	}

	/**
	 * This method subtracts the second rational parameter from the first rational parameter.
	 * @param  r1 rational that will have the second rational parameter subtracted from it
	 * @param  r2 rational that will be subtracted from the firt rational parameter
	 * @return rational difference of the parameters
	 */
	public static Rational subtract(Rational r1, Rational r2){
		Rational r3 = new Rational();
		if ((r1 != null) && (r2 != null)){ // subtracts using a common denominator
			int newNum = ((r1.getNumerator() * r2.getDenominator()) - (r2.getNumerator() * r1.getDenominator()));
			r3 = new Rational(newNum, (r1.getDenominator() * r2.getDenominator()));
			r3.simplify();
		}
		else{
			System.out.println("Cannot subtract null objects.");
			System.exit(0);
		}
		return r3;
	}

	/**
	 * This method multiplies the two rational parameters.
	 * @param  r1 rational that will multiply with the second rational parameter
	 * @param  r2 rational that will multiply with the first rational parameter
	 * @return rational that is the product of the two rational parameters
	 */
	public static Rational multiply(Rational r1, Rational r2){
		Rational r3 = new Rational();
		if ((r1 != null) && (r2 != null)){ // creates new rational with the multiplied rationals
			r3 = new Rational((r1.getNumerator() * r2.getNumerator()), (r1.getDenominator() * r2.getDenominator()));
			r3.simplify();
		}
		else{
			System.out.println("Cannot multiply null objects.");
			System.exit(0);
		}
		return r3;
	}

	/**
	 * This method divides the first rational parameter by the second rational parameter.
	 * @param  r1 rational that will be divided by second rational parameter
	 * @param  r2 rational that will divie the first rational parameter
	 * @return rational that is the rational quotient of the two rational parameters
	 */
	public static Rational divide(Rational r1, Rational r2){
		Rational r3 = new Rational();
		if ((r1 != null) && (r2 != null)){
			r3 = new Rational((r1.getNumerator() * r2.getDenominator()), (r1.getDenominator() * r2.getNumerator()));
			r3.simplify();
		}
		else{
			System.out.println("Cannot divide null objects.");
			System.exit(0);
		}
		return r3;
	}

	/**
	 * This method finds the reciprocal of the rational parameter.
	 * @param  r rational that will be reciprocated
	 * @return rational reciprocal of the rational parameter
	 */
	public static Rational reciprocate(Rational r){
		Rational r3 = new Rational();
		if ((r.getNumerator() != 0) && (r != null)) // creates a new rational with the denominator and numerator switched
			r3 = new Rational((r.getDenominator() * ((r.getNumerator() < 0) ? -1 : 1)), (r.getNumerator() * ((r.getNumerator() < 0) ? -1 : 1)));
		else{ // if the numerator is negative then hte rational is negated so the negative will remain on top
			System.out.println((r == null) ? "Cannot reciprocate a null object." : "Rationals of value 0 cannot be reciprocated.");
			System.exit(0);
		}
		return r3;
	}

	/**
	 * This method negates the rational parameter.
	 * @param  r rational that will be negated
	 * @return negation of the rational parameter
	 */
	public static Rational negate(Rational r){
		Rational r3 = new Rational();
		if (r != null){
			int newNum = r.getNumerator() * -1;
			int newDenom = r.getDenominator();
			r3 = new Rational(newNum, newDenom);
		}
		else{
			System.out.println("Cannot negate a null object.");
			System.exit(0);
		}
		return r3;
	}


	// non-static methods

	/**
	 * This method adds the rational parameter onto the calling rational.
	 * @param  r rational that will be added onto the calling rational
	 */
	public void add(Rational r){
		if (r != null){
			numerator = (numerator  * r.getDenominator()) + (r.getNumerator()  * denominator);
			denominator *= r.getDenominator(); // sets a common base
			this.simplify();
		}
		else{
			System.out.println("Cannot add a null object.");
			System.exit(0);
		}
	}

	/**
	 * This method subtracts the rational parameter from the calling rational.
	 * @param  r rational that will be subtracted from the calling rational
	 */
	public void subtract(Rational r){
		if (r != null){
			numerator = (numerator * r.getDenominator()) - (r.getNumerator() * denominator);
			denominator *= r.getDenominator(); // sets a common base
			this.simplify();
		}
		else{
			System.out.println("Cannot subtract null objects.");
			System.exit(0);
		}
	}

	/**
	 * This method multiplies the calling rational by the rational parameter.
	 * @param r rational that will multiply our calling rational
	 */
	public void multiply(Rational r){
		if (r != null){
			numerator *= r.getNumerator(); // multiplies
			denominator *= r.getDenominator();
			this.simplify();
		}
		else{
			System.out.println("Cannot multiply null objects.");
			System.exit(0);
		}
	}

	/**
	 * This method divides the calling rational by the rational parameter.
	 * @param  r rational that will divide our calling rational
	 */
	public void divide(Rational r){
		if ((r != null) && (r.getNumerator() != 0)){
			numerator *= r.getDenominator(); // dividng by a rational is equivalent to multiplying by
			denominator *= r.getNumerator(); // the inverse rational
			this.simplify();
		}
		else{
			System.out.println((r == null) ? "Cannot divide a null object." : "Cannot divive by 0.");
			System.exit(0);
		}

	}

	/**
	 * This method finds the reciprocal of the calling rational.
	 */
	public void reciprocate(){
		if (numerator != 0){ // interchanges the numerator and denominator
			int temp = numerator * ((numerator < 0) ? -1 : 1); // if the denominator would be negative then it is
			numerator = denominator * ((numerator < 0) ? -1 : 1); // made positive and the numerator is made negative
			denominator = temp;
		}
		else{ // cannot divide by 0
			System.out.println("Rationals of value 0 cannot be reciprocated.");
			System.exit(0);
		}
	}

	/**
	 * This method negates the calling rational.
	 */
	public void negate(){
		numerator = -1 * numerator;
	}

	// accessor and mutator methods

	/**
	 * The mutator method for the private variable numerator.
	 * <p>
	 * The method takes in the new numerator and then checks if this change will affect the gcd of the rational
	 * and then adjusts accordingly.
	 */
	public void setNumerator(int numerator){
		this.numerator = numerator;
		this.simplify();
	}

	/**
	 * The accessor method for the private variable numerator.
	 * @return the numerator of our calling rational
	 */
	public int getNumerator(){
		return numerator;
	}

	/**
	 * The mutator method for the private variable denominator.
	 * <p>
	 * The method takes in the new denominator and then checks if this change will affect the gcd of the rational
	 * and then adjusts accordingly.
	 */
	public void setDenominator(int denominator){
		if (denominator == 0) // cannot divide by 0
			System.out.println("Denominator cannot be 0.");
		else{ // sets denominator, taking into account negatives
			this.denominator = ((denominator > 0) ? denominator : (-1 * denominator));
			this.numerator *= ((denominator > 0) ? 1 : -1);
			this.simplify();
		}
	}

	/**
	 * The accessor method for the private variable denominator.
	 * @return the denominator of our calling rational
	 */
	public int getDenominator(){
		return denominator;
	}

	// additional methods

	/**
	 * This method finds the greatest common denominator of our calling rational and then divides by it.
	 */
	public void simplify(){
		int a = numerator, b = denominator;
		while (b != 0){ // finds the greatest common denominator
			int t = b;
			b = a % b;
			a = t;
		}

		a = Math.abs(a); // makes sure the gcd is positive
		numerator = numerator / a; // reduces the numerator
		denominator = denominator / a; // reduces the denominator
	}

	/**
	 * This method turns our rational into a string.
	 * @return a string of our rational
	 */
	public String toString(){
		return (numerator + "/" + denominator); // "x/y"
	}

	/**
	 * This method decides whether or not two objects are equal to each other.
	 * @return a boolean of whether or not the two objects are equal
	 */
	public boolean equals(Rational other){
		if (other != null)
			return ((numerator == other.getNumerator()) && (denominator == other.getDenominator()));
		else // if object is null, then false automatically
			return false;
	}

}
