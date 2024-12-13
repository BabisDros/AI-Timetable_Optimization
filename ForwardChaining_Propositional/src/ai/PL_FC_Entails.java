package ai;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Queue;
import java.util.Set;


public class PL_FC_Entails
{

	public static boolean PL_FC_ENTAILS(Set<ImplicationForm> KB, String q)
	{
		Map<ImplicationForm, Integer> count = new HashMap<>();
		Map<String, Boolean> inferred = new HashMap<>();
		Queue<String> agenda = new LinkedList<>();

		// Initializate Collections
		for (ImplicationForm implication : KB)
		{
			List<Literal> currentPremise = implication.getPremise();

			if (currentPremise.size() > 0)
			{
				count.put(implication, currentPremise.size());

				for (Literal literal : currentPremise)
				{
					inferred.put(literal.getName(), false);
				}
			}
			else
			{
				String conclusion = implication.getConclusion().getName();
				inferred.put(conclusion, false);
				agenda.add(conclusion);
			}
		}

		while (!agenda.isEmpty())
		{
			String p = agenda.remove();

			Boolean isPTrue = inferred.get(p);
			if (!isPTrue)
			{
				inferred.put(p, true);
				
				for (Map.Entry<ImplicationForm,Integer> entry : count.entrySet()) 
				{
					System.out.println("Key = " + entry.getKey() +
                            ", Value = " + entry.getValue());
				}
		            
		    
			}
		}

		return false;

	}
}
