package ai;

import java.util.List;

class ImplicationForm
{
	private List<Literal> premise;
	private Literal conclusion;

	public ImplicationForm(List<Literal> premise, Literal conclusionString)
	{
		this.premise = premise;
		this.conclusion = conclusionString;
	}

	public List<Literal> getPremise()
	{
		return premise;
	}

	public Literal getConclusion()
	{
		return conclusion;
	}
}
