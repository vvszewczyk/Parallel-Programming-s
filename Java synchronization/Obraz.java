import  java.util.Random;

class Obraz
{
    private int size_n;
    private int size_m;
    private char[][] tab;
    private char[] tab_symb;
    private int[] histogram;
	private int[] hist_parallel;
	private int[][] histograms_2D_block; // Wiersze odpowiadają wątkom a kolumny przechowują liczbe wystapein danego symbolu
	private int max_symbols;


	public int get_tab_symb_length()
	{
		return tab_symb.length;
	}

	public char[][] get_tab() 
	{
    	return tab;
	}

	public int get_size_n() 
	{
    	return size_n;
	}

	public int get_size_m() 
	{
    	return size_m;
	}

	public void initialize_histograms_2D_block(int p) 
{
    System.out.println("Initializing histograms_2D_block with " + p + " threads.");
    this.histograms_2D_block = new int[p][this.get_tab_symb_length()];
    System.out.println("histograms_2D_block initialized successfully.");
}

	public int[] get_single_histogram_2D(int id) 
	{
		return this.histograms_2D_block[id];
	}

	public void set_single_histogram_2D(int id, int[] histogram) 
	{
		this.histograms_2D_block[id] = histogram;
	}

    
    public Obraz(int n, int m) 
	{
		this.size_n = n;
		this.size_m = m;
		this.tab = new char[n][m];
		this.tab_symb = new char[94];
		this.max_symbols = this.get_tab_symb_length();
		
		final Random random = new Random();
		
		// for general case where symbols could be not just integers
		for (int k = 0; k < this.max_symbols; k++) 
		{
			tab_symb[k] = (char)(k + 33); // substitute symbols
		}

		for (int i = 0; i < n; i++) 
		{
			for (int j = 0; j < m; j++) 
			{	
			tab[i][j] = tab_symb[random.nextInt(this.max_symbols)];  // ascii 33-127
			//tab[i][j] = (char)(random.nextInt(94)+33);  // ascii 33-127
			System.out.print(tab[i][j]+" ");
			}
			System.out.print("\n");
		}
		System.out.print("\n\n"); 
		
		this.histogram = new int[this.max_symbols];
		clear_histogram();

		this.hist_parallel = new int[this.max_symbols];
		clear_parallel_histogram();
    }

	// ================== WSPOLNE ======================

	public Boolean check_similarity()
	{
		if(histograms_2D_block != null)
		{
			merge_histograms_2D();
		}
		for(int i = 0; i < this.max_symbols; i++)
		{
			if(histogram[i] != hist_parallel[i])
			{
				System.out.println("Difference found for symbol: " + (char) (i + 33));
				return false;
			}
		}
		return true;
	}

	public void clear_parallel_histogram()
	{
		for (int i = 0; i < this.max_symbols; i++) 
		{
			hist_parallel[i] = 0;
		}
    }

	public void print_parallel_histogram()
	{
		for (int i = 0; i < this.max_symbols; i++) 
		{
			System.out.print(tab_symb[i] + " " + hist_parallel[i]+"\n");	    
			//System.out.print((char)(i+33)+" "+histogram[i]+"\n");	    
		}
	}

	public synchronized void merge_histogram(int[] histogram)
	{
		for (int i = 0; i < histogram.length; i++)
		{
			hist_parallel[i] += histogram[i];
		}
	}

	// ================== FUNKCJE SEKWENCYJNE ==================
    
    public void clear_histogram()
	{
		for (int i = 0; i < this.max_symbols; i++) 
		{
			histogram[i] = 0;
		}
    }


	public void calculate_histogram()
	{
	  for (int i = 0; i < size_n; i++) 
	  {
	    for (int j = 0; j < size_m; j++) 
		{
		// optymalna wersja obliczania histogramu, wykorzystująca fakt, że symbole w tablicy
		// można przekształcić w indeksy tablicy histogramu
		// histogram[(int)tab[i][j]-33]++;
		// wersja bardziej ogólna dla tablicy symboli nie utożsamianych z indeksami
		// tylko dla tej wersji sensowne jest zrównoleglenie w dziedzinie zbioru znaków ASCII
		  for (int k = 0; k < this.max_symbols; k++) 
		  {
		    if(tab[i][j] == tab_symb[k]) 
			{
				histogram[k]++;
			}
		    //if(tab[i][j] == (char)(k+33)) histogram[k]++;	    
		  }

	    }
	  }
    }

