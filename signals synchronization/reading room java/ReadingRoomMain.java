public class ReadingRoomMain
{
    public static void main(String[] args)
    {
        ReadingRoom RR = new ReadingRoom();

        int writers = 5;
        int readers = 10;

        for(int i = 0; i < writers; i++)
        {
            Writer writer = new Writer(RR, i);
            Thread pisarzThread = new Thread(writer, "WRITER " + Thread.currentThread().getName());
            pisarzThread.start();
        }

        for(int i = 0; i < readers; i++)
        {
            Reader reader = new Reader(RR, i);
            Thread czytelnikThread = new Thread(reader, "READER " + Thread.currentThread().getName());
            czytelnikThread.start();
        }
    }
}
