#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N 1000 // giả sử cho 1000 bits
#define L 8    // tốc độ lấy mẫu là 8 mẫu mỗi bit

// hàm sinh số ngẫu nhiên, mô phỏng các bit nguồn 0/1
void generate_bits(int bits[])
{
    for (int i = 0; i < N; i++)
    {
        bits[i] = rand() % 2;
    }
}

// hàm kỹ thuật BPSK
void bpsk(int bits[], double signal[])
{
    double check;

    for (int i = 0; i < N; i++)
    {

        // điều chế: bit = 1 thì check = 1, bit = 0 thì check = -1
        if (bits[i] == 1)
        {
            check = 1;
        }
        else
        {
            check = -1;
        }

        // các bit sau khi điều chế được sắp xếp vào mảng signal
        for (int j = 0; j < L; j++)
        {
            signal[i * L + j] = check;
        }
    }
}

// hàm thêm AWGN, mô phỏng nhiễu tín hiệu
void add_awgn(double signal[], double noisy_signal[], int length, double noise_level)
{
    for (int i = 0; i < length; i++)
    {

        double u1 = (rand() + 1.0) / (RAND_MAX + 1.0);
        double u2 = (rand() + 1.0) / (RAND_MAX + 1.0);

        double noise = sqrt(-2.0 * log(u1)) * cos(2 * 3.141592653589793 * u2);

        noisy_signal[i] = signal[i] + noise_level * noise;
    }
}

void fir_filter(double input[], double output[], int length)
{

    // vòng lặp chạy từ đầu đến cuối mảng tín hiệu noisy_signal
    for (int i = 0; i < length; i++)
    {
        output[i] = 0;

        // vòng lặp chạy dọc chiều dài cửa sổ
        for (int n = 0; n < 51; n++)
        {
            double h_lp;

            if (n == 25)
            {
                h_lp = 0.2; // tại n = 25 thì đáp ứng xung lý tưởng sẽ có dạng 0/0, nhưng nhờ có biến đổi đại số nên phần lim 0/0 = 1, và phần còn lại là 0.2
            }
            else
            {
                h_lp = 0.2 * sin(0.2 * 3.141592653589793 * (n - 25)) / (0.2 * (n - 25));
            }

            // cửa sổ Hamming
            double w = 0.54 - 0.46 * cos((3.141592653589793 * n) / 25);

            h_lp *= w;

            if (i - n >= 0)
            {
                output[i] += h_lp * input[i - n];
            }
        }
    }
}

int main()
{

    int bits[N];
    double signal[N * L];       // mảng signal chứa các bit sau khi qua module BPSK có kích thước bằng tổng số mẫu lấy được của tín hiệu
    double noisy_signal[N * L]; // mảng noisy_signal chứa các bit thể hiện tín hiệu đã có nhiễu
    double after_FIR[N * L];    // mảng chứa các tín hiệu sau khi được lọc bởi FIR filter

    srand(time(NULL));

    generate_bits(bits);
    bpsk(bits, signal);
    add_awgn(signal, noisy_signal, N * L, 0.5);

    FILE *f = fopen("C:\\BTL\\noisy_signal.txt", "w");

    for (int i = 0; i < N * L; i++)
    {
        fprintf(f, "%f\n", noisy_signal[i]);
    }

    fclose(f);

    fir_filter(noisy_signal, after_FIR, N * L);

   

    // xuất dữ liệu để đưa vào Matlab
    FILE *f1 = fopen("C:\\BTL\\after_FIR.txt", "w");

    for (int i = 0; i < N * L; i++)
    {
        fprintf(f1, "%f\n", after_FIR[i]);
    }

    fclose(f1);

    return 0;
}