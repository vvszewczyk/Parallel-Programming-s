public class Watek3 implements Runnable
{
    private int id;
    private int start;
    private int end;
    private int stride;
    private Obraz obraz;
    private int[] histogram;

    public Watek3(int id, int stride, Obraz o) 
    {
        this.id = id;
        this.obraz = o;
        this.histogram = new int[this.obraz.get_tab_symb_length()];
        this.start = this.id;
        this.end = this.obraz.get_size_n();
        this.stride = stride;
    }

    public void run()
    {
        for (int i = this.start; i < this.end; i += this.stride)
        {
            for (int j = 0; j < this.obraz.get_size_m(); j++)
            {
                char z = this.obraz.get_tab()[i][j];
                this.histogram[z - 33]++;
            }
        }
  
        this.obraz.merge_histogram(this.histogram);
        this.obraz.print_parallel_histogram_for_row(this.id, this.stride);
    }
}