import java.util.Scanner;

public class A1Part2 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner (System.in);
        System.out.print("Please enter a number composed exclusively of digits 0 - 9: ");
        String number = scanner.nextLine(), frequency = "";
        char temp;
        int stringLength = number.length(), i, k, largestFrequency = 0;
        char[] numBank = number.toCharArray();
        int[] countBank = new int[10];

        for (i = 0; i < stringLength; i++){
            temp = numBank[i];
            if ((temp >= '0') && (temp <= '9'))
                countBank[temp - 48]++;
            else{
                System.out.println("There is an extraneous character in your number: \"" + temp + "\". The program will now close.");
                System.exit(0);
            }
        }

        System.out.print("\nThe sorted number is: ");
        for (i = 9; i >= 0; i--)
            for (k = 0; k < countBank[i]; k++)
                System.out.print(i);

        for (i = 0; i < 10; i++)
            if (countBank[i] > largestFrequency)
                largestFrequency = countBank[i];

        System.out.println("\n\nHere is the histogram of the frequency of digits: \n\n  y\n  ^\n  |");

        for (i = 0; i < largestFrequency; i++){
            frequency = ((largestFrequency - i) >= 10 ? "" : " ") + (largestFrequency - i);
            System.out.print(frequency + "|");

            for (k = 0; k < 10; k++)
                System.out.print("  " + (countBank[k] >= (largestFrequency - i) ? "*" : " "));
            System.out.print("\n");
        }

        System.out.print(" 0|------------------------------->x\n     0  1  2  3  4  5  6  7  8  9\n\n");
    }
}
