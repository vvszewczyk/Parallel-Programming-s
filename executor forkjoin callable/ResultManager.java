public class ResultManager 
{
    private double totalIntegral = 0.0;

    public synchronized void add(double partialResult) 
    {
        totalIntegral += partialResult;
    }

    public double getTotalIntegral() 
    {
        return totalIntegral;
    }
}
