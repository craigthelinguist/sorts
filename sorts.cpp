
#include <iostream>
#include <cstring>
#include <string>

/**
   Check if two C-style, null-terminated char* strings are equal.
 **/
#define STREQ(X,Y) !strcmp(X,Y)

/**
   A sorting function mutates a sequence of ints in memory (size many of them)
   and performs an in-place sort.
 **/
typedef void (*SortingFunction)(int *seq, const int size);

void selection_sort(int *arr, const int size) {
   for (int i = 0; i < size; i++) {
      // Find smallest element from index i'th onwards.
      int indexOfSmallest = i;
      for (int j = i+1; j < size; j++) {
         if (arr[j] < arr[indexOfSmallest]) {
            indexOfSmallest = j;
         }
      }

      // Swap i'th smallest to position i.
      int temp = arr[i];
      arr[i] = arr[indexOfSmallest];
      arr[indexOfSmallest] = temp;
   }
}

void quick_sort_recurse(int *arr, int lo, int hi) {

   // Base case: 0 or 1 element arrays are already sorted.
   if (hi - lo < 2) return;

   // Find a pivot.
   int pivotIndex = lo;
   int pivot = arr[pivotIndex];

   // Partition the array.
   int leftSide = lo;
   for (int i = leftSide; i < hi; i++) {
      if (arr[i] < pivot) {
         int temp = arr[leftSide];
         arr[leftSide] = arr[i];
         arr[i] = temp;
         leftSide++;
      }
   }

   // Recursively quicksort.
   quick_sort_recurse(arr, lo, leftSide);
   quick_sort_recurse(arr, leftSide+1, hi);

}

void quick_sort(int *arr, const int size) {
   quick_sort_recurse(arr, 0, size);
}

void quick_sort_iter(int *arr, const int size) {
   
   /* Use a stack to keep track of recursive cases. A 'job' consists of a pair
      (lo, hi). They are stored on the stack in that order. */
   int stack[size + 2];
   stack[0] = 0; // initial lo
   stack[1] = size; // initial hi
   int top = 2;

   /* While there are jobs to process... */
   while (top > 0) {

      // Get next job to perform.
      int hi = stack[top-1];
      int lo = stack[top-2];
      top -= 2;
      std::cout << "Processing job (" << lo << "," << hi << ")\n";
   
      // Base case: already sorted.
      if (hi - lo < 2) continue;
 
      // Find a pivot.
      int pivotIndex = lo;
      int pivot = arr[pivotIndex];

      // Partition the array.
      int leftSide = lo;
      for (int i = leftSide; i < hi; i++) {
         if (arr[i] < pivot) {
            int temp = arr[leftSide];
            arr[leftSide] = arr[i];
            arr[i] = temp;
            leftSide++;
         }
      }

      // Add two subjobs to the stack.
      std::cout << "Adding job (" << lo << "," << leftSide << ")\n";
      stack[top++] = lo;
      stack[top++] = leftSide;
      std::cout << "Adding job (" << leftSide+1 << "," << hi << ")\n";
      stack[top++] = leftSide+1;
      stack[top++] = hi;

   }

}

/**
   Return true if the string can be parsed as a number.
 **/
bool str_is_int(char *str) {
   for (int i = 0; str[i] != '\0'; i++) {
      if (str[i] < '0' || str[i] > '9') return false;   
   }
   return true;
}

/**
   From the name of an algorithm, return a pointer to the function
   implementing it.
 **/
SortingFunction str_to_sorting_function(char *name_of_func) {
   if (STREQ(name_of_func, "selection")) return &selection_sort;
   else if (STREQ(name_of_func, "iterative quicksort")) return &quick_sort_iter;
   else if (STREQ(name_of_func, "recursive quicksort")) return &quick_sort;
   else if (STREQ(name_of_func, "quicksort")) return &quick_sort;
   else return 0;
}

void pprint_array(int *array, const int length) {
   for (int i = 0; i < length; i++) {
      std::cout << "Index " << i << ": " << array[i] << "\n";
   }
}


int main(int argc, char *argv[]) {

   /* Sanitise the command-line arguments. */
   if (argc < 2) {
      std::cerr << "Must provide type of sort and the elements to sort.\n";
      return 1;
   }

   /* Figure out which sorting algorithm to use. */
   SortingFunction sorter = str_to_sorting_function(argv[1]);
   if (!sorter) {
      std::cerr << argv[1] << " is not a valid sorting algorithm.\n";
      return 2;
   }
   
   /* Parse cmd arguments into an array of ints. */
   int num_elems = argc-2;
   int array[num_elems];
   for (int i = 2; i < argc; i++) {
      if (!str_is_int(argv[i])) {
         std::cerr << argv[i] << " is not a number.\n";
         return 3;
      }
      array[i-2] = std::stoi(argv[i]);
   }

   /* Sort the array, print the sorted version. */
   (*sorter)(array, num_elems);
   pprint_array(array, num_elems);
   

   return 0;

}

