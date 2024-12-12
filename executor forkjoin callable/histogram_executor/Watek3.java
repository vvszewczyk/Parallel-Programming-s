import java.util.concurrent.Callable;

public class Watek3 implements Callable<int[]>
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

    public int[] call() 
    {
        int[] localHistogram = new int[this.obraz.get_tab_symb_length()];
        for (int i = this.start; i < this.end; i += this.stride) 
        {
            for (int j = 0; j < this.obraz.get_size_m(); j++) 
            {
                char z = this.obraz.get_tab()[i][j];
                localHistogram[z - 33]++;
            }
        }
        return localHistogram;
    }
}