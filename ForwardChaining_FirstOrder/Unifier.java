
import java.util.*;

class Unifier
{
	Map<String, String> substitutions = new HashMap<>();

	boolean unify(Term x, Term y)
	{
		if (x.isVariable)
			return substitute(x.name, y.name);
		if (y.isVariable)
			return substitute(y.name, x.name);
		return x.name.equals(y.name);
	}

	private boolean substitute(String var, String value)
	{
		if (substitutions.containsKey(var))
		{
			return substitutions.get(var).equals(value);
		}
		substitutions.put(var, value);
		return true;
	}

	@Override
	public String toString()
	{
		return substitutions.toString();
	}
}