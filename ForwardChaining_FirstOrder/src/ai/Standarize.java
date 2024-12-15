package ai;

import java.util.List;

public class Standarize
{
	int counter=1;
	void newVars(Implication implication)
	{
		if (implication.isFact())
			return;

		List<Predicate> premisePredicates = implication.getPremisePredicates();

		for (Predicate predicate : premisePredicates)
		{
			predicate.increaseVariableName(counter);
		}
		implication.getConclusionPredicate().increaseVariableName(counter);
		counter++;
	}
	
	
	public void resetCounter()
	{
		counter=1;
	}
}
