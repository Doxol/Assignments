/**
 * @author  David Sepulveda
 * @ID      10161931
 * @version V1.0
 * @since   January 28, 2015
*/

/**
 * This program will accept user given numbers and return the largest number
 * possible with the given numbers. The number will be taken in and then split
 * into its individual digits. Once the digits have been seperated, the program
 * will check if each number is larger than the one before it. Once the largest
 * number has been made, the program will check if the number entered is equal
 * to the largest number. If the largest number is the same as the original
 * number, then the program will print Bingo.
*/


import java.util.Scanner;

public class A1Part1 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner (System.in);
        System.out.print("Please enter a five digit number: ");
        int number = scanner.nextInt();

        int digitOne, digitTwo, digitThree, digitFour, digitFive, temp;

        // Seperates the entered number into individual digits.

        digitOne = number / 10000;
        digitTwo = (number / 1000) - (digitOne * 10);
        digitThree = (number / 100) - ((number/1000) * 10);
        digitFour = (number / 10) - ((number / 100) * 10);
        digitFive = number - ((number / 10) * 10);

        // Prints the digits of the entered number.

        System.out.println("The digits in " + number + " are: " + digitOne + ", " + digitTwo + ", " + digitThree + ", " + digitFour + ", " + digitFive);

        // Moves the largest number to the front if it isn't already there.

        if (digitTwo >= digitOne){
            temp = digitOne;
            digitOne = digitTwo;
            digitTwo = temp;
        }
        if (digitThree >= digitOne){
            temp = digitOne;
            digitOne = digitThree;
            digitThree = digitTwo;
            digitTwo = temp;
        }
        if (digitFour >= digitOne){
            temp = digitOne;
            digitOne = digitFour;
            digitFour = digitThree;
            digitThree = digitTwo;
            digitTwo = temp;
        }
        if (digitFive >= digitOne){
            temp = digitOne;
            digitOne = digitFive;
            digitFive = digitFour;
            digitFour = digitThree;
            digitThree = digitTwo;
            digitTwo = temp;
        }

        // Moves the second largest number to the second position, if it isn't already there.

        if (digitThree >= digitTwo){
            temp = digitTwo;
            digitTwo = digitThree;
            digitThree = temp;
        }
        if (digitFour >= digitTwo){
            temp = digitTwo;
            digitTwo = digitFour;
            digitFour = digitThree;
            digitThree = temp;
        }
        if (digitFive >= digitTwo){
            temp = digitTwo;
            digitTwo = digitFive;
            digitFive = digitFour;
            digitFour = digitThree;
            digitThree = temp;
        }

        // Moves the third largest number to the third position, if it isn't already there.

        if (digitFour >= digitThree){
            temp = digitThree;
            digitThree = digitFour;
            digitFour = temp;
        }
        if (digitFive >= digitThree){
            temp = digitThree;
            digitThree = digitFive;
            digitFive = digitFour;
            digitFour = temp;
        }

        // Moves the fourth largest number to the fourth position, if it isn't already there.

        if (digitFive >= digitFour){
            temp = digitFour;
            digitFour = digitFive;
            digitFive = temp;
        }

        // Concatenates the new number.

        int theBigOne = (digitOne * 10000) + (digitTwo * 1000) + (digitThree * 100) + (digitFour * 10) + (digitFive);

        System.out.println("The largest number with these digits is: " + theBigOne);

        // Prints out the most beautifully specific Bingo possible, if the original number is equal to the new number.

        if (theBigOne == number){

            System.out.println();
            System.out.println();

            System.out.println("          ******      *****     *       *     ******        *****      ");
            System.out.println("          *     *       *       * *     *     *     *      *     *     ");
            System.out.println("          *     *       *       *  *    *     *     *      *     *     ");
            System.out.println("          ******        *       *   *   *     *            *     *     ");
            System.out.println("          *     *       *       *    *  *     *   ***      *     *     ");
            System.out.println("          *     *       *       *     * *     *     *      *     *     ");
            System.out.println("          ******      *****     *       *     *******       *****      ");
        }
    }
}
