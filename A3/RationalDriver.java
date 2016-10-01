/**
 * The driver to test Rational.java
 * @author Zain Rizvi
 *
 */
public class RationalDriver {
	
	//Variables to hold values for grades per categort
	private int c1, c2, c3, c4, c5, c6, c7;
	
	public static void main(String[] args){
		
		RationalDriver a3 = new RationalDriver();
		
		//Call all test in order
		a3.constructorTests();
		a3.staticAddTests();
		a3.staticSubTests();
		a3.staticMulTests();
		a3.staticDivTests();
		a3.addTests();
		a3.subTests();
		a3.mulTests();
		a3.divTests();
		a3.staticRecipTests();
		a3.staticNegateTests();
		a3.recipTests();
		a3.negateTests();
		a3.accessorTests();
		a3.mutatorTests();
		a3.toStringTests();
		a3.equalsTests();
		//Print summary
		a3.summary();
	}
	
	/**
	 * Tests for 4 Constructors
	 */
	public void constructorTests(){
		System.out.println("CONSTRUCTOR TESTS");
		
		
		//Test 1: Default constructor
		Rational r = new Rational();
		System.out.println("Test 1");
		if (r.getNumerator()==0 && r.getDenominator()==1){
			System.out.println("Correct\n\n");
			c1++;
		} else {
			System.out.println("FAIL\n\n");
		}
		
		//Test 2: wholeNumber constructor
		r = new Rational(5);
		System.out.println("Test 2");
		if (r.getNumerator()==5 && r.getDenominator()==1){
			System.out.println("Correct\n\n");
			c1++;
		} else {
			System.out.println("FAIL\n\n");
		}
		
		//Test 3: 2 value constructor
		r = new Rational(2, -6);
		System.out.println("Test 3");
		if (r.getNumerator()==-1 && r.getDenominator()==3){
			System.out.println("Correct\n\n");
			c1++;
		} else {
			System.out.println("FAIL\n\n");
		}
		
		//Test 4: Copy constructor
		r = new Rational(1,2);
		Rational r1 = new Rational(r);
		System.out.println("Test 4");
		if (r1.getNumerator()==1 && r.getDenominator()==2){
			System.out.println("Correct\n\n");
			c1++;
		} else {
			System.out.println("FAIL\n\n");
		}
		
	}
	
	/**
	 * Tests for Static Add method
	 */
	public void staticAddTests(){
		
		System.out.println("STATIC ADD TESTS");
		int temp = 0;
		
		//Test 1: Same denominator. No simplification needed
		Rational r1 = new Rational(1,5);
		Rational r2 = new Rational(2,5);
		Rational r3 = Rational.add(r1, r2);
		System.out.println("Test 1");
		System.out.println("Expected: 1/5 + 2/5 = 3/5");
		System.out.println("Result: 1/5 + 2/5 = " + r3);
		if (r3.getNumerator() == 3 && r3.getDenominator() == 5){
			System.out.println("Correct\n\n");
			temp++;
		} else {
			System.out.println("FAIL\n\n");
		}
		
		//Test 2: Simplification needed
		r1 = new Rational(13,-24);
		r2 = new Rational(1,12);
		r3 = Rational.add(r1, r2);
		System.out.println("Test 2");
		System.out.println("Expected: 13/-24 + 1/12 = -11/24");
		System.out.println("Result: 13/-24 + 1/12 = " + r3);
		if (r3.getNumerator() == -11 && r3.getDenominator() == 24){
			System.out.println("Correct\n\n");
			temp++;
		} else {
			System.out.println("FAIL\n\n");
		}
		
		//Test 3
		r1 = new Rational(8,1);
		r2 = new Rational(2,1);
		r3 = Rational.add(r1, r2);
		System.out.println("Test 3");
		System.out.println("Expected: 8/1 + 2/1 = 10/1");
		System.out.println("Result: 8/1 + 2/1 = " + r3);
		if (r3.getNumerator() == 10 && r3.getDenominator() == 1){
			System.out.println("Correct\n\n");
			temp++;
		} else {
			System.out.println("FAIL\n\n");
		}
		
		//Test 4
		r1 = new Rational(25,-4);
		r2 = new Rational(12,11);
		r3 = Rational.add(r1, r2);
		System.out.println("Test 4");
		System.out.println("Expected: 25/-4 + 12/11 = -227/44");
		System.out.println("Result: 25/-4 + 12/11 = " + r3);
		if (r3.getNumerator() == -227 && r3.getDenominator() == 44){
			temp++;
			System.out.println("Correct\n\n");
		} else {
			System.out.println("FAIL\n\n");
		}
		
		//Sum up marks
		if (temp > 4){
			System.out.println("There is some issue!");
			System.exit(0);
		}
		if (temp == 4 || temp == 3){
			c2 += 3;
		} else {
			c2 += temp;
		}
	}
	
