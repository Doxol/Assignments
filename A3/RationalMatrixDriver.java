/**
 * The driver to test RationalMatrix.java
 * @author Zain Rizvi
 *
 */
public class RationalMatrixDriver {

	//Variables to hold values for grades per categort
	int c1, c2, c3, c4, c5, c7;
	double c6 = 0.0;

	public static void main (String[] args){
		RationalMatrixDriver a3 = new RationalMatrixDriver();
		//c1
		a3.constructorTests();
		//c2
		a3.staticAddTests();
		a3.staticSubTests();
		a3.staticMulTests();
		//c3
		a3.addTests();
		a3.subTests();
		a3.mulTests();
		//c4
		a3.staticTransposeTest();
		a3.staticNegateTests();
		//c5
		a3.transposeTest();
		a3.negateTests();
		//c6
		a3.accessorTests();
		a3.mutatorTests();
		//c7
		a3.toStringTest();
		a3.equalsTest();
		//summary
		a3.summary();
	}

	/**
	 * Constructor Test
	 */
	public void constructorTests(){
		System.out.println("CONSTRUCTOR TESTS");

		//Test 1 :rows and columns contructor
		System.out.println("Test 1");
		RationalMatrix rm = new RationalMatrix(2,3);
		Rational r = new Rational();

		boolean fail = false;

		if (rm.getMatrix().length == 2){
			for (int i = 0; i < 2; i++){
				if (rm.getMatrix()[0].length == 3){
					for (int j = 0; j < 3; j++){
						if (!rm.getAt(i, j).equals(r)){
							fail = true;
							break;
						}
					}
				} else {
					fail = true;
					break;
				}
			}
		} else {
			fail = true;
		}

		if (fail){
			System.out.println("FAIL\n");
		} else {
			System.out.println("PASS\n");
			c1 += 2;
		}


		//Test 2: rows, columns, and initial value
		System.out.println("Test 2");
		r = new Rational(2, 5);

		rm = new RationalMatrix(3,4, r);

		fail = false;

		if (rm.getMatrix().length == 3){
			for (int i = 0; i < 3; i++){
				if (rm.getMatrix()[0].length == 4){
					for (int j = 0; j < 4; j++){
						if (!rm.getAt(i, j).equals(r)){
							fail = true;
							break;
						}
					}
				} else {
					fail = true;
					break;
				}
			}
		} else {
			fail = true;
		}

		if (fail){
			System.out.println("FAIL\n");
		} else {
			System.out.println("PASS\n");
			c1 += 2;
		}


		//Test 3: 2D array constructor
		System.out.println("Test 3");
		Rational r1 = new Rational(1);
		Rational r2 = new Rational(2);
		Rational r3 = new Rational(3);
		Rational r4 = new Rational(4);

		Rational[][] matrix = {{r1,r2},{r3,r4}};
		rm = new RationalMatrix(matrix);

		fail = false;
		if (rm.getMatrix().length != 2 || rm.getMatrix()[0].length != 2){
			System.out.println(rm.getMatrix().length) ;
			fail = true;
		}
		else {
			if (!rm.getMatrix()[0][0].equals(r1) || !rm.getMatrix()[0][1].equals(r2) || !rm.getMatrix()[1][0].equals(r3) || !rm.getMatrix()[1][1].equals(r4)){
				fail = true;
			}
		}

		if (fail){
			System.out.println("FAIL\n");
		} else {
			System.out.println("PASS\n");
			c1 += 2;
		}

		//Test 4: Copy constructor
		System.out.println("Test 4");
		r = new Rational(2, 5);
		rm = new RationalMatrix(3,4, r);

		RationalMatrix rm2 = new RationalMatrix(rm);
		fail = false;

		if (rm2.getMatrix().length == 3){
			for (int i = 0; i < 3; i++){
				if (rm2.getMatrix()[0].length == 4){
					for (int j = 0; j < 4; j++){
						if (!rm2.getAt(i, j).equals(r)){
							fail = true;
							break;
						}
					}
				} else {
					fail = true;
					break;
				}
			}
		} else {
			fail = true;
		}

		if (fail){
			System.out.println("FAIL\n");
		} else {
			System.out.println("PASS\n");
			c1 += 2;
		}
	}

