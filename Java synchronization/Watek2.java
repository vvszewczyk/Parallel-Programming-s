public class Watek2 implements Runnable // może dziedziczyć po innej klasie / implementować inne interfejsy, używana z klasą Thread do uruchamiania wątków, obiekt klsay jest przekazywany do Thread
{
    private int id;
    private int start;
    private int end;
    private int stride;
    private Obraz obraz;

    public Watek2(int id, int start, int end, int stride, Obraz o) 
    {
    this.id = id;
    this.start = start;
    this.end = end;
    this.stride = stride;
    this.obraz = o;
    }

    public void run()
    {
        this.obraz.calculate_histogram_for_range(this.start, this.end);
        this.obraz.print_parallel_histogram_for_range(this.id, this.start, this.end, this.stride);
    }
}