	/**
	 * Tests for Static Subtract method
	 */
	public void staticSubTests(){
		int temp = 0;
		System.out.println("STATIC SUBTRACT TESTS");
		
		//Test 1: Same denominator
		Rational r1 = new Rational(2,5);
		Rational r2 = new Rational(1,5);
		Rational r3 = Rational.subtract(r1, r2);
		System.out.println("Test 1");
		System.out.println("Expected: 2/5 - 1/5 = 1/5");
		System.out.println("Result: 2/5 - 1/5 = " + r3);
		if (r3.getNumerator() == 1 && r3.getDenominator() == 5){
			temp++;
			System.out.println("Correct\n\n");
		} else {
			System.out.println("FAIL\n\n");
		}
		
		//Test 2: Need to find common denominator. Simplification needed
		r1 = new Rational(-1,2);
		r2 = new Rational(-2,9);
		r3 = Rational.subtract(r1, r2);
		System.out.println("Test 2");
		System.out.println("Expected: -1/2 - -2/9 = -5/18");
		System.out.println("Result: -1/2 - -2/9 = " + r3);
		if (r3.getNumerator() == -5 && r3.getDenominator() == 18){
			temp++;
			System.out.println("Correct\n\n");
		} else {
			System.out.println("FAIL\n\n");
		}
		
		//Test 3
		r1 = new Rational(39,2);
		r2 = new Rational(20,3);
		r3 = Rational.subtract(r1, r2);
		System.out.println("Test 3");
		System.out.println("Expected: 39/2 - 20/3 = 77/6");
		System.out.println("Result: 39/2 - 20/3 = " + r3);
		if (r3.getNumerator() == 77 && r3.getDenominator() == 6){
			System.out.println("Correct\n\n");
			temp++;
		} else {
			System.out.println("FAIL\n\n");
		}
		
		//Test 4
		r1 = new Rational(-25,4);
		r2 = new Rational(12,11);
		r3 = Rational.subtract(r1, r2);
		System.out.println("Test 4");
		System.out.println("Expected: -25/4 - 12/11 = -323/44");
		System.out.println("Result: -25/4 - 12/11 = " + r3);
		if (r3.getNumerator() == -323 && r3.getDenominator() == 44){
			System.out.println("Correct\n\n");
			temp++;
		} else {
			System.out.println("FAIL\n\n");
		}
		
		//Sum up marks
		if (temp > 4){
			System.out.println("There is some issue!");
			System.exit(0);
		}
		if (temp == 4 || temp == 3){
			c2 += 3;
		} else {
			c2 += temp;
		}
	}
	
	/**
	 * Tests for Static Multiply method
	 */
	public void staticMulTests(){
		int temp = 0;
		System.out.println("STATIC MULTIPLY TESTS");
		
		//Test 1
		Rational r1 = new Rational(1,2);
		Rational r2 = new Rational(1,2);
		Rational r3 = Rational.multiply(r1, r2);
		System.out.println("Test 1");
		System.out.println("Expected: 1/2 * 1/2 = 1/4");
		System.out.println("Result: 1/2 * 1/2 = " + r3);
		if (r3.getNumerator() == 1 && r3.getDenominator() == 4){
			System.out.println("Correct\n\n");
			temp++;
		} else {
			System.out.println("FAIL\n\n");
		}
		
		//Test 2
		r1 = new Rational(-1,2);
		r2 = new Rational(-2,9);
		r3 = Rational.multiply(r1, r2);
		System.out.println("Test 2");
		System.out.println("Expected: -1/2 * -2/9 = 1/9");
		System.out.println("Result: -1/2 * -2/9 = " + r3);
		if (r3.getNumerator() == 1 && r3.getDenominator() == 9){
			System.out.println("Correct\n\n");
			temp++;
		} else {
			System.out.println("FAIL\n\n");
		}
		
		//Test 3
		r1 = new Rational(39,2);
		r2 = new Rational(20,3);
		r3 = Rational.multiply(r1, r2);
		System.out.println("Test 3");
		System.out.println("Expected: 39/2 * 20/3 = 130/1");
		System.out.println("Result: 39/2 * 20/3 = " + r3);
		if (r3.getNumerator() == 130 && r3.getDenominator() == 1){
			System.out.println("Correct\n\n");
			temp++;
		} else {
			System.out.println("FAIL\n\n");
		}
		
		//Test 4
		r1 = new Rational(-25,4);
		r2 = new Rational(12,11);
		r3 = Rational.multiply(r1, r2);
		System.out.println("Test 4");
		System.out.println("Expected: -25/4 * 12/11 = -75/11");
		System.out.println("Result: -25/4 * 12/11 = " + r3);
		if (r3.getNumerator() == -75 && r3.getDenominator() == 11){
			System.out.println("Correct\n\n");
			temp++;
		} else {
			System.out.println("FAIL\n\n");
		}
		
		//Sum up marks
		if (temp > 4){
			System.out.println("There is some issue!");
			System.exit(0);
		}
		if (temp == 4 || temp == 3){
			c2 += 3;
		} else {
			c2 += temp;
		}
	}
	