	/**
	 * Test for static add method
	 */
	public void staticAddTests(){

		System.out.println("STATIC ADD TESTS");

		boolean fail = false;

		//Test 1
		System.out.println("Test 1");
		Rational r1 = new Rational(1);
		Rational r2 = new Rational(2);
		Rational r3 = new Rational(3);
		RationalMatrix rm1 = new RationalMatrix(2,2, r1);
		RationalMatrix rm2 = new RationalMatrix(2,2, r2);
		RationalMatrix rm3 = RationalMatrix.add(rm1, rm2);

		//Check result
		if (rm3.getMatrix().length != 2 || rm3.getMatrix()[0].length != 2){
			fail = true;
		} else {
			for (int i = 0; i < 2; i++){
				for (int j = 0; j < 2; j++){
					if (!rm3.getMatrix()[i][j].equals(r3)){
						fail = true;
						break;
					}
				}
			}
		}

		if (fail){
			System.out.println("FAIL\n");
		} else {
			System.out.println("PASS\n");
			c2++;
			c2++;
		}
	}

	/**
	 * Test for static subtract method
	 */
	public void staticSubTests(){

		System.out.println("STATIC SUB TESTS");

		boolean fail = false;

		//Test 1
		System.out.println("Test 1");
		Rational r1 = new Rational(3);
		Rational r2 = new Rational(2);
		Rational r3 = new Rational(1);
		RationalMatrix rm1 = new RationalMatrix(2,2, r1);
		RationalMatrix rm2 = new RationalMatrix(2,2, r2);
		RationalMatrix rm3 = RationalMatrix.subtract(rm1, rm2);


		//Check result
		if (rm3.getMatrix().length != 2 || rm3.getMatrix()[0].length != 2){
			fail = true;
		} else {
			for (int i = 0; i < 2; i++){
				for (int j = 0; j < 2; j++){
					if (!rm3.getMatrix()[i][j].equals(r3)){
						fail = true;
						break;
					}
				}
			}
		}

		if (fail){
			System.out.println("FAIL\n");
		} else {
			System.out.println("PASS\n");
			c2++;
			c2++;
		}

	}

	/**
	 * Test for static multiply method
	 */
	public void staticMulTests(){

		System.out.println("STATIC MULTIPLY TESTS");

		boolean fail = false;

		//Test 1
		System.out.println("Test 1");
		Rational r1 = new Rational(3);
		Rational r2 = new Rational(2);
		Rational r3 = new Rational(6);
		RationalMatrix rm1 = new RationalMatrix(2,2, r1);
		RationalMatrix rm3 = RationalMatrix.multiply(rm1, r2);
		//Check result
		if (rm3.getMatrix().length != 2 || rm3.getMatrix()[0].length != 2){
			fail = true;
		} else {
			for (int i = 0; i < 2; i++){
				for (int j = 0; j < 2; j++){
					if (!rm3.getMatrix()[i][j].equals(r3)){
						fail = true;
						break;
					}
				}
			}
		}

		if (fail){
			System.out.println("FAIL\n");
		} else {
			System.out.println("PASS\n");
			c2++;
		}

		fail = false;	//reset flag

		//Test 2
		System.out.println("Test 2");
		r1 = new Rational(10);
		r2 = new Rational(1,2);
		r3 = new Rational(5);
		rm1 = new RationalMatrix(2,2, r1);
		rm3 = RationalMatrix.multiply(rm1, r2);
		//Check result
		if (rm3.getMatrix().length != 2 || rm3.getMatrix()[0].length != 2){
			fail = true;
		} else {
			for (int i = 0; i < 2; i++){
				for (int j = 0; j < 2; j++){
					if (!rm3.getMatrix()[i][j].equals(r3)){
						fail = true;
						break;
					}
				}
			}
		}

		if (fail){
			System.out.println("FAIL\n");
		} else {
			System.out.println("PASS\n");
			c2++;
		}
	}

