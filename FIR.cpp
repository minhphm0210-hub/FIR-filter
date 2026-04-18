#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000 // giả sử cho 1000 bits
#define L 8    // tốc độ lấy mẫu là 8 mẫu mỗi bit

// hàm sinh số ngẫu nhiên, mô phỏng các bit nguồn 0/1
void generate_bits(int bits[]) {
    for (int i = 0; i < N; i++) {
        bits[i] = rand() % 2;
    }
}

// hàm kỹ thuật BPSK
void bpsk(int bits[], double signal[]) {
    double check;

    for (int i = 0; i < N; i++) {

        // điều chế: bit = 1 thì check = 1, bit = 0 thì check = -1
        if (bits[i] == 1) {
            check = 1.0;
        }
        else {
            check = -1;
        }
        
        // các bit sau khi điều chế được sắp xếp vào mảng signal
        for (int j = 0; j < L; j++) {
            signal[i * L + j] = check;
        }
    }
}

// hàm thêm AWGN, mô phỏng nhiễu tín hiệu
int main() {
    int bits[N];
    double signal[N * L]; // mảng signal có kích thước bằng tổng số mẫu lấy được của tín hiệu

    srand(time(NULL));
    generate_bits(bits);

  
   


    bpsk(bits, signal);

   

    return 0;
}