	/**
	 * Tests for Static Divide method
	 */
	public void staticDivTests(){
		int temp = 0;
		System.out.println("STATIC DIVIDE TESTS");
		
		//Test 1
		Rational r1 = new Rational(1,2);
		Rational r2 = new Rational(11,25);
		Rational r3 = Rational.divide(r1, r2);
		System.out.println("Test 1");
		System.out.println("Expected: 1/2 / 11/25 = 25/22");
		System.out.println("Result: 1/2 / 11/25 = " + r3);
		if (r3.getNumerator() == 25 && r3.getDenominator() == 22){
			System.out.println("Correct\n\n");
			temp++;
		} else {
				System.out.println("FAIL\n\n");
		}
		
		//Test 2
		r1 = new Rational(-1,2);
		r2 = new Rational(-2,9);
		r3 = Rational.divide(r1, r2);
		System.out.println("Test 2");
		System.out.println("Expected: -1/2 / -2/9 = 9/4");
		System.out.println("Result: -1/2 / -2/9 = " + r3);
		if (r3.getNumerator() == 9 && r3.getDenominator() == 4){
			System.out.println("Correct\n\n");
			temp++;
		} else {
			System.out.println("FAIL\n\n");
		}
		
		//Test 3
		r1 = new Rational(39,2);
		r2 = new Rational(20,3);
		r3 = Rational.divide(r1, r2);
		System.out.println("Test 3");
		System.out.println("Expected: 39/2 / 20/3 = 117/40");
		System.out.println("Result: 39/2 / 20/3 = " + r3);
		if (r3.getNumerator() == 117 && r3.getDenominator() == 40){
			System.out.println("Correct\n\n");
			temp++;
		} else {
			System.out.println("FAIL\n\n");
		}
		
		//Test 4
		r1 = new Rational(-25,4);
		r2 = new Rational(12,11);
		r3 = Rational.divide(r1, r2);
		System.out.println("Test 4");
		System.out.println("Expected: -25/4 * 12/11 = -275/48");
		System.out.println("Result: -25/4 * 12/11 = " + r3);
		if (r3.getNumerator() == -275 && r3.getDenominator() == 48){
			System.out.println("Correct\n\n");
			temp++;
		} else {
			System.out.println("FAIL\n\n");
		}
		
		//Sum up marks
		if (temp > 4){
			System.out.println("There is some issue!");
			System.exit(0);
		}
		if (temp == 4 || temp == 3){
			c2 += 3;
		} else {
			c2 += temp;
		}
	}
	
	/**
	 * Tests for Non-Static Add method
	 */
	public void addTests(){
		
		//Test 1
		System.out.println("ADD TEST");
		Rational r1 = new Rational(25,-4);
		Rational r2 = new Rational(12,11);
		r1.add(r2);
		System.out.println("Test 1");
		System.out.println("Expected: 25/-4 + 12/11 = -227/44");
		System.out.println("Result: 25/-4 + 12/11 = " + r1);
		if (r1.getNumerator() == -227 && r1.getDenominator() == 44){
			System.out.println("Correct\n\n");
			c3++;
		} else {
			System.out.println("FAIL\n\n");
		}
	}
	
