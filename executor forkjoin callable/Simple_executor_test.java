import java.util.concurrent.*;
import java.util.ArrayList;
import java.util.Scanner;

public class Simple_executor_test 
{

    private static final int p = 10;
	private static final int N = 50;
        
    public static void main(String[] args) 
	{
		double start = 0.0;
		double end = Math.PI;
		Scanner scanner = new Scanner(System.in);
		System.out.println("Enter dx: ");
		double dx = scanner.nextDouble();

		sequential(start, end, dx);
		parallel_executor(start, end, dx);
		parallel_runnable(start, end, dx);
    }

	public static void sequential(double s, double e, double dx)
	{
		Calka_callable calka = new Calka_callable(s, e, dx);
		double res = calka.compute_integral();
		System.out.println("Sequential integral: " + res);
	}
	public static void parallel_executor(double s, double e, double dx)
	{
		ExecutorService executor = Executors.newFixedThreadPool(p);
		Future<Double>[] results = new Future[N];

		double step = (e - s) / N;
		for (int i = 0; i < N; i++) 
		{
			double start = s + i * step;
			double end = start + step;
			Calka_callable n = new Calka_callable(start, end, dx);
			results[i] = (executor.submit(n)); // submit - zwraca obiekt typu Future
		}

		double integral = 0.0;
		try 
		{
			for (int i = 0; i < results.length; i++) 
			{
				integral += results[i].get(); // pobranie wyniku z metody call
			}
		} 
		catch (InterruptedException | ExecutionException error) 
		{
			error.printStackTrace();
		}

		executor.shutdown();
		while(!executor.isTerminated()){}

		System.out.println("\nThreads: " + p);
		System.out.println("Task count: " + N);
		System.out.println("Parallel integral (callable): " + integral);
	}
	public static void parallel_runnable(double s, double e, double dx)
	{
		ResultManager result_manager = new ResultManager();
		Thread[] threads = new Thread[N];
		double step = (e - s) / N;

		for(int i = 0; i < N; i++)
		{
			double start = s + i * step;
			double end = start + step;
			Calka_runnable n = new Calka_runnable(start, end, dx, result_manager);
			threads[i] = new Thread(n);
			threads[i].start();
		}

		for(int i = 0; i < N; i++)
		{
			try
			{
				threads[i].join();
			}
			catch(InterruptedException error)
			{
				error.printStackTrace();
			}
		}

		System.out.println("Parallel integral (runnable): " + result_manager.getTotalIntegral());
	}
} 