	/**
	 * Tests for non-static add method
	 */
	public void addTests(){

		System.out.println("ADD TESTS");

		boolean fail = false;

		System.out.println("Test 1");
		//Test 1
		Rational r1 = new Rational(1);
		Rational r2 = new Rational(2);
		Rational r3 = new Rational(3);
		RationalMatrix rm1 = new RationalMatrix(2,2, r1);
		RationalMatrix rm2 = new RationalMatrix(2,2, r2);
		rm1.add(rm2);

		//Check result
		if (rm1.getMatrix().length != 2 || rm1.getMatrix()[0].length != 2){
			fail = true;
		} else {
			for (int i = 0; i < 2; i++){
				for (int j = 0; j < 2; j++){
					if (!rm1.getMatrix()[i][j].equals(r3)){
						fail = true;
						break;
					}
				}
			}
		}

		if (fail){
			System.out.println("FAIL\n");
		} else {
			System.out.println("PASS\n");
			c3++;
		}

		fail = false;	//Reset flag

		System.out.println("Test 2");
		//Test 2
		r1 = new Rational(1);
		r2 = new Rational(2);
		r3 = new Rational(3);
		rm1 = new RationalMatrix(2,2, r1);
		rm2 = new RationalMatrix(2,2, r2);
		rm1.add(rm2);

		//Test result
		if (rm1.getMatrix().length != 2 || rm1.getMatrix()[0].length != 2){
			fail = true;
		} else {
			for (int i = 0; i < 2; i++){
				for (int j = 0; j < 2; j++){
					if (!rm1.getMatrix()[i][j].equals(r3)){
						fail = true;
						break;
					}
				}
			}
		}

		if (fail){
			System.out.println("FAIL\n");
		} else {
			System.out.println("PASS\n");
			c3++;
		}


	}

	/**
	 * Test for non-static subtract method
	 */
	public void subTests(){

		System.out.println("SUB TESTS");

		boolean fail = false;

		//Test 1
		System.out.println("Test 1");
		Rational r1 = new Rational(3);
		Rational r2 = new Rational(2);
		Rational r3 = new Rational(1);
		RationalMatrix rm1 = new RationalMatrix(2,2, r1);
		RationalMatrix rm2 = new RationalMatrix(2,2, r2);
		rm1.subtract(rm2);

		//Check result
		if (rm1.getMatrix().length != 2 || rm1.getMatrix()[0].length != 2){
			fail = true;
		} else {
			for (int i = 0; i < 2; i++){
				for (int j = 0; j < 2; j++){
					if (!rm1.getMatrix()[i][j].equals(r3)){
						fail = true;
						break;
					}
				}
			}
		}

		if (fail){
			System.out.println("FAIL\n");
		} else {
			System.out.println("PASS\n");
			c3++;
		}

		fail = false;	//Reset flag

		//Test 2
		System.out.println("Test 2");
		r1 = new Rational(1);
		r2 = new Rational(2);
		r3 = new Rational(-1);
		rm1 = new RationalMatrix(2,2, r1);
		rm2 = new RationalMatrix(2,2, r2);
		rm1.subtract(rm2);

		//Check result
		if (rm1.getMatrix().length != 2 || rm1.getMatrix()[0].length != 2){
			fail = true;
		} else {
			for (int i = 0; i < 2; i++){
				for (int j = 0; j < 2; j++){
					if (!rm1.getMatrix()[i][j].equals(r3)){
						fail = true;
						break;
					}
				}
			}
		}

		if (fail){
			System.out.println("FAIL\n");
		} else {
			System.out.println("PASS\n");
			c3++;
		}
	}

