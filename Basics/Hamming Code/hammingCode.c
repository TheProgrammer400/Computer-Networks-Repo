#include<stdio.h>
#include<math.h>

int isPowerOfTwo(int x){
    return x && (!(x & (x - 1)));
}

int main(){
    int n;
    printf("Enter the number of bits in Data Word: ");
    scanf("%d", &n);
    
    int dataword[n];

    printf("Enter %d Bits of Data One at a Time:\n", n);

    for (int i = 0; i < n; i++){
        scanf("%d", &dataword[i]);
    }

    // Calculate number of redundant bits
    int r = 0;
    while (pow(2, r) < (n + r + 1)){
        r++;
    }

    printf("The number of Redundant Bits: %d\n", r);

    int cw = n + r;
    int codeword[cw];

    // Fill codeword with placeholders
    int j = 0;

    for (int i = 1; i <= cw; i++){
        if (isPowerOfTwo(i)){
            codeword[i - 1] = 0; // placeholder for parity bit
        } else {
            codeword[i - 1] = dataword[j++];
        }
    }

    // Set parity bits
    for (int i = 0; i < r; i++) {
        int pos = pow(2, i);
        int parity = 0;

        for (int j = pos; j <= cw; j++) {
            if ((j >> i) & 1) {
                parity ^= codeword[j - 1];
            }
        }

        codeword[pos - 1] = parity;
        printf("r%d is %d\n", pos, parity);
    }

    // Display final codeword
    printf("Codeword: ");

    for (int i = 0; i < cw; i++) {
        printf("%d ", codeword[i]);
    }

    printf("\n");

    // Introduce an error
    int error_bit;
    printf("Enter the position to introduce an error (1 to %d): ", cw);
    scanf("%d", &error_bit);
    codeword[error_bit - 1] ^= 1;

    printf("Codeword with error: ");
    for (int i = 0; i < cw; i++) {
        printf("%d ", codeword[i]);
    }

    printf("\n");

    // Detect error position
    int error_position = 0;
    for (int i = 0; i < r; i++) {
        int pos = pow(2, i);
        int parity = 0;
        
        for (int j = pos; j <= cw; j++) {
            if ((j >> i) & 1) {
                parity ^= codeword[j - 1];
            }
        }

        if (parity)
            error_position += pos;
    }

    printf("Error Bit Position (Decimal): %d\n", error_position);
    printf("Error Bit Position (Binary): ");

    for (int i = r - 1; i >= 0; i--){
        printf("%d", (error_position >> i) & 1);
    }

    printf("\n");

    return 0;
}