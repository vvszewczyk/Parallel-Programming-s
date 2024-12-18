import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.Condition;

public class ReadingRoom
{
    private int readers;
    private int writers; // 0 or 1

    private final ReentrantLock lock;
    private final Condition readable;
    private final Condition writable;

    private static final boolean MY_DEBUG = true;

    public ReadingRoom()
    {
        this.readers = 0;
        this.writers = 0;

        this.lock = new ReentrantLock();
        this.readable = lock.newCondition();
        this.writable = lock.newCondition();
    }

    public void want_read() throws InterruptedException
    {
        lock.lock();
        try
        {
            while (writers > 0)
            {
                readable.await();
            }
            readers++;
            System.out.println("READER " + Thread.currentThread().getId() + " has started reading.");
        }
        finally
        {
            lock.unlock();
        }
    }

    public void reading()
    {
        try
        {
            debugging();
            Thread.sleep((long)(Math.random() * 3000));
        }
        catch (InterruptedException e)
        {
            Thread.currentThread().interrupt();
        }
    }

    public void reading_finished()
    {
        lock.lock();
        try
        {
            readers--;
            System.out.println("Reader " + Thread.currentThread().getId() + " has finished reading.");
            debugging();

            if (readers == 0)
            {
                writable.signal();
            }
            else
            {
                readable.signalAll();
            }
        }
        finally
        {
            lock.unlock();
        }
    }

    public void want_write() throws InterruptedException
    {
        lock.lock();
        try
        {
            while (readers > 0 || writers > 0)
            {
                writable.await();
            }
            writers++;
            System.out.println("Writer " + Thread.currentThread().getId() + " has started writing.");
        }
        finally
        {
            lock.unlock();
        }
    }

    public void writing()
    {
        try
        {
            System.err.println("WRITING");
            debugging();
            Thread.sleep((long)(Math.random() * 3000));
        }
        catch (InterruptedException e)
        {
            Thread.currentThread().interrupt();
        }
    }

    public void writing_finished()
    {
        lock.lock();
        try {
            writers--;
            System.out.println("WRITER " + Thread.currentThread().getId() + " has finished writing.");
            debugging();

            if(lock.hasWaiters(readable))
            {
                readable.signalAll();
            }
        }
        finally
        {
            lock.unlock();
        }
    }

    private void debugging()
    {
        if (!MY_DEBUG)
        {
            return;
        }

        int writer_count = writers;
        int reader_count = readers;

        System.out.println("Writers: " + writer_count + "\t\tReaders: " + reader_count);
        if (writer_count > 1)
        {
            System.err.println("Error: Simultaneous presence of more than one writer.");
            System.exit(1);
        }
        else if (writer_count == 1 && reader_count > 0)
        {
            System.err.println("Error: Simultaneous presence of writer and readers.");
            System.exit(1);
        }
        else if (writer_count < 0)
        {
            System.err.println("Error: Number of writers is negative.");
            System.exit(1);
        }
        else if (reader_count < 0)
        {
            System.err.println("Error: Number of readers is negative.");
            System.exit(1);
        }
    }
}
