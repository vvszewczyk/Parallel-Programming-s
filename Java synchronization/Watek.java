public class Watek extends Thread // nie może dziedziczyć po innej klasie, wątek ma pełny dostęp do metod klasy Thread, obiekt klasy uruchamiany przez start()
{
    private int id;
    private char znak;
    private Obraz obraz;

    public Watek(int id, char z, Obraz o)
    {
        this.id = id;
        this.znak = z;
        this.obraz = o;
    }

    @Override
    public void run()
    {
       this.obraz.caluculate_histogram_per_thread(this.znak);
       this.obraz.print_parallel_histogram_for_symbol(this.id, this.znak);
    }
}