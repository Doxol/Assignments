/**
* The Controller class is in charge of playing the game.
*
* It manages turns between players, keeps track of scores and
* handles rolling the die.
*
* @author David Sepulveda
* @version 2
* @since February 21, 2015
*/
public class Controller{

	// Declares the variables to be used later in the program.
	private int number, cpuScore, userScore, decision, points, currentTurn;
	private boolean cpuGoes;
	private ComputerPlayer cpuPlayer;
	private HumanPlayer userPlayer;
	private Dice dice;

	/**
	* Sole constructor.
	*
	* @param	cpuPlayer	The computer player object
	* @param   userPlayer	The human player object
	* @param	dice		The die to be used in the game
	*/

	public Controller(ComputerPlayer cpuPlayer, HumanPlayer userPlayer, Dice dice){
		number = 0;
		this.cpuPlayer = cpuPlayer;
		this.userPlayer = userPlayer;
		this.dice = dice;
		currentTurn = 1;
		points = 0;
	}

	public void start(){ // the method start controls

		System.out.println("\nGame Started.");
		while ((userScore < 100) && (cpuScore < 100)){ // will always go at least once
			decision = 1;                             // since integers do not default to greater than 100
			points = 0; // resets the points before next turn
			System.out.println("\n[" + currentTurn++ +  "] ComputerScore = " + cpuScore + "  HumanScore = " + userScore);
			cpuGoes = ((currentTurn % 2 != 0) ? true : false); // decides whose turn it is
			while (decision == 1){
				if ((number = dice.roll()) > 1){ // rolls the die and checks if the roll earns points
					System.out.println("        " + ((cpuGoes) ? "C" : "H") + ": dice = " + number + "  points = " + (points += number));
					System.out.print("        " + ((cpuGoes) ? "C" : "H") + ": (r)un or (h)old? ");// prompt for next order
					// get next order; if cpu's turn, gets order from cpu, otherwise it gets it from the user.
					decision = ((cpuGoes) ? cpuPlayer.play(cpuScore, userScore, points) : userPlayer.play(cpuScore, userScore, points));
				}
				else{ // roll earns no points
					decision = 0; // if cpu's turn, "C" then the appropriate numbers, else "H", and the appropriate numbers
					System.out.println("        " + ((cpuGoes) ? "C" : "H") + ": dice = " + number + "  points = " + (points = 0));
				}
			}

			if (cpuGoes) // awards points to the deserving player
				cpuScore += points;

			else
				userScore += points;
		}

		// Game ends and winning player is printed. If user has more points, human wins, else computer wins.
		System.out.println("\nGame over. "+ ((userScore > cpuScore) ? "Human player wins with score " + userScore + "!!\n" : "Computer wins with score " + cpuScore + "!!\n"));
	}
}
