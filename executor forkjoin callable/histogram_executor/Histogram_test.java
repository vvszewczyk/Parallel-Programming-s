import java.util.Scanner;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Future;

/*
run() - zawiera logikę wykonywana przez wątek, wywołana nie uruchomi nowego wątku tylko wykona się w aktualnym
start() - uruchamia nowy wątek i wewnątrz wywołuje metodę run(), przez co kod działa równolegle
*/

class Histogram_test
{
    public static void main(String[] args) 
	{
		Scanner scanner = new Scanner(System.in);
		
		System.out.println("Set image size: n (#rows), m(#columns)");
		int n = scanner.nextInt();
		int m = scanner.nextInt();
		Obraz obraz_1 = new Obraz(n, m);

		sequential_histogram(obraz_1);

		// WARIANT 3 - WIERSZOWY CYKLICZNY
		obraz_1.clear_parallel_histogram();
		wariant_3(obraz_1, scanner);
	}

	public static void sequential_histogram(Obraz obraz_1)
	{
		obraz_1.calculate_histogram();
		System.out.println("Sequential histogram:");
		obraz_1.print_histogram();
	}

	public static void wariant_3(Obraz obraz_1, Scanner scanner) 
	{
    System.out.println("Wariant 3 (executor z Future):");
    System.out.println("Set the number of threads: ");
    int p = scanner.nextInt();
    ExecutorService executor = Executors.newFixedThreadPool(p);
    List<Future<int[]>> futures = new ArrayList<>();

    for (int i = 0; i < p; i++) 
	{
        Watek3 w_callable = new Watek3(i, p, obraz_1);
        futures.add(executor.submit(w_callable));
    }

    for (Future<int[]> future : futures) 
	{
        try
		{
            obraz_1.merge_histogram(future.get());
        } 
        catch (Exception e) 
        {
            System.err.println("Error in thread execution: " + e.getMessage());
        }
    }

    executor.shutdown();

    System.out.println("Parallel histogram (row-cycle decomp):");
    System.out.println(obraz_1.check_similarity() ? "Histograms look the same" : "Histograms do not look the same");
    obraz_1.clear_parallel_histogram();
}
}

