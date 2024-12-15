package ai;

class Term
{
	String name;
	boolean isVariable;
//	int counter=1; 
	
	Term(String name, boolean isVariable)
	{
		this.name = name;
		this.isVariable = isVariable;
	}

	Term(String name)
	{	
		this.name = name;
		//variables are lower case letters
		//if (name.matches("a-z"))
		if(name.length()==1)
		{
			this.isVariable = true;
		}
	}

	public void increaseCounterInName(int counter)
	{
		if(!this.isVariable)
			return;
		this.name=name+counter;
	}
	
	@Override
	public String toString()
	{
		return name;
	}
}