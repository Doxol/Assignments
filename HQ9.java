import java.util.Scanner;
public class HQ9{
	public static void main(String[] args) {
		Scanner scanner = new Scanner(System.in);
		String k = scanner.nextLine();
		char com;
		boolean works = false;
		for (int i = 0; i < k.length(); i++){
			com = k.charAt(i);
			if ((com == 'H') || (com == 'Q') || (com == '9')){
				works = true;
				break;
			}
		}
		System.out.println(works ? "YES" : "NO");
	}
}