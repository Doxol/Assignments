import java.util.Scanner;
/**
 * @author David Sepulveda
 * @ID 10161931
 * @Since February 21, 2015
 *
 * The program implements the game of Pig in two modes: an interactive mode
 * in which the user decides when to run or hold, and an automated mode in which the program
 * determines when to run or hold based on an input threshold called the "base". One of the players
 * is always the computer which implements a heuristic algorithm. However, the program has been coded
 * in such a way as to allow a second human player named computerPlayer to play interactively as well.
 */
public class A2 {
	public static void main(String[] args) {

		int seed = 1234, i;
		String mode = "q", temp = ""; // mode: automated or interactive, temp: temporary variable
		                             // mode = "q" so the error check will go at least once.
		boolean veryNotDigit = true; // checks if the entered base is a digit
		HumanPlayer humanPlayer; // declares humanPlayer
		// check for any command line arguments
		// and initialize seed accordingly

		if (args.length > 1 ){
			System.out.println("Command Line Format: java Game <seed>.");
			System.exit(0);
		}

		if (args.length == 1){ // checks for command line qualifiers
			for (i = 0; i < args.length; i++)
				if (!Character.isDigit(args[0].charAt(i))){ // checks if qualifier is an integer
					System.out.println("Seed must be an integer.");
					System.exit(0); // if not an integer, the program exits
				}
			seed = Integer.parseInt(args[0]); // sets the command line qualifier as the seed
		}

		System.out.println("\nUsing the seed: " + seed);
		Scanner scanner = new Scanner (System.in);

		while (!(mode.equalsIgnoreCase("a")) && !(mode.equalsIgnoreCase("i"))){ // errors checks
			System.out.print("\nRun in automated or interactive mode?(a/i) ");
			mode = scanner.nextLine();
		}

		if (mode.equalsIgnoreCase("a")){
			while (veryNotDigit){
				veryNotDigit = false;
				System.out.print("\nWhat will be the hold base? ");
				temp = scanner.nextLine();
				for (i = 0; i < temp.length(); i++){
					if (!Character.isDigit(temp.charAt(i))) // checks if the characters in temp are digits
						veryNotDigit = true; // Very not digit. Much not number. Wow.
				}
			}
			int base = Integer.parseInt(temp);
			humanPlayer = new HumanPlayer(base);
		}

		else{
			humanPlayer = new HumanPlayer();
		}

		// start the game
		Controller controller = new Controller(new ComputerPlayer(), humanPlayer, new Dice(seed));
		controller.start();
	}
}
