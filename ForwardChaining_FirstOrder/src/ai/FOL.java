package ai;

import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Scanner;
import java.util.Set;

public class FOL
{
	Standarize standarize = new Standarize();

	public Map<String, String> fc_Ask(Set<Implication> KB, Implication a)
	{
		while (true)
		{
			// resetCounter
			standarize.resetCounter();

			Set<Implication> newSentences = new HashSet<>();

			for (Implication currentImplication : KB)
			{
				standarize.newVars(currentImplication);

				Map<String, String> substitutions = findImplicationMatch(currentImplication, KB);

				// there is an available substitution for current implication
				if (substitutions.size() > 0)
				{
					Predicate newFact = new Predicate(currentImplication.getConclusionPredicate());

					// replace variables in conclusion
					List<Term> conclusionTerms = newFact.getTerms();
					for (Map.Entry<String, String> entry : substitutions.entrySet())
					{
						for (Term term : conclusionTerms)
						{

							if (term.name.equals(entry.getKey()))
							{
								term.isVariable = false;
								term.name = entry.getValue();
							}
						}
					}

					Implication newSentence = new Implication(newFact);
					if (!findPredicateMatch(newFact, KB) && !findPredicateMatch(newFact, newSentences))
					{
						newSentences.add(newSentence);

						// if currentConclusion matches input from user
						if (newFact.getName().equals(a.getConclusionPredicate().getName()))
						{

							// check if their terms can be unified
							List<Term> inputTerms = a.getConclusionPredicate().getTerms();

							Unifier unifier = new Unifier();
							boolean canUnify = true;

							for (int i = 0; i < conclusionTerms.size(); i++)
							{
								Term currentTerm = conclusionTerms.get(i);
								Term otherTerm = inputTerms.get(i);

								if (!unifier.unify(currentTerm, otherTerm))
								{
									canUnify = false;
									System.out.println("cant unifiy");
									break;
								}
							}

							if (canUnify)
							{
								return unifier.substitutions;
							}
						}
					}
				}
			}

			if (newSentences.isEmpty())
			{
				return null;
			}

			KB.addAll(newSentences);
		}
	}

	Map<String, String> findImplicationMatch(Implication implication, Set<Implication> KB)
	{
		List<Predicate> currentPremisePredicates = implication.getPremisePredicates();
		Map<String, String> premiseTotalSubstitutions = new HashMap<>();

		boolean foundMatchForAllPremises = true;

		for (Predicate currentPredicate : currentPremisePredicates)
		{
			boolean foundMatchForPredicate = false;

			// Compare current predicate with facts in KB
			for (Implication otherImplication : KB)
			{
				if (otherImplication.isFact()
						&& otherImplication.getConclusionPredicate().getName().equals(currentPredicate.getName()))
				{
					foundMatchForPredicate = true;
					// check if their terms can be unified
					List<Term> otherImplicationTerms = otherImplication.getConclusionPredicate().getTerms();
					List<Term> currentPredicateTerms = currentPredicate.getTerms();

					Unifier unifier = new Unifier();
					boolean canUnify = true;

					for (int i = 0; i < currentPredicateTerms.size(); i++)
					{
						Term currentTerm = currentPredicateTerms.get(i);
						Term otherTerm = otherImplicationTerms.get(i);

						if (!unifier.unify(currentTerm, otherTerm))
						{
							canUnify = false;
							foundMatchForPredicate = false;
							break;
						}
					}

					if (canUnify)
					{
						premiseTotalSubstitutions.putAll(unifier.substitutions);
					}
				}
			}
			if (!foundMatchForPredicate)
			{
				foundMatchForAllPremises = false;
			}
		}

		if (foundMatchForAllPremises)
		{
			return premiseTotalSubstitutions;
		}
		else
		{
			return new HashMap<String, String>();
		}

	}

	boolean findPredicateMatch(Predicate predicate, Set<Implication> listToSearchIn)
	{
		boolean foundMatchForPredicate = false;

		for (Implication otherImplication : listToSearchIn)
		{
			if (otherImplication.isFact()
					&& otherImplication.getConclusionPredicate().getName().equals(predicate.getName()))
			{
				foundMatchForPredicate = true;

				// check if their terms can be unified
				List<Term> otherImplicationTerms = otherImplication.getConclusionPredicate().getTerms();
				List<Term> currentPredicateTerms = predicate.getTerms();

				Unifier unifier = new Unifier();

				for (int i = 0; i < currentPredicateTerms.size(); i++)
				{
					Term currentTerm = currentPredicateTerms.get(i);
					Term otherTerm = otherImplicationTerms.get(i);

					if (!unifier.unify(currentTerm, otherTerm))
					{
						System.out.println("cant unify");
						break;
					}
				}
			}
		}
		return foundMatchForPredicate;
	}

	public static void main(String[] args)
	{
		// color option from :
		// https://www.tutorialspoint.com/how-to-print-colored-text-in-java-console#:~:text=Step%2D1%3A%20Create%20ANSI%20escape,formatting%20to%20its%20original%20condition.
		String RESET = "\u001B[0m";
		String RED_TEXT = "\u001B[31m";
		String GREEN_TEXT = "\u001B[32m";
		
		FOLLoader loader = new FOLLoader();	

		FOL fol = new FOL();

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

				else if(option.equals("1"))
				{
					System.out.println("Please enter the file name to load (without .txt) or press enter to load default");
				
					inputFileName=keyboard.nextLine();
					if(inputFileName.isEmpty())
					{
						loader.initialize("KB_FOL.txt");
					}
					else 
					{
						loader.initialize(inputFileName+".txt");
					}
					
					System.out.println("Enter the sentence(accepted format:e.g Criminal(x) ) : ");
					input = keyboard.nextLine();

					Predicate inputPredicate = loader.convertInput(input);
					Implication inputSentence = new Implication(inputPredicate);

					Map<String, String> answer = fol.fc_Ask(loader.KB, inputSentence);
								
					if (answer != null)
					{
						System.out.println(GREEN_TEXT + "True" + RESET + " answer: " + answer);
					}
					else
					{
						System.out.println(RED_TEXT + "False" + RESET);
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
		System.out.println("1. Enter a sentence to prove");	
	}
}