	/**
	 * Tests for non-static multiply method
	 */
	public void mulTests(){

		System.out.println("MULTIPLY TESTS");

		boolean fail = false;
		//Test 1
		System.out.println("Test 1");
		Rational r1 = new Rational(3);
		Rational r2 = new Rational(2);
		Rational r3 = new Rational(6);
		RationalMatrix rm1 = new RationalMatrix(2,2, r1);
		rm1.multiply(r2);

		//Check result
		if (rm1.getMatrix().length != 2 || rm1.getMatrix()[0].length != 2){
			fail = true;
		} else {
			for (int i = 0; i < 2; i++){
				for (int j = 0; j < 2; j++){
					if (!rm1.getMatrix()[i][j].equals(r3)){
						fail = true;
						break;
					}
				}
			}
		}

		if (fail){
			System.out.println("FAIL\n");
		} else {
			System.out.println("PASS\n");
			c3++;
		}

		fail = false;	//Reset flag

		//Test 2
		System.out.println("Test 2");
		r1 = new Rational(10);
		r2 = new Rational(1,2);
		r3 = new Rational(5);
		rm1 = new RationalMatrix(2,2, r1);
		rm1.multiply(r2);

		//Check result
		if (rm1.getMatrix().length != 2 || rm1.getMatrix()[0].length != 2){
			fail = true;
		} else {
			for (int i = 0; i < 2; i++){
				for (int j = 0; j < 2; j++){
					if (!rm1.getMatrix()[i][j].equals(r3)){
						fail = true;
						break;
					}
				}
			}
		}

		if (fail){
			System.out.println("FAIL\n");
		} else {
			System.out.println("PASS\n");
			c3++;
		}
	}

	/**
	 * Tests for static transpose method
	 */
	public void staticTransposeTest(){

		System.out.println("STATIC TRANSPOSE TEST");

		boolean fail = false;
		//Test 1
		System.out.println("Test 1");
		Rational r1 = new Rational(1);
		Rational r2 = new Rational(2);
		Rational r3 = new Rational(3);
		Rational r4 = new Rational(4);
		Rational r5 = new Rational(5);
		Rational r6 = new Rational(6);


		Rational[][] temp1 = {{r1, r2, r3},{r4,r5,r6}};
		RationalMatrix rm = new RationalMatrix(temp1);
		RationalMatrix rm1 = RationalMatrix.transpose(rm);

		Rational[][] temp2 = rm1.getMatrix();
		if (temp2.length != 3 || temp2[0].length != 2){
			fail = true;
		} else {
			if (	(!temp2[0][0].equals(r1))||
					(!temp2[0][1].equals(r4))||
					(!temp2[1][0].equals(r2))||
					(!temp2[1][1].equals(r5))||
					(!temp2[2][0].equals(r3))||
					(!temp2[2][1].equals(r6))
					) {
				fail = true;
			}
		}

		if (fail){
			System.out.println("FAIL\n");
		} else {
			System.out.println("PASS\n");
			c4++;
		}

		fail = false;	//Reset flag

		//Test 2
		System.out.println("Test 2");
		temp2 = rm.getMatrix();
		if (temp2.length != 2 || temp2[0].length != 3){
			fail = true;
		} else {
			if (	(!temp2[0][0].equals(r1))||
					(!temp2[0][1].equals(r2))||
					(!temp2[0][2].equals(r3))||
					(!temp2[1][0].equals(r4))||
					(!temp2[1][1].equals(r5))||
					(!temp2[1][2].equals(r6))
					) {
				fail = true;
			}
		}

		if (fail){
			System.out.println("FAIL\n");
		} else {
			System.out.println("PASS\n");
			c4++;
		}
	}

