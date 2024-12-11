public class Watek4 implements Runnable
 {
    private int id;
    private int start;
    private int end;
    private int stride;
    private Obraz obraz;
    private int[] histogram;

    public Watek4(int id, int start, int end, int stride, Obraz o) 
    {
        this.id = id;
        this.obraz = o;
        this.histogram = new int[this.obraz.get_tab_symb_length()];
        this.start = start;
        this.end = end;
        this.stride = stride;
    }

    public void run() 
    {
        for (int i = 0; i < this.obraz.get_size_n(); i++) 
        {
            for (int j = this.start; j < this.end; j += this.stride) 
            {
                char z = this.obraz.get_tab()[i][j];
                this.histogram[z - 33]++;
            }
        }
        this.obraz.merge_histogram(this.histogram);
        this.obraz.print_parallel_histogram_for_column(this.id, this.start, this.end);
    }
}
