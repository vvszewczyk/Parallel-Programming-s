import java.util.Scanner;

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

		// WARIANT 1
		wariant_1(obraz_1);

		// WARIANT 2 - PO ZNAKACH BLOKOWY
		//wariant_2(obraz_1, scanner);

		// WARIANT 3 - WIERSZOWY CYKLICZNY
		wariant_3(obraz_1, scanner);

		// WARIANT 4 - KOLUMNOWO BLOKOWY
		//wariant_4(obraz_1, scanner);

		// WARIANT 5 - 2D BLOKOWY
		//wariant_5(obraz_1, scanner);
	}

	public static void sequential_histogram(Obraz obraz_1)
	{
		obraz_1.calculate_histogram();
		System.out.println("Sequential histogram:");
		//obraz_1.print_histogram();
	}

	public static void wariant_1(Obraz obraz_1)
	{
		System.out.println("Wariant 1:");
		Watek[] NewThr = new Watek[obraz_1.get_tab_symb_length()];
		
		for (int i = 0; i < obraz_1.get_tab_symb_length(); i++)
		{
			NewThr[i] = new Watek(i, (char)(i + 33), obraz_1);
			NewThr[i].start();
		}
		
		for (int i = 0; i < obraz_1.get_tab_symb_length(); i++) 
		{
			try 
			{
				NewThr[i].join();
			} 
			catch (InterruptedException e) {}
		}
		
		obraz_1.print_parallel_histogram();
		System.out.println(obraz_1.check_similarity() ? "Histograms look the same" : "Histograms do not look the same");
		obraz_1.clear_parallel_histogram();
	}

	public static void wariant_2(Obraz obraz_1, Scanner scanner)
	{
		System.out.println("Wariant 2:");
		System.out.println("Set the number of threads: ");
		int p = scanner.nextInt();
		Thread[] NewThr2 = new Thread[p];
		int el_on_thread = (int) Math.ceil((double) obraz_1.get_tab_symb_length() / p);

		for (int i = 0; i < p; i++)
		{
			int start = (el_on_thread * i) + 33;
			int end = (el_on_thread * (i + 1)) + 33; 
			int stride = 1;
			if(end > 127) end = 127;
			Watek2 w_runnable = new Watek2(i, start, end, stride, obraz_1);
			NewThr2[i] = new Thread(w_runnable);
			NewThr2[i].start();
		}

		for (int i = 0; i < p; i++) 
		{
			try 
			{
				NewThr2[i].join();
			} 
			catch (InterruptedException e) {}
		}

		//obraz_1.print_parallel_histogram();
		System.out.println(obraz_1.check_similarity() ? "Histograms look the same" : "Histograms do not look the same");
		obraz_1.clear_parallel_histogram();
	}

	public static void wariant_3(Obraz obraz_1, Scanner scanner)
	{
		System.out.println("Wariant 3:");
		System.out.println("Set the number of threads: ");
		int p = scanner.nextInt();
		Thread[] NewThr3 = new Thread[p];

		for (int i = 0; i < p; i++)
		{
			// Tworzenie wątków z odpowiednim numerem i odstępem
			Watek3 w_runnable2 = new Watek3(i, p, obraz_1);
			NewThr3[i] = new Thread(w_runnable2);
			NewThr3[i].start();
		}

		// Oczekiwanie na zakończenie pracy wątków
		for (int i = 0; i < p; i++) 
		{
			try 
			{
				NewThr3[i].join();
			} 
			catch (InterruptedException e) {}
		}

		System.out.println("Parallel histogram (row-cycle decomp):");
		//obraz_1.print_parallel_histogram();
		System.out.println(obraz_1.check_similarity() ? "Histograms look the same" : "Histograms do not look the same");
		obraz_1.clear_parallel_histogram();
	}

	public static void wariant_4(Obraz obraz_1, Scanner scanner)
	{
		System.out.println("Wariant 4:");
		System.out.println("Set the number of threads: ");
		int p = scanner.nextInt();
		Thread[] NewThr4 = new Thread[p];
		int cols_on_thread = (int) Math.ceil((double) obraz_1.get_size_m() / p);

		for (int i = 0; i < p; i++) 
		{
			int start = i * cols_on_thread;
			int end = (i + 1) * cols_on_thread;
			if(end > obraz_1.get_size_m()) end = obraz_1.get_size_m();
			
			if(start < end)
			{
				Watek4 w_runnable3 = new Watek4(i, start, end, 1, obraz_1);
				NewThr4[i] = new Thread(w_runnable3);
				NewThr4[i].start();
			}
			else
			{
				System.out.println("Thread " + i + ": No columns assigned.");
			}
			
		}

		for (int i = 0; i < p; i++) 
		{
			try 
			{
				if(NewThr4[i] != null)
				{
					NewThr4[i].join();
				}
			} 
			catch (InterruptedException e) 
			{
				e.printStackTrace();
			}
		}

		System.out.println("Parallel histogram (column-block decomposition):");
		//obraz_1.print_parallel_histogram();
		System.out.println(obraz_1.check_similarity() ? "Histograms look the same" : "Histograms do not look the same");
		obraz_1.clear_parallel_histogram();
	}

	public static void wariant_5(Obraz obraz_1, Scanner scanner)
	{
		System.out.println("Wariant 5:");
		System.out.println("Set the number of threads (pxp): ");
		int p = scanner.nextInt();
		obraz_1.initialize_histograms_2D_block(p * p); // p - liczba podziałów w jednym wymiarze
		System.out.println("Parallel histogram (2D block decomposition):");
		Thread[] NewThr5 = new Thread[p * p];
		int el_on_thread_n = (int) Math.ceil((double) obraz_1.get_size_n() / p);
		int el_on_thread_m = (int) Math.ceil((double) obraz_1.get_size_m() / p);
		

		for (int i = 0; i < p; i++)
		{
			for (int j = 0; j < p; j++)
			{
				int start_n = i * el_on_thread_n;
				int end_n = (i + 1) * el_on_thread_n;
				if (end_n > obraz_1.get_size_n()) end_n = obraz_1.get_size_n();

				int start_m = j * el_on_thread_m;
				int end_m = (j + 1) * el_on_thread_m;
				if (end_m > obraz_1.get_size_m()) end_m = obraz_1.get_size_m();

				int id = i * p + j; // j zwieksza numerację w ramach aktualnego wiersza, gdy znieniamy i to numeracja przechodzi do kolejnych p wartości
        		//System.out.println("Thread " + id + " assigned to block: [" + start_n + ":" + end_n + "] x [" + start_m + ":" + end_m + "]");

				Watek5 w_runnable4 = new Watek5(id, start_n, end_n, start_m, end_m, obraz_1);
				NewThr5[id] = new Thread(w_runnable4);
				NewThr5[id].start();
			}
		}

		for (int i = 0; i < p * p; i++) 
		{
			try 
			{
				NewThr5[i].join();
			} 
			catch (InterruptedException e) 
			{
				e.printStackTrace();
			}
		}
		obraz_1.print_histograms_2D_block(p * p);
		//obraz_1.print_parallel_histogram();
		System.out.println(obraz_1.check_similarity() ? "Histograms look the same" : "Histograms do not look the same");
		obraz_1.clear_parallel_histogram();
	}
}

