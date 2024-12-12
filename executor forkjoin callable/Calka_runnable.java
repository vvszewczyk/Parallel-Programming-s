public class Calka_runnable implements Runnable 
{
    private double dx;
    private double xp;
    private double xk;
    private int N;
    private ResultManager resultManager;

    public Calka_runnable(double xp, double xk, double dx, ResultManager resultManager) 
    {
        this.xp = xp;
        this.xk = xk;
        this.N = (int) Math.ceil((xk - xp) / dx);
        this.dx = (xk - xp) / N;
        this.resultManager = resultManager;
    }

    private double getFunction(double x) 
    {
        return Math.sin(x);
    }

    private double compute_integral() 
    {
        double calka = 0;
        for (int i = 0; i < N; i++) 
        {
            double x1 = xp + i * dx;
            double x2 = x1 + dx;
            calka += ((getFunction(x1) + getFunction(x2)) / 2.0) * dx;
        }
        return calka;
    }

    public void run() 
    {
        double partialResult = compute_integral();
        resultManager.add(partialResult);
    }
}
