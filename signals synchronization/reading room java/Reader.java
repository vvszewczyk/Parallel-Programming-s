public class Reader implements Runnable
{
    private final ReadingRoom RR;
    private final int id;

    public Reader(ReadingRoom czytelnia, int id)
    {
        this.RR = czytelnia;
        this.id = id;
    }

    public void run()
    {
        try
        {
            for(;;)
            {
                Thread.sleep((long)(Math.random() * 1000));
                System.out.println("READER " + id + " - before lock");
                RR.want_read();
                System.out.println("READER " + id + " - go in");
                RR.reading();
                System.out.println("READER " + id + " - leave");
                RR.reading_finished();
                System.out.println("READER " + id + " - after lock");
                Thread.sleep(5000);
            }
        }
        catch (InterruptedException e)
        {
            Thread.currentThread().interrupt();
        }
    }
}
