package ai;

public class Literal
{
	private String name;
	private boolean isNegated;

	Literal(String name, boolean neg)
	{
		this.name = name;
		this.isNegated = neg;
	}

	boolean isNegated()
	{
		return this.isNegated;
	}

	String getName()
	{
		return this.name;
	}

	public void setNegated(boolean value)
	{
		this.isNegated = value;
	}

	@Override
	public String toString()
	{
		String nameString = "";
		if (this.isNegated)
			nameString = "NOT " + this.name;
		else
			nameString = this.name;
		return nameString;
	}

	@Override
	public boolean equals(Object obj)
	{
		Literal l = (Literal) obj;
		if (l.getName().equals(this.name) && l.isNegated() == this.isNegated)
			return true;
		else
			return false;
	}

	@Override
	public int hashCode()
	{
		if (this.isNegated)
			return this.name.hashCode() + 1;
		else
			return this.name.hashCode();
	}
}
