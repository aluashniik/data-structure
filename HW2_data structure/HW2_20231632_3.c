#include <stdio.h>
#include <stdlib.h>

void Sorting(int arr[], int n) {
    int i, chk, j;
    for (i = 1; i < n; i++) {
        chk = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > chk) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = chk;
    }
}

int check_array(int arr[], int n) {
    Sorting(arr, n);

    for (int i = 0; i < n - 1; i++) {
        if (arr[i] + 1 != arr[i + 1]) {
            return 0; 
        }
    }
    return 1; 
}

int main() {
    int n;
    scanf("%d", &n);
    int *arr = (int *)malloc(n * sizeof(int));
    
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    
    printf("%d\n", check_array(arr, n));
    
    free(arr);
    return 0;
}
