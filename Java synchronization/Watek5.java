public class Watek5 implements Runnable 
{
    private int id;
    private int start_n;
    private int end_n;
    private int start_m;
    private int end_m;
    private Obraz obraz;
    private int[] histogram;

    public Watek5(int id, int start_n, int end_n, int start_m, int end_m, Obraz o) 
    {
        this.id = id;
        this.obraz = o;
        this.histogram = new int[obraz.get_tab_symb_length()];
        this.start_n = start_n;
        this.end_n = end_n;
        this.start_m = start_m;
        this.end_m = end_m;
    }

    public void run() 
    {
        for (int i = this.start_n; i < this.end_n; i++) 
        {
            for (int j = this.start_m; j < this.end_m; j++) 
            {
                char z = this.obraz.get_tab()[i][j];
                this.histogram[z - 33]++;
            }
        }

        this.obraz.set_single_histogram_2D(this.id, this.histogram);
    }
}
