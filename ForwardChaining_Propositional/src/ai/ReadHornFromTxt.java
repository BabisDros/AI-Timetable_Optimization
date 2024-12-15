package ai;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class ReadHornFromTxt
{

	public static Set<ImplicationForm> readHornClauses(String filename)
	{
		Set<ImplicationForm> implications = new HashSet<>();

		try (BufferedReader reader = new BufferedReader(new FileReader(filename)))
		{
			String line;

			while ((line = reader.readLine()) != null)
			{
				// skip to next line if empty line
				if (line.trim().isEmpty())
				{
					continue;
				}

				// Check validity of current clause. If invalid, skip to next line.
				if (!isValidHornClause(line))
				{
					System.out.println("Clause in line:" + line + " is not in correct format. "
							+ "Correct format: (l1 ∨ ... ∨ lk) where li is a symbol or it's negation, with at most one positive.");
					continue;
				}

				String clauseWithoutParetheses = line.substring(1, line.length() - 1).trim();

				// split literals by symbol "∨"
				String[] literals = clauseWithoutParetheses.split("∨");
				List<String> premise = new ArrayList<>();
				String conclusion = null;

				for (String literal : literals)
				{
					literal = literal.trim();

					if (literal.startsWith("¬"))
					{
						literal = literal.replace("¬", "");
						premise.add(literal);
					}
					
					else
					{
						conclusion = literal;
					}
				}

				implications.add(new ImplicationForm(premise, conclusion));
			}

			System.out.println("All data were read succesfully\n");
		}

		catch (IOException exception)
		{
			System.out.println("Error reading file...\nThe program will now exit");
			System.exit(0);
			exception.printStackTrace();
		}

		if(implications.isEmpty())
		{
			System.out.println("The text file is empty...\\nThe program will now exit");
			System.exit(0);
		}
		return implications;
	}

	public static boolean isValidHornClause(String line)
	{

		line = line.trim();

		// clause should be between parentheses
		if (!line.startsWith("(") || !line.endsWith(")"))
		{
			return false;
		}

		String clauseWithoutParetheses = line.substring(1, line.length() - 1).trim();

		// First condition lets clauses with 1 posive literal to be valid. The second
		// parentheses demands inner ¬ to be preceded by ∨.
		if (clauseWithoutParetheses.contains("¬")
				&& (!clauseWithoutParetheses.startsWith("¬") && !clauseWithoutParetheses.matches(".*[∨¬].*")))
		{
			return false;
		}

		String[] literals = clauseWithoutParetheses.split("∨");
		if (literals.length == 0)
		{
			return false;
		}

		int positiveLiterals = 0;

		for (String literal : literals)
		{
			literal = literal.trim();

			if (literal.isEmpty() || (!literal.startsWith("¬") && !literal.matches("[A-Z0-9]+")))
			{
				return false;
			}

			if (!literal.startsWith("¬"))
			{
				positiveLiterals++;
			}
		}

		if (positiveLiterals == 1)
		{
			return true;
		}

		else
		{
			return false;
		}
	}
}