	/**
	 * Tests for Non-Static Subtract method 
	 */
	public void subTests(){
		
		System.out.println("SUBTRACT TEST");
		
		//Test 1
		Rational r1 = new Rational(-25,4);
		Rational r2 = new Rational(12,11);
		r1.subtract(r2);
		System.out.println("Test 1");
		System.out.println("Expected: -25/4 - 12/11 = -323/44");
		System.out.println("Result: -25/4 - 12/11 = " + r1);
		if (r1.getNumerator() == -323 && r1.getDenominator() == 44){
			System.out.println("Correct\n\n");
			c3++;
		} else {
				System.out.println("FAIL\n\n");
		}
	}
	
	/**
	 * Tests for Non-Static Multiply method
	 */
	public void mulTests(){
		
		System.out.println("MULTIPLY TEST");
		
		//Test 1
		Rational r1 = new Rational(-25,4);
		Rational r2 = new Rational(12,11);
		r1.multiply(r2);
		System.out.println("Test 1");
		System.out.println("Expected: -25/4 * 12/11 = -75/11");
		System.out.println("Result: -25/4 * 12/11 = " + r1);
		if (r1.getNumerator() == -75 && r1.getDenominator() == 11){
			System.out.println("Correct\n\n");
			c3++;
		} else {
			System.out.println("FAIL\n\n");
		}
	}
	
	/**
	 * Tests for Non-Static Divide method
	 */
	public void divTests(){
		
		System.out.println("DIVIDE TEST");
		
		//Test 1
		Rational r1 = new Rational(-25,4);
		Rational r2 = new Rational(12,11);
		r1.divide(r2);
		System.out.println("Test 1");
		System.out.println("Expected: -25/4 * 12/11 = -275/48");
		System.out.println("Result: -25/4 * 12/11 = " + r1);
		if (r1.getNumerator() == -275 && r1.getDenominator() == 48){
			System.out.println("Correct\n\n");
			c3++;
		} else {
			System.out.println("FAIL\n\n");
		}
	}
	
	/**
	 * Tests for Static Reciprocate method
	 */
	public void staticRecipTests(){
		System.out.println("STATIC RECIPROCATE TESTS");
		
		//Test 1
		Rational r1 = new Rational(3,4);
		Rational r2 = Rational.reciprocate(r1);
		System.out.println("Test 1");
		System.out.println("Expected: Reciprocate 3/4 = 4/3" );
		System.out.println("Result: Reciprocate 3/4 = " + r2);
		if (r2.getNumerator() == 4 && r2.getDenominator() == 3){
			System.out.println("Correct\n\n");
			c4++;
		} else {
			System.out.println("FAIL\n\n");
		}
		
		//Test 2
		r1 = new Rational(-6, 3);
		r2 = Rational.reciprocate(r1);
		System.out.println("Test 2");
		System.out.println("Expected: Reciprocate -6/3 = -1/2" );
		System.out.println("Result: Reciprocate -6/3 = " + r2);
		if (r2.getNumerator() == -1 && r2.getDenominator() == 2){
			System.out.println("Correct\n\n");
			c4++;
		} else {
			System.out.println("FAIL\n\n");
		}
	}
	
	/**
	 * Tests for Static Negate method
	 */
	public void staticNegateTests(){
		System.out.println("STATIC NEGATE TESTS");
		
		//Test 1
		Rational r1 = new Rational(3,4);
		Rational r2 = Rational.negate(r1);
		System.out.println("Test 1");
		System.out.println("Expected: Negate 3/4 = -3/4" );
		System.out.println("Result: Negate 3/4 = " + r2);
		if (r2.getNumerator() == -3 && r2.getDenominator() == 4){
			System.out.println("Correct\n\n");
			c4++;
		} else {
			System.out.println("FAIL\n\n");
		}
		
		//Test 2
		r1 = new Rational(-6, 3);
		r2 = Rational.negate(r1);
		System.out.println("Test 2");
		System.out.println("Expected: Negate -6/3 = 2/1" );
		System.out.println("Result: Negate -6/3 = " + r2);
		if (r2.getNumerator() == 2 && r2.getDenominator() == 1){
			System.out.println("Correct\n\n");
			c4++;
		} else {
			System.out.println("FAIL\n\n");
		}
	}
	