	/**
	 * Tests for static negate method
	 */
	public void staticNegateTests(){
		System.out.println("STATIC NEGATE TEST");

		//Test 1; Checks newly created object
		System.out.println("Test 1");
		boolean fail = false;
		Rational r1 = new Rational(3,4);
		Rational r2 = Rational.negate(r1);
		RationalMatrix rm = new RationalMatrix(2,3,r1);
		RationalMatrix rm1 = RationalMatrix.negate(rm);
		//Check result
		for (int i = 0; i < rm1.getMatrix().length; i++){
			for (int j = 0; j < rm1.getMatrix()[0].length; j++){
				if (!rm.getMatrix()[i][j].equals(r2)){
					fail = false;
				}
			}
		}
		if (fail){
			System.out.println("FAIL\n");
		} else {
			System.out.println("PASS\n");
			c4++;
		}

		fail = false;	//Reset flag

		//Test 2; Checks that the original object doesn't change
		System.out.println("Test 2");
		for (int i = 0; i < rm.getMatrix().length; i++){
			for (int j = 0; j < rm.getMatrix()[0].length; j++){
				if (!rm.getMatrix()[i][j].equals(r1)){
					fail = false;
				}
			}
		}
		if (fail){
			System.out.println("FAIL\n");
		} else {
			System.out.println("PASS\n");
			c4++;
		}
	}


	/**
	 * Tests for non-static tranpose method
	 */
	public void transposeTest(){

		System.out.println("TRANSPOSE TEST");

		boolean fail = false;
		//Test 1
		System.out.println("Test 1");
		Rational r1 = new Rational(1);
		Rational r2 = new Rational(2);
		Rational r3 = new Rational(3);
		Rational r4 = new Rational(4);
		Rational r5 = new Rational(5);
		Rational r6 = new Rational(6);


		Rational[][] temp1 = {{r1, r2, r3},{r4,r5,r6}};
		RationalMatrix rm = new RationalMatrix(temp1);
		rm.transpose();

		Rational[][] temp2 = rm.getMatrix();
		if (temp2.length != 3 || temp2[0].length != 2){
			fail = true;
		} else {
			if (	(!temp2[0][0].equals(r1))||
					(!temp2[0][1].equals(r4))||
					(!temp2[1][0].equals(r2))||
					(!temp2[1][1].equals(r5))||
					(!temp2[2][0].equals(r3))||
					(!temp2[2][1].equals(r6))
					) {
				fail = true;
			}
		}

		if (fail){
			System.out.println("FAIL\n");
		} else {
			System.out.println("PASS\n");
			c5++;
		}
	}

	/**
	 * Tests for non-static negate method
	 */
	public void negateTests(){
		System.out.println("NEGATE TEST");

		//Test 1
		System.out.println("Test 1");
		boolean fail = false;
		Rational r1 = new Rational(3,4);
		Rational r2 = Rational.negate(r1);
		RationalMatrix rm = new RationalMatrix(2,3,r1);
		rm.negate();

		for (int i = 0; i < rm.getMatrix().length; i++){
			for (int j = 0; j < rm.getMatrix()[0].length; j++){
				if (!rm.getMatrix()[i][j].equals(r2)){
					fail = false;
				}
			}
		}
		if (fail){
			System.out.println("FAIL\n");
		} else {
			System.out.println("PASS\n");
			c5++;
		}
	}

	/**
	 * Tests for accessor methods
	 */
	public void accessorTests(){
		System.out.println("ACCESSOR TESTS");
		boolean fail = false;

		//Test 1; Test getMatrix
		System.out.println("Test 1");
		Rational r = new Rational(3, 4);
		RationalMatrix rm = new RationalMatrix(2,3,r);

		Rational[][] temp = {{r,r,r,r},{r,r,r,r},{r,r,r,r}};
		Rational[][] temp2 = rm.getMatrix();

		//Check result
		if (temp2.length == 3 && temp2[0].length == 4){
			for (int i = 0; i < 3; i++){
				for (int j = 0; j < 4; j++){
					if (temp2[i][j].equals(temp[i][j])){
						fail = true;
						break;
					}
				}
			}
		}

		if (fail){
			System.out.println("FAIL");
		} else {
			System.out.println("PASS");
			c6 = c6 + 2.0;
		}

		//Test 3: Tests getAt method, expects value
		System.out.println("Test 2");
		Rational r2 = rm.getAt(1, 1);
		if (r.equals(r2)){
			System.out.println("PASS\n");
			c6 = c6 + 1.0;
		} else {
			System.out.println("FAIL\n");
		}
	}

