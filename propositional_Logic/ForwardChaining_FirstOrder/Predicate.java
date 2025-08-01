import java.util.*;

class Predicate
{
	private String name;
	List<Term> terms;
	boolean isNegated;

	Predicate(String name, boolean isNegated)
	{
		this.name = name;
		this.terms = new ArrayList<Term>();
		this.isNegated = isNegated;
	}

	Predicate(String name, List<Term> terms, boolean isNegated)
	{
		this.name = name;
		this.terms = terms;
		this.isNegated = isNegated;
	}
	
	public Predicate(Predicate other)
	{
	    this.name = other.getName();
	    this.isNegated = other.isNegated;
	    if(other.getTerms().isEmpty())
	    	return;
	    this.terms = new ArrayList<>();
        for (Term term : other.terms)
        {
            this.terms.add(new Term(term));
        }

	}

	public List<Term> getTerms()
	{
		return this.terms;
	}

	
	public String getName()
	{
		return this.name;
	}
	public void negate()
	{
		this.isNegated = !this.isNegated;
	}
	

	public void increaseVariableName(int counter )
	{
		for(Term term:terms)
		{
			term.increaseCounterInName(counter);
		}
	}
	@Override
	public boolean equals(Object obj)
	{
		if (this == obj)
			return true;
		if (obj == null || getClass() != obj.getClass())
			return false;
		Predicate other = (Predicate) obj;
		return isNegated == other.isNegated && name.equals(other.name) && terms.equals(other.terms);
	}

	@Override
	public int hashCode()
	{
		return Objects.hash(name, terms, isNegated);
	}

	@Override
	public String toString()
	{
//		return (isNegated ? "¬" : "") + name + "(" + terms + ")";
		return name + "(" + terms + ")";
	}

	boolean isNegation(Predicate other)
	{
		return this.name.equals(other.name) && this.isNegated != other.isNegated;
	}
}
