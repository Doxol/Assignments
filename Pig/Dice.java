import java.util.Random;

public class Dice{

	private Random random;	// the internal Java Random object

	/**
	* The sole constructor of the class Dice.
	*
	* @param seed 	The seed used to initilize class Random.
	*/
	public Dice(int seed){
		random = new Random(seed);
	}

	/**
	* It is used to simulate the rolling of a die.
	* Every time it is called, it return a number from 1 to 6.
	*
	* @return	The number rolled.
	*/
	public int roll(){
		return (random.nextInt(6) + 1);
	}
}
