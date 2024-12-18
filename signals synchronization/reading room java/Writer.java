public class Writer implements Runnable
{
    private final ReadingRoom RR;
    private final int id;

    public Writer(ReadingRoom czytelnia, int id)
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
                Thread.sleep((long)(Math.random() * 3000));
                System.out.println("WRITER " + id + " - before lock");
                RR.want_write();
                System.out.println("WRITER " + id + " - go in");
                RR.writing();
                System.out.println("WRITER " + id + " - leave");
                RR.writing_finished();
                System.out.println("WRITER " + id + " - after lock");
                Thread.sleep(500);
            }
        }
        catch (InterruptedException e)
        {
            Thread.currentThread().interrupt();
        }
    }
}
