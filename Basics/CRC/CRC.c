#include <stdio.h>
#include <string.h>

void xorOperation(char *remainder, char *divisor, int divisorLength) {
    for (int i = 0; i < divisorLength; i++) {
        if (remainder[i] == divisor[i]) {
            remainder[i] = '0';
        } else {
            remainder[i] = '1';
        }
    }
}

void CRC(char *data, const char *divisor, char *remainder) {
    int dataLength = strlen(data);
    int divisorLength = strlen(divisor);

    // Initialize the remainder with the data followed by 0s
    char tempRemainder[dataLength + divisorLength - 1];
    strcpy(tempRemainder, data);

    for (int i = dataLength; i < dataLength + divisorLength - 1; i++) {
        tempRemainder[i] = '0';
    }

    tempRemainder[dataLength + divisorLength - 1] = '\0';

    for (int i = 0; i < dataLength; i++) {
        if (tempRemainder[i] == '1') {
            xorOperation(tempRemainder + i, divisor, divisorLength);
        }
    }

    strcpy(remainder, tempRemainder + dataLength);
}

void checkReceiverCodeword(char *receivedCodeword, const char *divisor) {
    int dataLength = strlen(receivedCodeword) - strlen(divisor) + 1;
    char receivedRemainder[dataLength + strlen(divisor) - 1];

    CRC(receivedCodeword, divisor, receivedRemainder);

    printf("Calculated CRC: ");

    prinf("%s\n", receivedRemainder);

    int isError = 0;

    for (int i = 0; i < strlen(receivedRemainder); i++) {
        if (receivedRemainder[i] != '0') {
            isError = 1;
            break;
        }
    }

    if (isError) {
        printf("Error in received codeword.\n");
    } else {
        printf("No error in received codeword.\n");
    }
}

int main() {
    char data[100], divisor[100], remainder[100], receivedCodeword[100];

    printf("Enter the data (binary): ");
    scanf("%s", data);

    printf("Enter the divisor (binary): ");
    scanf("%s", divisor);

    CRC(data, divisor, remainder);
    printf("CRC: %s\n", remainder);

    strcat(data, remainder);
    printf("Transmitted Codeword: %s\n", data);

    printf("Enter the received codeword: ");
    scanf("%s", receivedCodeword);

    checkReceiverCodeword(receivedCodeword, divisor);

    return 0;
}
