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
		String defaultFilename = "horn_clauses.txt";

		String option, input, inputFileName;

		try (Scanner keyboard = new Scanner(System.in))
		{
			while (true)
			{
				printMenu();
				System.out.print("Option: ");
				option = keyboard.nextLine();

				if (option.equals("0"))
				{
					System.out.println("Exiting program...");
					break;
				}

				else if (option.equals("1"))
				{
					Set<ImplicationForm> kB;
					System.out.println("Please enter the file name to load (without .txt) or press enter to load default");

					inputFileName = keyboard.nextLine();
					if (inputFileName.isEmpty())
					{
						kB = ReadHornFromTxt.readHornClauses(defaultFilename);
					}
					else
					{
						kB = ReadHornFromTxt.readHornClauses(inputFileName + ".txt");
					}

					System.out.println("Enter the Symbol (accepted format:e.g Q): ");
					input = keyboard.nextLine();
					input = input.toUpperCase();

					if (doesPL_FC_Entails(kB, input))
					{
						System.out.println( "True");
					}
					else
					{
						System.out.println("False");
					}
				}

			}

			keyboard.close();
		}
		System.exit(0);

	}

	public static void printMenu()
	{
		System.out.println("\nWhat would you like to do?");
		System.out.println("0. Exit");
		System.out.println("1. Enter a Symbol to prove");
	}
}
