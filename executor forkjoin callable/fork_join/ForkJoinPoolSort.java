import java.util.concurrent.ForkJoinPool;
import java.util.Random;

public class ForkJoinPoolSort
{
    private static int SIZE = 100;

    public static void main(String args[])
    {
        int[] array = new int[SIZE];
        Random generator = new Random();
        System.out.println("Before sort: ");
        for (int i = 0; i < array.length; i++)
        {
            array[i] = generator.nextInt(10000);
            System.out.print(array[i] + " ");
        }
        System.out.println();
        
        ForkJoinPool fjp = new ForkJoinPool(); 
        DivideTask task = new DivideTask(array);
        fjp.execute(task);  // można robić inne równoległe rzeczy
        //double result = fjp.invoke(task); // zamisat execute, umożliwia oczekiwanie na wynik
        int[] sorted = task.join();

        System.out.println("After sort: ");
        for(int i = 0; i < sorted.length; i++)
        {
            System.out.print(sorted[i] + " ");
        }
        System.out.println();
    }
}