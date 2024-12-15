package ai;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Queue;
import java.util.Scanner;
import java.util.Set;

public class PL_FC_Entails
{

	public static boolean doesPL_FC_Entails(Set<ImplicationForm> KB, String q)
	{
		Map<ImplicationForm, Integer> count = new HashMap<>();
		Map<String, Boolean> inferred = new HashMap<>();
		Queue<String> agenda = new LinkedList<>();

		// Initializate Collections
		for (ImplicationForm implication : KB)
		{
			List<String> currentPremise = implication.getPremise();
			String conclusion = implication.getConclusion();

			if (currentPremise.size() > 0)
			{
				count.put(implication, currentPremise.size());

				for (String symbol : currentPremise)
				{
					inferred.put(symbol, false);
				}
			}
			else // symbols known to be true because of zero premise symbols
			{
				agenda.add(conclusion);
			}

			inferred.put(conclusion, false);

		}
		// main part
		while (!agenda.isEmpty())
		{

			String p = agenda.remove();
			if (p.equals(q))
				return true;

			Boolean isPInferred = inferred.get(p);

			if (!isPInferred)
			{
				inferred.put(p, true);

				for (Map.Entry<ImplicationForm, Integer> countElement : count.entrySet())
				{
					int numberOfPremises = countElement.getValue();

					// if the symbol is in the premise of current implication
					if (countElement.getKey().getPremise().contains(p))
					{
						count.put(countElement.getKey(), numberOfPremises - 1);
					}

					if (countElement.getValue().equals(0))
					{
						String conclusion = countElement.getKey().getConclusion();
						if (conclusion.equals(q))
						{
							return true;
						}
						agenda.add(conclusion);
					}
				}
			}
		}
		return false;
	}

	public static void main(String[] args)
	{
		String filename = "horn_clauses.txt";
		Set<ImplicationForm> kB = ReadHornFromTxt.readHornClauses(filename);
		for (ImplicationForm implication : kB)
		{
			System.out.println("Premise: " + implication.getPremise());
			System.out.println("Conclusion: " + implication.getConclusion());
			System.out.println("");
		}

		try (Scanner keyboard = new Scanner(System.in))
		{
			while (true)
			{
				System.out.print("Εισάγετε τον τύπο προς απόδειξη: ");

				String input = keyboard.nextLine();
				input = input.toUpperCase();

				if (input.equals("0"))
				{
					System.out.println("Exiting program...");
					break;

				}

				// color option from :
				// https://www.tutorialspoint.com/how-to-print-colored-text-in-java-console#:~:text=Step%2D1%3A%20Create%20ANSI%20escape,formatting%20to%20its%20original%20condition.
				String RESET = "\u001B[0m";
				String RED_TEXT = "\u001B[31m";
				String GREEN_TEXT = "\u001B[32m";

				if (doesPL_FC_Entails(kB, input))
				{
					System.out.println(GREEN_TEXT + "True" + RESET);
				}
				else
				{
					System.out.println(RED_TEXT + "False" + RESET);
					System.out.println("Εισάγετε νέο τυπο ή 0 για έξοδο");
				}
			}

			keyboard.close();
		}
		System.exit(0);

	}
}
