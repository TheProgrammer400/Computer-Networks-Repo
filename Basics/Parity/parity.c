#include<stdio.h>
#include<stdlib.h>

int main() {
    int n;
    printf("Enter the length of the data: ");
    scanf("%d", &n);

    char arr[n + 1], newline, type;
    int number;

    scanf("%c", &newline);

    printf("Enter the data: ");
    scanf("%s", arr);

    printf("Even(e) or Odd(o) Parity: ");
    scanf("%c", &newline);
    scanf("%c", &type);

    printf("Enter the number: ");
    scanf("%d", &number);

    int count = 0;

    switch (type) {
        case 'e': // Even parity
            
            if (number == 0) {
                for (int i = 0; i < n; i++) {
                    if (arr[i] == '0') {
                        count++;
                    }
                }

                if (count % 2 == 0) {
                    arr[n] = '1';
                } else {
                    arr[n] = '0';
                }
                arr[n + 1] = '\0';

            } else {
                
                for (int i = 0; i < n; i++) {
                    if (arr[i] == '1') {
                        count++;
                    }
                }

                if (count % 2 == 0) {
                    arr[n] = '0';
                } else {
                    arr[n] = '1';
                }
                arr[n + 1] = '\0';
            }
            break;

        case 'o': // Odd parity
            if (number == 0) {
                
                for (int i = 0; i < n; i++) {
                    if (arr[i] == '0') {
                        count++;
                    }
                }

                if (count % 2 == 0) {
                    arr[n] = '0';
                } else {
                    arr[n] = '1';
                }
                arr[n + 1] = '\0';

            } else {
                for (int i = 0; i < n; i++) {
                    if (arr[i] == '1') {
                        count++;
                    }
                }

                if (count % 2 == 0) {
                    arr[n] = '1';
                } else {
                    arr[n] = '0';
                }
                arr[n + 1] = '\0';
            }

            break;

        default:
            printf("Invalid Inputs\n");
            return 1;
    }

    printf("Data with parity: ");
    puts(arr);

    arr[2] = '0';
    count = 0;

    printf("\nData Received: ");
    puts(arr);

    switch (type) {
        case 'e': // Even parity
            
            if (number == 0) {
                for (int i = 0; i < n + 1; i++) {
                    if (arr[i] == '0') {
                        count++;
                    }
                }

                if (count % 2 == 0) {
                    printf("\nNo Error");
                } else {
                    printf("\nError");
                }

            } else {
                for (int i = 0; i < n + 1; i++) {
                    if (arr[i] == '1') {
                        count++;
                    }
                }

                if (count % 2 == 0) {
                    printf("\nNo Error");
                } else {
                    printf("\nError");
                }
            }
            break;

        case 'o': // Odd parity
            if (number == 0) {
                for (int i = 0; i < n + 1; i++) {
                    if (arr[i] == '0') {
                        count++;
                    }
                }

                if (count % 2 == 0) {
                    printf("\nError");
                } else {
                    printf("\nNo Error");
                }

            } else {
                for (int i = 0; i < n + 1; i++) {
                    if (arr[i] == '1') {
                        count++;
                    }
                }

                if (count % 2 == 0) {
                    printf("\nError");
                } else {
                    printf("\nNo Error");
                }
            }
            break;

        default:
            printf("Invalid Inputs\n");
            return 1;
    }

    printf("\n");

    return 0;
}
