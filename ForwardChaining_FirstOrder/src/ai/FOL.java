package ai;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Scanner;
import java.util.Set;

public class FOL
{

	int counter = 1;

	public Map<String, String> fc_Ask(Set<Implication> KB, Implication a)
	{
		while (true)
		{
			// resetCounter
			counter = 1;
			Set<Implication> newSentences = new HashSet<>();

			for (Implication currentImplication : KB)
			{
				standarizeVariable(currentImplication);
				Map<String, String> substitutions = findImplicationMatch(currentImplication, KB);

				// there is an available substitution for current implication
				if (substitutions.size() > 0)
				{
					System.out.println("current impl: " + currentImplication + "subs:" + substitutions);

					Predicate newFact = new Predicate(currentImplication.getConclusionPredicate());

					// replace variables in conclusion
					List<Term> conclusionTerms = newFact.getTerms();
					for (Map.Entry<String, String> entry : substitutions.entrySet())
					{
//			            System.out.println("Key: " + entry.getKey() + ", Value: " + entry.getValue());
						for (Term term : conclusionTerms)
						{
							
							if (term.name.equals(entry.getKey()))
							{
								term.isVariable=false;
								term.name = entry.getValue();
							}
						}
					}
					// print to check correct substitution
					System.out.println(newFact);

					Implication newSentence = new Implication(newFact);
					if (!findPredicateMatch(newFact, KB) && !findPredicateMatch(newFact, newSentences))
//					if (!KB.contains(newSentence) && !newSentences.contains(newSentence))
					{
						newSentences.add(newSentence);
						
						Map<String, String> totalSubstitutions = new HashMap<>();
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

//									System.out.println(currentPredicate + " current: " + currentTerm + " other: " + otherTerm);
							}

							if (canUnify)
							{
//								System.out.println(implication + "=========  PRED: " + currentPredicate + " subs: "
//										+ unifier.substitutions);
								System.out.println("Subs: "+unifier.substitutions);
								return unifier.substitutions;
							}
						}
					}

				}

//			for(Implication implication:KB)
//			{
//				System.out.println(implication+" size:"+implication.getPremisePredicates().size());
//			}

			}
			System.out.println("\nSTART current facts ");
			for(Implication currentImplication : KB)
			{
				
				if(currentImplication.isFact())
					System.out.println(currentImplication);
				
			}
			
			System.out.println("END current facts \n");
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
							System.out.println("Ca");
							// return new HashMap<String, String>();
							break;
						}

//							System.out.println(currentPredicate + " current: " + currentTerm + " other: " + otherTerm);
					}

					if (canUnify)
					{
//						System.out.println(implication + "=========  PRED: " + currentPredicate + " subs: "
//								+ unifier.substitutions);
						premiseTotalSubstitutions.putAll(unifier.substitutions);
					}

				}
			}
			if (!foundMatchForPredicate)
			{
				foundMatchForAllPremises = false;
//				System.out.println("Predicate: " + currentPredicate + "of implication: " + implication
//						+ " didnt find a matching fact");
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
				boolean canUnify = true;

				for (int i = 0; i < currentPredicateTerms.size(); i++)
				{

					Term currentTerm = currentPredicateTerms.get(i);
					Term otherTerm = otherImplicationTerms.get(i);

					if (!unifier.unify(currentTerm, otherTerm))
					{
						canUnify = false;
						// return new HashMap<String, String>();
						System.out.println("cant unify");
						break;
					}

//						System.out.println(currentPredicate + " current: " + currentTerm + " other: " + otherTerm);
				}

				if (canUnify)
				{
//					System.out.println("=========  PRED: " + predicate + " subs: " + unifier.substitutions);
				}

			}
		}
		return foundMatchForPredicate;
	}

	void standarizeVariable(Implication implication)
	{
		if (implication.isFact())
			return;

		List<Predicate> premisePredicates = implication.getPremisePredicates();

//		System.out.print(false);
		for (Predicate predicate : premisePredicates)
		{
			predicate.increaseVariableName(counter);
		}
		implication.getConclusionPredicate().increaseVariableName(counter);
		counter++;
	}

	public static void main(String[] args)
	{
		FOLLoader loader = new FOLLoader();

		loader.initialize("KB_FOL.txt");

//		for(Implication implication:loader.KB)
//		{
//			System.out.println(implication+" size:"+implication.getPremisePredicates().size());
//		}
		FOL fol = new FOL();

		try (Scanner keyboard = new Scanner(System.in))
		{
			while (true)
			{
				System.out.print("Εισάγετε τον τύπο προς απόδειξη: ");

				String input = keyboard.nextLine();

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

				Predicate inputPredicate = loader.convertInput(input);
				Implication inputSentence = new Implication(inputPredicate);

				System.out.println("INPUT PREDICATE"+ inputPredicate.getTerms());
				Map<String, String> answer = fol.fc_Ask(loader.KB, inputSentence);
				if (answer != null)
				{
					System.out.println(GREEN_TEXT + "True" + RESET+" answer: "+answer);
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
