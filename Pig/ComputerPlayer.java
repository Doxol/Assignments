/**
* The ComputerPlayer class implements the gaming strategy of the computer player.
*
* It has a single constructor.
* The computer player works similar to the automated human player
* but it determines the base value for Hold on its own.
*/

public class ComputerPlayer{

	private int base, decision, score;
	// sole constructor for computer player
	public ComputerPlayer(){
		base = 21;
		decision = 1;
		score = 0;
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

		double baseAdjust = ((cpuScore - userScore) / 8.0);
		int base = 21 + ((int)Math.round(baseAdjust));
		if ((((cpuScore >= 71) || (userScore >= 71)) || (points < base)) && ((cpuScore + points) < 100)){
			decision = 1;
		}

		else
			decision = 0;

		System.out.println(((decision == 1) && (userScore < 100)) && ((cpuScore + points) < 100) ? "r" : "h");
		return decision;
	}
}
