import java.util.Scanner;

/**
* The HumanPlayer class implements the gaming strategy of a human player.
*
* It has two modes of operation: interactive or automated.
* To create an interactive human player, use the no-argument constructor.
* To create an automated human player use the constructor with one parameter.
* In the interactive mode, the user should be prompted to choose between Hold or Roll
* after every roll of the die.
*
* The method "play" is called by the Controller
*
* @author David Sepulveda
* @since February 21, 2015
*/

public class HumanPlayer{

	private boolean auto; // whether or not the player is automated
	private int base, decision; // base is the hold base, decision is whether the player rolds or holds

	// Constructor for creating an interactive human player.
	public void HumanPlayer(){
		auto = false;
	}

	/**
	* Constructor for creating an automated human player.
	*
	* @param	base	The value of the base when deciding whether to Roll or Hold.
	*/

	public void HumanPlayer(int base){
		auto = true;
		this.base = base;
	}

	/**
	* The method play() is called by the Controller after each die roll.
	* The scores do not change in the middle of a turn.
	* Only at the end of a turn, the Controller updates the scores.
	*
	* @param	computerScore	the computer player score at the beginning of this turn
	* @param	humanScore		the human player score at the beginning of this turn
	* @param	points			the sum of rolls collected so far in this turn
	* @return	the next move of the player as an integer: Roll or Hold
	*/

	public int play(int cpuScore, int userScore, int points){

		// Program interprets decision = 1 as roll and decision = 0 as hold.
		String temp = "";
		if (auto){
			decision = (((points < base) && ((userScore + points) < 100)) ? 1 : 0);
			System.out.println(((decision == 1) && ((userScore + points) < 100)) ? "r" : "h");
		}

		else{
			Scanner scanner = new Scanner (System.in);
			while ((!temp.equalsIgnoreCase("h")) && (!temp.equalsIgnoreCase("r"))){ // error checks
				temp = scanner.nextLine();
				// If the input is valid then nothing is printed, otherwise an error message is printed.
				System.out.print((!temp.equalsIgnoreCase("h")) && (!temp.equalsIgnoreCase("r")) ? "        H: (r)un or (h)old? " : "");
			}
			decision = ((temp.equals("r")) ? 1 : 0); // whether to roll or hold
		}

		return decision;
	}
}
