import java.util.concurrent.RecursiveTask;

class DivideTask extends RecursiveTask<int[]> 
{
 private static final int THRESHOLD = 10; // Próg, poniżej którego wykonujemy sortowanie sekwencyjnie
  int[] arrayToDivide;
 
  public DivideTask(int[] arrayToDivide) 
  {
    this.arrayToDivide = arrayToDivide;
  }
  protected int[] compute() 
  {
      if(arrayToDivide.length <= THRESHOLD)
      {
        return sortSequentially(arrayToDivide);
      }

      int center = arrayToDivide.length / 2;
      int[] left = new int[center];
      int[] right = new int[arrayToDivide.length - center];
      System.arraycopy(arrayToDivide, 0, left, 0, center);
      System.arraycopy(arrayToDivide, center, right, 0, right.length);

      DivideTask task1 = new DivideTask(left);
      DivideTask task2 = new DivideTask(right);

      task1.fork();
      task2.fork();

      //Wait for results from both tasks
      int[] tab1 = task1.join();
      int[] tab2 = task2.join();
      
      int[] merged = new int[arrayToDivide.length];

      scal_tab(tab1, tab2, merged);
      return merged;
  }
  private int[] sortSequentially(int[] array) 
  {
    int[] sorted = array.clone();
    for (int i = 1; i < sorted.length; i++) 
    {
        int key = sorted[i];
        int j = i - 1;
        while (j >= 0 && sorted[j] > key) 
        {
            sorted[j + 1] = sorted[j];
            j--;
        }
        sorted[j + 1] = key;
    }
    return sorted;
  }
 
 private void scal_tab(int[] tab1, int[] tab2, int[] scal_tab) 
  {
    int i = 0, j = 0, k = 0;
     
    while ((i < tab1.length) && (j < tab2.length)) 
    {
      if (tab1[i] < tab2[j]) 
      {
        scal_tab[k] = tab1[i++];
      } 
      else 
      {
        scal_tab[k] = tab2[j++];
      }
       
      k++;
    }
     
    if (i == tab1.length) 
    {
      for (int a = j; a < tab2.length; a++)
      {
        scal_tab[k++] = tab2[a];
      }
       
    } 
    else 
    {
      for (int a = i; a < tab1.length; a++) 
      {
        scal_tab[k++] = tab1[a];
      }
    }
  }
}