	/**
	 * Tests for mutator methods
	 */
	public void mutatorTests(){
		System.out.println("MUTATOR TESTS");
		boolean fail = false;

		//Test 1; setMatrix
		System.out.println("Test 1");
		Rational r = new Rational(3,4);
		Rational r2 = new Rational(4,3);

		RationalMatrix rm = new RationalMatrix(2,3,r);

		Rational[][] temp = {{r2,r2,r2},{r2,r2,r2}};
		rm.setMatrix(temp);
		System.out.println(rm.toString());
		Rational[][] temp2 = rm.getMatrix();
		//Check result
		System.out.println(temp2.toString());

		if (temp2.length == 2 && temp2[0].length == 3){
			for (int i = 0; i < 2; i++){
				for (int j = 0; j < 3; j++){
					if (!temp2[i][j].equals(temp[i][j])){
						System.out.println(temp2[i][j].toString());
						System.out.println(temp[i][j].toString());
						fail = true;
						break;
					}
				}
			}
		} else {
			fail = true;
		}

		if (fail){
			System.out.println("FAIL");
		} else {
			System.out.println("PASS");
			c6 = c6 + 2.0;
		}

		fail = false;	//Reset flag

		//Test 2; Test setAt method; expects change
		System.out.println("Test 2");
		rm = new RationalMatrix(1,1, r);
		rm.setAt(0, 0, new Rational());
		if (rm.getAt(0, 0).equals(new Rational())){
			System.out.println("PASS\n");
			c6 = c6 + 1.0;
		} else {
			System.out.println("FAIL\n");
		}

	}

	/**
	 * Tests for toString method
	 * This test does not update the marks, the method must be tested manually
	 */
	public void toStringTest(){
		System.out.println("TOSTRING TESTS");
		//Test 1
		Rational r = new Rational(5);
		RationalMatrix rm = new RationalMatrix(2,2,r);

		String str = rm.toString();

		String str2 = "Expected: \n5/1 5/1 \n5/1 5/1 ";
		System.out.println(str2);
		System.out.println("Result:\n" + str);
		System.out.println();
	}

	/**
	 * Tests for equals method
	 */
	public void equalsTest(){
		System.out.println("EQUALS TESTS");
		//Test 1
		System.out.println("Test 1");
		Rational r = new Rational(5);
		Rational r2 = new Rational(3);
		RationalMatrix rm1 = new RationalMatrix(2,2,r);
		RationalMatrix rm2 = new RationalMatrix(2,2,r);
		RationalMatrix rm3 = new RationalMatrix(2,2,r2);

		//Expects true
		if (rm1.equals(rm2)){
			System.out.println("PASS");
			c7++;
		} else {
			System.out.println("FAIL");
		}

		//Test 2
		System.out.println("Test 2");
		//Expects false
		if (!rm1.equals(rm3)){
			System.out.println("PASS\n");
			c7++;
		} else {
			System.out.println("FAIL\n");
		}
	}

	public void summary(){
		System.out.println("Summary");
		System.out.println("c1: " + c1);
		System.out.println("c2: " + c2);
		System.out.println("c3: " + c3/2.0);
		System.out.println("c4: " + c4);
		System.out.println("c5: " + c5);
		System.out.println("c6: " + c6);
		System.out.println("c7: " + c7);
		System.out.println("Be sure to inspect the output of the toString method manually");
		System.out.println("Correct output: c7 + 2");
		System.out.println("All on one value and correct values: c7 + 1");
		System.out.println("Each value on one line and correct values: c7 + 1");

	}
}
