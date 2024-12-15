package ai;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Set;

public class FOLLoader
{
	protected static Set<Implication> KB;

	public void initialize(String filename)
	{
		BufferedReader br = null;

		try
		{
			br = new BufferedReader(new FileReader(filename));
			String sCurrentLine;
			Implication implication;
			Predicate  predicate;
			KB = new HashSet<Implication>();
	
			boolean check = true;

			int aCurrentSize, aCurrentSize1, andCurrentSize;
			while (((sCurrentLine = br.readLine()) != null) && (check))
			{
				sCurrentLine = sCurrentLine.trim().replaceAll(" +", " ");
				
				String[] aCurrentLine = sCurrentLine.split(" "); // separator is: " " = [whitespace]
				aCurrentSize = aCurrentLine.length;
				
				if (sCurrentLine.contains("OR"))
				{
					String[] andArrCurrentLine = sCurrentLine.split(" OR "); // separator is: " OR " =[whitespace]+OR+[whitespace]
																			
					andCurrentSize = andArrCurrentLine.length;
					implication = new Implication();
					int g;
					for (g = 0; g < andCurrentSize; g++)
					{
						aCurrentLine = andArrCurrentLine[g].split(" ");
						String[] aCurrentLine1 = aCurrentLine[0].split("\\("); // separator is: "(" = [left parenthesis]
											
						aCurrentSize = aCurrentLine.length;
						aCurrentSize1 = aCurrentLine1.length;
						
						if (aCurrentSize1 != 2)
							check = false;
						else
						{
							String name = aCurrentLine1[0];
							
							//createPredicate
							if (name.startsWith("NOT"))
							{
								name = name.replace("NOT", "");
								predicate = new Predicate(name, true);
							}
							else
							{
								predicate = new Predicate(name, false);
							}
								
							
							//if reached end set check to false 
							if (aCurrentLine1[1].charAt(aCurrentLine1[1].length() - 1) != ','
									&& aCurrentLine1[1].charAt(aCurrentLine1[1].length() - 1) != ')')
								check = false;
							else //if not reached end
							{
								predicate.getTerms().add(
										new Term(aCurrentLine1[1].substring(0, aCurrentLine1[1].length() - 1)));
								for (int j = 1; j < aCurrentSize; j++)
								{
									if ((j < aCurrentSize - 1
											&& aCurrentLine[j].charAt(aCurrentLine[j].length() - 1) != ',')
											|| (j == aCurrentSize - 1 && aCurrentLine[j]
													.charAt(aCurrentLine[j].length() - 1) != ')'))
									{
										check = false;
										break;
									}
									else
									{
										predicate.getTerms().add(new Term(
												aCurrentLine[j].substring(0, aCurrentLine[j].length() - 1)));
									}
								}

								//correctly add to premise(body) or conclusion(head)
								if(!predicate.isNegated)
								{
									implication.setConclusionPredicate(predicate);
								}
								else 
								{
									implication.getPremisePredicates().add(predicate);
								}
							}
						}
					}
					/* Clause Handling */
					KB.add(implication);
				}
				else
				{
					String[] aCurrentLine1 = aCurrentLine[0].split("\\("); // separator is: "(" = [left
																			// parenthesis]
					aCurrentSize1 = aCurrentLine1.length;
					implication = new Implication();
					if (aCurrentSize1 != 2)
						check = false;
					else
					{
						String name = aCurrentLine1[0];
						if (name.startsWith("NOT"))
						{
							name = name.replace("NOT", "");
							predicate = new Predicate(name, true);
						}
						else
						{
							predicate = new Predicate(name, false);
						}
							
						
						
						if (aCurrentLine1[1].charAt(aCurrentLine1[1].length() - 1) != ','
								&& aCurrentLine1[1].charAt(aCurrentLine1[1].length() - 1) != ')')
							check = false;
						else
						{
							predicate.getTerms().add(
									new Term(aCurrentLine1[1].substring(0, aCurrentLine1[1].length() - 1)));
							for (int j = 1; j < aCurrentSize; j++)
							{
								if ((j < aCurrentSize - 1
										&& aCurrentLine[j].charAt(aCurrentLine[j].length() - 1) != ',')
										|| (j == aCurrentSize - 1
												&& aCurrentLine[j].charAt(aCurrentLine[j].length() - 1) != ')'))
								{
									check = false;
									break;
								}
								else
								{
									predicate.getTerms().add(new Term(
											aCurrentLine[j].substring(0, aCurrentLine[j].length() - 1)));
								}
							}
							
							if(!predicate.isNegated)
							{
								implication.setConclusionPredicate(predicate);
							}
							else 
							{
								implication.getPremisePredicates().add(predicate);
							}
						}
					}
					KB.add(implication);
				}
				
			}
			if (!check)
			{
				System.out.println("Wrong file format; please try again.");
			}
	
		}
		catch (IOException e)
		{
			System.err.println("The system could not find the file specified");
			e.printStackTrace();
		}
		finally
		{ // Stream closure should be executed at any case
			try
			{
				if (br != null)
					br.close();
			}
			catch (IOException exception)
			{
				exception.printStackTrace();
			}
		}
	}
	
}



