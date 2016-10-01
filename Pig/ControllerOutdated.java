/**
* The Controller class is in charge of playing the game.
*
* It manages turns between players, keeps track of scores and
* handles rolling the die. The start() method should be called
* after initialization to start the game.
*
* @author Majid Ghaderi
* @since Feb 5, 2015
*/
public class Controller{
	// Declares the variables to be used later in the program.
	private int number, cpuScore, userScore, decision, points, currentTurn;
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

	public void start(){
		System.out.println("\nGame Started.");
		while ((userScore < 100) && (cpuScore < 100)){ // will always go at least once
			decision = 1;                             // since integers do not default to greater than 100
			points = 0; // resets the points before the user's turn
			System.out.println("\n[" + currentTurn++ +  "] ComputerScore = " + cpuScore + "  HumanScore = " + userScore);
			while (decision == 1){
				number = dice.roll(); // roll the die
				if (number > 1){ // if roll earns points
					System.out.println("        H: dice = " + number + "  points = " + (points += number));
					System.out.print("        H: (r)un or (h)old? "); // prompt for next order
					decision = userPlayer.play(cpuScore, userScore, points); // get next order from user
				}
				else{ // roll earns no points
					decision = 0;
					System.out.println("        H: dice = " + number + "  points = " + (points = 0));
				}
			}
			userScore += points;
			points = 0;
 			decision = 1;
			System.out.print((userScore < 100) ? "\n[" + currentTurn++ + "] ComputerScore = " + cpuScore + "  HumanScore = " + userScore + "\n": "");
			while (((decision == 1) && (userScore < 100))){
				number = dice.roll();
				if (number > 1){
					System.out.println("        C: dice = " + number + "  points = " + (points += number));
					System.out.print("        C: (r)un or (h)old? ");
					decision = cpuPlayer.play(cpuScore, userScore, points);
				}
				else{
					decision = 0;
					System.out.println("        C: dice = " + number + "  points = " + (points = 0));
				}
			}
			cpuScore += points;
		}
		System.out.println("\nGame over. " + ((userScore > cpuScore) ? "Human player wins with score " + userScore + "!!\n" : "Computer wins with score " + cpuScore + "!!\n"));
	}
}