	/**
	 * Tests for Non-Static Reciprocate method
	 */
	public void recipTests(){
		System.out.println("STATIC RECIPROCATE TESTS");
		
		//Test 1
		Rational r1 = new Rational(-6, 3);
		r1.reciprocate();
		System.out.println("Test 1");
		System.out.println("Expected: Reciprocate -6/3 = -1/2" );
		System.out.println("Result: Reciprocate -6/3 = " + r1);
		if (r1.getNumerator() == -1 && r1.getDenominator() == 2){
			System.out.println("Correct\n\n");
			c5++;
		} else {
			System.out.println("FAIL\n\n");
		}
	}
	
	/**
	 * Tests for Non-Static Negate method
	 */
	public void negateTests(){
		System.out.println("STATIC NEGATE TESTS");
		Rational r1 = new Rational(-6, 3);
		r1.negate();
		System.out.println("Test 1");
		System.out.println("Expected: Negate -6/3 = 2/1" );
		System.out.println("Result: Negate -6/3 = " + r1);
		if (r1.getNumerator() == 2 && r1.getDenominator() == 1){
			System.out.println("Correct\n\n");
			c5++;
		} else {
			System.out.println("FAIL\n\n");
		}
	}
	
	/**
	 * Tests for accessor methods
	 */
	public void accessorTests(){
		System.out.println("ACCESSOR TESTS");
		
		//Test 1
		Rational r = new Rational(2, 5);
		System.out.println("getNumerator Test");
		if (r.getNumerator() == 2){
			System.out.println("Correct\n\n");
			c6++;
		} else {
			System.out.println("FAIL");
		}
		
		//Test 2
		System.out.println("getDenominator Test");
		if (r.getDenominator() == 5){
			System.out.println("Correct\n\n");
			c6++;
		} else {
			System.out.println("FAIL");
		}
	}
	
	/**
	 * Tests for mutator methods
	 */
	public void mutatorTests(){
		System.out.println("MUTATOR TESTS");
		
		//Test 1
		Rational r = new Rational(2, 5);
		System.out.println("setNumerator Test");
		r.setNumerator(5);
		if (r.getNumerator() == 1 && r.getDenominator() == 1){
			System.out.println("Correct\n\n");
			c6++;
		} else {
			System.out.println("FAIL\n\n");
		}
		
		//Test 2
		r =  new Rational(2,5);
		System.out.println("setDenominator Test");
		r.setDenominator(-5);
		if (r.getNumerator() == -2 && r.getDenominator() == 5){
			System.out.println("Correct\n\n");
			c6++;
		} else {
			System.out.println(" FAIL\n\n");
		}
	}
	
	/**
	 * Tests for toString method.
	 * This method does not add to the marks. 
	 * The output needs to be checked manually.
	 */
	public void toStringTests(){
		
		System.out.println("TOSTRING TESTS");
		
		//Test 1
		Rational r = new Rational(1,2);
		String str = r.toString();
		System.out.println("Test 1:");
		System.out.println("Excepted: 1/2");
		System.out.println("Result: " + str);
		
		//Test 2
		r = new Rational(-2, 3);
		str = r.toString();
		System.out.println("Test 2:");
		System.out.println("Excepted: -2/3");
		System.out.println("Result: " + str);
		
	}
	
	/**
	 * Test for equals method
	 */
	public void equalsTests(){
		System.out.println("EQUALS TESTS");
		
		//Test 1: Expects true
		Rational r1 = new Rational(1,2);
		Rational r2 = new Rational(1,2);
		
		System.out.println("Test 1:");
		if (r1.equals(r2)){
			System.out.println("Correct\n\n");
			c7++;
		} else {
			System.out.println("FAIL\n\n");
		}
		
		//Test 2: Expects true (r2 needs to be reduced)
		r2 = new Rational(2,4);
		System.out.println("Test 2:");
		if (r1.equals(r2)){
			System.out.println("Correct\n\n");
			c7++;
		} else {
			System.out.println("FAIL\n\n");
		}
	}
	
	/**
	 * Prints summary
	 */
	public void summary(){
		//Print summary
		System.out.println("Rational Class");
		System.out.println("c1: " + c1);
		System.out.println("c2: " + c2);
		System.out.println("c3: " + c3);
		System.out.println("c4: " + c4);
		System.out.println("c5: " + c5);
		System.out.println("c6: " + c6);
		System.out.println("c7: " + c7);
		System.out.println("Be sure to manually check the output of the toString method manully. Add marks to c7\n");
		System.out.println("The above tests explicity force Normalized and Simplified Rationals. "
				+ "Be sure to inspect the code to see if the submission does simplify and normalize.");
		System.out.println("Examine the code to check if the static methods do not change the argument objects");
		
		
	}
}