	public void print_histogram()
	{
		for (int i = 0; i < this.max_symbols; i++) 
		{
			System.out.print(tab_symb[i] + " " + histogram[i]+"\n");	    
			//System.out.print((char)(i+33)+" "+histogram[i]+"\n");	    
		}
	}

	// =============== WATEK 1 ================

	public synchronized void caluculate_histogram_per_thread(char z) // Do zliczenia wystąpień znaku w tablicy
	{
		for(int i = 0; i < size_n; i++)
		{
			for(int j = 0; j < size_m; j++)
			{
				if(tab[i][j] == z)
				{
					hist_parallel[z - 33]++; // przesunięcie na index 0
				}
			}
		}
	}

	public synchronized void print_parallel_histogram_for_symbol(int id, char z) // Wyświetlenie histogramu dla konkretnego znaku
	{
		System.out.print(id < 10 ? "Thread " + id + ": " + z + "  |" : "Thread " + id + ": " + z + " |");

		for (int i = 0; i < hist_parallel[z - 33]; i++) // Dla każdego wystąpienia znaku
		{
			System.out.print("=");
		}
		System.out.println();
	}

	// =============== WATEK 2 ================

	public synchronized void calculate_histogram_for_range(int start, int end) 
	{
		for (int i = 0; i < size_n; i++) 
		{
			for (int j = 0; j < size_m; j++) 
			{
				char z = tab[i][j];
				if (z >= start && z < end) // Czy mieści się w zakresie bloku
				{
					hist_parallel[z - 33]++;
				}
			}
		}
	}

	public synchronized void print_parallel_histogram_for_range(int id, int start, int end, int stride) 
	{
		for (int i = start; i < end; i += stride) 
		{
			char z = (char) (i);
			System.out.print(id < 10 ? "Thread " + id + ": " + z + "  |" : "Thread " + id + ": " + z + " |");
			for (int j = 0; j < hist_parallel[z - 33]; j++)  // Dla każdego wystąpienia znaku
			{
				System.out.print("=");
			}
			System.out.println();
		}
	}

	// =============== WATEK 3 ================

	public synchronized void print_parallel_histogram_for_row(int id, int stride) 
	{
		System.out.println("Thread " + id + ":");
		for (int i = id; i < size_n; i += stride) 
		{
			for (int j = 0; j < size_m; j++) 
			{
				char z = tab[i][j];
				System.out.print(z + " |");
				for (int k = 0; k < hist_parallel[z - 33]; k++) 
				{
					System.out.print("=");
				}
				System.out.println();
			}
		}
		System.out.println();
	}

	// =============== WATEK 4 ================

	public synchronized void print_parallel_histogram_for_column(int id, int start, int end) 
	{
		System.out.println("Thread " + id + ": Columns [" + start + ":" + end + "]");
		for (int j = start; j < end; j++) 
		{
			for (int i = 0; i < size_n; i++) 
			{
				char z = tab[i][j];
				System.out.print(z + " |");
				for (int k = 0; k < hist_parallel[z - 33]; k++) 
				{
					System.out.print("=");
				}
				System.out.println();
			}
		}
		System.out.println();
	}

	// =============== WATEK 5 ================

	public synchronized void merge_histograms_2D() 
	{
		//clear_parallel_histogram();

		for (int id = 0; id < histograms_2D_block.length; id++) 
		{
			for (int i = 0; i < histograms_2D_block[id].length; i++) 
			{
				hist_parallel[i] += histograms_2D_block[id][i];
			}
		}
	}

	public synchronized void print_histograms_2D_block(int threadCount) 
	{
		for (int threadId = 0; threadId < threadCount; threadId++) 
		{
			System.out.println("Thread " + threadId + ":");
			for (int i = 0; i < histograms_2D_block[threadId].length; i++) 
			{
				if (histograms_2D_block[threadId][i] > 0) 
				{
					char symbol = (char) (i + 33);
					System.out.print(symbol + "  |");
					for (int j = 0; j < histograms_2D_block[threadId][i]; j++) 
					{
						System.out.print("=");
					}
					System.out.println();
				}
			}
			System.out.println();
		}
	}
}
