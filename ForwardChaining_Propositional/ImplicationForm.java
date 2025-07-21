
import java.util.ArrayList;
import java.util.List;

class ImplicationForm
{
	private List<String> premise= new ArrayList<>();
	private String conclusion;

	public ImplicationForm(List<String> premise, String conclusionString)
	{
		this.premise = premise;
		this.conclusion = conclusionString;
	}

	public List<String> getPremise()
	{
		return premise;
	}

	public String getConclusion()
	{
		return conclusion;
	}
}
