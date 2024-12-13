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
			List<String> currentPremise = implication.getPremise();

			if (currentPremise.size() > 0)
			{
				count.put(implication, currentPremise.size());

				for (String symbol : currentPremise)
				{
					inferred.put(symbol, false);
				}
			}
			else
			{
				String conclusion = implication.getConclusion();
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
					int currentValue=entry.getValue();
					if(entry.getKey().getPremise().contains(p))
					{
						entry.setValue(currentValue-1);
					}
					
					if(entry.getValue()==0)
					{
						if(entry.getKey().getConclusion()==q)
						{
							return true;
						}
						agenda.add(entry.getKey().getConclusion());
					}		
				}       
			}
		}

		return false;

	}
}
