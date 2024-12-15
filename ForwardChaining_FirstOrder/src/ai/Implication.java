package ai;

import java.util.*;

class Implication
{

	private List<Predicate> premisePredicates = new ArrayList<Predicate>();
	private Predicate conclusionPredicate;


	Implication()
	{
		this.premisePredicates = new ArrayList<Predicate>();
	}

	Implication(Predicate predicate)
	{
		this.conclusionPredicate = predicate;
	}
	Implication(List<Predicate> predicates)
	{
		this.premisePredicates = predicates;
	}

	@Override
	public boolean equals(Object obj)
	{
		if (this == obj)
			return true;
		if (obj == null || getClass() != obj.getClass())
			return false;
		Implication other = (Implication) obj;
		return Objects.equals(new HashSet<>(premisePredicates), new HashSet<>(other.premisePredicates))
				&& Objects.equals(conclusionPredicate, other.conclusionPredicate); // Compare sets of predicates
	}

	public List<Predicate> getPremisePredicates()
	{
		return this.premisePredicates;
	}

	//Only facts have no premises (Their premises implied as true)
	public boolean isFact()
	{
		return premisePredicates.size()==0;
	}
	public Predicate getConclusionPredicate()
	{
		return this.conclusionPredicate;
	}

	public void setConclusionPredicate(Predicate conclusion)
	{
		this.conclusionPredicate = conclusion;
	}

	public boolean isEmpty()
	{
		return premisePredicates.isEmpty() && conclusionPredicate == null;
	}

	@Override
	public int hashCode()
	{
		return Objects.hash(new HashSet<>(premisePredicates), conclusionPredicate);
	}

	@Override
	public String toString()
	{
		return "premise:" + premisePredicates.toString() + " | Conclusion:" + conclusionPredicate;
	}
}
